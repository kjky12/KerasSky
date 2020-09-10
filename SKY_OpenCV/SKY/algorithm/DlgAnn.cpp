// DlgAnn.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SKY.h"
#include "DlgAnn.h"
#include "afxdialogex.h"

#include "SKYDlg.h"
static const int MAX_CLASS = 7;	// Ŭ���� ��


#include <sys/timeb.h>

// DlgAnn ��ȭ �����Դϴ�.

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

	//! ���� �Էµ��ִ� ���� ����Ʈ�� �Ѵ�.

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
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}



// DlgAnn �޽��� ó�����Դϴ�.

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

	// �н� �������� �� ��

	int data_count = 0;
	gf_ReadDataFile (mapTestData,data_count);
	const int sample_count = gf_ReadDataFile (mapTranData,data_count);

	// �н� �����Ϳ� Ŭ������ �Ҵ��� ��� ����
	CvMat *train_data  = cvCreateMat (sample_count, data_count, CV_32FC1);
	CvMat *train_class = cvCreateMat (sample_count, MAX_CLASS, CV_32FC1);
	CvMat *layer_size  = cvCreateMat (1, 3, CV_32SC1);
	cvZero (train_class);

	CV_MAT_ELEM(*layer_size, long, 0,0) = data_count;
	CV_MAT_ELEM(*layer_size, long, 0,1) = 12; // ���� ���̾� �Է� �ʿ�
	CV_MAT_ELEM(*layer_size, long, 0,2) = MAX_CLASS;

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
			CV_MAT_ELEM(*train_class,float,k,_wtoi(iterData->first)) = 1.f;
			k++;
		}
	}

	printf("�н������� �Է� ����\n");

	printf("�н� ����\n"); // ���� �ð�

	CvANN_MLP bpn(layer_size, CvANN_MLP::GAUSSIAN, 1, 1);

	CvTermCriteria term = cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS,5000,0.001);
	CvANN_MLP_TrainParams train_params = CvANN_MLP_TrainParams( term, CvANN_MLP_TrainParams::BACKPROP, 0.3, 0.3 );

	bpn.train (train_data, train_class, NULL, 0, train_params);

	printf("�н� ����\n"); // ���� �ð� // �ð���!! 

	CvMat *sample  = cvCreateMat (1, data_count, CV_32FC1);;
	CvMat *response = cvCreateMat (1, MAX_CLASS, CV_32FC1);

	// �̹����� ��� �ȼ��� ���� �� �ȼ��� ��� Ŭ������ ���ϴ��� �����Ͽ� Ŭ������ �Ҵ��Ѵ�.

	printf("�ν� ����\n");
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
	printf("�ν� ����\n");

	printf("�׽�Ʈ ������ �� : %d\n",nTotalPredictCnt);
	printf("�ν� ���� �� : %d\n",nOKPredictCnt);
	printf("�νķ� : %f\n",float((float)nOKPredictCnt / (float)nTotalPredictCnt) * 100.0);

	cvReleaseMat (&sample);
	cvReleaseMat (&response);

	cvReleaseMat (&train_class);
	cvReleaseMat (&train_data);
	cvReleaseMat (&layer_size);


	// Ű�� ������ ����
	cvWaitKey (0);



}


void DlgAnn::OnBnClickedBtnAnalysis()
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
	CvMat *train_class = cvCreateMat (sample_count, MAX_CLASS, CV_32FC1);
	CvMat *layer_size  = cvCreateMat (1, 3, CV_32SC1);
	cvZero (train_class);

	CV_MAT_ELEM(*layer_size, long, 0,0) = nDataCount;
	CV_MAT_ELEM(*layer_size, long, 0,1) = 12; // ���� ���̾� �Է� �ʿ�
	CV_MAT_ELEM(*layer_size, long, 0,2) = MAX_CLASS;

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
			CV_MAT_ELEM(*train_class,float,k,_wtoi(iterData->first)) = 1.f;
			k++;
		}
	}

	m_pDlgSKYDlg->DispLogInfo(L"�н������� �Է� ����");

	m_pDlgSKYDlg->DispLogInfo(L"�н� ����"); // ���� �ð�
	CString  strStartTime = L"", strEndTime = L"";

	//////////////////////////////////////////////////////////////////////////
	// Train�� �ʿ��� �� �ֱ�

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
	m_pDlgSKYDlg->DispLogInfo(L"�н� ����"); // ���� �ð� // �ð���!! 

	//////////////////////////////////////////////////////////////////////////
	//���� ����
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

// 	File.WriteString(L"�н� ���� �ð� : " + strStartTime  + L"\n");
// 	File.WriteString(L"�н� �� �ð� : " + strEndTime + L"\n");
// 
// 	double dStart = _wtof(strStartTime);
// 	double dEnd = _wtof(strEndTime);
// 	CString strTim = L"";
// 	strTim.Format(L"�ɸ� �ð� : %lf", dEnd - dStart);
// 	File.WriteString(strTim + L"\n");

	// �̹����� ��� �ȼ��� ���� �� �ȼ��� ��� Ŭ������ ���ϴ��� �����Ͽ� Ŭ������ �Ҵ��Ѵ�.

	m_pDlgSKYDlg->DispLogInfo(L"�ν� ����");

	
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
			bpn.predict (sample, response);
			int c = FindMaxClassID (response);

			strWriteTmp.Format(L"%d,%s,",c, iterData->first);
			strWrite += strWriteTmp;
			strHeader += L"����,����,����/����\n";

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
	//csv ������ ����
	CString strTail = L"", strTailTmp = L"";

	strTailTmp.Format(L"�׽�Ʈ ������ �� : %d\n",nTotalPredictCnt);		strTail += strTailTmp;
	strTailTmp.Format(L"�ν� ���� �� : %d\n",nOKPredictCnt);			strTail += strTailTmp;
	strTailTmp.Format(L"�νķ� %f\n",float((float)nOKPredictCnt / (float)nTotalPredictCnt) * 100.0);	strTail += strTailTmp;
	File.WriteString(strTail);
	File.Close();

	m_pDlgSKYDlg->DispLogInfo(L"�ν� ����");

	strTmp.Format(L"�׽�Ʈ ������ �� : %d",nTotalPredictCnt);
	m_pDlgSKYDlg->DispLogInfo(strTmp);
	strTmp.Format(L"�ν� ���� �� : %d",nOKPredictCnt);
	m_pDlgSKYDlg->DispLogInfo(strTmp);
	strTmp.Format(L"�νķ� : %f",float((float)nOKPredictCnt / (float)nTotalPredictCnt) * 100.0);
	m_pDlgSKYDlg->DispLogInfo(strTmp);

	m_pDlgSKYDlg->m_vecScenarioRate.push_back(float((float)nOKPredictCnt / (float)nTotalPredictCnt) * 100.0);
	


	cvReleaseMat (&sample);
	cvReleaseMat (&response);

	cvReleaseMat (&train_class);
	cvReleaseMat (&train_data);
	cvReleaseMat (&layer_size);


	//! ���� �ó�����
	if(m_pDlgSKYDlg->m_vecScenario.size() > 0 && m_radioScenario.GetCheck())
	{
		m_pDlgSKYDlg->DispListScenario();
		m_pDlgSKYDlg->PostMessageW(WM_USER + 888, dfALGORITHM_PATTERN_ANN ,0);
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





void DlgAnn::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnOK();
}
