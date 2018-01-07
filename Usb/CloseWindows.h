#ifndef _CLOSEWINDOWS_H
#define _CLOSEWINDOWS_H

DWORD WINAPI Thread(LPVOID lp);
BOOL StrCmpEx( TCHAR* pStr1, TCHAR* pStr2 );
BOOL CALLBACK EnumWinProc( HWND hWnd, LPARAM lp);


#endif