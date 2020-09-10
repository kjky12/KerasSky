#pragma once

#include <opencv2/opencv.hpp>
#include "afxwin.h"

class CSKYDlg;
class DlgKnn : public CDialogEx
{
	DECLARE_DYNAMIC(DlgKnn)

public:
	DlgKnn(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DlgKnn();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_KNN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.



public:
	CSKYDlg*	m_pDlgSKYDlg;

	DECLARE_MESSAGE_MAP()
	void KNNTest ();
public:
	afx_msg void OnBnClickedBtnAnalysis();
	CEdit m_editMaxK;
	CButton m_chkRegression;
	CButton m_chkMaxK;
	afx_msg void OnBnClickedCheck1();
	virtual BOOL OnInitDialog();

	CButton m_radioDefault;
	CButton m_radioScenario;
};
