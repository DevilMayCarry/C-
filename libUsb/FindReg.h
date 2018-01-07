#ifndef _FINDREG_H
#define _FINDREG_H

#pragma once
#include <Windows.h>
#include <winreg.h>
#include <tchar.h>
#include <vector>


using namespace std;

#define HKEY_USBSOTR    "SYSTEM\\CurrentControlSet\\Enum\\USBSTOR\\"

typedef struct _tagWrite_Reg 
{
	wstring  s;
	HKEY	hKey;

}Write_Reg;

class FindReg
{
public:
	FindReg();
	~FindReg();

	BOOL ReadReg();	
	void WriteReg();
	void DeleteReg();

protected:
	HKEY		m_hKey;

	BOOL InitReg();
	BOOL QueryReg( HKEY hKey, LPDWORD pSubCount );
	BOOL EnumReg( HKEY hKey, std::vector<wstring>& v );
	void GetRegKey(std::vector<Write_Reg>& v );

};

#endif