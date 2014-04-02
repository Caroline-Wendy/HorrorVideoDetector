/*! @file
********************************************************************************
<PRE>
模块名 : 图像特征
文件名 : HarmonyFeatures.h 
相关文件 : HarmonyFeatures.cpp
文件实现功能 : 图像和谐度特征类声明
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

//视频检测命名空间
namespace vd 
{

	/*! @class
	********************************************************************************
	<PRE>
	类名称 : HarmonyFeatures
	功能 : 图像和谐度特征
	异常类 : 无
	--------------------------------------------------------------------------------
	备注 : 无
	典型用法 : HarmonyFeatures iHF;
	--------------------------------------------------------------------------------
	作者 : C.L.Wang
	</PRE>
	*******************************************************************************/
	class HarmonyFeatures : public ImageFeatures
	{

	public:

		HarmonyFeatures (void); //构造函数

		explicit HarmonyFeatures (const cv::Mat& _image); //参数构造函数

		~HarmonyFeatures (void); //析构函数

	public:

		const bool extractFeatures (void) override; //提取特征

	private:

		static const std::size_t FEATURE_NUM = 26; //特征数量

		static const double MY_PI; //PI值(数学)
		static const double MY_E; //E值(数学)

		static const double MAX_HARMONY; //最大和谐度
		static const double MIN_HARMONY; //最小和谐度

	private:

		inline const double __calculateHarmonyScore (
			const cv::Scalar& _s1, const double _L2, const double _A, const double _B); //计算索引

		inline const std::size_t __calculateIndex (const double _harmonyScore); //计算索引

	};

}