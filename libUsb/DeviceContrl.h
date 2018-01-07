#ifndef _DEVICE_H
#define _DEVICE_H
#pragma once

#include <Windows.h>
#include <winioctl.h>
#include <winsvc.h>
#include <vector>
#include "ReEnjoin.h"

using namespace std;

#define IOCTL_ENJOIN_USB CTL_CODE(FILE_DEVICE_DISK, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_FREE_USB CTL_CODE(FILE_DEVICE_DISK, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

class CDeviceContrl
{
public:
	CDeviceContrl(void);
	~CDeviceContrl(void);

	ReEnjoin	m_Enjoin;

	BOOL		IsDeviceRun();
	void		SetDeviceCotrl( ULONG ulIoContrlCode );

protected:

	HANDLE		m_Thread;
	DWORD		m_IDThread;
	HANDLE		GetDeviceHanle(LPCWSTR pDeviceName);

};

#endif