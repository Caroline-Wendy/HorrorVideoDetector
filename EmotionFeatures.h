/*! @file
********************************************************************************
<PRE>
模块名 : 图像特征
文件名 : EmotionFeatures.h 
相关文件 : EmotionFeatures.cpp
文件实现功能 : 图像情感特征类声明
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

#pragma once

#include "ImageFeatures.h"

//视频检测命名空间
namespace vd 
{

	/*! @class
	********************************************************************************
	<PRE>
	类名称 : EmotionFeatures
	功能 : 图像情感特征
	异常类 : 无
	--------------------------------------------------------------------------------
	备注 : 无
	典型用法 : EmotionFeatures iEF;
	--------------------------------------------------------------------------------
	作者 : C.L.Wang
	</PRE>
	*******************************************************************************/
	class EmotionFeatures : public ImageFeatures
	{

	public:

		EmotionFeatures (void); //构造函数

		explicit EmotionFeatures (const cv::Mat& _image); //参数构造函数

		~EmotionFeatures (void); //析构函数

	public:

		const bool extractFeatures (void) override; //提取特征

	private:

		static const std::size_t FEATURE_NUM = 64; //特征数量

		static const double MY_PI; //PI值(数学)

		/*标准色谱图*/

		static const double MAX_ACTIVITY; //活动性最大值
		static const double MIN_ACTIVITY; //活动性最小值
		static const double MAX_WEIGHT; //重要度最大值
		static const double MIN_WEIGHT; //重要度最小值
		static const double MAX_HEAT; //热度最大值
		static const double MIN_HEAT; //热度最小值

	private:

		inline void __processPixel (
			const cv::Scalar& _s, double& _activity, double& _weight, double& _heat) const; //处理像素

		inline const std::size_t __calculateIndex (
			const double _activity, const double _weight, const double _heat) const; //计算索引

	};

}