/*! @file
********************************************************************************
<PRE>
模块名 : 图像特征
文件名 : StatisticsFeatures.h 
相关文件 : StatisticsFeatures.cpp
文件实现功能 : 图像统计特征类声明
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
	类名称 : StatisticsFeatures
	功能 : 图像统计特征
	异常类 : 无
	--------------------------------------------------------------------------------
	备注 : 无
	典型用法 : StatisticsFeatures iSF;
	--------------------------------------------------------------------------------
	作者 : C.L.Wang
	</PRE>
	*******************************************************************************/
	class StatisticsFeatures : public ImageFeatures
	{

	public:

		StatisticsFeatures (void); //构造函数

		explicit StatisticsFeatures (const cv::Mat& _image); //参数构造函数

		~StatisticsFeatures (void); //析构函数

	public:

		const bool extractFeatures (void) override; //提取特征

	private:

		static const std::size_t FEATURE_NUM = 9; //特征数量

	private:

		void __calculateHSV (
			std::vector<double>& _hsvFeatures, /*HSV, 6维特征*/
			double& _lightnessKey /*亮度值*/
			); //计算HSV特征(6维), 及亮度值(V)

		void __calculateHLS (
			double& _lightnessMedian, /*亮度中值*/
			double& _shadowProportion /*阴影比例*/
			); //计算亮度中值和阴影比例

	};

}