#pragma once

#include "ScintillaDocView.h"


class CScintillaDemoCtrl : public CScintillaCtrl
{
};

class CScintillaDemoView : public CScintillaView
{
protected: //create from serialization only
  CScintillaDemoView();

public:
//Constructors / Destructors
  CScintillaDemoView(const CScintillaDemoView&) = delete;
  CScintillaDemoView(CScintillaDemoView&&) = delete;
  ~CScintillaDemoView() = default; //NOLINT(modernize-use-override)

//Methods
  CScintillaDemoView& operator=(const CScintillaDemoView&) = delete;
  CScintillaDemoView& operator=(CScintillaDemoView&&) = delete;
  CScintillaDemoDoc* GetDocument();
  void OnDraw(CDC* pDC) override;
  void OnInitialUpdate() override;

protected:
//Member variables
  Scintilla::ILexer5* m_pCLexer;

//Methods
  void OnCharAdded(_Inout_ SCNotification* pSCNotification) override;
  void OnDwellStart(_Inout_ SCNotification* pSCNotification) override;
  void OnDwellEnd(_Inout_ SCNotification* pSCNotification) override;
  void OnModifyAttemptRO(_Inout_ SCNotification* pSCNotification) override;
  void OnModified(_Inout_ SCNotification* pSCNotification) override;
  std::unique_ptr<CScintillaCtrl> CreateScintillaControl() override;
  void SetAStyle(int style, COLORREF fore, COLORREF back = RGB(0xff, 0xff, 0xff), int size = -1, const char* face = nullptr);
  void DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back);

//Message handlers
  afx_msg void OnOptionsAddmarker();
  afx_msg void OnOptionsDeletemarker();
  afx_msg void OnUpdateOptionsDeletemarker(CCmdUI* pCmdUI);
  afx_msg void OnOptionsFindNextmarker();
  afx_msg void OnOptionsFindPrevmarker();
  afx_msg void OnOptionsFoldMargin();
  afx_msg void OnOptionsSelectionMargin();
  afx_msg void OnUpdateOptionsSelectionMargin(CCmdUI* pCmdUI);
  afx_msg void OnOptionsViewLinenumbers();
  afx_msg void OnUpdateOptionsViewLinenumbers(CCmdUI* pCmdUI);
  afx_msg void OnUpdateOptionsAddmarker(CCmdUI* pCmdUI);
  afx_msg void OnUpdateOptionsFoldMargin(CCmdUI* pCmdUI);
  afx_msg void OnUpdateLine(CCmdUI* pCmdUI);
  afx_msg void OnUpdateStyle(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFold(CCmdUI* pCmdUI);
  afx_msg void OnUpdateInsert(CCmdUI* pCmdUI);
  afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

  DECLARE_MESSAGE_MAP()

  DECLARE_DYNCREATE(CScintillaDemoView)
};
