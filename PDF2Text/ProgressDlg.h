#pragma once
#include "resource.h"

class CProgressDlg : public CDialogImpl<CProgressDlg>, //public CMessageThreadImpl<CProgressDlg>,
    public CWinDataExchange<CProgressDlg>
{
public:
	CProgressBarCtrl wndProgress1;
	CString strProcessingFile;	
	int iTotalSteps;
	bool bUserCanceled;

public:
    enum { IDD = IDD_PROGRESSDLG };

    CProgressDlg();
    ~CProgressDlg();

    BEGIN_MSG_MAP(CProgressDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_HANDLER_EX(IDCANCEL, BN_CLICKED, OnCancel)
		COMMAND_HANDLER_EX(IDC_btnCancel, BN_CLICKED, OnCancel)
    END_MSG_MAP()

    BEGIN_DDX_MAP(CProgressDlg)
		DDX_CONTROL_HANDLE(IDC_PROGRESS1, wndProgress1)
		DDX_TEXT(IDC_STATIC1, strProcessingFile)		
    END_DDX_MAP()

public:
    // Notification handlers
    LRESULT OnInitDialog(HWND hwndFocus, LPARAM lParam);
    LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl);
    void CloseDialog(int nVal);
	void ProcessMessages();
	void StepProgress(int iStep);	
};