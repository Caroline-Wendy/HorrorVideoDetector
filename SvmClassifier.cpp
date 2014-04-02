/*! @file
********************************************************************************
<PRE>
模块名 : 分类器
文件名 : SvmClassifier.cpp
相关文件 : SvmClassifier.h
文件实现功能 : SVM分类器类实现
作者 : C.L.Wang
Email: morndragon@126.com
版本 : 1.0
--------------------------------------------------------------------------------
多线程安全性 : 是
异常时安全性 : 是
--------------------------------------------------------------------------------
备注 : 无
--------------------------------------------------------------------------------
修改记录 :  
日 期              版本   修改人         修改内容 
2014/03/27  1.0    C.L.Wang        Create
</PRE>
********************************************************************************

* 版权所有(c) C.L.Wang, 保留所有权利

*******************************************************************************/

#include "stdafx.h"

#include "SvmClassifier.h"

#include <opencv.hpp>

using namespace std;
using namespace cv;
using namespace vd;

const std::string SvmClassifier::NORM_NAME = "normalization.xml"; //归一化模型
const std::string SvmClassifier::SVM_MODEL_NAME = "hvd.model"; //Svm模型
bool SvmClassifier::m_mutex = true; //互斥锁

/*! @function
********************************************************************************
<PRE>
函数名 : SvmClassifier
功能 : 参数构造函数
参数 : 
const Mat& _videoFeature, 视频特征; 
const string& _modelPath, 模型路径;
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : SvmClassifier iSF(_videoFeature, _modelPath);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
SvmClassifier::SvmClassifier (
	const cv::Mat& _videoFeature, /*特征*/
	const std::string& _modelPath /*模型路径*/
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
函数名 : ~SvmClassifier
功能 : 析构函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iSC.~SvmClassifier();
--------------------------------------------------------------------------------
作者 : C.L.Wang
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
函数名 : calculateResult
功能 : 计算分类结果
参数 : void
返回值 : const double, 分类结果
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : result = iSC.calculateResult();
--------------------------------------------------------------------------------
作者 : C.L.Wang
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
函数名 : _predictValue
功能 : 预测值
参数 : void
返回值 : const double, 预测值;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : result = _predictValue();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
const double SvmClassifier::_predictValue (void) const
{
	double label (0.0);
	double prop (0.0);
	const int nr_class (2);
	double* prob_estimates = (double *) malloc(nr_class*sizeof(double));

	label = svm_predict_probability(m_model, m_node, prob_estimates);
	prop = prob_estimates[0]; //返回预测概率值

	delete[] prob_estimates;

	return prop;
}

/*! @function
********************************************************************************
<PRE>
函数名 : _initModel
功能 : 初始化模型
参数 : void
返回值 : void
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : _initModel();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void SvmClassifier::_initModel (void)
{
	/*完整路径*/
	
	std::string modelName (m_modelPath); //模型名称
	std::string normName (m_modelPath); //归一化名称

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
函数名 : __transSvmNode
功能 : 转换Svm结点
参数 : const string& normName, 归一化模型路径
返回值 : void
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : __transSvmNode(normName);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
void SvmClassifier::__transSvmNode (const std::string& _normName)
{
	cv::FileStorage fs(_normName, FileStorage::READ);
	cv::Mat maxNorm;
	fs["normalization"] >> maxNorm;
	fs.release(); 

	/*归一化视频特征*/

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