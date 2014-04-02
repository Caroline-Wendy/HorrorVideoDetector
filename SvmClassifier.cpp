/*! @file
********************************************************************************
<PRE>
ģ���� : ������
�ļ��� : SvmClassifier.cpp
����ļ� : SvmClassifier.h
�ļ�ʵ�ֹ��� : SVM��������ʵ��
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
2014/03/27  1.0    C.L.Wang        Create
</PRE>
********************************************************************************

* ��Ȩ����(c) C.L.Wang, ��������Ȩ��

*******************************************************************************/

#include "stdafx.h"

#include "SvmClassifier.h"

#include <opencv.hpp>

using namespace std;
using namespace cv;
using namespace vd;

const std::string SvmClassifier::NORM_NAME = "normalization.xml"; //��һ��ģ��
const std::string SvmClassifier::SVM_MODEL_NAME = "hvd.model"; //Svmģ��
bool SvmClassifier::m_mutex = true; //������

/*! @function
********************************************************************************
<PRE>
������ : SvmClassifier
���� : �������캯��
���� : 
const Mat& _videoFeature, ��Ƶ����; 
const string& _modelPath, ģ��·��;
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : SvmClassifier iSF(_videoFeature, _modelPath);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
SvmClassifier::SvmClassifier (
	const cv::Mat& _videoFeature, /*����*/
	const std::string& _modelPath /*ģ��·��*/
	) :
	Classifier(_videoFeature, _modelPath),
	m_model(nullptr),
	m_node(nullptr)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : ~SvmClassifier
���� : ��������
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iSC.~SvmClassifier();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
SvmClassifier::~SvmClassifier (void)
{
	if (m_model != nullptr) {
		svm_free_and_destroy_model(&m_model);
	}

	if (m_node != nullptr) {
		delete[] m_node;
		m_node = nullptr;
	}

	return;
}

/*! @function
********************************************************************************
<PRE>
������ : calculateResult
���� : ���������
���� : void
����ֵ : const double, ������
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : result = iSC.calculateResult();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const double SvmClassifier::calculateResult (void)
{
	double result(0.0);

	while(1) {
		if (m_mutex == true) 
		{
			m_mutex = false;
			_initModel();

			result = _predictValue();

			if (m_model != nullptr) {
				svm_free_and_destroy_model(&m_model);
			}

			if (m_node != nullptr) {
				delete[] m_node;
				m_node = nullptr;
			}
			m_mutex = true;
			break;
		}
	}

	return result;
}

/*! @function
********************************************************************************
<PRE>
������ : _predictValue
���� : Ԥ��ֵ
���� : void
����ֵ : const double, Ԥ��ֵ;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : result = _predictValue();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const double SvmClassifier::_predictValue (void) const
{
	double label (0.0);
	double prop (0.0);
	const int nr_class (2);
	double* prob_estimates = (double *) malloc(nr_class*sizeof(double));

	label = svm_predict_probability(m_model, m_node, prob_estimates);
	prop = prob_estimates[0]; //����Ԥ�����ֵ

	delete[] prob_estimates;

	return prop;
}

/*! @function
********************************************************************************
<PRE>
������ : _initModel
���� : ��ʼ��ģ��
���� : void
����ֵ : void
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : _initModel();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
void SvmClassifier::_initModel (void)
{
	/*����·��*/
	
	std::string modelName (m_modelPath); //ģ������
	std::string normName (m_modelPath); //��һ������

	const std::string slash("/");

	modelName.append(slash);
	modelName.append(SVM_MODEL_NAME);

	normName.append(slash);
	normName.append(NORM_NAME);

	std::ifstream ifs;
	ifs.open(modelName, ios::in);
	if (ifs.fail()) {
		__printLog(std::cerr, "Failed to open the model file!");
	}
	ifs.close();

	ifs.open(normName, ios::in);
	if (ifs.fail()) {
		__printLog(std::cerr, "Failed to open the model file!");
	}
	ifs.close();

	if (m_model != nullptr) {
		svm_free_and_destroy_model(&m_model);
	}
	m_model = svm_load_model(modelName.c_str());

	__transSvmNode(normName);

	return;
}

/*! @function
********************************************************************************
<PRE>
������ : __transSvmNode
���� : ת��Svm���
���� : const string& normName, ��һ��ģ��·��
����ֵ : void
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : __transSvmNode(normName);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
void SvmClassifier::__transSvmNode (const std::string& _normName)
{
	cv::FileStorage fs(_normName, FileStorage::READ);
	cv::Mat maxNorm;
	fs["normalization"] >> maxNorm;
	fs.release(); 

	/*��һ����Ƶ����*/

	cv::Mat normFeature = 
		cv::Mat::zeros(1, maxNorm.cols-2, CV_64FC1);

	for (int j=2; j<m_videoFeature.cols; ++j) {
		for(int i=0; i<m_videoFeature.rows; ++i) {
			normFeature.at<double>(0, j-2) += m_videoFeature.at<double>(i, j);
		}
	}

	for (int j=0; j<normFeature.cols; ++j)
	{
		normFeature.at<double>(0, j) /= m_videoFeature.rows;
		if (maxNorm.at<double>(0, j+2) > 0.0001)
			normFeature.at<double>(0, j) /= maxNorm.at<double>(0, j+2);
	}
	normFeature.at<double>(0,0) = 0.0;

	if (m_node != nullptr) {
		delete[] m_node;
		m_node = nullptr;
	}

	m_node = new svm_node[normFeature.cols];
	for (int j=1; j < normFeature.cols; ++j) {
		m_node[j-1].index = j;
		m_node[j-1].value = normFeature.at<double>(0, j);
	}

	m_node[normFeature.cols-1].index = -1;
	m_node[normFeature.cols-1].value = 0;

	return;
}