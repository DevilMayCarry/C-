#include "stdafx.h"
#include "libUsb.h"
#include "UsbFilter.h"

CUsbFilter gMyUsb;

int OnMyCallBack(int nType)
{

	EnterCriticalSection(&gMyUsb.m_Section);

	if (nType == 0)
	{
		//�ر�USB�ӿ�
		gMyUsb.CloseUsb();

	}
	else if (nType == 1)
	{
		//��USB�ӿ�
		gMyUsb.OpenUsb();

	}

	LeaveCriticalSection(&gMyUsb.m_Section);

	return 1;
}