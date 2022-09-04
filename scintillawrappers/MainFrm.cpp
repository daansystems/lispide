#include "stdafx.h"
#include "ScintillaDemo.h"
#include "ScintillaDocView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#pragma warning(suppress: 26433 26440 26477)
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

#pragma warning(suppress: 26433 26440)
BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd) //NOLINT(modernize-avoid-c-arrays)
  ON_WM_CREATE()
  ON_WM_SETTINGCHANGE()
  ON_WM_SYSCOLORCHANGE()
  ON_WM_PALETTECHANGED()
  ON_WM_QUERYNEWPALETTE()
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_LINE, &CMainFrame::OnUpdateLine) 
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_STYLE, &CMainFrame::OnUpdateStyle) 
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_FOLD, &CMainFrame::OnUpdateFold) 
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR, &CMainFrame::OnUpdateInsert)
  ON_WM_DROPFILES()
END_MESSAGE_MAP()

std::array<UINT, 8> g_Indicators
{
  ID_SEPARATOR, //status line indicator
  ID_INDICATOR_FOLD,
  ID_INDICATOR_STYLE,
  ID_INDICATOR_LINE,
  ID_INDICATOR_OVR,
  ID_INDICATOR_CAPS,
  ID_INDICATOR_NUM,
  ID_INDICATOR_SCRL
};

#pragma warning(suppress: 26434)
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  //Let the base class do its thing
  if (__super::OnCreate(lpCreateStruct) == -1)
    return -1;

  //Create the toolbar
  if (!m_wndToolBar.Create(this) || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
  {
    TRACE(_T("Failed to create toolbar\n"));
    return -1;
  }

  //Set the title of the toolbar
  CString sTitle;
  VERIFY(sTitle.LoadString(IDS_TOOLBAR));
  m_wndToolBar.SetWindowText(sTitle);

  //Create the status bar
  #pragma warning(suppress: 26472)
  if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(g_Indicators.data(), static_cast<int>(g_Indicators.size())))
  {
    TRACE(_T("Failed to create status bar\n"));
    return -1;
  }

  //Setup docing an tooltips for the toolbar
  m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndToolBar);

  return 0;
}

#pragma warning(suppress: 26434)
void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
  //Validate our parameters
  const CWinApp* pApp{AfxGetApp()};
#pragma warning(suppress: 26496)
  AFXASSUME(pApp != nullptr);

  //Pass the message on to all scintilla control
  POSITION posTemplate{pApp->GetFirstDocTemplatePosition()};
  if (posTemplate != nullptr)
  {
#pragma warning(suppress: 26429)
    const CDocTemplate* pTemplate{pApp->GetNextDocTemplate(posTemplate)};
    ASSERT(pTemplate != nullptr);
    POSITION posDoc{pTemplate->GetFirstDocPosition()};
    if (posDoc != nullptr)
    {
      const CDocument* pDocument{pTemplate->GetNextDoc(posDoc)};
      if (pDocument != nullptr)
      {
        POSITION posView{pDocument->GetFirstViewPosition()};
        if (posView != nullptr)
        {
#pragma warning(suppress: 26429 26466)
          auto pView{static_cast<CScintillaView*>(pDocument->GetNextView(posView))};
          ASSERT(pView != nullptr);
          if (pView->IsKindOf(RUNTIME_CLASS(CScintillaView)))
          {
            const MSG* pMsg{GetCurrentMessage()};
            pView->GetCtrl().SendMessage(WM_SETTINGCHANGE, pMsg->wParam, pMsg->lParam);
          }
        }
      }
    }
  }

  //Let the base class do its thing
  __super::OnSettingChange(uFlags, lpszSection);
}

#pragma warning(suppress: 26434)
void CMainFrame::OnSysColorChange()
{
  const CWinApp* pApp{AfxGetApp()};
#pragma warning(suppress: 26496)
  AFXASSUME(pApp != nullptr);

  //Pass the message on to all scintilla control
  POSITION posTemplate{pApp->GetFirstDocTemplatePosition()};
  if (posTemplate != nullptr)
  {
#pragma warning(suppress: 26429)
    const CDocTemplate* pTemplate{pApp->GetNextDocTemplate(posTemplate)};
    ASSERT(pTemplate != nullptr);
    POSITION posDoc{pTemplate->GetFirstDocPosition()};
    if (posDoc != nullptr)
    {
      const CDocument* pDocument{pTemplate->GetNextDoc(posDoc)};
      if (pDocument != nullptr)
      {
        POSITION posView{pDocument->GetFirstViewPosition()};
        if (posView != nullptr)
        {
#pragma warning(suppress: 26429 26466)
          auto pView{static_cast<CScintillaView*>(pDocument->GetNextView(posView))};
          ASSERT(pView != nullptr);
          if (pView->IsKindOf(RUNTIME_CLASS(CScintillaView)))
          {
            const MSG* pMsg{GetCurrentMessage()};
            pView->GetCtrl().SendMessage(WM_SYSCOLORCHANGE, pMsg->wParam, pMsg->lParam);
          }
        }
      }
    }
  }

  //Let the base class do its thing
  __super::OnSysColorChange();
}

#pragma warning(suppress: 26434)
void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd) 
{
  const CWinApp* pApp{AfxGetApp()};
#pragma warning(suppress: 26496)
  AFXASSUME(pApp != nullptr);

  //Pass the message on to all scintilla control
  POSITION posTemplate{pApp->GetFirstDocTemplatePosition()};
  if (posTemplate != nullptr)
  {
#pragma warning(suppress: 26429)
    const CDocTemplate* pTemplate{pApp->GetNextDocTemplate(posTemplate)};
    ASSERT(pTemplate != nullptr);
    POSITION posDoc{pTemplate->GetFirstDocPosition()};
    if (posDoc != nullptr)
    {
      const CDocument* pDocument{pTemplate->GetNextDoc(posDoc)};
      if (pDocument != nullptr)
      {
        POSITION posView{pDocument->GetFirstViewPosition()};
        if (posView != nullptr)
        {
#pragma warning(suppress: 26429 26466)
          auto pView{static_cast<CScintillaView*>(pDocument->GetNextView(posView))};
          ASSERT(pView != nullptr);
          if (pView->IsKindOf(RUNTIME_CLASS(CScintillaView)))
          {
            const MSG* pMsg{GetCurrentMessage()};
            pView->GetCtrl().SendMessage(WM_PALETTECHANGED, pMsg->wParam, pMsg->lParam);
          }
        }
      }
    }
  }

  //Let the base class do its thing
  __super::OnPaletteChanged(pFocusWnd);
}

#pragma warning(suppress: 26434)
BOOL CMainFrame::OnQueryNewPalette()
{
  const CWinApp* pApp{AfxGetApp()};
#pragma warning(suppress: 26496)
  AFXASSUME(pApp);

  //Pass the message on to all scintilla control
  POSITION posTemplate{pApp->GetFirstDocTemplatePosition()};
  if (posTemplate != nullptr)
  {
#pragma warning(suppress: 26429)
    const CDocTemplate* pTemplate{pApp->GetNextDocTemplate(posTemplate)};
    ASSERT(pTemplate != nullptr);
    POSITION posDoc{pTemplate->GetFirstDocPosition()};
    if (posDoc != nullptr)
    {
      const CDocument* pDocument{pTemplate->GetNextDoc(posDoc)};
      if (pDocument != nullptr)
      {
        POSITION posView{pDocument->GetFirstViewPosition()};
        if (posView != nullptr)
        {
#pragma warning(suppress: 26429 26466)
          auto pView{static_cast<CScintillaView*>(pDocument->GetNextView(posView))};
          ASSERT(pView != nullptr);
          if (pView->IsKindOf(RUNTIME_CLASS(CScintillaView)))
          {
            const MSG* pMsg{GetCurrentMessage()};
            pView->GetCtrl().SendMessage(WM_QUERYNEWPALETTE, pMsg->wParam, pMsg->lParam);
          }
        }
      }
    }
  }

  //Let the base class do its thing
  return __super::OnQueryNewPalette();
}

#pragma warning(suppress: 26429)
void CMainFrame::OnUpdateInsert(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
  pCmdUI->SetText(_T(""));
}

#pragma warning(suppress: 26429)
void CMainFrame::OnUpdateStyle(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
   pCmdUI->SetText(_T(""));
}

#pragma warning(suppress: 26429)
void CMainFrame::OnUpdateFold(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
  pCmdUI->SetText(_T(""));
}

#pragma warning(suppress: 26429)
void CMainFrame::OnUpdateLine(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
  pCmdUI->SetText(_T(""));
}
