/*************************************************
	Copyright (C), 2008, HuiGui Tech. Co., Ltd.
	FileName:	Service.cpp		
	Date:		2008-05-06
	Author:    	Xu Fan			   
	Purpose:	服务

*************************************************/
#include "stdafx.h"
#include "service.h"

#pragma warning(disable:4996)

//全局变量
SERVICE_STATUS			ServiceStatus;
SERVICE_STATUS_HANDLE	ServiceStatusHandle;
SERVICE_STATUS_HANDLE	hServiceStatusHandle;

#define BUFSIZE 500

TCHAR pServiceName[BUFSIZE + 1] = {0};
//CHAR pExeFile[BUFSIZE + 1] = {0};
//CHAR lpCmdLineData[BUFSIZE + 1] = {0};
SERVICE_TABLE_ENTRY lpServiceStartTable[] = 
{
	{pServiceName, ServiceMain},
	{NULL, NULL}
};

void charTowchar( TCHAR* pWchar, CHAR* pChar )
{
	int nLen = MultiByteToWideChar( CP_ACP, MB_COMPOSITE, pChar, -1, NULL, 0 );
	TCHAR* pNewWchar = new TCHAR[nLen];
	MultiByteToWideChar( CP_ACP, MB_COMPOSITE, pChar, -1, pNewWchar, nLen );
	wcsncpy(pWchar, pNewWchar, nLen);
	if (pNewWchar)
	{
		delete []pNewWchar;
		pNewWchar = NULL;
	}
}


VOID InstallService(TCHAR *pServicePath, CHAR *pSvcName)
{
	TCHAR wcSvcName[MAX_PATH] = {0};
	charTowchar(wcSvcName, pSvcName);
	CHAR szTmp[121] = {0};
	ULONG ulError = 0;
	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE); 
	
	if (schSCManager == 0) 
	{
		ulError = GetLastError();
		sprintf(szTmp, "OpenSCManager failed, error code = %d\n", ulError);
		printf(szTmp);
	}
	else
	{
		SC_HANDLE schService = CreateService
		( 
			schSCManager,				/* SCManager database      */ 
			wcSvcName,					/* name of service         */ 
			wcSvcName,					/* service name to display */ 
			SERVICE_ALL_ACCESS,			/* desired access          */ 
			SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, /* service type            */ 
			SERVICE_AUTO_START,			/* start type              */ 
			SERVICE_ERROR_NORMAL,		/* error control type      */ 
			pServicePath,				/* service's binary        */ 
			NULL,						/* no load ordering group  */ 
			NULL,						/* no tag identifier       */ 
			NULL,						/* no dependencies         */ 
			NULL,						/* LocalSystem account     */ 
			NULL						/* no password             */
		);								 
		if (schService == 0) 
		{
			ulError =  GetLastError();
			sprintf(szTmp, "Failed to create service %s, error code = %d\n", pSvcName, ulError);
			printf(szTmp);
		}
		else
		{
			sprintf(szTmp, "Service %s installed\n", pSvcName);
			printf(szTmp);
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager);
	}
}

VOID UnInstallService(CHAR *pSvcName)
{
	TCHAR wcSvcName[MAX_PATH] = {0};
	charTowchar(wcSvcName, pSvcName);
	CHAR szTmp[121] = {0};
	ULONG ulError = 0;
	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	
	if (schSCManager==0) 
	{
		ulError = GetLastError();
		sprintf(szTmp, "OpenSCManager failed, error code = %d\n", ulError);
		printf(szTmp);
	}
	else
	{
		SC_HANDLE schService = OpenService(schSCManager, wcSvcName, SERVICE_ALL_ACCESS);
		if (schService==0) 
		{
			ulError = GetLastError();
			sprintf(szTmp, "OpenService failed, error code = %d\n", ulError);
			printf(szTmp);
		}
		else
		{
			if(!DeleteService(schService)) 
			{
				sprintf(szTmp, "Failed to delete service %s\n", pSvcName);
				printf(szTmp);
			}
			else 
			{
				sprintf(szTmp, "Service %s removed\n", pSvcName);
				printf(szTmp);
			}
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager);	
	}
}

BOOL StopService(CHAR *pSvcName)
{
	TCHAR wcSvcName[MAX_PATH] = {0};
	charTowchar(wcSvcName, pSvcName);
	CHAR szTmp[121] = {0};
	ULONG ulError = 0;
	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	
	if (schSCManager==0) 
	{
		ulError = GetLastError();
		sprintf(szTmp, "OpenSCManager failed, error code = %d\n", ulError);
		printf(szTmp);
	}
	else
	{
		// open the service
		SC_HANDLE schService = OpenService(schSCManager, wcSvcName, SERVICE_ALL_ACCESS);
		if (schService==0) 
		{
			ulError = GetLastError();
			sprintf(szTmp, "OpenService failed, error code = %d\n", ulError);
			printf(szTmp);
		}
		else
		{
			// call ControlService to kill the given service
			SERVICE_STATUS status;
			if(ControlService(schService, SERVICE_CONTROL_STOP, &status))
			{
				CloseServiceHandle(schService); 
				CloseServiceHandle(schSCManager);
				sprintf(szTmp, "Service %s stoped.\n", pSvcName);
				printf(szTmp);
				return TRUE;
			}
			else
			{
				ulError = GetLastError();
				sprintf(szTmp, "Stop server %s failed, error code = %d\n", pSvcName, ulError);
				printf(szTmp);
			}
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager); 
	}

	return FALSE;
}

BOOL RunService(CHAR *pSvcName)
{
	TCHAR wcSvcName[MAX_PATH] = {0};
	charTowchar(wcSvcName, pSvcName);
	CHAR szTmp[121] = {0};
	ULONG ulError = 0;
	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	
	if (schSCManager==0) 
	{
		ulError = GetLastError();
		sprintf(szTmp, "OpenSCManager failed, error code = %d\n", ulError);
		printf(szTmp);
	}
	else
	{
		// open the service
		SC_HANDLE schService = OpenService(schSCManager, wcSvcName, SERVICE_ALL_ACCESS);
		if (schService==0) 
		{
			ulError = GetLastError();
			sprintf(szTmp, "OpenService failed, error code = %d\n", ulError);
			printf(szTmp);
		}
		else
		{
			// call StartService to run the service
			if(StartService(schService, 0, NULL))
			{
				CloseServiceHandle(schService); 
				CloseServiceHandle(schSCManager); 
				return TRUE;
			}
			else
			{
				ulError = GetLastError();
				sprintf(szTmp, "StartService %s failed, error code = %d\n", pSvcName, ulError);
				printf(szTmp);
			}
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager); 
	}

	return FALSE;
}

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
	DWORD   status = 0; 
    DWORD   specificError = 0xfffffff; 
	CHAR	szTmp[121] = {0};
	ULONG	ulError = 0;

    ServiceStatus.dwServiceType        = SERVICE_WIN32; 
    ServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
    ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE; 
    ServiceStatus.dwWin32ExitCode      = 0; 
    ServiceStatus.dwServiceSpecificExitCode = 0; 
    ServiceStatus.dwCheckPoint         = 0; 
    ServiceStatus.dwWaitHint           = 0; 
 
	hServiceStatusHandle = RegisterServiceCtrlHandler(pServiceName, ServiceHandler); 
    if (hServiceStatusHandle == 0) 
    {
		ulError = GetLastError();
		sprintf(szTmp, "RegisterServiceCtrlHandler failed, error code = %d\n", ulError);
		printf(szTmp);
        return; 
    } 
 
    // Initialization complete - report running status 
    ServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
    ServiceStatus.dwCheckPoint         = 0; 
    ServiceStatus.dwWaitHint           = 0;  
    if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus)) 
    { 
		ulError = GetLastError();
		sprintf(szTmp, "SetServiceStatus failed, error code = %d\n", ulError);
		printf(szTmp);
    } 

	HWINSTA  hWinStation = CreateWindowStation(NULL, 0, GENERIC_ALL, NULL);
	
	if (NULL != hWinStation)
	{
		SetProcessWindowStation(hWinStation);
		CloseWindowStation(hWinStation);
	}
}

VOID WINAPI ServiceHandler(DWORD fdwControl)
{
	switch(fdwControl) 
	{
		case SERVICE_CONTROL_STOP:
		case SERVICE_CONTROL_SHUTDOWN:
			ServiceStatus.dwWin32ExitCode = 0; 
			ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
			ServiceStatus.dwCheckPoint    = 0; 
			ServiceStatus.dwWaitHint      = 0;
			// terminate all processes started by this service before shutdown
			break; 
		case SERVICE_CONTROL_PAUSE:
			ServiceStatus.dwCurrentState = SERVICE_PAUSED; 
			break;
		case SERVICE_CONTROL_CONTINUE:
			ServiceStatus.dwCurrentState = SERVICE_RUNNING; 
			break;
		case SERVICE_CONTROL_INTERROGATE:
			break;
		default:
			break;
	};

    if (!SetServiceStatus(hServiceStatusHandle,  &ServiceStatus)) 
	{ 
		ULONG	ulError = GetLastError();
		CHAR	szTmp[121] = {0};
		sprintf(szTmp, "SetServiceStatus failed, error code = %d\n", ulError);
		printf(szTmp);
    } 
}