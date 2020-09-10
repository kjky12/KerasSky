#pragma once

#include <opencv2/opencv.hpp>

// DlgBayesianClassifier 대화 상자입니다.

class CSKYDlg;
class DlgBayesianClassifier : public CDialogEx
{
	DECLARE_DYNAMIC(DlgBayesianClassifier)

public:
	DlgBayesianClassifier(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DlgBayesianClassifier();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_BAYESIAN_CLASSIFIER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


public:
	CSKYDlg*	m_pDlgSKYDlg;


	DECLARE_MESSAGE_MAP()
	void BayesianClassifierTest ();
public:

	afx_msg void OnBnClickedBtnAnalysis();

	CButton m_radioDefault;
	CButton m_radioScenario;
	virtual BOOL OnInitDialog();
};
