/*! @file
********************************************************************************
<PRE>
ģ���� : ͼ������
�ļ��� : StochasticFeatures.h 
����ļ� : StochasticFeatures.cpp
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
	������ : StochasticFeatures
	���� : ͼ���������
	�쳣�� : ��
	--------------------------------------------------------------------------------
	��ע : ��
	�����÷� : StochasticFeatures iSF;
	--------------------------------------------------------------------------------
	���� : C.L.Wang
	</PRE>
	*******************************************************************************/
	class StochasticFeatures : public ImageFeatures
	{

	public:

		StochasticFeatures (void); //���캯��

		explicit StochasticFeatures (const cv::Mat& _image); //�������캯��

		~StochasticFeatures (void); //��������

	public:

		const bool extractFeatures (void) override; //��ȡ����

	private:

		static const int MY_SIGMA; //������ֵ
		static const double MY_PI; //PIֵ(��ѧ)
		static const double MY_E; //Eֵ(��ѧ)

		static const std::size_t FEATURE_NUM = 2; //��������

	private:

		void __weibullMle (
			double& _scale, /*�߶�����*/
			double& _shape /*��״����*/
			); //Τ���ֲ�

		const double ___newton (
			const double _g, 
			const std::vector<double>& _x
			); //ţ�ٺ���

		void ___gaussianFilter (cv::Mat& _gauss_mat); //��˹�˲�

	};

}