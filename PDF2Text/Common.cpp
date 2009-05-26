#include "stdafx.h"
#include "Common.h"
#include "resource.h"



int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    if((uMsg == BFFM_INITIALIZED) && lpData)
        ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
    return 0;
}

BOOL RegReadStr(HKEY key, LPCTSTR strParamName, CString* strOut)
{
    HKEY hKey;
    DWORD dwStrSSize = MAX_PATH;
    TCHAR *buf = new TCHAR [MAX_PATH + 1];
    ZeroMemory(buf, sizeof(TCHAR) * (MAX_PATH + 1));
    
    CString strKeyName, strAppName;
	strAppName = L"PDF2Text";
	strKeyName.Format(_T("Software\\Two Pilots\\%s"), strAppName);

    LONG lRes = RegCreateKeyEx(key, strKeyName, 0, 0, 0, KEY_QUERY_VALUE, 0, &hKey, 0);

    if(lRes != ERROR_SUCCESS)
    {
        return FALSE;
    }
    
    lRes = RegQueryValueEx(hKey, strParamName, 0, NULL, (LPBYTE)buf, &dwStrSSize);
    if(lRes != ERROR_SUCCESS)
    {        
        return FALSE;
    }

    (*strOut).Format(_T("%s"), buf);
    RegCloseKey(hKey);
    delete buf;

    return TRUE;
}

bool IsDirectoryExist(LPCTSTR szPath) 
{
	if (!szPath)
		return false;
    DWORD attr = ::GetFileAttributes(szPath);
    return (attr != INVALID_FILE_ATTRIBUTES);
}

bool IsFileExist(LPCTSTR szPath)
{
    bool bRet = false;

    HANDLE hFile = ::CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);	
    if(hFile != INVALID_HANDLE_VALUE)
    {
        bRet = true;
        ::CloseHandle(hFile);
    }

    return bRet;
}

BOOL RegWriteStr(HKEY key, LPCTSTR strParamName, LPCTSTR strValue)
{
    HKEY hKey;	
    CString strKeyName, strAppName;
	strAppName = L"PDF2Text";
    strKeyName.Format(_T("Software\\Two Pilots\\%s"), strAppName);	

    if(ERROR_SUCCESS != RegCreateKeyEx(key, strKeyName, 0, 0, 0, KEY_SET_VALUE , 0, &hKey, 0))
        return FALSE;

	if(ERROR_SUCCESS != RegSetValueEx(hKey, strParamName, 0, REG_SZ, (const BYTE*)(LPCTSTR)strValue, (DWORD)2*wcslen(strValue)))
        return FALSE;
	
    RegCloseKey(hKey);
    return TRUE;
}

void ShowWarning(UINT idMsg, UINT idCap)
{
    CString msg, cap;

    msg.LoadString(idMsg);
	cap.LoadString(idCap);
    MessageBox(NULL, msg, cap, MB_OK | MB_ICONEXCLAMATION);
}

void ShowError(UINT idMsg, UINT idCap)
{
    CString msg, cap;

    msg.LoadString(idMsg);
    cap.LoadString(idCap);
    MessageBox(NULL, msg, cap, MB_OK | MB_ICONERROR);
}

void ShowFormatError(UINT idMsg, LPCTSTR lpszValue, UINT idCap)
{
    CString buf, msg, cap;

    buf.LoadString(idMsg);
    msg.Format(buf, lpszValue);
    cap.LoadString(idCap);
    MessageBox(NULL, msg, cap, MB_OK | MB_ICONERROR);
}

void ShowHelp()
{
	CString strHelpPath;
	CString strAppModuleName;
    
    TCHAR lpszAppModuleName[MAX_PATH];
    ::GetModuleFileNameW(NULL, lpszAppModuleName, MAX_PATH);		
    strAppModuleName = lpszAppModuleName;    

	int ind = strAppModuleName.ReverseFind(_T('\\'));
	strHelpPath = strAppModuleName.Left(ind + 1) + HELP;

	if((int)ShellExecute(NULL, _T("open"), (LPCTSTR)strHelpPath, NULL, NULL, SW_SHOWDEFAULT) <= 32)        
            ShowFormatError(IDS_CANTCREATEPROCESS, (LPCTSTR)strHelpPath, IDS_ERROR);
}