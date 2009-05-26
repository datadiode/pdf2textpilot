#pragma once

class CPasswordDlg : public CDialogImpl<CPasswordDlg>,
    public CWinDataExchange<CPasswordDlg>
{
public:
	CString strCurFile;
	CString strPassword;
	bool IsIgnoreAll;
	
public:
    enum { IDD = IDD_PASSWORDDLG };

    
    BEGIN_MSG_MAP(CPasswordDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_HANDLER_EX(IDC_btnIgnore, BN_CLICKED, OnIgnore)
		COMMAND_HANDLER_EX(IDC_btnIgnoreAll, BN_CLICKED, OnIgnoreAll)
		COMMAND_HANDLER_EX(IDC_btnOk, BN_CLICKED, OnOK)
    END_MSG_MAP()

    BEGIN_DDX_MAP(CPasswordDlg)		
		DDX_TEXT(IDC_staticCurFile, strCurFile)
		DDX_TEXT(IDC_editPassword, strPassword)
    END_DDX_MAP()

public:
    // Notification handlers
    LRESULT OnInitDialog(HWND hwndFocus, LPARAM lParam);
    LRESULT OnIgnore(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnIgnoreAll(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl);
};