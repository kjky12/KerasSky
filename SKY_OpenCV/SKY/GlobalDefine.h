#ifndef _GLOBALDEFINE_H_
#define _GLOBALDEFINE_H_
#include "afxmt.h"

#include <sys/timeb.h>




#pragma  pack(push,1)

#define dfTAB_SIZEX_START_GAP		1
#define dfTAB_SIZEX_END_GAP			4
#define dfTAB_SIZEY_START_GAP		21
#define dfTAB_SIZEY_END_GAP			23


#define dfALGORITHM_PATTERN_ANN					WM_USER + 888
#define dfALGORITHM_PATTERN_BAYESIAN_CLASSIFER	WM_USER + 889
#define dfALGORITHM_PATTERN_BOOSTING			WM_USER + 890
#define dfALGORITHM_PATTERN_KNN					WM_USER + 8891

typedef enum E_TYPE
{
	E_ALL = 0,
	E_TEST,
	E_TRAIN,
	E_DEFAULT
}E_TYPE;


typedef struct ST_SCENARIO
{
	set<CString>	setTest;
	set<CString>	setTrain;

}ST_SCENARIO;



//! ���Ͽ��� ���� ������ ���� g_mapData<���, �ڼ� ������>
extern map<CString ,map<CString, vector< vector<float>>>> g_mapData;


//csv �ѱ� �б�
extern CString	gf_ReadCsvKor(CString strFilePath);

// �������� ��ġ ��������
extern CString	gf_GetModulePath();

extern CString gf_GetCtrlText(LPCTSTR sPath,LPCTSTR lpGrpName, LPCTSTR lpCtrlName);

// �ؽ�Ʈ �Է� ���� ��ġ�� ������ ����
extern BOOL gf_CreateTextFile(CString strFilePath);

// �Է��� ����� ���Ͽ� �ؽ�Ʈ�� �Է�.
extern BOOL gf_InsertTextFile(CString strFilePath,CString strInsetText);


//! �ش� ���丮�� �б�
extern int gf_ReadDataFileName(CString strName, map<CString, vector< vector<float>>>& mapData,int& nDataCnt);

//! ��ü �� �б�
extern int gf_ReadDataFile(map<CString, vector< vector<float>>>& mapData,int& nDataCnt);

//! �ð� �޾ƿ���
extern CString gf_RecvCurrentTimeString(bool bMilisecFlag, struct tm &tmtime, struct _timeb &dLogTime);


#pragma  pack(pop)


#endif _GLOBALDEFINE_H_