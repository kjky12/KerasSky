
// KerasTestDlg.h : 헤더 파일
//

#pragma once


#include "WndCommClientMng.h"



//학습 모드
#define df_MESS_PY_LEARNING_ALL_INIT             1001  //학습 모드 - 전체 초기화
#define df_MESS_PY_LEARNING_POSE_INIT            1002  //학습 모드 - 부분 초기화
#define df_MESS_PY_LEARNING_ADD_DATA             1003  //학습 모드 - 데이터 추가
#define df_MESS_PY_LEARNING_LEARN                1004  //학습 모드 - 학습

//인식 모드
#define df_MESS_PY_CHK_COUNT_POSE                2001  //인식 모드 - 자세 갯수 확인
#define df_MESS_PY_CHK_POSE                      2002  //인식 모드 - 현재 자세 확인

#define df_MESS_PY_RECV_ERROR   			 	 1200	//서버 에러
#define PY_FAIL_DATA		                	 -9991	//Deep Lerning Fail



//! 헤더만 전송(MessageID)
//학습 모드 - 전체 초기화	
//학습 모드 - 학습
//인식 모드 - 자세 갯수 확인

typedef struct P_HEADE
{
	int			nMessageSize;
	int			nMessageID;
	int			nTemp;
}P_HEADER;


typedef struct ST_MESS_PY_LEARNING_POSE_INIT	//학습 모드 - 부분 초기화
{
	P_HEADER	pHeader;
	int			nPos;	//삭제할 자세 번호
}ST_MESS_PY_LEARNING_POSE_INIT;

typedef struct ST_MESS_PY_LEARNING_ADD_DATA		//학습 모드 - 데이터 추가
{
	P_HEADER	pHeader;
	int			nAddPoseData;		//! 추가할 자세 데이터 갯수
	int			nPose;				//! 추가할 자세
	int			nPoseDataCnt;		//! 자세 갯수(ex : 63)
	float		fPoseData[1024];	//! 자세 데이터
}ST_MESS_PY_LEARNING_ADD_DATA;

typedef struct ST_MESS_PY_CHK_POSE				//인식 모드 - 현재 자세 확인
{
	P_HEADER	pHeader;
	int			nPoseDataCnt;		//! 자세 갯수(ex : 63)
	float		fPoseData[1024];		//! 자세 데이터
}ST_MESS_PY_CHK_POSE;


// CKerasTestDlg 대화 상자
class CKerasTestDlg : public CDialogEx
{
// 생성입니다.
public:
	CKerasTestDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_KERASTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


	CWndCommClientMng*		m_pWndCommClient;



// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
