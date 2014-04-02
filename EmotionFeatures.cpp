/*! @file
********************************************************************************
<PRE>
ģ���� : ͼ������
�ļ��� : EmotionFeatures.cpp
����ļ� : EmotionFeatures.h
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

#include "EmotionFeatures.h"

using namespace std;
using namespace cv;
using namespace vd;


/*PI��ֵ*/

const double EmotionFeatures::MY_PI = 3.1415926536;

/*��׼ɫ��ͼ*/

const double EmotionFeatures::MAX_ACTIVITY = 16.1235*0.5; //������ֵ
const double EmotionFeatures::MIN_ACTIVITY = 6.77574*1.5; //�����Сֵ
const double EmotionFeatures::MAX_WEIGHT = 2.45524*0.5; //��Ҫ�����ֵ
const double EmotionFeatures::MIN_WEIGHT = -7.74189*0.5; //��Ҫ����Сֵ
const double EmotionFeatures::MAX_HEAT = 5.41763*0.5; //�ȶ����ֵ
const double EmotionFeatures::MIN_HEAT = 0.426733*1.5; //�ȶ���Сֵ

/*! @function
********************************************************************************
<PRE>
������ : EmotionFeatures
���� : Ĭ�Ϲ��캯��
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : EmotionFeatures iEF;
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
EmotionFeatures::EmotionFeatures (void) : 
	ImageFeatures()
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : EmotionFeatures
���� : �������캯��
���� : const cv::Mat& _image, ͼ��;
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : EmotionFeatures iEF(image);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
EmotionFeatures::EmotionFeatures (const cv::Mat& _image) :
	ImageFeatures(_image)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : ~EmotionFeatures
���� : ��������
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iEF.~EmotionFeatures();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
EmotionFeatures::~EmotionFeatures (void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : extractFeatures
���� : ��ȡ����
���� : void
����ֵ : const bool, �Ƿ�ɹ�;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : isOK = extractFeatures();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool EmotionFeatures::extractFeatures (void)
{
	if (!m_isImage) { //�ж��Ƿ����ͼ��
		__printLog(std::cerr, "Please load image first!");
		return false;
	}

	m_pFeatures.reset(new std::vector<double>(FEATURE_NUM)); //��������

	cv::Scalar s; //�洢���ص�

	double activity(0.0); //���
	double weight(0.0); //��Ҫ��
	double heat(0.0); //�ȶ�

	cv::Mat labImage; //labͼ��
	cv::cvtColor(m_image, labImage, CV_BGR2Lab);

	for (int i=0; i<labImage.rows; ++i)
	{
		for (int j=0; j<labImage.cols; ++j)
		{
			s.val[0] = labImage.at<cv::Vec3b>(i,j)[0];
			s.val[1] = labImage.at<cv::Vec3b>(i,j)[1];
			s.val[2] = labImage.at<cv::Vec3b>(i,j)[2];

			__processPixel(s, activity, weight, heat); //��������

			const std::size_t indexEmotion = 
				__calculateIndex(activity, weight, heat); //��������

			++(*m_pFeatures)[indexEmotion];
		}
	}

	const double product = labImage.rows*labImage.cols;
	for (int i=0; i<FEATURE_NUM ; ++i) {
		(*m_pFeatures)[i] /= product;
	}

	labImage.release();

	m_isFeatures = true; //�����������

	return true;
}

/*! @function
********************************************************************************
<PRE>
������ : __processPixel
���� : �������ص�
���� : 
const cv::Scalar& _s, ���ص�;
double& _activity, ���;
double& _weight, ��Ҫ��;
double& _heat, �ȶ�;
����ֵ : void
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : __processPixel(s, activity, weight, heat);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
void EmotionFeatures::__processPixel (
	const cv::Scalar& _s, double& _activity, double& _weight, double& _heat) const
{
	const double chroma = sqrt(pow(_s.val[1],2)+pow(_s.val[2],2)); //ɫ��
	const double hue = atan(static_cast<double>(_s.val[2])/static_cast<double>(_s.val[1])); //�Ҷ�

	_activity = -2.1+0.06*sqrt(pow(_s.val[0]-50,2)+pow(_s.val[1]-3,2)+pow(_s.val[2]-17,2));
	_weight = -1.8+0.04*(100-_s.val[0])+0.45*cos(hue-100*MY_PI/180);
	_heat = -0.5+0.02*pow(chroma,1.07)*cos(hue-100*MY_PI/180);

	return;
}

/*! @function
********************************************************************************
<PRE>
������ : __calculateIndex
���� : ��������ֵ
���� : 
const double _activity, ���; 
const double _weight, ��Ҫ��; 
const double _heat, �ȶ�;
����ֵ : const size_t ����ֵ, ��ʾ����ֱ��ͼ��bin;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : index = __calculateIndex(activity, weight, heat);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const std::size_t EmotionFeatures::__calculateIndex (
	const double _activity, const double _weight, const double _heat) const
{
	const double normActivity = (_activity-MIN_ACTIVITY)/(MAX_ACTIVITY-MIN_ACTIVITY);
	const double normWeight = (_weight-MIN_WEIGHT)/(MAX_WEIGHT-MIN_WEIGHT);
	const double normHeat = (_heat-MIN_HEAT)/(MAX_HEAT-MIN_HEAT);

	double indexActivity = normActivity*48.0;
	if ( indexActivity < 0 ) {
		indexActivity = 0.0;
	} else if ( indexActivity > 48.0 ) {
		indexActivity = 48.0;
	}

	double indexWeight = normWeight*12.0;
	if ( indexWeight < 0 ) {
		indexWeight = 0.0;
	} else if ( indexWeight > 12.0 ){
		indexWeight = 12.0;
	}

	double indexHeat = normHeat*3.0;
	if ( indexHeat < 0 ) {
		indexHeat = 0.0;
	} else if ( indexHeat >3.0) {
		indexHeat = 3.0;
	}

	const std::size_t index = static_cast<std::size_t>(indexHeat + indexWeight + indexActivity);

	return index;
}
