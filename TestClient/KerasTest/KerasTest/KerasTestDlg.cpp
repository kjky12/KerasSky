
// KerasTestDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "KerasTest.h"
#include "KerasTestDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CKerasTestDlg 대화 상자



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


// CKerasTestDlg 메시지 처리기

BOOL CKerasTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	::NetMngr_CreateManager();



	if (!m_pWndCommClient)
	{
		m_pWndCommClient = new CWndCommClientMng(this);
		m_pWndCommClient->Create(NULL, NULL,	WS_CHILD, CRect(0,0,0,0), this, 11100);
		//m_pWndCommClient->ConnectNetWork(L"192.168.0.9", 56789);
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CKerasTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CKerasTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CKerasTestDlg::OnBnClickedOk()
{
	
	CDialogEx::OnOK();
}

//! 전체 초기화
void CKerasTestDlg::OnBnClickedButton1()
{
	P_HEADER	pHeader;
	pHeader.nMessageSize = sizeof(P_HEADER);
	pHeader.nMessageID = df_MESS_PY_LEARNING_ALL_INIT;
	pHeader.nTemp = 0;

	char* psStructData = (char*)&pHeader;

	m_pWndCommClient->SendDevCtrlInfoMsg(psStructData, pHeader.nMessageSize);
}

//! 부분 초기화
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

//! 데이터 추가
void CKerasTestDlg::OnBnClickedButton3()
{
	ST_MESS_PY_LEARNING_ADD_DATA	stMessPyLearningAddData;
	stMessPyLearningAddData.pHeader.nMessageSize = sizeof(ST_MESS_PY_LEARNING_ADD_DATA);
	stMessPyLearningAddData.pHeader.nMessageID = df_MESS_PY_LEARNING_ADD_DATA;
	stMessPyLearningAddData.pHeader.nTemp = 0;

	stMessPyLearningAddData.nAddPoseData = 1;		//! 추가할 자세 데이터 갯수
	stMessPyLearningAddData.nPose = 2;				//! 추가할 자세
	stMessPyLearningAddData.nPoseDataCnt = 63;		//! 자세 갯수(ex : 63)
	
 	float fPoseData[1024] = {343.1102,355.7784,356.4846,344.1396,0.924377,355.3392,339.2221,358.3772,355.5045,339.2221,358.3772,355.5045,22.99709,40.60537,312.3546,328.4938,325.6841,270.7996,330.2419,325.7921,273.0175,330.2419,325.7921,273.0175,17.69855,304.8584,41.12686,328.6619,23.65369,92.75285,327.2409,23.43454,97.50562,327.2409,23.43455,97.50561,287.4767,5.51445,1.50843,358.9025,6.944602,0.4530426,359.2996,350.1308,0.08980922,359.2996,350.1308,0.08980922,278.8006,17.06667,357.8333,356.9378,14.94316,359.6681,1.753556,37.68499,0.5184861,1.753556,37.68499,0.5184861,346.608,5.423135,354.2506};
	memset(stMessPyLearningAddData.fPoseData, NULL, 1024);


	int nDataSize = stMessPyLearningAddData.nAddPoseData * stMessPyLearningAddData.nPoseDataCnt;
 	memcpy(stMessPyLearningAddData.fPoseData, fPoseData, nDataSize * sizeof(float));

	char* psStructData = (char*)&stMessPyLearningAddData;

	m_pWndCommClient->SendDevCtrlInfoMsg(psStructData, stMessPyLearningAddData.pHeader.nMessageSize - ((1024  - nDataSize) *sizeof(float)) );
}

//! 학습
void CKerasTestDlg::OnBnClickedButton4()
{
	P_HEADER	pHeader;
	pHeader.nMessageSize = sizeof(P_HEADER);
	pHeader.nMessageID = df_MESS_PY_LEARNING_LEARN;
	pHeader.nTemp = 0;

	char* psStructData = (char*)&pHeader;
	
	m_pWndCommClient->SendDevCtrlInfoMsg(psStructData, pHeader.nMessageSize);


}

//! 자세 개수확인
void CKerasTestDlg::OnBnClickedButton5()
{
	P_HEADER	pHeader;
	pHeader.nMessageSize = sizeof(P_HEADER);
	pHeader.nMessageID = df_MESS_PY_CHK_COUNT_POSE;
	pHeader.nTemp = 0;

	char* psStructData = (char*)&pHeader;

	m_pWndCommClient->SendDevCtrlInfoMsg(psStructData, pHeader.nMessageSize);
}

//! 현재 자세 확인
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
