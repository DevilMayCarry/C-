/*************************************************
	Copyright (C), 2008, HuiGui Tech. Co., Ltd.
	FileName:	Service.h		
	Date:		2008-05-06
	Author:    	Xu Fan			   
	Purpose:	服务
	History:    参考胡国静老版本的MC服务制作过程

*************************************************/

#ifndef OE_SERVICE_H
#define OE_SERVICE_H

#pragma once

#include <Windows.h>
#include <winsvc.h>
#include <stdio.h>

// 函数声明
VOID InstallService(TCHAR *pServicePath, CHAR *pSvcName);
VOID UnInstallService(CHAR *pSvcName);
BOOL StopService(CHAR *pSvcName);
BOOL RunService(CHAR *pSvcName);

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
VOID WINAPI ServiceHandler(DWORD fdwControl);

#endif