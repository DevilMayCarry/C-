/*************************************************
	Copyright (C), 2008, HuiGui Tech. Co., Ltd.
	FileName:	Service.h		
	Date:		2008-05-06
	Author:    	Xu Fan			   
	Purpose:	����
	History:    �ο��������ϰ汾��MC������������

*************************************************/

#ifndef OE_SERVICE_H
#define OE_SERVICE_H

#pragma once

#include <Windows.h>
#include <winsvc.h>
#include <stdio.h>

// ��������
VOID InstallService(TCHAR *pServicePath, CHAR *pSvcName);
VOID UnInstallService(CHAR *pSvcName);
BOOL StopService(CHAR *pSvcName);
BOOL RunService(CHAR *pSvcName);

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
VOID WINAPI ServiceHandler(DWORD fdwControl);

#endif