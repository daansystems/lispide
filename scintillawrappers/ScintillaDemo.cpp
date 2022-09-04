#include "stdafx.h"
#include "ScintillaDemo.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "ScintillaDemoDoc.h"
#include "ScintillaDemoView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(suppress: 26440 26433)
BEGIN_MESSAGE_MAP(CScintillaDemoApp, CWinApp) //NOLINT(clang-diagnostic-unused-local-typedef, modernize-avoid-c-arrays)
  ON_COMMAND(ID_APP_ABOUT, &CScintillaDemoApp::OnAppAbout)
  ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
  ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
  ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

#pragma warning(suppress: 26426)
CScintillaDemoApp theApp;


#pragma warning(suppress: 26455)
CScintillaDemoApp::CScintillaDemoApp() : m_pCreateLexer{nullptr},
                                         m_hScintilla{nullptr},
                                         m_hLexilla{nullptr}
{
}

HMODULE CScintillaDemoApp::LoadLibraryFromApplicationDirectory(LPCTSTR lpFileName)
{
  //Get the Application diretory
  CString sFullPath;
  const DWORD dwGMFN{GetModuleFileName(nullptr, sFullPath.GetBuffer(_MAX_PATH), _MAX_PATH)};
  sFullPath.ReleaseBuffer();
  if (dwGMFN == 0)
#pragma warning(suppress: 26487)
    return nullptr;

  //Form the new path
  CString sDrive;
  CString sDir;
  _tsplitpath_s(sFullPath, sDrive.GetBuffer(_MAX_DRIVE), _MAX_DRIVE, sDir.GetBuffer(_MAX_DIR), _MAX_DIR, nullptr, 0, nullptr, 0);
  sDir.ReleaseBuffer();
  sDrive.ReleaseBuffer();
  CString sFname;
  CString sExt;
  _tsplitpath_s(lpFileName, nullptr, 0, nullptr, 0, sFname.GetBuffer(_MAX_FNAME), _MAX_FNAME, sExt.GetBuffer(_MAX_EXT), _MAX_EXT);
  sExt.ReleaseBuffer();
  sFname.ReleaseBuffer();
  _tmakepath_s(sFullPath.GetBuffer(_MAX_PATH), _MAX_PATH, sDrive, sDir, sFname, sExt);
  sFullPath.ReleaseBuffer();

  //Delegate to LoadLibrary
#pragma warning(suppress: 26487)
  return LoadLibrary(sFullPath);
}

BOOL CScintillaDemoApp::InitInstance()
{
  //Load the Scintilla \ Lexilla dlls
  CString sLexillaDLL;
  sLexillaDLL.Format(_T("%s%s"), _T(LEXILLA_LIB), _T(LEXILLA_EXTENSION));
  m_hLexilla = LoadLibraryFromApplicationDirectory(sLexillaDLL);
  if (m_hLexilla == nullptr)
  {
    CString sMsg;
    sMsg.Format(_T("%s is not installed, Please built the Scintilla '%s' and copy it into this application's directory"), sLexillaDLL.GetString(), sLexillaDLL.GetString());
    AfxMessageBox(sMsg);
    return FALSE;
  }
  m_hScintilla = LoadLibraryFromApplicationDirectory(_T("Scintilla.dll"));
  if (m_hScintilla == nullptr)
  {
    AfxMessageBox(_T("Scintilla DLL is not installed, Please built the Scintilla 'Scintilla.dll' and copy it into this application's directory"));
    return FALSE;
  }

  //Create the C++ Lexer from Lexilla
#pragma warning(suppress: 26490)
  m_pCreateLexer = reinterpret_cast<Lexilla::CreateLexerFn>(GetProcAddress(m_hLexilla, LEXILLA_CREATELEXER));
  if (m_pCreateLexer == nullptr)
  {
    AfxMessageBox(_T("Could not find the Lexilla CreateLexer function"));
    return FALSE;
  }

  SetRegistryKey(_T("PJ Naughter"));
  LoadStdProfileSettings(); //Load standard INI file options (including MRU)

  //Register the application's document templates.  Document templates
  //serve as the connection between documents, frame windows and views.
#pragma warning(suppress: 26400 26403 26409)
  auto pDocTemplate{new CMultiDocTemplate(IDR_SCINTITYPE,
                                          RUNTIME_CLASS(CScintillaDemoDoc),
                                          RUNTIME_CLASS(CChildFrame), // custom MDI child frame
                                          RUNTIME_CLASS(CScintillaDemoView))};
  AddDocTemplate(pDocTemplate);

  //create main MDI Frame window
#pragma warning(suppress: 26400 26403 26409)
  auto pMainFrame{new CMainFrame};
  if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
    return FALSE;
  m_pMainWnd = pMainFrame;

  //Parse command line for standard shell commands, DDE, file open
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);

  //Dispatch commands specified on the command line
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;

  //The main window has been initialized, so show and update it.
  pMainFrame->ShowWindow(m_nCmdShow);
  pMainFrame->UpdateWindow();

  //Enable drag/drop open
  m_pMainWnd->DragAcceptFiles();

  return TRUE;
}

int CScintillaDemoApp::ExitInstance()
{
  //Unload the Scintilla \ Lexilla dlls
  if (m_hScintilla != nullptr)
  {
    FreeLibrary(m_hScintilla);
    m_hScintilla = nullptr;
  }
  if (m_hLexilla != nullptr)
  {
    FreeLibrary(m_hLexilla);
    m_hLexilla = nullptr;
  }

  //Let the base class do its thing
  return __super::ExitInstance();
}


class CAboutDlg : public CDialog
{
public:
  CAboutDlg();

  enum { IDD = IDD_ABOUTBOX };

protected:
  void DoDataExchange(CDataExchange* pDX) override;

  DECLARE_MESSAGE_MAP()
};

#pragma warning(suppress: 26455)
CAboutDlg::CAboutDlg() : CDialog{CAboutDlg::IDD}
{
}

#pragma warning(suppress: 26433)
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  __super::DoDataExchange(pDX);
}

#pragma warning(suppress: 26440 26433)
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog) //NOLINT(clang-diagnostic-unused-local-typedef, modernize-avoid-c-arrays)
END_MESSAGE_MAP()

void CScintillaDemoApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}
