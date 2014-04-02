/*! @file
********************************************************************************
<PRE>
ģ���� : ������
�ļ��� : SvmClassifier.h 
����ļ� : SvmClassifier.cpp
�ļ�ʵ�ֹ��� : SVM������������
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
2014/03/27   1.0    C.L.Wang        Create
</PRE>
********************************************************************************

* ��Ȩ����(c) C.L.Wang, ��������Ȩ��

*******************************************************************************/

#pragma once

#include <fstream>

#include "svm.h"

#include "Classifier.h"

namespace vd
{
	/*! @class
	********************************************************************************
	<PRE>
	������ : SvmClassifier
	���� : SVM������
	�쳣�� : ��
	--------------------------------------------------------------------------------
	��ע : ��
	�����÷� : SvmClassifier iSC(_videoFeature, _modelPath);
	--------------------------------------------------------------------------------
	���� : C.L.Wang
	</PRE>
	*******************************************************************************/
	class SvmClassifier : Classifier 
	{
	public:

		SvmClassifier( //���캯��
			const cv::Mat& _videoFeature, /*����*/
			const std::string& _modelPath /*ģ��·��*/
		);

		~SvmClassifier(void); //��������

	public:

		const double calculateResult (void)  override; //Ԥ����

	private:

		static const std::string NORM_NAME; //��һ��ģ��

		static const std::string SVM_MODEL_NAME; //Svmģ������

		static bool m_mutex; //������

	private:

		svm_node* m_node; //Svm���

		svm_model* m_model; //Svmģ��

	private:

		const double _predictValue (void) const override; //Ԥ����

		void _initModel (void) override; //��ʼ��ģ��

	private:

		void __transSvmNode (const std::string& _normName); //ת��SVM���

	};
}