// SettingsDlg.cpp : implementation of the CSettingsDlg class

#include "stdafx.h"
#include "resource.h"
#include "PasswordDlg.h"


LRESULT CPasswordDlg::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
    CenterWindow();
	
	CString buf;
	buf.LoadString(IDS_PROTECTED);
    strCurFile.Format(buf, (LPCTSTR)strCurFile);
	DoDataExchange(FALSE);
	return TRUE;
}

LRESULT CPasswordDlg::OnIgnore(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{   	
    return EndDialog(IDCANCEL);
}

LRESULT CPasswordDlg::OnIgnoreAll(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{  
	IsIgnoreAll = true;
    return EndDialog(IDCANCEL);
}

LRESULT CPasswordDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	DoDataExchange(TRUE);	
    return EndDialog(IDOK);
}