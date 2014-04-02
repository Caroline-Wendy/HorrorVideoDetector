/*! @file
********************************************************************************
<PRE>
ģ���� : ͼ������
�ļ��� : HarmonyFeatures.cpp
����ļ� : HarmonyFeatures.h
�ļ�ʵ�ֹ��� : ͼ���г��������ʵ��
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

#include "HarmonyFeatures.h"

using namespace std;
using namespace cv;
using namespace vd;

const double HarmonyFeatures::MY_PI = 3.1415926536; //PIֵ(��ѧ)
const double HarmonyFeatures::MY_E = 2.718281828459046; //Eֵ(��ѧ)

/*��׼��г��*/

const double HarmonyFeatures::MAX_HARMONY = 0.697041*0.9;
const double HarmonyFeatures::MIN_HARMONY = -0.785739*0.9;

/*! @function
********************************************************************************
<PRE>
������ : HarmonyFeatures
���� : Ĭ�Ϲ��캯��
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : HarmonyFeatures iHF;
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
HarmonyFeatures::HarmonyFeatures (void) : 
ImageFeatures()
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : HarmonyFeatures
���� : �������캯��
���� : const cv::Mat& _image, ͼ��;
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : HarmonyFeatures iHF(image);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
HarmonyFeatures::HarmonyFeatures (const cv::Mat& _image) : 
ImageFeatures(_image)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : ~HarmonyFeatures
���� : ��������
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iHF.~HarmonyFeatures();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
HarmonyFeatures::~HarmonyFeatures (void)
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
�����÷� : isOK = iHF.extractFeatures();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool HarmonyFeatures::extractFeatures (void)
{
	if (!m_isImage) {
		__printLog(std::cerr, "Please, load image first!");
		return false;
	}

	m_pFeatures.reset(new std::vector<double>(FEATURE_NUM)); //��������

	cv::Mat labImage; //labͼ��
	cv::cvtColor(m_image, labImage, CV_BGR2Lab);

	for (int i=1; i<labImage.rows-1; ++i)
	{
		for (int j=1 ; j<labImage.cols-1; ++j)
		{
			cv::Scalar s1;

			s1.val[0] = labImage.at<cv::Vec3b>(i,j)[0];
			s1.val[1] = labImage.at<cv::Vec3b>(i,j)[1];
			s1.val[2] = labImage.at<cv::Vec3b>(i,j)[2];

			double xL2(0.0), A(0.0), B(0.0);

			for (int m=i-1; m<i+2; ++m)
			{
				for (int n=j-1; n<j+2; ++n)
				{
					cv::Scalar s2;

					s2.val[0] = labImage.at<cv::Vec3b>(m,n)[0];
					s2.val[1] = labImage.at<cv::Vec3b>(m,n)[1];
					s2.val[2] = labImage.at<cv::Vec3b>(m,n)[2];

					xL2 += s2.val[0];
					A += s2.val[1];
					B += s2.val[2];
				}
			}

			const double harmonyScore = 
				__calculateHarmonyScore (s1, xL2, A, B); //�����г�ȵ÷�

			const std::size_t indexHarmony = 
				__calculateIndex (harmonyScore); //�����г������

			++(*m_pFeatures)[indexHarmony];
		}
	}

	const double product = (labImage.rows-2)*(labImage.cols-2);

	for (int i=0; i<FEATURE_NUM; ++i) {
		(*m_pFeatures)[i] /= product;
	}

	m_isFeatures = true; //�����������

	return true;
}

/*! @function
********************************************************************************
<PRE>
������ : __calculateHarmonyScore
���� : �����г�ȷ���
���� : 
const cv::Scalar& s1, ���ص�;
const double L2, L��; 
double A, A��;
double B, B��;
����ֵ : const double, ��г�ȷ���;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : harmonyScore = __calculateHarmonyScore (s1, L2, A, B);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const double HarmonyFeatures::__calculateHarmonyScore (
	const cv::Scalar& _s1, const double _L2, const double _A, const double _B)
{
	double harmonyScore (0.0); //��г�ȵ÷�

	double HC(0.0); //���Ͷ�����
	double HL(0.0); //��������
	double HH(0.0); //ɫ������

	double L2 = (_L2-_s1.val[0])/8;
	double A = (_A-_s1.val[1])/8;
	double B = (_B-_s1.val[2])/8;

	double L1,C1,H1,C2,H2;

	L1 = _s1.val[0];
	C1 = sqrt(pow(_s1.val[1],2)+pow(_s1.val[2],2));
	H1 = atan(_s1.val[2]/_s1.val[1]);
	C2 = sqrt(pow(A,2)+pow(B,2));
	H2 = atan(B/A);

	/*����ÿ�����ص���������*/

	double temp1,temp2,temp3,temp4;
	double temp5,temp6,temp7;

	temp1 = sqrt(pow(H1-H2,2)+pow((C1-C2)/1.46,2));
	HC = 0.04+0.53*tanh(0.8-0.045*temp1);

	temp1 = 0.28+0.54*tanh(-3.88+0.029*(L1+L2));
	HL = temp1+0.14+0.15*tanh(-2+0.2*abs(L1-L2));

	temp2 = 0.5+0.5*tanh(-2+0.5*C1);
	temp3 = -0.08-0.14*sin(H1+50*MY_PI/180)-0.07*(2*H1+MY_PI/2);
	temp4 = ((0.22*L1-12.8)/10) * 
		pow(MY_E,(90-H1*180/(MY_PI*10)-pow(MY_E,90-H1*180/(MY_PI*10))));
	temp5 = 0.5+0.5*tanh(-2+0.5*C2);
	temp6 = -0.08-0.14*sin(H2+50*MY_PI/180)-0.07*(2*H2+MY_PI/2);
	temp7 = ((0.22*L2-12.8)/10)*
		pow(MY_E,(90-H2*180/(MY_PI*10)-pow(MY_E,90-H2*180/(MY_PI*10))));

	HH = temp2*(temp3+temp4)+temp5*(temp6+temp7);

	harmonyScore = HC + HL + HH; //�����г��ֵ

	return harmonyScore;
}

/*! @function
********************************************************************************
<PRE>
������ : __calculateIndex
���� : ��������ֵ
���� : double _harmonyScore, ��г�ȷ���;
����ֵ : const std::size_t ����ֵ, ��ʾ����ֱ��ͼ��bin;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : index = __calculateIndex (harmonyScore);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const std::size_t HarmonyFeatures::__calculateIndex (const double _harmonyScore)
{
	const double normHarmony = 
		(_harmonyScore-MIN_HARMONY)/
		(MAX_HARMONY-MIN_HARMONY); //��һ��

	int indexHarmony = static_cast<int>(normHarmony*25.0);

	if ( indexHarmony < 0 ) {
		indexHarmony = 0;
	} else if ( indexHarmony > 25) {
		indexHarmony = 25;
	}

	return indexHarmony;
}
