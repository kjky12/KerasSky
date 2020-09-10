
// SKYDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"

#include "DlgAnn.h"
#include "DlgBayesianClassifier.h"
#include "DlgBoosting.h"
#include "DlgKnn.h"


// CSKYDlg ��ȭ ����
class CSKYDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CSKYDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SKY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

	DlgAnn*						m_pDlgAnn;
	DlgBayesianClassifier*		m_pDlgBayesianClassifier;
	DlgBoosting*				m_pDlgBoosting;
	DlgKnn*						m_pDlgKnn;




// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	//! �� ��Ʈ�� �ʱ�ȭ
	CTabCtrl m_Tab;
	void InitTap();

	//! ������ ������ ����
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

	//! ����Ʈ �׸� ���ΰ�ħ
	afx_msg void OnBnClickedBtnRefresh();
	
	//! ����Ʈ �¿� �̵�
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
