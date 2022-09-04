#pragma once

#ifndef STRICT
#define STRICT
#endif

#ifndef WINVER
#define WINVER 0x0600
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0600
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

#define VC_EXTRALEAN
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _AFX_ALL_WARNINGS

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#include <afxwin.h>
#include <afxext.h>
#include <memory>
#include <array>
#include <vector>
#include <string_view>


//Pull in support for Scintilla \ Lexilla
#pragma warning(push)
#pragma warning(disable: 26812)
#pragma warning(pop)
#include <scintilla.h>
#include <ILexer.h>
#include <Lexilla.h>
#include <SciLexer.h>


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
