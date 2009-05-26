#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"


CMainDlg::CMainDlg(void)
{
}

void CMainDlg::ProcessMessages()
{
    MSG msg;
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void CMainDlg::DisableAll(BOOL bEnable)
{
	SrcFileList.EnableWindow(bEnable);
	DstFileList.EnableWindow(bEnable);	
	btnAddFile.EnableWindow(bEnable);
	btnAddFolder.EnableWindow(bEnable);

	if(!bEnable)
	{
		btnRemove.EnableWindow(bEnable);
		btnMoveUp.EnableWindow(bEnable);
		BtnMoveDown.EnableWindow(bEnable);
	}

	btnBrowse.EnableWindow(bEnable);
	btnSettings.EnableWindow(bEnable);
	btnHelp.EnableWindow(bEnable);
	btnConvert.EnableWindow(bEnable);	
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_LBUTTONUP && pMsg->hwnd == SrcFileList.m_hWnd)
    {
        if(AreWeDraggingNow)
        {
            if(nItemLastHighlighted >= 0)
            {
                SrcFileList.SetItemState(nItemLastHighlighted, 0, LVIS_SELECTED);
                nItemLastHighlighted = -1;
            }
            ::ReleaseCapture();
            AreWeDraggingNow = false;
            POINT pt = pMsg->pt;
            ::ScreenToClient(SrcFileList.m_hWnd, &pt);
            UINT uFlags;
            int nItem = SrcFileList.HitTest(pt, &uFlags);
            if(uFlags & LVHT_ONITEMLABEL)
            {
                nItemDragEnd = nItem;
                DoDrop();
            }
        }
    }
	if(pMsg->message == WM_MOUSEMOVE && pMsg->hwnd == SrcFileList.m_hWnd)
    {
        HCURSOR hCur;
        if(AreWeDraggingNow)
        {
            hCur = curDrag;
            
            POINT pt = pMsg->pt;
            ::ScreenToClient(SrcFileList.m_hWnd, &pt);
            UINT uFlags, uState;
            int nItem = SrcFileList.HitTest(pt, &uFlags);
            uState = SrcFileList.GetItemState(nItem, LVIS_SELECTED);
            if((uFlags & LVHT_ONITEMLABEL) && !(uState & LVIS_SELECTED) && (nItem > -1) && (nItemLastHighlighted != nItem))
            {
                SrcFileList.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
                if(nItemLastHighlighted > -1)
                {
                    SrcFileList.SetItemState(nItemLastHighlighted, 0, LVIS_SELECTED);
                }
                nItemLastHighlighted = nItem;
            }
            SrcFileList.EnsureVisible(nItem, FALSE);
        }
        else
            hCur = LoadCursor(NULL, IDC_ARROW);
        SetCursor(hCur);
    }

	if(m_Thread != 0)
	{	
		if(m_Thread->IsRunning() && m_Thread->prDlg.bUserCanceled)
		{
			m_Thread->Abort();
			DisableAll(TRUE);

			UINT nTotal = m_Thread->arrOutputFile.GetCount();

			if(m_Thread->fileSettings.AllToOne)
				DstFileList.InsertItem(0, m_Thread->arrOutputFile[0]);
			else
				for(UINT i = 0; i < nTotal; ++i)
					DstFileList.InsertItem(i, m_Thread->arrOutputFile[i]);

			m_Thread->prDlg.bUserCanceled = false;
		}

		if(m_Thread->bFinish)
		{			
			UINT nTotal = m_Thread->arrOutputFile.GetCount();

			if(m_Thread->fileSettings.AllToOne)
				DstFileList.InsertItem(0, m_Thread->arrOutputFile[0]);
			else
				for(UINT i = 0; i < nTotal; ++i)
					DstFileList.InsertItem(i, m_Thread->arrOutputFile[i]);

			m_Thread->bFinish = false;
			DisableAll(TRUE);
			SetFocus();
		}		
	}
	

    return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
    return FALSE;
}

void CMainDlg::InitCaption()
{
	DoDataExchange(FALSE);

	// set style for list
    DstFileList.ModifyStyle(0, LVS_SINGLESEL);
    SrcFileList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);
    DstFileList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);

	//add columns to list
	RECT r;
    SrcFileList.GetClientRect(&r);
    SrcFileList.InsertColumn(0, L"FileName", 0, r.right - r.left - 16);
    DstFileList.InsertColumn(0, L"FileName", 0, r.right - r.left - 16);	
}

void CMainDlg::DoDrop()
{
    int dif = nItemDragStart - nItemDragEnd;
    int nItems = SrcFileList.GetItemCount();

    if(dif > 0) //drag up
    {
        for(int i=0; i<nItems; i++)
        {
            UINT uFlag = SrcFileList.GetItemState(i, LVIS_SELECTED | LVIS_FOCUSED);
            if(uFlag & LVIS_SELECTED)
            {
                if(i - dif >= 0)
                {
                    TCHAR buf[MAX_PATH + 1];
                    SrcFileList.GetItemText(i, 0, buf, MAX_PATH);
                    SrcFileList.InsertItem(i - dif, buf);
                    SrcFileList.DeleteItem(i + 1);
                    UINT uNewState = LVIS_SELECTED;
                    if(uFlag & LVIS_FOCUSED)
                    {
                         uNewState |= LVIS_FOCUSED;
                         nItemDragStart -= dif;
                    }
                    SrcFileList.SetItemState(i - dif, uNewState, LVIS_SELECTED | LVIS_FOCUSED);
                }
                else
                    break;
            }
        }
    }
    else //drag down
    {
        for(int i=nItems - 1; i>=0; i--)
        {
            UINT uFlag = SrcFileList.GetItemState(i, LVIS_SELECTED | LVIS_FOCUSED);
            if(uFlag & LVIS_SELECTED)
            {
                if(i - dif < nItems)
                {
                    TCHAR buf[MAX_PATH + 1];
                    SrcFileList.GetItemText(i, 0, buf, MAX_PATH);
                    SrcFileList.InsertItem(i - dif + 1, buf);
                    SrcFileList.DeleteItem(i);
                    UINT uNewState = LVIS_SELECTED;
                    if(uFlag & LVIS_FOCUSED)
                    {
                         uNewState |= LVIS_FOCUSED;
                         nItemDragStart -= dif;
                    }
                    SrcFileList.SetItemState(i - dif, uNewState, LVIS_SELECTED | LVIS_FOCUSED);
                }
                else
                    break;
            }
        }
    }	
}

bool CMainDlg::AddFiles(LPCTSTR lpPath, LPCTSTR lpMask, bool bInc)
{
    int ret=0;
    TCHAR name1[MAX_PATH + 1];
    WIN32_FIND_DATA info;
    HANDLE hp;
    TCHAR *cp;
    bool bFound = true;	

	swprintf_s(name1, MAX_PATH + 1, _T("%s\\%s"), lpPath, lpMask);

    // search for files    
    hp = FindFirstFile(name1, &info);
    if(!hp || hp == INVALID_HANDLE_VALUE)
    {
        // no files found
        bFound = false;
    }

    if(bFound)
    {
        do
        {
            cp = info.cFileName;
            if(cp[1] == 0 && *cp == _T('.'))
                continue;
            else 
                if(cp[2] == 0 && *cp == _T('.') && cp[1] == _T('.'))
                    continue;

            swprintf_s(name1, MAX_PATH + 1, _T("%s\\%s"), lpPath, info.cFileName);

            CString strNewItem = name1;
            strNewItem.Replace(_T("\\\\"), _T("\\"));
          
            if(!(info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                int n = SrcFileList.GetItemCount() + 1;
                if(n <= MAX_FILES)
                {
                    LVFINDINFO fi;
                    fi.flags = LVFI_STRING;
                    fi.psz = strNewItem;
                    if(SrcFileList.FindItem(&fi, -1) > -1)
                    {
                        //ShowWarning(IDS_FILEALREADYADDED, IDS_WARNING);
                    }
                    else
                    {
                        SrcFileList.InsertItem(n++, strNewItem);
                    }
                }
                else
                {
                    ShowWarning(IDS_FILESLIMITEXCEDED, IDS_WARNING);
                    FindClose(hp);
                    return false;
                }
            }

        }while(FindNextFile(hp, &info));
        FindClose(hp);
    }
    
    // now search for (sub)folders    
    if(bInc)
    {
        swprintf_s(name1, MAX_PATH + 1, _T("%s\\*"), lpPath);

        hp = FindFirstFile(name1, &info);
        if(!hp || hp == INVALID_HANDLE_VALUE)
        {
            // no items found
            return false;
        }

        do
        {
            int n = SrcFileList.GetItemCount() + 1;
            if(n > MAX_FILES)
                break;

            cp = info.cFileName;
            if(cp[1] == 0 && *cp == _T('.'))
                continue;
            else 
                if(cp[2] == 0 && *cp == _T('.') && cp[1] == _T('.'))
                    continue;

            swprintf_s(name1, MAX_PATH + 1, _T("%s\\%s"), lpPath, info.cFileName);

            CString strNewItem = name1;
            strNewItem.Replace(_T("\\\\"), _T("\\"));

            if(info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
            {
                AddFiles(strNewItem, lpMask, true);
            }
        }while(FindNextFile(hp, &info));
        FindClose(hp);
    }
    return true;
}

LRESULT CMainDlg::OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
{	
	CenterWindow();
	SetFocus();
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	IsAuthor = false;
	IsSubject = false;
	IsTitle = false;
	IsKeywords = false;

	m_Thread = 0;

	IsAll = false;

	InitCaption();

    AreWeDraggingNow = false;
    nItemDragStart = -1;
    nItemDragEnd = -1;
    curDrag = LoadCursor(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDC_DRAGCUR));
    nItemLastHighlighted = -1;
	DlgResize_Init(true, true);

	OnItemChanged(NULL);
	
    // read registry
    BOOL res = RegReadStr(HKEY_CURRENT_USER, _T("SaveFolder"), &strSaveFolder);
    if(FALSE == res || !IsDirectoryExist(strSaveFolder))
    {
        TCHAR buf[MAX_PATH];
        SHGetSpecialFolderPath(NULL, buf, CSIDL_PERSONAL, TRUE);
        strSaveFolder.Format(_T("%s"), buf);
    }
    
    res = RegReadStr(HKEY_CURRENT_USER, _T("LastOpenFolder"), &strLastOpenFolder);
    if(FALSE == res || !IsDirectoryExist(strLastOpenFolder))
    {
        TCHAR buf[MAX_PATH];
        SHGetSpecialFolderPath(NULL, buf, CSIDL_PERSONAL, TRUE);
        strLastOpenFolder.Format(_T("%s"), buf);
    }

    DoDataExchange(FALSE);	
    
	return TRUE;
}

LRESULT CMainDlg::OnSetFocus(HWND hwndOldFocus)
{
    HWND hBut = ::GetDlgItem(hProgrWnd, IDCANCEL);
    if(hProgrWnd || hwndOldFocus == hBut)    
        ::SetFocus(hwndOldFocus);
    
    return 0;
}

LRESULT CMainDlg::OnHelp(LPHELPINFO lpHelpInfo)
{
	ShowHelp();	
	return TRUE;
}

LRESULT CMainDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    if(GetFocus() == GetDlgItem(IDC_listOutputFile))
    {
        NMITEMACTIVATE q;
        q.iItem = DstFileList.GetSelectedIndex();
        q.iSubItem = 0;

        OnDstListDblClick(LPNMHDR(&q));
    }

    return 0;
}

LRESULT CMainDlg::OnCancel(WORD, WORD wID, HWND, BOOL&)
{
	DoDataExchange(TRUE);

    // save to REGISTRY
    if(FALSE == RegWriteStr(HKEY_CURRENT_USER, _T("SaveFolder"), strSaveFolder))
        ShowError(IDS_REGSAVEFAIL, IDS_ERROR);
    
    if(FALSE == RegWriteStr(HKEY_CURRENT_USER, _T("LastOpenFolder"), strLastOpenFolder))
        ShowError(IDS_REGSAVEFAIL, IDS_ERROR);

    DestroyWindow();
    ::PostQuitMessage(wID);
	
	return 0;
}

LRESULT CMainDlg::OnbtnHelp(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	OnHelp(0);
	return 0;
}

LRESULT CMainDlg::OnAddFile(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	CString str;
    int n;
    n = SrcFileList.GetItemCount();

    if(n + 1 > MAX_FILES)
    {
        ShowWarning(IDS_FILESLIMITEXCEDED, IDS_WARNING);
        return 0;
    }

	CFileDialogEx dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, PDF_FILTER, this->m_hWnd);
    dlg.m_ofn.Flags |= OFN_EXPLORER;	

	if(IDOK == dlg.DoModal())
    {
        WTL::POSITION pos = dlg.GetStartPosition();		
        while(pos)
        {
            if(n + 1 > MAX_FILES)
            {
                ShowWarning(IDS_FILESLIMITEXCEDED, IDS_WARNING);
                break;
            }
            CString str = dlg.GetNextPathName(pos);
            
            LVFINDINFO fi;
            fi.flags = LVFI_STRING;
            fi.psz = str;

            if(SrcFileList.FindItem(&fi, -1) > -1)
            {
                
            }
			else
			{
				SrcFileList.InsertItem(n++, str);
                btnConvert.EnableWindow(TRUE);
			}
        }

		TCHAR buf[MAX_PATH];
		memset(buf, 0, sizeof(TCHAR)*MAX_PATH);
		int nLastInd = SrcFileList.GetItemCount() - 1;
		SrcFileList.GetItemText(nLastInd, 0, buf, MAX_PATH); 
		if(!IsFileExist(buf))
			SrcFileList.DeleteItem(nLastInd);
    }
	
	OnItemChanged(NULL);
    BOOL res = SrcFileList.EnsureVisible(SrcFileList.GetItemCount() - 1, FALSE);

	return 0;
}

LRESULT CMainDlg::OnAddFolder(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
    CFolderDialogEx dlg(m_hWnd);	
	dlg.m_lpstrInitialFolder = strLastOpenFolder;
    dlg.ShowIncludeSubfolders(true);

    if(dlg.DoModal() == IDOK)
    {
        strLastOpenFolder = dlg.GetFolderPath();
        bool bInc = dlg.IncludeSubfolders();		
        AddFiles(strLastOpenFolder, MASK, bInc);
        btnConvert.EnableWindow(SrcFileList.GetItemCount() > 0);
    }

	return 0;
}

LRESULT CMainDlg::OnRemove(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
    int nRes = 0;
    DoDataExchange(TRUE);

    int nTotalSel = SrcFileList.GetSelectedCount();
    if(nTotalSel == 0)
        return nRes;

    UINT uFlags;
    int nTotal = SrcFileList.GetItemCount();
    for(int i=0; i<nTotal; i++)
    {
        uFlags = SrcFileList.GetItemState(i, LVIS_SELECTED | LVIS_FOCUSED);
        if(uFlags & LVIS_SELECTED)
        {
            SrcFileList.DeleteItem(i);
            i--;
            nTotalSel--;
        }
    }
	if(SrcFileList.GetItemCount() == 0)
		btnConvert.EnableWindow(FALSE);

    return nRes;
}

LRESULT CMainDlg::OnUp(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
    int nRes = 0;
    DoDataExchange(TRUE);

    int nTotalSel = SrcFileList.GetSelectedCount();
    if(nTotalSel == 0)
        return nRes;

    UINT uFlags;
    for(int i=1; i<nTotalSel; i++) //moveup is impossible for first element
    {
        uFlags = SrcFileList.GetItemState(i, LVIS_SELECTED | LVIS_FOCUSED);
        if(uFlags & LVIS_SELECTED)
        {
            nItemDragStart = i;
            break;
        }
    }

    nItemDragEnd = nItemDragStart - 1;
    DoDrop();
    SrcFileList.SetFocus();

    return nRes;
}

LRESULT CMainDlg::OnDown(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
    int nRes = 0;
    DoDataExchange(TRUE);

    int nTotalSel = SrcFileList.GetSelectedCount();
    if(nTotalSel == 0)
        return nRes;

    UINT uFlags;
    for(int i=nTotalSel-2; i>=0; i--) //movedown is impossible for last element
    {
        uFlags = SrcFileList.GetItemState(i, LVIS_SELECTED | LVIS_FOCUSED);
        if(uFlags & LVIS_SELECTED)
        {
            nItemDragStart = i;
            break;
        }
    }

    nItemDragEnd = nItemDragStart + 1;
    DoDrop();
    SrcFileList.SetFocus();

    return nRes;
}

LRESULT CMainDlg::OnBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	CString strTitle;
    TCHAR szInitDir[MAX_PATH] = {0};
    memcpy(szInitDir, strSaveFolder, strSaveFolder.GetLength());

    TCHAR szFolPath[MAX_PATH] = {0};
    BROWSEINFO bi = {0};
    bi.hwndOwner  = m_hWnd;
    bi.pidlRoot   = NULL;
    bi.lpszTitle  = strTitle;
    bi.ulFlags    = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;
    bi.lpfn       = BrowseCallbackProc;
    bi.lParam     = (LPARAM)szInitDir;
    bi.pszDisplayName = szFolPath;

    LPITEMIDLIST pItemIDList = ::SHBrowseForFolder(&bi);
    if(pItemIDList)
    {
        if(::SHGetPathFromIDList(pItemIDList, szFolPath))
        {
            strSaveFolder = szFolPath;
            DoDataExchange(FALSE);
        }
        
        // free memory used
        IMalloc *imalloc = 0;
        if( S_OK == SHGetMalloc(&imalloc))
        {
            imalloc->Free(pItemIDList);
            imalloc->Release();
        }
    }

    return 0;
}

LRESULT CMainDlg::OnSettings(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	dlgSettings.IsAuthor = IsAuthor;
	dlgSettings.IsSubject = IsSubject;
	dlgSettings.IsTitle = IsTitle;
	dlgSettings.IsKeywords = IsKeywords;
	
    int res = dlgSettings.DoModal();
	if(res == IDOK)
	{
		IsAuthor = dlgSettings.IsAuthor;
		IsSubject = dlgSettings.IsSubject;
		IsTitle = dlgSettings.IsTitle;
		IsKeywords = dlgSettings.IsKeywords;
	}

    return 0;   
}

LRESULT CMainDlg::OnConvert(WORD wNotifyCode, WORD wID, HWND hWndCtl)
{
	if(!IsDirectoryExist(strSaveFolder))
	{
		ShowFormatError(IDS_FOLDERNOTEXISTS, strSaveFolder, IDS_ERROR);
		return 0;
	}

	if(!m_Thread)
		delete m_Thread;

	m_Thread = new CMyThread;

	if(m_Thread->prDlg !=0)
	{
		m_Thread->prDlg.ShowWindow(SW_HIDE);
		m_Thread->prDlg.DestroyWindow();
	}

	m_Thread->m_hWnd = m_hWnd;
	DstFileList.DeleteAllItems();
	
	UINT nTotal = SrcFileList.GetItemCount();

	m_Thread->prDlg.iTotalSteps = 2*nTotal + 1;
	m_Thread->prDlg.strProcessingFile = L"Creating file list...";
	m_Thread->prDlg.Create(m_hWnd);
	m_Thread->prDlg.ShowWindow(SW_SHOW);

	for(UINT i=0; i<nTotal; i++)
	{
		TCHAR *buf = new TCHAR[MAX_PATH];
		memset(buf, 0, MAX_PATH);
		SrcFileList.GetItemText(i, 0, buf, MAX_PATH);
		CString strSrc = buf;

		if(!IsFileExist(buf))
		{
			ShowFormatError(IDS_CANTCREATEPROCESS, buf, IDS_ERROR);
			m_Thread->prDlg.ShowWindow(SW_HIDE);
			m_Thread->prDlg.DestroyWindow();
			DisableAll(TRUE);
			delete m_Thread;
			return 0;
		}
		delete buf;
		
		m_Thread->arrInputFileName.Add(strSrc);

		CString strCurFile = strSrc;
		strCurFile = strCurFile.Right(strCurFile.GetLength() - strCurFile.ReverseFind(_T('\\')) - 1);

		CString FileName;
		FileName = strSaveFolder + "\\" + strCurFile + ".txt";

		m_Thread->arrOutputFileName.Add(FileName);
	}

	DoDataExchange(TRUE);

	m_Thread->fileSettings.AllToOne = IsAll;
	m_Thread->fileSettings.Author = IsAuthor;
	m_Thread->fileSettings.Keywords = IsKeywords;
	m_Thread->fileSettings.Subject = IsSubject;
	m_Thread->fileSettings.Title = IsTitle;

	CLSID clsid_PDFCreatorPilot;
    CLSIDFromProgID(OLESTR("PDFCreatorPilot.PDFDocument4"), &clsid_PDFCreatorPilot);

	IPDFDocument4 *PDF = 0;
	::CoCreateInstance(clsid_PDFCreatorPilot, NULL, CLSCTX_ALL, __uuidof(IPDFDocument4), (LPVOID*)&PDF);

	m_Thread->PDF = PDF;
	m_Thread->SetAutoClean();
	m_Thread->bFinish = false;

	DisableAll(FALSE);

	m_Thread->Start();

	return 0;
}

LRESULT CMainDlg::OnSrcListDblClick(LPNMHDR pnmh)
{
    //for ListView - (LPNMITEMACTIVATE)pnmh
    //for StatusBar - (LPNMMOUSE)pnmh
    int iIndex = ((LPNMITEMACTIVATE)pnmh)->iItem;
    int iSubIndex = ((LPNMITEMACTIVATE)pnmh)->iSubItem;
    if(iIndex == -1)    
        OnAddFile(0, 0, 0);
    else
    {
        TCHAR buf[MAX_PATH];
        SrcFileList.GetItemText(iIndex, iSubIndex, buf, MAX_PATH);

        if((int)ShellExecute(NULL, _T("open"), buf, NULL, NULL, SW_SHOWDEFAULT) <= 32)        
            ShowFormatError(IDS_CANTCREATEPROCESS, buf, IDS_ERROR);        
    }
    return 0;
}

LRESULT CMainDlg::OnDstListDblClick(LPNMHDR pnmh)
{
    //for ListView - (LPNMITEMACTIVATE)pnmh
    //for StatusBar - (LPNMMOUSE)pnmh
    int iIndex = ((LPNMITEMACTIVATE)pnmh)->iItem;
    int iSubIndex = ((LPNMITEMACTIVATE)pnmh)->iSubItem;
    if(iIndex == -1)    
        OnBrowse(0, 0, 0);    
    else
    {
        TCHAR buf[MAX_PATH];
        DstFileList.GetItemText(iIndex, iSubIndex, buf, MAX_PATH);

        if((int)ShellExecute(NULL, _T("open"), buf, NULL, NULL, SW_SHOWDEFAULT) <= 32)        
            ShowFormatError(IDS_CANTCREATEPROCESS, buf, IDS_ERROR);        
    }
    return 0;
}

LRESULT CMainDlg::OnSrcListKeyDown(LPNMHDR pnmh)
{
    LPNMLVKEYDOWN pnkd = (LPNMLVKEYDOWN)pnmh;
    if(pnkd->wVKey == VK_DELETE)    
        OnRemove(0, 0, NULL);
    
    if(pnkd->wVKey == VK_INSERT)    
        OnAddFile(0, 0, NULL);
    
    return 0;
}

LRESULT CMainDlg::OnBeginDrag(LPNMHDR pnmh)
{
    int iItem = ((LPNMLISTVIEW)pnmh)->iItem;

    ::SetCapture(SrcFileList.m_hWnd);
    AreWeDraggingNow = true;
    nItemDragStart = iItem;

    return 0;
}

LRESULT CMainDlg::OnItemChanged(LPNMHDR pnmh)
{
    LPNMLISTVIEW pnmv = (LPNMLISTVIEW)pnmh;

    //If the iItem member of the structure pointed to by pnmv is -1, 
    //the change has been applied to all items in the list view.
    UINT nTotal = SrcFileList.GetSelectedCount();
    int nItemLast = SrcFileList.GetItemCount() - 1;
    UINT uState;
    BOOL bMayUp, bMayDown;

    btnRemove.EnableWindow(nTotal > 0);

    if(nTotal > 0)
    {
        uState = SrcFileList.GetItemState(0, LVIS_SELECTED);
        bMayUp = !(uState & LVIS_SELECTED);
        uState = SrcFileList.GetItemState(nItemLast, LVIS_SELECTED);
        bMayDown = !(uState & LVIS_SELECTED);
    }
    else    
        bMayUp = bMayDown = FALSE;
    

    btnMoveUp.EnableWindow(bMayUp);
    BtnMoveDown.EnableWindow(bMayDown);

    return 0;
}