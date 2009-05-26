#pragma once

class CSettingsDlg : public CDialogImpl<CSettingsDlg>,
    public CWinDataExchange<CSettingsDlg>
{
public:    
	bool IsAuthor, IsSubject, IsTitle, IsKeywords;
	bool Author, Subject, Title, Keywords;

	CButton btnOk, btnCancel;	

public:
    enum { IDD = IDD_SETTINGSDLG };
   
    BEGIN_MSG_MAP(CAdvSettingsDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_HANDLER_EX(IDCANCEL, BN_CLICKED, OnCancel)		
		COMMAND_HANDLER(IDC_checkAuthor, BN_CLICKED, OnBnClickedcheckAuthor)
		COMMAND_HANDLER(IDC_checkSubject, BN_CLICKED, OnBnClickedcheckSubject)
		COMMAND_HANDLER(IDC_checkTitle, BN_CLICKED, OnBnClickedcheckTitle)
		COMMAND_HANDLER(IDC_checkKeywords, BN_CLICKED, OnBnClickedcheckKeywords)
		COMMAND_HANDLER(IDC_btnCancel, BN_CLICKED, OnBnClickedbtnCancel)
		COMMAND_HANDLER(IDC_btnOk, BN_CLICKED, OnBnClickedbtnOk)
	END_MSG_MAP()

    BEGIN_DDX_MAP(CAdvSettingsDlg)
		DDX_CONTROL_HANDLE(IDC_btnOk, btnOk)
		DDX_CONTROL_HANDLE(IDC_btnCancel, btnCancel)
		DDX_CHECK(IDC_checkAuthor, Author)
		DDX_CHECK(IDC_checkSubject, Subject)
		DDX_CHECK(IDC_checkTitle, Title)
		DDX_CHECK(IDC_checkKeywords, Keywords)
    END_DDX_MAP()

public:
    // Notification handlers
    LRESULT OnInitDialog(HWND hwndFocus, LPARAM lParam);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnBnClickedcheckAuthor(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedcheckSubject(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedcheckTitle(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedcheckKeywords(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedbtnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedbtnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};
