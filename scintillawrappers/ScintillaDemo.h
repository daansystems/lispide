#pragma once


#include "resource.h"


class CScintillaDemoApp : public CWinApp
{
public:
//Constructors / Destructors
  CScintillaDemoApp();

//Member variables
  Lexilla::CreateLexerFn m_pCreateLexer;

protected:
//Methods
  BOOL InitInstance() override;
  int ExitInstance() override;
  HMODULE LoadLibraryFromApplicationDirectory(LPCTSTR lpFileName);

//Message handlers
  afx_msg void OnAppAbout();

  DECLARE_MESSAGE_MAP()

//Member variables
  HINSTANCE m_hScintilla;
  HINSTANCE m_hLexilla;
};
