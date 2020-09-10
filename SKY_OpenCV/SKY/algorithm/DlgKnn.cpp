// DlgKnn.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SKY.h"
#include "DlgKnn.h"
#include "afxdialogex.h"


#include "SKYDlg.h"


#include <sys/timeb.h>


static const int MAX_K = 39;
// DlgKnn ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(DlgKnn, CDialogEx)

DlgKnn::DlgKnn(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgKnn::IDD, pParent)
{
	m_pDlgSKYDlg = (CSKYDlg*)pParent;

}

DlgKnn::~DlgKnn()
{
}

void DlgKnn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MAX_K, m_editMaxK);
	DDX_Control(pDX, IDC_CHECK_REGRESSION, m_chkRegression);
	DDX_Control(pDX, IDC_CHECK1, m_chkMaxK);
	DDX_Control(pDX, IDC_RADIO_DEFAULT, m_radioDefault);
	DDX_Control(pDX, IDC_RADIO_SCENARIO, m_radioScenario);
}


BEGIN_MESSAGE_MAP(DlgKnn, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ANALYSIS, &DlgKnn::OnBnClickedBtnAnalysis)
	ON_BN_CLICKED(IDC_CHECK1, &DlgKnn::OnBnClickedCheck1)
END_MESSAGE_MAP()



BOOL DlgKnn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	OnBnClickedCheck1();

	m_radioDefault.SetCheck(TRUE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void DlgKnn::KNNTest ()
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

	int MAX_K = sqrt(k);	// üũ�ڽ��� �ڵ� ��꿩�� �Ǵ� �Է� �� ���� �ְ�!!!

	// learn classifier
	CvKNearest knn (train_data, train_class, 0, false, MAX_K);

	CvMat *nearests = cvCreateMat (1, MAX_K, CV_32FC1);

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
			float response = knn.find_nearest (sample, MAX_K, 0, 0, nearests, 0);

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
	cvReleaseMat (&nearests);

	// Ű�� ������ ����
	cvWaitKey (0);



}




void DlgKnn::OnBnClickedBtnAnalysis()
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
	m_pDlgSKYDlg->DispLogInfo(L"�ν� ������ �ε�");
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


	//////////////////////////////////////////////////////////////////////////
	// Train�� �ʿ��� �� �ֱ�

	int MAX_K = sqrt(k);	// üũ�ڽ��� �ڵ� ��꿩�� �Ǵ� �Է� �� ���� �ְ�!!!
	//! max K üũ�ڽ� ���ý� ����Ʈ���� ����
	if(m_chkMaxK.GetCheck())
	{
		CString strTemp = L"";
		m_editMaxK.GetWindowTextW(strTemp);
		MAX_K = _wtoi(strTemp);
	}
	
	//! ȸ�� �м� ���� ����
	bool bRegression = false;
	if(m_chkRegression.GetCheck())
		bRegression = true;
	


	//////////////////////////////////////////////////////////////////////////



	

	m_pDlgSKYDlg->DispLogInfo(L"�н� ����"); // ���� �ð�

	CString  strStartTime = L"", strEndTime = L"";
	strStartTime = gf_RecvCurrentTimeString(TRUE, tStart, dLogTime);

	// learn classifier
	CvKNearest knn (train_data, train_class, 0, bRegression, MAX_K);

	CvMat *nearests = cvCreateMat (1, MAX_K, CV_32FC1);


	strEndTime = gf_RecvCurrentTimeString(TRUE, tStart, dLogTime);

	m_pDlgSKYDlg->DispLogInfo(L"�н� ����"); // ���� �ð� // �ð���!! 

	struct _timeb dLogTimeTmp; struct tm tTmp;
	CString  strFileName = L""; 
	strFileName.Format(L"Knn%s.csv",gf_RecvCurrentTimeString(FALSE, tTmp, dLogTimeTmp));
	CString strFilePath = gf_GetModulePath() + L"Data_Result\\" + strFileName;
	CStdioFile File;
	if (!File.Open(strFilePath, CFile::modeReadWrite | CFile::modeCreate| CFile::modeNoTruncate))
	{
		return ;
	}

	CString strTmp = L"";

	if(bRegression == 0)
		File.WriteString(L"Regression : FALSE\n");
	else
		File.WriteString(L"Regression : TRUE\n");
	
	strTmp.Format(L"Max L : %d\n", MAX_K);
	File.WriteString(strTmp);


	CString strTrainTest = L"";
	for(iterFolder = setFolderTrain.begin(); iterFolder != setFolderTrain.end(); iterFolder++)
		strTrainTest += *iterFolder + L",";
	File.WriteString(L"\nTRAIN," + strTrainTest  + L"\n");
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
			// KNN �з��Ⱑ �־��� �ȼ��� ��� Ŭ������ ���ϴ��� �����Ѵ�.
			float response = knn.find_nearest (sample, MAX_K, 0, 0, nearests, 0);

			strWriteTmp.Format(L"%d,%s,", int(response), iterData->first);
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

//	CString strTmp = L"";
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
	cvReleaseMat (&nearests);

	// Ű�� ������ ����
	//cvWaitKey (0);


	//! ���� �ó�����
	if(m_pDlgSKYDlg->m_vecScenario.size() > 0 && m_radioScenario.GetCheck())
	{
		m_pDlgSKYDlg->DispListScenario();

		m_pDlgSKYDlg->PostMessageW(WM_USER + 888, dfALGORITHM_PATTERN_KNN ,0);
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


void DlgKnn::OnBnClickedCheck1()
{
	if(m_chkMaxK.GetCheck())
	{
		m_editMaxK.EnableWindow(TRUE);
	}
	else
	{
		m_editMaxK.EnableWindow(FALSE);
	}
}


