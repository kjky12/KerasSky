#pragma once

#include <opencv2/opencv.hpp>
#include "afxwin.h"

// DlgAnn 대화 상자입니다.

class CSKYDlg;
class DlgAnn : public CDialogEx
{
	DECLARE_DYNAMIC(DlgAnn)

public:
	DlgAnn(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DlgAnn();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_ANN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CSKYDlg*	m_pDlgSKYDlg;

	int FindMaxClassID (CvMat *response);
	void ANNTest ();

	void CopyMap(map<CString,vector<vector<float>>> mapData, map<CString,vector<vector<float>>> &mapDataTarget);

	afx_msg void OnBnClickedBtnAnalysis();
	CComboBox m_comboPossibleActivFunc;
	CEdit m_editMaxiter;
	CEdit m_editEpsilon;
	CComboBox m_comboTrainMethod;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();


	CButton m_radioDefault;
	CButton m_radioScenario;
	
	CEdit m_editTrainParam1;
	CEdit m_editTrainParam2;

	

};
