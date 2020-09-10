#pragma once

#include <opencv2/opencv.hpp>

// DlgBoosting ��ȭ �����Դϴ�.

class CSKYDlg;
class DlgBoosting : public CDialogEx
{
	DECLARE_DYNAMIC(DlgBoosting)

public:
	DlgBoosting(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~DlgBoosting();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_BOOSTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.


public:
	CSKYDlg*	m_pDlgSKYDlg;


	DECLARE_MESSAGE_MAP()
	void BoostingTest();
public:
	afx_msg void OnBnClickedBtnAnalysis();

	CButton m_radioDefault;
	CButton m_radioScenario;
	virtual BOOL OnInitDialog();
};
