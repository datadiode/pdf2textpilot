// ProgressDlg.cpp : implementation of the CProgressDlg class

#include "stdafx.h"
#include "resource.h"
#include "ProgressDlg.h"

CProgressDlg::CProgressDlg() : CDialogImpl<CProgressDlg>()
{
    bUserCanceled = false;
}

CProgressDlg::~CProgressDlg()
{
	bUserCanceled = false;
}

LRESULT CProgressDlg::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
    DoDataExchange(FALSE);    

    wndProgress1.SetRange(0, iTotalSteps);
    CenterWindow();

    return TRUE; // set focus to default control
}

LRESULT CProgressDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
    CloseDialog(wID);
    bUserCanceled = true;
    ProcessMessages();
    return 0;
}

void CProgressDlg::CloseDialog(int nVal)
{
    DestroyWindow();
    ::PostQuitMessage(nVal);
}

void CProgressDlg::ProcessMessages()
{
    MSG msg;
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void CProgressDlg::StepProgress(int iStep)
{
    wndProgress1.SetPos(wndProgress1.GetPos() + iStep);
    wndProgress1.SetStep(iStep);
    //wndProgress1.StepIt();
    ProcessMessages();
}