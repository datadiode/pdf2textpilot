// SettingsDlg.cpp : implementation of the CSettingsDlg class

#include "stdafx.h"
#include "resource.h"
#include "SettingsDlg.h"


LRESULT CSettingsDlg::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
    CenterWindow();
	DoDataExchange(TRUE);
	if(IsAuthor) Author = true;
	if(IsSubject) Subject = true;
	if(IsTitle) Title = true;
	if(IsKeywords) Keywords = true;
	DoDataExchange(FALSE);
	return TRUE;
}

LRESULT CSettingsDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{    
    return EndDialog(IDCANCEL);
}

LRESULT CSettingsDlg::OnBnClickedcheckAuthor(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	IsAuthor = !IsAuthor;
	return 0;
}

LRESULT CSettingsDlg::OnBnClickedcheckSubject(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	IsSubject = !IsSubject;
	return 0;
}


LRESULT CSettingsDlg::OnBnClickedcheckTitle(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	IsTitle = !IsTitle;
	return 0;
}

LRESULT CSettingsDlg::OnBnClickedcheckKeywords(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	IsKeywords = !IsKeywords;
	return 0;
}

LRESULT CSettingsDlg::OnBnClickedbtnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	return EndDialog(IDCANCEL);
}

LRESULT CSettingsDlg::OnBnClickedbtnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{	
	return EndDialog(IDOK);
}
