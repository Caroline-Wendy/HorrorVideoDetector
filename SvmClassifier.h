/*! @file
********************************************************************************
<PRE>
模块名 : 分类器
文件名 : SvmClassifier.h 
相关文件 : SvmClassifier.cpp
文件实现功能 : SVM分类器类声明
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
2014/03/27   1.0    C.L.Wang        Create
</PRE>
********************************************************************************

* 版权所有(c) C.L.Wang, 保留所有权利

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
	类名称 : SvmClassifier
	功能 : SVM分类器
	异常类 : 无
	--------------------------------------------------------------------------------
	备注 : 无
	典型用法 : SvmClassifier iSC(_videoFeature, _modelPath);
	--------------------------------------------------------------------------------
	作者 : C.L.Wang
	</PRE>
	*******************************************************************************/
	class SvmClassifier : Classifier 
	{
	public:

		SvmClassifier( //构造函数
			const cv::Mat& _videoFeature, /*特征*/
			const std::string& _modelPath /*模型路径*/
		);

		~SvmClassifier(void); //析构函数

	public:

		const double calculateResult (void)  override; //预测结果

	private:

		static const std::string NORM_NAME; //归一化模型

		static const std::string SVM_MODEL_NAME; //Svm模型名称

		static bool m_mutex; //互斥锁

	private:

		svm_node* m_node; //Svm结点

		svm_model* m_model; //Svm模型

	private:

		const double _predictValue (void) const override; //预测结果

		void _initModel (void) override; //初始化模型

	private:

		void __transSvmNode (const std::string& _normName); //转换SVM结点

	};
}