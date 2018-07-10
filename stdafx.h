// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// Change these values to use different versions
//#define WINVER		0x0500
//#define _WIN32_WINNT	0x0501
//#define _WIN32_IE	0x0501
//#define _RICHEDIT_VER	0x0200

#include <atlbase.h>
#include <atlapp.h>
#include <atlcrack.h>
#include <atlsplit.h>
#include <atlctrls.h>
#include <atlmisc.h>
#include <atlframe.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atlwin.h>
#include "htmlhelp.h"

#include <fstream>
using namespace std;
#include "appsettings.h"
extern CAppModule _Module;
extern CAppSettings g_Settings;

#include <io.h>
#include <Scintilla.h>
#include <Scilexer.h>
#include "resource.h"
#include "keywords.h"
#include "ScintillaCtrl.h"
#include "OpenDocumentList.h"
#include "LispIDEView.h"
#include "LispIDEPipe.h"
#include "LispTabView.h"
#include "aboutdlg.h"
#include "MainFrm.h"

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
