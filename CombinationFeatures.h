/*! @file
********************************************************************************
<PRE>
模块名 : 图像特征
文件名 : CombinationFeatures.h 
相关文件 : CombinationFeatures.cpp
文件实现功能 : 图像组合特征类声明
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

#pragma once

#include "ImageFeatures.h"
#include "EmotionFeatures.h"
#include "HarmonyFeatures.h"
#include "StatisticsFeatures.h"
#include "StochasticFeatures.h"

//视频检测命名空间
namespace vd 
{

	/*! @class
	********************************************************************************
	<PRE>
	类名称 : CombinationFeatures
	功能 : 图像组合特征
	异常类 : 无
	--------------------------------------------------------------------------------
	备注 : 无
	典型用法 : CombinationFeatures iCF;
	--------------------------------------------------------------------------------
	作者 : C.L.Wang
	</PRE>
	*******************************************************************************/
	class CombinationFeatures : public ImageFeatures
	{

	public:

		CombinationFeatures (void); //默认构造函数

		explicit CombinationFeatures (const cv::Mat& _image); //使用图像的构造函数

		~CombinationFeatures (void); //析构函数

	public:

		const bool extractFeatures (void) override; //提取特征

	};

}