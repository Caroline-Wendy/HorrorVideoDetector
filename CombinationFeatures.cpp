/*! @file
********************************************************************************
<PRE>
ģ���� : ͼ������
�ļ��� : CombinationFeatures.cpp
����ļ� : CombinationFeatures.h
�ļ�ʵ�ֹ��� : ͼ�����������ʵ��
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

#include "stdafx.h"

#include "CombinationFeatures.h"

using namespace std;
using namespace cv;
using namespace vd;

/*! @function
********************************************************************************
<PRE>
������ : CombinationFeatures
���� : Ĭ�Ϲ��캯��
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : CombinationFeatures iCF;
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
CombinationFeatures::CombinationFeatures (void) : 
	ImageFeatures()
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : CombinationFeatures
���� : �������캯��
���� : const cv::Mat& _image, ͼ��;
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : CombinationFeatures iCF(image);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
CombinationFeatures::CombinationFeatures (const cv::Mat& _image) :
	ImageFeatures(_image)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : ~CombinationFeatures
���� : ��������
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iCF.~CombinationFeatures();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
CombinationFeatures::~CombinationFeatures (void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : extractFeatures
���� : ��ȡ����
���� : void
����ֵ : const bool, �Ƿ�ɹ�
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : isOK = extractFeatures();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool CombinationFeatures::extractFeatures (void)
{
	if (!m_isImage) {
		__printLog(std::cerr, "Please, load image first!");
		return false;
	}

	m_pFeatures.reset(new std::vector<double>); //����Ϊ��!

	StatisticsFeatures cSTA (m_image); //ͳ����
	StochasticFeatures cSTO (m_image); //�����
	EmotionFeatures cEF  (m_image); //�����
	HarmonyFeatures cHF  (m_image); //��г��

	std::shared_ptr<std::vector<double> > staFeatures; //ͳ������
	std::shared_ptr<std::vector<double> > stoFeatures; //�������
	std::shared_ptr<std::vector<double> > efFeatures; //�������
	std::shared_ptr<std::vector<double> > hfFeatures; //��г����

	if (cSTA.extractFeatures()) {
		staFeatures = cSTA.getFeatures();
	} else {
		__printLog(std::cerr, "Failed to extract statistics features! ");
		return false;
	}

	if (cSTO.extractFeatures()) {
		stoFeatures = cSTO.getFeatures();
	} else {
		__printLog(std::cerr, "Failed to extract stochastic features! ");
		return false;
	}

	if (cEF.extractFeatures()) {
		efFeatures = cEF.getFeatures();
	} else {
		__printLog(std::cerr, "Failed to extract emotion features! ");
		return false;
	}

	if (cHF.extractFeatures()) {
		hfFeatures = cHF.getFeatures();
	} else {
		__printLog(std::cerr, "Failed to extract harmony features! ");
		return false;
	}

	/*�������*/

	m_pFeatures->insert(m_pFeatures->end(), staFeatures->begin(), staFeatures->end());
	m_pFeatures->insert(m_pFeatures->end(), stoFeatures->begin(), stoFeatures->end());
	m_pFeatures->insert(m_pFeatures->end(), efFeatures->begin(), efFeatures->end());
	m_pFeatures->insert(m_pFeatures->end(), hfFeatures->begin(), hfFeatures->end());

	m_isFeatures = true; //�������

	return true;
}