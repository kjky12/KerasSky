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



//! 파일에서 읽은 데이터 변수 g_mapData<경로, 자세 데이터>
extern map<CString ,map<CString, vector< vector<float>>>> g_mapData;


//csv 한글 읽기
extern CString	gf_ReadCsvKor(CString strFilePath);

// 실행파일 위치 가져오기
extern CString	gf_GetModulePath();

extern CString gf_GetCtrlText(LPCTSTR sPath,LPCTSTR lpGrpName, LPCTSTR lpCtrlName);

// 텍스트 입력 파일 위치에 파일을 생성
extern BOOL gf_CreateTextFile(CString strFilePath);

// 입력한 경로의 파일에 텍스트를 입력.
extern BOOL gf_InsertTextFile(CString strFilePath,CString strInsetText);


//! 해당 디렉토리만 읽기
extern int gf_ReadDataFileName(CString strName, map<CString, vector< vector<float>>>& mapData,int& nDataCnt);

//! 전체 다 읽기
extern int gf_ReadDataFile(map<CString, vector< vector<float>>>& mapData,int& nDataCnt);

//! 시간 받아오기
extern CString gf_RecvCurrentTimeString(bool bMilisecFlag, struct tm &tmtime, struct _timeb &dLogTime);


#pragma  pack(pop)


#endif _GLOBALDEFINE_H_