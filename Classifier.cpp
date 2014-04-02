/*! @file
********************************************************************************
<PRE>
模块名 : 分类器
文件名 : Classifier.cpp
相关文件 : Classifier.h
文件实现功能 : 分类器类实现
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
日期              版本   修改人         修改内容 
2014/03/27   1.0    C.L.Wang        Create
</PRE>
********************************************************************************

* 版权所有(c) C.L.Wang, 保留所有权利

*******************************************************************************/ 

#include "stdafx.h"

#include "Classifier.h"

using namespace std;
using namespace cv;
using namespace vd;

/*! @function
********************************************************************************
<PRE>
函数名 : Classifier
功能 : 参数构造函数
参数 : 
const Mat& _videoFeature, 视频特征; 
const string& _modelPath, 模型路径;
返回值 : 无
抛出异常 : exception, 参数错误
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : Classifier iCF(_videoFeature, _modelPath);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
Classifier::Classifier (
	const cv::Mat& _videoFeature, /*特征*/
	const std::string& _modelPath /*模型路径*/
) :
	m_videoFeature(_videoFeature), /*特征*/
	m_modelPath(_modelPath) /*模型路径*/
{
	if (m_videoFeature.empty()) {
		__printLog (std::cerr, "Sorry, video feature is empty! ");
		throw std::exception("Video Feature Error");
	}

	if (_access(m_modelPath.c_str(), 0)) {
		__printLog (std::cerr, "Sorry, the model path doesn't exist! ");
		throw std::exception("Model Path Error");
	}

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : ~Classifier
功能 : 析构函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iCF.~Classifier();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
Classifier::~Classifier (void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : setVideoFeature
功能 : 设置视频特征
参数 : 
const Mat& _videoFeature, 视频特征
返回值 : void
抛出异常 : exception, 参数错误
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iCF.setVideoFeature(_videoFeature);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void Classifier::setVideoFeature (const cv::Mat& _videoFeature)
{
	m_videoFeature = _videoFeature;

	if (m_videoFeature.empty()) {
		__printLog (std::cerr, "Sorry, video feature is empty! ");
		throw std::exception("Video Feature Error");
	}

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : setModelPath
功能 : 设置模型路径
参数 : 
const string& _modelPath, 模型路径
返回值 : void
抛出异常 : 参数错误
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iCF.setModelPath(_modelPath);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void Classifier::setModelPath (const std::string& _modelPath)
{
	m_modelPath = _modelPath;

	if (_access(m_modelPath.c_str(), 0)) {
		__printLog (std::cerr, "Sorry, the model path doesn't exist! ");
		throw std::exception("Model Path Error");
	}

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : __printLog
功能 : 输出信息
参数 : 
ostream& _os, 输出格式; 
const std::string& _msg, 输出内容;
返回值 : std::ostream&;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : __printLog(cout, "Nothing");
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
std::ostream& Classifier::__printLog (
	std::ostream& _os, const std::string& _msg) const
{
	if (std::cerr == _os) {
		_os << "Warning : " << _msg << std::endl;
	} else if (std::clog == _os) {
		_os << "Log : " << _msg << std::endl;
	} else {
		_os << "Message : " << _msg << std::endl;
	}

	return _os;
}