#pragma once

#include <opencv2/opencv.hpp>

// DlgBayesianClassifier ��ȭ �����Դϴ�.

class CSKYDlg;
class DlgBayesianClassifier : public CDialogEx
{
	DECLARE_DYNAMIC(DlgBayesianClassifier)

public:
	DlgBayesianClassifier(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~DlgBayesianClassifier();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_BAYESIAN_CLASSIFIER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.


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
