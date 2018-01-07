#include "stdafx.h"
#pragma warning(disable:4996)
#include "CloseWindows.h"
#include "DeviceContrl.h"

//Add by Johnny
BOOL CALLBACK EnumWinProc(HWND hWnd, LPARAM lp);
DWORD WINAPI Thread(LPVOID lp);

BOOL StrCmpEx( TCHAR* pStr1, TCHAR* pStr2 )
{
	TCHAR szBuff[MAX_PATH] = {0};
	TCHAR szBuff1[MAX_PATH] = {0};
	wcscpy(szBuff, pStr1);

	int i = 0;
	int nLen = (int)wcslen(szBuff);
	while ( i <= nLen)
	{
		wcsncpy(szBuff1, szBuff + i, nLen);
		if ( wcsncmp(szBuff1, pStr2, wcslen(pStr2)) == 0)
		{
			return TRUE;
		}
		i++;
	}
	return FALSE;
}

DWORD WINAPI Thread(LPVOID lp)
{
	CDeviceContrl* pContrl = (CDeviceContrl*)lp;

	::EnumWindows(EnumWinProc, 0);
	::EnumWindows(EnumWinProc, 0);
	Sleep(1000);

	//屏蔽磁盘操作
	//pContrl->m_Enjoin.ControlDisk( TRUE, ENJOIN_DISK );
	//pContrl->m_Enjoin.ControlDisk( FALSE, ENJOIN_DISK );

	//屏蔽U盘操作
	//pContrl->m_Enjoin.ControlDisk(TRUE,  ENJOIN_USB);
	pContrl->m_Enjoin.ControlDisk(FALSE, ENJOIN_USB);

	return 0;
}

//To make sure close all usb, you should close all files which is running..		Add by Johnny in 2017-03-22 17:05:12
BOOL CALLBACK EnumWinProc( HWND hWnd, LPARAM lp)
{
	if ( hWnd == NULL )
	{
		return FALSE;
	}

	TCHAR szClassName[MAX_PATH] = {0};
	GetClassName( hWnd, szClassName, MAX_PATH );
	if ( wcscmp( _T("CabinetWClass"), szClassName ) == 0)
	{
		::PostMessage( hWnd, WM_CLOSE, 0, 0 );
	}
	else if ( wcscmp( _T("#32770"), szClassName ) == 0)
	{
		TCHAR szWindowsText[MAX_PATH] = {0};
		::GetWindowText( hWnd, szWindowsText, MAX_PATH );
		if ( wcscmp( _T("Destination Folder Access Denied"), szWindowsText ) == 0)
		{
			::PostMessage( hWnd, WM_CLOSE, 0, 0 );
		}
		else if (wcsncmp( _T("remaining"), szWindowsText, wcslen(_T("remaining"))) == 0)
		{
			::PostMessage( hWnd, WM_CLOSE, 0, 0 );
		}
		else if (StrCmpEx(szWindowsText, _T("remaining")))
		{		
			::PostMessage( hWnd, WM_CLOSE, 0, 0 );
		}
		else if (wcsncmp( _T("Copying..."), szWindowsText, wcslen(_T("Copying..."))) == 0)
		{
			::PostMessage( hWnd, WM_CLOSE, 0, 0 );
		}
		else if (StrCmpEx(szWindowsText, _T("Copying...")))
		{		
			::PostMessage( hWnd, WM_CLOSE, 0, 0 );
		}
		else if (wcsncmp( _T("Moving..."), szWindowsText, wcslen(_T("Moving..."))) == 0)
		{
			::PostMessage( hWnd, WM_CLOSE, 0, 0 );
		}
		else if (StrCmpEx(szWindowsText, _T("Moving...")))
		{		
			::PostMessage( hWnd, WM_CLOSE, 0, 0 );
		}
		else if (wcsncmp( _T("剩余"), szWindowsText, wcslen(_T("剩余"))) == 0)
		{
			::PostMessage( hWnd, WM_CLOSE, 0, 0 );
		}
		else if (StrCmpEx(szWindowsText, _T("剩余")))
		{		
			::PostMessage( hWnd, WM_CLOSE, 0, 0 );
		}
		else if (wcsncmp( _T("正在复制..."), szWindowsText, wcslen(_T("正在复制..."))) == 0)
		{
			::PostMessage( hWnd, WM_CLOSE, 0, 0 );
		}
		else if (StrCmpEx(szWindowsText, _T("正在复制...")))
		{		
			::PostMessage( hWnd, WM_CLOSE, 0, 0 );
		}
		else if (wcsncmp( _T("正在移动..."), szWindowsText, wcslen(_T("正在移动..."))) == 0)
		{
			::PostMessage( hWnd, WM_CLOSE, 0, 0 );
		}
		else if (StrCmpEx(szWindowsText, _T("正在移动...")))
		{		
			::PostMessage( hWnd, WM_CLOSE, 0, 0 );
		}
	}

	return TRUE;
}