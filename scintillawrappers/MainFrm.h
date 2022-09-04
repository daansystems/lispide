#pragma once

class CMainFrame : public CMDIFrameWnd
{
protected:
//Member variables
  CStatusBar m_wndStatusBar;
  CToolBar m_wndToolBar;

//Member variables
  CString GetLongPathName(LPCTSTR pszPathName);

//Message handlers
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
  afx_msg void OnSysColorChange();
  afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
  afx_msg BOOL OnQueryNewPalette();
  afx_msg void OnUpdateLine(CCmdUI* pCmdUI);
  afx_msg void OnUpdateStyle(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFold(CCmdUI* pCmdUI);
  afx_msg void OnUpdateInsert(CCmdUI* pCmdUI);

  DECLARE_MESSAGE_MAP()
  DECLARE_DYNAMIC(CMainFrame)
};
