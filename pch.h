// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
using namespace ATL;
#include "appsettings.h"
extern CAppModule _Module;
extern CAppSettings g_Settings;

#include <io.h>
#include <ILexer.h>
#include <Scintilla.h>
#include "resource.h"
#include "keywords.h"
#include "ScintillaLispCtrl.h"
#include "OpenDocumentList.h"
#include "LispIDEView.h"
#include "LispIDEPipe.h"
#include "LispTabView.h"
#include "aboutdlg.h"
#include "MainFrm.h"
#include <fstream>


#endif //PCH_H
