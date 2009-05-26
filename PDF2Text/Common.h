#pragma once

#ifndef __COMMON_H__
#define __COMMON_H__

#define HELP _T("Help.chm")


int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
BOOL RegReadStr(HKEY key, LPCTSTR strParamName, CString* strOut);
BOOL RegWriteStr(HKEY key, LPCTSTR strParamName, LPCTSTR strValue);
bool IsDirectoryExist(LPCTSTR szPath);
bool IsFileExist(LPCTSTR szPath);
void ShowWarning(UINT idMsg, UINT idCap);
void ShowError(UINT idMsg, UINT idCap);
void ShowFormatError(UINT idMsg, LPCTSTR lpszValue, UINT idCap);
void ShowHelp();

#endif