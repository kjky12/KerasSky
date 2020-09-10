#include "stdafx.h"
#include "GlobalDefine.h"
#include <sys/timeb.h>

#include "locale.h"

map<CString ,map<CString, vector< vector<float>>>> g_mapData;




CString	gf_ReadCsvKor(CString strFilePath)
{
	CStdioFile file;
	CString rStr = L"";

	setlocale(LC_ALL, "korean");

	if (file.Open(strFilePath, CFile::modeRead))
	{
		UINT nBytes = file.GetLength();
		if (nBytes >= 0)
		{
			CString strLine = L"";
			while (file.ReadString(strLine))
			{
				rStr += strLine + L"\n";
			}
		}
		file.Close();
	} 

	return rStr;
}


CString	gf_GetModulePath()
{
	wchar_t szPath[MAX_PATH];
	memset(szPath, 0x00, MAX_PATH);

	::GetModuleFileName(NULL, szPath, MAX_PATH);

	CString sTempPath = szPath;	

	int iPos = sTempPath.ReverseFind(TCHAR('\\'));

	CString sModulePath = sTempPath.Left(iPos);
	sModulePath += L"\\";

	return sModulePath;
}



CString gf_GetCtrlText(LPCTSTR sPath,LPCTSTR lpGrpName, LPCTSTR lpCtrlName)
{
	/**
	@details	��Ʈ�� Text ����
	@param[in]	sPath : ���� ��θ�
	@param[in]	lpGrpName : �׷��
	@param[in]	lpCtrlName : ��Ʈ�Ѹ�
	@return		CString
	*/

	CString strText;

	wchar_t szValue[32768];
	memset(szValue, 0, 32768);

	BOOL bResult = ::GetPrivateProfileString(lpGrpName,
		lpCtrlName,
		_T(""),
		szValue,
		32768,
		sPath);

	strText = szValue;

	return strText;
}


// �ؽ�Ʈ �Է� ���� ��ġ�� ������ ����
BOOL gf_CreateTextFile(CString strFilePath)
{
	/** 
	@details	�ؽ�Ʈ �Է� ���� ��ġ�� ������ ����
	@param[in]	strFilePath : ���ϰ��
	@return		BOOL
	*/

	CStdioFile File;

	if(!File.Open(strFilePath, CFile::modeCreate|CFile::modeReadWrite ))
		return FALSE;
	
	File.Close();

	return TRUE;
}

// �Է��� ����� ���Ͽ� �ؽ�Ʈ�� �Է�.
BOOL gf_InsertTextFile(CString strFilePath,CString strInsetText)
{
	/** 
	@details	�Է��� ����� ���Ͽ� �ؽ�Ʈ�� �Է�.
	@param[in]	strFilePath : ���ϰ��
	@param[in]	strInsetText : �Է��� ������
	@return		BOOL
	*/

	CStdioFile File;
	if(!File.Open(strFilePath,CFile::modeReadWrite | CFile::modeNoTruncate))
	{
		return FALSE;
	}

	File.SeekToEnd();
	File.WriteString(strInsetText);
	File.Close();

	return TRUE;
}



int gf_ReadDataFileName(CString strName, map<CString, vector< vector<float>>>& mapData,int& nDataCnt)
{
	mapData.clear();


	CString strFilePath = L"";
	CString strFile = L"";
	CFileFind File;

	strFile = gf_GetModulePath() + L"Data\\*.*";

	BOOL bIsFind = File.FindFile( strFile);
	vector<CString> vecDataFolder;

	while (bIsFind)
	{
		bIsFind = File.FindNextFileW();

		if(File.IsDirectory())
		{
			strFilePath = File.GetFilePath();

			// "."�� ".." ������ �����Ѵ�.
			if(strFilePath.Find(L".") > -1 || strFilePath.Find(L"..") > -1)
				continue;

			CString	strNameTmp = File.GetFileName();
			if(strName == strNameTmp) //����
				vecDataFolder.push_back(strFilePath);
		}
	}

	typedef struct ST_DATA_FILE
	{
		CString	strUser;
		CString	strDataFile;
		CString strDataFilePos;
	}ST_DATA_FILE;

	vector<ST_DATA_FILE>	vecDataFile;
	for(int v = 0 ; v < vecDataFolder.size() ; v++)
	{
		strFile = vecDataFolder[v] + L"\\*.*";

		bIsFind = File.FindFile( strFile);

		while (bIsFind)
		{
			bIsFind = File.FindNextFileW();

			if(!File.IsDirectory())
			{
				strFilePath = File.GetFilePath();
				CString strTitle = File.GetFileTitle();

				ST_DATA_FILE	stDataFile;
				CString strTT = vecDataFolder[v];
				stDataFile.strUser = strTT.Mid(strTT.ReverseFind('\\') + 1, strTT.GetLength() - strTT.ReverseFind('\\'));
				stDataFile.strDataFile = strFilePath;
				stDataFile.strDataFilePos = strTitle;
				vecDataFile.push_back(stDataFile);
			}
		}
	}


	vector<ST_DATA_FILE>	vecDataFileFilter;
	vecDataFileFilter.clear();
	
	//! �۷ι��� �ִ��� Ȯ���Ѵ�. �ٽ� ���� ������ ������ ����
	ST_DATA_FILE	stDataFile = vecDataFile[0];
	map<CString ,map<CString, vector< vector<float>>>>::iterator iterDataTEST;
	iterDataTEST = g_mapData.find(stDataFile.strUser);
	if(iterDataTEST != g_mapData.end())	//������ ������
	{
		mapData = iterDataTEST->second;
	}
	else //! ������
	{
		vecDataFileFilter =  vecDataFile;
	}
	



	char chTmp[500];
	CString strReadData = L"";
	char *token[63], *context;
	CString strData = L"";
	vector<float> vecData;
	vector<vector<float>> vecLineData;

	map<CString,vector<vector<float>>>::iterator iterData;

//  	if(bTrain)
//  		printf("�н� ������ �� : %d\n",vecDataFile.size());
//  	else
//  		printf("�ν� ������ �� : %d\n",vecDataFile.size());
	printf("��ü ������ �� : %d\n",vecDataFile.size());
	int nCount = 0;

	for(int vf = 0 ; vf < vecDataFileFilter.size() ; vf++)
	{
		ST_DATA_FILE	stDataFile = vecDataFileFilter[vf];

		CStdioFile stdPtnFile( stDataFile.strDataFile, CFile::modeRead | CFile::typeText );

		printf("%d ��° ���� �ε� ����\n",vf);

		BOOL bDataOn = FALSE;

		iterData = mapData.find(stDataFile.strDataFilePos);
		if(iterData != mapData.end())
		{
			vecLineData = iterData->second;
			bDataOn = TRUE;
		}
		else
			vecLineData.clear();


		while ( stdPtnFile.ReadString(strReadData) )
		{
			vecData.clear();
			int i = 0;
			memset(chTmp,NULL,500);
			strcpy_s( chTmp, CT2A(strReadData));

			token[i] = strtok_s(chTmp, ",", &context);

			strData.Format(_T("%s"),CA2T(token[i]));

			vecData.push_back(_wtof(strData));

			i++;
			while((token[i] = strtok_s(NULL, ",", &context)) != NULL) 
			{
				strData.Format(_T("%s"),CA2T(token[i]));
				vecData.push_back(_wtof(strData));
				i++;
			}
			vecLineData.push_back(vecData);

			nDataCnt = vecData.size();

			nCount++;
		}


		if(bDataOn)
		{
			iterData->second = vecLineData;			
		}
		else
		{
			mapData.insert(make_pair(stDataFile.strDataFilePos,vecLineData));
			//g_mapData.insert(make_pair(stDataFile.strDataFile,mapData));
		}

		printf("%d ��° ���� �ε� ����\n",vf);
	}


	return nCount;
}





int gf_ReadDataFile(map<CString, vector<vector<float>>>& mapData,int& nDataCnt)
{
	mapData.clear();


	CString strFilePath = L"";
	CString strFile = L"";
	CFileFind File;

	strFile = gf_GetModulePath() + L"Data\\*.*";

	BOOL bIsFind = File.FindFile( strFile);
	vector<CString> vecDataFolder;

	while (bIsFind)
	{
		bIsFind = File.FindNextFileW();

		if(File.IsDirectory())
		{
			strFilePath = File.GetFilePath();

			// "."�� ".." ������ �����Ѵ�.
			if(strFilePath.Find(L".") > -1 || strFilePath.Find(L"..") > -1)
				continue;

			vecDataFolder.push_back(strFilePath);
		}
	}

	typedef struct ST_DATA_FILE
	{
		CString	strDataFile;
		CString strDataFilePos;
	}ST_DATA_FILE;

	vector<ST_DATA_FILE>	vecDataFile;
	for(int v = 0 ; v < vecDataFolder.size() ; v++)
	{
		strFile = vecDataFolder[v] + L"\\*.*";

		bIsFind = File.FindFile( strFile);

		while (bIsFind)
		{
			bIsFind = File.FindNextFileW();

			if(!File.IsDirectory())
			{
				strFilePath = File.GetFilePath();
				CString strTitle = File.GetFileTitle();

				ST_DATA_FILE	stDataFile;
				stDataFile.strDataFile = strFilePath;
				stDataFile.strDataFilePos = strTitle;
				vecDataFile.push_back(stDataFile);
			}
		}
	}

	char chTmp[500];
	CString strReadData = L"";
	char *token[63], *context;
	CString strData = L"";
	vector<float> vecData;
	vector<vector<float>> vecLineData;

	map<CString,vector<vector<float>>>::iterator iterData;

	//  	if(bTrain)
	//  		printf("�н� ������ �� : %d\n",vecDataFile.size());
	//  	else
	//  		printf("�ν� ������ �� : %d\n",vecDataFile.size());
	printf("��ü ������ �� : %d\n",vecDataFile.size());
	int nCount = 0;

	for(int vf = 0 ; vf < vecDataFile.size() ; vf++)
	{
		ST_DATA_FILE	stDataFile = vecDataFile[vf];

		CStdioFile stdPtnFile( stDataFile.strDataFile, CFile::modeRead | CFile::typeText );

		printf("%d ��° ���� �ε� ����\n",vf);

		BOOL bDataOn = FALSE;

		iterData = mapData.find(stDataFile.strDataFilePos);
		if(iterData != mapData.end())
		{
			vecLineData = iterData->second;
			bDataOn = TRUE;
		}
		else
			vecLineData.clear();


		while ( stdPtnFile.ReadString(strReadData) )
		{
			vecData.clear();
			int i = 0;
			memset(chTmp,NULL,500);
			strcpy_s( chTmp, CT2A(strReadData));

			token[i] = strtok_s(chTmp, ",", &context);

			strData.Format(_T("%s"),CA2T(token[i]));

			vecData.push_back(_wtof(strData));

			i++;
			while((token[i] = strtok_s(NULL, ",", &context)) != NULL) 
			{
				strData.Format(_T("%s"),CA2T(token[i]));
				vecData.push_back(_wtof(strData));
				i++;
			}
			vecLineData.push_back(vecData);

			nDataCnt = vecData.size();

			nCount++;
		}


		if(bDataOn)
			iterData->second = vecLineData;
		else
			mapData.insert(make_pair(stDataFile.strDataFilePos,vecLineData));

		printf("%d ��° ���� �ε� ����\n",vf);
	}


	return nCount;
}




CString gf_RecvCurrentTimeString(bool bMilisecFlag, struct tm &tmtime, struct _timeb &dLogTime)
{
	//bMilisecFlag 1: �и���ũ ����, 0 : �и���ũ ������
	
	_ftime(&dLogTime);
	tmtime = *localtime(&dLogTime.time);

	CString strTime = L"";
	if(bMilisecFlag)	//�и���ũ ����
		strTime.Format(L"%04d%02d%02d%02d%02d%02d.%03d",tmtime.tm_year+1900,tmtime.tm_mon+1,tmtime.tm_mday,tmtime.tm_hour,tmtime.tm_min,tmtime.tm_sec,dLogTime.millitm);
	else
		strTime.Format(L"%04d%02d%02d%02d%02d%02d",tmtime.tm_year+1900,tmtime.tm_mon+1,tmtime.tm_mday,tmtime.tm_hour,tmtime.tm_min,tmtime.tm_sec);

	return strTime;
}


