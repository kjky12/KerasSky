
// KerasTestDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "KerasTest.h"
#include "KerasTestDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKerasTestDlg ��ȭ ����



CKerasTestDlg::CKerasTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKerasTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pWndCommClient = NULL;
}

void CKerasTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKerasTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CKerasTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CKerasTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CKerasTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CKerasTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CKerasTestDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CKerasTestDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CKerasTestDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CKerasTestDlg �޽��� ó����

BOOL CKerasTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	::NetMngr_CreateManager();



	if (!m_pWndCommClient)
	{
		m_pWndCommClient = new CWndCommClientMng(this);
		m_pWndCommClient->Create(NULL, NULL,	WS_CHILD, CRect(0,0,0,0), this, 11100);
		//m_pWndCommClient->ConnectNetWork(L"192.168.0.9", 56789);
	}

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}


void CKerasTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();


	M_DELETE(m_pWndCommClient);
	NetMngr_DeleteeManager();
}


void CKerasTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CKerasTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CKerasTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CKerasTestDlg::OnBnClickedOk()
{
	
	CDialogEx::OnOK();
}

//! ��ü �ʱ�ȭ
void CKerasTestDlg::OnBnClickedButton1()
{
	P_HEADER	pHeader;
	pHeader.nMessageSize = sizeof(P_HEADER);
	pHeader.nMessageID = df_MESS_PY_LEARNING_ALL_INIT;
	pHeader.nTemp = 0;

	char* psStructData = (char*)&pHeader;

	m_pWndCommClient->SendDevCtrlInfoMsg(psStructData, pHeader.nMessageSize);
}

//! �κ� �ʱ�ȭ
void CKerasTestDlg::OnBnClickedButton2()
{
	ST_MESS_PY_LEARNING_POSE_INIT	stMessPyLearningPoseInit;
	stMessPyLearningPoseInit.pHeader.nMessageSize = sizeof(ST_MESS_PY_LEARNING_POSE_INIT);
	stMessPyLearningPoseInit.pHeader.nMessageID = df_MESS_PY_LEARNING_POSE_INIT;
	stMessPyLearningPoseInit.pHeader.nTemp = 0;
	stMessPyLearningPoseInit.nPos = 2;

	char* psStructData = (char*)&stMessPyLearningPoseInit;

	m_pWndCommClient->SendDevCtrlInfoMsg(psStructData, stMessPyLearningPoseInit.pHeader.nMessageSize);

}

//! ������ �߰�
void CKerasTestDlg::OnBnClickedButton3()
{
	ST_MESS_PY_LEARNING_ADD_DATA	stMessPyLearningAddData;
	stMessPyLearningAddData.pHeader.nMessageSize = sizeof(ST_MESS_PY_LEARNING_ADD_DATA);
	stMessPyLearningAddData.pHeader.nMessageID = df_MESS_PY_LEARNING_ADD_DATA;
	stMessPyLearningAddData.pHeader.nTemp = 0;

	stMessPyLearningAddData.nAddPoseData = 1;		//! �߰��� �ڼ� ������ ����
	stMessPyLearningAddData.nPose = 2;				//! �߰��� �ڼ�
	stMessPyLearningAddData.nPoseDataCnt = 63;		//! �ڼ� ����(ex : 63)
	
 	float fPoseData[1024] = {343.1102,355.7784,356.4846,344.1396,0.924377,355.3392,339.2221,358.3772,355.5045,339.2221,358.3772,355.5045,22.99709,40.60537,312.3546,328.4938,325.6841,270.7996,330.2419,325.7921,273.0175,330.2419,325.7921,273.0175,17.69855,304.8584,41.12686,328.6619,23.65369,92.75285,327.2409,23.43454,97.50562,327.2409,23.43455,97.50561,287.4767,5.51445,1.50843,358.9025,6.944602,0.4530426,359.2996,350.1308,0.08980922,359.2996,350.1308,0.08980922,278.8006,17.06667,357.8333,356.9378,14.94316,359.6681,1.753556,37.68499,0.5184861,1.753556,37.68499,0.5184861,346.608,5.423135,354.2506};
	memset(stMessPyLearningAddData.fPoseData, NULL, 1024);


	int nDataSize = stMessPyLearningAddData.nAddPoseData * stMessPyLearningAddData.nPoseDataCnt;
 	memcpy(stMessPyLearningAddData.fPoseData, fPoseData, nDataSize * sizeof(float));

	char* psStructData = (char*)&stMessPyLearningAddData;

	m_pWndCommClient->SendDevCtrlInfoMsg(psStructData, stMessPyLearningAddData.pHeader.nMessageSize - ((1024  - nDataSize) *sizeof(float)) );
}

//! �н�
void CKerasTestDlg::OnBnClickedButton4()
{
	P_HEADER	pHeader;
	pHeader.nMessageSize = sizeof(P_HEADER);
	pHeader.nMessageID = df_MESS_PY_LEARNING_LEARN;
	pHeader.nTemp = 0;

	char* psStructData = (char*)&pHeader;
	
	m_pWndCommClient->SendDevCtrlInfoMsg(psStructData, pHeader.nMessageSize);


}

//! �ڼ� ����Ȯ��
void CKerasTestDlg::OnBnClickedButton5()
{
	P_HEADER	pHeader;
	pHeader.nMessageSize = sizeof(P_HEADER);
	pHeader.nMessageID = df_MESS_PY_CHK_COUNT_POSE;
	pHeader.nTemp = 0;

	char* psStructData = (char*)&pHeader;

	m_pWndCommClient->SendDevCtrlInfoMsg(psStructData, pHeader.nMessageSize);
}

//! ���� �ڼ� Ȯ��
void CKerasTestDlg::OnBnClickedButton6()
{
	ST_MESS_PY_CHK_POSE	stMessPyChkPose;

	stMessPyChkPose.pHeader.nMessageSize = sizeof(ST_MESS_PY_CHK_POSE);
	stMessPyChkPose.pHeader.nMessageID = df_MESS_PY_CHK_POSE;
	stMessPyChkPose.pHeader.nTemp = 0;

	stMessPyChkPose.nPoseDataCnt = 63;
 	float fPoseData[1024] = {343.1102,355.7784,356.4846,344.1396,0.924377,355.3392,339.2221,358.3772,355.5045,339.2221,358.3772,355.5045,22.99709,40.60537,312.3546,328.4938,325.6841,270.7996,330.2419,325.7921,273.0175,330.2419,325.7921,273.0175,17.69855,304.8584,41.12686,328.6619,23.65369,92.75285,327.2409,23.43454,97.50562,327.2409,23.43455,97.50561,287.4767,5.51445,1.50843,358.9025,6.944602,0.4530426,359.2996,350.1308,0.08980922,359.2996,350.1308,0.08980922,278.8006,17.06667,357.8333,356.9378,14.94316,359.6681,1.753556,37.68499,0.5184861,1.753556,37.68499,0.5184861,346.608,5.423135,354.2506};

	memset(stMessPyChkPose.fPoseData, NULL, 1024);
	int nDataSize = stMessPyChkPose.nPoseDataCnt;
	memcpy(stMessPyChkPose.fPoseData, fPoseData, nDataSize * sizeof(float));

	char* psStructData = (char*)&stMessPyChkPose;
	m_pWndCommClient->SendDevCtrlInfoMsg(psStructData, stMessPyChkPose.pHeader.nMessageSize - ((1024  - nDataSize) *sizeof(float)) );

}
