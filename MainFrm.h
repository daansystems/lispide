// MainFrm.h : interface of the CMainFrame class
//
////////////////////////////////////////////////////////////////////////////

#pragma once

#define WINDOW_MENU_POSITION	5
#define RECENT_MENU_POSITION	2

static LPCTSTR lpcstrLispIDERegKey = _T("Software\\DaanSystems\\LispIDE");
static UINT FIND_REPLACE_MESSAGE = CFindReplaceDialog::GetFindReplaceMsg();
class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
	public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)
	CLispTabView m_view;
	CLispIDEPipe m_pipeview;
	CFindReplaceDialog *m_FindReplace;
	CCommandBarCtrl m_CmdBar;
	CHorSplitterWindow m_wndSplit;
	CRecentDocumentList m_mru;
	COpenDocumentList m_opendocs;
	CString m_FileExtensions;

	int m_DocumentCounter;
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if (CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;
		// catch enter key in find/replace dialog
		if (m_FindReplace && pMsg->message == WM_CHAR && pMsg->wParam == VK_RETURN && ::GetActiveWindow() == *m_FindReplace) {
			m_FindReplace->SendMessage(WM_COMMAND, IDOK);
		}
		return m_view.PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		BOOL enable = FALSE;
		BOOL canlisp = FALSE;
		int active_page = m_view.GetActivePage();
		if (active_page != -1) {
			CLispIDEView* pView = (CLispIDEView*)m_view.GetPageData(active_page);
			if (pView) {
				enable = pView->GetModify();
				m_view.SetPageImage(active_page, enable ? 1 : 0);
				canlisp = pView->m_CanLisp;
			}
		}
		UIEnable(ID_FILE_SAVE, enable);
		UIEnable(ID_FILE_SAVE_AS, enable);
		UIEnable(ID_EDIT_SENDTOLISP, canlisp);
		UIEnable(ID_EDIT_MACROEXPAND1, canlisp);
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_FILE_SAVE, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_FILE_SAVE_AS, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_SENDTOLISP, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_MACROEXPAND1, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_SETTINGS_TABUSESSPACES, UPDUI_MENUPOPUP | UPDUI_CHECKED)
		UPDATE_ELEMENT(ID_SETTINGS_EOL_WIN, UPDUI_MENUPOPUP | UPDUI_CHECKED)
		UPDATE_ELEMENT(ID_SETTINGS_EOL_MAC, UPDUI_MENUPOPUP | UPDUI_CHECKED)
		UPDATE_ELEMENT(ID_SETTINGS_EOL_NIX, UPDUI_MENUPOPUP | UPDUI_CHECKED)
		UPDATE_ELEMENT(ID_SETTINGS_TABSIZE_2, UPDUI_MENUPOPUP | UPDUI_CHECKED)
		UPDATE_ELEMENT(ID_SETTINGS_TABSIZE_4, UPDUI_MENUPOPUP | UPDUI_CHECKED)
		UPDATE_ELEMENT(ID_SETTINGS_TABSIZE_8, UPDUI_MENUPOPUP | UPDUI_CHECKED)
		UPDATE_ELEMENT(ID_KEYWORDSET_COMMONLISP, UPDUI_MENUPOPUP | UPDUI_CHECKED)
		UPDATE_ELEMENT(ID_KEYWORDSET_SCHEME, UPDUI_MENUPOPUP | UPDUI_CHECKED)
		UPDATE_ELEMENT(ID_KEYWORDSET_NEWLISP, UPDUI_MENUPOPUP | UPDUI_CHECKED)
		UPDATE_ELEMENT(ID_KEYWORDSET_ISLISP, UPDUI_MENUPOPUP | UPDUI_CHECKED)
		UPDATE_ELEMENT(ID_KEYWORDSET_PICOLISP, UPDUI_MENUPOPUP | UPDUI_CHECKED)
		UPDATE_ELEMENT(ID_KEYWORDSET_CLOJURE, UPDUI_MENUPOPUP | UPDUI_CHECKED)
		UPDATE_ELEMENT(ID_KEYWORDSET_JANET, UPDUI_MENUPOPUP | UPDUI_CHECKED)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP_EX(CMainFrame)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_CONTEXTMENU(OnContextMenu)
		MSG_WM_DROPFILES(OnDropFiles)
		MESSAGE_HANDLER_EX(WM_USER + 2, OnLispHandler)
		MESSAGE_HANDLER_EX(FIND_REPLACE_MESSAGE, OnFindReplace)
		COMMAND_ID_HANDLER_EX(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER_EX(ID_FILE_OPEN, OnFileOpen)
		COMMAND_ID_HANDLER_EX(ID_FILE_SAVE, OnFileSave)
		COMMAND_ID_HANDLER_EX(ID_FILE_SAVE_AS, OnFileSaveAs)
		COMMAND_ID_HANDLER_EX(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER_EX(ID_EDIT_CUT, OnEditCut)
		COMMAND_ID_HANDLER_EX(ID_EDIT_COPY, OnEditCopy)
		COMMAND_ID_HANDLER_EX(ID_EDIT_PASTE, OnEditPaste)
		COMMAND_ID_HANDLER_EX(ID_EDIT_UNDO, OnEditUndo)
		COMMAND_ID_HANDLER_EX(ID_EDIT_REDO, OnEditRedo)
		COMMAND_ID_HANDLER_EX(ID_EDIT_FIND, OnEditFind)
		COMMAND_ID_HANDLER_EX(ID_EDIT_REPLACE, OnEditFind)
		COMMAND_ID_HANDLER_EX(ID_EDIT_SENDTOLISP, OnEditSendToLisp)
		COMMAND_ID_HANDLER_EX(ID_EDIT_MACROEXPAND1, OnEditMacroExpand1)
		COMMAND_ID_HANDLER_EX(ID_EDIT_PRETTYPRINT, OnEditPrettyPrint)
		COMMAND_ID_HANDLER_EX(ID_RESETLISP, OnResetLisp)
		COMMAND_ID_HANDLER_EX(ID_SETTINGS_SETLISPPATH, OnSetLispPath)
		COMMAND_ID_HANDLER_EX(ID_SETTINGS_SETFONT, OnSetFont)
		COMMAND_ID_HANDLER_EX(ID_SETTINGS_TABUSESSPACES, OnTabUsesSpaces)
		COMMAND_ID_HANDLER_EX(ID_SETTINGS_EOL_WIN, OnSetEOL)
		COMMAND_ID_HANDLER_EX(ID_SETTINGS_EOL_MAC, OnSetEOL)
		COMMAND_ID_HANDLER_EX(ID_SETTINGS_EOL_NIX, OnSetEOL)
		COMMAND_ID_HANDLER_EX(ID_SETTINGS_TABSIZE_2, OnSetTabSize)
		COMMAND_ID_HANDLER_EX(ID_SETTINGS_TABSIZE_4, OnSetTabSize)
		COMMAND_ID_HANDLER_EX(ID_SETTINGS_TABSIZE_8, OnSetTabSize)
		COMMAND_ID_HANDLER_EX(ID_KEYWORDSET_COMMONLISP, OnSetKeyWordSet)
		COMMAND_ID_HANDLER_EX(ID_KEYWORDSET_SCHEME, OnSetKeyWordSet)
		COMMAND_ID_HANDLER_EX(ID_KEYWORDSET_NEWLISP, OnSetKeyWordSet)
		COMMAND_ID_HANDLER_EX(ID_KEYWORDSET_ISLISP, OnSetKeyWordSet)
		COMMAND_ID_HANDLER_EX(ID_KEYWORDSET_PICOLISP, OnSetKeyWordSet)
		COMMAND_ID_HANDLER_EX(ID_KEYWORDSET_CLOJURE, OnSetKeyWordSet)
		COMMAND_ID_HANDLER_EX(ID_KEYWORDSET_JANET, OnSetKeyWordSet)
		COMMAND_ID_HANDLER_EX(ID_HELP_COMMONLISPHYPERSPEC, OnCLHS)
		COMMAND_ID_HANDLER_EX(ID_HELP_COMMONLISPTHELANGUAGE, OnCLTL)
		COMMAND_ID_HANDLER_EX(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER_EX(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER_EX(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER_EX(ID_WINDOW_CLOSE, OnWindowClose)
		COMMAND_ID_HANDLER_EX(ID_WINDOW_CLOSE_ALL, OnWindowCloseAll)
		COMMAND_RANGE_HANDLER_EX(ID_WINDOW_TABFIRST, ID_WINDOW_TABLAST, OnWindowActivate)
		COMMAND_RANGE_HANDLER_EX(ID_FILE_MRU_FIRST, ID_FILE_MRU_LAST, OnFileRecent)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
		NOTIFY_CODE_HANDLER_EX(SCN_UPDATEUI, OnUpdateUI)
		END_MSG_MAP()

	LRESULT OnCLHS(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/) {
		TCHAR szBuffer[MAX_PATH];
		::GetModuleFileName(NULL, szBuffer, MAX_PATH);
		CString path(szBuffer);
		path.Delete(path.ReverseFind('\\'), MAX_PATH);
		HtmlHelp(this->m_hWnd, path + "\\CLHS.chm", HH_DISPLAY_TOPIC, NULL);
		return 1;
	}

	LRESULT OnCLTL(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/) {
		TCHAR szBuffer[MAX_PATH];
		::GetModuleFileName(NULL, szBuffer, MAX_PATH);
		CString path(szBuffer);
		path.Delete(path.ReverseFind('\\'), MAX_PATH);
		HtmlHelp(this->m_hWnd, path + "\\CLtL2.chm", HH_DISPLAY_TOPIC, NULL);
		return 1;
	}

	void OnContextMenu(HWND wnd, CPoint &point) {
		m_CmdBar.TrackPopupMenu(GetSubMenu(m_CmdBar.GetMenu(), 1), TPM_LEFTALIGN, point.x, point.y);
	}

	void OnDropFiles(HDROP hDropInfo) {
		SetActiveWindow();      // activate us first !
		UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
		for (UINT iFile = 0; iFile < nFiles; iFile++)
		{
			TCHAR szFileName[_MAX_PATH];
			::DragQueryFile(hDropInfo, iFile, szFileName, _MAX_PATH);
			OpenTheFile(CString(szFileName));
		}
		::DragFinish(hDropInfo);
	}

	void OnClose() {
		for (int i = 0; i < m_view.GetPageCount(); i++) {
			CLispIDEView* pView = (CLispIDEView*)m_view.GetPageData(i);
			m_opendocs.AddToList(pView->m_FilePath);
		}
		m_opendocs.WriteToRegistry(lpcstrLispIDERegKey);
		if (OnWindowCloseAll(0, 0, 0)) {
			SetMsgHandled(FALSE);
		}
	}

	LRESULT OnLispHandler(UINT uMsg, WPARAM wp, LPARAM lp)
	{
		m_pipeview.WriteStdOut("\n");
		m_pipeview.m_CarryNewLine = true;
		m_pipeview.Print(*((CString *)wp) + "\n");
		return 0;
	}

	LRESULT OnFindReplace(UINT uMsg, WPARAM wp, LPARAM lp)
	{
		if (m_FindReplace->IsTerminating()) {
			m_FindReplace = NULL;
			return TRUE;
		}
		CLispIDEView* pView = GetActiveView();
		if (pView) {
			if (m_FindReplace->FindNext() || m_FindReplace->ReplaceCurrent() || m_FindReplace->ReplaceAll()) {
				CString what = m_FindReplace->GetFindString();
				while (1) {
					Sci_TextToFind ft;
					ft.lpstrText = (const char *)(LPCTSTR)what;
					ft.chrg.cpMin = m_FindReplace->SearchDown() ? pView->GetCurrentPos() : pView->GetCurrentPos() - 1;
					ft.chrg.cpMax = m_FindReplace->SearchDown() ? pView->GetTextLength() : 0;
					int searchFlags = 0;
					if (m_FindReplace->MatchCase()) searchFlags += SCFIND_MATCHCASE;
					if (m_FindReplace->MatchWholeWord()) searchFlags += SCFIND_WHOLEWORD;

					if (m_FindReplace->ReplaceCurrent() || m_FindReplace->ReplaceAll()) {
						if (!pView->GetSelText().IsEmpty())
							pView->ReplaceSel(m_FindReplace->GetReplaceString());
					}
					if (pView->FindText(searchFlags, &ft) == -1) break;
					pView->SetSel(ft.chrgText.cpMin, ft.chrgText.cpMax);

					if (!m_FindReplace->ReplaceAll()) break;
				}
			}
		}
		return 0;
	}

	LRESULT OnUpdateUI(LPNMHDR lParam)
	{
		return CWindow(lParam->hwndFrom).SendMessage(WM_NOTIFY, 0, (LPARAM)lParam);
	}

	LRESULT OnEditCut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		return CWindow(::GetFocus()).SendMessage(WM_CUT);
	}

	LRESULT OnEditCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		return CWindow(::GetFocus()).SendMessage(WM_COPY);
	}
	LRESULT OnEditPaste(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		return CWindow(::GetFocus()).SendMessage(WM_PASTE);
	}

	LRESULT OnEditUndo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		return CWindow(::GetFocus()).SendMessage(WM_UNDO);
	}

	LRESULT OnEditRedo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		return CWindow(::GetFocus()).SendMessage(SCI_REDO);
	}

	LRESULT OnEditSendToLisp(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
	{
		CString pipetext = GetActiveView()->GetPipeString();
		if (!pipetext.IsEmpty())
			SendMessage(WM_USER + 2, (WPARAM)&pipetext, 0);
		return 1;
	}

	LRESULT OnEditMacroExpand1(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
	{
		CString pipetext = GetActiveView()->GetPipeString();
		if (!pipetext.IsEmpty()) {
			CString newtext = "(pprint (macroexpand-1 '" + pipetext + "))";
			SendMessage(WM_USER + 2, (WPARAM)&newtext, 0);
		}
		return 1;
	}

	LRESULT OnEditPrettyPrint(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
	{
		CString pipetext = GetActiveView()->GetPipeString();
		if (!pipetext.IsEmpty()) {

			CString newtext = "(pprint '" + pipetext + ")";

			SendMessage(WM_USER + 2, (WPARAM)&newtext, 0);
		}
		return 1;
	}

	LRESULT OnResetLisp(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
	{
		m_pipeview.WriteStdOut("\r\n");
		m_pipeview.StartLisp();
		return 1;
	}

	CLispIDEView* GetActiveView() {
		int nActivePage = m_view.GetActivePage();
		if (nActivePage != -1) {
			return (CLispIDEView*)m_view.GetPageData(nActivePage);
		}
		return NULL;
	}

	LRESULT OnEditFind(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
	{
		CLispIDEView* pView = GetActiveView();
		if (pView) {
			if (!m_FindReplace) {

				m_FindReplace = new CFindReplaceDialog();
				m_FindReplace->Create(wID == ID_EDIT_REPLACE ? FALSE : TRUE, // TRUE for Find, FALSE for FindReplace
					pView->GetSelText(),
					NULL,
					FR_DOWN, *this);
				//	m_FindReplace->SetWindowText("Find/Replace");
				m_FindReplace->SetActiveWindow();
			}
			m_FindReplace->ShowWindow(SW_SHOW);
		}
		return 1;
	}

	LRESULT OnCreate(LPCREATESTRUCT cs)
	{
		// create command bar window
		m_DocumentCounter = 0;
		g_Settings.SetRegKey(lpcstrLispIDERegKey);
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
		SetMenu(NULL);
		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
		CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		AddSimpleReBarBand(hWndCmdBar);
		AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
		CreateSimpleStatusBar();
		m_wndSplit.Create(*this, rcDefault);
		m_hWndClient = m_wndSplit;
		m_wndSplit.SetSplitterPanes(m_view.Create(m_wndSplit, rcDefault, NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE)
			, m_pipeview.Create(m_wndSplit, rcDefault, NULL, WS_CHILD | WS_VISIBLE
				| WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL |
				ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL |
				ES_WANTRETURN, WS_EX_CLIENTEDGE));
		UpdateLayout();
		m_wndSplit.SetSplitterPos(400);
		UIAddToolBar(hWndToolBar);
		UISetCheck(ID_VIEW_TOOLBAR, 1);
		UISetCheck(ID_VIEW_STATUS_BAR, 1);
		UISetCheck(ID_SETTINGS_EOL_WIN, g_Settings.GetUserProfileDword("Settings", "LineEnding") == 0);
		UISetCheck(ID_SETTINGS_EOL_MAC, g_Settings.GetUserProfileDword("Settings", "LineEnding") == 1);
		UISetCheck(ID_SETTINGS_EOL_NIX, g_Settings.GetUserProfileDword("Settings", "LineEnding") == 2);
		UISetCheck(ID_SETTINGS_TABUSESSPACES, g_Settings.GetUserProfileDword("Settings", "TabUsesSpaces") == 1);
		UISetCheck(ID_KEYWORDSET_COMMONLISP, g_Settings.GetUserProfileDword("Settings", "KeywordSet") == 0);
		UISetCheck(ID_KEYWORDSET_SCHEME, g_Settings.GetUserProfileDword("Settings", "KeywordSet") == 1);
		UISetCheck(ID_KEYWORDSET_NEWLISP, g_Settings.GetUserProfileDword("Settings", "KeywordSet") == 2);
		UISetCheck(ID_KEYWORDSET_ISLISP, g_Settings.GetUserProfileDword("Settings", "KeywordSet") == 3);
		UISetCheck(ID_KEYWORDSET_PICOLISP, g_Settings.GetUserProfileDword("Settings", "KeywordSet") == 4);
		UISetCheck(ID_KEYWORDSET_CLOJURE, g_Settings.GetUserProfileDword("Settings", "KeywordSet") == 5);
		UISetCheck(ID_KEYWORDSET_JANET, g_Settings.GetUserProfileDword("Settings", "KeywordSet") == 6);
		int tabSize = g_Settings.GetUserProfileDword("Settings", "TabSize");
		if (tabSize == 0) tabSize = 4;
		UISetCheck(ID_SETTINGS_TABSIZE_2, tabSize == 2);
		UISetCheck(ID_SETTINGS_TABSIZE_4, tabSize == 4);
		UISetCheck(ID_SETTINGS_TABSIZE_8, tabSize == 8);
		CMenuHandle menuMain = m_CmdBar.GetMenu();
		m_view.SetWindowMenu(menuMain.GetSubMenu(WINDOW_MENU_POSITION));
		m_view.m_nMenuItemsCount = 100;
		m_view.SetTitleBarWindow(*this);
		m_opendocs.SetMaxEntries(32);
		m_mru.SetMaxEntries(16);
		m_mru.SetMenuHandle(menuMain.GetSubMenu(0).GetSubMenu(RECENT_MENU_POSITION));
		m_mru.ReadFromRegistry(lpcstrLispIDERegKey);
		m_opendocs.ReadFromRegistry(lpcstrLispIDERegKey);
		m_opendocs.Sort();
		m_FindReplace = NULL;
		CSimpleArray<CString> docs;
		for (int i = 0; i < m_opendocs.m_arrDocs.GetSize(); i++) {
			docs.Add(m_opendocs.m_arrDocs[i].szDocName);
		}
		m_opendocs.m_arrDocs.RemoveAll();
		for (int i = 0; i < docs.GetSize(); i++) {
			OpenTheFile(docs[i]);
		}
		m_pipeview.m_LispPath = g_Settings.GetUserProfileString("Settings", "Lisp");
		if (m_pipeview.m_LispPath.IsEmpty()) {
			if (!OnSetLispPath(0, 0, NULL))
				exit(0);
		}
		m_pipeview.Init();
		m_pipeview.StartLisp();
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		DWORD keywordset = g_Settings.GetUserProfileDword("Settings", "KeywordSet");
		switch (keywordset) {
		case 0:
			m_FileExtensions = _T("Lisp Files (*.lisp;*.lsp)\0*.lisp;*.lsp\0All Files\0*.*\0");
			break;
		case 1:
			m_FileExtensions = _T("Scheme Files (*.scm;*.sps;*.sls;*.sld;*.rkt)\0*.scm;*.sps;*.sls;*.sld;*.rkt\0All Files\0*.*\0");
			break;
		case 2:
			m_FileExtensions = _T("newLisp Files (*.lsp)\0*.lsp\0All Files\0*.*\0");
			break;
		case 3:
			m_FileExtensions = _T("ISLisp Files (*.lsp)\0*.lsp\0All Files\0*.*\0");
			break;
		case 4:
			m_FileExtensions = _T("{PicoLisp Files (*.l)\0*.l\0All Files\0*.*\0");
			break;
		case 5:
			m_FileExtensions = _T("Clojure Files(*.clj)\0 * .clj\0All Files\0 * .*\0");
			break;
		case 6:
			m_FileExtensions = _T("Janet Files(*.janet)\0 * .janet\0All Files\0 * .*\0");
			break;
		}

		return 0;
	}

	LRESULT OnSetLispPath(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
	{
		CFileDialog fileDlg(true, NULL, NULL,
			OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
			_T("Lisp startup files (*.exe;*.bat;*.cmd)\0*.exe;*.bat;*.cmd\0"));
		fileDlg.m_ofn.lpstrTitle = _T("Set Lisp path");
		if (IDOK == fileDlg.DoModal()) {
			TCHAR path[MAX_PATH];
			::GetModuleFileName(GetModuleHandle(NULL), path, MAX_PATH);
			if (_tcsicmp(path, fileDlg.m_szFileName) == 0) {
				MessageBox("Please dont select LispIDE itself but your Lisp console executable.", "LispIDE", MB_OK | MB_ICONWARNING);
				return 0;
			}
			m_pipeview.m_LispPath = fileDlg.m_szFileName;
			if (!g_Settings.WriteUserProfileString("Settings", "Lisp", m_pipeview.m_LispPath)) {
				MessageBox("Cannot save setting.", "LispIDE", MB_OK | MB_ICONWARNING);
			}
			return 1;
		}
		return 0;
	}


	LRESULT OnSetFont(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
	{
		LOGFONT logfont;
		m_pipeview.m_Font.GetLogFont(logfont);
		CFontDialog f(&logfont);
		if (f.DoModal() == IDOK) {
			g_Settings.WriteUserProfileString("Settings", "FontName", f.GetFaceName());
			g_Settings.WriteUserProfileDword("Settings", "FontSize", f.GetSize());
			AskRestart();
		}
		return 0;
	}

	void AskRestart() {
		MessageBox("Please restart LispIDE", "LispIDE", MB_OK | MB_ICONWARNING);
	}

	LRESULT OnTabUsesSpaces(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
	{
		int check = UIGetState(ID_SETTINGS_TABUSESSPACES)& UPDUI_CHECKED;
		UISetCheck(ID_SETTINGS_TABUSESSPACES, !check);
		g_Settings.WriteUserProfileDword("Settings", "TabUsesSpaces", !check);
		AskRestart();
		return 0;
	}

	LRESULT OnSetKeyWordSet(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
	{
		switch (wID) {
		case ID_KEYWORDSET_COMMONLISP:
			g_Settings.WriteUserProfileDword("Settings", "KeywordSet", 0);
			break;
		case ID_KEYWORDSET_SCHEME:
			g_Settings.WriteUserProfileDword("Settings", "KeywordSet", 1);
			break;
		case ID_KEYWORDSET_NEWLISP:
			g_Settings.WriteUserProfileDword("Settings", "KeywordSet", 2);
			break;
		case ID_KEYWORDSET_ISLISP:
			g_Settings.WriteUserProfileDword("Settings", "KeywordSet", 3);
			break;
		case ID_KEYWORDSET_PICOLISP:
			g_Settings.WriteUserProfileDword("Settings", "KeywordSet", 4);
			break;
		case ID_KEYWORDSET_CLOJURE:
			g_Settings.WriteUserProfileDword("Settings", "KeywordSet", 5);
			break;
		case ID_KEYWORDSET_JANET:
			g_Settings.WriteUserProfileDword("Settings", "KeywordSet", 6);
			break;
		}
		AskRestart();
		return TRUE;
	}

	LRESULT OnSetEOL(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
	{
		int check = UIGetState(wID)& UPDUI_CHECKED;
		if (!check)
		{
			UISetCheck(ID_SETTINGS_EOL_WIN, false);
			UISetCheck(ID_SETTINGS_EOL_MAC, false);
			UISetCheck(ID_SETTINGS_EOL_NIX, false);
			short eolMode;
			UISetCheck(wID, true);
			switch (wID)
			{
			case ID_SETTINGS_EOL_MAC:
				eolMode = 1;
				break;
			case ID_SETTINGS_EOL_NIX:
				eolMode = 2;
				break;
			default:
				eolMode = 0;
				break;
			}
			g_Settings.WriteUserProfileDword("Settings", "LineEnding", eolMode);
			SetEOLs(eolMode);
		}
		return 0;
	}

	LRESULT OnSetTabSize(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
	{
		int check = UIGetState(wID)& UPDUI_CHECKED;
		if (!check) {
			UISetCheck(ID_SETTINGS_TABSIZE_2, false);
			UISetCheck(ID_SETTINGS_TABSIZE_4, false);
			UISetCheck(ID_SETTINGS_TABSIZE_8, false);
			UISetCheck(wID, true);
			short tabSize;
			switch (wID)
			{
			case ID_SETTINGS_TABSIZE_2:
				tabSize = 2;
				break;
			case ID_SETTINGS_TABSIZE_4:
				tabSize = 4;
				break;
			case ID_SETTINGS_TABSIZE_8:
				tabSize = 8;
				break;
			default:
				tabSize = 4;
				break;
			}
			g_Settings.WriteUserProfileDword("Settings", "TabSize", tabSize);
		}
		AskRestart();
		return 0;
	}

	void SetEOLs(int eolMode)
	{
		for (int i = 0; i < m_view.GetPageCount(); i++) {
			CLispIDEView* pView = (CLispIDEView*)m_view.GetPageData(i);
			pView->SetEOLMode(eolMode);
			pView->ConvertEOLs(eolMode);
		}
	}

	LRESULT OnDestroy()
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);
		SetMsgHandled(FALSE);
		return 1;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		CLispIDEView* pView = new CLispIDEView;
		pView->Create(m_view, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL, 0);
		pView->Init();

		

		CString docname;
		docname.Format("Document%d", ++m_DocumentCounter);
		m_view.AddPage(pView->m_hWnd, docname, 0, pView);
		return (LRESULT)pView;
	}
	LRESULT OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		CFileDialog fileDlg(true, _T("lisp"), NULL,
			OFN_FILEMUSTEXIST, m_FileExtensions);
		if (IDOK == fileDlg.DoModal()) {
			OpenTheFile(fileDlg.m_szFileName);

		}
		return 1;
	}

	LRESULT OpenTheFile(LPCTSTR filename) {
		for (int i = 0; i < m_view.GetPageCount(); i++) {
			CLispIDEView* pView = (CLispIDEView*)m_view.GetPageData(i);
			if (pView->m_FilePath == filename) {
				m_view.SetActivePage(i);
				return 0;
			}
		}
		CString path(filename);
		if (_access(filename, 0) != 0) {
			return 0;
		}
		int found = path.ReverseFind('\\');
		CLispIDEView* pView = (CLispIDEView*)OnFileNew(0, 0, 0);
		m_view.SetPageTitle(m_view.GetPageCount() - 1, path.Mid(found + 1));
		pView->LoadFile(filename);
		m_mru.AddToList(filename);
		m_mru.WriteToRegistry(lpcstrLispIDERegKey);
		m_view.UpdateTitleBar();
		return 1;
	}

	LRESULT OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		CLispIDEView* pView = GetActiveView();
		if (pView->m_FilePath.IsEmpty()) {
			return OnFileSaveAs(0, 0, 0);
		}
		else {
			return pView->SaveFile(pView->m_FilePath);
		}
	}

	LRESULT OnFileSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		CString sSelectedFile;
		CFileDialog fileDlg(false, _T("lisp"), NULL,
			OFN_OVERWRITEPROMPT, m_FileExtensions);
		if (IDOK == fileDlg.DoModal()) {
			sSelectedFile = fileDlg.m_szFileName;
			CLispIDEView* pView = GetActiveView();
			pView->SaveFile(sSelectedFile);
			m_view.SetPageTitle(m_view.GetActivePage(), fileDlg.m_szFileTitle);
			m_mru.AddToList(sSelectedFile);
			m_mru.WriteToRegistry(lpcstrLispIDERegKey);
			return 1;
		}
		return 0;
	}
	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		static BOOL bVisible = TRUE;	// initially visible
		bVisible = !bVisible;
		CReBarCtrl rebar = m_hWndToolBar;
		int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
		rebar.ShowBand(nBandIndex, bVisible);
		UISetCheck(ID_VIEW_TOOLBAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnWindowClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		int nActivePage = m_view.GetActivePage();
		if (nActivePage != -1) {
			CLispIDEView* pView = (CLispIDEView*)m_view.GetPageData(nActivePage);
			if (pView && pView->GetModify()) {
				CString warning;
				warning.Format("Save changes to %s ?", m_view.GetPageTitle(nActivePage));
				int iResponse = MessageBox(warning, "LispIDE", MB_YESNOCANCEL | MB_ICONWARNING);
				if (iResponse == IDCANCEL) return 0;
				if (iResponse == IDYES) {
					if (!OnFileSave(0, 0, 0)) return 0;
				}
			}
			m_view.RemovePage(nActivePage);
			return 1;
		}
		else {
			::MessageBeep((UINT)-1);
		}
		return 0;
	}

	LRESULT OnWindowCloseAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/)
	{
		int count = m_view.GetPageCount() - 1;
		for (int i = count; i >= 0; i--) {
			m_view.m_nActivePage = i;
			if (!OnWindowClose(0, 0, 0)) return 0;
		}
		return 1;
	}

	LRESULT OnWindowActivate(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
	{
		int nPage = wID - ID_WINDOW_TABFIRST;
		if (m_view.IsValidPageIndex(nPage)) {
			m_view.SetActivePage(nPage);
		}
		return 0;
	}

	LRESULT OnFileRecent(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
	{
		// get file name from the MRU list
		TCHAR szFile[MAX_PATH];
		if (m_mru.GetFromList(wID, szFile, MAX_PATH))
		{
			// open file
			if (OpenTheFile(szFile))
				m_mru.MoveToTop(wID);
			else
				m_mru.RemoveFromList(wID);
			m_mru.WriteToRegistry(lpcstrLispIDERegKey);
		}
		else
		{
			::MessageBeep(MB_ICONERROR);
		}
		return 0;
	}

};