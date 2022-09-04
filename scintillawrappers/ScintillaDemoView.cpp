#include "stdafx.h"
#include "ScintillaDemo.h"
#include "ScintillaDemoDoc.h"
#include "ScintillaDemoView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const TCHAR* g_cppKeyWords
{_T("alignas alignof and and_eq asm atomic_cancel atomic_commit atomic_noexcept auto bitand bitor bool break ")
 _T("case catch char char8_t char16_t char32_t class compl concept const consteval constexpr constinit const_cast continue ")
 _T("co_await co_return co_yield decltype default delete do double dynamic_cast else enum explicit export extern false float for ")
 _T("friend goto if inline int long mutable namespace new noexcept not not_eq nullptr ")
 _T("operator or or_eq private protected public ")
 _T("reflexpr register reinterpret_cast requires return short signed sizeof static static_assert static_cast struct switch synchronized ")
 _T("template this thread_local throw true try typedef typeid typename union unsigned using ")
 _T("virtual void volatile wchar_t while xor xor_eq")};


#pragma warning(suppress: 26409 26433 26440 26477)
IMPLEMENT_DYNCREATE(CScintillaDemoView, CScintillaView)

#pragma warning(suppress: 26433 26440)
BEGIN_MESSAGE_MAP(CScintillaDemoView, CScintillaView) //NOLINT(modernize-avoid-c-arrays)
  ON_COMMAND(ID_OPTIONS_ADDMARKER, &CScintillaDemoView::OnOptionsAddmarker)
  ON_COMMAND(ID_OPTIONS_DELETEMARKER, &CScintillaDemoView::OnOptionsDeletemarker)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_DELETEMARKER, &CScintillaDemoView::OnUpdateOptionsDeletemarker)
  ON_COMMAND(ID_OPTIONS_FIND_NEXTMARKER, &CScintillaDemoView::OnOptionsFindNextmarker)
  ON_COMMAND(ID_OPTIONS_FIND_PREVMARKER, &CScintillaDemoView::OnOptionsFindPrevmarker)
  ON_COMMAND(ID_OPTIONS_FOLD_MARGIN, &CScintillaDemoView::OnOptionsFoldMargin)
  ON_COMMAND(ID_OPTIONS_SELECTION_MARGIN, &CScintillaDemoView::OnOptionsSelectionMargin)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_SELECTION_MARGIN, &CScintillaDemoView::OnUpdateOptionsSelectionMargin)
  ON_COMMAND(ID_OPTIONS_VIEW_LINENUMBERS, &CScintillaDemoView::OnOptionsViewLinenumbers)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_VIEW_LINENUMBERS, &CScintillaDemoView::OnUpdateOptionsViewLinenumbers)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_ADDMARKER, &CScintillaDemoView::OnUpdateOptionsAddmarker)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_FOLD_MARGIN, &CScintillaDemoView::OnUpdateOptionsFoldMargin)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_LINE, &CScintillaDemoView::OnUpdateLine) 
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_STYLE, &CScintillaDemoView::OnUpdateStyle) 
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_FOLD, &CScintillaDemoView::OnUpdateFold) 
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR, &CScintillaDemoView::OnUpdateInsert)
  ON_WM_ACTIVATE()
  ON_WM_CREATE()
END_MESSAGE_MAP()


#pragma warning(suppress: 26439 26455)
CScintillaDemoView::CScintillaDemoView() : m_pCLexer{nullptr}
{
  LoadMarginSettings();
}

void CScintillaDemoView::OnDraw(CDC* /*pDC*/)
{
  const CScintillaDemoDoc* pDoc{GetDocument()};
  ASSERT_VALID(pDoc);
  UNREFERENCED_PARAMETER(pDoc);
}

#pragma warning(suppress: 26434 26440)
CScintillaDemoDoc* CScintillaDemoView::GetDocument()
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScintillaDemoDoc)));
#pragma warning(suppress: 26466)
  return static_cast<CScintillaDemoDoc*>(m_pDocument);
}

void CScintillaDemoView::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char* face) 
{
  CScintillaCtrl& rCtrl{GetCtrl()};

  rCtrl.StyleSetFore(style, fore);
  rCtrl.StyleSetBack(style, back);
  if (size >= 1)
    rCtrl.StyleSetSize(style, size);
  if (face)
    rCtrl.StyleSetFont(style, face);
}

void CScintillaDemoView::DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back) 
{
  CScintillaCtrl& rCtrl{GetCtrl()};

  rCtrl.MarkerDefine(marker, markerType);
  rCtrl.MarkerSetFore(marker, fore);
  rCtrl.MarkerSetBack(marker, back);
}

void CScintillaDemoView::OnInitialUpdate()
{
  //Let the base class do its thing
  __super::OnInitialUpdate();

  CScintillaCtrl& rCtrl{GetCtrl()};

  //Setup the C++ Lexer
  rCtrl.SetILexer(m_pCLexer);
  rCtrl.SetKeyWords(0, g_cppKeyWords);

  //Setup styles
  SetAStyle(STYLE_DEFAULT, RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 11, "Verdana");
  rCtrl.StyleClearAll();
  SetAStyle(SCE_C_DEFAULT, RGB(0, 0, 0));
  SetAStyle(SCE_C_COMMENT, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTLINE, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTDOC, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTLINEDOC, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTDOCKEYWORD, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTDOCKEYWORDERROR, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_NUMBER, RGB(0, 0x80, 0x80));
  SetAStyle(SCE_C_WORD, RGB(0, 0, 0x80));
  rCtrl.StyleSetBold(SCE_C_WORD, 1);
  SetAStyle(SCE_C_STRING, RGB(0x80, 0, 0x80));
  SetAStyle(SCE_C_IDENTIFIER, RGB(0, 0, 0));
  SetAStyle(SCE_C_PREPROCESSOR, RGB(0x80, 0, 0));
  SetAStyle(SCE_C_OPERATOR, RGB(0x80, 0x80, 0));

  //Setup folding
  rCtrl.SetMarginWidthN(2, 16);
  rCtrl.SetMarginSensitiveN(2, TRUE);
  rCtrl.SetMarginTypeN(2, SC_MARGIN_SYMBOL);
  rCtrl.SetMarginMaskN(2, SC_MASK_FOLDERS);
  rCtrl.SetScintillaProperty(_T("fold"), _T("1"));

  //Setup markers
  DefineMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0xFF));
  DefineMarker(SC_MARKNUM_FOLDER, SC_MARK_PLUS, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));

  //Setup auto completion
  rCtrl.AutoCSetSeparator(10); //Use a separator of line feed

  //Setup call tips
  rCtrl.SetMouseDwellTime(1000);

  //Enable Multiple selection
  rCtrl.SetMultipleSelection(TRUE);

#ifdef _DEBUG
  AfxDump(this);
#endif
}

void CScintillaDemoView::OnOptionsAddmarker()
{
  CScintillaCtrl& rCtrl{GetCtrl()};
  const Sci_Position nPos{rCtrl.GetCurrentPos()};
  const auto nLine{rCtrl.LineFromPosition(nPos)};
  rCtrl.MarkerAdd(nLine, 0);
}

#pragma warning(suppress: 26429)
void CScintillaDemoView::OnUpdateOptionsAddmarker(CCmdUI* pCmdUI)
{
  CScintillaCtrl& rCtrl{GetCtrl()};
  const Sci_Position nPos{rCtrl.GetCurrentPos()};
  const auto nLine{rCtrl.LineFromPosition(nPos)};
  const int nBits{rCtrl.MarkerGet(nLine)};
#pragma warning(suppress: 26486)
  pCmdUI->Enable((nBits & 0x1) == 0);
}

void CScintillaDemoView::OnOptionsDeletemarker()
{
  CScintillaCtrl& rCtrl{GetCtrl()};
  const Sci_Position nPos{rCtrl.GetCurrentPos()};
  const auto nLine{rCtrl.LineFromPosition(nPos)};
  rCtrl.MarkerDelete(nLine, 0);
}

#pragma warning(suppress: 26429)
void CScintillaDemoView::OnUpdateOptionsDeletemarker(CCmdUI* pCmdUI)
{
  CScintillaCtrl& rCtrl{GetCtrl()};
  const Sci_Position nPos{rCtrl.GetCurrentPos()};
  const auto nLine{rCtrl.LineFromPosition(nPos)};
  const int nBits{rCtrl.MarkerGet(nLine)};
#pragma warning(suppress: 26486)
  pCmdUI->Enable(nBits & 0x1);
}

void CScintillaDemoView::OnOptionsFindNextmarker()
{
  CScintillaCtrl& rCtrl{GetCtrl()};
  const Sci_Position nPos{rCtrl.GetCurrentPos()};
  const auto nLine{rCtrl.LineFromPosition(nPos)};
  const auto nFoundLine{rCtrl.MarkerNext(nLine + 1, 0x1)};
  if (nFoundLine >= 0)
    rCtrl.GotoLine(nFoundLine);
  else
    MessageBeep(MB_ICONHAND);
}

void CScintillaDemoView::OnOptionsFindPrevmarker()
{
  CScintillaCtrl& rCtrl{GetCtrl()};
  const Sci_Position nPos{rCtrl.GetCurrentPos()};
  const auto nLine{rCtrl.LineFromPosition(nPos)};
  const auto nFoundLine{rCtrl.MarkerPrevious(nLine - 1, 0x1)};
  if (nFoundLine >= 0)
    rCtrl.GotoLine(nFoundLine);
  else
    MessageBeep(MB_ICONHAND);
}

void CScintillaDemoView::OnOptionsFoldMargin()
{
  CScintillaCtrl& rCtrl{GetCtrl()};
  const int nMarginWidth{rCtrl.GetMarginWidthN(2)};
  if (nMarginWidth)
    rCtrl.SetMarginWidthN(2, 0);
  else
    rCtrl.SetMarginWidthN(2, 16);
}

#pragma warning(suppress: 26429)
void CScintillaDemoView::OnUpdateOptionsFoldMargin(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
  pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(2) != 0);
}

void CScintillaDemoView::OnOptionsSelectionMargin()
{
  CScintillaCtrl& rCtrl{GetCtrl()};
  const int nMarginWidth{rCtrl.GetMarginWidthN(1)};
  if (nMarginWidth)
    rCtrl.SetMarginWidthN(1, 0);
  else
    rCtrl.SetMarginWidthN(1, 16);
}

#pragma warning(suppress: 26429)
void CScintillaDemoView::OnUpdateOptionsSelectionMargin(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
  pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(1) != 0);
}

void CScintillaDemoView::OnOptionsViewLinenumbers()
{
  CScintillaCtrl& rCtrl{GetCtrl()};
  const int nMarginWidth{rCtrl.GetMarginWidthN(0)};
  if (nMarginWidth)
    rCtrl.SetMarginWidthN(0, 0);
  else
    rCtrl.SetMarginWidthN(0, 32);
}

#pragma warning(suppress: 26429)
void CScintillaDemoView::OnUpdateOptionsViewLinenumbers(CCmdUI* pCmdUI)
{
#pragma warning(suppress: 26486)
  pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(0) != 0);
}

#pragma warning(suppress: 26429)
void CScintillaDemoView::OnUpdateInsert(CCmdUI* pCmdUI)
{
  CString sText;
  if (GetCtrl().GetOvertype())
    VERIFY(sText.LoadString(ID_INDICATOR_OVR));
#pragma warning(suppress: 26486)
  pCmdUI->SetText(sText);
}

#pragma warning(suppress: 26429)
void CScintillaDemoView::OnUpdateStyle(CCmdUI* pCmdUI)
{
   CScintillaCtrl& rCtrl{GetCtrl()};
   const Sci_Position nPos{rCtrl.GetCurrentPos()};
   const int nStyle{rCtrl.GetStyleAt(nPos)};
   CString sLine;
   sLine.Format(IDS_STYLE_INDICATOR, nStyle);
#pragma warning(suppress: 26486)
   pCmdUI->SetText(sLine);
}

#pragma warning(suppress: 26429)
void CScintillaDemoView::OnUpdateFold(CCmdUI* pCmdUI)
{
  CScintillaCtrl& rCtrl{GetCtrl()};
  const Sci_Position nPos{rCtrl.GetCurrentPos()};
  const auto nLine{rCtrl.LineFromPosition(nPos)};
  int nLevel{rCtrl.GetFoldLevel(nLine) & SC_FOLDLEVELNUMBERMASK};
  nLevel -= 1024;
  CString sLine;
  sLine.Format(IDS_FOLD_INDICATOR, nLevel);
#pragma warning(suppress: 26486)
  pCmdUI->SetText(sLine);
}

#pragma warning(suppress: 26429)
void CScintillaDemoView::OnUpdateLine(CCmdUI* pCmdUI)
{
  CScintillaCtrl& rCtrl{GetCtrl()};
  const Sci_Position nPos{rCtrl.GetCurrentPos()};
  const auto nLine{rCtrl.LineFromPosition(nPos)};
  const Sci_Position nColumn{rCtrl.GetColumn(nPos)};

  CString sLine;
  sLine.Format(IDS_LINE_INDICATOR, nLine, nColumn, nPos);
#pragma warning(suppress: 26486)
  pCmdUI->SetText(sLine);
}

//Some simple examples of implementing auto completion
void CScintillaDemoView::OnCharAdded(_Inout_ SCNotification* /*pSCNotification*/)
{
  CScintillaCtrl& rCtrl{GetCtrl()};

  //Get the previous 13 characters and if it is "scintilla is " case insensitive
  //then display a list which allows "very cool", "easy" and "way cool!!"
  const Sci_Position nStartSel{rCtrl.GetSelectionStart()};
  const Sci_Position nEndSel{rCtrl.GetSelectionEnd()};
  if ((nStartSel == nEndSel) && (nStartSel >= 13))
  {
    Sci_TextRangeFull tr{};
    tr.chrg.cpMin = nStartSel - 13;
    tr.chrg.cpMax = nEndSel;
    CStringA sText;
    tr.lpstrText = sText.GetBuffer(13);
    rCtrl.GetTextRangeFull(&tr);
    sText.ReleaseBuffer();

    //Display the auto completion list
    if (sText.CompareNoCase("scintilla is ") == 0)
    {
      //Display the auto completion list
      //rCtrl.AutoCSetOrder(SC_ORDER_PERFORMSORT);
      //int nOrder{rCtrl.AutoCGetOrder()};
      rCtrl.AutoCShow(0, _T("very cool\neasy to use\nway cool!!"));
    }
  }

#ifdef _DEBUG
  //Test out some of the methods
  /*
  rCtrl.AutoCSetOptions(SC_AUTOCOMPLETE_FIXED_SIZE);
  const int nOptions{rCtrl.AutoCGetOptions()}; //NOLINT(clang-analyzer-deadcode.DeadStores)
  UNREFERENCED_PARAMETER(nOptions);
  rCtrl.AllocateLines(10);
  int nTabMinimumWidth{rCtrl.GetTabMinimumWidth()}; //NOLINT(clang-analyzer-deadcode.DeadStores, clang-diagnostic-unused-but-set-variable)
  rCtrl.SetTabMinimumWidth(0);
  nTabMinimumWidth = rCtrl.GetTabMinimumWidth(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  BOOL bCommandEvents{rCtrl.GetCommandEvents()}; //NOLINT(clang-analyzer-deadcode.DeadStores, clang-diagnostic-unused-but-set-variable)
  rCtrl.SetCommandEvents(FALSE);
  bCommandEvents = rCtrl.GetCommandEvents(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  Sci_Position nPos{rCtrl.GetCurrentPos()};
  int nStyle{rCtrl.GetStyleAt(nPos)}; //NOLINT(clang-analyzer-deadcode.DeadStores)
  nStyle = rCtrl.GetStyleIndexAt(nPos);
  UNREFERENCED_PARAMETER(nStyle);
  int nSubStyleStart{rCtrl.GetSubStylesStart(nStyle)}; //NOLINT(clang-analyzer-deadcode.DeadStores, clang-diagnostic-unused-but-set-variable)
  int nSubStyleLength{rCtrl.GetSubStylesLength(nStyle)}; //NOLINT(clang-analyzer-deadcode.DeadStores, clang-diagnostic-unused-but-set-variable)
  rCtrl.AllocateSubStyles(nStyle, 2);
  nSubStyleStart = rCtrl.GetSubStylesStart(nStyle); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nSubStyleLength = rCtrl.GetSubStylesLength(nStyle); //NOLINT(clang-analyzer-deadcode.DeadStores)
  const int nDistanceToSecondaryStyles{rCtrl.DistanceToSecondaryStyles()};
  UNREFERENCED_PARAMETER(nDistanceToSecondaryStyles);
  rCtrl.FreeSubStyles();
  int nLineTypesAllowed{rCtrl.GetLineEndTypesAllowed()}; //NOLINT(clang-analyzer-deadcode.DeadStores, clang-diagnostic-unused-but-set-variable)
  const int nLineTypesSupported{rCtrl.GetLineEndTypesSupported()};
  UNREFERENCED_PARAMETER(nLineTypesSupported);
  const int nLineTypesActive{rCtrl.GetLineEndTypesActive()};
  UNREFERENCED_PARAMETER(nLineTypesActive);
  rCtrl.SetLineEndTypesAllowed(SC_LINE_END_TYPE_UNICODE);
  nLineTypesAllowed = rCtrl.GetLineEndTypesAllowed(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.AllocateExtendedStyles(3);
  rCtrl.ReleaseAllExtendedStyles();
  int nAutomaticFold{rCtrl.GetAutomaticFold()}; //NOLINT(clang-analyzer-deadcode.DeadStores, clang-diagnostic-unused-but-set-variable)
  rCtrl.SetAutomaticFold(SC_AUTOMATICFOLD_SHOW | SC_AUTOMATICFOLD_CHANGE);
  nAutomaticFold = rCtrl.GetAutomaticFold(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nPos = rCtrl.GetCurrentPos();
  auto nLine{rCtrl.LineFromPosition(nPos)};
  UNREFERENCED_PARAMETER(nLine);
  rCtrl.FoldAll(SC_FOLDACTION_TOGGLE);
  rCtrl.ScrollRange(40, 100);
  CString sLine1{rCtrl.GetCurLine()};
  CString sLine2{rCtrl.GetLine(0)};
  CString sProperty{rCtrl.GetScintillaProperty(_T("fold"))};
  CString sProperty2{rCtrl.GetPropertyExpanded(_T("fold"))};
  CString sTempText{rCtrl.GetText(10)};
  CString sFont{rCtrl.StyleGetFont(1)};
  const int nProperty = rCtrl.GetPropertyInt(_T("fold"), 0);
  UNREFERENCED_PARAMETER(nProperty);
  rCtrl.AppendText(2, _T("XY"));
  rCtrl.SetTargetStart(0);
  rCtrl.SetTargetEnd(10);
  rCtrl.ReplaceTarget(2, _T("XY"));
  CString Target{rCtrl.GetTargetText()};
  CString sAutoCCurrentText{rCtrl.AutoCGetCurrentText()};
  CString sLexerLanguage{rCtrl.GetLexerLanguage()};
  CString sPropertyNames{rCtrl.PropertyNames()};
  const int nPropertyType{rCtrl.PropertyType(_T("fold.at.else"))};
  UNREFERENCED_PARAMETER(nPropertyType);
  CString sPropertyDescription{rCtrl.DescribeProperty(_T("fold.at.else"))};
  CString sDescribeKeyWordSets{rCtrl.DescribeKeyWordSets()};
  rCtrl.SetWhitespaceSize(3);
  const int nWhitespaceSize{rCtrl.GetWhitespaceSize()};
  UNREFERENCED_PARAMETER(nWhitespaceSize);
  rCtrl.SetFontQuality(2);
  const int nFontQuality{rCtrl.GetFontQuality()};
  UNREFERENCED_PARAMETER(nFontQuality);
  rCtrl.SetFirstVisibleLine(3);
  rCtrl.SetMultiPaste(2);
  const int nMultiPaste{rCtrl.GetMultiPaste()};
  UNREFERENCED_PARAMETER(nMultiPaste);
  rCtrl.SetCaretSticky(1);
  const int nCaretSticky{rCtrl.GetCaretSticky()};
  UNREFERENCED_PARAMETER(nCaretSticky);
  CString sTag{rCtrl.GetTag(0)};
  const char* pszPointer{rCtrl.GetCharacterPointer()};
  UNREFERENCED_PARAMETER(pszPointer);
  rCtrl.SetSelectionNStart(0, 2);
  const Sci_Position nSelectionNStart{rCtrl.GetSelectionNStart(0)};
  UNREFERENCED_PARAMETER(nSelectionNStart);
  const Sci_Position nSelectionNEnd{rCtrl.GetSelectionNEnd(0)};
  UNREFERENCED_PARAMETER(nSelectionNEnd);
  rCtrl.SetAdditionalCaretsVisible(FALSE);
  const BOOL bAdditionCaretVisible{rCtrl.GetAdditionalCaretsVisible()};
  UNREFERENCED_PARAMETER(bAdditionCaretVisible);
  const auto nContractedFoldNext{rCtrl.ContractedFoldNext(0)};
  UNREFERENCED_PARAMETER(nContractedFoldNext);
  rCtrl.VerticalCentreCaret();
  rCtrl.ChangeLexerState(0, -1);
  const Sci_Position nCharacters{rCtrl.CountCharacters(0, 100)};
  UNREFERENCED_PARAMETER(nCharacters);
  const BOOL bTest{rCtrl.GetSelectionEmpty()};
  UNREFERENCED_PARAMETER(bTest);
  const int nPhasesDraw{rCtrl.GetPhasesDraw()};
  rCtrl.SetPhasesDraw(nPhasesDraw);
  rCtrl.AddTabStop(0, 40);
  rCtrl.AddTabStop(0, 60);
  const int nNextTabStop{rCtrl.GetNextTabStop(0, 40)};
  UNREFERENCED_PARAMETER(nNextTabStop);
  rCtrl.ClearTabStops(0);
  const int nIME{rCtrl.GetIMEInteraction()};
  rCtrl.SetIMEInteraction(nIME);
  rCtrl.CallTipSetPosStart(100);
  const Sci_Position nPosition{rCtrl.PositionRelative(0, 3)};
  UNREFERENCED_PARAMETER(nPosition);
  const int nMulti{rCtrl.AutoCGetMulti()};
  rCtrl.AutoCSetMulti(nMulti);
  const BOOL bMouseSelection{rCtrl.GetMouseSelectionRectangularSwitch()};
  rCtrl.SetMouseSelectionRectangularSwitch(bMouseSelection);
  rCtrl.DropSelectionN(0);
  rCtrl.SetRepresentation("\xe2\x84\xa6", "U+2126 \xe2\x84\xa6");
  CStringA sRepresentation;
#pragma warning(suppress: 26485)
  rCtrl.GetRepresentation("\xe2\x84\xa6", sRepresentation.GetBuffer(32));
  sRepresentation.ReleaseBuffer();
  rCtrl.ClearRepresentation("\xe2\x84\xa6");
  nStyle = rCtrl.GetStyleFromSubStyle(2);
  const int nPrimaryStyle{rCtrl.GetPrimaryStyleFromStyle(nStyle)};
  UNREFERENCED_PARAMETER(nPrimaryStyle);
  rCtrl.SetEdgeMode(EDGE_BACKGROUND);
  rCtrl.SetEdgeColour(RGB(200, 0, 0));
  rCtrl.MultiEdgeAddLine(3, RGB(0, 200, 0));
  rCtrl.MultiEdgeClearAll();
  rCtrl.SetMouseWheelCaptures(TRUE);
  const BOOL bCaptures{rCtrl.GetMouseWheelCaptures()};
  UNREFERENCED_PARAMETER(bCaptures);
  rCtrl.ToggleFoldShowText(28, _T("PJ"));
  rCtrl.FoldDisplayTextSetStyle(SC_FOLDDISPLAYTEXT_BOXED);
  int nMode{rCtrl.GetTabDrawMode()}; //NOLINT(clang-analyzer-deadcode.DeadStores, clang-diagnostic-unused-but-set-variable)
  rCtrl.SetTabDrawMode(SCTD_STRIKEOUT);
  nMode = rCtrl.GetTabDrawMode(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.UsePopUp(SC_POPUP_TEXT);
  const int nFrame{rCtrl.GetCaretLineFrame()};
  rCtrl.SetCaretLineFrame(nFrame);
  const Sci_Position nChars{rCtrl.CountCodeUnits(0, 3)};
  UNREFERENCED_PARAMETER(nChars);
  nPos = rCtrl.PositionRelativeCodeUnits(0, 3); //NOLINT(clang-analyzer-deadcode.DeadStores)
  int nIndex{rCtrl.GetLineCharacterIndex()}; //NOLINT(clang-analyzer-deadcode.DeadStores, clang-diagnostic-unused-but-set-variable)
  rCtrl.AllocateLineCharacterIndex(SC_LINECHARACTERINDEX_UTF16);
  nIndex = rCtrl.GetLineCharacterIndex(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.ReleaseLineCharacterIndex(SC_LINECHARACTERINDEX_UTF16);
  nLine = rCtrl.LineFromIndexPosition(0, 3);
  nPos = rCtrl.IndexPositionFromLine(nLine, 3); //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.SetCharacterCategoryOptimization(0x1000);
  int nTemp{rCtrl.GetCharacterCategoryOptimization()}; //NOLINT(clang-analyzer-deadcode.DeadStores, clang-diagnostic-unused-but-set-variable)
  nTemp = rCtrl.FoldDisplayTextGetStyle(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.FoldDisplayTextSetStyle(SC_FOLDDISPLAYTEXT_BOXED);
  nTemp = rCtrl.FoldDisplayTextGetStyle(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.SetDefaultFoldDisplayText(_T("X"));
  CString sTemp{rCtrl.GetDefaultFoldDisplayText()};
  rCtrl.SetTargetStartVirtualSpace(42);
  nPos = rCtrl.GetTargetStartVirtualSpace(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.SetTargetEndVirtualSpace(42);
  nPos = rCtrl.GetTargetEndVirtualSpace(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nPos = rCtrl.GetSelectionNStartVirtualSpace(0); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nPos = rCtrl.GetSelectionNEndVirtualSpace(0); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nPos = rCtrl.BraceMatchNext(0, 0); //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.EOLAnnotationSetText(0, _T("ASSEMBLER1"));
  rCtrl.EOLAnnotationSetText(1, _T("ASSEMBLER2"));
  CString sEOLText{rCtrl.EOLAnnotationGetText(0)};
  rCtrl.EOLAnnotationSetStyle(0, 3);
  rCtrl.EOLAnnotationSetStyle(1, 3);
  const int nEOLStyle{rCtrl.EOLAnnotationGetStyle(0)};
  UNREFERENCED_PARAMETER(nEOLStyle);
  //rCtrl.EOLAnnotationClearAll();
  rCtrl.EOLAnnotationSetVisible(ANNOTATION_STANDARD);
  const int nVisible{rCtrl.EOLAnnotationGetVisible()};
  UNREFERENCED_PARAMETER(nVisible);
  rCtrl.SetFontLocale("en-us");
  const int nSize{rCtrl.GetFontLocale(nullptr)};
  UNREFERENCED_PARAMETER(nSize);
  CStringA sFontLocale;
  rCtrl.GetFontLocale(sFontLocale.GetBuffer(nSize));
  sFontLocale.ReleaseBuffer();
  constexpr int redTrasparent(0x030000FF);
  rCtrl.MarkerSetForeTranslucent(SC_MARKNUM_FOLDEROPEN, redTrasparent);
  rCtrl.MarkerSetBackTranslucent(SC_MARKNUM_FOLDEROPEN, redTrasparent);
  rCtrl.MarkerSetBackSelectedTranslucent(SC_MARKNUM_FOLDEROPEN, redTrasparent);
  rCtrl.MarkerSetStrokeWidth(SC_MARKNUM_FOLDEROPEN, 50);
  rCtrl.SetElementColour(SC_ELEMENT_LIST, redTrasparent);
  rCtrl.SetElementColour(SC_ELEMENT_LIST_BACK, 0x0300FFFF);
  rCtrl.SetElementColour(SC_ELEMENT_LIST_SELECTED, 0xFF00FF00);
  int nColor{rCtrl.GetElementColour(SC_ELEMENT_LIST)};
  UNREFERENCED_PARAMETER(nColor);
  const BOOL bIsSet{rCtrl.GetElementIsSet(SC_ELEMENT_LIST)};
  UNREFERENCED_PARAMETER(bIsSet);
  const BOOL bAllow{rCtrl.GetElementAllowsTranslucent(SC_ELEMENT_LIST)};
  UNREFERENCED_PARAMETER(bAllow);
  //rCtrl.ResetElementColour(SC_ELEMENT_LIST);
  const BOOL bSupports{rCtrl.SupportsFeature(SC_SUPPORTS_FRACTIONAL_STROKE_WIDTH)};
  UNREFERENCED_PARAMETER(bSupports);
  rCtrl.IndicSetStrokeWidth(0, 50);
  const int nWidth{rCtrl.IndicGetStrokeWidth(0)};
  UNREFERENCED_PARAMETER(nWidth);
  int nLayer{rCtrl.MarkerGetLayer(SC_MARKNUM_FOLDEROPEN)}; //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.MarkerSetLayer(SC_MARKNUM_FOLDEROPEN, 3);
  nLayer = rCtrl.MarkerGetLayer(SC_MARKNUM_FOLDEROPEN);
  UNREFERENCED_PARAMETER(nLayer);
  nColor = rCtrl.GetElementBaseColour(SC_ELEMENT_LIST);
  UNREFERENCED_PARAMETER(nColor);
  nLayer = rCtrl.GetSelectionLayer(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.SetSelectionLayer(2);
  nLayer = rCtrl.GetSelectionLayer(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nLayer = rCtrl.GetCaretLineLayer(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.SetCaretLineLayer(4);
  nLayer = rCtrl.GetCaretLineLayer(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.ReplaceRectangular(0, "Hello World");
  rCtrl.ClearAllRepresentations();
  rCtrl.SetRepresentation("\x01", "U+001");
  rCtrl.SetRepresentationAppearance("\x01", SC_REPRESENTATION_BLOB);
  const int nRep{rCtrl.GetRepresentationAppearance("\x01")};
  UNREFERENCED_PARAMETER(nRep);
  rCtrl.SetRepresentationColour("\x01", RGB(255, 0, 0));
  const int nRepColour{rCtrl.GetRepresentationColour("\x01")};
  UNREFERENCED_PARAMETER(nRepColour);
  int nStatus{0};
#pragma warning(suppress: 26490)
  const LRESULT lResult{rCtrl.Call(SCI_GETREPRESENTATION, reinterpret_cast<WPARAM>("\x01"), 0, &nStatus)};
  UNREFERENCED_PARAMETER(lResult);
  UNREFERENCED_PARAMETER(nStatus);
  rCtrl.StyleSetCheckMonospaced(SCE_C_STRING, TRUE);
  const BOOL bMonospaced{rCtrl.StyleGetCheckMonospaced(SCE_C_STRING)};
  UNREFERENCED_PARAMETER(bMonospaced);
  rCtrl.SetCaretLineHighlightSubLine(TRUE);
  const BOOL bCaretLineHighlightSubLine{rCtrl.GetCaretLineHighlightSubLine()};
  UNREFERENCED_PARAMETER(bCaretLineHighlightSubLine);
  rCtrl.SetLayoutThreads(4);
  const int nThreads = rCtrl.GetLayoutThreads();
  UNREFERENCED_PARAMETER(nThreads);
  rCtrl.SetLineState(0, 12);
  const auto nState{rCtrl.GetLineState(0)};
  UNREFERENCED_PARAMETER(nState);
  const auto nMarker = rCtrl.MarkerNumberFromLine(0, 0);
  UNREFERENCED_PARAMETER(nMarker);
  const auto nMarkerHandle = rCtrl.MarkerHandleFromLine(0, 0);
  UNREFERENCED_PARAMETER(nMarkerHandle);
  nLine = rCtrl.MarkerLineFromHandle(nMarkerHandle); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nLine = rCtrl.MarkerNumberFromLine(0, 0); //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.MarkerAddSet(0, 13);
  rCtrl.SetLineIndentation(0, 3);
  const auto nIndentation{rCtrl.GetLineIndentation(0)};
  UNREFERENCED_PARAMETER(nIndentation);
  nPos = rCtrl.GetLineIndentPosition(0); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nPos = rCtrl.GetLineEndPosition(0); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nLine = rCtrl.GetFirstVisibleLine(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nLine = rCtrl.GetLineCount(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nPos = rCtrl.PositionFromLine(0); //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.LineScroll(3, 4);
  nLine = rCtrl.VisibleFromDocLine(0); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nLine = rCtrl.DocLineFromVisible(0); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nLine = rCtrl.WrapCount(0); //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.SetFoldLevel(0, 1);
  nLine = rCtrl.GetLastChild(0, 1); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nLine = rCtrl.GetFoldParent(0); //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.ShowLines(0, 1);
  rCtrl.HideLines(0, 1);
  const auto bVisible{rCtrl.GetLineVisible(0)};
  UNREFERENCED_PARAMETER(bVisible);
  rCtrl.SetFoldExpanded(0, TRUE);
  const auto bFolded{rCtrl.GetFoldExpanded(0)}; //NOLINT(clang-analyzer-deadcode.DeadStores)
  UNREFERENCED_PARAMETER(bFolded);
  rCtrl.FoldLine(0, SC_FOLDACTION_TOGGLE);
  rCtrl.FoldChildren(0, SC_FOLDACTION_TOGGLE);
  rCtrl.ExpandChildren(0, 1);
  rCtrl.EnsureVisibleEnforcePolicy(0);
  const auto nHeight(rCtrl.TextHeight(0));
  UNREFERENCED_PARAMETER(nHeight);
  nLine = rCtrl.LinesOnScreen(); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nPos = rCtrl.GetLineSelStartPosition(0); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nPos = rCtrl.GetLineSelEndPosition(0); //NOLINT(clang-analyzer-deadcode.DeadStores)
  nPos = rCtrl.FindColumn(0, 3); //NOLINT(clang-analyzer-deadcode.DeadStores)
  rCtrl.MarginSetText(0, _T("X"));
  rCtrl.AnnotationSetText(0, _T("Y"));
  CStringA sMargin;
  rCtrl.MarginGetText(0, sMargin.GetBuffer(rCtrl.MarginGetText(0, nullptr)));
  sMargin.ReleaseBuffer();
  const auto sMargin2{rCtrl.MarginGetText(0)};
  rCtrl.MarginSetStyle(0, 3);
  nStyle = rCtrl.MarginGetStyle(0); //NOLINT(clang-analyzer-deadcode.DeadStores)
  const auto sWordChars{rCtrl.GetWordChars()};
  const auto sPunctuationChars{rCtrl.GetPunctuationChars()};
  const auto sStyle{rCtrl.NameOfStyle(1)};
  const auto sTags{rCtrl.TagsOfStyle(1)};
  const auto sDescription{rCtrl.DescriptionOfStyle(1)};
  const auto sLocale{rCtrl.GetFontLocale()};
  auto sStyles{rCtrl.MarginGetStyles(0)};
  const auto sAnnotation{rCtrl.AnnotationGetText(0)};
  sStyles = rCtrl.AnnotationGetStyles(0);
  const auto sBases{rCtrl.GetSubStyleBases()};
  const auto sWhitespace{rCtrl.GetWhitespaceChars()};
  rCtrl.AnnotationSetText(0, _T("G"));
  rCtrl.AnnotationSetStyle(0, 2);
  const auto nStyle2{rCtrl.AnnotationGetStyle(0)};
  UNREFERENCED_PARAMETER(nStyle2);
  rCtrl.AnnotationSetStyles(0, "H");
  const auto nAGetLines{rCtrl.AnnotationGetLines(0)};
  UNREFERENCED_PARAMETER(nAGetLines);
  */
#endif

  //As another example, get the previous 2 characters and if it is "res" case sensitive
  //then display a list which allows "resize", "restart" and "restore"
  if ((nStartSel == nEndSel) && (nStartSel >= 3))
  {
    Sci_TextRangeFull tr{};
    tr.chrg.cpMin = nStartSel - 3;
    tr.chrg.cpMax = nEndSel;
    CStringA sText;
    tr.lpstrText = sText.GetBuffer(3);
    rCtrl.GetTextRangeFull(&tr);
    sText.ReleaseBuffer();

    //Display the auto completion list
    if (sText == "res")
    {
      //Display the auto completion list
      rCtrl.AutoCShow(3, _T("resize\nrestart\nrestore"));
    }
  }

  //As another example, get the previous 16 characters and if it is "SAMPLE_INDICATOR" case insensitive
  //then display that text with a squiggly underline
  if ((nStartSel == nEndSel) && (nStartSel >= 16))
  {
    Sci_TextRangeFull tr{};
    tr.chrg.cpMin = nStartSel - 16;
    tr.chrg.cpMax = nEndSel;
    CStringA sText;
    tr.lpstrText = sText.GetBuffer(16);
    rCtrl.GetTextRangeFull(&tr);
    sText.ReleaseBuffer();

    //Display the Sample indicator
    if (sText.CompareNoCase("sample_indicator") == 0)
    {
      //For demonstration purposes lets make the "C" Comment style into hotspots which turn red
      rCtrl.SetIndicatorCurrent(0);
      rCtrl.StyleSetHotSpot(SCE_C_COMMENT, TRUE);
      rCtrl.SetHotspotActiveFore(TRUE, RGB(255, 0, 0));

      //Show the indicator
#pragma warning(suppress: 26472)
      rCtrl.IndicatorFillRange(tr.chrg.cpMin, static_cast<int>(nStartSel));
    }
  }
}

//A simple example of call tips
void CScintillaDemoView::OnDwellStart(_Inout_ SCNotification* pSCNotification)
{
  CScintillaCtrl& rCtrl{GetCtrl()};

  //Only display the call tip if the scintilla window has focus
  const CWnd* pFocusWnd{GetFocus()};
  if (pFocusWnd != nullptr)
  {
    if (pFocusWnd->GetSafeHwnd() == rCtrl.GetSafeHwnd())
    {
      //Get the previous 7 characters and next 7 characters around
      //the current dwell position and if it is "author " case insensitive
      //then display "PJ Naughter" as a call tip
      Sci_TextRangeFull tr{};
      tr.chrg.cpMin = max(0, pSCNotification->position - 7);
      tr.chrg.cpMax = min(pSCNotification->position + 7, rCtrl.GetLength());
      CStringA sText;
      tr.lpstrText = sText.GetBuffer(14);
      rCtrl.GetTextRangeFull(&tr);
      sText.ReleaseBuffer();

      //Display the call tip
      sText.MakeUpper();
      if (sText.Find("AUTHOR ") != -1)
        rCtrl.CallTipShow(pSCNotification->position, _T("PJ Naughter"));
    }
  }
}

void CScintillaDemoView::OnDwellEnd(_Inout_ SCNotification* /*pSCNotification*/)
{
  CScintillaCtrl& rCtrl{GetCtrl()};

  //Cancel any outstanding call tip
  if (rCtrl.CallTipActive())
    rCtrl.CallTipCancel();
}

#pragma warning(suppress: 26434)
void CScintillaDemoView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
  //Let the base class do its thing
  __super::OnActivate(nState, pWndOther, bMinimized);

  CScintillaCtrl& rCtrl{GetCtrl()};

  //Cancel any outstanding call tip
  if ((nState == WA_INACTIVE) && rCtrl.CallTipActive())
    rCtrl.CallTipCancel();
}

void CScintillaDemoView::OnModifyAttemptRO(_Inout_ SCNotification* /*pSCNotification*/)
{
#pragma warning(suppress: 26493)
  if (AfxMessageBox(IDP_ALLOW_MODIFY_READONLY_FILE, MB_YESNO) == IDYES)
    GetCtrl().SetReadOnly(FALSE);
}

#pragma warning(suppress: 26440)
void CScintillaDemoView::OnModified(_Inout_ SCNotification* pSCNotification)
{
  if (pSCNotification->modificationType & SC_MOD_INSERTCHECK)
  {
  #ifdef _DEBUG
    //Just some demo code to test out SCI_CHANGEINSERTION
    CScintillaCtrl& rCtrl{GetCtrl()};
#pragma warning(suppress: 26486)
    if (strcmp(pSCNotification->text, "Dublin") == 0)
      rCtrl.ChangeInsertion(18, _T("Capital of Ireland"));
  #endif
  }
}

std::unique_ptr<CScintillaCtrl> CScintillaDemoView::CreateScintillaControl()
{
  return std::make_unique<CScintillaDemoCtrl>();
}

#pragma warning(suppress: 26434)
int CScintillaDemoView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  //Let the base class do its thing
  if (__super::OnCreate(lpCreateStruct) == -1)
    return -1;

  //Create the C++ Lexer
#pragma warning(suppress: 26429)
  const CScintillaDemoApp* theApp{dynamic_cast<CScintillaDemoApp*>(AfxGetApp())};
  ASSERT(theApp != nullptr);
  m_pCLexer = theApp->m_pCreateLexer("cpp");
  if (m_pCLexer == nullptr)
    return -1;

  return 0;
}
