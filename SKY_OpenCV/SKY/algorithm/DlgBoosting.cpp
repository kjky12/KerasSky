// DlgBoosting.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SKY.h"
#include "DlgBoosting.h"
#include "afxdialogex.h"

#include "SKYDlg.h"

#include <sys/timeb.h>

// DlgBoosting ��ȭ �����Դϴ�.

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
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

// DlgBoosting �޽��� ó�����Դϴ�.

void DlgBoosting::BoostingTest()
{


	map<CString,vector<vector<float>>> mapTranData;
	map<CString,vector<vector<float>>> mapTestData;

	map<CString,vector<vector<float>>>::iterator iterData;

	// �н� �������� �� ��
	
	int data_count = 0;
	gf_ReadDataFile (mapTestData,data_count);
	const int sample_count = gf_ReadDataFile (mapTranData,data_count);
	
	// �н� �����Ϳ� Ŭ������ �Ҵ��� ��� ����
	CvMat *train_data  = cvCreateMat (sample_count, data_count, CV_32FC1);
	CvMat *train_class = cvCreateMat (sample_count, 1, CV_32FC1);

	vector<float> vecData;
	vector<vector<float>> vecLineData;

	// �������� �н� �����͸� ���� �� ���� ���� Ŭ������ �Ҵ�

	printf("�н������� �Է� ����\n");
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

	printf("�н������� �Է� ����\n");
		

	printf("�н� ����\n");

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

	// �ν�Ʈ �з���
	CvBoost boost;
	
	// �ν�Ʈ �з��Ⱑ �־��� �����Ϳ� Ŭ������ �н��Ѵ�.
	boost.train (train_data, CV_ROW_SAMPLE, train_class, 0, 0, 0, 0, param, false);
	
	printf("�н� ����\n");

	
	printf("�ν� ����\n");

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
			// KNN �з��Ⱑ �־��� �ȼ��� ��� Ŭ������ ���ϴ��� �����Ѵ�.
			float response = boost.predict (sample, 0);

			nTotalPredictCnt++;

			if(response == _wtoi(iterData->first))
				nOKPredictCnt++;
		}
	}
	printf("�ν� ����\n");

	printf("�׽�Ʈ ������ �� : %d\n",nTotalPredictCnt);
	printf("�ν� ���� �� : %d\n",nOKPredictCnt);
	printf("�νķ� : %f\n",float((float)nOKPredictCnt / (float)nTotalPredictCnt) * 100.0);

	cvReleaseMat (&sample);
	
	cvReleaseMat (&train_class);
	cvReleaseMat (&train_data);

	// Ű�� ������ ����
	cvWaitKey (0);



}




void DlgBoosting::OnBnClickedBtnAnalysis()
{
	//! �ó������� ��쿡 vector���� �޴´�.
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
			AfxMessageBox(L"��ϵ� �ó������� �����ϴ�.");
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
	//! ���õ� ����
	set<CString>::iterator iterFolder;
	set<CString> setFolderTrain = m_pDlgSKYDlg->m_setFolderTrain;
	set<CString> setFolderTest = m_pDlgSKYDlg->m_setFolderTest;

	//////////////////////////////////////////////////////////////////////////
	//�׽�Ʈ ���� �б�
	int data_count = 0;
	for(iterFolder = setFolderTest.begin(); iterFolder != setFolderTest.end(); iterFolder++)
	{
		gf_ReadDataFileName (*iterFolder,mapTestDataTmp,data_count);

		//! �۷ι� ������ �־��ش�.
		g_mapData.insert(make_pair(*iterFolder,mapTestDataTmp));

		for(iterDataTmp = mapTestDataTmp.begin(); iterDataTmp != mapTestDataTmp.end(); iterDataTmp++)
		{
			iterData = mapTestData.find(iterDataTmp->first);
			if(iterData != mapTestData.end()) //������
			{
				for(int i = 0; i < iterDataTmp->second.size(); i++)
					iterData->second.push_back(iterDataTmp->second[i]);
			}
			else //������
				mapTestData.insert(make_pair(iterDataTmp->first, iterDataTmp->second));
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Ʈ���� ���� �б�
	for(iterFolder = setFolderTrain.begin(); iterFolder != setFolderTrain.end(); iterFolder++)
	{
		gf_ReadDataFileName (*iterFolder, mapTranDataTmp,data_count);

		//! �۷ι� ������ �־��ش�.
		g_mapData.insert(make_pair(*iterFolder,mapTranDataTmp));

		for(iterDataTmp = mapTranDataTmp.begin(); iterDataTmp != mapTranDataTmp.end(); iterDataTmp++)
		{
			iterData = mapTranData.find(iterDataTmp->first);
			if(iterData != mapTranData.end()) //������
			{
				for(int i = 0; i < iterDataTmp->second.size(); i++)
					iterData->second.push_back(iterDataTmp->second[i]);
			}
			else //������
				mapTranData.insert(make_pair(iterDataTmp->first, iterDataTmp->second));
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if(mapTestData.size() <= 0 || mapTranData.size() <= 0)
	{
		AfxMessageBox(L"������ �����Ͱ� �����ϴ�.�н�/�ν� ���丮�� Ȯ���ϼ���.");
		return;
	}

	int sample_count = 0;
	for(iterData = mapTranData.begin() ; iterData != mapTranData.end() ; iterData++)
		sample_count += iterData->second.size();

	iterData = mapTranData.begin();
	const int nDataCount = iterData->second[0].size();
	
	// �н� �����Ϳ� Ŭ������ �Ҵ��� ��� ����
	CvMat *train_data  = cvCreateMat (sample_count, nDataCount, CV_32FC1);
	CvMat *train_class = cvCreateMat (sample_count, 1, CV_32FC1);

	vector<float> vecData;
	vector<vector<float>> vecLineData;

	// �������� �н� �����͸� ���� �� ���� ���� Ŭ������ �Ҵ�

	printf("�н������� �Է� ����\n");
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

	printf("�н������� �Է� ����\n");
		

	printf("�н� ����\n");
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

	// �ν�Ʈ �з���
	CvBoost boost;
	
	// �ν�Ʈ �з��Ⱑ �־��� �����Ϳ� Ŭ������ �н��Ѵ�.
	boost.train (train_data, CV_ROW_SAMPLE, train_class, 0, 0, 0, 0, param, false);
	

	strEndTime = gf_RecvCurrentTimeString(TRUE, tStart, dLogTime);
	printf("�н� ����\n");
	
	//////////////////////////////////////////////////////////////////////////
	//���� ����
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

	File.WriteString(L"�н� ���� �ð� : " + strStartTime  + L"\n");
	File.WriteString(L"�н� �� �ð� : " + strEndTime + L"\n");
	double dStart = _wtof(strStartTime);
	double dEnd = _wtof(strEndTime);
	CString strTim = L"";
	strTim.Format(L"�ɸ� �ð� : %lf", dEnd - dStart);
	File.WriteString(strTim + L"\n");

	printf("�ν� ����\n");

	
	CvMat *sample  = cvCreateMat (1, nDataCount, CV_32FC1);;


	int nTotalPredictCnt = 0;
	int nOKPredictCnt = 0;
	for(iterData = mapTestData.begin() ; iterData != mapTestData.end() ; iterData++)
	{
		vecLineData = iterData->second;
		for(int vl = 0 ; vl < vecLineData.size() ; vl++)
		{
			vecData = vecLineData[vl];
			//! ���� ����
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
			// KNN �з��Ⱑ �־��� �ȼ��� ��� Ŭ������ ���ϴ��� �����Ѵ�.
			float response = boost.predict (sample, 0);

			strWriteTmp.Format(L"%d,%s,",response, iterData->first);
			strWrite += strWriteTmp;
			strHeader += L"����,����,����/����\n";

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
	//csv ������ ����
	CString strTail = L"", strTailTmp = L"";
	
	strTailTmp.Format(L"�׽�Ʈ ������ �� : %d\n",nTotalPredictCnt);		strTail += strTailTmp;
	strTailTmp.Format(L"�ν� ���� �� : %d\n",nOKPredictCnt);			strTail += strTailTmp;
	strTailTmp.Format(L"�νķ� %f\n",float((float)nOKPredictCnt / (float)nTotalPredictCnt) * 100.0);	strTail += strTailTmp;
	File.WriteString(strTail);
	File.Close();

	printf("�ν� ����\n");

	printf("�׽�Ʈ ������ �� : %d\n",nTotalPredictCnt);
	printf("�ν� ���� �� : %d\n",nOKPredictCnt);
	printf("�νķ� : %f\n",float((float)nOKPredictCnt / (float)nTotalPredictCnt) * 100.0);

	cvReleaseMat (&sample);
	
	cvReleaseMat (&train_class);
	cvReleaseMat (&train_data);

	// Ű�� ������ ����
	cvWaitKey (0);


	//! ���� �ó�����
	if(m_pDlgSKYDlg->m_vecScenario.size() > 0 && m_radioScenario.GetCheck())
	{
		//OnBnClickedBtnAnalysis();
		m_pDlgSKYDlg->PostMessageW(WM_USER + 888, dfALGORITHM_PATTERN_BOOSTING ,0);
	}
	else if(m_radioScenario.GetCheck())
	{
		AfxMessageBox(L"�ó����� �м��� ����Ǿ����ϴ�.");
	}
	else
	{
		AfxMessageBox(L"�м��� ����Ǿ����ϴ�.");
	}
}

