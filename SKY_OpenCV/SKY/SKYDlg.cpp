
// SKYDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "SKY.h"
#include "SKYDlg.h"
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


// CSKYDlg 대화 상자



CSKYDlg::CSKYDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSKYDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pDlgAnn = NULL;
	m_pDlgBayesianClassifier = NULL;
	m_pDlgBoosting = NULL;
	m_pDlgKnn = NULL;

	m_nLogCnt = 0;

}

void CSKYDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALL, m_listAll);
	DDX_Control(pDX, IDC_LIST_TRAIN, m_listTrain);
	DDX_Control(pDX, IDC_LIST_TEST, m_listTest);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Control(pDX, IDC_LIST_SCENARIO, m_ListScenarioInfo);
	DDX_Control(pDX, IDC_RICHEDIT21, m_richLogInfo);
}

BEGIN_MESSAGE_MAP(CSKYDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSKYDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSKYDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CSKYDlg::OnBnClickedBtnRefresh)
	ON_BN_CLICKED(IDC_BTN_TRAIN_PLUS, &CSKYDlg::OnBnClickedBtnTrainPlus)
	ON_BN_CLICKED(IDC_BTN_TRAIN_MINUS, &CSKYDlg::OnBnClickedBtnTrainMinus)
	ON_BN_CLICKED(IDC_BTN_TEST_PLUS, &CSKYDlg::OnBnClickedBtnTestPlus)
	ON_BN_CLICKED(IDC_BTN_TEST_MINUS, &CSKYDlg::OnBnClickedBtnTestMinus)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CSKYDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON_SCENARIO_SAVE, &CSKYDlg::OnBnClickedButtonScenarioSave)
	ON_BN_CLICKED(IDC_BUTTON_SCENARIO_CHK, &CSKYDlg::OnBnClickedButtonScenarioChk)
	ON_MESSAGE(WM_USER + 888, &CSKYDlg::AlgorithmMess)
	ON_BN_CLICKED(IDC_BUTTON_SCENARIO_DEL, &CSKYDlg::OnBnClickedButtonScenarioDel)
END_MESSAGE_MAP()


// CSKYDlg 메시지 처리기

BOOL CSKYDlg::OnInitDialog()
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


	g_mapData.clear();

	InitTap();
	OnBnClickedBtnRefresh();


	//! 선택 정보 리스트
	CRect rcClient;
	m_ListScenarioInfo.GetClientRect(&rcClient);
	ListView_SetExtendedListViewStyle(m_ListScenarioInfo.m_hWnd, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListScenarioInfo.InsertColumn(0, L"학습 데이터", LVCFMT_CENTER, (rcClient.Width()/2) - 50);		
	m_ListScenarioInfo.InsertColumn(1, L"인식 데이터", LVCFMT_CENTER, (rcClient.Width()/2) - 50);		
	m_ListScenarioInfo.InsertColumn(2, L"인식률(%)", LVCFMT_CENTER, 80);		



	//  [6/27/2008/peter] LOG INFO FONT
	CFont ftREdit;
	ftREdit.CreateFont(12, 0,0,0,FW_BOLD , 0,0,0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, 
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,_T("굴림체"));//MS Gothic"));

	m_richLogInfo.SetFont(&ftREdit, FALSE);



	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSKYDlg::DispLogInfo(CString strMsg)
{

	COleDateTime dt=COleDateTime::GetCurrentTime();

	CString strLogTime;

	if(m_nLogCnt != 0)
		strMsg= L"\n" + strMsg;

	long g_nStartChar = -1;
	long g_nEndChar = -1;

	g_nStartChar = m_richLogInfo.GetTextLength();
	m_richLogInfo.SetSel(g_nStartChar, -1);
	m_richLogInfo.ReplaceSel(strMsg);

	int nTotalLine = m_richLogInfo.GetLineCount();
	int nVisibleLine = m_richLogInfo.GetFirstVisibleLine(); 

	int nLowerLine = nTotalLine - nVisibleLine;
	int nScroll;

	nScroll = nLowerLine - 8;
	m_richLogInfo.LineScroll(nScroll);

	m_nLogCnt++;

	RefreshMessage();
}




void  CSKYDlg::DispListScenario()
{
	CString strTmp = L"";
	CString strTemp = L"";
	m_ListScenarioInfo.DeleteAllItems();

	for(int i = 0;i < m_vecScenario.size(); i++)
	{
		set<CString>::iterator	iterTemp;
		ST_SCENARIO		stScenario = m_vecScenario[i];
		strTemp = L"";
		for(iterTemp= stScenario.setTrain.begin(); iterTemp != stScenario.setTrain.end(); iterTemp++)
			strTemp += *iterTemp + L", ";
		strTemp.Delete(strTemp.GetLength() - 2, 2);
		m_ListScenarioInfo.InsertItem(i,strTemp);

		strTemp = L"";
		for(iterTemp = stScenario.setTest.begin(); iterTemp != stScenario.setTest.end(); iterTemp++)
			strTemp += *iterTemp  + L", ";
		strTemp.Delete(strTemp.GetLength() - 2, 2);
		m_ListScenarioInfo.SetItemText(i,1,strTemp);


		if(m_vecScenarioRate.size() == m_vecScenario.size())
		{
			if(m_vecScenarioRate.size() > i)
			{
				strTemp.Format(L"%.02f%%",m_vecScenarioRate[i]);
				m_ListScenarioInfo.SetItemText(i,2,strTemp);
			}
		}
	}

	RefreshMessage();
}



void CSKYDlg::OnBnClickedButtonScenarioDel()
{
	//참고
	if(m_ListScenarioInfo.GetSelectedCount() == 0)
	{
		AfxMessageBox(L"선택된 시나리오가 없습니다.");
		return;
	}

	int nDeleteCnt = 0;

	POSITION Position = m_ListScenarioInfo.GetFirstSelectedItemPosition();

	set<int> setSelect;

	set<int>::iterator iterSelect;

	for(int n = 0 ; n < (int)m_ListScenarioInfo.GetSelectedCount() ; n++)
	{
		int nItem = m_ListScenarioInfo.GetNextSelectedItem(Position);

		setSelect.insert(nItem);

		nDeleteCnt++;
	}

	vector<ST_SCENARIO>		vecScenarioTmp;
	vecScenarioTmp.clear();
	for(int v = 0 ; v < (int)m_vecScenario.size() ; v++)
	{
		iterSelect = setSelect.find(v);
		if(iterSelect == setSelect.end())
		{
			vecScenarioTmp.push_back(m_vecScenario[v]);
		}
	}

	m_vecScenario = vecScenarioTmp;
	m_vecScenarioTmp = m_vecScenario;

	DispListScenario();

	CString strTmp = L"";
	strTmp.Format(L" %d건을 삭제했습니다.", nDeleteCnt);
	AfxMessageBox(strTmp);
}



void CSKYDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	M_W_DELETE(m_pDlgAnn);
	M_W_DELETE(m_pDlgBayesianClassifier);
	M_W_DELETE(m_pDlgBoosting);
	M_W_DELETE(m_pDlgKnn);
}

void CSKYDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSKYDlg::OnPaint()
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
HCURSOR CSKYDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSKYDlg::OnBnClickedOk()
{
	return ; 
	CDialogEx::OnOK();
}


void CSKYDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CSKYDlg::RefreshMessage()
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	while (GetTickCount() - dwStart < 3/*mseconds*/)
	{
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}


int CSKYDlg::ReadDirectory()
{
	m_setFolderAll.clear();
	m_setFolderTrain.clear();
	m_setFolderTest.clear();

	m_listAll.DeleteAllItems();
	m_listTrain.DeleteAllItems();
	m_listTest.DeleteAllItems();
	

	CString strFileName = L"";
	CString strFile = L"";
	CFileFind File;

	strFile = gf_GetModulePath() + L"Data\\*.*";

	BOOL bIsFind = File.FindFile( strFile);

	while (bIsFind)
	{
		bIsFind = File.FindNextFileW();

		if(File.IsDirectory())
		{
			strFileName = File.GetFileName();


			// "."과 ".." 폴더는 무시한다.
			if(strFileName.Find(L".") > -1 || strFileName.Find(L"..") > -1)
				continue;

			m_listAll.InsertItem(0,strFileName);
			m_setFolderAll.insert(strFileName);
		}
	}

	

	return 0;//vecDataFolder.size();

}

void CSKYDlg::InitTap()
{

	// tab 초기화
 
 	m_Tab.InsertItem(0, L"ANN",0);
	m_Tab.InsertItem(1, L"Bayesian Classifier",0);
	m_Tab.InsertItem(2, L"KNN",0);
//	m_Tab.InsertItem(3, L"Boosting",0);

 
 	CRect    Rect;
 	m_Tab.GetClientRect(&Rect);
 
 	//DlgAnn
 	m_pDlgAnn = new DlgAnn(this);
 	m_pDlgAnn->Create(IDD_DLG_ANN, &m_Tab);
 	m_pDlgAnn->SetWindowPos(NULL, dfTAB_SIZEX_START_GAP, dfTAB_SIZEY_START_GAP, Rect.Width() - dfTAB_SIZEX_END_GAP, Rect.Height() - dfTAB_SIZEY_END_GAP, 
 		SWP_SHOWWINDOW | SWP_NOZORDER);
 	m_pDlgAnn->ShowWindow(SW_SHOW);


	//DlgBayesianClassifier
	m_pDlgBayesianClassifier = new DlgBayesianClassifier(this);
	m_pDlgBayesianClassifier->Create(IDD_DLG_BAYESIAN_CLASSIFIER, &m_Tab);
	m_pDlgBayesianClassifier->SetWindowPos(NULL, dfTAB_SIZEX_START_GAP, dfTAB_SIZEY_START_GAP, Rect.Width() - dfTAB_SIZEX_END_GAP, Rect.Height() - dfTAB_SIZEY_END_GAP, 
		SWP_SHOWWINDOW | SWP_NOZORDER);
	m_pDlgBayesianClassifier->ShowWindow(SW_HIDE);

	//DlgKnn
	m_pDlgKnn = new DlgKnn(this);
	m_pDlgKnn->Create(IDD_DLG_KNN, &m_Tab);
	m_pDlgKnn->SetWindowPos(NULL, dfTAB_SIZEX_START_GAP, dfTAB_SIZEY_START_GAP, Rect.Width() - dfTAB_SIZEX_END_GAP, Rect.Height() - dfTAB_SIZEY_END_GAP, 
		SWP_SHOWWINDOW | SWP_NOZORDER);
	m_pDlgKnn->ShowWindow(SW_HIDE);

	//DlgBoosting
// 	m_pDlgBoosting = new DlgBoosting(this);
// 	m_pDlgBoosting->Create(IDD_DLG_BOOSTING, &m_Tab);
// 	m_pDlgBoosting->SetWindowPos(NULL, dfTAB_SIZEX_START_GAP, dfTAB_SIZEY_START_GAP, Rect.Width() - dfTAB_SIZEX_END_GAP, Rect.Height() - dfTAB_SIZEY_END_GAP, 
// 		SWP_SHOWWINDOW | SWP_NOZORDER);
// 	m_pDlgBoosting->ShowWindow(SW_HIDE);
}


void CSKYDlg::OnBnClickedBtnRefresh()
{
	ReadDirectory();
}


void CSKYDlg::OnBnClickedBtnTrainPlus()
{
	POSITION Position = m_listAll.GetFirstSelectedItemPosition();

	for(int n = 0 ; n < m_listAll.GetSelectedCount() ; n++)
	{
		int nItem = m_listAll.GetNextSelectedItem(Position);
		CString strTmp = m_listAll.GetItemText(nItem,0);

		m_setFolderTrain.insert(strTmp);
	}
	RefreshList(E_TRAIN);
}


void CSKYDlg::OnBnClickedBtnTrainMinus()
{
	POSITION Position = m_listTrain.GetFirstSelectedItemPosition();

	for(int n = 0 ; n < m_listTrain.GetSelectedCount() ; n++)
	{
		int nItem = m_listTrain.GetNextSelectedItem(Position);
		CString strTmp = m_listTrain.GetItemText(nItem,0);

		m_setFolderTrain.erase(strTmp);
	}
	RefreshList(E_TRAIN);
}


void CSKYDlg::OnBnClickedBtnTestPlus()
{
	POSITION Position = m_listAll.GetFirstSelectedItemPosition();

	for(int n = 0 ; n < m_listAll.GetSelectedCount() ; n++)
	{
		int nItem = m_listAll.GetNextSelectedItem(Position);
		CString strTmp = m_listAll.GetItemText(nItem,0);

		m_setFolderTest.insert(strTmp);
	}
	RefreshList(E_TEST);
}


void CSKYDlg::OnBnClickedBtnTestMinus()
{
	POSITION Position = m_listTest.GetFirstSelectedItemPosition();

	for(int n = 0 ; n < m_listTest.GetSelectedCount() ; n++)
	{
		int nItem = m_listTest.GetNextSelectedItem(Position);
		CString strTmp = m_listTest.GetItemText(nItem,0);

		m_setFolderTest.erase(strTmp);
	}
	RefreshList(E_TEST);
}


void CSKYDlg::RefreshList(E_TYPE eType)
{
	set<CString>::iterator iterTmp ;

	if(eType == E_TRAIN || eType == E_DEFAULT)
	{
		m_listTrain.DeleteAllItems();
		for(iterTmp = m_setFolderTrain.begin(); iterTmp != m_setFolderTrain.end(); iterTmp++)
		{
			m_listTrain.InsertItem(0, *iterTmp);
		}
	}

	if(eType == E_TEST || eType == E_DEFAULT)
	{
		m_listTest.DeleteAllItems();
		for(iterTmp = m_setFolderTest.begin(); iterTmp != m_setFolderTest.end(); iterTmp++)
		{
			m_listTest.InsertItem(0, *iterTmp);
		}
	}
}



void CSKYDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	//! 탭 컨트롤
	int nIndex = m_Tab.GetCurSel();

	m_pDlgAnn->ShowWindow(SW_HIDE);
	m_pDlgBayesianClassifier->ShowWindow(SW_HIDE);
//	m_pDlgBoosting->ShowWindow(SW_HIDE);
	m_pDlgKnn->ShowWindow(SW_HIDE);

	switch (nIndex)
	{
	case 0:
		m_pDlgAnn->ShowWindow(SW_SHOW);
		break;
	case 1:
		m_pDlgBayesianClassifier->ShowWindow(SW_SHOW);
		break;
	case 2:
		m_pDlgKnn->ShowWindow(SW_SHOW);
		break;
	case 3:
		m_pDlgBoosting->ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}

	*pResult = 0;
}


void CSKYDlg::OnBnClickedButtonScenarioSave()
{
	
	int nListCnt = m_listTest.GetItemCount();
	CString strTemp = L"";
	
	ST_SCENARIO		stScenario;
	for(int i = 0; i < nListCnt; i++)
	{
		strTemp = m_listTest.GetItemText(i,0);
		stScenario.setTest.insert(strTemp);
	}

	nListCnt = m_listTrain.GetItemCount();
	for(int i = 0; i < nListCnt; i++)
	{
		strTemp = m_listTrain.GetItemText(i,0);
		stScenario.setTrain.insert(strTemp);
	}

	m_vecScenario.push_back(stScenario);

	m_vecScenarioTmp = m_vecScenario;

	m_setFolderTest.clear();
	m_setFolderTrain.clear();

	m_vecScenarioRate.clear();

	RefreshList(E_DEFAULT);

	DispListScenario();
}


void CSKYDlg::OnBnClickedButtonScenarioChk()
{
	CString strTmp = L"";
	CString strTemp = L"";
	for(int i = 0;i < m_vecScenario.size(); i++)
	{
		set<CString>::iterator	iterTemp;
		strTemp.Format(L"시나리오 - %d\n", i);
		ST_SCENARIO		stScenario = m_vecScenario[i];
		strTemp += L"Test  : ";
		for(iterTemp = stScenario.setTest.begin(); iterTemp != stScenario.setTest.end(); iterTemp++)
			strTemp += *iterTemp  + L", ";
		strTemp.Delete(strTemp.GetLength() - 2, 2);
		strTemp += L"\n";

		strTemp += L"Train : ";
		for(iterTemp= stScenario.setTrain.begin(); iterTemp != stScenario.setTrain.end(); iterTemp++)
			strTemp += *iterTemp + L", ";
		strTemp.Delete(strTemp.GetLength() - 2, 2);
		strTemp += L"\n\n";
		
		strTmp += strTemp;
	}

	AfxMessageBox(strTmp);
}


LRESULT CSKYDlg::AlgorithmMess(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case dfALGORITHM_PATTERN_ANN :
		{
			m_pDlgAnn->OnBnClickedBtnAnalysis();
		}
		break;
	case dfALGORITHM_PATTERN_BAYESIAN_CLASSIFER :
		{
			m_pDlgBayesianClassifier->OnBnClickedBtnAnalysis();
		}
		break;
	case dfALGORITHM_PATTERN_BOOSTING :
		{
			m_pDlgBoosting->OnBnClickedBtnAnalysis();
		}
		break;
	case dfALGORITHM_PATTERN_KNN :
		{
			m_pDlgKnn->OnBnClickedBtnAnalysis();
		}
		break;

	default:
		break;
	}

	return TRUE;
}
