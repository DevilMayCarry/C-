#pragma once

#include "stdafx.h"
#include "DeviceContrl.h"

class CUsbFilter
{
public:
	CUsbFilter(void);
	~CUsbFilter(void);

	BOOL			m_bEnjoin;

	CDeviceContrl	m_DeviceContrl;

	CRITICAL_SECTION m_Section;

	void OpenUsb();
	void CloseUsb();
};
