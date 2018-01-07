#ifndef _REENJOIN_H
#define _REENJOIN_H

#include <windows.h>
#include <tchar.h>
#include <setupapi.h>

#define		ENJOIN_FD			0
#define		ENJOIN_CD			1
#define		ENJOIN_USB			2
#define		ENJOIN_DISK			3

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "Rpcrt4.lib")

class ReEnjoin
{
public:
	BOOL ControlDisk( BOOL bEnjoin, BOOL nIndex );

private:
	HDEVINFO	m_hDevInfo;

	void FormatMSG(DWORD dwError,LPTSTR *lpszMsg);
	BOOL ChangeStatus(DWORD NewStatus, DWORD SelectedItem, HDEVINFO hDevInfo);
	BOOL GetClassInstallParams( HDEVINFO hDevInfo, DWORD SelectedItem, 
		PSP_DEVINFO_DATA pDeviceInfoData );

};

#endif