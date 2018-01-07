#include "stdafx.h"
#pragma warning(disable:4996)
#include "FindReg.h"
#include <string.h>
#include "ReEnjoin.h"


FindReg::FindReg()
{
	m_hKey = NULL;
}

FindReg::~FindReg()
{
	RegCloseKey(m_hKey);
}

BOOL FindReg::InitReg(void)
{
 	if( ERROR_SUCCESS != RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T(HKEY_USBSOTR), 0,
 		KEY_READ | KEY_WRITE | KEY_ALL_ACCESS, &m_hKey))
 	{
 		return FALSE;
 	}
	return TRUE;
}


BOOL FindReg::QueryReg( HKEY hKey, LPDWORD pSubCount )
{
	if( ERROR_SUCCESS != RegQueryInfoKey( hKey, NULL, NULL, NULL, pSubCount, NULL, NULL, 
		NULL, NULL, NULL, NULL, NULL))  
	{
		return FALSE;
	}
	return TRUE;
}

BOOL FindReg::EnumReg( HKEY hKey, std::vector<wstring>& v )
{
	DWORD dwSubCount = 0;
	if ( QueryReg(hKey, &dwSubCount) )
	{
		for ( DWORD i = 0; i < dwSubCount; i++)
		{		
			DWORD dwcName = MAX_PATH;
			wstring s;
			TCHAR	szSubName[MAX_PATH] = {0};
			if(ERROR_SUCCESS == RegEnumKeyEx( hKey, i, szSubName, &dwcName, 
				NULL, NULL, NULL, NULL))
			{
				s = szSubName;
				v.push_back(s);
			}
		}
		return TRUE;
	}
	return FALSE;
}

void FindReg::GetRegKey( std::vector<Write_Reg>& v )
{
	HKEY hKey = NULL;
	std::vector<wstring> vs;

	if ( InitReg() )
	{
		if ( EnumReg(m_hKey, vs) )
		{
			for ( int i = 0; i < (int)vs.size(); i++ )
			{
				Write_Reg write;
				memset(&write, 0, sizeof(Write_Reg));
				TCHAR szSubName[MAX_PATH] = {0};
				wcscpy(szSubName, _T(HKEY_USBSOTR));
				wcscpy(szSubName+strlen(HKEY_USBSOTR), vs[i].data());

				if (ERROR_SUCCESS == RegOpenKey( HKEY_LOCAL_MACHINE, szSubName, &hKey))
				{
					wcscpy(szSubName+wcslen(szSubName), _T("\\"));
					write.s = szSubName;
					write.hKey = hKey;
					v.push_back(write);
				}
			}
		}
	}
}

BOOL FindReg::ReadReg()
{
	HKEY	hKey = NULL;
	HKEY	hChildKey = NULL;
	DWORD	dwNameLen = MAX_PATH;
	TCHAR	szValuesName[MAX_PATH] = {0};
	std::vector<wstring> v;
	std::vector<Write_Reg> vWrite;

	GetRegKey( vWrite ) ;

	for (int i = 0; i < (int)vWrite.size(); i++)
	{
		if ( EnumReg( vWrite[i].hKey, v) )
		{
			for (int j = 0; j < (int)v.size(); j++)
			{
				TCHAR szSubName[MAX_PATH] = {0};
				wcscpy(szSubName, vWrite[i].s.data());
				wcscpy(szSubName+wcslen(szSubName), v[j].data());

				if (ERROR_SUCCESS == RegOpenKey( HKEY_LOCAL_MACHINE, szSubName, &hChildKey))
				{
					if(ERROR_SUCCESS == RegQueryValueEx( hChildKey, _T("ClassGUID"), NULL,\
						NULL, (LPBYTE)&szValuesName, &dwNameLen ))
					{
						if (wcscmp(szValuesName, _T("{4D36E967-E325-11CE-BFC1-08002BE10318}")) == 0)
						{
							return TRUE;
						}
					}
				}
			}
			v.clear();
		}
	}
	return FALSE;
}

void FindReg::WriteReg()
{
	HKEY	hKey = NULL;
	HKEY	hChildKey = NULL;
	std::vector<wstring> v;
	std::vector<Write_Reg> vWrite;

	GetRegKey( vWrite ) ;

	for (int i = 0; i < (int)vWrite.size(); i++)
	{
		if ( EnumReg( vWrite[i].hKey, v) )
		{
			for (int j = 0; j < (int)v.size(); j++)
			{
				TCHAR szSubName[MAX_PATH] = {0};
				wcscpy(szSubName, vWrite[i].s.data());
				wcscpy(szSubName+wcslen(szSubName), v[j].data());

				if (ERROR_SUCCESS == RegOpenKey( HKEY_LOCAL_MACHINE, szSubName, &hChildKey))
				{
					TCHAR szValuesName[MAX_PATH] = {0};
					DWORD dwNameLen = MAX_PATH;
					if(ERROR_SUCCESS != RegQueryValueEx( hChildKey, _T("LowerFilters"), NULL,\
						NULL, (LPBYTE)&szValuesName, &dwNameLen ))
					{
						RegSetValueEx( hChildKey, _T("LowerFilters"), 0, REG_SZ, (LPBYTE)_T("usbstorfilter"),
							sizeof(_T("usbstorfilter")));	

						ReEnjoin enjoin;
						enjoin.ControlDisk( TRUE, ENJOIN_DISK );
						enjoin.ControlDisk( FALSE, ENJOIN_DISK );
					}
				}
			}
			v.clear();
		}
	}
}

void FindReg::DeleteReg()
{
	HKEY	hKey = NULL;
	HKEY	hChildKey = NULL;
	std::vector<wstring> v;
	std::vector<Write_Reg> vWrite;

	GetRegKey( vWrite ) ;

	for (int i = 0; i < (int)vWrite.size(); i++)
	{
		if ( EnumReg( vWrite[i].hKey, v) )
		{
			for (int j = 0; j < (int)v.size(); j++)
			{
				TCHAR szSubName[MAX_PATH] = {0};
				wcscpy(szSubName, vWrite[i].s.data());
				wcscpy(szSubName+wcslen(szSubName), v[j].data());

				if (ERROR_SUCCESS == RegOpenKey( HKEY_LOCAL_MACHINE, szSubName, &hChildKey))
				{
					RegDeleteValue(hChildKey, _T("LowerFilters"));
				}
			}
		}
	}
}