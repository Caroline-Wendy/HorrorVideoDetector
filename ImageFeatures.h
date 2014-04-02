/*! @file
********************************************************************************
<PRE>
模块名 : 图像特征
文件名 : ImageFeatures.h 
相关文件 : ImageFeatures.cpp
文件实现功能 : 图像特征类声明
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
2014/03/27   1.0    C.L.Wang        创建
</PRE>
********************************************************************************

* 版权所有(c) C.L.Wang, 保留所有权利

*******************************************************************************/ 

#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

#include <opencv.hpp>

//视频检测命名空间
namespace vd 
{

	/*! @class
	********************************************************************************
	<PRE>
	类名称 : ImageFeatures
	功能 : 图像特征
	异常类 : 无
	--------------------------------------------------------------------------------
	备注 : 无
	典型用法 : ImageFeatures iIF
	--------------------------------------------------------------------------------
	作者 : C.L.Wang
	</PRE>
	*******************************************************************************/
	class ImageFeatures
	{

	public:

		ImageFeatures (void); //默认构造

		explicit ImageFeatures (const cv::Mat& _image); //输入图像的构造函数

		virtual ~ImageFeatures (void); //析构函数

	public:

		void loadImage (const cv::Mat& _image); //载入图像

		void showImage (void) const; //显示图像

		virtual const bool extractFeatures (void) = 0; //提取特征

		const std::shared_ptr<std::vector<double> > 
			getFeatures (void) const; //返回特征

	protected:

		cv::Mat m_image; //存储图像

		bool m_isImage; //是否包含图像

		bool m_isFeatures; //是否计算特征

		std::shared_ptr<std::vector<double> > m_pFeatures; //特征

	protected:

		inline void __resetData (void); //重置数据

		inline std::ostream& __printLog (
			std::ostream& _os, const std::string& _msg) const; //打印信息

	};

}
