
// MainWnd.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMainWndApp: 
// �йش����ʵ�֣������ MainWnd.cpp
//

class CMainWndApp : public CWinApp
{
public:
	CMainWndApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMainWndApp theApp;