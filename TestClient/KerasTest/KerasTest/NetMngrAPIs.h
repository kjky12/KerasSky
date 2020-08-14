//
//NetMngrAPIs.h
//

#ifndef _NETMNGRAPIS_H_
#define _NETMNGRAPIS_H_

#define dfPING_SOCK_ERROR					-1
#define dfPING_HOST_ERROR					-2
#define dfPING_TIME_OUT						-3

//
#define dfMAX_PKT_TTL_SIZE					1024


//User Defined Window Messages
#define WM_NETMNGR_RCVD_DATA							WM_APP + 2100
#define WM_NETMNGR_SOCKET_CONNECTED						WM_APP + 2101
#define WM_NETMNGR_SOCKET_DISCONNECTED					WM_APP + 2102

//typedef void (CALLBACK *callbackRcvData)(char* szData, int nCount);

//NetMngr APIs
BOOL WINAPI NetMngr_CreateManager();
BOOL WINAPI NetMngr_DeleteeManager();

BOOL WINAPI NetMngr_StartListen(HWND hWndParent, int nPort);
void WINAPI NetMngr_StopListen(int nPort);

void WINAPI NetMngr_Disconnect(int nIndex);
int  WINAPI NetMngr_Connect(HWND hWndParent, char* szIPAddress, int nPort);

void WINAPI NetMngr_SetBlocking(int nIndex, BOOL bBlocking, int nTimeOut = 5000);
int  WINAPI NetMngr_SendData(int nIndex, char* szData, int nDataSize);
int  WINAPI NetMngr_GetData(int nIndex, char* szData);

BOOL WINAPI NetMngr_WaitRcvData(int nIndex);

int WINAPI NetMngr_HostPing(int nLoop, LPCTSTR szIPAddr);

//void WINAPI NetMngr_RcvDataFuncs(int nIndex, callbackRcvData f1);

#endif