/*! @file
********************************************************************************
<PRE>
模块名 : 图像特征
文件名 : HarmonyFeatures.cpp
相关文件 : HarmonyFeatures.h
文件实现功能 : 图像和谐度特征类实现
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
2014/03/10   1.0    C.L.Wang        创建
</PRE>
********************************************************************************

* 版权所有(c) C.L.Wang, 保留所有权利

*******************************************************************************/ 

#include "stdafx.h"

#include "HarmonyFeatures.h"

using namespace std;
using namespace cv;
using namespace vd;

const double HarmonyFeatures::MY_PI = 3.1415926536; //PI值(数学)
const double HarmonyFeatures::MY_E = 2.718281828459046; //E值(数学)

/*标准和谐度*/

const double HarmonyFeatures::MAX_HARMONY = 0.697041*0.9;
const double HarmonyFeatures::MIN_HARMONY = -0.785739*0.9;

/*! @function
********************************************************************************
<PRE>
函数名 : HarmonyFeatures
功能 : 默认构造函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : HarmonyFeatures iHF;
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
HarmonyFeatures::HarmonyFeatures (void) : 
ImageFeatures()
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : HarmonyFeatures
功能 : 参数构造函数
参数 : const cv::Mat& _image, 图像;
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : HarmonyFeatures iHF(image);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
HarmonyFeatures::HarmonyFeatures (const cv::Mat& _image) : 
ImageFeatures(_image)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : ~HarmonyFeatures
功能 : 析构函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iHF.~HarmonyFeatures();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
HarmonyFeatures::~HarmonyFeatures (void)
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
典型用法 : isOK = iHF.extractFeatures();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool HarmonyFeatures::extractFeatures (void)
{
	if (!m_isImage) {
		__printLog(std::cerr, "Please, load image first!");
		return false;
	}

	m_pFeatures.reset(new std::vector<double>(FEATURE_NUM)); //重置特征

	cv::Mat labImage; //lab图像
	cv::cvtColor(m_image, labImage, CV_BGR2Lab);

	for (int i=1; i<labImage.rows-1; ++i)
	{
		for (int j=1 ; j<labImage.cols-1; ++j)
		{
			cv::Scalar s1;

			s1.val[0] = labImage.at<cv::Vec3b>(i,j)[0];
			s1.val[1] = labImage.at<cv::Vec3b>(i,j)[1];
			s1.val[2] = labImage.at<cv::Vec3b>(i,j)[2];

			double xL2(0.0), A(0.0), B(0.0);

			for (int m=i-1; m<i+2; ++m)
			{
				for (int n=j-1; n<j+2; ++n)
				{
					cv::Scalar s2;

					s2.val[0] = labImage.at<cv::Vec3b>(m,n)[0];
					s2.val[1] = labImage.at<cv::Vec3b>(m,n)[1];
					s2.val[2] = labImage.at<cv::Vec3b>(m,n)[2];

					xL2 += s2.val[0];
					A += s2.val[1];
					B += s2.val[2];
				}
			}

			const double harmonyScore = 
				__calculateHarmonyScore (s1, xL2, A, B); //计算和谐度得分

			const std::size_t indexHarmony = 
				__calculateIndex (harmonyScore); //计算和谐度索引

			++(*m_pFeatures)[indexHarmony];
		}
	}

	const double product = (labImage.rows-2)*(labImage.cols-2);

	for (int i=0; i<FEATURE_NUM; ++i) {
		(*m_pFeatures)[i] /= product;
	}

	m_isFeatures = true; //计算特征完成

	return true;
}

/*! @function
********************************************************************************
<PRE>
函数名 : __calculateHarmonyScore
功能 : 计算和谐度分数
参数 : 
const cv::Scalar& s1, 像素点;
const double L2, L域; 
double A, A域;
double B, B域;
返回值 : const double, 和谐度分数;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : harmonyScore = __calculateHarmonyScore (s1, L2, A, B);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const double HarmonyFeatures::__calculateHarmonyScore (
	const cv::Scalar& _s1, const double _L2, const double _A, const double _B)
{
	double harmonyScore (0.0); //和谐度得分

	double HC(0.0); //饱和度因子
	double HL(0.0); //亮度因子
	double HH(0.0); //色调因子

	double L2 = (_L2-_s1.val[0])/8;
	double A = (_A-_s1.val[1])/8;
	double B = (_B-_s1.val[2])/8;

	double L1,C1,H1,C2,H2;

	L1 = _s1.val[0];
	C1 = sqrt(pow(_s1.val[1],2)+pow(_s1.val[2],2));
	H1 = atan(_s1.val[2]/_s1.val[1]);
	C2 = sqrt(pow(A,2)+pow(B,2));
	H2 = atan(B/A);

	/*计算每个像素的三个分量*/

	double temp1,temp2,temp3,temp4;
	double temp5,temp6,temp7;

	temp1 = sqrt(pow(H1-H2,2)+pow((C1-C2)/1.46,2));
	HC = 0.04+0.53*tanh(0.8-0.045*temp1);

	temp1 = 0.28+0.54*tanh(-3.88+0.029*(L1+L2));
	HL = temp1+0.14+0.15*tanh(-2+0.2*abs(L1-L2));

	temp2 = 0.5+0.5*tanh(-2+0.5*C1);
	temp3 = -0.08-0.14*sin(H1+50*MY_PI/180)-0.07*(2*H1+MY_PI/2);
	temp4 = ((0.22*L1-12.8)/10) * 
		pow(MY_E,(90-H1*180/(MY_PI*10)-pow(MY_E,90-H1*180/(MY_PI*10))));
	temp5 = 0.5+0.5*tanh(-2+0.5*C2);
	temp6 = -0.08-0.14*sin(H2+50*MY_PI/180)-0.07*(2*H2+MY_PI/2);
	temp7 = ((0.22*L2-12.8)/10)*
		pow(MY_E,(90-H2*180/(MY_PI*10)-pow(MY_E,90-H2*180/(MY_PI*10))));

	HH = temp2*(temp3+temp4)+temp5*(temp6+temp7);

	harmonyScore = HC + HL + HH; //计算和谐度值

	return harmonyScore;
}

/*! @function
********************************************************************************
<PRE>
函数名 : __calculateIndex
功能 : 计算索引值
参数 : double _harmonyScore, 和谐度分数;
返回值 : const std::size_t 索引值, 表示特征直方图的bin;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : index = __calculateIndex (harmonyScore);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const std::size_t HarmonyFeatures::__calculateIndex (const double _harmonyScore)
{
	const double normHarmony = 
		(_harmonyScore-MIN_HARMONY)/
		(MAX_HARMONY-MIN_HARMONY); //归一化

	int indexHarmony = static_cast<int>(normHarmony*25.0);

	if ( indexHarmony < 0 ) {
		indexHarmony = 0;
	} else if ( indexHarmony > 25) {
		indexHarmony = 25;
	}

	return indexHarmony;
}
