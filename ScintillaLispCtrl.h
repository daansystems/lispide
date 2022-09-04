
#pragma once

#ifndef __SCINTILLALISPCTRL_H__
#define __SCINTILLALISPCTRL_H__


#define CWnd ATL::CWindow
#define DECLARE_DYNAMIC(x) // Do nothing
#define IMPLEMENT_DYNAMIC(x, y) // Do nothing
#define ASSERT ATLASSERT
#define CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam) \
	Create(lpszClassName, HWND(pParentWnd), _U_RECT((RECT&)rect), lpszWindowName, dwStyle, dwExStyle, nID, lpParam)

#include "ScintillaCtrl.h"
#include "Lexilla.h"
#include "SciLexer.h"

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

class SCINTILLACTRL_EXT_CLASS CScintillaLispCtrl : public CScintillaCtrl
{
public:
	BOOL	m_CanLisp;
	CFont	m_Font;
	UINT	m_KeyWordSet;

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
		// SetupDirectAccess();

		SetCallDirect(FALSE);

		ILexer5* pLexer = CreateLexer("lisp");
		SetILexer(pLexer);

		StyleClearAll();



		// SCI_SETILEXER(pLexer);
		// SetLexer(SCLEX_LISP);
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
		case 6:
			SetKeyWords(0, g_JanetKeyWords);
			break;
		}

		StyleSetBack(STYLE_BRACELIGHT, RGB(0, 255, 0));
		StyleSetBack(STYLE_BRACEBAD, RGB(255, 0, 0));
	}

};

// #include "ScintillaCtrl.cpp"

#endif //#ifndef __SCINTILLACTRL_H__
