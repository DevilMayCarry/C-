#include "StdAfx.h"
#pragma warning(disable:4996)
#include "DeviceContrl.h"
#include "CloseWindows.h"


DWORD WINAPI Thread(LPVOID lp);

CDeviceContrl::CDeviceContrl(void)
{
	m_Thread = NULL;
}

CDeviceContrl::~CDeviceContrl(void)
{
}

//// 取我的设备路径
//CString GetMyDevicePath(void)
//{
//	CString path = L"";
//	CString pszDevicePath[24];
//	int nCount = 0;
//	//=================  枚举HID设备  =================
//	HDEVINFO hDevInfoSet;
//	SP_DEVICE_INTERFACE_DATA ifdata;
//	PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail;
//	BOOL bResult;
//
//
//
//	GUID hidGuid;
//	::HidD_GetHidGuid((LPGUID)&hidGuid);					// 取HID设备GUID
//	TCHAR *GUIDString = NULL;
//	GUIDString = _T("a5dcbf10-6530-11d2-901f-00c04fb951ed");
//	UuidFromString((RPC_WSTR)GUIDString, &hidGuid);
//
//	// 取得一个该GUID相关的设备信息集句柄
//	hDevInfoSet = SetupDiGetClassDevs(&hidGuid,				// class GUID 
//		NULL,												// 无关键字 
//		NULL,												// 不指定父窗口句柄 
//		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);				// 目前存在的设备
//	// 失败...
//	if (hDevInfoSet == INVALID_HANDLE_VALUE)
//	{
//		return path;
//	}
//	// 申请设备接口数据空间
//	pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)::GlobalAlloc(LMEM_ZEROINIT, 1024);
//
//	if (pDetail == NULL)
//	{
//		SetupDiDestroyDeviceInfoList(hDevInfoSet);		// 清理句柄
//		return path;
//	}
//	pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
//	bResult = TRUE;
//	// 设备序号=0,1,2... 逐一测试设备接口，到失败为止
//	while (bResult)
//	{
//		ifdata.cbSize = sizeof(ifdata);
//
//		// 枚举符合该GUID的设备接口
//		bResult = ::SetupDiEnumDeviceInterfaces(
//			hDevInfoSet,     // 设备信息集句柄
//			NULL,            // 不需额外的设备描述
//			&hidGuid,        // GUID
//			(ULONG)nCount,	 // 设备信息集里的设备序号
//			&ifdata);        // 设备接口信息
//
//		if (bResult)
//		{
//			// 取得该设备接口的细节(设备路径)
//			bResult = SetupDiGetInterfaceDeviceDetail(
//				hDevInfoSet,    // 设备信息集句柄
//				&ifdata,        // 设备接口信息
//				pDetail,        // 设备接口细节(设备路径)
//				1024,			// 输出缓冲区大小
//				NULL,           // 不需计算输出缓冲区大小(直接用设定值)
//				NULL);          // 不需额外的设备描述
//
//			if (bResult)
//			{
//				// 复制设备路径到输出缓冲区
//				pszDevicePath[nCount] = pDetail->DevicePath; ////<<------------这里的Devicepath可以作为Portname来调用CreateFile()吗? 想用这个CreateFile()返回的handle再来调用Readfile和Writefile.
//				// 调整计数值
//				nCount++;
//			}
//		}
//	}
//	int error = -1;
//	//=======================  查找需要的设备  =======================
//	for (int i = 0; i <nCount; i++)
//	{
//		//if (GetVidAndPid(pszDevicePath[i]))
//		//{
//			path = pszDevicePath[i];
//			/*HANDLE hDevice = CreateFile(path, GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
//			error = GetLastError();*/
//			
//		//}
//	}
//	return path;
//}

HANDLE CDeviceContrl::GetDeviceHanle( LPCWSTR pDeviceName )
{
	DWORD dw = 0;
	HANDLE hDevice = CreateFile(pDeviceName, GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	int error = GetLastError();

	//GetMyDevicePath();

	if( INVALID_HANDLE_VALUE == hDevice )
	{
		return NULL;
	}
	
	return hDevice;
}

void CDeviceContrl::SetDeviceCotrl( ULONG ulIoContrlCode )
{
	BOOL bFlag = FALSE;
	DWORD dw = 0;
	HANDLE hDevice = GetDeviceHanle( _T("\\\\?\\usb") );
	bFlag = DeviceIoControl( hDevice, ulIoContrlCode, NULL, 0, NULL, 0, &dw, 0 );
	int error = GetLastError();
	CloseHandle(hDevice);

	

	if ( ulIoContrlCode == IOCTL_ENJOIN_USB )
	{
		::CreateThread( NULL, 0, Thread, this, 0, &dw );
		
	}
 	else if ( ulIoContrlCode == IOCTL_FREE_USB )
 	{
		m_Enjoin.ControlDisk(TRUE, ENJOIN_USB);
			
 	}

}


BOOL CDeviceContrl::IsDeviceRun()
{
	CHAR szTmp[121] = {0};
	ULONG ulError = 0;
	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if ( schSCManager == 0 ) 
	{
		ulError = GetLastError();
		sprintf(szTmp, "OpenSCManager failed, error code = %d\n", ulError);
		printf(szTmp);
		return FALSE;
	}
	else
	{
		// open the service
		SC_HANDLE schService = OpenService(schSCManager, _T("usbstorfilter"), SERVICE_ALL_ACCESS);
		if ( schService == 0 )  
		{
			ulError = GetLastError();
			sprintf(szTmp, "OpenService failed, error code = %d\n", ulError);
			printf(szTmp);
			return FALSE;
		}
		else
		{
			SERVICE_STATUS ser_sta;
			QueryServiceStatus(schService, &ser_sta);
			if ( ser_sta.dwCurrentState == SERVICE_RUNNING )
			{
				CloseServiceHandle(schService); 
				CloseServiceHandle(schSCManager); 

				return TRUE;
			}
		}
		CloseServiceHandle(schService); 
	}
	CloseServiceHandle(schSCManager); 

	return FALSE;
}

