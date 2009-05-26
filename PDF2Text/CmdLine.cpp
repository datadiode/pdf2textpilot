#include "stdafx.h"
#include "CmdLine.h"
#include "resource.h"
#include "MainDlg.h"


CString GetNextParam(CAtlArray<CString>* arrParams, UINT iIndex)
{
    CString val;
    iIndex++;
    if(iIndex < arrParams->GetCount())
    {
        val = (*arrParams)[iIndex];
        if(val[0] == _T('\"') && val[val.GetLength() - 1] == _T('\"'))
        val = val.Mid(1, val.GetLength() - 2);
    }
    else
        throw(1);
    
    //val.MakeLower();
    return val;
}

CString MakeAbsolutePath(LPCTSTR strFilePath, LPCTSTR strBasePath)
{
	CString path = strFilePath;
	CString res = strFilePath;

	if(path.Find(_T(":\\"), 0) > 0) // strFilePath == "X:\some_loc_path_next"
		return res;

	if(path.Left(2) == _T("\\\\")) // strFilePath == "\\some_net_path_next"
		return res;

	if(path.Left(2) == _T(".\\")) // trim leading ".\" in ".\path\to\file"
		path = path.Right(path.GetLength() - 2);

	CString strAppModulePath;
	if (!strBasePath)
	{
		TCHAR lpszAppModuleName[MAX_PATH];
		::GetModuleFileNameW(NULL, lpszAppModuleName, MAX_PATH);
		strAppModulePath = lpszAppModuleName;
	}
	else
	{
		strAppModulePath = strBasePath;
	}
	int pos = strAppModulePath.ReverseFind(_T('\\'));
	strAppModulePath = strAppModulePath.Left(pos); // path with no last slash "d:\q\w\e"

	int iLevelsUp = 0;
	while(path.Left(3) == _T("..\\"))
	{
		path = path.Right(path.GetLength() - 3);
		iLevelsUp++;
	}

	while(iLevelsUp > 0)
	{
		int pos = strAppModulePath.ReverseFind(_T('\\'));
		if(pos >= 0)
			strAppModulePath = strAppModulePath.Left(pos);
		else
			return CString("");
		iLevelsUp--;
	}

	res.Format(_T("%s\\%s"), strAppModulePath, path);

	return res;
}

bool String2Params(LPSTR lpstrCmdLine, CAtlArray<CString>* arrParams)
{
    if(!lpstrCmdLine)
        return false;

    TCHAR *p = (TCHAR*)lpstrCmdLine;
    TCHAR *start = p;
    TCHAR *end = NULL;
    TCHAR wait_char;
    
    TCHAR space_char = _T(' ');
    TCHAR quote_char = _T('\"');

    bool iswaiting = false;

    while(true)
    {
        if(!iswaiting)
        {
            if(*p == quote_char)
            {
                wait_char = quote_char;
                p = ::CharNext(p);
            }
            else
                wait_char = space_char;
            iswaiting = true;
        }

        if(*p == wait_char || (DWORD)2*wcslen(p) == 0)
        {
            end = p;

            int n = end - start + 1;
            
            if(wait_char == quote_char) // include "
                n++;

			TCHAR *buf = 0 ;
			buf = new TCHAR [n];
            memset(buf, 0, n);
            
			for(int i = 0; i < n-1; ++i)
				buf[i] = start[i];
			buf[n-1] = '\0';

            if((DWORD)2*wcslen(buf) > 0)
            {
                CString tmp(buf);                
                arrParams->Add(tmp);
            }

            delete buf;
            iswaiting = false;
            start = ::CharNext(end);

            if((DWORD)2*wcslen(p) == 0)
                break;
        }
        p = ::CharNext(p);
    }
    return true;
}

int Params2Settings(CAtlArray<CString>* arrParams, UINT iStartIndex, CDocumentSettings& FileSettings)
{
	CString var, val;
    UINT n = arrParams->GetCount();

    for(UINT i = iStartIndex; i < n; ++i)
    {
		if(i >= n)
            return CMDERR_CMDFORMATERROR;

        var = (*arrParams)[i];

        var.MakeLower();

		try
        {
			if(var == "/all2one")            
				FileSettings.AllToOne = true;
                        
			else if(var == "/password")
			{
				val = GetNextParam(arrParams, i);
				FileSettings.Password = val;
				i++;
			}
			else if(var == "/author")
				FileSettings.Author = true;

			else if(var == "/subject")
				FileSettings.Subject = true;

			else if(var == "/title")
				FileSettings.Title = true;

			else if(var == "/keywords")
				FileSettings.Keywords = true;

			else // unknown               
                return CMDERR_CMDFORMATERROR;			

		}
		catch(int iErrCode)
        {
            switch(iErrCode)
            {
                case 1:                    
                    return CMDERR_CMDFORMATERROR;
                    break;
                case 2:
                    //no error, just skip this option
                    i++;
                    break;
            }
        }
	}
	return 0;
}

bool File2Array(CAtlArray<CString>* arrFiles, CString strInFileList)
{
	fstream in(strInFileList);

	if(!in)
		return false;
	
	char ch;
	CString strTmp;

	while(!in.eof())
	{
		in.get(ch);
		if(!in)
		{
			if(strTmp != "")
				arrFiles->Add(strTmp);
			break;
		}
		if(ch == '\n')
		{
			if(strTmp != "")
				arrFiles->Add(strTmp);
			strTmp = "";
		}
		else
			strTmp = strTmp + ch;
	}	
	in.close();

	UINT n = arrFiles->GetCount();
	if(!n)
		return false;

    return true;
}

int ConvertFile(CString in, CString out, CDocumentSettings FileSettings)
{
	CString strInFileName = MakeAbsolutePath(in);
	CString strOutFileName = MakeAbsolutePath(out);

	if(!IsFileExist((LPCTSTR)strInFileName))
		return CMDERR_SRCFILENOEXT;

	CString strTmp;

	int ind = strOutFileName.ReverseFind(_T('.'));
    if(ind > 0)    
        strTmp = strOutFileName.Right(strOutFileName.GetLength() - ind - 1);
    else
        return CMDERR_DSTFILENOEXT;
    
    strTmp.MakeLower();

	if(strTmp != "txt")
		return CMDERR_FORMATNOTSUPPORTED;


	ind = strOutFileName.ReverseFind(_T('\\'));
    if(ind > 0)    
        strTmp = strOutFileName.Left(ind + 1);
    else
        return CMDERR_DSTFILENOEXT;
    
    strTmp.MakeLower();

	if(!IsDirectoryExist((LPCTSTR)strTmp))
		return CMDERR_DSTDIRNOEXT;

	CLSID clsid_PDFCreatorPilot;
    CLSIDFromProgID(OLESTR("PDFCreatorPilot.PDFDocument4"), &clsid_PDFCreatorPilot);

	IPDFDocument4* PDF = 0;
	::CoCreateInstance(clsid_PDFCreatorPilot, NULL, CLSCTX_ALL, __uuidof(IPDFDocument4), (LPVOID*)&PDF);

	_bstr_t bszFileName = strInFileName;
	_bstr_t bszPassword = FileSettings.Password;

	HRESULT hErr = PDF->Open(bszFileName, bszPassword);

	if(hErr == S_FALSE)
		return CMDERR_CANTOPENSRCFILE;
	else
	{
		ofstream out (strOutFileName);

		if(FileSettings.Author)
			out << "Author: " << PDF->GetUnicodeAuthor() << endl;

		if(FileSettings.Subject)
			out << "Subject: " << PDF->GetUnicodeAuthor() << endl;

		if(FileSettings.Title)
			out << "Title: " << PDF->GetUnicodeAuthor() << endl;

		if(FileSettings.Keywords)
			out << "Keywords: " << PDF->GetUnicodeAuthor() << endl;

		out << PDF->GetDocumentText();
		out.close();
	}

	if(!PDF)
		delete PDF;

	return 0;
}

int ConvertFileList2Dir(CString in, CString out, CDocumentSettings FileSettings)
{
	CString strInFileList = MakeAbsolutePath(in);
	CString strOutFileDir = MakeAbsolutePath(out);

	if(!IsFileExist((LPCTSTR)strInFileList))
		return CMDERR_SRCFILELISTNOEXT;

	if(!IsDirectoryExist((LPCTSTR)strOutFileDir))
		return CMDERR_DSTFILEDIRNOEXT;

	CAtlArray<CString> arrFiles;
	if(!File2Array(&arrFiles, strInFileList))
		return CMDERR_SRCFILELISTERR;

	CString var;
	UINT n = arrFiles.GetCount();
	for(UINT i = 0; i < n; ++i)
	{
		var = arrFiles[i];
        var.MakeLower();
		if(!IsFileExist((LPCTSTR)var))
			return CMDERR_SRCFILELISTERR;
	}

	for(UINT i = 0; i < n; ++i)
	{
		CString strOutFileName, strInFileName;
		strInFileName = arrFiles[i];
		int ind = strInFileName.ReverseFind(_T('\\'));
		strOutFileName = strOutFileDir + strInFileName.Right(strInFileName.GetLength() - ind - 1) + ".txt";

		CLSID clsid_PDFCreatorPilot;
		CLSIDFromProgID(OLESTR("PDFCreatorPilot.PDFDocument4"), &clsid_PDFCreatorPilot);

		IPDFDocument4* PDF = 0;
		::CoCreateInstance(clsid_PDFCreatorPilot, NULL, CLSCTX_ALL, __uuidof(IPDFDocument4), (LPVOID*)&PDF);

		_bstr_t bszFileName = strInFileName;
		_bstr_t bszPassword = FileSettings.Password;

		if(FileSettings.AllToOne)
		{
			HRESULT hErr = PDF->Open(bszFileName, bszPassword);

			if(hErr == S_FALSE)
				return CMDERR_CANTOPENSRCLISTFILE;

			ofstream out (strOutFileName);

			if(FileSettings.Author)
				out << "Author: " << PDF->GetUnicodeAuthor() << endl;

			if(FileSettings.Subject)
				out << "Subject: " << PDF->GetUnicodeAuthor() << endl;

			if(FileSettings.Title)
				out << "Title: " << PDF->GetUnicodeAuthor() << endl;

			if(FileSettings.Keywords)
				out << "Keywords: " << PDF->GetUnicodeAuthor() << endl;

			out << PDF->GetDocumentText() << endl << endl;

			for(UINT k = 1; k < n; ++k)
			{
				bszFileName = arrFiles[k];

				hErr = PDF->Open(bszFileName, bszPassword);

				if(hErr == S_FALSE)
				{
					out.close();
					return CMDERR_CANTOPENSRCLISTFILE;
				}

				if(FileSettings.Author)
				out << "Author: " << PDF->GetUnicodeAuthor() << endl;

				if(FileSettings.Subject)
					out << "Subject: " << PDF->GetUnicodeAuthor() << endl;

				if(FileSettings.Title)
					out << "Title: " << PDF->GetUnicodeAuthor() << endl;

				if(FileSettings.Keywords)
					out << "Keywords: " << PDF->GetUnicodeAuthor() << endl;

				out << PDF->GetDocumentText() << endl << endl;

			}
			out.close();
			return 0;
		}//AllToOne
		else
		{
			HRESULT hErr = PDF->Open(bszFileName, bszPassword);

			if(hErr == S_FALSE)
				return CMDERR_CANTOPENSRCLISTFILE;

			ofstream out (strOutFileName);

			if(FileSettings.Author)
				out << "Author: " << PDF->GetUnicodeAuthor() << endl;

			if(FileSettings.Subject)
				out << "Subject: " << PDF->GetUnicodeAuthor() << endl;

			if(FileSettings.Title)
				out << "Title: " << PDF->GetUnicodeAuthor() << endl;

			if(FileSettings.Keywords)
				out << "Keywords: " << PDF->GetUnicodeAuthor() << endl;

			out << PDF->GetDocumentText();
			out.close();
		}		
	}
	return 0;
}

int ConvertFileList2FileList(CString in, CString out, CDocumentSettings FileSettings)
{
	CString strInFileList = MakeAbsolutePath(in);
	CString strOutFileList = MakeAbsolutePath(out);

	if(!IsFileExist((LPCTSTR)strInFileList))
		return CMDERR_SRCFILELISTNOEXT;

	if(!IsFileExist((LPCTSTR)strOutFileList))
		return CMDERR_DSTFILELISTNOEXT;

	CAtlArray<CString> arrFilesIn, arrFilesOut;

	if(!File2Array(&arrFilesIn, strInFileList))
		return CMDERR_SRCFILELISTERR;

	if(!File2Array(&arrFilesOut, strOutFileList))
		return CMDERR_DSTFILELISTERR;

	if(arrFilesIn.GetCount() != arrFilesOut.GetCount())		
		return CMDERR_INOUTLISTSMISMATCH;

	UINT n = arrFilesIn.GetCount();
	int nRet;

	for(UINT i = 0; i < n; ++i)
	{
		nRet = ConvertFile(arrFilesIn[i], arrFilesOut[i], FileSettings);
		if(nRet != 0)
			return nRet;
	}
	return 0;
}