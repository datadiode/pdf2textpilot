// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#pragma once

// Change these values to use different versions

/*
#define WINVER      0x0400
//#define _WIN32_WINNT  0x0400
#define _WIN32_IE   0x0400
#define _RICHEDIT_VER   0x0100
*/
#define WINVER          0x0501
#define _WIN32_WINNT    0x0501
#define _WIN32_IE       0x0501
#define _RICHEDIT_VER   0x0100


#define CMDERR_CMDFORMATERROR           -1
#define CMDERR_SRCFILENOEXT             -2
#define CMDERR_DSTFILENOEXT             -3
#define CMDERR_FORMATNOTSUPPORTED       -4
#define CMDERR_DSTDIRNOEXT              -5
#define CMDERR_CANTOPENSRCFILE          -6
#define CMDERR_SRCFILELISTNOEXT         -7
#define CMDERR_DSTFILEDIRNOEXT          -8
#define CMDERR_SRCFILELISTERR           -9
#define CMDERR_CANTOPENSRCLISTFILE     -10
#define CMDERR_DSTFILELISTNOEXT        -11
#define CMDERR_DSTFILELISTERR          -12
#define CMDERR_INOUTLISTSMISMATCH      -13


// turn off deprecate warnings
#define _CRT_SECURE_NO_DEPRECATE

#define _WTL_NEW_PAGE_NOTIFY_HANDLERS

#include <atlbase.h>

#define _WTL_NO_CSTRING
using namespace ATL;
#include <atlstr.h>

#include <atlapp.h>
#include <atlmisc.h>

extern CAppModule _Module;
extern HINSTANCE _hinstRes;

#include <atlwin.h>
#include <fstream>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlcrack.h>
#include <atlctrlx.h>
#include <atlddx.h>
#include <atlcoll.h>

using namespace std;

#define MAX_FILES 4194304

////////////////////////////
#include "atlfiledialogex.h"
#include "FolderDialogEx.h"
#include "Common.h"
#include "CmdLine.h"
#include "Converter.h"


#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
