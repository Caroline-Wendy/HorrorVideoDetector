/*! @file
********************************************************************************
<PRE>
ģ���� : ͼ������
�ļ��� : EmotionFeatures.h 
����ļ� : EmotionFeatures.cpp
�ļ�ʵ�ֹ��� : ͼ���������������
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
2014/03/10   1.0    C.L.Wang        ����
</PRE>
********************************************************************************

* ��Ȩ����(c) C.L.Wang, ��������Ȩ��

*******************************************************************************/ 

#pragma once

#include "ImageFeatures.h"

//��Ƶ��������ռ�
namespace vd 
{

	/*! @class
	********************************************************************************
	<PRE>
	������ : EmotionFeatures
	���� : ͼ���������
	�쳣�� : ��
	--------------------------------------------------------------------------------
	��ע : ��
	�����÷� : EmotionFeatures iEF;
	--------------------------------------------------------------------------------
	���� : C.L.Wang
	</PRE>
	*******************************************************************************/
	class EmotionFeatures : public ImageFeatures
	{

	public:

		EmotionFeatures (void); //���캯��

		explicit EmotionFeatures (const cv::Mat& _image); //�������캯��

		~EmotionFeatures (void); //��������

	public:

		const bool extractFeatures (void) override; //��ȡ����

	private:

		static const std::size_t FEATURE_NUM = 64; //��������

		static const double MY_PI; //PIֵ(��ѧ)

		/*��׼ɫ��ͼ*/

		static const double MAX_ACTIVITY; //������ֵ
		static const double MIN_ACTIVITY; //�����Сֵ
		static const double MAX_WEIGHT; //��Ҫ�����ֵ
		static const double MIN_WEIGHT; //��Ҫ����Сֵ
		static const double MAX_HEAT; //�ȶ����ֵ
		static const double MIN_HEAT; //�ȶ���Сֵ

	private:

		inline void __processPixel (
			const cv::Scalar& _s, double& _activity, double& _weight, double& _heat) const; //��������

		inline const std::size_t __calculateIndex (
			const double _activity, const double _weight, const double _heat) const; //��������

	};

}