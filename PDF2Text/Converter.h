#pragma once

#ifndef __CONVERTER_H__
#define __CONVERTER_H__

#include "Thread.h"
#include "ProgressDlg.h"
#include "CmdLine.h"
#import "..\\dlls\\PDFCreatorPilot.dll"


using namespace PDFCreatorPilotLib;

class CMyThread : public CThreadImpl<CMyThread>
{
public:
	CProgressDlg prDlg;
	IPDFDocument4 *PDF;
	HWND m_hWnd;
	//int m_iLoops;
	CAtlArray<CString> arrInputFileName;
	CAtlArray<CString> arrOutputFileName;
	CAtlArray<CString> arrOutputFile;
	CDocumentSettings fileSettings;
	bool bFinish;
	
	DWORD Run(); 
};




#endif