// MainDlg.h : interface of the CMainDlg class
#pragma once
#include "stdafx.h"
#include "SettingsDlg.h"
#include "ProgressDlg.h"

#include "Converter.h"


#define PDF_FILTER _T("PDF Files\0*.pdf\0")
#define MASK _T("*.pdf")


class CMainDlg : public CDialogImpl<CMainDlg>, public CDialogResize<CMainDlg>, public CWinDataExchange<CMainDlg>,
				 public CMessageFilter, public CIdleHandler, public CUpdateUI<CMainDlg>
{
private:
	CSettingsDlg dlgSettings;
	CString strLastOpenFolder;
	HWND hProgrWnd;
	

public:
	bool IsAuthor;
	bool IsSubject;
	bool IsTitle;
	bool IsKeywords;

public:	
	bool AreWeDraggingNow, IsAll;
	HCURSOR curDrag;
	UINT nItemDragStart;
	UINT nItemDragEnd;
	int nItemLastHighlighted;
	CMyThread *m_Thread;

	CListViewCtrl SrcFileList, DstFileList;

	CButton btnAddFile, btnAddFolder, btnRemove, btnMoveUp, BtnMoveDown, btnBrowse, btnSettings, btnConvert, btnHelp;
	CString strSaveFolder;

public:
	CMainDlg(void);
	
	enum { IDD = IDD_MAINDLG };
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MSG_WM_SETFOCUS(OnSetFocus)
		MSG_WM_HELP(OnHelp)		
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER_EX(IDC_btnAddFile, BN_CLICKED, OnAddFile)
		COMMAND_HANDLER_EX(IDC_btnAddFolder, BN_CLICKED, OnAddFolder)
		COMMAND_HANDLER_EX(IDC_btnRemove, BN_CLICKED, OnRemove)
		COMMAND_HANDLER_EX(IDC_btnMoveUp, BN_CLICKED, OnUp)
		COMMAND_HANDLER_EX(IDC_BtnMoveDown, BN_CLICKED, OnDown)
		COMMAND_HANDLER_EX(IDC_btnBrowse, BN_CLICKED, OnBrowse)
		COMMAND_HANDLER_EX(IDC_btnSettings, BN_CLICKED, OnSettings)
		COMMAND_HANDLER_EX(IDC_btnHelp, BN_CLICKED, OnbtnHelp)
		COMMAND_HANDLER_EX(IDC_btnConvert, BN_CLICKED, OnConvert)
		NOTIFY_HANDLER_EX(IDC_listInputFile, NM_DBLCLK, OnSrcListDblClick)
		NOTIFY_HANDLER_EX(IDC_listOutputFile, NM_DBLCLK, OnDstListDblClick)
		NOTIFY_HANDLER_EX(IDC_listInputFile, LVN_KEYDOWN, OnSrcListKeyDown)
		NOTIFY_HANDLER_EX(IDC_listInputFile, LVN_BEGINDRAG, OnBeginDrag)		
		CHAIN_MSG_MAP(CDialogResize<CMainDlg>)
		NOTIFY_HANDLER_EX(IDC_listInputFile, LVN_ITEMCHANGED, OnItemChanged)
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(CMainDlg)
		DLGRESIZE_CONTROL(IDC_btnAddFile, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_btnAddFolder, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_btnRemove, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_btnMoveUp, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_BtnMoveDown, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_btnBrowse, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_btnSettings, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_btnHelp, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_btnConvert, DLSZ_MOVE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_checkBox, DLSZ_MOVE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_staticFilesToConvert, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_listInputFile, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_listOutputFile, DLSZ_SIZE_X|DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_editSaveFolder, DLSZ_SIZE_X)
	END_DLGRESIZE_MAP()

	BEGIN_DDX_MAP(CMainDlg)
		DDX_CONTROL_HANDLE(IDC_listInputFile, SrcFileList)		
		DDX_CONTROL_HANDLE(IDC_listOutputFile, DstFileList)
		DDX_TEXT(IDC_editSaveFolder, strSaveFolder)
		DDX_CONTROL_HANDLE(IDC_btnAddFile, btnAddFile)
		DDX_CONTROL_HANDLE(IDC_btnAddFolder, btnAddFolder)
		DDX_CONTROL_HANDLE(IDC_btnRemove, btnRemove)
		DDX_CONTROL_HANDLE(IDC_btnMoveUp, btnMoveUp)
		DDX_CONTROL_HANDLE(IDC_BtnMoveDown, BtnMoveDown)
		DDX_CONTROL_HANDLE(IDC_btnBrowse, btnBrowse)
		DDX_CONTROL_HANDLE(IDC_btnSettings, btnSettings)
		DDX_CONTROL_HANDLE(IDC_btnHelp, btnHelp)
		DDX_CONTROL_HANDLE(IDC_btnConvert, btnConvert)
		DDX_CHECK(IDC_checkBox, IsAll)
	END_DDX_MAP()
	
	void ProcessMessages();	
	void InitCaption();
	void DoDrop();
	void DisableAll(BOOL bEnable);
	bool AddFiles(LPCTSTR lpPath, LPCTSTR lpMask, bool bInc = false);
	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSetFocus(HWND hwndOldFocus);
	LRESULT OnHelp(LPHELPINFO lpHelpInfo);	
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD, WORD wID, HWND, BOOL&);
	LRESULT OnAddFile(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnAddFolder(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnRemove(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnUp(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnDown(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnSettings(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnbtnHelp(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnConvert(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnSrcListDblClick(LPNMHDR pnmh);
	LRESULT OnDstListDblClick(LPNMHDR pnmh);
	LRESULT OnSrcListKeyDown(LPNMHDR pnmh);
	LRESULT OnBeginDrag(LPNMHDR pnmh);
	LRESULT OnItemChanged(LPNMHDR pnmh);	
};