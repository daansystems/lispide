/*
Module : ScintillaCtrl.h
Purpose: Defines the interface for an MFC wrapper class for the Scintilla edit control (www.scintilla.org)
Created: PJN / 19-03-2004

Copyright (c) 2004 - 2018 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise)
when your product is released in binary form. You are allowed to modify the source code in any way you want
except you cannot modify the copyright details at the top of each module. If you want to distribute source
code with your application, then you are only allowed to distribute versions released by the author. This is
to maintain a single distribution point for the source code.

*/


//////////////////// Macros / Defines /////////////////////////////////////////

#pragma once

#ifndef __SCINTILLACTRL_H__
#define __SCINTILLACTRL_H__

#ifndef SCINTILLA_H
#pragma message("To avoid this message, please put scintilla.h in your pre compiled header (normally stdafx.h)")
#include <Scintilla.h>
#endif //#ifndef SCINTILLA_H

#ifndef __ATTR_SAL
#pragma message("To avoid this message, please put sal.h in your pre compiled header (normally stdafx.h)")
#include <sal.h>
#endif //#ifndef __ATTR_SAL

#ifndef SCINTILLACTRL_EXT_CLASS
#define SCINTILLACTRL_EXT_CLASS
#endif //#ifndef SCINTILLACTRL_EXT_CLASS

// A few basic colors
const COLORREF black = RGB(0, 0, 0);
const COLORREF grey = RGB(128, 128, 128);
const COLORREF white = RGB(255, 255, 255);
const COLORREF green = RGB(0, 128, 0);
const COLORREF red = RGB(255, 0, 0);
const COLORREF blue = RGB(0, 0, 255);
const COLORREF yellow = RGB(255, 255, 0);
const COLORREF magenta = RGB(255, 0, 255);
const COLORREF cyan = RGB(0, 255, 255);
const COLORREF brown = RGB(165, 42, 42);

struct SScintillaColors
{
	int         iItem;
	COLORREF    rgb;
};

static SScintillaColors g_rgbSyntaxLisp[] =
{
	{ SCE_LISP_DEFAULT, blue },
{ SCE_LISP_COMMENT, grey },
{ SCE_LISP_NUMBER , green },
{ SCE_LISP_KEYWORD, blue },
{ SCE_LISP_KEYWORD_KW, blue },
{ SCE_LISP_SYMBOL, red },
{ SCE_LISP_STRING, magenta },
{ SCE_LISP_STRINGEOL, magenta },
{ SCE_LISP_IDENTIFIER, black },
{ SCE_LISP_OPERATOR, red },
{ SCE_LISP_SPECIAL, red },
{ SCE_LISP_MULTI_COMMENT, cyan },
{ -1,  0 }
};
//////////////////// Classes //////////////////////////////////////////////////

class SCINTILLACTRL_EXT_CLASS CScintillaCtrl : public ATL::CWindow
{
public:
	//Constructors / Destructors
	CScintillaCtrl();
	virtual ~CScintillaCtrl();
	BOOL	m_CanLisp;
	CFont m_Font;
	UINT m_KeyWordSet;
	//Creation
	BOOL Create(DWORD dwStyle, RECT& rect, CWindow* pParentWnd, UINT nID, DWORD dwExStyle = 0, LPVOID lpParam = nullptr);

	//Misc
	void SetupDirectAccess();
	LRESULT Call(_In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
	LRESULT GetDirectFunction();
	LRESULT GetDirectPointer();
	BOOL    GetCallDirect() const;
	void    SetCallDirect(_In_ BOOL bDirect);

	//Unicode support
#ifdef _UNICODE
	void AddText(_In_ int length, _In_ const wchar_t* text);
	void InsertText(_In_ Sci_Position pos, _In_z_ const wchar_t* text);
	void ChangeInsertion(_In_ int length, _In_z_ const wchar_t* text);
	CStringW GetSelText();
	CStringW GetCurLine();
	void StyleSetFont(_In_ int style, _In_z_ const wchar_t* fontName);
	void SetWordChars(_In_z_ const wchar_t* characters);
	void AutoCShow(_In_ int lenEntered, _In_z_ const wchar_t* itemList);
	void AutoCStops(_In_z_ const wchar_t* characterSet);
	void AutoCSelect(_In_z_ const wchar_t* text);
	void AutoCSetFillUps(_In_z_ const wchar_t* characterSet);
	void UserListShow(_In_ int listType, _In_z_ const wchar_t* itemList);
	CStringW GetLine(_In_ int line);
	void ReplaceSel(_In_z_ const wchar_t* text);
	void SetText(_In_z_ const wchar_t* text);
	CStringW GetText(_In_ int length);
	int ReplaceTarget(_In_ int length, _In_ const wchar_t* text);
	int ReplaceTargetRE(_In_ int length, _In_ const wchar_t* text);
	int SearchInTarget(_In_ int length, _In_ const wchar_t* text);
	void CallTipShow(_In_ Sci_Position pos, _In_z_ const wchar_t* definition);
	int TextWidth(_In_ int style, _In_z_ const wchar_t* text);
	void AppendText(_In_ int length, _In_ const wchar_t* text);
	int SearchNext(_In_ int flags, _In_z_ const wchar_t* text);
	int SearchPrev(_In_ int flags, _In_z_ const wchar_t* text);
	void CopyText(_In_ int length, _In_ const wchar_t* text);
	void SetWhitespaceChars(_In_z_ const wchar_t* characters);
	void SetProperty(_In_z_ const wchar_t* key, _In_z_ const wchar_t* value);
	void SetKeyWords(_In_ int keywordSet, _In_z_ const wchar_t* keyWords);
	void SetIdentifiers(_In_ int style, _In_z_ const wchar_t* identifiers);
	void SetLexerLanguage(_In_z_ const wchar_t* language);
	void LoadLexerLibrary(_In_z_ const wchar_t* path);
	CStringW GetProperty(_In_z_ const wchar_t* key);
	CStringW GetPropertyExpanded(_In_z_ const wchar_t* key);
	int GetPropertyInt(_In_z_ const wchar_t* key, _In_ int defaultValue);
	CStringW StyleGetFont(_In_ int style);
	void MarginSetText(_In_ int line, _In_z_ const wchar_t* text);
	//Note we do not have a MarginGetText method as Scintilla does not provide a way of a priori working out a valid length of a UTF8 buffer for SCI_MARGINGETTEXT
	void AnnotationSetText(_In_ int line, _In_z_ const wchar_t* text);
	//Note we do not have a AnnotationGetText method as Scintilla does not provide a way of a priori working out a valid length of a UTF8 buffer for SCI_ANNOTATIONGETTEXT
	CStringW AutoCGetCurrentText();
	CStringW GetLexerLanguage();
	CStringW PropertyNames();
	int PropertyType(_In_z_ const wchar_t* name);
	void ToggleFoldShowText(_In_ int line, _In_ const wchar_t* text);
	CStringW DescribeProperty(_In_z_ const wchar_t* name);
	CStringW DescribeKeyWordSets();
	CStringW GetTag(_In_ int tagNumber);
	CStringW GetWordChars();
	CStringW GetWhitespaceChars();
	CStringW GetPunctuationChars();
	CStringW GetTargetText();
	CStringW NameOfStyle(_In_ int style);
	CStringW TagsOfStyle(_In_ int style);
	CStringW DescriptionOfStyle(_In_ int style);

	static CStringW UTF82W(_In_NLS_string_(nLength) const char* pszText, _In_ int nLength);
	static CStringA W2UTF8(_In_NLS_string_(nLength) const wchar_t* pszText, _In_ int nLength);
#else
	CStringA GetSelText();
	CStringA GetCurLine();
	CStringA GetLine(_In_ int line);
	CStringA GetProperty(_In_z_ const char* key);
	CStringA GetText(_In_ int length);
	CStringA GetPropertyExpanded(_In_z_ const char* key);
	CStringA StyleGetFont(_In_ int style);
	CStringA AutoCGetCurrentText();
	CStringA GetLexerLanguage();
	CStringA PropertyNames();
	CStringA DescribeProperty(_In_z_ const char* name);
	CStringA DescribeKeyWordSets();
	CStringA GetTag(_In_ int tagNumber);
	CStringA GetWordChars();
	CStringA GetWhitespaceChars();
	CStringA GetPunctuationChars();
	CStringA GetTargetText();
	CStringA NameOfStyle(_In_ int style);
	CStringA TagsOfStyle(_In_ int style);
	CStringA DescriptionOfStyle(_In_ int style);
#endif //#ifdef _UNICODE

	//Auto generated using the "ConvertScintillaiface.js" script
	void AddText(_In_ int length, _In_reads_bytes_(length) const char* text);
	void AddStyledText(_In_ int length, _In_reads_bytes_(length) char* c);
	void InsertText(_In_ Sci_Position pos, _In_z_ const char* text);
	void ChangeInsertion(_In_ int length, _In_reads_bytes_(length) const char* text);
	void ClearAll();
	void DeleteRange(_In_ Sci_Position start, _In_ int lengthDelete);
	void ClearDocumentStyle();
	int GetLength();
	int GetCharAt(_In_ Sci_Position pos);
	Sci_Position GetCurrentPos();
	Sci_Position GetAnchor();
	int GetStyleAt(_In_ Sci_Position pos);
	void Redo();
	void SetUndoCollection(_In_ BOOL collectUndo);
	void SelectAll();
	void SetSavePoint();
	int GetStyledText(_Inout_ Sci_TextRange* tr);
	BOOL CanRedo();
	int MarkerLineFromHandle(_In_ int markerHandle);
	void MarkerDeleteHandle(_In_ int markerHandle);
	BOOL GetUndoCollection();
	int GetViewWS();
	void SetViewWS(_In_ int viewWS);
	int GetTabDrawMode();
	void SetTabDrawMode(_In_ int tabDrawMode);
	Sci_Position PositionFromPoint(_In_ int x, _In_ int y);
	Sci_Position PositionFromPointClose(_In_ int x, _In_ int y);
	void GotoLine(_In_ int line);
	void GotoPos(_In_ Sci_Position caret);
	void SetAnchor(_In_ Sci_Position anchor);
	int GetCurLine(_In_ int length, _Inout_opt_ char* text);
	Sci_Position GetEndStyled();
	void ConvertEOLs(_In_ int eolMode);
	int GetEOLMode();
	void SetEOLMode(_In_ int eolMode);
	void StartStyling(_In_ Sci_Position start, _In_ int unused);
	void SetStyling(_In_ int length, _In_ int style);
	BOOL GetBufferedDraw();
	void SetBufferedDraw(_In_ BOOL buffered);
	void SetTabWidth(_In_ int tabWidth);
	int GetTabWidth();
	void ClearTabStops(_In_ int line);
	void AddTabStop(_In_ int line, _In_ int x);
	int GetNextTabStop(_In_ int line, _In_ int x);
	void SetCodePage(_In_ int codePage);
	int GetIMEInteraction();
	void SetIMEInteraction(_In_ int imeInteraction);
	void MarkerDefine(_In_ int markerNumber, _In_ int markerSymbol);
	void MarkerSetFore(_In_ int markerNumber, _In_ COLORREF fore);
	void MarkerSetBack(_In_ int markerNumber, _In_ COLORREF back);
	void MarkerSetBackSelected(_In_ int markerNumber, _In_ COLORREF back);
	void MarkerEnableHighlight(_In_ BOOL enabled);
	int MarkerAdd(_In_ int line, _In_ int markerNumber);
	void MarkerDelete(_In_ int line, _In_ int markerNumber);
	void MarkerDeleteAll(_In_ int markerNumber);
	int MarkerGet(_In_ int line);
	int MarkerNext(_In_ int lineStart, _In_ int markerMask);
	int MarkerPrevious(_In_ int lineStart, _In_ int markerMask);
	void MarkerDefinePixmap(_In_ int markerNumber, _In_ const char* pixmap);
	void MarkerAddSet(_In_ int line, _In_ int markerSet);
	void MarkerSetAlpha(_In_ int markerNumber, _In_ int alpha);
	void SetMarginTypeN(_In_ int margin, _In_ int marginType);
	int GetMarginTypeN(_In_ int margin);
	void SetMarginWidthN(_In_ int margin, _In_ int pixelWidth);
	int GetMarginWidthN(_In_ int margin);
	void SetMarginMaskN(_In_ int margin, _In_ int mask);
	int GetMarginMaskN(_In_ int margin);
	void SetMarginSensitiveN(_In_ int margin, _In_ BOOL sensitive);
	BOOL GetMarginSensitiveN(_In_ int margin);
	void SetMarginCursorN(_In_ int margin, _In_ int cursor);
	int GetMarginCursorN(_In_ int margin);
	void SetMarginBackN(_In_ int margin, _In_ COLORREF back);
	COLORREF GetMarginBackN(_In_ int margin);
	void SetMargins(_In_ int margins);
	int GetMargins();
	void StyleClearAll();
	void StyleSetFore(_In_ int style, _In_ COLORREF fore);
	void StyleSetBack(_In_ int style, _In_ COLORREF back);
	void StyleSetBold(_In_ int style, _In_ BOOL bold);
	void StyleSetItalic(_In_ int style, _In_ BOOL italic);
	void StyleSetSize(_In_ int style, _In_ int sizePoints);
	void StyleSetFont(_In_ int style, _In_z_ const char* fontName);
	void StyleSetEOLFilled(_In_ int style, _In_ BOOL eolFilled);
	void StyleResetDefault();
	void StyleSetUnderline(_In_ int style, _In_ BOOL underline);
	COLORREF StyleGetFore(_In_ int style);
	COLORREF StyleGetBack(_In_ int style);
	BOOL StyleGetBold(_In_ int style);
	BOOL StyleGetItalic(_In_ int style);
	int StyleGetSize(_In_ int style);
	int StyleGetFont(_In_ int style, _Inout_ char* fontName);
	BOOL StyleGetEOLFilled(_In_ int style);
	BOOL StyleGetUnderline(_In_ int style);
	int StyleGetCase(_In_ int style);
	int StyleGetCharacterSet(_In_ int style);
	BOOL StyleGetVisible(_In_ int style);
	BOOL StyleGetChangeable(_In_ int style);
	BOOL StyleGetHotSpot(_In_ int style);
	void StyleSetCase(_In_ int style, _In_ int caseVisible);
	void StyleSetSizeFractional(_In_ int style, _In_ int sizeHundredthPoints);
	int StyleGetSizeFractional(_In_ int style);
	void StyleSetWeight(_In_ int style, _In_ int weight);
	int StyleGetWeight(_In_ int style);
	void StyleSetCharacterSet(_In_ int style, _In_ int characterSet);
	void StyleSetHotSpot(_In_ int style, _In_ BOOL hotspot);
	void SetSelFore(_In_ BOOL useSetting, _In_ COLORREF fore);
	void SetSelBack(_In_ BOOL useSetting, _In_ COLORREF back);
	int GetSelAlpha();
	void SetSelAlpha(_In_ int alpha);
	BOOL GetSelEOLFilled();
	void SetSelEOLFilled(_In_ BOOL filled);
	void SetCaretFore(_In_ COLORREF fore);
	void AssignCmdKey(_In_ DWORD keyDefinition, _In_ int sciCommand);
	void ClearCmdKey(_In_ DWORD keyDefinition);
	void ClearAllCmdKeys();
	void SetStylingEx(_In_ int length, _In_ const char* styles);
	void StyleSetVisible(_In_ int style, _In_ BOOL visible);
	int GetCaretPeriod();
	void SetCaretPeriod(_In_ int periodMilliseconds);
	void SetWordChars(_In_ const char* characters);
	int GetWordChars(_Inout_opt_ char* characters);
	void BeginUndoAction();
	void EndUndoAction();
	void IndicSetStyle(_In_ int indicator, _In_ int indicatorStyle);
	int IndicGetStyle(_In_ int indicator);
	void IndicSetFore(_In_ int indicator, _In_ COLORREF fore);
	COLORREF IndicGetFore(_In_ int indicator);
	void IndicSetUnder(_In_ int indicator, _In_ BOOL under);
	BOOL IndicGetUnder(_In_ int indicator);
	void IndicSetHoverStyle(_In_ int indicator, _In_ int indicatorStyle);
	int IndicGetHoverStyle(_In_ int indicator);
	void IndicSetHoverFore(_In_ int indicator, _In_ COLORREF fore);
	COLORREF IndicGetHoverFore(_In_ int indicator);
	void IndicSetFlags(_In_ int indicator, _In_ int flags);
	int IndicGetFlags(_In_ int indicator);
	void SetWhitespaceFore(_In_ BOOL useSetting, _In_ COLORREF fore);
	void SetWhitespaceBack(_In_ BOOL useSetting, _In_ COLORREF back);
	void SetWhitespaceSize(_In_ int size);
	int GetWhitespaceSize();
	void SetLineState(_In_ int line, _In_ int state);
	int GetLineState(_In_ int line);
	int GetMaxLineState();
	BOOL GetCaretLineVisible();
	void SetCaretLineVisible(_In_ BOOL show);
	COLORREF GetCaretLineBack();
	void SetCaretLineBack(_In_ COLORREF back);
	int GetCaretLineFrame();
	void SetCaretLineFrame(_In_ int width);
	void StyleSetChangeable(_In_ int style, _In_ BOOL changeable);
	void AutoCShow(_In_ int lengthEntered, _In_z_ const char* itemList);
	void AutoCCancel();
	BOOL AutoCActive();
	Sci_Position AutoCPosStart();
	void AutoCComplete();
	void AutoCStops(_In_z_ const char* characterSet);
	void AutoCSetSeparator(_In_ int separatorCharacter);
	int AutoCGetSeparator();
	void AutoCSelect(_In_z_ const char* select);
	void AutoCSetCancelAtStart(_In_ BOOL cancel);
	BOOL AutoCGetCancelAtStart();
	void AutoCSetFillUps(_In_z_ const char* characterSet);
	void AutoCSetChooseSingle(_In_ BOOL chooseSingle);
	BOOL AutoCGetChooseSingle();
	void AutoCSetIgnoreCase(_In_ BOOL ignoreCase);
	BOOL AutoCGetIgnoreCase();
	void UserListShow(_In_ int listType, _In_z_ const char* itemList);
	void AutoCSetAutoHide(_In_ BOOL autoHide);
	BOOL AutoCGetAutoHide();
	void AutoCSetDropRestOfWord(_In_ BOOL dropRestOfWord);
	BOOL AutoCGetDropRestOfWord();
	void RegisterImage(_In_ int type, _In_ const char* xpmData);
	void ClearRegisteredImages();
	int AutoCGetTypeSeparator();
	void AutoCSetTypeSeparator(_In_ int separatorCharacter);
	void AutoCSetMaxWidth(_In_ int characterCount);
	int AutoCGetMaxWidth();
	void AutoCSetMaxHeight(_In_ int rowCount);
	int AutoCGetMaxHeight();
	void SetIndent(_In_ int indentSize);
	int GetIndent();
	void SetUseTabs(_In_ BOOL useTabs);
	BOOL GetUseTabs();
	void SetLineIndentation(_In_ int line, _In_ int indentation);
	int GetLineIndentation(_In_ int line);
	Sci_Position GetLineIndentPosition(_In_ int line);
	int GetColumn(_In_ Sci_Position pos);
	int CountCharacters(_In_ Sci_Position start, _In_ Sci_Position end);
	void SetHScrollBar(_In_ BOOL visible);
	BOOL GetHScrollBar();
	void SetIndentationGuides(_In_ int indentView);
	int GetIndentationGuides();
	void SetHighlightGuide(_In_ int column);
	int GetHighlightGuide();
	Sci_Position GetLineEndPosition(_In_ int line);
	int GetCodePage();
	COLORREF GetCaretFore();
	BOOL GetReadOnly();
	void SetCurrentPos(_In_ Sci_Position caret);
	void SetSelectionStart(_In_ Sci_Position anchor);
	Sci_Position GetSelectionStart();
	void SetSelectionEnd(_In_ Sci_Position caret);
	Sci_Position GetSelectionEnd();
	void SetEmptySelection(_In_ Sci_Position caret);
	void SetPrintMagnification(_In_ int magnification);
	int GetPrintMagnification();
	void SetPrintColourMode(_In_ int mode);
	int GetPrintColourMode();
	Sci_Position FindText(_In_ int searchFlags, _Inout_ Sci_TextToFind* ft);
	Sci_Position FormatRange(_In_ BOOL draw, _In_ Sci_RangeToFormat* fr);
	int GetFirstVisibleLine();
	int GetLine(_In_ int line, _Inout_ char* text);
	int GetLineCount();
	void SetMarginLeft(_In_ int pixelWidth);
	int GetMarginLeft();
	void SetMarginRight(_In_ int pixelWidth);
	int GetMarginRight();
	BOOL GetModify();
	void SetSel(_In_ Sci_Position anchor, _In_ Sci_Position caret);
	int GetSelText(_Inout_opt_ char* text);
	int GetTextRange(_Inout_ Sci_TextRange* tr);
	void HideSelection(_In_ BOOL hide);
	int PointXFromPosition(_In_ Sci_Position pos);
	int PointYFromPosition(_In_ Sci_Position pos);
	int LineFromPosition(_In_ Sci_Position pos);
	Sci_Position PositionFromLine(_In_ int line);
	void LineScroll(_In_ int columns, _In_ int lines);
	void ScrollCaret();
	void ScrollRange(_In_ Sci_Position secondary, _In_ Sci_Position primary);
	void ReplaceSel(_In_z_ const char* text);
	void SetReadOnly(_In_ BOOL readOnly);
	void Null();
	BOOL CanPaste();
	BOOL CanUndo();
	void EmptyUndoBuffer();
	void Undo();
	void Cut();
	void Copy();
	void Paste();
	void Clear();
	void SetText(_In_z_ const char* text);
	int GetText(_In_ int length, _Inout_updates_opt_(length) char* text);
	int GetTextLength();
	void SetOvertype(_In_ BOOL overType);
	BOOL GetOvertype();
	void SetCaretWidth(_In_ int pixelWidth);
	int GetCaretWidth();
	void SetTargetStart(_In_ Sci_Position start);
	Sci_Position GetTargetStart();
	void SetTargetEnd(_In_ Sci_Position end);
	Sci_Position GetTargetEnd();
	void SetTargetRange(_In_ Sci_Position start, _In_ Sci_Position end);
	int GetTargetText(_Inout_opt_ char* text);
	void TargetFromSelection();
	void TargetWholeDocument();
	int ReplaceTarget(_In_ int length, _In_ const char* text);
	int ReplaceTargetRE(_In_ int length, _In_ const char* text);
	int SearchInTarget(_In_ int length, _In_reads_bytes_(length) const char* text);
	void SetSearchFlags(_In_ int searchFlags);
	int GetSearchFlags();
	void CallTipShow(_In_ Sci_Position pos, _In_z_ const char* definition);
	void CallTipCancel();
	BOOL CallTipActive();
	Sci_Position CallTipPosStart();
	void CallTipSetPosStart(_In_ int posStart);
	void CallTipSetHlt(_In_ int highlightStart, _In_ int highlightEnd);
	void CallTipSetBack(_In_ COLORREF back);
	void CallTipSetFore(_In_ COLORREF fore);
	void CallTipSetForeHlt(_In_ COLORREF fore);
	void CallTipUseStyle(_In_ int tabSize);
	void CallTipSetPosition(_In_ BOOL above);
	int VisibleFromDocLine(_In_ int docLine);
	int DocLineFromVisible(_In_ int displayLine);
	int WrapCount(_In_ int docLine);
	void SetFoldLevel(_In_ int line, _In_ int level);
	int GetFoldLevel(_In_ int line);
	int GetLastChild(_In_ int line, _In_ int level);
	int GetFoldParent(_In_ int line);
	void ShowLines(_In_ int lineStart, _In_ int lineEnd);
	void HideLines(_In_ int lineStart, _In_ int lineEnd);
	BOOL GetLineVisible(_In_ int line);
	BOOL GetAllLinesVisible();
	void SetFoldExpanded(_In_ int line, _In_ BOOL expanded);
	BOOL GetFoldExpanded(_In_ int line);
	void ToggleFold(_In_ int line);
	void ToggleFoldShowText(_In_ int line, _In_ const char* text);
	void FoldDisplayTextSetStyle(_In_ int style);
	void FoldLine(_In_ int line, _In_ int action);
	void FoldChildren(_In_ int line, _In_ int action);
	void ExpandChildren(_In_ int line, _In_ int level);
	void FoldAll(_In_ int action);
	void EnsureVisible(_In_ int line);
	void SetAutomaticFold(_In_ int automaticFold);
	int GetAutomaticFold();
	void SetFoldFlags(_In_ int flags);
	void EnsureVisibleEnforcePolicy(_In_ int line);
	void SetTabIndents(_In_ BOOL tabIndents);
	BOOL GetTabIndents();
	void SetBackSpaceUnIndents(_In_ BOOL bsUnIndents);
	BOOL GetBackSpaceUnIndents();
	void SetMouseDwellTime(_In_ int periodMilliseconds);
	int GetMouseDwellTime();
	int WordStartPosition(_In_ Sci_Position pos, _In_ BOOL onlyWordCharacters);
	int WordEndPosition(_In_ Sci_Position pos, _In_ BOOL onlyWordCharacters);
	BOOL IsRangeWord(_In_ Sci_Position start, _In_ Sci_Position end);
	void SetIdleStyling(_In_ int idleStyling);
	int GetIdleStyling();
	void SetWrapMode(_In_ int wrapMode);
	int GetWrapMode();
	void SetWrapVisualFlags(_In_ int wrapVisualFlags);
	int GetWrapVisualFlags();
	void SetWrapVisualFlagsLocation(_In_ int wrapVisualFlagsLocation);
	int GetWrapVisualFlagsLocation();
	void SetWrapStartIndent(_In_ int indent);
	int GetWrapStartIndent();
	void SetWrapIndentMode(_In_ int wrapIndentMode);
	int GetWrapIndentMode();
	void SetLayoutCache(_In_ int cacheMode);
	int GetLayoutCache();
	void SetScrollWidth(_In_ int pixelWidth);
	int GetScrollWidth();
	void SetScrollWidthTracking(_In_ BOOL tracking);
	BOOL GetScrollWidthTracking();
	int TextWidth(_In_ int style, _In_z_ const char* text);
	void SetEndAtLastLine(_In_ BOOL endAtLastLine);
	BOOL GetEndAtLastLine();
	int TextHeight(_In_ int line);
	void SetVScrollBar(_In_ BOOL visible);
	BOOL GetVScrollBar();
	void AppendText(_In_ int length, _In_reads_bytes_(length) const char* text);
	int GetPhasesDraw();
	void SetPhasesDraw(_In_ int phases);
	void SetFontQuality(_In_ int fontQuality);
	int GetFontQuality();
	void SetFirstVisibleLine(_In_ int displayLine);
	void SetMultiPaste(_In_ int multiPaste);
	int GetMultiPaste();
	int GetTag(_In_ int tagNumber, _Inout_opt_ char* tagValue);
	void LinesJoin();
	void LinesSplit(_In_ int pixelWidth);
	void SetFoldMarginColour(_In_ BOOL useSetting, _In_ COLORREF back);
	void SetFoldMarginHiColour(_In_ BOOL useSetting, _In_ COLORREF fore);
	void SetAccessibility(_In_ int accessibility);
	int GetAccessibility();
	void LineDown();
	void LineDownExtend();
	void LineUp();
	void LineUpExtend();
	void CharLeft();
	void CharLeftExtend();
	void CharRight();
	void CharRightExtend();
	void WordLeft();
	void WordLeftExtend();
	void WordRight();
	void WordRightExtend();
	void Home();
	void HomeExtend();
	void LineEnd();
	void LineEndExtend();
	void DocumentStart();
	void DocumentStartExtend();
	void DocumentEnd();
	void DocumentEndExtend();
	void PageUp();
	void PageUpExtend();
	void PageDown();
	void PageDownExtend();
	void EditToggleOvertype();
	void Cancel();
	void DeleteBack();
	void Tab();
	void BackTab();
	void NewLine();
	void FormFeed();
	void VCHome();
	void VCHomeExtend();
	void ZoomIn();
	void ZoomOut();
	void DelWordLeft();
	void DelWordRight();
	void DelWordRightEnd();
	void LineCut();
	void LineDelete();
	void LineTranspose();
	void LineReverse();
	void LineDuplicate();
	void LowerCase();
	void UpperCase();
	void LineScrollDown();
	void LineScrollUp();
	void DeleteBackNotLine();
	void HomeDisplay();
	void HomeDisplayExtend();
	void LineEndDisplay();
	void LineEndDisplayExtend();
	void HomeWrap();
	void HomeWrapExtend();
	void LineEndWrap();
	void LineEndWrapExtend();
	void VCHomeWrap();
	void VCHomeWrapExtend();
	void LineCopy();
	void MoveCaretInsideView();
	int LineLength(_In_ int line);
	void BraceHighlight(_In_ Sci_Position posA, _In_ Sci_Position posB);
	void BraceHighlightIndicator(_In_ BOOL useSetting, _In_ int indicator);
	void BraceBadLight(_In_ Sci_Position pos);
	void BraceBadLightIndicator(_In_ BOOL useSetting, _In_ int indicator);
	Sci_Position BraceMatch(_In_ Sci_Position pos, _In_ int maxReStyle);
	BOOL GetViewEOL();
	void SetViewEOL(_In_ BOOL visible);
	void* GetDocPointer();
	void SetDocPointer(_In_opt_ void* doc);
	void SetModEventMask(_In_ int eventMask);
	int GetEdgeColumn();
	void SetEdgeColumn(_In_ int column);
	int GetEdgeMode();
	void SetEdgeMode(_In_ int edgeMode);
	COLORREF GetEdgeColour();
	void SetEdgeColour(_In_ COLORREF edgeColour);
	void MultiEdgeAddLine(_In_ int column, _In_ COLORREF edgeColour);
	void MultiEdgeClearAll();
	void SearchAnchor();
	int SearchNext(_In_ int searchFlags, _In_z_ const char* text);
	int SearchPrev(_In_ int searchFlags, _In_z_ const char* text);
	int LinesOnScreen();
	void UsePopUp(_In_ int popUpMode);
	BOOL SelectionIsRectangle();
	void SetZoom(_In_ int zoomInPoints);
	int GetZoom();
	void* CreateDocument(_In_ int bytes, _In_ int documentOption);
	void AddRefDocument(_In_ void* doc);
	void ReleaseDocument(_In_ void* doc);
	int GetModEventMask();
	void SCISetFocus(_In_ BOOL focus);
	BOOL GetFocus();
	void SetStatus(_In_ int status);
	int GetStatus();
	void SetMouseDownCaptures(_In_ BOOL captures);
	BOOL GetMouseDownCaptures();
	void SetMouseWheelCaptures(_In_ BOOL captures);
	BOOL GetMouseWheelCaptures();
	void SetCursor(_In_ int cursorType);
	int GetCursor();
	void SetControlCharSymbol(_In_ int symbol);
	int GetControlCharSymbol();
	void WordPartLeft();
	void WordPartLeftExtend();
	void WordPartRight();
	void WordPartRightExtend();
	void SetVisiblePolicy(_In_ int visiblePolicy, _In_ int visibleSlop);
	void DelLineLeft();
	void DelLineRight();
	void SetXOffset(_In_ int xOffset);
	int GetXOffset();
	void ChooseCaretX();
	void GrabFocus();
	void SetXCaretPolicy(_In_ int caretPolicy, _In_ int caretSlop);
	void SetYCaretPolicy(_In_ int caretPolicy, _In_ int caretSlop);
	void SetPrintWrapMode(_In_ int wrapMode);
	int GetPrintWrapMode();
	void SetHotspotActiveFore(_In_ BOOL useSetting, _In_ COLORREF fore);
	COLORREF GetHotspotActiveFore();
	void SetHotspotActiveBack(_In_ BOOL useSetting, _In_ COLORREF back);
	COLORREF GetHotspotActiveBack();
	void SetHotspotActiveUnderline(_In_ BOOL underline);
	BOOL GetHotspotActiveUnderline();
	void SetHotspotSingleLine(_In_ BOOL singleLine);
	BOOL GetHotspotSingleLine();
	void ParaDown();
	void ParaDownExtend();
	void ParaUp();
	void ParaUpExtend();
	Sci_Position PositionBefore(_In_ Sci_Position pos);
	Sci_Position PositionAfter(_In_ Sci_Position pos);
	Sci_Position PositionRelative(_In_ Sci_Position pos, _In_ int relative);
	void CopyRange(_In_ Sci_Position start, _In_ Sci_Position end);
	void CopyText(_In_ int length, _In_reads_bytes_(length) const char* text);
	void SetSelectionMode(_In_ int selectionMode);
	int GetSelectionMode();
	BOOL GetMoveExtendsSelection();
	Sci_Position GetLineSelStartPosition(_In_ int line);
	Sci_Position GetLineSelEndPosition(_In_ int line);
	void LineDownRectExtend();
	void LineUpRectExtend();
	void CharLeftRectExtend();
	void CharRightRectExtend();
	void HomeRectExtend();
	void VCHomeRectExtend();
	void LineEndRectExtend();
	void PageUpRectExtend();
	void PageDownRectExtend();
	void StutteredPageUp();
	void StutteredPageUpExtend();
	void StutteredPageDown();
	void StutteredPageDownExtend();
	void WordLeftEnd();
	void WordLeftEndExtend();
	void WordRightEnd();
	void WordRightEndExtend();
	void SetWhitespaceChars(_In_z_ const char* characters);
	int GetWhitespaceChars(_Inout_opt_ char* characters);
	void SetPunctuationChars(_In_z_ const char* characters);
	int GetPunctuationChars(_Inout_opt_ char* characters);
	void SetCharsDefault();
	int AutoCGetCurrent();
	int AutoCGetCurrentText(_Inout_opt_ char* text);
	void AutoCSetCaseInsensitiveBehaviour(_In_ int behaviour);
	int AutoCGetCaseInsensitiveBehaviour();
	void AutoCSetMulti(_In_ int multi);
	int AutoCGetMulti();
	void AutoCSetOrder(_In_ int order);
	int AutoCGetOrder();
	void Allocate(_In_ int bytes);
	int TargetAsUTF8(_Inout_ char* s);
	void SetLengthForEncode(_In_ int bytes);
	int EncodedFromUTF8(_In_ const char* utf8, _Inout_ char* encoded);
	int FindColumn(_In_ int line, _In_ int column);
	int GetCaretSticky();
	void SetCaretSticky(_In_ int useCaretStickyBehaviour);
	void ToggleCaretSticky();
	void SetPasteConvertEndings(_In_ BOOL convert);
	BOOL GetPasteConvertEndings();
	void SelectionDuplicate();
	void SetCaretLineBackAlpha(_In_ int alpha);
	int GetCaretLineBackAlpha();
	void SetCaretStyle(_In_ int caretStyle);
	int GetCaretStyle();
	void SetIndicatorCurrent(_In_ int indicator);
	int GetIndicatorCurrent();
	void SetIndicatorValue(_In_ int value);
	int GetIndicatorValue();
	void IndicatorFillRange(_In_ Sci_Position start, _In_ int lengthFill);
	void IndicatorClearRange(_In_ Sci_Position start, _In_ int lengthClear);
	int IndicatorAllOnFor(_In_ Sci_Position pos);
	int IndicatorValueAt(_In_ int indicator, _In_ Sci_Position pos);
	int IndicatorStart(_In_ int indicator, _In_ Sci_Position pos);
	int IndicatorEnd(_In_ int indicator, _In_ Sci_Position pos);
	void SetPositionCache(_In_ int size);
	int GetPositionCache();
	void CopyAllowLine();
	const char* GetCharacterPointer();
	void* GetRangePointer(_In_ Sci_Position start, _In_ int lengthRange);
	Sci_Position GetGapPosition();
	void IndicSetAlpha(_In_ int indicator, _In_ int alpha);
	int IndicGetAlpha(_In_ int indicator);
	void IndicSetOutlineAlpha(_In_ int indicator, _In_ int alpha);
	int IndicGetOutlineAlpha(_In_ int indicator);
	void SetExtraAscent(_In_ int extraAscent);
	int GetExtraAscent();
	void SetExtraDescent(_In_ int extraDescent);
	int GetExtraDescent();
	int MarkerSymbolDefined(_In_ int markerNumber);
	void MarginSetText(_In_ int line, _In_z_ const char* text);
	int MarginGetText(_In_ int line, _Inout_ char* text);
	void MarginSetStyle(_In_ int line, _In_ int style);
	int MarginGetStyle(_In_ int line);
	void MarginSetStyles(_In_ int line, _In_z_ const char* styles);
	int MarginGetStyles(_In_ int line, _Inout_ char* styles);
	void MarginTextClearAll();
	void MarginSetStyleOffset(_In_ int style);
	int MarginGetStyleOffset();
	void SetMarginOptions(_In_ int marginOptions);
	int GetMarginOptions();
	void AnnotationSetText(_In_ int line, _In_z_ const char* text);
	int AnnotationGetText(_In_ int line, _Inout_ char* text);
	void AnnotationSetStyle(_In_ int line, _In_ int style);
	int AnnotationGetStyle(_In_ int line);
	void AnnotationSetStyles(_In_ int line, _In_z_ const char* styles);
	int AnnotationGetStyles(_In_ int line, _Inout_ char* styles);
	int AnnotationGetLines(_In_ int line);
	void AnnotationClearAll();
	void AnnotationSetVisible(_In_ int visible);
	int AnnotationGetVisible();
	void AnnotationSetStyleOffset(_In_ int style);
	int AnnotationGetStyleOffset();
	void ReleaseAllExtendedStyles();
	int AllocateExtendedStyles(_In_ int numberStyles);
	void AddUndoAction(_In_ int token, _In_ int flags);
	Sci_Position CharPositionFromPoint(_In_ int x, _In_ int y);
	Sci_Position CharPositionFromPointClose(_In_ int x, _In_ int y);
	void SetMouseSelectionRectangularSwitch(_In_ BOOL mouseSelectionRectangularSwitch);
	BOOL GetMouseSelectionRectangularSwitch();
	void SetMultipleSelection(_In_ BOOL multipleSelection);
	BOOL GetMultipleSelection();
	void SetAdditionalSelectionTyping(_In_ BOOL additionalSelectionTyping);
	BOOL GetAdditionalSelectionTyping();
	void SetAdditionalCaretsBlink(_In_ BOOL additionalCaretsBlink);
	BOOL GetAdditionalCaretsBlink();
	void SetAdditionalCaretsVisible(_In_ BOOL additionalCaretsVisible);
	BOOL GetAdditionalCaretsVisible();
	int GetSelections();
	BOOL GetSelectionEmpty();
	void ClearSelections();
	void SetSelection(_In_ Sci_Position caret, _In_ Sci_Position anchor);
	void AddSelection(_In_ Sci_Position caret, _In_ Sci_Position anchor);
	void DropSelectionN(_In_ int selection);
	void SetMainSelection(_In_ int selection);
	int GetMainSelection();
	void SetSelectionNCaret(_In_ int selection, _In_ Sci_Position caret);
	Sci_Position GetSelectionNCaret(_In_ int selection);
	void SetSelectionNAnchor(_In_ int selection, _In_ Sci_Position anchor);
	Sci_Position GetSelectionNAnchor(_In_ int selection);
	void SetSelectionNCaretVirtualSpace(_In_ int selection, _In_ int space);
	int GetSelectionNCaretVirtualSpace(_In_ int selection);
	void SetSelectionNAnchorVirtualSpace(_In_ int selection, _In_ int space);
	int GetSelectionNAnchorVirtualSpace(_In_ int selection);
	void SetSelectionNStart(_In_ int selection, _In_ Sci_Position anchor);
	Sci_Position GetSelectionNStart(_In_ int selection);
	void SetSelectionNEnd(_In_ int selection, _In_ Sci_Position caret);
	Sci_Position GetSelectionNEnd(_In_ int selection);
	void SetRectangularSelectionCaret(_In_ Sci_Position caret);
	Sci_Position GetRectangularSelectionCaret();
	void SetRectangularSelectionAnchor(_In_ Sci_Position anchor);
	Sci_Position GetRectangularSelectionAnchor();
	void SetRectangularSelectionCaretVirtualSpace(_In_ int space);
	int GetRectangularSelectionCaretVirtualSpace();
	void SetRectangularSelectionAnchorVirtualSpace(_In_ int space);
	int GetRectangularSelectionAnchorVirtualSpace();
	void SetVirtualSpaceOptions(_In_ int virtualSpaceOptions);
	int GetVirtualSpaceOptions();
	void SetRectangularSelectionModifier(_In_ int modifier);
	int GetRectangularSelectionModifier();
	void SetAdditionalSelFore(_In_ COLORREF fore);
	void SetAdditionalSelBack(_In_ COLORREF back);
	void SetAdditionalSelAlpha(_In_ int alpha);
	int GetAdditionalSelAlpha();
	void SetAdditionalCaretFore(_In_ COLORREF fore);
	COLORREF GetAdditionalCaretFore();
	void RotateSelection();
	void SwapMainAnchorCaret();
	void MultipleSelectAddNext();
	void MultipleSelectAddEach();
	int ChangeLexerState(_In_ Sci_Position start, _In_ Sci_Position end);
	int ContractedFoldNext(_In_ int lineStart);
	void VerticalCentreCaret();
	void MoveSelectedLinesUp();
	void MoveSelectedLinesDown();
	void SetIdentifier(_In_ int identifier);
	int GetIdentifier();
	void RGBAImageSetWidth(_In_ int width);
	void RGBAImageSetHeight(_In_ int height);
	void RGBAImageSetScale(_In_ int scalePercent);
	void MarkerDefineRGBAImage(_In_ int markerNumber, _In_ const char* pixels);
	void RegisterRGBAImage(_In_ int type, _In_ const char* pixels);
	void ScrollToStart();
	void ScrollToEnd();
	void SetTechnology(_In_ int technology);
	int GetTechnology();
	void* CreateLoader(_In_ int bytes, _In_ int documentOption);
	void FindIndicatorShow(_In_ Sci_Position start, _In_ Sci_Position end);
	void FindIndicatorFlash(_In_ Sci_Position start, _In_ Sci_Position end);
	void FindIndicatorHide();
	void VCHomeDisplay();
	void VCHomeDisplayExtend();
	BOOL GetCaretLineVisibleAlways();
	void SetCaretLineVisibleAlways(_In_ BOOL alwaysVisible);
	void SetLineEndTypesAllowed(_In_ int lineEndBitSet);
	int GetLineEndTypesAllowed();
	int GetLineEndTypesActive();
	void SetRepresentation(_In_ const char* encodedCharacter, _In_ const char* representation);
	int GetRepresentation(_In_ const char* encodedCharacter, _Inout_z_ char* representation);
	void ClearRepresentation(_In_ const char* encodedCharacter);
	void StartRecord();
	void StopRecord();
	void SetLexer(_In_ int lexer);
	int GetLexer();
	void Colourise(_In_ Sci_Position start, _In_ Sci_Position end);
	void SetProperty(_In_z_ const char* key, _In_z_ const char* value);
	void SetKeyWords(_In_ int keyWordSet, _In_z_ const char* keyWords);
	void SetLexerLanguage(_In_z_ const char* language);
	void LoadLexerLibrary(_In_z_ const char* path);
	int GetProperty(_In_z_ const char* key, _Inout_opt_ char* value);
	int GetPropertyExpanded(_In_z_ const char* key, _Inout_opt_ char* value);
	int GetPropertyInt(_In_z_ const char* key, _In_ int defaultValue);
	int GetLexerLanguage(_Inout_opt_ char* language);
	void* PrivateLexerCall(_In_ int operation, _In_opt_ void* pointer);
	int PropertyNames(_Inout_opt_ char* names);
	int PropertyType(_In_z_ const char* name);
	int DescribeProperty(_In_z_ const char* name, _Inout_opt_ char* description);
	int DescribeKeyWordSets(_Inout_opt_ char* descriptions);
	int GetLineEndTypesSupported();
	int AllocateSubStyles(_In_ int styleBase, _In_ int numberStyles);
	int GetSubStylesStart(_In_ int styleBase);
	int GetSubStylesLength(_In_ int styleBase);
	int GetStyleFromSubStyle(_In_ int subStyle);
	int GetPrimaryStyleFromStyle(_In_ int style);
	void FreeSubStyles();
	void SetIdentifiers(_In_ int style, _In_z_ const char* identifiers);
	int DistanceToSecondaryStyles();
	int GetSubStyleBases(_Inout_z_ char* styles);
	int GetNamedStyles();
	int NameOfStyle(_In_ int style, _Inout_opt_ char* name);
	int TagsOfStyle(_In_ int style, _Inout_opt_ char* tags);
	int DescriptionOfStyle(_In_ int style, _Inout_opt_ char* description);
	int GetBidirectional();
	void SetBidirectional(_In_ int bidirectional);

	int OnUpdateUI(LPNMHDR nmhdr)
	{
		// do brace matching
		Sci_Position lStart = GetCurrentPos();
		int chBr = GetCharAt(lStart - 1);
		m_CanLisp = FALSE;
		if (chBr && CString("()[]{}").Find(chBr) != -1) {
			Sci_Position lEnd = BraceMatch(lStart - 1, 0);
			// if there is a matching brace highlight it
			if (lEnd >= 0) {
				BraceHighlight(lStart - 1, lEnd);
				m_CanLisp = TRUE;
			}
			// if there is NO matching brace erase old highlight
			else {
				BraceBadLight(lStart - 1);
			}
		}
		else {
			BraceHighlight(-1, -1);
		}

		if (GetSelectionEnd() != GetSelectionStart())
			m_CanLisp = TRUE;


		//		SetMsgHandled(false);
		return 0;
	}
	void Init() {
		//	SetupDirectAccess();
		StyleClearAll();
		SetLexer(SCLEX_LISP);
		SetMarginWidthN(0, 40);
		SetMarginWidthN(1, 0);

		//		ClearCmdKey(SCK_RETURN + (SCMOD_NORM << 16));
		//		ClearCmdKey(SCK_RETURN + (SCMOD_SHIFT << 16));

		int tabSize = g_Settings.GetUserProfileDword("Settings", "TabSize");
		SetTabWidth(tabSize);

		if (g_Settings.GetUserProfileDword("Settings", "TabUsesSpaces"))
			SetUseTabs(FALSE);

		// Set syntax colors


		DWORD fontsize = g_Settings.GetUserProfileDword("Settings", "FontSize");
		CString fontname = g_Settings.GetUserProfileString("Settings", "FontName", NULL);
		if (!fontname.IsEmpty() && fontsize) {
			m_Font.CreatePointFont(fontsize, fontname);
		}
		else {
			m_Font = (HFONT)GetStockObject(SYSTEM_FIXED_FONT);
		}

		LOGFONT logfont;
		m_Font.GetLogFont(logfont);
		int nPixels = (-MulDiv(logfont.lfHeight, 72, GetDeviceCaps(GetDC(), LOGPIXELSY)));
		for (long i = 0; g_rgbSyntaxLisp[i].iItem != -1; i++) {
			StyleSetFore(g_rgbSyntaxLisp[i].iItem, g_rgbSyntaxLisp[i].rgb);
			StyleSetFont(g_rgbSyntaxLisp[i].iItem, logfont.lfFaceName);
			StyleSetSize(g_rgbSyntaxLisp[i].iItem, nPixels);
		}

		DWORD keywordset = g_Settings.GetUserProfileDword("Settings", "KeywordSet");
		switch (keywordset) {
		case 0:
			SetKeyWords(0, g_LispKeyWords);
			break;
		case 1:
			SetKeyWords(0, g_SchemeKeyWords);
			break;
		case 2:
			SetKeyWords(0, g_NewLispKeyWords);
			break;
		case 3:
			SetKeyWords(0, g_ISLispKeyWords);
			break;
		case 4:
			SetKeyWords(0, g_PicoLispKeyWords);
			break;
		case 5:
			SetKeyWords(0, g_ClojureKeyWords);
			break;
		}

		StyleSetBack(STYLE_BRACELIGHT, RGB(0, 255, 0));
		StyleSetBack(STYLE_BRACEBAD, RGB(255, 0, 0));
	}
protected:
	//DECLARE_DYNAMIC(CScintillaCtrl)

  //Member variables
	BOOL    m_bCallDirect;
	LRESULT m_DirectFunction;
	LRESULT m_DirectPointer;
};


#endif //#ifndef __SCINTILLACTRL_H__
