// DlgAnn.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SKY.h"
#include "DlgAnn.h"
#include "afxdialogex.h"

#include "SKYDlg.h"
static const int MAX_CLASS = 7;	// 클래스 수


#include <sys/timeb.h>

// DlgAnn 대화 상자입니다.

IMPLEMENT_DYNAMIC(DlgAnn, CDialogEx)

DlgAnn::DlgAnn(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgAnn::IDD, pParent)
{
	m_pDlgSKYDlg = (CSKYDlg*)pParent;
}

DlgAnn::~DlgAnn()
{
}

void DlgAnn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_POSSIBLE_ACTIV_FUNC, m_comboPossibleActivFunc);
	DDX_Control(pDX, IDC_EDIT_MAX_ITER, m_editMaxiter);
	DDX_Control(pDX, IDC_EDIT_EPSILON, m_editEpsilon);
	DDX_Control(pDX, IDC_COMBO_TRAIN_METHOD, m_comboTrainMethod);
	DDX_Control(pDX, IDC_RADIO_DEFAULT, m_radioDefault);
	DDX_Control(pDX, IDC_RADIO_SCENARIO, m_radioScenario);
	DDX_Control(pDX, IDC_EDIT_TRAIN_PARAM1, m_editTrainParam1);
	DDX_Control(pDX, IDC_EDIT_TRAIN_PARAM2, m_editTrainParam2);
}


BEGIN_MESSAGE_MAP(DlgAnn, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ANALYSIS, &DlgAnn::OnBnClickedBtnAnalysis)
	ON_BN_CLICKED(IDOK, &DlgAnn::OnBnClickedOk)
END_MESSAGE_MAP()




BOOL DlgAnn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//! 기존 입력되있던 값을 디폴트로 한다.

	m_comboPossibleActivFunc.AddString(L"IDENTITY");
	m_comboPossibleActivFunc.AddString(L"SIGMOID_SYM");
	m_comboPossibleActivFunc.AddString(L"GAUSSIAN");
	m_comboPossibleActivFunc.SetCurSel(2);

	m_editMaxiter.SetWindowTextW(L"5000");

	m_editEpsilon.SetWindowTextW(L"0.001");

	m_comboTrainMethod.AddString(L"BACKPROP");
	m_comboTrainMethod.AddString(L"RPROP");
	m_comboTrainMethod.SetCurSel(0);

	m_radioDefault.SetCheck(TRUE);

	m_editTrainParam1.SetWindowTextW(L"0.03");
	m_editTrainParam2.SetWindowTextW(L"0.03");



	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



// DlgAnn 메시지 처리기입니다.

int DlgAnn::FindMaxClassID (CvMat *response)
{
	int max_c = -1;
	float max_v = -1;

	for (int i=0; i<MAX_CLASS; ++i) {
		float v = CV_MAT_ELEM(*response, float, 0, i);
		if (max_v < v) {
			max_v = v;
			max_c = i;
		}
	}
	return max_c;
}




void DlgAnn::ANNTest ()
{
	map<CString,vector<vector<float>>> mapTranData;
	map<CString,vector<vector<float>>> mapTestData;

	map<CString,vector<vector<float>>>::iterator iterData;

	// 학습 데이터의 총 수

	int data_count = 0;
	gf_ReadDataFile (mapTestData,data_count);
	const int sample_count = gf_ReadDataFile (mapTranData,data_count);

	// 학습 데이터와 클래스를 할당할 행렬 생성
	CvMat *train_data  = cvCreateMat (sample_count, data_count, CV_32FC1);
	CvMat *train_class = cvCreateMat (sample_count, MAX_CLASS, CV_32FC1);
	CvMat *layer_size  = cvCreateMat (1, 3, CV_32SC1);
	cvZero (train_class);

	CV_MAT_ELEM(*layer_size, long, 0,0) = data_count;
	CV_MAT_ELEM(*layer_size, long, 0,1) = 12; // 히든 레이어 입력 필요
	CV_MAT_ELEM(*layer_size, long, 0,2) = MAX_CLASS;

	vector<float> vecData;
	vector<vector<float>> vecLineData;

	// 무작위로 학습 데이터를 생성 후 영역 별로 클래스를 할당

	printf("학습데이터 입력 시작\n");
	int k=0;
	for(iterData = mapTranData.begin() ; iterData != mapTranData.end() ; iterData++)
	{
		vecLineData = iterData->second;
		for(int vl = 0 ; vl < vecLineData.size() ; vl++)
		{
			vecData = vecLineData[vl];
			for(int v = 0 ; v < vecData.size() ; v++)
			{
				CV_MAT_ELEM(*train_data, float,k,v) = vecData[v];
			}
			CV_MAT_ELEM(*train_class,float,k,_wtoi(iterData->first)) = 1.f;
			k++;
		}
	}

	printf("학습데이터 입력 종료\n");

	printf("학습 시작\n"); // 시작 시간

	CvANN_MLP bpn(layer_size, CvANN_MLP::GAUSSIAN, 1, 1);

	CvTermCriteria term = cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS,5000,0.001);
	CvANN_MLP_TrainParams train_params = CvANN_MLP_TrainParams( term, CvANN_MLP_TrainParams::BACKPROP, 0.3, 0.3 );

	bpn.train (train_data, train_class, NULL, 0, train_params);

	printf("학습 종료\n"); // 종료 시간 // 시간차!! 

	CvMat *sample  = cvCreateMat (1, data_count, CV_32FC1);;
	CvMat *response = cvCreateMat (1, MAX_CLASS, CV_32FC1);

	// 이미지의 모든 픽셀에 대해 각 픽셀이 어느 클래스에 속하는지 추정하여 클래스를 할당한다.

	printf("인식 시작\n");
	int nTotalPredictCnt = 0;
	int nOKPredictCnt = 0;
	for(iterData = mapTestData.begin() ; iterData != mapTestData.end() ; iterData++)
	{
		vecLineData = iterData->second;
		for(int vl = 0 ; vl < vecLineData.size() ; vl++)
		{
			vecData = vecLineData[vl];
			for(int v = 0 ; v < vecData.size() ; v++)
			{
				CV_MAT_ELEM(*sample, float, 0, v) = vecData[v];

			}
			bpn.predict (sample, response);
			int c = FindMaxClassID (response);

			nTotalPredictCnt++;

			if(c == _wtoi(iterData->first))
				nOKPredictCnt++;
		}
	}
	printf("인식 종료\n");

	printf("테스트 데이터 수 : %d\n",nTotalPredictCnt);
	printf("인식 성공 수 : %d\n",nOKPredictCnt);
	printf("인식률 : %f\n",float((float)nOKPredictCnt / (float)nTotalPredictCnt) * 100.0);

	cvReleaseMat (&sample);
	cvReleaseMat (&response);

	cvReleaseMat (&train_class);
	cvReleaseMat (&train_data);
	cvReleaseMat (&layer_size);


	// 키를 누르면 종료
	cvWaitKey (0);



}


void DlgAnn::OnBnClickedBtnAnalysis()
{
	//! 시나리오일 경우에 vector에서 받는다.
	if(m_radioScenario.GetCheck())
	{
		if(m_pDlgSKYDlg->m_vecScenarioTmp.size() == m_pDlgSKYDlg->m_vecScenario.size())
			m_pDlgSKYDlg->m_vecScenarioRate.clear();

		if(m_pDlgSKYDlg->m_vecScenario.size() > 0) 
		{
			ST_SCENARIO	stScenario = m_pDlgSKYDlg->m_vecScenario[0];
			m_pDlgSKYDlg->m_setFolderTest = stScenario.setTest;
			m_pDlgSKYDlg->m_setFolderTrain = stScenario.setTrain;

			m_pDlgSKYDlg->m_vecScenario.erase(m_pDlgSKYDlg->m_vecScenario.begin());
		}
		else
		{
			AfxMessageBox(L"등록된 시나리오가 없습니다.");
			return;
		}
	}
	
	

	struct _timeb dLogTime;
	struct tm tStart, tEnd;
	setlocale(LC_ALL,"Korean");

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	map<CString,vector<vector<float>>> mapTranData;
	map<CString,vector<vector<float>>> mapTranDataTmp;
	map<CString,vector<vector<float>>> mapTestData;
	map<CString,vector<vector<float>>> mapTestDataTmp;

	map<CString,vector<vector<float>>>::iterator iterData;
	map<CString,vector<vector<float>>>::iterator iterDataTmp;

	//////////////////////////////////////////////////////////////////////////
	//! 선택된 폴더
	set<CString>::iterator iterFolder;
	set<CString> setFolderTrain = m_pDlgSKYDlg->m_setFolderTrain;
	set<CString> setFolderTest = m_pDlgSKYDlg->m_setFolderTest;

	//////////////////////////////////////////////////////////////////////////

	m_pDlgSKYDlg->DispLogInfo(L"인식 데이터 로드");

	//테스트 파일 읽기
	int data_count = 0;
	for(iterFolder = setFolderTest.begin(); iterFolder != setFolderTest.end(); iterFolder++)
	{

		m_pDlgSKYDlg->DispLogInfo(*iterFolder + L"폴더 데이터 로드중");

		gf_ReadDataFileName (*iterFolder,mapTestDataTmp,data_count);

		//! 글로벌 변수에 넣어준다.
		g_mapData.insert(make_pair(*iterFolder,mapTestDataTmp));

		for(iterDataTmp = mapTestDataTmp.begin(); iterDataTmp != mapTestDataTmp.end(); iterDataTmp++)
		{
			iterData = mapTestData.find(iterDataTmp->first);
			if(iterData != mapTestData.end()) //있으면
			{
				for(int i = 0; i < iterDataTmp->second.size(); i++)
					iterData->second.push_back(iterDataTmp->second[i]);
			}
			else //없으면
				mapTestData.insert(make_pair(iterDataTmp->first, iterDataTmp->second));
		}
	}

	//////////////////////////////////////////////////////////////////////////

	m_pDlgSKYDlg->DispLogInfo(L"학습 데이터 로드");

	//트레인 파일 읽기
	for(iterFolder = setFolderTrain.begin(); iterFolder != setFolderTrain.end(); iterFolder++)
	{
		m_pDlgSKYDlg->DispLogInfo(*iterFolder + L"폴더 데이터 로드중");

		gf_ReadDataFileName (*iterFolder, mapTranDataTmp,data_count);

		//! 글로벌 변수에 넣어준다.
		g_mapData.insert(make_pair(*iterFolder,mapTranDataTmp));

		for(iterDataTmp = mapTranDataTmp.begin(); iterDataTmp != mapTranDataTmp.end(); iterDataTmp++)
		{
			iterData = mapTranData.find(iterDataTmp->first);
			if(iterData != mapTranData.end()) //있으면
			{
				for(int i = 0; i < iterDataTmp->second.size(); i++)
					iterData->second.push_back(iterDataTmp->second[i]);
			}
			else //없으면
				mapTranData.insert(make_pair(iterDataTmp->first, iterDataTmp->second));
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if(mapTestData.size() <= 0 || mapTranData.size() <= 0)
	{
		AfxMessageBox(L"설정된 데이터가 없습니다.학습/인식 디렉토리를 확인하세요.");
		return;
	}


	int sample_count = 0;
	for(iterData = mapTranData.begin() ; iterData != mapTranData.end() ; iterData++)
		 sample_count += iterData->second.size();

	iterData = mapTranData.begin();
	const int nDataCount = iterData->second[0].size();


	// 학습 데이터와 클래스를 할당할 행렬 생성
	CvMat *train_data  = cvCreateMat (sample_count, nDataCount, CV_32FC1);
	CvMat *train_class = cvCreateMat (sample_count, MAX_CLASS, CV_32FC1);
	CvMat *layer_size  = cvCreateMat (1, 3, CV_32SC1);
	cvZero (train_class);

	CV_MAT_ELEM(*layer_size, long, 0,0) = nDataCount;
	CV_MAT_ELEM(*layer_size, long, 0,1) = 12; // 히든 레이어 입력 필요
	CV_MAT_ELEM(*layer_size, long, 0,2) = MAX_CLASS;

	vector<float> vecData;
	vector<vector<float>> vecLineData;

	// 무작위로 학습 데이터를 생성 후 영역 별로 클래스를 할당

	m_pDlgSKYDlg->DispLogInfo(L"학습데이터 입력 시작");
	int k=0;
	for(iterData = mapTranData.begin() ; iterData != mapTranData.end() ; iterData++)
	{
		vecLineData = iterData->second;
		for(int vl = 0 ; vl < vecLineData.size() ; vl++)
		{
			vecData = vecLineData[vl];
			for(int v = 0 ; v < vecData.size() ; v++)
			{
				CV_MAT_ELEM(*train_data, float,k,v) = vecData[v];
			}
			CV_MAT_ELEM(*train_class,float,k,_wtoi(iterData->first)) = 1.f;
			k++;
		}
	}

	m_pDlgSKYDlg->DispLogInfo(L"학습데이터 입력 종료");

	m_pDlgSKYDlg->DispLogInfo(L"학습 시작"); // 시작 시간
	CString  strStartTime = L"", strEndTime = L"";

	//////////////////////////////////////////////////////////////////////////
	// Train에 필요한 값 넣기

	int nPossibleActivFunc = 0;
	int nMaxiter = 0;
	double dEpsilons = 0.0;
	int nTrainMethod = 0;
	CString strTmp;

	double dTrainParam1 = 0.0;
	double dTrainParam2 = 0.0;

	nPossibleActivFunc = m_comboPossibleActivFunc.GetCurSel();
	
	m_editMaxiter.GetWindowText(strTmp);
	nMaxiter = _wtoi(strTmp);
	
	m_editEpsilon.GetWindowText(strTmp);
	dEpsilons = _wtof(strTmp);

	nTrainMethod = m_comboTrainMethod.GetCurSel();

	m_editTrainParam1.GetWindowText(strTmp);
	dTrainParam1 = _wtof(strTmp);

	m_editTrainParam2.GetWindowText(strTmp);
	dTrainParam2 = _wtof(strTmp);

	
	//////////////////////////////////////////////////////////////////////////


	strStartTime = gf_RecvCurrentTimeString(TRUE, tStart, dLogTime);
	CvANN_MLP bpn(layer_size, nPossibleActivFunc, 1, 1);
	CvTermCriteria term = cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS,nMaxiter,dEpsilons);
	CvANN_MLP_TrainParams train_params = CvANN_MLP_TrainParams( term, nTrainMethod, dTrainParam1, dTrainParam2 );
	bpn.train (train_data, train_class, NULL, 0, train_params);
	strEndTime = gf_RecvCurrentTimeString(TRUE, tEnd, dLogTime);
	m_pDlgSKYDlg->DispLogInfo(L"학습 종료"); // 종료 시간 // 시간차!! 

	//////////////////////////////////////////////////////////////////////////
	//파일 생성
	struct _timeb dLogTimeTmp; struct tm tTmp;
	CString  strFileName = L""; 
	strFileName.Format(L"ANN%s.csv",gf_RecvCurrentTimeString(FALSE, tTmp, dLogTimeTmp));
	CString strFilePath = gf_GetModulePath() + L"Data_Result\\" + strFileName;
	CStdioFile File;
	if (!File.Open(strFilePath, CFile::modeReadWrite | CFile::modeCreate| CFile::modeNoTruncate))
	{
		return ;
	}



	if(nPossibleActivFunc == 0)
		File.WriteString(L"Activate Func : IDENTITY\n");
	else if(nPossibleActivFunc == 1)
		File.WriteString(L"Activate Func : SIGMOID_SYM\n");
	else
		File.WriteString(L"Activate Func : GAUSSIAN\n"); 

	if(nTrainMethod == 0)
		strTmp.Format(L"Max iter : %d\nEpsilons : %g\nTrain Method : BACKPROP\nTrain Param1 : %g\nTrain Param2 : %g\n",nMaxiter,dEpsilons,dTrainParam1,dTrainParam2);
	else
		strTmp.Format(L"Max iter : %d\nEpsilons : %g\nTrain Method : RPROP\nTrain Param1 : %g\nTrain Param2 : %g\n",nMaxiter,dEpsilons,dTrainParam1,dTrainParam2);
	File.WriteString(strTmp);


	CString strTrainTest = L"";
	for(iterFolder = setFolderTrain.begin(); iterFolder != setFolderTrain.end(); iterFolder++)
		strTrainTest += *iterFolder + L",";
	File.WriteString(L"\nTRAIN," + strTrainTest  + L"\n");
	strTrainTest = L"";
	for(iterFolder = setFolderTest.begin(); iterFolder != setFolderTest.end(); iterFolder++)
		strTrainTest += *iterFolder + L",";
	File.WriteString(L"TEST," + strTrainTest  + L"\n");

// 	File.WriteString(L"학습 시작 시간 : " + strStartTime  + L"\n");
// 	File.WriteString(L"학습 끝 시간 : " + strEndTime + L"\n");
// 
// 	double dStart = _wtof(strStartTime);
// 	double dEnd = _wtof(strEndTime);
// 	CString strTim = L"";
// 	strTim.Format(L"걸린 시간 : %lf", dEnd - dStart);
// 	File.WriteString(strTim + L"\n");

	// 이미지의 모든 픽셀에 대해 각 픽셀이 어느 클래스에 속하는지 추정하여 클래스를 할당한다.

	m_pDlgSKYDlg->DispLogInfo(L"인식 시작");

	
	CvMat *sample  = cvCreateMat (1, nDataCount, CV_32FC1);;
	CvMat *response = cvCreateMat (1, MAX_CLASS, CV_32FC1);

	int nTotalPredictCnt = 0;
	int nOKPredictCnt = 0;
	for(iterData = mapTestData.begin() ; iterData != mapTestData.end() ; iterData++)
	{
		vecLineData = iterData->second;
		for(int vl = 0 ; vl < vecLineData.size() ; vl++)
		{
			vecData = vecLineData[vl];
			//! 파일 라인
			CString strWrite = L"";
			CString strWriteTmp = L"";
			CString strHeader = L"";
			for(int v = 0 ; v < vecData.size() ; v++)
			{
				float fData = vecData[v];
				CV_MAT_ELEM(*sample, float, 0, v) = fData;

				strWriteTmp.Format(L"%f,",fData);
				strWrite += strWriteTmp;
				strHeader += L",";
			}
			bpn.predict (sample, response);
			int c = FindMaxClassID (response);

			strWriteTmp.Format(L"%d,%s,",c, iterData->first);
			strWrite += strWriteTmp;
			strHeader += L"예상,실제,성공/실패\n";

			nTotalPredictCnt++;

			if(c == _wtoi(iterData->first))
			{
				nOKPredictCnt++;

				strWrite += L"TRUE\n";
			}
			else
			{
				strWrite += L"FALSE\n";
			}
			if(nTotalPredictCnt == 1)
				File.WriteString(strHeader);
			File.WriteString(strWrite);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//csv 마지막 저장
	CString strTail = L"", strTailTmp = L"";

	strTailTmp.Format(L"테스트 데이터 수 : %d\n",nTotalPredictCnt);		strTail += strTailTmp;
	strTailTmp.Format(L"인식 성공 수 : %d\n",nOKPredictCnt);			strTail += strTailTmp;
	strTailTmp.Format(L"인식률 %f\n",float((float)nOKPredictCnt / (float)nTotalPredictCnt) * 100.0);	strTail += strTailTmp;
	File.WriteString(strTail);
	File.Close();

	m_pDlgSKYDlg->DispLogInfo(L"인식 종료");

	strTmp.Format(L"테스트 데이터 수 : %d",nTotalPredictCnt);
	m_pDlgSKYDlg->DispLogInfo(strTmp);
	strTmp.Format(L"인식 성공 수 : %d",nOKPredictCnt);
	m_pDlgSKYDlg->DispLogInfo(strTmp);
	strTmp.Format(L"인식률 : %f",float((float)nOKPredictCnt / (float)nTotalPredictCnt) * 100.0);
	m_pDlgSKYDlg->DispLogInfo(strTmp);

	m_pDlgSKYDlg->m_vecScenarioRate.push_back(float((float)nOKPredictCnt / (float)nTotalPredictCnt) * 100.0);
	


	cvReleaseMat (&sample);
	cvReleaseMat (&response);

	cvReleaseMat (&train_class);
	cvReleaseMat (&train_data);
	cvReleaseMat (&layer_size);


	//! 다음 시나리오
	if(m_pDlgSKYDlg->m_vecScenario.size() > 0 && m_radioScenario.GetCheck())
	{
		m_pDlgSKYDlg->DispListScenario();
		m_pDlgSKYDlg->PostMessageW(WM_USER + 888, dfALGORITHM_PATTERN_ANN ,0);
	}
	else if(m_radioScenario.GetCheck())
	{
		
		AfxMessageBox(L"시나리오 분석이 종료되었습니다.");
		m_pDlgSKYDlg->m_vecScenario = m_pDlgSKYDlg->m_vecScenarioTmp;
		m_pDlgSKYDlg->DispListScenario();
	}
	else
	{
		AfxMessageBox(L"분석이 종료되었습니다.");
	}
}





void DlgAnn::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}
