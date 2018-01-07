#include "StdAfx.h"
#include "UsbFilter.h"


DWORD WINAPI DeviceManage(LPVOID lp);


CUsbFilter::CUsbFilter(void)
{
	m_bEnjoin = TRUE;
	InitializeCriticalSection(&m_Section);

	DWORD dwIDThread;

	::CreateThread( NULL, 0, DeviceManage, this, 0, &dwIDThread );	
	
}

CUsbFilter::~CUsbFilter(void)
{
	DeleteCriticalSection(&m_Section);
}

DWORD WINAPI DeviceManage(LPVOID lp)
{
	CUsbFilter* pDlg = (CUsbFilter*)lp;

	while (TRUE)
	{
		BOOL bFlag = pDlg->m_DeviceContrl.IsDeviceRun();
		if ( bFlag )
		{
			if ( !pDlg->m_bEnjoin )
			{
				pDlg->m_DeviceContrl.SetDeviceCotrl( IOCTL_FREE_USB );
			}
		}
		Sleep( 500 );
	}
}


void CUsbFilter::OpenUsb()
{
	m_DeviceContrl.SetDeviceCotrl( IOCTL_FREE_USB );

}
void CUsbFilter::CloseUsb()
{
	m_DeviceContrl.SetDeviceCotrl( IOCTL_ENJOIN_USB );
}


