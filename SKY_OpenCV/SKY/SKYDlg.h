
// SKYDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"

#include "DlgAnn.h"
#include "DlgBayesianClassifier.h"
#include "DlgBoosting.h"
#include "DlgKnn.h"


// CSKYDlg 대화 상자
class CSKYDlg : public CDialogEx
{
// 생성입니다.
public:
	CSKYDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SKY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	DlgAnn*						m_pDlgAnn;
	DlgBayesianClassifier*		m_pDlgBayesianClassifier;
	DlgBoosting*				m_pDlgBoosting;
	DlgKnn*						m_pDlgKnn;




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
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	//! 탭 컨트롤 초기화
	CTabCtrl m_Tab;
	void InitTap();

	//! 폴더의 개수를 읽음
	int ReadDirectory();
	CListCtrl m_listAll;
	set<CString> m_setFolderAll;

	CListCtrl m_listTrain;
	set<CString> m_setFolderTrain;

	CListCtrl m_listTest;
	set<CString> m_setFolderTest;
	
	void RefreshList(E_TYPE eType);

	vector<ST_SCENARIO>		m_vecScenario;
	vector<float>			m_vecScenarioRate;
	vector<ST_SCENARIO>		m_vecScenarioTmp;

	void RefreshMessage();

	//! 리스트 항목 새로고침
	afx_msg void OnBnClickedBtnRefresh();
	
	//! 리스트 좌우 이동
	afx_msg void OnBnClickedBtnTrainPlus();
	afx_msg void OnBnClickedBtnTrainMinus();
	afx_msg void OnBnClickedBtnTestPlus();
	afx_msg void OnBnClickedBtnTestMinus();
	afx_msg void OnDestroy();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonScenarioSave();
	afx_msg void OnBnClickedButtonScenarioChk();

	afx_msg LRESULT AlgorithmMess(WPARAM wParam, LPARAM lParam);
	CListCtrl m_ListScenarioInfo;
	void  DispListScenario();
	afx_msg void OnBnClickedButtonScenarioDel();
	CRichEditCtrl m_richLogInfo;

	void DispLogInfo(CString strMsg);

	int m_nLogCnt;
};
