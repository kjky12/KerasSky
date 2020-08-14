/**
* @file		WndSendCommClientMng.cpp
* CWndCommClientMng 클래스 구현
* @author	이수열 (joseph@i-art.co.kr)
* @brief	서버와 통신을 위한 클라이언트 모듈
* @date
*			- 2014.06.24 이수열 Start!
* @copyright copyright by Advanced Radio Technology, Ltd.
*****************************************************************************/

#include "stdafx.h"
#include "WndCommClientMng.h"
#include "KerasTest.h"


#include "KerasTestDlg.h"

#include <atlconv.h>
// CWndCommClientMng

IMPLEMENT_DYNAMIC(CWndCommClientMng, CWnd)

CWndCommClientMng::CWndCommClientMng(CWnd* pParent)
{
	/** 
	@details	CWndCommClientMng 클래스 생성자 함수
	@return		none
	*/ 

	m_pKerasTestDlg = (CKerasTestDlg*)pParent;


	m_nIndex = -1;

//	f_callbackRcvData = NULL;
	m_dwUser1 = 0;
	m_dwUser2 = 0;

	
	m_nBuffer = 0;
	m_pszBuffer = new char[dfMAX_PKT_TTL_SIZE];
	memset(m_pszBuffer, 0, dfMAX_PKT_TTL_SIZE);


	m_nTimeVal = 5000;
	memset(m_chIpAddr, 0x00, 100);
	
	m_nPortNum = 5025;

	m_nRecvNum = -1;

	// 메시지 수신 개수
	//m_nRcvMsgCnt = 0;
}

CWndCommClientMng::~CWndCommClientMng()
{
	/** 
	@details	CWndCommClientMng 클래스 소멸자 함수
	@return		none
	*/ 

	KillTimer(1000);
	M_A_DELETE(m_pszBuffer);
	
	memset(m_chIpAddr, 0x00, 100);


}


BEGIN_MESSAGE_MAP(CWndCommClientMng, CWnd)
	ON_WM_DESTROY()
	ON_WM_CREATE()

	ON_MESSAGE(WM_NETMNGR_SOCKET_CONNECTED, &CWndCommClientMng::OnNcsSocketConnected)
	ON_MESSAGE(WM_NETMNGR_SOCKET_DISCONNECTED, &CWndCommClientMng::OnNcsSocketDisconnected)
	ON_MESSAGE(WM_NETMNGR_RCVD_DATA, &CWndCommClientMng::OnNcsRcvdData)
	ON_WM_TIMER()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CWndCommClientMng 메시지 처리기입니다.

int CWndCommClientMng::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	/** 
	@details	화면 생성 이벤트 함수
	@param[in]	lpCreateStruct : 생성 화면 구조체
	@return		성공시 0, 오류시 -1을 리턴 
	*/


	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	ConnectNetWork(L"127.0.0.1", 56789);

	return 0;
}

BOOL CWndCommClientMng::ConnectNetWork(CString strIPAddress, int nPort)
{
	if (m_nIndex > -1)
	{
		
		::NetMngr_Disconnect(m_nIndex);
	}

	// 	char chTmp[100];
	// 	memset(chTmp,NULL,100);
	// 	strcpy_s(chTmp, CT2A(strIPAddress));
	//	m_nIndex = ::NetMngr_Connect(this->m_hWnd,chTmp,nPort);

	//! ip주소
	memset(m_chIpAddr,NULL,100);
	strcpy_s(m_chIpAddr, CT2A(strIPAddress));
	//! 포트 번호
	m_nPortNum = nPort;

	m_nIndex = ::NetMngr_Connect(this->m_hWnd,m_chIpAddr,m_nPortNum);

	bool bConnect = FALSE;
	if (m_nIndex >= 0)
	{
		bConnect =TRUE;
		KillTimer(1000);
	}
	else
	{
		if(m_nIndex == dfPING_SOCK_ERROR)
		{
			//AfxMessageBox(L"소켓 에러!!");
		}
		else if(m_nIndex == dfPING_HOST_ERROR)
		{
			//AfxMessageBox(L"핑 에러!!");
		}
		else if(m_nIndex == dfPING_TIME_OUT)
		{
			//AfxMessageBox(L"타임 아웃 에러!!");
		}
		SetTimer(1000,10000,NULL);
	}

	return bConnect;
}

void CWndCommClientMng::OnDestroy()
{
	/** 
	@details	화면 종료 이벤트 함수
	@return		none
	*/

	
	M_A_DELETE(m_pszBuffer);

	memset(m_chIpAddr, 0x00, 100);

	

	if (m_nIndex > -1)
		::NetMngr_Disconnect(m_nIndex);


	

	CWnd::OnDestroy();
}


// 소켓이 연결된 경우 알림 함수
LRESULT CWndCommClientMng::OnNcsSocketConnected(WPARAM wParam, LPARAM lParam)
{
	int nIndex = (int)lParam;
	
	if (m_nIndex == nIndex)
	{
		m_nIndex = -1;


		//		m_pWndCNMmMain->DislayHistoryToListCtrl(L"서버와 연결이 끊어졌습니다.");

		//		m_pWndCNMmMain->PostMessage(WM_SEND_CNRSU_NETWORK,0,0);
		//m_pWndCNMmMain->DeleteFileNetWorkInfoClientModule(nIndex);
	}
	else
		m_nIndex = nIndex;

	return TRUE;
}

LRESULT CWndCommClientMng::OnNcsSocketDisconnected(WPARAM wParam, LPARAM lParam)
{
	/** 
	@details	소켓이 끊어졌을경우 알림 함수
	@return		none
	*/

	int nIndex = (int)lParam;
	if (m_nIndex == nIndex)
	{
		m_nIndex = -1;


//		m_pWndCNMmMain->DislayHistoryToListCtrl(L"서버와 연결이 끊어졌습니다.");

//		m_pWndCNMmMain->PostMessage(WM_SEND_CNRSU_NETWORK,0,0);
		//m_pWndCNMmMain->DeleteFileNetWorkInfoClientModule(nIndex);
	}
	return TRUE;
}

LRESULT CWndCommClientMng::OnNcsRcvdData(WPARAM wParam, LPARAM lParam)
{
	/** 
	@details	NCS로부터 데이터 수신 알림 함수
	@return		none
	*/ 
	CString strFileID = L"";
	int nCount = 0;
	char szRcvData[dfMAX_PKT_TTL_SIZE];
	memset(szRcvData, 0x00, dfMAX_PKT_TTL_SIZE);

	m_nBuffer = 0;

	if (m_nBuffer > 0)
	{
		memcpy(szRcvData, m_pszBuffer, m_nBuffer);

		nCount = ::NetMngr_GetData(m_nIndex, szRcvData + m_nBuffer);
		nCount += m_nBuffer;
		m_nBuffer = nCount;
	}
	else
	{
		nCount = ::NetMngr_GetData(m_nIndex, szRcvData);
		m_nBuffer = nCount;
	}

	P_HEADE	pHeade;

	char* pRcvData = szRcvData;
	memcpy(&pHeade, pRcvData, sizeof(P_HEADE));

	switch (pHeade.nMessageID)
	{
	case df_MESS_PY_LEARNING_ALL_INIT :
		{
			AfxMessageBox(L"전체 초기화");
		}
		break;

	case df_MESS_PY_LEARNING_POSE_INIT:
		{
			AfxMessageBox(L"부분 초기화");

		}
		break;

	case df_MESS_PY_LEARNING_ADD_DATA :
		{
			AfxMessageBox(L"데이터 추가");

		}
		break;

	case df_MESS_PY_LEARNING_LEARN    :
		{
			AfxMessageBox(L"학습");

		}
		break;

	case df_MESS_PY_CHK_COUNT_POSE    :
		{
			CString strTmp = L"";
			int nPos = -1;
			memcpy(&nPos, pRcvData + sizeof(P_HEADE), sizeof(int));
			strTmp.Format(L"갯수 :  %d", nPos);

			AfxMessageBox(L"자세 갯수 확인 " + strTmp);
			
		}
		break;

	case df_MESS_PY_CHK_POSE          :
		{
			CString strTmp = L"";
			int nPos = -1;
			memcpy(&nPos, pRcvData + sizeof(P_HEADE), sizeof(int));
			strTmp.Format(L"자세 : %d ", nPos);
			AfxMessageBox(L"현재 자세 확인 " + strTmp);
		}
		break;

	case df_MESS_PY_RECV_ERROR   	  :
		{
			AfxMessageBox(L"서버 에러");

		}
		break;

	case PY_FAIL_DATA		          :
		{
			AfxMessageBox(L"Deep Lerning Fail");

		}
		break;



	default:
		break;
	}



	return TRUE;
}

// 에뮬레이터 정보 알려주는 메시지
void CWndCommClientMng::SendToEmulMsg(int nIndex)
{
	//if(stDevLna.bModeAutoManual == 0)
	//{
	//	char psStructData[100] = "{@CMA}";
	//	psStructData[6] = CheckSum(psStructData,6);
	//	::NetMngr_SendData(m_nIndex, (char*)&psStructData, 7);
	//}
}


void CWndCommClientMng::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(1000);

	CString strIP;
	strIP = m_chIpAddr;

	ConnectNetWork(strIP, m_nPortNum);

	CWnd::OnTimer(nIDEvent);
}


void CWndCommClientMng::SendDevCtrlInfoMsg(char* psStructData,int nSize)
{
	int nError = ::NetMngr_SendData(m_nIndex, psStructData,nSize);


	//M_A_DELETE(pSendData);

	if(nError ==dfPING_SOCK_ERROR)
	{
		//m_pParent->DispLogInfo(L"소켓 오류 입니다.");
	}
	else if(nError ==dfPING_HOST_ERROR)
	{
		//m_pParent->DispLogInfo(L"호스트 오류 입니다.");
	}
	else if(nError ==dfPING_TIME_OUT)
	{
		//m_pParent->DispLogInfo(L"타임 아웃 오류 입니다.");
	}
	else
	{
		if(nError >= 0)
		{
			CString strTmp = L"";
			// 			strTmp.Format(L"T - MSG [ %d ]",nMsgID);
			// 			//	m_pParent->DispLogInfo(strTmp);
		}
	}
}

