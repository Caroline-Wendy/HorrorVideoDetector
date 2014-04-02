/*! @file
********************************************************************************
<PRE>
ģ���� : �����ӿ�
�ļ��� : ExportInterface.h
����ļ� : ExportInterface.cpp
�ļ�ʵ�ֹ��� : �����ӿ�������
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
2014/03/27   1.0    C.L.Wang        Create
</PRE>
********************************************************************************

* ��Ȩ����(c) C.L.Wang, ��������Ȩ��

*******************************************************************************/

#ifndef HORRORVIDEODETECTORDLL_H_
#define HORRORVIDEODETECTORDLL_H_

#include <windows.h>

struct SceneInfo {
	char imagePath[1024]; //ͼƬ·��
	double prop; //���Ŷ�
	unsigned int bpos; //��ʼλ��
	unsigned int epos; //����λ��
	double btime; //��ʼʱ��
	double etime; //����ʱ��
};

/*! @function
********************************************************************************
<PRE>
������ : HorrorVideoSceneNum
���� : �ֲ���Ƶ��������
���� : 
const char* const _fileName, �ļ���;
const unsigned int _shotInterval, ��ͷ���;
const unsigned int _sceneShotNum, ��������;
����ֵ : const unsigned int, ��������;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : num = HorrorVideoSceneNum (_fileName, _shotInterval, _sceneShotNum);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const unsigned int HorrorVideoSceneNum (
	const char* const _fileName,
	const unsigned int _shotInterval = 100, 
	const unsigned int _sceneShotNum = 20
);

/*! @function
********************************************************************************
<PRE>
������ : HorrorVideoDetector
���� : �ֲ���Ƶ���
���� : 
double* _resultSet, ���صĽ������;
const char* const _fileName, �ļ���;
const char* const _modelPath, ģ��·��;
const unsigned int _shotInterval, ��ͷ���;
const unsigned int _sceneShotNum, ��������;
����ֵ : const bool, �Ƿ�ɹ�;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : 
isOK = HorrorVideoDetector (
_resultSet, _fileName, _modelPath, _shotInterval, _sceneShotNum);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool HorrorVideoDetector (
	double* _resultSet,
	const char* const _fileName, 
	const char* const _modelPath,
	const unsigned int _shotInterval = 100, 
	const unsigned int _sceneShotNum = 20
);

/*! @function
********************************************************************************
<PRE>
������ : HorrorVideoDetector_Info
���� : �ֲ���Ƶ���-��Ϣ
���� : 
SceneInfo* _infoSet, ������Ϣ����;
const char* const _fileName, �ļ���;
const char* const _folderPath, �ļ���·��;
const char* const _modelPath, ģ��·��;
const unsigned int _shotInterval, ��ͷ���;
const unsigned int _sceneShotNum, ��������;
����ֵ : const bool, �Ƿ�ɹ�;
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : 
isOK = HorrorVideoDetector (
_infoSet, _folderPath, _fileName, _modelPath, _shotInterval, _sceneShotNum);
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
const bool HorrorVideoDetector_Info (
	SceneInfo* _resultSet,
	const char* const _folderPath,
	const char* const _fileName, 
	const char* const _modelPath,
	const unsigned int _shotInterval = 100, 
	const unsigned int _sceneShotNum = 20
);

/*! @function
********************************************************************************
<PRE>
������ : HorrorVideoDetector_Help
���� : �ֲ���Ƶ���-����
���� : void
����ֵ : void
�׳��쳣 : ��
--------------------------------------------------------------------------------
���Ӷ� : ��
��ע : ��
�����÷� : HorrorVideoDetector_Help();
--------------------------------------------------------------------------------
���� : C.L.Wang
</PRE>
*******************************************************************************/ 
void HorrorVideoDetector_Help(void);

#endif /* HorrorVideoDetectorDll_H_ */