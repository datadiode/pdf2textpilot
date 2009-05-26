#pragma once

// Identifier of the upper static control within Browse for Folder dialog
// Do not change.
#define IDC_DIALOG_UPPER_STATIC                 0x00003742

// Identifier of the "Include subfolders" checkbox
// Value can be changed
#define IDC_INCLUDE_SUBFOLDERS                  101

class CFolderDialogEx : public CFolderDialogImpl<CFolderDialogEx>
{
public:
    typedef CFolderDialogEx T;

    CFolderDialogEx(HWND hWndParent = 0, LPCTSTR lpstrTitle = 0, UINT uFlags = BIF_RETURNONLYFSDIRS)
        : CFolderDialogImpl<CFolderDialogEx>(hWndParent, lpstrTitle, uFlags)
    {
        m_bShowIncludeSubfolders = false;
        m_bIncludeSubfolders = false;

        m_sCheckBoxCaption = _T("Include subfolders");
        
        m_bInitialized = false;
    }

    void ShowIncludeSubfolders(bool bShow)
    {
        m_bShowIncludeSubfolders = bShow;
    }

    void SetIncludeSubfolders(bool bInclude)
    {
        m_bIncludeSubfolders = bInclude;
    }

    bool IncludeSubfolders()
    {
        return m_bIncludeSubfolders;
    }

    void SetCheckBoxCaption(CString sCaption)
    {
        m_sCheckBoxCaption = sCaption;
    }

    void SelectFolder(CString sFolder)
    {
        m_sSelectedFolder = sFolder;
    }

    CString GetSelectedFolder()
    {
        return m_sSelectedFolder;
    }

    static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
    {
        CallInfo* pci = (CallInfo*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
        switch(uMsg)
        {
            case WM_COMMAND: 
                if (LOWORD(lParam) == IDC_INCLUDE_SUBFOLDERS)
                {
                    T* pT = (T*)pci->pCallee;
                    pT->OnCheck(::IsDlgButtonChecked(hWnd, IDC_INCLUDE_SUBFOLDERS) == TRUE);
                }
                break; 
        }

        // Retrieve original window procedure pointer from the user data value.
        WNDPROC OriginalWndProc = pci->OriginalWndProc;
        if (!OriginalWndProc)
            OriginalWndProc = (WNDPROC)::GetWindowLongPtr(hWnd, GWLP_WNDPROC); 
    
        return CallWindowProc(OriginalWndProc, hWnd, uMsg, lParam, lpData);
    }

    void OnInitialized()
    {
        m_ci.OriginalWndProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG)BrowseCallbackProc);
        m_ci.pCallee = this;

        // Save current window procedure pointer as user data value
        ::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG)&m_ci);

        CreateCheckBox();
        
        if (m_bShowIncludeSubfolders)
        {
            ShowWindow(m_hwndCheckBox, SW_SHOW);
            SetWindowText(m_hwndCheckBox, (LPCTSTR)m_sCheckBoxCaption);

            CButton btnCheck = m_hwndCheckBox;
            if (m_bIncludeSubfolders)
                btnCheck.SetCheck(BST_CHECKED);
            else
                btnCheck.SetCheck(BST_UNCHECKED);

            if (m_sSelectedFolder.GetLength() != 0)
                SetSelection(m_sSelectedFolder);
        }

        m_bInitialized = true;
    }

    void OnSelChanged(LPITEMIDLIST pItemIDList)
    {
        if (!m_bInitialized)
            return;

        TCHAR szBuffer[_MAX_PATH + 1];
        ZeroMemory(szBuffer, (_MAX_PATH + 1) * sizeof(TCHAR));
        ::SHGetPathFromIDList(pItemIDList, szBuffer);

        m_sSelectedFolder = szBuffer;
    }

    void OnCheck(bool bChecked)
    {
        m_bIncludeSubfolders = bChecked;
    }

protected:
    /** Creates "Include Subfolders" checkbox.
     *  You can change checkbox caption through call to SetCheckBoxCaption
     */
    void CreateCheckBox()
    {
        HWND hwndStatic = ::GetDlgItem(m_hWnd, IDC_DIALOG_UPPER_STATIC);

        RECT rcStatic;
        ::GetClientRect(hwndStatic, &rcStatic);

        RECT rClient;
        ::GetClientRect(m_hWnd, &rClient); 

        ::ShowWindow(hwndStatic, SW_HIDE);		
		
        m_hwndCheckBox = CreateWindow((LPCTSTR)(CString)"BUTTON", m_sCheckBoxCaption,
            BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            rClient.left + 11, rClient.top + 5, 200,
            rcStatic.bottom - rcStatic.top, m_hWnd,
            (HMENU)IDC_INCLUDE_SUBFOLDERS, GetModuleHandle(0), 0);		

        // Set font of the checkbox to be the same as dialog font 
        if (m_hwndCheckBox) 
        { 
            HFONT hFontCurr = (HFONT)SendMessage(m_hWnd, WM_GETFONT, 0, 0); 
            if (hFontCurr)
                ::SendMessage(m_hwndCheckBox, WM_SETFONT, (WPARAM)hFontCurr, 0);

            ::UpdateWindow(m_hwndCheckBox);
        }
    }

private:
    struct CallInfo
    {
        CFolderDialogEx* pCallee;                   //!< pointer to a CFolderDialogEx to call
        WNDPROC OriginalWndProc;                    //!< original window procedure
    };

    CallInfo m_ci;                                  //!< call information
    bool m_bInitialized;                            //!< true after call to OnInitialized()
    HWND m_hwndCheckBox;                            //!< "Include Subfolders" checkbox window handle
    CString m_sCheckBoxCaption;                     //!< "Include Subfolders" checkbox caption

    CString m_sSelectedFolder;                      //!< name of the folder currently selected

    bool m_bShowIncludeSubfolders;                  //!< whether to show "Include subfolders" checkbox
    bool m_bIncludeSubfolders;                      //!< whether "Include subfolders" checked
};