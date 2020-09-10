// DlgBayesianClassifier.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SKY.h"
#include "DlgBayesianClassifier.h"
#include "afxdialogex.h"

#include "SKYDlg.h"

#include <sys/timeb.h>


// DlgBayesianClassifier ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(DlgBayesianClassifier, CDialogEx)

DlgBayesianClassifier::DlgBayesianClassifier(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgBayesianClassifier::IDD, pParent)
{

	m_pDlgSKYDlg = (CSKYDlg*)pParent;
}

DlgBayesianClassifier::~DlgBayesianClassifier()
{
}

void DlgBayesianClassifier::DoDataExchange(CDataExchange* pDX)
{

	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_DEFAULT, m_radioDefault);
	DDX_Control(pDX, IDC_RADIO_SCENARIO, m_radioScenario);
}


BEGIN_MESSAGE_MAP(DlgBayesianClassifier, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ANALYSIS, &DlgBayesianClassifier::OnBnClickedBtnAnalysis)
END_MESSAGE_MAP()



BOOL DlgBayesianClassifier::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_radioDefault.SetCheck(TRUE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

// DlgBayesianClassifier �޽��� ó�����Դϴ�.

void DlgBayesianClassifier::BayesianClassifierTest ()
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

	// ������ �з��Ⱑ �־��� �����Ϳ� Ŭ������ �н��Ѵ�.
	CvNormalBayesClassifier bayes (train_data, train_class, 0, 0);

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
			int response = bayes.predict (sample,0);

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




void DlgBayesianClassifier::OnBnClickedBtnAnalysis()
{
	//! �ó������� ��쿡 vector���� �޴´�.
	if(m_radioScenario.GetCheck())
	{
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

	m_pDlgSKYDlg->DispLogInfo(L"�ν� ������ �ε�");
	//�׽�Ʈ ���� �б�
	int data_count = 0;
	for(iterFolder = setFolderTest.begin(); iterFolder != setFolderTest.end(); iterFolder++)
	{
		m_pDlgSKYDlg->DispLogInfo(*iterFolder + L"���� ������ �ε���");

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
	m_pDlgSKYDlg->DispLogInfo(L"�н� ������ �ε�");
	//Ʈ���� ���� �б�
	for(iterFolder = setFolderTrain.begin(); iterFolder != setFolderTrain.end(); iterFolder++)
	{
		m_pDlgSKYDlg->DispLogInfo(*iterFolder + L"���� ������ �ε���");

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

	m_pDlgSKYDlg->DispLogInfo(L"�н������� �Է� ����");
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

	m_pDlgSKYDlg->DispLogInfo(L"�н������� �Է� ����");

	m_pDlgSKYDlg->DispLogInfo(L"�н� ����"); // ���� �ð�


	CString  strStartTime = L"", strEndTime = L"";
	strStartTime = gf_RecvCurrentTimeString(TRUE, tStart, dLogTime);


	// ������ �з��Ⱑ �־��� �����Ϳ� Ŭ������ �н��Ѵ�.
	CvNormalBayesClassifier bayes (train_data, train_class, 0, 0);


	strEndTime = gf_RecvCurrentTimeString(TRUE, tStart, dLogTime);
	m_pDlgSKYDlg->DispLogInfo(L"�н� ����"); // ���� �ð� // �ð���!! 

	//////////////////////////////////////////////////////////////////////////
	//���� ����
	struct _timeb dLogTimeTmp; struct tm tTmp;
	CString  strFileName = L""; 
	strFileName.Format(L"BayesianClassifier%s.csv",gf_RecvCurrentTimeString(FALSE, tTmp, dLogTimeTmp));
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

// 	File.WriteString(L"�н� ���� �ð� : " + strStartTime  + L"\n");
// 	File.WriteString(L"�н� �� �ð� : " + strEndTime + L"\n");
// 	double dStart = _wtof(strStartTime);
// 	double dEnd = _wtof(strEndTime);
// 	CString strTim = L"";
// 	strTim.Format(L"�ɸ� �ð� : %lf", dEnd - dStart);
// 	File.WriteString(strTim + L"\n");

	m_pDlgSKYDlg->DispLogInfo(L"�ν� ����");

	
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
			int response = bayes.predict (sample,0);

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
	

	m_pDlgSKYDlg->DispLogInfo(L"�ν� ����");

	CString strTmp = L"";
	strTmp.Format(L"�׽�Ʈ ������ �� : %d",nTotalPredictCnt);
	m_pDlgSKYDlg->DispLogInfo(strTmp);
	strTmp.Format(L"�ν� ���� �� : %d",nOKPredictCnt);
	m_pDlgSKYDlg->DispLogInfo(strTmp);
	strTmp.Format(L"�νķ� : %f",float((float)nOKPredictCnt / (float)nTotalPredictCnt) * 100.0);
	m_pDlgSKYDlg->DispLogInfo(strTmp);
	
	m_pDlgSKYDlg->m_vecScenarioRate.push_back(float((float)nOKPredictCnt / (float)nTotalPredictCnt) * 100.0);


	cvReleaseMat (&sample);
	cvReleaseMat (&train_class);
	cvReleaseMat (&train_data);


	//! ���� �ó�����
	if(m_pDlgSKYDlg->m_vecScenario.size() > 0 && m_radioScenario.GetCheck())
	{
		m_pDlgSKYDlg->DispListScenario();

		m_pDlgSKYDlg->PostMessageW(WM_USER + 888, dfALGORITHM_PATTERN_BAYESIAN_CLASSIFER ,0);
	}
	else if(m_radioScenario.GetCheck())
	{
		AfxMessageBox(L"�ó����� �м��� ����Ǿ����ϴ�.");

		m_pDlgSKYDlg->m_vecScenario = m_pDlgSKYDlg->m_vecScenarioTmp;
		m_pDlgSKYDlg->DispListScenario();
	}
	else
	{
		AfxMessageBox(L"�м��� ����Ǿ����ϴ�.");
	}
}


