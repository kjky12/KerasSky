
// KerasTestDlg.h : ��� ����
//

#pragma once


#include "WndCommClientMng.h"



//�н� ���
#define df_MESS_PY_LEARNING_ALL_INIT             1001  //�н� ��� - ��ü �ʱ�ȭ
#define df_MESS_PY_LEARNING_POSE_INIT            1002  //�н� ��� - �κ� �ʱ�ȭ
#define df_MESS_PY_LEARNING_ADD_DATA             1003  //�н� ��� - ������ �߰�
#define df_MESS_PY_LEARNING_LEARN                1004  //�н� ��� - �н�

//�ν� ���
#define df_MESS_PY_CHK_COUNT_POSE                2001  //�ν� ��� - �ڼ� ���� Ȯ��
#define df_MESS_PY_CHK_POSE                      2002  //�ν� ��� - ���� �ڼ� Ȯ��

#define df_MESS_PY_RECV_ERROR   			 	 1200	//���� ����
#define PY_FAIL_DATA		                	 -9991	//Deep Lerning Fail



//! ����� ����(MessageID)
//�н� ��� - ��ü �ʱ�ȭ	
//�н� ��� - �н�
//�ν� ��� - �ڼ� ���� Ȯ��

typedef struct P_HEADE
{
	int			nMessageSize;
	int			nMessageID;
	int			nTemp;
}P_HEADER;


typedef struct ST_MESS_PY_LEARNING_POSE_INIT	//�н� ��� - �κ� �ʱ�ȭ
{
	P_HEADER	pHeader;
	int			nPos;	//������ �ڼ� ��ȣ
}ST_MESS_PY_LEARNING_POSE_INIT;

typedef struct ST_MESS_PY_LEARNING_ADD_DATA		//�н� ��� - ������ �߰�
{
	P_HEADER	pHeader;
	int			nAddPoseData;		//! �߰��� �ڼ� ������ ����
	int			nPose;				//! �߰��� �ڼ�
	int			nPoseDataCnt;		//! �ڼ� ����(ex : 63)
	float		fPoseData[1024];	//! �ڼ� ������
}ST_MESS_PY_LEARNING_ADD_DATA;

typedef struct ST_MESS_PY_CHK_POSE				//�ν� ��� - ���� �ڼ� Ȯ��
{
	P_HEADER	pHeader;
	int			nPoseDataCnt;		//! �ڼ� ����(ex : 63)
	float		fPoseData[1024];		//! �ڼ� ������
}ST_MESS_PY_CHK_POSE;


// CKerasTestDlg ��ȭ ����
class CKerasTestDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CKerasTestDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_KERASTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


	CWndCommClientMng*		m_pWndCommClient;



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
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
