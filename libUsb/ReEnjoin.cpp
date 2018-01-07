#include "stdafx.h"
#include "ReEnjoin.h"

#include "Rpc.h"

//出错信息
void ReEnjoin::FormatMSG(DWORD dwError,LPTSTR *lpszMsg)
{
	BOOL bOK=FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|
		FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL,dwError,
		MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),
		(LPTSTR)lpszMsg,0,NULL);
	if(!bOK)
	{
		HMODULE hDll=LoadLibraryEx(_T("netmsg.dll"),NULL,DONT_RESOLVE_DLL_REFERENCES);
		if(hDll=NULL)
		{
			FormatMessage(FORMAT_MESSAGE_FROM_HMODULE|
				FORMAT_MESSAGE_FROM_SYSTEM,hDll,dwError,
				MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),
				(LPTSTR)lpszMsg,0,NULL);
			MessageBox(NULL, (LPTSTR)lpszMsg, NULL, MB_OK);

			FreeLibrary(hDll);
		}
	}
}

BOOL ReEnjoin::ChangeStatus(DWORD NewStatus,DWORD SelectedItem,HDEVINFO hDevInfo)
{
	LPTSTR lpszMsg=NULL;
	HCURSOR hCursor=NULL;
	try
	{
		SP_PROPCHANGE_PARAMS PropChangeParams={sizeof(SP_CLASSINSTALL_HEADER)};
		SP_DEVINFO_DATA DeviceInfoData={sizeof(SP_DEVINFO_DATA)};

		hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

		//GET A handle to the selected item
		if (!SetupDiEnumDeviceInfo(hDevInfo,SelectedItem,&DeviceInfoData))
		{
			FormatMSG(GetLastError(),&lpszMsg);
			throw lpszMsg;
		}

		int error1 = GetLastError();

		//set the PropChangeParams structure
		PropChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
		PropChangeParams.Scope = DICS_FLAG_GLOBAL;
		PropChangeParams.StateChange = NewStatus;

		error1 = GetLastError();

		/// if your system is x64, you need Switch the compiler from win32 To x64.
		/// of course, if your system is x86, you only use the win32.
		/// Add by Johnny in 2017-03-22 17:11:46

		if(!SetupDiSetClassInstallParams(hDevInfo,&DeviceInfoData, (SP_CLASSINSTALL_HEADER*)&PropChangeParams, 
			sizeof(PropChangeParams)))
		{
			FormatMSG(GetLastError(),&lpszMsg);
			throw lpszMsg;
		}
		int error2 = GetLastError();

		///if error : 5, you need provide more Higher authority. Maybe need Administrator ..

		///Add by Johnny in 2017-03-22 17:07:21

		//call the ClassInstaller and perform the change
		if(!SetupDiCallClassInstaller(DIF_PROPERTYCHANGE,hDevInfo,&DeviceInfoData))
		{
			FormatMSG(GetLastError(),&lpszMsg);
			int x = GetLastError();
			throw lpszMsg;
		}
		SetCursor(hCursor);
		int error3 = GetLastError();

		return TRUE;
	}
	catch(...)
	{
		SetCursor(hCursor);
		if(lpszMsg!=NULL)
		{
			LocalFree((HLOCAL)lpszMsg);
		}
		return FALSE;
	}
}

BOOL ReEnjoin::GetClassInstallParams( HDEVINFO hDevInfo, DWORD SelectedItem, PSP_DEVINFO_DATA pDeviceInfoData )
{
	LPTSTR lpszMsg=NULL;
	DWORD dwRequire = 0;
	SP_PROPCHANGE_PARAMS PropChangeParams={sizeof(SP_CLASSINSTALL_HEADER)};

	try
	{
		if (!SetupDiGetClassInstallParams(hDevInfo, pDeviceInfoData, (SP_CLASSINSTALL_HEADER*)&PropChangeParams, 
			sizeof(PropChangeParams), &dwRequire))
		{
			FormatMSG( GetLastError(), &lpszMsg );
			throw lpszMsg;
		}
		return TRUE;
	}
	catch (...)
	{
		if(lpszMsg!=NULL)
		{
			LocalFree((HLOCAL)lpszMsg);
		}
	}
	return FALSE;
}


BOOL ReEnjoin::ControlDisk( BOOL bEnjoin, BOOL nIndex )
{
	LPTSTR lpszMsg=NULL;
	try
	{
		TCHAR *GUIDString = NULL;
		GUID guid;
		ZeroMemory(&guid, sizeof(GUID));
		switch (nIndex)
		{
		case ENJOIN_FD://0代表软驱
			GUIDString = _T("4D36E980-E325-11CE-BFC1-08002BE10318");
			UuidFromString((RPC_WSTR)GUIDString, &guid);
			break;

		case ENJOIN_CD://1代表光驱
			GUIDString = _T("4D36E965-E325-11CE-BFC1-08002BE10318");
			UuidFromString((RPC_WSTR)GUIDString, &guid);
			break;

		case ENJOIN_USB://2代表USB
			GUIDString = _T("36FC9E60-C465-11CF-8056-444553540000");
			UuidFromString((RPC_WSTR)GUIDString, &guid);
			break;

		case ENJOIN_DISK://磁盘
			GUIDString = _T("4D36E967-E325-11CE-BFC1-08002BE10318");
			UuidFromString((RPC_WSTR)GUIDString, &guid);
			break;
		}
		m_hDevInfo = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT);
		
		if (INVALID_HANDLE_VALUE == m_hDevInfo)
		{
			FormatMSG(GetLastError(), &lpszMsg);
			throw lpszMsg;
		}

		SP_DEVINFO_DATA DeviceInfoData;
		ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
		DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

		/// 枚举设备信息
		for( int i = 0; SetupDiEnumDeviceInfo( m_hDevInfo, i, &DeviceInfoData ); ++i)
		{
			//Error : 317
			//GetClassInstallParams( m_hDevInfo, i, &DeviceInfoData );

			//int error2 = GetLastError();

			//转换
			if (bEnjoin)
			{
				ChangeStatus(DICS_ENABLE, i, m_hDevInfo);

			}
			else
			{
				ChangeStatus(DICS_DISABLE, i, m_hDevInfo);
			}
			//error2 = GetLastError();				
		}

		//int error3 = GetLastError();

		//释放device information set
		return SetupDiDestroyDeviceInfoList( m_hDevInfo );
		
	}
	catch(...)
	{
		if(lpszMsg!=NULL)
		{
			LocalFree((HLOCAL)lpszMsg);
		}
		return FALSE;
	}
	return FALSE;
}

