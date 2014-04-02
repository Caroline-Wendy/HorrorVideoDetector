/*! @file
********************************************************************************
<PRE>
ģ���� : ͼ������
�ļ��� : StochasticFeatures.cpp
����ļ� : StochasticFeatures.h
�ļ�ʵ�ֹ��� : ͼ�����������ʵ��
���� : C.L.Wang
Email: morndragon@126.com
�汾 : 1.0
--------------------------------------------------------------------------------
���̰߳�ȫ�� : ��
�쳣ʱ��ȫ�� : ��
--------------------------------------------------------------------------------
��ע : ��
--------------------------------------------------------------------------------
�޸ļ�¼ :  
�� ��              �汾   �޸���         �޸����� 
2014/03/27   1.0    C.L.Wang        ����
</PRE>
********************************************************************************

* ��Ȩ����(c) C.L.Wang, ��������Ȩ��

*******************************************************************************/ 

#include "stdafx.h"

#include "StochasticFeatures.h"

using namespace std;
using namespace cv;
using namespace vd;

const int StochasticFeatures::MY_SIGMA = 3; //������ֵ
const double StochasticFeatures::MY_PI = 3.1415926536; //PIֵ(��ѧ)
const double StochasticFeatures::MY_E = 2.718281828459046; //Eֵ(��ѧ)

/*! @function
********************************************************************************
<PRE>
������ : StochasticFeatures (void)
���� : Ĭ�Ϲ��캯��
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : StochasticFeatures iSF;
--------------------------------------------------------------------------------
���� : C.L.Wang
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
������ : StochasticFeatures
���� : �������캯��
���� : const cv::Mat& _image, ͼ��;
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : StochasticFeatures iSF(image);
--------------------------------------------------------------------------------
���� : C.L.Wang
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
������ : ~StochasticFeatures
���� : ��������
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iSF.~StochasticFeatures();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
StochasticFeatures::~StochasticFeatures (void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : extractFeatures
���� : ��ȡ����
���� : void
����ֵ : const bool, �Ƿ�ɹ�;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : isOK = extractFeatures();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool StochasticFeatures::extractFeatures (void)
{
	if (!m_isImage) {
		__printLog(std::cerr, "Please, load image first!");
		return false;
	}

	m_pFeatures.reset(new std::vector<double>(FEATURE_NUM)); //��������

	double scale (0.0); //�߶�����
	double shape (0.0); //��״����

	__weibullMle(scale, shape);

	(*m_pFeatures)[0] = scale;
	(*m_pFeatures)[1] = shape;

	m_isFeatures = true;

	return true;
}

/*! @function
********************************************************************************
<PRE>
������ : ___newton
���� : ţ�ٺ���
���� : 
const double _g, ����;
const std::vector<double>& _x ����;
����ֵ : const double, ţ�ٺ������ֵ;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : value = ___newton(shape,imgData);
--------------------------------------------------------------------------------
���� : C.L.Wang
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
	
	double out(0.0); //����ֵ

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
������ : ___gaussianFilter
���� : ��˹�˲�
���� : cv::Mat& _gauss_mat, ��˹�˲�����
����ֵ : void
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : ___gaussianFilter(gaussMat);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
void StochasticFeatures::___gaussianFilter (cv::Mat& _gauss_mat)
{
	cv::Mat mat = cv::Mat::Mat(m_image.size(), CV_32FC1); //ͼ��
	
	cv::Mat grayMat; //�Ҷ�ͼ��
	cv::cvtColor(m_image, grayMat, CV_BGR2GRAY);
	grayMat.convertTo(mat, mat.type()); //ת��Ϊfloat����

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
������ : __weibullMle
���� : Τ���ֲ�
���� : 
double& _scale, �߶�����; 
double& _shape, ��״����;
����ֵ : void
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : __weibullMle(scale, shape);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
void StochasticFeatures::__weibullMle (
	double& _scale, /*�߶�����*/
	double& _shape /*��״����*/
)
{
	cv::Mat grayImage;//�Ҷ�ͼ��
	cv::cvtColor(m_image, grayImage, CV_BGR2GRAY);

	cv::Mat gaussMat = cv::Mat::Mat(m_image.size(), CV_32FC1); //��������
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

	_shape = shape_next; //��״ֵ

	for (std::size_t i=0; i<imgData.size(); ++i){
		sum += std::pow(imgData[i], _shape)/n;
	}
	_scale = std::pow(sum, 1.0/_shape); //�߶�ֵ

	return;
}