class CLispTabView : public CTabViewImpl<CLispTabView>
{
public:
	DECLARE_WND_CLASS_EX(_T("WTL_TabView"), 0, COLOR_APPWORKSPACE)

	void UpdateTitleBar()
	{
		if (!m_wndTitleBar.IsWindow() || m_lpstrTitleBarBase == NULL) {
			return;   // nothing to do
		}
		if (m_nActivePage != -1)
		{
			CLispIDEView *view = (CLispIDEView*)GetPageData(m_nActivePage);
			if (view) {
				m_wndTitleBar.SetWindowText(CString("LispIDE - ") + view->m_FilePath);
			}
		}
	}

	bool CreateTabControl()
	{
		m_tab.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TCS_TOOLTIPS | TCS_MULTILINE, 0, m_nTabID);
		ATLASSERT(m_tab.m_hWnd != NULL);
		if (m_tab.m_hWnd == NULL) {
			return false;
		}
		m_tab.SetFont(AtlGetDefaultGuiFont());
		m_tab.SetItemExtra(sizeof(TABVIEWPAGE));
		CImageList imagelist;
		imagelist.Create(IDB_DOCSTATE, 5, 0, RGB(0, 0, 0));
		SetImageList(imagelist);
		m_cyTabHeight = this->CalcTabHeight();
		return true;
	}

	int CalcTabHeight()
	{
		int rows = m_tab.GetRowCount();
		if (rows > 0) {
			CRect rect;
			m_tab.GetItemRect(0, rect);
			return rect.Height()* rows + 4;
		}
		return 0;
	}

	void UpdateLayout()
	{
		RECT rect;
		GetClientRect(&rect);
		if (m_tab.IsWindow() && ((m_tab.GetStyle() & WS_VISIBLE) != 0)) {
			m_tab.SetWindowPos(NULL, 0, 0, rect.right - rect.left, m_cyTabHeight, SWP_NOZORDER);
		}
		if (m_nActivePage != -1) {
			::SetWindowPos(GetPageHWND(m_nActivePage), NULL, 0, m_cyTabHeight, rect.right - rect.left, rect.bottom - rect.top - m_cyTabHeight, SWP_NOZORDER);
		}
		int newTabHeight = this->CalcTabHeight();
		if (newTabHeight != m_cyTabHeight) {
			m_cyTabHeight = newTabHeight;
			UpdateLayout();
		}
	}
};