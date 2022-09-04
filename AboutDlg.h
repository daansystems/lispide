// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_ABOUTBOX };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_WEBSITE, OnWebsite)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());
		TCHAR text[255];
		sprintf_s(text, "LispIDE v%.2f %s", get_version(), sizeof(void*) == 4 ? "32bit" : "64bit");
		SetDlgItemText(IDC_VERSION, text);
		return TRUE;
	}

	double get_version() {
		UINT year;
		UINT day;
		UINT hour;
		char month[4];
		UINT monthnum;
		sscanf_s(__DATE__, "%s %d %d", month, 4,  &day, &year);
		sscanf_s(__TIME__, "%d", &hour);
		char months[12][4] = { "Jan","Feb","Mar","Apr","May","Jun","Ju","Aug","Sep","Oct","Nov","Dec" };
		for (monthnum = 0; monthnum < 12; monthnum++) {
			if (strcmp(months[monthnum], month) == 0) break;
		}
		monthnum++;
		double result = double(year * 10000 + monthnum * 100 + day) + double(hour) / 100.0f;
		return result;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnWebsite(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		ShellExecute(NULL, NULL, "http://www.daansystems.com", NULL, NULL, SW_SHOWNORMAL);
		return 0;
	}
};
