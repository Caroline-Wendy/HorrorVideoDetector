/*! @file
********************************************************************************
<PRE>
模块名 : 图像特征
文件名 : StochasticFeatures.h 
相关文件 : StochasticFeatures.cpp
文件实现功能 : 图像随机特征类声明
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
	类名称 : StochasticFeatures
	功能 : 图像随机特征
	异常类 : 无
	--------------------------------------------------------------------------------
	备注 : 无
	典型用法 : StochasticFeatures iSF;
	--------------------------------------------------------------------------------
	作者 : C.L.Wang
	</PRE>
	*******************************************************************************/
	class StochasticFeatures : public ImageFeatures
	{

	public:

		StochasticFeatures (void); //构造函数

		explicit StochasticFeatures (const cv::Mat& _image); //参数构造函数

		~StochasticFeatures (void); //析构函数

	public:

		const bool extractFeatures (void) override; //提取特征

	private:

		static const int MY_SIGMA; //西格玛值
		static const double MY_PI; //PI值(数学)
		static const double MY_E; //E值(数学)

		static const std::size_t FEATURE_NUM = 2; //特征数量

	private:

		void __weibullMle (
			double& _scale, /*尺度特征*/
			double& _shape /*型状特征*/
			); //韦博分布

		const double ___newton (
			const double _g, 
			const std::vector<double>& _x
			); //牛顿函数

		void ___gaussianFilter (cv::Mat& _gauss_mat); //高斯滤波

	};

}