/*! @file
********************************************************************************
<PRE>
模块名 : 图像特征
文件名 : StatisticsFeatures.cpp
相关文件 : StatisticsFeatures.h
文件实现功能 : 图像统计特征类实现
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

#include "StatisticsFeatures.h"

using namespace std;
using namespace cv;
using namespace vd;

/*! @function
********************************************************************************
<PRE>
函数名 : StatisticsFeatures
功能 : 默认构造函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : StatisticsFeatures iSF;
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
StatisticsFeatures::StatisticsFeatures(void) : 
ImageFeatures()
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : StatisticsFeatures
功能 : 参数构造函数
参数 : const cv::Mat& _image, 图像;
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : StatisticsFeatures iSF(image);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
StatisticsFeatures::StatisticsFeatures (const cv::Mat& _image) : 
ImageFeatures(_image)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : ~StatisticsFeatures
功能 : 析构函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iSF.~StatisticsFeatures();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
StatisticsFeatures::~StatisticsFeatures (void)
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
const bool StatisticsFeatures::extractFeatures (void)
{
	if (!m_isImage) {
		__printLog(std::cerr, "Please load image first!");
		return false;
	}

	m_pFeatures.reset(new std::vector<double>(FEATURE_NUM)); //重置特征

	const std::size_t HSV_NUM = 6; //HSV特征数
	std::vector<double> hsvFeatures(HSV_NUM); //HSV, 6维特征
	double lightnessKey (0.0); //亮度值

	__calculateHSV(hsvFeatures, lightnessKey);

	for (int i=0; i<6; ++i) {
		(*m_pFeatures)[i] = hsvFeatures[i];
	}
	(*m_pFeatures)[6] = lightnessKey;

	double lightnessMedian(0.0); //亮度中值
	double shadowProportion(0.0); //阴影比例

	__calculateHLS(lightnessMedian, shadowProportion);

	(*m_pFeatures)[7] = lightnessMedian;
	(*m_pFeatures)[8] = shadowProportion;

	m_isFeatures = true;

	return true;
}

/*! @function
********************************************************************************
<PRE>
函数名 : __calculateHSV
功能 : 计算HSV特征(6维), 及亮度值(V)
参数 : 
vector<double>& _hsvFeatures, HSV, 6维特征;
double& _lightnessKey, 亮度值;
返回值 : void
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : __calculateHSV(hsvFeatures, lightnessKey);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void StatisticsFeatures::__calculateHSV (
	std::vector<double>& _hsvFeatures,
	double& _lightnessKey
	)
{
	cv::Mat hsvImage; //HSV图像
	cv::cvtColor(m_image, hsvImage, CV_BGR2HSV);

	cv::Scalar mean, stddev;
	cv::meanStdDev(hsvImage, mean, stddev);

	_hsvFeatures[0] = mean[0];
	_hsvFeatures[1] = mean[1];
	_hsvFeatures[2] = mean[2];
	_hsvFeatures[3] = stddev[0];
	_hsvFeatures[4] = stddev[1];
	_hsvFeatures[5] = stddev[2];

	_lightnessKey = _hsvFeatures[2]*_hsvFeatures[5];

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : __calculateHLS
功能 : 计算亮度中值和阴影比例
参数 : 		
double& _lightnessMedian, 亮度中值;
double& _shadowProportion, 阴影比例;
返回值 : void
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : __calculateHLS(lightnessMedian, shadowProportion);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void StatisticsFeatures::__calculateHLS (
	double& _lightnessMedian, /*亮度中值*/
	double& _shadowProportion /*阴影比例*/
	)
{
	cv::Mat hlsImage; //HLS图像
	cv::cvtColor(m_image, hlsImage, CV_BGR2HLS);

	std::vector<double> ldata;
	long num(0);
	cv::Scalar s;

	for(int i=0; i<hlsImage.rows; ++i)
	{
		for(int j=0; j<hlsImage.cols; ++j)
		{
			s.val[1] = hlsImage.at<cv::Vec3b>(i,j)[1];
			ldata.push_back(s.val[1]);
			if (s.val[1]<46)
				num++;
		}
	}

	const long medianPos = static_cast<long>(ldata.size()/2);
	std::nth_element(ldata.begin(), ldata.begin()+medianPos, ldata.end()); //排序

	_lightnessMedian = ldata[medianPos]; //计算亮度均值

	const double tmp = hlsImage.size().width*hlsImage.size().height;
	_shadowProportion = num/tmp; //计算阴影比例

	hlsImage.release();

	return;
}

