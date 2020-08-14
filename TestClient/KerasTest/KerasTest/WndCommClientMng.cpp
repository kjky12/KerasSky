/**
* @file		WndSendCommClientMng.cpp
* CWndCommClientMng Ŭ���� ����
* @author	�̼��� (joseph@i-art.co.kr)
* @brief	������ ����� ���� Ŭ���̾�Ʈ ���
* @date
*			- 2014.06.24 �̼��� Start!
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
	@details	CWndCommClientMng Ŭ���� ������ �Լ�
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

	// �޽��� ���� ����
	//m_nRcvMsgCnt = 0;
}

CWndCommClientMng::~CWndCommClientMng()
{
	/** 
	@details	CWndCommClientMng Ŭ���� �Ҹ��� �Լ�
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



// CWndCommClientMng �޽��� ó�����Դϴ�.

int CWndCommClientMng::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	/** 
	@details	ȭ�� ���� �̺�Ʈ �Լ�
	@param[in]	lpCreateStruct : ���� ȭ�� ����ü
	@return		������ 0, ������ -1�� ���� 
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

	//! ip�ּ�
	memset(m_chIpAddr,NULL,100);
	strcpy_s(m_chIpAddr, CT2A(strIPAddress));
	//! ��Ʈ ��ȣ
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
			//AfxMessageBox(L"���� ����!!");
		}
		else if(m_nIndex == dfPING_HOST_ERROR)
		{
			//AfxMessageBox(L"�� ����!!");
		}
		else if(m_nIndex == dfPING_TIME_OUT)
		{
			//AfxMessageBox(L"Ÿ�� �ƿ� ����!!");
		}
		SetTimer(1000,10000,NULL);
	}

	return bConnect;
}

void CWndCommClientMng::OnDestroy()
{
	/** 
	@details	ȭ�� ���� �̺�Ʈ �Լ�
	@return		none
	*/

	
	M_A_DELETE(m_pszBuffer);

	memset(m_chIpAddr, 0x00, 100);

	

	if (m_nIndex > -1)
		::NetMngr_Disconnect(m_nIndex);


	

	CWnd::OnDestroy();
}


// ������ ����� ��� �˸� �Լ�
LRESULT CWndCommClientMng::OnNcsSocketConnected(WPARAM wParam, LPARAM lParam)
{
	int nIndex = (int)lParam;
	
	if (m_nIndex == nIndex)
	{
		m_nIndex = -1;


		//		m_pWndCNMmMain->DislayHistoryToListCtrl(L"������ ������ ���������ϴ�.");

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
	@details	������ ����������� �˸� �Լ�
	@return		none
	*/

	int nIndex = (int)lParam;
	if (m_nIndex == nIndex)
	{
		m_nIndex = -1;


//		m_pWndCNMmMain->DislayHistoryToListCtrl(L"������ ������ ���������ϴ�.");

//		m_pWndCNMmMain->PostMessage(WM_SEND_CNRSU_NETWORK,0,0);
		//m_pWndCNMmMain->DeleteFileNetWorkInfoClientModule(nIndex);
	}
	return TRUE;
}

LRESULT CWndCommClientMng::OnNcsRcvdData(WPARAM wParam, LPARAM lParam)
{
	/** 
	@details	NCS�κ��� ������ ���� �˸� �Լ�
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
			AfxMessageBox(L"��ü �ʱ�ȭ");
		}
		break;

	case df_MESS_PY_LEARNING_POSE_INIT:
		{
			AfxMessageBox(L"�κ� �ʱ�ȭ");

		}
		break;

	case df_MESS_PY_LEARNING_ADD_DATA :
		{
			AfxMessageBox(L"������ �߰�");

		}
		break;

	case df_MESS_PY_LEARNING_LEARN    :
		{
			AfxMessageBox(L"�н�");

		}
		break;

	case df_MESS_PY_CHK_COUNT_POSE    :
		{
			CString strTmp = L"";
			int nPos = -1;
			memcpy(&nPos, pRcvData + sizeof(P_HEADE), sizeof(int));
			strTmp.Format(L"���� :  %d", nPos);

			AfxMessageBox(L"�ڼ� ���� Ȯ�� " + strTmp);
			
		}
		break;

	case df_MESS_PY_CHK_POSE          :
		{
			CString strTmp = L"";
			int nPos = -1;
			memcpy(&nPos, pRcvData + sizeof(P_HEADE), sizeof(int));
			strTmp.Format(L"�ڼ� : %d ", nPos);
			AfxMessageBox(L"���� �ڼ� Ȯ�� " + strTmp);
		}
		break;

	case df_MESS_PY_RECV_ERROR   	  :
		{
			AfxMessageBox(L"���� ����");

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

// ���ķ����� ���� �˷��ִ� �޽���
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
		//m_pParent->DispLogInfo(L"���� ���� �Դϴ�.");
	}
	else if(nError ==dfPING_HOST_ERROR)
	{
		//m_pParent->DispLogInfo(L"ȣ��Ʈ ���� �Դϴ�.");
	}
	else if(nError ==dfPING_TIME_OUT)
	{
		//m_pParent->DispLogInfo(L"Ÿ�� �ƿ� ���� �Դϴ�.");
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

