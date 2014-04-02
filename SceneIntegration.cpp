/*! @file
********************************************************************************
<PRE>
模块名 : 场景集成
文件名 : SceneIntegration.cpp
相关文件 : SceneIntegration.h
文件实现功能 : 场景集成类实现
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
2014/03/28    1.0    C.L.Wang        创建
</PRE>
********************************************************************************

* 版权所有(c) C.L.Wang, 保留所有权利

*******************************************************************************/ 

#include "stdafx.h"

#include "SceneIntegration.h"

using namespace std;
using namespace cv;
using namespace vd;

/*! @function
********************************************************************************
<PRE>
函数名 : SceneIntegration
功能 : 参数构造函数
参数 : 
const string& _fileName, 视频名;
const size_t  _shotInterval, 镜头间隔;
const size_t _sceneInterval, 场景间隔;
返回值 : 无
抛出异常 : exception, 参数错误
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : SceneIntegration iSI(fileName, shotInterval, sceneInterval);
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
SceneIntegration::SceneIntegration (
	const std::string& _fileName, 
	const std::size_t _shotInterval,
	const std::size_t _sceneInterval
	) : 
	m_fileName(_fileName),
	m_shotInterval(_shotInterval),
	m_sceneInterval(_sceneInterval),
	m_pKeyFrames(new std::vector<cv::Mat>),
	m_framePositions(new std::vector<std::size_t>)
{
	std::size_t maxShotInterval(999); 
	std::size_t maxSceneInterval(9999);

	/*判断文件是否存在*/

	std::ifstream ifile;
	ifile.open(m_fileName, ios::in);
	if (ifile.fail()) {
		_printLog(std::cerr, "Failed to open the video!");
		throw std::exception("File Name Error");
	}
	ifile.close();

	/*镜头间隔判断*/

	if (m_shotInterval < 1 || m_shotInterval > maxShotInterval) {
		_printLog(std::cerr, "The value of the shot interval is invalid.");
		throw std::exception("Shot Interval Error");
	}

	/*场景间隔判断*/

	if (m_sceneInterval < 1 || m_sceneInterval > maxSceneInterval) {
		_printLog(std::cerr, "The value of the scene interval is invalid.");
		throw std::exception("Scene Interval Error");
	}

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : ~SceneIntegration
功能 : 析构函数
参数 : void
返回值 : 无
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : iSI.~SceneIntegration ();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
SceneIntegration::~SceneIntegration (void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : extractSceneFeatures
功能 : 提取场景特征
参数 : const size_t _beg, 场景起始位置
返回值 : shared_ptr<Mat>
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : pSceneFeatures = iSI.extractSceneFeatures ();
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
std::shared_ptr<cv::Mat> SceneIntegration::extractSceneFeatures (const std::size_t _beg) 
{
	cv::VideoCapture cVC (m_fileName); //音频读取

	if (!cVC.isOpened()) {
		_printLog(std::cerr, "Failed to open the video!");
		return nullptr;
	}

	std::size_t framesNum = 
		static_cast<std::size_t>(cVC.get(CV_CAP_PROP_FRAME_COUNT)); //总帧数

	MiddleKeyFrames iMKF(m_fileName, m_shotInterval, _beg, _beg+m_sceneInterval);

	if(!iMKF.extractKeyFrames() ) {
		_printLog(std::cerr, "Failed to extract key frames.");
		return nullptr;
	}

	m_pKeyFrames = iMKF.getKeyFrames();
	m_framePositions = iMKF.getFramePositions();

	std::shared_ptr<std::vector<double> > 
		pTempFeatures(new std::vector<double>); //临时存储特征
	std::vector<std::vector<double> > totalFeatures; //总特征

	for (size_t i=0; i != m_pKeyFrames->size(); ++i)
	{
		CombinationFeatures iCF((*m_pKeyFrames)[i]);

		if (iCF.extractFeatures()) {
			pTempFeatures = iCF.getFeatures();
		} else {
			_printLog(std::cerr, "Failed to extract features! ");
			pTempFeatures = nullptr;
		}

		totalFeatures.push_back(*pTempFeatures);
		pTempFeatures->clear();
	}

	int totalRows = totalFeatures.size();
	int totalCols = totalFeatures[0].size()+3;
	cv::Mat totalFeaturesMat = cv::Mat::zeros(totalRows, totalCols, CV_64FC1);

	/*存储进Mat*/

	for (std::size_t i=0; i<m_pKeyFrames->size(); ++i)
	{
		totalFeaturesMat.at<double>(i,0) = static_cast<double>(i+1.0);	//镜头
		totalFeaturesMat.at<double>(i,1) = 1.0; //视频(任意整数)
		totalFeaturesMat.at<double>(i,2) = 1.0; //标签(恐怖)
		for (std::size_t j=0; j<static_cast<int>(totalFeatures[i].size()); ++j) {
			totalFeaturesMat.at<double>(i, j+3) = 
				static_cast<double>(totalFeatures[i][j]);
		}
	}

	std::shared_ptr<cv::Mat> pTotalFeatureMat = 
		std::make_shared<cv::Mat>(totalFeaturesMat);

	return pTotalFeatureMat;
}


/*! @function
********************************************************************************
函数名 : getKeyInformation
功能 : 获取关键帧信息
参数 : void
返回值 : 
shared_ptr<vector<Mat> >& _keyFrames, 关键帧集合;
vector<size_t>& _framePositions, 关键帧位置集合;
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : getKeyFrames();
--------------------------------------------------------------------------------
作者 : C.L.Wang
*******************************************************************************/ 
void SceneIntegration::getKeyInformation (
	std::vector<cv::Mat>& _keyFrames,
	std::vector<std::size_t>& _framePositions
)
{
	if (m_pKeyFrames != nullptr || m_framePositions != nullptr) {
		_keyFrames = *m_pKeyFrames;
		_framePositions = *m_framePositions;
	}

	return;
}

/*! @function
********************************************************************************
<PRE>
函数名 : __printLog
功能 : 打印信息
参数 : 
_os, 打印格式; 
_msg, 打印内容;
返回值 : std::ostream& 打印流
抛出异常 : 无
--------------------------------------------------------------------------------
复杂度 : 无
备注 : 无
典型用法 : __printLog(cout, "Nothing");
--------------------------------------------------------------------------------
作者 : C.L.Wang
</PRE>
*******************************************************************************/ 
std::ostream& SceneIntegration::_printLog (
	std::ostream& _os, const std::string& _msg)
{
	if (std::cerr == _os) {
		_os << "Warning : " << _msg << std::endl;
	} else if (std::clog == _os) {
		_os << "Log : " << _msg << std::endl;
	} else {
		_os << "Message : " << _msg << std::endl;
	}

	return _os;
}