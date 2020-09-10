// DlgBoosting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SKY.h"
#include "DlgBoosting.h"
#include "afxdialogex.h"

#include "SKYDlg.h"

#include <sys/timeb.h>

// DlgBoosting 대화 상자입니다.

IMPLEMENT_DYNAMIC(DlgBoosting, CDialogEx)

DlgBoosting::DlgBoosting(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgBoosting::IDD, pParent)
{
	m_pDlgSKYDlg = (CSKYDlg*)pParent;

}

DlgBoosting::~DlgBoosting()
{
}

void DlgBoosting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_DEFAULT, m_radioDefault);
	DDX_Control(pDX, IDC_RADIO_SCENARIO, m_radioScenario);
}


BEGIN_MESSAGE_MAP(DlgBoosting, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ANALYSIS, &DlgBoosting::OnBnClickedBtnAnalysis)
END_MESSAGE_MAP()




BOOL DlgBoosting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_radioDefault.SetCheck(TRUE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// DlgBoosting 메시지 처리기입니다.

void DlgBoosting::BoostingTest()
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
	CvMat *train_class = cvCreateMat (sample_count, 1, CV_32FC1);

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
			CV_MAT_ELEM(*train_class,float,k,0) = _wtoi(iterData->first);
			k++;
		}
	}

	printf("학습데이터 입력 종료\n");
		

	printf("학습 시작\n");

	/*
	CvBoostParams( 
		int boost_type,		// Type of the boosting algorithm. Possible values are:
							//	CvBoost::DISCRETE Discrete AbaBoost.
							//	CvBoost::REAL Real AdaBoost. It is a technique that utilizes 
							//		confidence-rated predictions and works well with categorical data.
							//	CvBoost::LOGIT LogitBoost. It can produce good regression fits.
							//	CvBoost::GENTLE Gentle AdaBoost. It puts less weight on outlier data points 
							//		and for that reason is often good with regression data.
							//	Gentle AdaBoost and Real AdaBoost are often the preferable choices.
		int weak_count,		// The number of weak classifiers.
		double weight_trim_rate,	// A threshold between 0 and 1 used to save computational time. 
							// Set this parameter to 0 to turn off this functionality.
		int max_depth, 
		bool use_surrogates, 
		const float* priors );
	*/
	CvBoostParams param (CvBoost::REAL, 10, 0.95, 3, false, 0);
	param.split_criteria = CvBoost::DEFAULT;

	// 부스트 분류기
	CvBoost boost;
	
	// 부스트 분류기가 주어진 데이터와 클래스를 학습한다.
	boost.train (train_data, CV_ROW_SAMPLE, train_class, 0, 0, 0, 0, param, false);
	
	printf("학습 종료\n");

	
	printf("인식 시작\n");

	CvMat *sample  = cvCreateMat (1, data_count, CV_32FC1);;


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
			// KNN 분류기가 주어진 픽셀이 어느 클래스에 속하는지 추정한다.
			float response = boost.predict (sample, 0);

			nTotalPredictCnt++;

			if(response == _wtoi(iterData->first))
				nOKPredictCnt++;
		}
	}
	printf("인식 종료\n");

	printf("테스트 데이터 수 : %d\n",nTotalPredictCnt);
	printf("인식 성공 수 : %d\n",nOKPredictCnt);
	printf("인식률 : %f\n",float((float)nOKPredictCnt / (float)nTotalPredictCnt) * 100.0);

	cvReleaseMat (&sample);
	
	cvReleaseMat (&train_class);
	cvReleaseMat (&train_data);

	// 키를 누르면 종료
	cvWaitKey (0);



}




void DlgBoosting::OnBnClickedBtnAnalysis()
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
	//테스트 파일 읽기
	int data_count = 0;
	for(iterFolder = setFolderTest.begin(); iterFolder != setFolderTest.end(); iterFolder++)
	{
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
	//트레인 파일 읽기
	for(iterFolder = setFolderTrain.begin(); iterFolder != setFolderTrain.end(); iterFolder++)
	{
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
	CvMat *train_class = cvCreateMat (sample_count, 1, CV_32FC1);

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
			CV_MAT_ELEM(*train_class,float,k,0) = _wtoi(iterData->first);
			k++;
		}
	}

	printf("학습데이터 입력 종료\n");
		

	printf("학습 시작\n");
	CString  strStartTime = L"", strEndTime = L"";
	strStartTime = gf_RecvCurrentTimeString(TRUE, tStart, dLogTime);

	/*
	CvBoostParams( 
		int boost_type,		// Type of the boosting algorithm. Possible values are:
							//	CvBoost::DISCRETE Discrete AbaBoost.
							//	CvBoost::REAL Real AdaBoost. It is a technique that utilizes 
							//		confidence-rated predictions and works well with categorical data.
							//	CvBoost::LOGIT LogitBoost. It can produce good regression fits.
							//	CvBoost::GENTLE Gentle AdaBoost. It puts less weight on outlier data points 
							//		and for that reason is often good with regression data.
							//	Gentle AdaBoost and Real AdaBoost are often the preferable choices.
		int weak_count,		// The number of weak classifiers.
		double weight_trim_rate,	// A threshold between 0 and 1 used to save computational time. 
							// Set this parameter to 0 to turn off this functionality.
		int max_depth, 
		bool use_surrogates, 
		const float* priors );
	*/
	CvBoostParams param (CvBoost::REAL, 10, 0.95, 3, false, 0);
	param.split_criteria = CvBoost::DEFAULT;

	// 부스트 분류기
	CvBoost boost;
	
	// 부스트 분류기가 주어진 데이터와 클래스를 학습한다.
	boost.train (train_data, CV_ROW_SAMPLE, train_class, 0, 0, 0, 0, param, false);
	

	strEndTime = gf_RecvCurrentTimeString(TRUE, tStart, dLogTime);
	printf("학습 종료\n");
	
	//////////////////////////////////////////////////////////////////////////
	//파일 생성
	struct _timeb dLogTimeTmp; struct tm tTmp;
	CString  strFileName = L""; 
	strFileName.Format(L"Boosting%s.csv",gf_RecvCurrentTimeString(FALSE, tTmp, dLogTimeTmp));
	CString strFilePath = gf_GetModulePath() + L"Data_Result\\" + strFileName;
	CStdioFile File;
	if (!File.Open(strFilePath, CFile::modeReadWrite | CFile::modeCreate| CFile::modeNoTruncate))
	{
		return ;
	}

	CString strTrainTest = L"";
	for(iterFolder = setFolderTrain.begin(); iterFolder != setFolderTrain.end(); iterFolder++)
		strTrainTest += *iterFolder + L",";
	File.WriteString(L"TRAIN," + strTrainTest  + L"\n");
	strTrainTest = L"";
	for(iterFolder = setFolderTest.begin(); iterFolder != setFolderTest.end(); iterFolder++)
		strTrainTest += *iterFolder + L",";
	File.WriteString(L"TEST," + strTrainTest  + L"\n");

	File.WriteString(L"학습 시작 시간 : " + strStartTime  + L"\n");
	File.WriteString(L"학습 끝 시간 : " + strEndTime + L"\n");
	double dStart = _wtof(strStartTime);
	double dEnd = _wtof(strEndTime);
	CString strTim = L"";
	strTim.Format(L"걸린 시간 : %lf", dEnd - dStart);
	File.WriteString(strTim + L"\n");

	printf("인식 시작\n");

	
	CvMat *sample  = cvCreateMat (1, nDataCount, CV_32FC1);;


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
			// KNN 분류기가 주어진 픽셀이 어느 클래스에 속하는지 추정한다.
			float response = boost.predict (sample, 0);

			strWriteTmp.Format(L"%d,%s,",response, iterData->first);
			strWrite += strWriteTmp;
			strHeader += L"예상,실제,성공/실패\n";

			nTotalPredictCnt++;

			if(response == _wtoi(iterData->first))
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

	printf("인식 종료\n");

	printf("테스트 데이터 수 : %d\n",nTotalPredictCnt);
	printf("인식 성공 수 : %d\n",nOKPredictCnt);
	printf("인식률 : %f\n",float((float)nOKPredictCnt / (float)nTotalPredictCnt) * 100.0);

	cvReleaseMat (&sample);
	
	cvReleaseMat (&train_class);
	cvReleaseMat (&train_data);

	// 키를 누르면 종료
	cvWaitKey (0);


	//! 다음 시나리오
	if(m_pDlgSKYDlg->m_vecScenario.size() > 0 && m_radioScenario.GetCheck())
	{
		//OnBnClickedBtnAnalysis();
		m_pDlgSKYDlg->PostMessageW(WM_USER + 888, dfALGORITHM_PATTERN_BOOSTING ,0);
	}
	else if(m_radioScenario.GetCheck())
	{
		AfxMessageBox(L"시나리오 분석이 종료되었습니다.");
	}
	else
	{
		AfxMessageBox(L"분석이 종료되었습니다.");
	}
}

