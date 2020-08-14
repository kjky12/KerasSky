/**
* @file		WndCommClientMng.h
* CWndCommClientMng Ŭ���� ����
* @author	������ (jsseo@i-art.co.kr)
* @brief	����ȹ �ý��� ���� ���� �۽��� ���� Ŭ���̾�Ʈ ���
* @date
*			- 2013.01.10 ������ Start!
*			- 2013.01.16 ���ؿ� Update!
*			- 2013.05.08 �̼��� Update
* @copyright copyright by ARTechnology, Ltd.
*****************************************************************************/

#pragma once

/**
@brief		����ȹ �ý��� ���� ���� SW Ŭ����
@details	����ȹ �ý��� ���� ���� �۽��� ���� Ŭ���̾�Ʈ ��� Ŭ����
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

	// �޽��� ���� ����
	//int m_nRcvMsgCnt;
	
public:

protected:

	//! ���� �ε���
	int m_nIndex;


	//! �ݹ��Լ�1 �������
	DWORD_PTR m_dwUser1;
	//! �ݹ��Լ�2 �������
	DWORD_PTR m_dwUser2;


	//! �ӽ� ���� (��Ŷ ������ ����Ƽ ���� ��� ����)
	char* m_pszBuffer;	
	//! �ӽ� ���� ������
	int m_nBuffer;
	//! Ÿ�̸� �ð� ����
	int m_nTimeVal;
	//! ������ ������ IP
	char m_chIpAddr[100];
	//! ������ ������ ��Ʈ��ȣ
	int	m_nPortNum;
	//! ������ ���� ����
	int	m_nFileType;

public:

	BOOL ConnectNetWork(CString strIPAddress, int nPort);

	//���� ������ ��ġ��
	CString						m_strRecvData;
	//! Recv ��Ī
	int m_nRecvNum;


	
	
protected:

	// ȭ�� ���� �̺�Ʈ �Լ�
	afx_msg void OnDestroy();
	// ȭ�� ���� �̺�Ʈ �Լ�
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// ������ ����� ��� �˸� �Լ�
	afx_msg LRESULT OnNcsSocketConnected(WPARAM wParam, LPARAM lParam);
	// ������ ����������� �˸� �Լ�
	afx_msg LRESULT OnNcsSocketDisconnected(WPARAM wParam, LPARAM lParam);
	// NCS�κ��� ������ ���� �˸� �Լ�
	afx_msg LRESULT OnNcsRcvdData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);


	void SendToEmulMsg(int nIndex);

	void SendDevCtrlInfoMsg(char* psStructData,int nSize);
};


