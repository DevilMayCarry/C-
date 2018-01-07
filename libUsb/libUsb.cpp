#include "stdafx.h"
#include "libUsb.h"
#include "UsbFilter.h"

CUsbFilter gMyUsb;

int OnMyCallBack(int nType)
{

	EnterCriticalSection(&gMyUsb.m_Section);

	if (nType == 0)
	{
		//关闭USB接口
		gMyUsb.CloseUsb();

	}
	else if (nType == 1)
	{
		//打开USB接口
		gMyUsb.OpenUsb();

	}

	LeaveCriticalSection(&gMyUsb.m_Section);

	return 1;
}