/*! @file
********************************************************************************
<PRE>
ģ���� : ͼ������
�ļ��� : HarmonyFeatures.h 
����ļ� : HarmonyFeatures.cpp
�ļ�ʵ�ֹ��� : ͼ���г������������
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

#include "ImageFeatures.h"

//��Ƶ��������ռ�
namespace vd 
{

	/*! @class
	********************************************************************************
	<PRE>
	������ : HarmonyFeatures
	���� : ͼ���г������
	�쳣�� : ��
	--------------------------------------------------------------------------------
	��ע : ��
	�����÷� : HarmonyFeatures iHF;
	--------------------------------------------------------------------------------
	���� : C.L.Wang
	</PRE>
	*******************************************************************************/
	class HarmonyFeatures : public ImageFeatures
	{

	public:

		HarmonyFeatures (void); //���캯��

		explicit HarmonyFeatures (const cv::Mat& _image); //�������캯��

		~HarmonyFeatures (void); //��������

	public:

		const bool extractFeatures (void) override; //��ȡ����

	private:

		static const std::size_t FEATURE_NUM = 26; //��������

		static const double MY_PI; //PIֵ(��ѧ)
		static const double MY_E; //Eֵ(��ѧ)

		static const double MAX_HARMONY; //����г��
		static const double MIN_HARMONY; //��С��г��

	private:

		inline const double __calculateHarmonyScore (
			const cv::Scalar& _s1, const double _L2, const double _A, const double _B); //��������

		inline const std::size_t __calculateIndex (const double _harmonyScore); //��������

	};

}