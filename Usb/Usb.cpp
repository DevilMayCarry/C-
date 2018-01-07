// Usb.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "UsbFilter.h"

CUsbFilter gMyUsb;

extern "C" __declspec(dllexport) BOOL OnMyCallBack(int nType)
{

	EnterCriticalSection(&gMyUsb.m_Section);

	if(nType == CLOSE_USB)
	{
		//关闭USB接口
		gMyUsb.CloseUsb();

	}
	else if(nType == OPEN_USB)
	{
		//打开USB接口
		gMyUsb.OpenUsb();

	}

	LeaveCriticalSection(&gMyUsb.m_Section);

	return TRUE;
}