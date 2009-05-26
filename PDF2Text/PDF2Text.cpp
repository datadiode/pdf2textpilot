// PDF2Text.cpp : main source file for PDF2Text.exe

#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"

CAppModule _Module;


int Run(LPTSTR lpstrCmdLine = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	int nRet = 0;	
	CString sCmd = lpstrCmdLine;
	
	if((DWORD)2*wcslen(lpstrCmdLine) > 0)
	{
		CAtlArray<CString> arrParams;
        String2Params((LPSTR)lpstrCmdLine, &arrParams);

		UINT n = (UINT)arrParams.GetCount();

		//help
		if(arrParams[0] == _T("/?") || arrParams[0] == _T("/h") || arrParams[0] == _T("/help"))
        {			
			ShowHelp();			
			return 0;
        }
		CString in, out;
        CString buf;
		CDocumentSettings FileSettings = {L"", 0, 0, 0, 0, 0};
    
        UINT i = 0;
		CString aParam = arrParams[i];
		aParam.MakeLower();

		//inlist
		if(aParam == _T("/inlist"))
        {
            i++;
            if(i >= n)                            
                return CMDERR_CMDFORMATERROR;
            

            in = arrParams[i];
            
            i++;
            if(i >= n)                
                return CMDERR_CMDFORMATERROR;
            

			aParam = arrParams[i];
			aParam.MakeLower();

			//outlist
            if(aParam == _T("/outlist"))
            {
                i++;
                if(i >= n)                    
                    return CMDERR_CMDFORMATERROR;
                

                out = arrParams[i];
    
                // list to list conversion
                in.Trim(_T("\" "));
                out.Trim(_T("\" "));

                nRet = Params2Settings(&arrParams, i+1, FileSettings);
                if(nRet != 0)
                    return nRet;
                nRet = ConvertFileList2FileList(in, out, FileSettings);
            }
			//outdir
            else if(aParam == _T("/outdir"))
            {
                i++;
                if(i >= n)
                    return CMDERR_CMDFORMATERROR;

				out = arrParams[i];
    
                // list to dir conversion
                in.TrimLeft(_T('\"'));
                in.TrimRight(_T('\"'));
                out.TrimLeft(_T('\"'));
                out.TrimRight(_T('\"'));

                nRet = Params2Settings(&arrParams, i+1, FileSettings);
                if(nRet != 0)
                    return nRet;
                nRet = ConvertFileList2Dir(in, out, FileSettings);
            }
        }//inlist

		else
        {
            in = arrParams[i];
            in.Trim(_T("\" "));
            
            if(i+1 >= n)
				out = in + L".txt";
			else
			{
	            i++;
				if(arrParams[i].Left(1) != _T("/"))
				{
					out = arrParams[i];
					out.Trim(_T("\" "));
				}
				else
				{
					i--;
					out = in + L".txt";
				}
			}

			// file to file conversion
            nRet = Params2Settings(&arrParams, i+1, FileSettings);
            if(nRet != 0)
                return nRet;

            // we allready want to convert single file
			nRet = ConvertFile(in, out, FileSettings);           
        }
	}
////////////////////////////////////////////////////////////////////////////////////
	else
	{
	
		CMessageLoop theLoop;
		_Module.AddMessageLoop(&theLoop);

		CMainDlg dlgMain;
		if(dlgMain.Create(NULL) == NULL)
		{
			ATLTRACE(_T("Main dialog creation failed!\n"));
			return 0;
		}

		dlgMain.ShowWindow(nCmdShow);    

		int nRet = theLoop.Run();
		_Module.RemoveMessageLoop();
	}

	return nRet;    
}



int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
//	If you are running on NT 4.0 or higher you can use the following call instead to 
//	make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	//SetThreadLocale(LANG_ID);
	hRes = _Module.Init(NULL, hInstance);

	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
