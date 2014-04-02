/*! @file
********************************************************************************
<PRE>
模块名 : 图像特征
文件名 : CombinationFeatures.cpp
相关文件 : CombinationFeatures.h
文件实现功能 : 图像组合特征类实现
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

#include "CombinationFeatures.h"

using namespace std;
using namespace cv;
using namespace vd;

/*! @function
********************************************************************************
<PRE>
函数名 : CombinationFeatures
功能 : 默认构造函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : CombinationFeatures iCF;
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
CombinationFeatures::CombinationFeatures (void) : 
	ImageFeatures()
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : CombinationFeatures
功能 : 参数构造函数
参数 : const cv::Mat& _image, 图像;
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : CombinationFeatures iCF(image);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
CombinationFeatures::CombinationFeatures (const cv::Mat& _image) :
	ImageFeatures(_image)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : ~CombinationFeatures
功能 : 析构函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iCF.~CombinationFeatures();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
CombinationFeatures::~CombinationFeatures (void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : extractFeatures
功能 : 提取特征
参数 : void
返回值 : const bool, 是否成功
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : isOK = extractFeatures();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool CombinationFeatures::extractFeatures (void)
{
	if (!m_isImage) {
		__printLog(std::cerr, "Please, load image first!");
		return false;
	}

	m_pFeatures.reset(new std::vector<double>); //重置为空!

	StatisticsFeatures cSTA (m_image); //统计类
	StochasticFeatures cSTO (m_image); //随机类
	EmotionFeatures cEF  (m_image); //情感类
	HarmonyFeatures cHF  (m_image); //和谐类

	std::shared_ptr<std::vector<double> > staFeatures; //统计特征
	std::shared_ptr<std::vector<double> > stoFeatures; //随机特征
	std::shared_ptr<std::vector<double> > efFeatures; //情感特征
	std::shared_ptr<std::vector<double> > hfFeatures; //和谐特征

	if (cSTA.extractFeatures()) {
		staFeatures = cSTA.getFeatures();
	} else {
		__printLog(std::cerr, "Failed to extract statistics features! ");
		return false;
	}

	if (cSTO.extractFeatures()) {
		stoFeatures = cSTO.getFeatures();
	} else {
		__printLog(std::cerr, "Failed to extract stochastic features! ");
		return false;
	}

	if (cEF.extractFeatures()) {
		efFeatures = cEF.getFeatures();
	} else {
		__printLog(std::cerr, "Failed to extract emotion features! ");
		return false;
	}

	if (cHF.extractFeatures()) {
		hfFeatures = cHF.getFeatures();
	} else {
		__printLog(std::cerr, "Failed to extract harmony features! ");
		return false;
	}

	/*组合特征*/

	m_pFeatures->insert(m_pFeatures->end(), staFeatures->begin(), staFeatures->end());
	m_pFeatures->insert(m_pFeatures->end(), stoFeatures->begin(), stoFeatures->end());
	m_pFeatures->insert(m_pFeatures->end(), efFeatures->begin(), efFeatures->end());
	m_pFeatures->insert(m_pFeatures->end(), hfFeatures->begin(), hfFeatures->end());

	m_isFeatures = true; //计算完成

	return true;
}