/*! @file
********************************************************************************
<PRE>
ģ���� : ��������
�ļ��� : SceneIntegration.cpp
����ļ� : SceneIntegration.h
�ļ�ʵ�ֹ��� : ����������ʵ��
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
2014/03/28    1.0    C.L.Wang        ����
</PRE>
********************************************************************************

* ��Ȩ����(c) C.L.Wang, ��������Ȩ��

*******************************************************************************/ 

#include "stdafx.h"

#include "SceneIntegration.h"

using namespace std;
using namespace cv;
using namespace vd;

/*! @function
********************************************************************************
<PRE>
������ : SceneIntegration
���� : �������캯��
���� : 
const string& _fileName, ��Ƶ��;
const size_t  _shotInterval, ��ͷ���;
const size_t _sceneInterval, �������;
����ֵ : ��
�׳��쳣 : exception, ��������
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : SceneIntegration iSI(fileName, shotInterval, sceneInterval);
--------------------------------------------------------------------------------
���� : C.L.Wang
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

	/*�ж��ļ��Ƿ����*/

	std::ifstream ifile;
	ifile.open(m_fileName, ios::in);
	if (ifile.fail()) {
		_printLog(std::cerr, "Failed to open the video!");
		throw std::exception("File Name Error");
	}
	ifile.close();

	/*��ͷ����ж�*/

	if (m_shotInterval < 1 || m_shotInterval > maxShotInterval) {
		_printLog(std::cerr, "The value of the shot interval is invalid.");
		throw std::exception("Shot Interval Error");
	}

	/*��������ж�*/

	if (m_sceneInterval < 1 || m_sceneInterval > maxSceneInterval) {
		_printLog(std::cerr, "The value of the scene interval is invalid.");
		throw std::exception("Scene Interval Error");
	}

	return;
}

/*! @function
********************************************************************************
<PRE>
������ : ~SceneIntegration
���� : ��������
���� : void
����ֵ : ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : iSI.~SceneIntegration ();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
SceneIntegration::~SceneIntegration (void)
{
	return;
}

/*! @function
********************************************************************************
<PRE>
������ : extractSceneFeatures
���� : ��ȡ��������
���� : const size_t _beg, ������ʼλ��
����ֵ : shared_ptr<Mat>
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : pSceneFeatures = iSI.extractSceneFeatures ();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
std::shared_ptr<cv::Mat> SceneIntegration::extractSceneFeatures (const std::size_t _beg) 
{
	cv::VideoCapture cVC (m_fileName); //��Ƶ��ȡ

	if (!cVC.isOpened()) {
		_printLog(std::cerr, "Failed to open the video!");
		return nullptr;
	}

	std::size_t framesNum = 
		static_cast<std::size_t>(cVC.get(CV_CAP_PROP_FRAME_COUNT)); //��֡��

	MiddleKeyFrames iMKF(m_fileName, m_shotInterval, _beg, _beg+m_sceneInterval);

	if(!iMKF.extractKeyFrames() ) {
		_printLog(std::cerr, "Failed to extract key frames.");
		return nullptr;
	}

	m_pKeyFrames = iMKF.getKeyFrames();
	m_framePositions = iMKF.getFramePositions();

	std::shared_ptr<std::vector<double> > 
		pTempFeatures(new std::vector<double>); //��ʱ�洢����
	std::vector<std::vector<double> > totalFeatures; //������

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

	/*�洢��Mat*/

	for (std::size_t i=0; i<m_pKeyFrames->size(); ++i)
	{
		totalFeaturesMat.at<double>(i,0) = static_cast<double>(i+1.0);	//��ͷ
		totalFeaturesMat.at<double>(i,1) = 1.0; //��Ƶ(��������)
		totalFeaturesMat.at<double>(i,2) = 1.0; //��ǩ(�ֲ�)
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
������ : getKeyInformation
���� : ��ȡ�ؼ�֡��Ϣ
���� : void
����ֵ : 
shared_ptr<vector<Mat> >& _keyFrames, �ؼ�֡����;
vector<size_t>& _framePositions, �ؼ�֡λ�ü���;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : getKeyFrames();
--------------------------------------------------------------------------------
���� : C.L.Wang
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
������ : __printLog
���� : ��ӡ��Ϣ
���� : 
_os, ��ӡ��ʽ; 
_msg, ��ӡ����;
����ֵ : std::ostream& ��ӡ��
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : __printLog(cout, "Nothing");
--------------------------------------------------------------------------------
���� : C.L.Wang
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