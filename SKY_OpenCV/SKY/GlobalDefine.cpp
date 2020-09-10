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
	@details	컨트롤 Text 리턴
	@param[in]	sPath : 파일 경로명
	@param[in]	lpGrpName : 그룹명
	@param[in]	lpCtrlName : 컨트롤명
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


// 텍스트 입력 파일 위치에 파일을 생성
BOOL gf_CreateTextFile(CString strFilePath)
{
	/** 
	@details	텍스트 입력 파일 위치에 파일을 생성
	@param[in]	strFilePath : 파일경로
	@return		BOOL
	*/

	CStdioFile File;

	if(!File.Open(strFilePath, CFile::modeCreate|CFile::modeReadWrite ))
		return FALSE;
	
	File.Close();

	return TRUE;
}

// 입력한 경로의 파일에 텍스트를 입력.
BOOL gf_InsertTextFile(CString strFilePath,CString strInsetText)
{
	/** 
	@details	입력한 경로의 파일에 텍스트를 입력.
	@param[in]	strFilePath : 파일경로
	@param[in]	strInsetText : 입력할 데이터
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

			// "."과 ".." 폴더는 무시한다.
			if(strFilePath.Find(L".") > -1 || strFilePath.Find(L"..") > -1)
				continue;

			CString	strNameTmp = File.GetFileName();
			if(strName == strNameTmp) //있음
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
	
	//! 글로벌에 있는지 확인한다. 다시 파일 데이터 읽음을 방지
	ST_DATA_FILE	stDataFile = vecDataFile[0];
	map<CString ,map<CString, vector< vector<float>>>>::iterator iterDataTEST;
	iterDataTEST = g_mapData.find(stDataFile.strUser);
	if(iterDataTEST != g_mapData.end())	//파일이 있으면
	{
		mapData = iterDataTEST->second;
	}
	else //! 없으면
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
//  		printf("학습 데이터 수 : %d\n",vecDataFile.size());
//  	else
//  		printf("인식 데이터 수 : %d\n",vecDataFile.size());
	printf("전체 데이터 수 : %d\n",vecDataFile.size());
	int nCount = 0;

	for(int vf = 0 ; vf < vecDataFileFilter.size() ; vf++)
	{
		ST_DATA_FILE	stDataFile = vecDataFileFilter[vf];

		CStdioFile stdPtnFile( stDataFile.strDataFile, CFile::modeRead | CFile::typeText );

		printf("%d 번째 파일 로드 시작\n",vf);

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

		printf("%d 번째 파일 로드 종료\n",vf);
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

			// "."과 ".." 폴더는 무시한다.
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
	//  		printf("학습 데이터 수 : %d\n",vecDataFile.size());
	//  	else
	//  		printf("인식 데이터 수 : %d\n",vecDataFile.size());
	printf("전체 데이터 수 : %d\n",vecDataFile.size());
	int nCount = 0;

	for(int vf = 0 ; vf < vecDataFile.size() ; vf++)
	{
		ST_DATA_FILE	stDataFile = vecDataFile[vf];

		CStdioFile stdPtnFile( stDataFile.strDataFile, CFile::modeRead | CFile::typeText );

		printf("%d 번째 파일 로드 시작\n",vf);

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

		printf("%d 번째 파일 로드 종료\n",vf);
	}


	return nCount;
}




CString gf_RecvCurrentTimeString(bool bMilisecFlag, struct tm &tmtime, struct _timeb &dLogTime)
{
	//bMilisecFlag 1: 밀리세크 포함, 0 : 밀리세크 미포함
	
	_ftime(&dLogTime);
	tmtime = *localtime(&dLogTime.time);

	CString strTime = L"";
	if(bMilisecFlag)	//밀리세크 포함
		strTime.Format(L"%04d%02d%02d%02d%02d%02d.%03d",tmtime.tm_year+1900,tmtime.tm_mon+1,tmtime.tm_mday,tmtime.tm_hour,tmtime.tm_min,tmtime.tm_sec,dLogTime.millitm);
	else
		strTime.Format(L"%04d%02d%02d%02d%02d%02d",tmtime.tm_year+1900,tmtime.tm_mon+1,tmtime.tm_mday,tmtime.tm_hour,tmtime.tm_min,tmtime.tm_sec);

	return strTime;
}


