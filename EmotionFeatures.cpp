/*! @file
********************************************************************************
<PRE>
模块名 : 图像特征
文件名 : EmotionFeatures.cpp
相关文件 : EmotionFeatures.h
文件实现功能 : 图像情感特征类实现
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

#include "EmotionFeatures.h"

using namespace std;
using namespace cv;
using namespace vd;


/*PI的值*/

const double EmotionFeatures::MY_PI = 3.1415926536;

/*标准色谱图*/

const double EmotionFeatures::MAX_ACTIVITY = 16.1235*0.5; //活动性最大值
const double EmotionFeatures::MIN_ACTIVITY = 6.77574*1.5; //活动性最小值
const double EmotionFeatures::MAX_WEIGHT = 2.45524*0.5; //重要度最大值
const double EmotionFeatures::MIN_WEIGHT = -7.74189*0.5; //重要度最小值
const double EmotionFeatures::MAX_HEAT = 5.41763*0.5; //热度最大值
const double EmotionFeatures::MIN_HEAT = 0.426733*1.5; //热度最小值

/*! @function
********************************************************************************
<PRE>
函数名 : EmotionFeatures
功能 : 默认构造函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : EmotionFeatures iEF;
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
EmotionFeatures::EmotionFeatures (void) : 
	ImageFeatures()
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : EmotionFeatures
功能 : 参数构造函数
参数 : const cv::Mat& _image, 图像;
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : EmotionFeatures iEF(image);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
EmotionFeatures::EmotionFeatures (const cv::Mat& _image) :
	ImageFeatures(_image)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : ~EmotionFeatures
功能 : 析构函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iEF.~EmotionFeatures();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
EmotionFeatures::~EmotionFeatures (void)
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
const bool EmotionFeatures::extractFeatures (void)
{
	if (!m_isImage) { //判断是否包含图像
		__printLog(std::cerr, "Please load image first!");
		return false;
	}

	m_pFeatures.reset(new std::vector<double>(FEATURE_NUM)); //重置特征

	cv::Scalar s; //存储像素点

	double activity(0.0); //活动性
	double weight(0.0); //重要度
	double heat(0.0); //热度

	cv::Mat labImage; //lab图像
	cv::cvtColor(m_image, labImage, CV_BGR2Lab);

	for (int i=0; i<labImage.rows; ++i)
	{
		for (int j=0; j<labImage.cols; ++j)
		{
			s.val[0] = labImage.at<cv::Vec3b>(i,j)[0];
			s.val[1] = labImage.at<cv::Vec3b>(i,j)[1];
			s.val[2] = labImage.at<cv::Vec3b>(i,j)[2];

			__processPixel(s, activity, weight, heat); //处理像素

			const std::size_t indexEmotion = 
				__calculateIndex(activity, weight, heat); //计算索引

			++(*m_pFeatures)[indexEmotion];
		}
	}

	const double product = labImage.rows*labImage.cols;
	for (int i=0; i<FEATURE_NUM ; ++i) {
		(*m_pFeatures)[i] /= product;
	}

	labImage.release();

	m_isFeatures = true; //计算特征完成

	return true;
}

/*! @function
********************************************************************************
<PRE>
函数名 : __processPixel
功能 : 处理像素点
参数 : 
const cv::Scalar& _s, 像素点;
double& _activity, 活动性;
double& _weight, 重要度;
double& _heat, 热度;
返回值 : void
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : __processPixel(s, activity, weight, heat);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void EmotionFeatures::__processPixel (
	const cv::Scalar& _s, double& _activity, double& _weight, double& _heat) const
{
	const double chroma = sqrt(pow(_s.val[1],2)+pow(_s.val[2],2)); //色度
	const double hue = atan(static_cast<double>(_s.val[2])/static_cast<double>(_s.val[1])); //灰度

	_activity = -2.1+0.06*sqrt(pow(_s.val[0]-50,2)+pow(_s.val[1]-3,2)+pow(_s.val[2]-17,2));
	_weight = -1.8+0.04*(100-_s.val[0])+0.45*cos(hue-100*MY_PI/180);
	_heat = -0.5+0.02*pow(chroma,1.07)*cos(hue-100*MY_PI/180);

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : __calculateIndex
功能 : 计算索引值
参数 : 
const double _activity, 活动性; 
const double _weight, 重要度; 
const double _heat, 热度;
返回值 : const size_t 索引值, 表示特征直方图的bin;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : index = __calculateIndex(activity, weight, heat);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const std::size_t EmotionFeatures::__calculateIndex (
	const double _activity, const double _weight, const double _heat) const
{
	const double normActivity = (_activity-MIN_ACTIVITY)/(MAX_ACTIVITY-MIN_ACTIVITY);
	const double normWeight = (_weight-MIN_WEIGHT)/(MAX_WEIGHT-MIN_WEIGHT);
	const double normHeat = (_heat-MIN_HEAT)/(MAX_HEAT-MIN_HEAT);

	double indexActivity = normActivity*48.0;
	if ( indexActivity < 0 ) {
		indexActivity = 0.0;
	} else if ( indexActivity > 48.0 ) {
		indexActivity = 48.0;
	}

	double indexWeight = normWeight*12.0;
	if ( indexWeight < 0 ) {
		indexWeight = 0.0;
	} else if ( indexWeight > 12.0 ){
		indexWeight = 12.0;
	}

	double indexHeat = normHeat*3.0;
	if ( indexHeat < 0 ) {
		indexHeat = 0.0;
	} else if ( indexHeat >3.0) {
		indexHeat = 3.0;
	}

	const std::size_t index = static_cast<std::size_t>(indexHeat + indexWeight + indexActivity);

	return index;
}
