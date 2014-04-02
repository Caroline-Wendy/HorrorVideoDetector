/*! @file
********************************************************************************
<PRE>
ģ���� : ������
�ļ��� : Classifier.h 
����ļ� : Classifier.cpp
�ļ�ʵ�ֹ��� : ������������
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
�� ��              �汾   �޸���         �޸����� 
2014/03/27	   1.0    C.L.Wang        Create
</PRE>
********************************************************************************

* ��Ȩ����(c) C.L.Wang, ��������Ȩ��

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
	������ : Classifier
	���� : ������
	�쳣�� : ��
	--------------------------------------------------------------------------------
	��ע : ��
	�����÷� : Classifier iCF(_videoFeature, _modelPath);
	--------------------------------------------------------------------------------
	���� : C.L.Wang
	</PRE>
	*******************************************************************************/
	class Classifier 
	{

	public:

		Classifier(
			const cv::Mat& _videoFeature, /*����*/
			const std::string& _modelPath /*ģ��·��*/
		);

		virtual ~Classifier(void);

	public:

		void setVideoFeature (const cv::Mat& _videoFeature); //��Ƶ����

		void setModelPath (const std::string& _modelPath); //ģ��·��

	public:
		
		virtual const double calculateResult (void) = 0; //Ԥ����

	protected:

		cv::Mat m_videoFeature; //��Ƶ����

		std::string m_modelPath; //ģ��·��

	protected:

		virtual const double _predictValue (void) const = 0; //Ԥ����

		virtual void _initModel (void) = 0; //��ʼ��ģ��

		inline std::ostream& __printLog (
			std::ostream& _os, const std::string& _msg) const; //��ӡ��Ϣ

	};
}