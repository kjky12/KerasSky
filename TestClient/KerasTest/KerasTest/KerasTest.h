
// KerasTest.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CKerasTestApp:
// �� Ŭ������ ������ ���ؼ��� KerasTest.cpp�� �����Ͻʽÿ�.
//

class CKerasTestApp : public CWinApp
{
public:
	CKerasTestApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CKerasTestApp theApp;