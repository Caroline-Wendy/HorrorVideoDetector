/*! @file
********************************************************************************
<PRE>
模块名 : 分类器
文件名 : Classifier.h 
相关文件 : Classifier.cpp
文件实现功能 : 分类器类声明
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
2014/03/27	   1.0    C.L.Wang        Create
</PRE>
********************************************************************************

* 版权所有(c) C.L.Wang, 保留所有权利

*******************************************************************************/ 

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <io.h>

#include <opencv.hpp>

namespace vd
{
	/*! @class
	********************************************************************************
	<PRE>
	类名称 : Classifier
	功能 : 分类器
	异常类 : 无
	--------------------------------------------------------------------------------
	备注 : 无
	典型用法 : Classifier iCF(_videoFeature, _modelPath);
	--------------------------------------------------------------------------------
	作者 : C.L.Wang
	</PRE>
	*******************************************************************************/
	class Classifier 
	{

	public:

		Classifier(
			const cv::Mat& _videoFeature, /*特征*/
			const std::string& _modelPath /*模型路径*/
		);

		virtual ~Classifier(void);

	public:

		void setVideoFeature (const cv::Mat& _videoFeature); //视频特征

		void setModelPath (const std::string& _modelPath); //模型路径

	public:
		
		virtual const double calculateResult (void) = 0; //预测结果

	protected:

		cv::Mat m_videoFeature; //视频特征

		std::string m_modelPath; //模型路径

	protected:

		virtual const double _predictValue (void) const = 0; //预测结果

		virtual void _initModel (void) = 0; //初始化模型

		inline std::ostream& __printLog (
			std::ostream& _os, const std::string& _msg) const; //打印信息

	};
}