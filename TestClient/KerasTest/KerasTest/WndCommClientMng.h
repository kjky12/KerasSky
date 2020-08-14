/**
* @file		WndCommClientMng.h
* CWndCommClientMng 클래스 선언
* @author	서진석 (jsseo@i-art.co.kr)
* @brief	망계획 시스템 간의 파일 송신을 위한 클라이언트 모듈
* @date
*			- 2013.01.10 서진석 Start!
*			- 2013.01.16 박준영 Update!
*			- 2013.05.08 이수열 Update
* @copyright copyright by ARTechnology, Ltd.
*****************************************************************************/

#pragma once

/**
@brief		망계획 시스템 메인 정보 SW 클래스
@details	망계획 시스템 간의 파일 송신을 위한 클라이언트 모듈 클래스
@author		jsseo@i-art.co.kr
@version	1.0
@date		2013.01.10
*/


class CKerasTestDlg;
class CWndCommClientMng : public CWnd
{
	DECLARE_DYNAMIC(CWndCommClientMng)

public:
	CWndCommClientMng(CWnd* pParent);
	virtual ~CWndCommClientMng();

	int m_nCtrlInx;

	CKerasTestDlg	*m_pKerasTestDlg;

	// 메시지 수신 개수
	//int m_nRcvMsgCnt;
	
public:

protected:

	//! 소켓 인덱스
	int m_nIndex;


	//! 콜백함수1 멤버변수
	DWORD_PTR m_dwUser1;
	//! 콜백함수2 멤버변수
	DWORD_PTR m_dwUser2;


	//! 임시 버퍼 (패킷 사이즈 데이티 수신 대기 버퍼)
	char* m_pszBuffer;	
	//! 임시 버퍼 사이즈
	int m_nBuffer;
	//! 타이머 시간 변수
	int m_nTimeVal;
	//! 접속할 서버의 IP
	char m_chIpAddr[100];
	//! 접속할 서버의 포트번호
	int	m_nPortNum;
	//! 보내는 파일 형식
	int	m_nFileType;

public:

	BOOL ConnectNetWork(CString strIPAddress, int nPort);

	//읽은 데이터 합치기
	CString						m_strRecvData;
	//! Recv 매칭
	int m_nRecvNum;


	
	
protected:

	// 화면 종료 이벤트 함수
	afx_msg void OnDestroy();
	// 화면 생성 이벤트 함수
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// 소켓이 연결된 경우 알림 함수
	afx_msg LRESULT OnNcsSocketConnected(WPARAM wParam, LPARAM lParam);
	// 소켓이 끊어졌을경우 알림 함수
	afx_msg LRESULT OnNcsSocketDisconnected(WPARAM wParam, LPARAM lParam);
	// NCS로부터 데이터 수신 알림 함수
	afx_msg LRESULT OnNcsRcvdData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);


	void SendToEmulMsg(int nIndex);

	void SendDevCtrlInfoMsg(char* psStructData,int nSize);
};


