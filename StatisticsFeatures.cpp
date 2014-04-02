/*! @file
********************************************************************************
<PRE>
ģ���� : ͼ������
�ļ��� : StatisticsFeatures.cpp
����ļ� : StatisticsFeatures.h
�ļ�ʵ�ֹ��� : ͼ��ͳ��������ʵ��
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

#include "StatisticsFeatures.h"

using namespace std;
using namespace cv;
using namespace vd;

/*! @function
********************************************************************************
<PRE>
������ : StatisticsFeatures
���� : Ĭ�Ϲ��캯��
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : StatisticsFeatures iSF;
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
StatisticsFeatures::StatisticsFeatures(void) : 
ImageFeatures()
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : StatisticsFeatures
���� : �������캯��
���� : const cv::Mat& _image, ͼ��;
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : StatisticsFeatures iSF(image);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
StatisticsFeatures::StatisticsFeatures (const cv::Mat& _image) : 
ImageFeatures(_image)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : ~StatisticsFeatures
���� : ��������
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iSF.~StatisticsFeatures();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
StatisticsFeatures::~StatisticsFeatures (void)
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
const bool StatisticsFeatures::extractFeatures (void)
{
	if (!m_isImage) {
		__printLog(std::cerr, "Please load image first!");
		return false;
	}

	m_pFeatures.reset(new std::vector<double>(FEATURE_NUM)); //��������

	const std::size_t HSV_NUM = 6; //HSV������
	std::vector<double> hsvFeatures(HSV_NUM); //HSV, 6ά����
	double lightnessKey (0.0); //����ֵ

	__calculateHSV(hsvFeatures, lightnessKey);

	for (int i=0; i<6; ++i) {
		(*m_pFeatures)[i] = hsvFeatures[i];
	}
	(*m_pFeatures)[6] = lightnessKey;

	double lightnessMedian(0.0); //������ֵ
	double shadowProportion(0.0); //��Ӱ����

	__calculateHLS(lightnessMedian, shadowProportion);

	(*m_pFeatures)[7] = lightnessMedian;
	(*m_pFeatures)[8] = shadowProportion;

	m_isFeatures = true;

	return true;
}

/*! @function
********************************************************************************
<PRE>
������ : __calculateHSV
���� : ����HSV����(6ά), ������ֵ(V)
���� : 
vector<double>& _hsvFeatures, HSV, 6ά����;
double& _lightnessKey, ����ֵ;
����ֵ : void
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : __calculateHSV(hsvFeatures, lightnessKey);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
void StatisticsFeatures::__calculateHSV (
	std::vector<double>& _hsvFeatures,
	double& _lightnessKey
	)
{
	cv::Mat hsvImage; //HSVͼ��
	cv::cvtColor(m_image, hsvImage, CV_BGR2HSV);

	cv::Scalar mean, stddev;
	cv::meanStdDev(hsvImage, mean, stddev);

	_hsvFeatures[0] = mean[0];
	_hsvFeatures[1] = mean[1];
	_hsvFeatures[2] = mean[2];
	_hsvFeatures[3] = stddev[0];
	_hsvFeatures[4] = stddev[1];
	_hsvFeatures[5] = stddev[2];

	_lightnessKey = _hsvFeatures[2]*_hsvFeatures[5];

	return;
}

/*! @function
********************************************************************************
<PRE>
������ : __calculateHLS
���� : ����������ֵ����Ӱ����
���� : 		
double& _lightnessMedian, ������ֵ;
double& _shadowProportion, ��Ӱ����;
����ֵ : void
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : __calculateHLS(lightnessMedian, shadowProportion);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
void StatisticsFeatures::__calculateHLS (
	double& _lightnessMedian, /*������ֵ*/
	double& _shadowProportion /*��Ӱ����*/
	)
{
	cv::Mat hlsImage; //HLSͼ��
	cv::cvtColor(m_image, hlsImage, CV_BGR2HLS);

	std::vector<double> ldata;
	long num(0);
	cv::Scalar s;

	for(int i=0; i<hlsImage.rows; ++i)
	{
		for(int j=0; j<hlsImage.cols; ++j)
		{
			s.val[1] = hlsImage.at<cv::Vec3b>(i,j)[1];
			ldata.push_back(s.val[1]);
			if (s.val[1]<46)
				num++;
		}
	}

	const long medianPos = static_cast<long>(ldata.size()/2);
	std::nth_element(ldata.begin(), ldata.begin()+medianPos, ldata.end()); //����

	_lightnessMedian = ldata[medianPos]; //�������Ⱦ�ֵ

	const double tmp = hlsImage.size().width*hlsImage.size().height;
	_shadowProportion = num/tmp; //������Ӱ����

	hlsImage.release();

	return;
}

