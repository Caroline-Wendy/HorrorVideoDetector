/*! @file
********************************************************************************
<PRE>
ģ���� : ͼ������
�ļ��� : ImageFeatures.h 
����ļ� : ImageFeatures.cpp
�ļ�ʵ�ֹ��� : ͼ������������
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
2014/03/27   1.0    C.L.Wang        ����
</PRE>
********************************************************************************

* ��Ȩ����(c) C.L.Wang, ��������Ȩ��

*******************************************************************************/ 

#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

#include <opencv.hpp>

//��Ƶ��������ռ�
namespace vd 
{

	/*! @class
	********************************************************************************
	<PRE>
	������ : ImageFeatures
	���� : ͼ������
	�쳣�� : ��
	--------------------------------------------------------------------------------
	��ע : ��
	�����÷� : ImageFeatures iIF
	--------------------------------------------------------------------------------
	���� : C.L.Wang
	</PRE>
	*******************************************************************************/
	class ImageFeatures
	{

	public:

		ImageFeatures (void); //Ĭ�Ϲ���

		explicit ImageFeatures (const cv::Mat& _image); //����ͼ��Ĺ��캯��

		virtual ~ImageFeatures (void); //��������

	public:

		void loadImage (const cv::Mat& _image); //����ͼ��

		void showImage (void) const; //��ʾͼ��

		virtual const bool extractFeatures (void) = 0; //��ȡ����

		const std::shared_ptr<std::vector<double> > 
			getFeatures (void) const; //��������

	protected:

		cv::Mat m_image; //�洢ͼ��

		bool m_isImage; //�Ƿ����ͼ��

		bool m_isFeatures; //�Ƿ��������

		std::shared_ptr<std::vector<double> > m_pFeatures; //����

	protected:

		inline void __resetData (void); //��������

		inline std::ostream& __printLog (
			std::ostream& _os, const std::string& _msg) const; //��ӡ��Ϣ

	};

}
