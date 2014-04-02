/*! @file
********************************************************************************
<PRE>
ģ���� : ������
�ļ��� : Classifier.cpp
����ļ� : Classifier.h
�ļ�ʵ�ֹ��� : ��������ʵ��
���� : C.L.Wang
Email: morndragon@126.com
�汾 : 1.0
--------------------------------------------------------------------------------
���̰߳�ȫ�� : ��
�쳣ʱ��ȫ�� : ��
--------------------------------------------------------------------------------
��ע : ��
--------------------------------------------------------------------------------
�޸ļ�¼ :  
����              �汾   �޸���         �޸����� 
2014/03/27   1.0    C.L.Wang        Create
</PRE>
********************************************************************************

* ��Ȩ����(c) C.L.Wang, ��������Ȩ��

*******************************************************************************/ 

#include "stdafx.h"

#include "Classifier.h"

using namespace std;
using namespace cv;
using namespace vd;

/*! @function
********************************************************************************
<PRE>
������ : Classifier
���� : �������캯��
���� : 
const Mat& _videoFeature, ��Ƶ����; 
const string& _modelPath, ģ��·��;
����ֵ : ��
�׳��쳣 : exception, ��������
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : Classifier iCF(_videoFeature, _modelPath);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
Classifier::Classifier (
	const cv::Mat& _videoFeature, /*����*/
	const std::string& _modelPath /*ģ��·��*/
) :
	m_videoFeature(_videoFeature), /*����*/
	m_modelPath(_modelPath) /*ģ��·��*/
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
������ : ~Classifier
���� : ��������
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iCF.~Classifier();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
Classifier::~Classifier (void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : setVideoFeature
���� : ������Ƶ����
���� : 
const Mat& _videoFeature, ��Ƶ����
����ֵ : void
�׳��쳣 : exception, ��������
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iCF.setVideoFeature(_videoFeature);
--------------------------------------------------------------------------------
���� : C.L.Wang
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
������ : setModelPath
���� : ����ģ��·��
���� : 
const string& _modelPath, ģ��·��
����ֵ : void
�׳��쳣 : ��������
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iCF.setModelPath(_modelPath);
--------------------------------------------------------------------------------
���� : C.L.Wang
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
������ : __printLog
���� : �����Ϣ
���� : 
ostream& _os, �����ʽ; 
const std::string& _msg, �������;
����ֵ : std::ostream&;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : __printLog(cout, "Nothing");
--------------------------------------------------------------------------------
���� : C.L.Wang
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