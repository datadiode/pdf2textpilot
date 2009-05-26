#include "stdafx.h"
#include "PasswordDlg.h"
#include "Common.h"
#include "resource.h"

DWORD CMyThread:: Run()
{
	bFinish = false;
	
	//converting
	prDlg.StepProgress(1);

	PDF->UseLogging = VARIANT_TRUE;

	CPasswordDlg dlgPasw;
	dlgPasw.IsIgnoreAll = false;

	UINT nTotal = arrInputFileName.GetCount();
	

	if(fileSettings.AllToOne)
	{
		CString strSrc = arrInputFileName[0];
		CString strCurFile = strSrc;
		strCurFile = strCurFile.Right(strCurFile.GetLength() - strCurFile.ReverseFind(_T('\\')) - 1);

		prDlg.strProcessingFile = strCurFile;
		prDlg.strProcessingFile += _T("...");
		prDlg.DoDataExchange(FALSE);
		prDlg.ProcessMessages();			

		_bstr_t bszFileName = strSrc;
			
		HRESULT hErr = PDF->Open(bszFileName, "");
		_bstr_t bszLastErr = PDF->GetLastError();

		CString strLastErr = bszLastErr.copy();
		int ind = strLastErr.ReverseFind(_T('-'));			
		strLastErr = strLastErr.Right(strLastErr.GetLength() - ind - 2);//Password is incorrect

		prDlg.StepProgress(1);

		int res = IDOK;
		if(hErr == S_FALSE && !dlgPasw.IsIgnoreAll && strLastErr == L"Password is incorrect")
		{				
			while(res == IDOK && hErr == S_FALSE)
			{
				dlgPasw.strCurFile = strCurFile;
				res = dlgPasw.DoModal();
				if(res == IDOK)
				{
					_bstr_t bszPassword = dlgPasw.strPassword;
					hErr = PDF->Open(bszFileName, bszPassword);
				}
				dlgPasw.strPassword = L"";
			}				
		}

		if(hErr == S_FALSE && strLastErr != L"Password is incorrect")
				ShowFormatError(IDS_CANTOPEN, strSrc, IDS_ERROR);

		CString FileName = arrOutputFileName[0];
		arrOutputFile.Add(FileName);

		ofstream out (FileName);

		if(res == IDOK && hErr != S_FALSE)
		{			
			if(fileSettings.Author)			
				out << "Author: " << PDF->GetUnicodeAuthor() << endl;

			if(fileSettings.Subject)
				out << "Subject: " << PDF->GetUnicodeSubject() << endl;

			if(fileSettings.Title)
				out << "Title: " << PDF->GetUnicodeTitle() << endl;

			if(fileSettings.Keywords)
				out << "Keywords: " << PDF->GetUnicodeKeywords() << endl;			
				
			out << PDF->GetDocumentText()<< endl << endl;			

			prDlg.StepProgress(1);
		}

		for(UINT i = 1; i < nTotal; ++i)
		{
			strSrc = arrInputFileName[i];
			strCurFile = strSrc;
			strCurFile = strCurFile.Right(strCurFile.GetLength() - strCurFile.ReverseFind(_T('\\')) - 1);

			prDlg.strProcessingFile = strCurFile;
			prDlg.strProcessingFile += _T("...");
			prDlg.DoDataExchange(FALSE);
			prDlg.ProcessMessages();			

			bszFileName = strSrc;
				
			hErr = PDF->Open(bszFileName, "");
			bszLastErr = PDF->GetLastError();

			strLastErr = bszLastErr.copy();
			ind = strLastErr.ReverseFind(_T('-'));			
			strLastErr = strLastErr.Right(strLastErr.GetLength() - ind - 2);//Password is incorrect

			prDlg.StepProgress(1);			

			int res = IDOK;
			if(hErr == S_FALSE && !dlgPasw.IsIgnoreAll && strLastErr == L"Password is incorrect")
			{				
				while(res == IDOK && hErr == S_FALSE)
				{
					dlgPasw.strCurFile = strCurFile;
					res = dlgPasw.DoModal();
					if(res == IDOK)
					{
						_bstr_t bszPassword = dlgPasw.strPassword;
						hErr = PDF->Open(bszFileName, bszPassword);
					}
					dlgPasw.strPassword = L"";
				}				
			}

			if(hErr == S_FALSE && strLastErr != L"Password is incorrect")
				ShowFormatError(IDS_CANTOPEN, strSrc, IDS_ERROR);

			if(res == IDOK && hErr != S_FALSE)
			{				
				if(fileSettings.Author)			
					out << "Author: " << PDF->GetUnicodeAuthor() << endl;

				if(fileSettings.Subject)
					out << "Subject: " << PDF->GetUnicodeSubject() << endl;

				if(fileSettings.Title)
					out << "Title: " << PDF->GetUnicodeTitle() << endl;

				if(fileSettings.Keywords)
					out << "Keywords: " << PDF->GetUnicodeKeywords() << endl;			
					
				out << PDF->GetDocumentText()<< endl << endl;				

				prDlg.StepProgress(1);
			}
		}

		out.close();

	}//All2One
	else
	{
		for(UINT i = 0; i < nTotal; ++i)
		{
			CString strSrc = arrInputFileName[i];
			CString strCurFile = strSrc;
			strCurFile = strCurFile.Right(strCurFile.GetLength() - strCurFile.ReverseFind(_T('\\')) - 1);

			prDlg.strProcessingFile = strCurFile;
			prDlg.strProcessingFile += _T("...");
			prDlg.DoDataExchange(FALSE);
			prDlg.ProcessMessages();			

			_bstr_t bszFileName = strSrc;
			
			HRESULT hErr = PDF->Open(bszFileName, "");
			_bstr_t bszLastErr = PDF->GetLastError();

			CString strLastErr = bszLastErr.copy();
			int ind = strLastErr.ReverseFind(_T('-'));			
			strLastErr = strLastErr.Right(strLastErr.GetLength() - ind - 2);//Password is incorrect

			prDlg.StepProgress(1);

			int res = IDOK;
			if(hErr == S_FALSE && !dlgPasw.IsIgnoreAll && strLastErr == L"Password is incorrect")
			{				
				while(res == IDOK && hErr == S_FALSE)
				{
					dlgPasw.strCurFile = strCurFile;
					res = dlgPasw.DoModal();
					if(res == IDOK)
					{
						_bstr_t bszPassword = dlgPasw.strPassword;
						hErr = PDF->Open(bszFileName, bszPassword);
					}
					dlgPasw.strPassword = L"";
				}				
			}
			
			if(hErr == S_FALSE && strLastErr != L"Password is incorrect")
				ShowFormatError(IDS_CANTOPEN, strSrc, IDS_ERROR);
			

			if(res == IDOK && hErr != S_FALSE)
			{
				CString FileName = arrOutputFileName[i];

				ofstream out (FileName);

				if(fileSettings.Author)			
					out << "Author: " << PDF->GetUnicodeAuthor() << endl;

				if(fileSettings.Subject)
					out << "Subject: " << PDF->GetUnicodeSubject() << endl;

				if(fileSettings.Title)
					out << "Title: " << PDF->GetUnicodeTitle() << endl;

				if(fileSettings.Keywords)
					out << "Keywords: " << PDF->GetUnicodeKeywords() << endl;			
				
				out << PDF->GetDocumentText();
				out.close ();

				arrOutputFile.Add(FileName);

				prDlg.StepProgress(1);
			}
		}
	}

	if(prDlg != 0)
	{
		prDlg.ShowWindow(SW_HIDE);
		prDlg.DestroyWindow();		
	}
	
	//Send notification and exit
    if( ::IsWindow(m_hWnd) ){
      ::PostMessage(m_hWnd, WM_APP + 100, 0, 0L);
    }

	PDF->Release();
	PDF = NULL;
	::CoUninitialize();

	bFinish = true;
    return 0;
}