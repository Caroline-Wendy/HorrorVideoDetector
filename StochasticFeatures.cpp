/*! @file
********************************************************************************
<PRE>
模块名 : 图像特征
文件名 : StochasticFeatures.cpp
相关文件 : StochasticFeatures.h
文件实现功能 : 图像随机特征类实现
作者 : C.L.Wang
Email: morndragon@126.com
版本 : 1.0
--------------------------------------------------------------------------------
多线程安全性 : 是
异常时安全性 : 是
--------------------------------------------------------------------------------
备注 : 无
--------------------------------------------------------------------------------
修改记录 :  
日 期              版本   修改人         修改内容 
2014/03/27   1.0    C.L.Wang        创建
</PRE>
********************************************************************************

* 版权所有(c) C.L.Wang, 保留所有权利

*******************************************************************************/ 

#include "stdafx.h"

#include "StochasticFeatures.h"

using namespace std;
using namespace cv;
using namespace vd;

const int StochasticFeatures::MY_SIGMA = 3; //西格玛值
const double StochasticFeatures::MY_PI = 3.1415926536; //PI值(数学)
const double StochasticFeatures::MY_E = 2.718281828459046; //E值(数学)

/*! @function
********************************************************************************
<PRE>
函数名 : StochasticFeatures (void)
功能 : 默认构造函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : StochasticFeatures iSF;
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
StochasticFeatures::StochasticFeatures (void) :
	ImageFeatures()
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : StochasticFeatures
功能 : 参数构造函数
参数 : const cv::Mat& _image, 图像;
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : StochasticFeatures iSF(image);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
StochasticFeatures::StochasticFeatures (const cv::Mat& _image) :
	ImageFeatures(_image)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : ~StochasticFeatures
功能 : 析构函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iSF.~StochasticFeatures();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
StochasticFeatures::~StochasticFeatures (void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : extractFeatures
功能 : 提取特征
参数 : void
返回值 : const bool, 是否成功;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : isOK = extractFeatures();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool StochasticFeatures::extractFeatures (void)
{
	if (!m_isImage) {
		__printLog(std::cerr, "Please, load image first!");
		return false;
	}

	m_pFeatures.reset(new std::vector<double>(FEATURE_NUM)); //重置特征

	double scale (0.0); //尺度特征
	double shape (0.0); //型状特征

	__weibullMle(scale, shape);

	(*m_pFeatures)[0] = scale;
	(*m_pFeatures)[1] = shape;

	m_isFeatures = true;

	return true;
}

/*! @function
********************************************************************************
<PRE>
函数名 : ___newton
功能 : 牛顿函数
参数 : 
const double _g, 参数;
const std::vector<double>& _x 参数;
返回值 : const double, 牛顿函数输出值;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : value = ___newton(shape,imgData);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const double StochasticFeatures::___newton (
	const double _g, 
	const std::vector<double>& _x
)
{
	std::vector <double> x_g;
	std::vector <double> x_i;
	std::vector <double> ln_x_i;
	std::vector <double> lambda;
	
	double out(0.0); //返回值

	double f(0.0), fPrime(0.0);
	double sum(0.0), sum1(0.0), sum2(0.0), sum3(0.0);

	std::size_t n = _x.size();

	for (std::size_t i=0; i<_x.size(); ++i) {
		x_g.push_back(pow(_x[i], _g));
		sum += x_g[i];
	}

	for (std::size_t i=0; i<n; ++i) {
		x_i.push_back(x_g[i]/sum);
		ln_x_i.push_back(log(n*x_i[i]));
		sum1 = sum1 + x_g[i]*(log(abs(_x[i])));
	}

	for (std::size_t i=0; i<n; ++i) {
		double val = x_g[i]*(log(abs(_x[i]))*sum -sum1)/(pow(sum,2));
		lambda.push_back(val);
	}

	for (std::size_t i=0; i<n; ++i) {
		double  value1 = ln_x_i[i] - n*x_i[i]*ln_x_i[i];
		f += value1;
		double value2 = lambda[i]*(sum/x_g[i]-n*ln_x_i[i]-n);
		fPrime += value2;
	}
	f += n;
	out = f/fPrime;

	return out;
}

/*! @function
********************************************************************************
<PRE>
函数名 : ___gaussianFilter
功能 : 高斯滤波
参数 : cv::Mat& _gauss_mat, 高斯滤波矩阵
返回值 : void
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : ___gaussianFilter(gaussMat);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void StochasticFeatures::___gaussianFilter (cv::Mat& _gauss_mat)
{
	cv::Mat mat = cv::Mat::Mat(m_image.size(), CV_32FC1); //图像
	
	cv::Mat grayMat; //灰度图像
	cv::cvtColor(m_image, grayMat, CV_BGR2GRAY);
	grayMat.convertTo(mat, mat.type()); //转换为float类型

	double x[8*MY_SIGMA+1];
	for (int i=0; i<mat.rows; ++i) {
		for (int j=0; j<mat.cols; ++j) {
			mat.at<float>(i,j) = static_cast<float>(mat.at<float>(i,j)/255.0);
		}
	}

	cv::Mat GG = cv::Mat::zeros(1,8*MY_SIGMA+1,CV_32FC1);
	cv::Mat dGG = cv::Mat::zeros(1,8*MY_SIGMA+1,CV_32FC1);

	for (int i=0,t=(-4*MY_SIGMA); i<=8*MY_SIGMA; ++i) {
		x[i] = t++;
	}

	for (int i = 0; i<=8*MY_SIGMA; ++i) {
		double value1 = pow(MY_E,-pow(x[i],2.0)/(2*pow(MY_SIGMA,2.0)))/
			(MY_SIGMA*sqrt(2*MY_PI));
		GG.at<float>(0,i) = static_cast<float>(value1);
		double value2 = -(x[i]*pow(MY_E,-pow(x[i],2.0)/(2*pow(MY_SIGMA,2.0))))/
			(pow(MY_SIGMA,3.0)*sqrt(2*MY_PI));
		dGG.at<float>(0,i) = static_cast<float>(value2);
	}

	cv::Mat GGT = cv::Mat::Mat(MY_SIGMA*8+1, 1, CV_32FC1);
	cv::Mat dGGT = cv::Mat::Mat(MY_SIGMA*8+1, 1, CV_32FC1);
	cv::transpose(GG, GGT);
	cv::transpose(dGG, dGGT);

	cv::Mat gaussDerivativeX1 = mat.clone();
	cv::Mat gaussDerivativeX2 = mat.clone();
	cv::Mat gaussDerivativeY2 = mat.clone();
	cv::Mat gaussDerivativeY1 = mat.clone();

	cv::filter2D(mat,gaussDerivativeX1,-1,dGG,cv::Point(-1,-1));
	double a = gaussDerivativeX1.at<float>(0,0);
	cv::filter2D(gaussDerivativeX1, gaussDerivativeX2, -1, GGT, cv::Point(-1,-1));
	cv::filter2D(mat,gaussDerivativeY1, -1, dGGT, cv::Point(-1,-1));
	cv::filter2D(gaussDerivativeY1, gaussDerivativeY2, -1, GG, cv::Point(-1,-1));

	for (int i=0; i<gaussDerivativeX2.rows; ++i) 
	{
		for (int j=0; j<gaussDerivativeX2.cols; ++j) 
		{
			double value1 = gaussDerivativeX2.at<float>(i,j);
			double value2 = gaussDerivativeY2.at<float>(i,j);
			double value3 = sqrt(pow(value1,2)+pow(value2,2));
			_gauss_mat.at<float>(i,j) = static_cast<float>(value3);
		}
	}

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : __weibullMle
功能 : 韦博分布
参数 : 
double& _scale, 尺度特征; 
double& _shape, 型状特征;
返回值 : void
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : __weibullMle(scale, shape);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void StochasticFeatures::__weibullMle (
	double& _scale, /*尺度特征*/
	double& _shape /*型状特征*/
)
{
	cv::Mat grayImage;//灰度图像
	cv::cvtColor(m_image, grayImage, CV_BGR2GRAY);

	cv::Mat gaussMat = cv::Mat::Mat(m_image.size(), CV_32FC1); //浮点类型
	___gaussianFilter(gaussMat);

	std::vector<double> imgData;

	for(int i=0; i<gaussMat.rows; ++i){
		for(int j=0; j<gaussMat.cols; ++j){
			imgData.push_back(gaussMat.at<float>(i,j)+0.00001);
		}
	}

	double eps(0.01);
	_shape = 0.1;
	double shape_next = _shape - ___newton(_shape, imgData);
	int nIteration(1);

	while (std::abs(shape_next-_shape)>eps)
	{
		if (shape_next>20 || nIteration>25){
			break;
		}
		else if (shape_next <= 0){
			shape_next = 0.000001;
			break;
		}
		_shape = shape_next;
		shape_next = _shape - ___newton(_shape, imgData);
		nIteration++;
	}
	double sum(0.0);
	double n (imgData.size());

	_shape = shape_next; //型状值

	for (std::size_t i=0; i<imgData.size(); ++i){
		sum += std::pow(imgData[i], _shape)/n;
	}
	_scale = std::pow(sum, 1.0/_shape); //尺度值

	return;
}