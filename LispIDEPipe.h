// LispIDEView.h : interface of the CLispIDEPipe class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Tlhelp32.h"
#include <vector>

#define CRLF _T("\r\n")
#define _TEST_REDIR

#define MAXTEXT (1024*1024)

class CLispIDEPipe : public CWindowImpl<CLispIDEPipe, CScintillaCtrl>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, "Scintilla")

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	HWND m_hReceiver;
	UINT m_uCallbackMsg;
	DWORD m_hChildThreadId;
	DWORD m_hChildProcessId;
	CString m_LispPath;
	CSimpleArray<CString> m_History;
	int m_CurCommand;
	Sci_Position m_CurPos;
	bool m_CarryNewLine;
	bool m_newLISPMode;

	BEGIN_MSG_MAP_EX(CLispIDEPipe)
		MSG_WM_CREATE(OnCreate)
		MESSAGE_HANDLER(WM_KEYDOWN, OnWMKeyDown)
		MESSAGE_HANDLER(WM_CHAR, OnWMChar)
		MESSAGE_HANDLER(WM_PASTE, OnPaste)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		NOTIFY_CODE_HANDLER_EX(SCN_UPDATEUI, OnUpdateUI)
		END_MSG_MAP()

	LRESULT OnCreate(LPCREATESTRUCT cs) {
		m_hStdinWrite = NULL;
		m_hStdoutRead = NULL;
		m_hChildProcess = NULL;
		m_hChildThread = NULL;
		m_hOutputThread = NULL;
		m_hEvtStop = NULL;
		m_dwThreadId = 0;
		m_dwWaitTime = 100;
		m_CurCommand = 0;
		m_CurPos = -1;
		m_hChildThreadId = 0;
		m_hChildProcessId = 0;
		m_CarryNewLine = false;
		m_newLISPMode = false;
		SetMsgHandled(FALSE);
		return 0;
	}

	void StartLisp() {
		Open(m_LispPath);
	}

	void Init() {
		CScintillaCtrl::Init();
		SetMarginWidthN(0, 0);
		SetMarginWidthN(1, 0);
		SetWrapMode(SC_WRAP_WORD);
		DWORD keywordset = g_Settings.GetUserProfileDword("Settings", "KeywordSet");
		if (keywordset == 2) { // newlisp
			m_newLISPMode = true;
		}
	}

	int NextCommand() {
		m_CurCommand++;
		if (m_CurCommand >= m_History.GetSize()) {
			m_CurCommand = 0;
		}
		return m_CurCommand;
	}

	int PrevCommand() {
		m_CurCommand--;
		if (m_CurCommand < 0) {
			m_CurCommand = m_History.GetSize() - 1;
		}
		return m_CurCommand;
	}

	LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) {
		GetTopLevelParent().SendMessage(uMsg, wParam, lParam);
		return 1;
	}

	void GetSel(Sci_Position &start, Sci_Position &end) {
		start = GetSelectionStart();
		end = GetSelectionEnd();
	}

	LRESULT OnWMKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) {
		if (wParam == VK_CONTROL) {
			return TRUE;
		}
		Sci_Position currentpos = GetSelectionEnd();
		int len = GetLength();
		if (m_CurPos == -1 || m_CurPos > currentpos) {
			m_CurPos = currentpos;
			if (m_CurPos != len) {
				GotoPos(len);
				m_CurPos = len;
			}
		}
		switch (wParam) {
		case VK_UP:
			if (m_History.GetSize()) {
				SetSel(m_CurPos, len);
				CString command = m_History[PrevCommand()];
				ReplaceSel(command);
			}
			break;
		case VK_DOWN:
			if (m_History.GetSize()) {
				SetSel(m_CurPos, len);
				CString command = m_History[NextCommand()];
				ReplaceSel(command);
			}
		case VK_LEFT:
		case VK_BACK:
			if (GetCurrentPos() > m_CurPos)
				bHandled = FALSE;
			break;
		case VK_PRIOR:
		case VK_NEXT:
			break;
		case VK_HOME:
			SetSel(m_CurPos, m_CurPos);
			break;
		case VK_RETURN:
			GotoPos(len);
			bHandled = FALSE;
			break;
		default:
			bHandled = FALSE;
		}
		return 0;
	}

	LRESULT OnWMChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) {
		switch (wParam) {
		case VK_RETURN:
		{
			CString text;
			Sci_TextRange tr;
			tr.chrg.cpMin = m_CurPos;
			tr.chrg.cpMax = GetWindowTextLength();
			tr.lpstrText = (char *)text.GetBufferSetLength(tr.chrg.cpMax - tr.chrg.cpMin);
			GetTextRange(&tr);
			text.TrimRight("\r\n");
			if (!text.IsEmpty()) m_History.Add(text);
			m_CurCommand = m_History.GetSize();
			int len = GetWindowTextLength();
			GotoPos(len);
			text += "\n";
			Print(text);
			m_CurPos = -1;
			m_CarryNewLine = true;
			bHandled = FALSE;
			return 1;
		}
		default:
			bHandled = FALSE;
		}
		return 1;
	}

	LRESULT OnPaste(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) {
		if (OpenClipboard()) {
			// LPCTSTR pc = (LPCTSTR)GetClipboardData(CF_UNICODETEXT);
			LPCTSTR pc = (LPCTSTR)GetClipboardData(CF_TEXT);
			if (pc) {
				CString str(pc);
				if (m_CurPos == -1) m_CurPos = GetCurrentPos();
				for (int i = 0; i < str.GetLength(); i++) {
					SendMessage(WM_CHAR, (WPARAM)str[i], 0);
				}
			}
			CloseClipboard();
		}
		return 1;
	}

	int GetWindowTextLength() {
		return GetLength();
	}

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////
	~CLispIDEPipe()
	{
		Close();
	}

	//////////////////////////////////////////////////////////////////////
	// CRedirector implementation
	//////////////////////////////////////////////////////////////////////
	BOOL Open(LPCTSTR pszCmdLine)
	{
		HANDLE hStdoutReadTmp;				// parent stdout read handle
		HANDLE hStdoutWrite, hStderrWrite;	// child stdout write handle
		HANDLE hStdinWriteTmp;				// parent stdin write handle
		HANDLE hStdinRead;					// child stdin read handle
		SECURITY_ATTRIBUTES sa;
		Close();
		hStdoutReadTmp = NULL;
		hStdoutWrite = hStderrWrite = NULL;
		hStdinWriteTmp = NULL;
		hStdinRead = NULL;
		// Set up the security attributes struct.
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		BOOL bOK = FALSE;
		__try
		{
			// Create a child stdout pipe.
			if (!::CreatePipe(&hStdoutReadTmp, &hStdoutWrite, &sa, 0)) {
				__leave;
			}
			// Create a duplicate of the stdout write handle for the std
			// error write handle. This is necessary in case the child
			// application closes one of its std output handles.
			if (!::DuplicateHandle(
				::GetCurrentProcess(),
				hStdoutWrite,
				::GetCurrentProcess(),
				&hStderrWrite,
				0, TRUE,
				DUPLICATE_SAME_ACCESS)) {
				__leave;
			}
			// Create a child stdin pipe.
			if (!::CreatePipe(&hStdinRead, &hStdinWriteTmp, &sa, 0)) {
				__leave;
			}
			// Create new stdout read handle and the stdin write handle.
			// Set the inheritance properties to FALSE. Otherwise, the child
			// inherits the these handles; resulting in non-closeable
			// handles to the pipes being created.
			if (!::DuplicateHandle(
				::GetCurrentProcess(),
				hStdoutReadTmp,
				::GetCurrentProcess(),
				&m_hStdoutRead,
				0, FALSE,			// make it uninheritable.
				DUPLICATE_SAME_ACCESS)) {
				__leave;
			}
			if (!::DuplicateHandle(
				::GetCurrentProcess(),
				hStdinWriteTmp,
				::GetCurrentProcess(),
				&m_hStdinWrite,
				0, FALSE,			// make it uninheritable.
				DUPLICATE_SAME_ACCESS)) {
				__leave;
			}

			// Close inheritable copies of the handles we do not want to
			// be inherited.
			DestroyHandle(hStdoutReadTmp);
			DestroyHandle(hStdinWriteTmp);
			// launch the child process
			if (!LaunchChild(pszCmdLine, hStdoutWrite, hStdinRead, hStderrWrite)) {
				__leave;
			}
			// Child is launched. Close the parents copy of those pipe
			// handles that only the child should have open.
			// Make sure that no handles to the write end of the stdout pipe
			// are maintained in this process or else the pipe will not
			// close when the child process exits and ReadFile will hang.
			DestroyHandle(hStdoutWrite);
			DestroyHandle(hStdinRead);
			DestroyHandle(hStderrWrite);
			// Launch a thread to receive output from the child process.
			m_hEvtStop = ::CreateEvent(NULL, TRUE, FALSE, NULL);
			m_hOutputThread = ::CreateThread(
				NULL, 0,
				OutputThread,
				this,
				0,
				&m_dwThreadId);
			if (!m_hOutputThread) {
				__leave;
			}
			bOK = TRUE;
		}

		__finally
		{
			if (!bOK)
			{
				DWORD dwOsErr = ::GetLastError();
				TCHAR szMsg[40];
				::_stprintf(szMsg, "Redirect console error: %x\r\n", dwOsErr);
				WriteStdError(szMsg);
				DestroyHandle(hStdoutReadTmp);
				DestroyHandle(hStdoutWrite);
				DestroyHandle(hStderrWrite);
				DestroyHandle(hStdinWriteTmp);
				DestroyHandle(hStdinRead);
				Close();
				::SetLastError(dwOsErr);
			}
		}
		return bOK;
	}

	BOOL TerminateProcessTree(DWORD myprocID, HANDLE hProcessSnap)
	{
		BOOL bRet = TRUE;
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (::Process32First(hProcessSnap, &pe32)) {
			std::vector<UINT> children;
			do {
				if (pe32.th32ParentProcessID == myprocID)
				{
					ATLTRACE("going to terminate: %s\n", pe32.szExeFile);
					children.push_back(pe32.th32ProcessID);
				}
			} while (::Process32Next(hProcessSnap, &pe32));
			for (UINT i = 0; i < children.size(); i++) {
				TerminateProcessTree(children[i], hProcessSnap);
			}
			// kill the main process 
			HANDLE hProc = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, myprocID);
			if (hProc)
			{
				::TerminateProcess(hProc, -2);
				::CloseHandle(hProc);
			}
		}
		return bRet;
	}

	void Close()
	{
		HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		DestroyHandle(m_hStdinWrite);
		DestroyHandle(m_hStdoutRead);
		if (m_hOutputThread != NULL)
		{
			// this function might be called from redir thread
			if (::GetCurrentThreadId() != m_dwThreadId)
			{
				ATLASSERT(m_hEvtStop != NULL);
				::SetEvent(m_hEvtStop);
				if (::WaitForSingleObject(m_hOutputThread, 5000) == WAIT_TIMEOUT)
				{
					::TerminateThread(m_hOutputThread, -2);
				}
			}
			DestroyHandle(m_hOutputThread);
		}
		if (m_hChildProcessId) {
			TerminateProcessTree(m_hChildProcessId, hProcessSnap);
			m_hChildThreadId = 0;
			m_hChildProcessId = 0;
		}
		DestroyHandle(m_hEvtStop);
		DestroyHandle(m_hChildProcess);
		DestroyHandle(m_hChildThread);
		DestroyHandle(hProcessSnap);
		m_dwThreadId = 0;
	}


	BOOL Print(CString &pszFormat)
	{
		if (!m_hStdinWrite) {
			return FALSE;
		}
		if (this->m_newLISPMode) {
			pszFormat = "[cmd]\n" + pszFormat + "[/cmd]\n";
		}
		DWORD dwWritten;
		return ::WriteFile(m_hStdinWrite, pszFormat,
			pszFormat.GetLength(), &dwWritten, NULL);
	}

	BOOL LaunchChild(LPCTSTR pszCmdLine,
		HANDLE hStdOut,
		HANDLE hStdIn,
		HANDLE hStdErr)
	{
		// set path to lisp exec
		CString commandline = pszCmdLine;
		commandline.Delete(commandline.ReverseFind('\\'), MAX_PATH);
		::SetCurrentDirectory(commandline);
		PROCESS_INFORMATION pi;
		STARTUPINFO si;
		//	ATLASSERT(::AfxIsValidString(pszCmdLine));
		ATLASSERT(m_hChildProcess == NULL);
		// Set up the start up info struct.
		::ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.hStdOutput = hStdOut;
		si.hStdInput = hStdIn;
		si.hStdError = hStdErr;
		si.dwFlags = STARTF_USESTDHANDLES;
		// Note that dwFlags must include STARTF_USESHOWWINDOW if we
		// use the wShowWindow flags. This also assumes that the
		// CreateProcess() call will use CREATE_NEW_CONSOLE.
		// Launch the child process.
		if (!::CreateProcess(
			NULL,
			(LPTSTR)pszCmdLine,
			NULL, NULL,
			TRUE,
			//			0,
			CREATE_NO_WINDOW,
			NULL, NULL,
			&si,
			&pi)) {
			return FALSE;
		}
		m_hChildProcess = pi.hProcess;
		m_hChildProcessId = pi.dwProcessId;
		// Close any unuseful handles
		//	m_hChildThread = pi.hThread;
		m_hChildThreadId = pi.dwThreadId;
		::CloseHandle(pi.hThread);
		return TRUE;
	}

	// redirect the child process's stdout:
	// return: 1: no more data, 0: child terminated, -1: os error
	int RedirectStdout()
	{
		ATLASSERT(m_hStdoutRead != NULL);
		for (;;)
		{
			DWORD dwAvail = 0;
			if (!::PeekNamedPipe(m_hStdoutRead, NULL, 0, NULL,
				&dwAvail, NULL)) {			// error
				break;
			}
			if (!dwAvail) {					// not data available
				return 1;
			}
			char *szOutput = new char[dwAvail + 1];
			DWORD dwRead = 0;
			if (!::ReadFile(m_hStdoutRead, szOutput, dwAvail,
				&dwRead, NULL) || !dwRead) {	// error, the child might ended
				break;
			}
			szOutput[dwRead] = 0;
			WriteStdOut(szOutput);
			delete szOutput;
		}
		DWORD dwError = ::GetLastError();
		if (dwError == ERROR_BROKEN_PIPE ||	// pipe has been ended
			dwError == ERROR_NO_DATA)		// pipe closing in progress
		{
#ifdef _TEST_REDIR
			WriteStdOut("\r\n<ERROR>: Child process ended\r\n");
#endif
			return 0;	// child process ended
		}
		WriteStdError("Read stdout pipe error\r\n");
		return -1;		// os error
	}

	void DestroyHandle(HANDLE& rhObject)
	{
		if (rhObject != NULL)
		{
			::CloseHandle(rhObject);
			rhObject = NULL;
		}
	}

	void WriteStdOut(TCHAR *pszOutput)
	{
		CString text(pszOutput);
		if (m_CarryNewLine && text[0] == '\n') {
			m_CarryNewLine = false;
			text.Delete(0);
		}
		CScintillaCtrl::AppendText(text.GetLength(), text);
		GotoPos(GetWindowTextLength());
	}

	void WriteStdError(TCHAR *pszError)
	{
		WriteStdOut(pszError);
	}

private:
	HANDLE m_hOutputThread;		// thread to receive the output of the child process
	HANDLE m_hChildThread;
	HANDLE m_hEvtStop;		// event to notify the redir thread to exit
	DWORD m_dwThreadId;		// id of the redir thread
	DWORD m_dwWaitTime;		// wait time to check the status of the child process

protected:
	HANDLE m_hStdinWrite;	// write end of child's stdin pipe
	HANDLE m_hStdoutRead;	// read end of child's stdout pipe
	HANDLE m_hChildProcess;

	// thread to receive output of the child process
	static DWORD WINAPI OutputThread(LPVOID lpvThreadParam)
	{
		HANDLE aHandles[2];
		int nRet;
		CLispIDEPipe* pRedir = (CLispIDEPipe*)lpvThreadParam;
		ATLASSERT(pRedir != NULL);
		aHandles[0] = pRedir->m_hChildProcess;
		aHandles[1] = pRedir->m_hEvtStop;
		for (;;)
		{
			// redirect stdout till there's no more data.
			nRet = pRedir->RedirectStdout();
			if (nRet <= 0)
				break;

			// check if the child process has terminated.
			DWORD dwRc = ::WaitForMultipleObjects(
				2, aHandles, FALSE, pRedir->m_dwWaitTime);
			if (WAIT_OBJECT_0 == dwRc)		// the child process ended
			{
				nRet = pRedir->RedirectStdout();
				if (nRet > 0)
					nRet = 0;
				break;
			}
			if (WAIT_OBJECT_0 + 1 == dwRc)	// m_hEvtStop was signalled
			{
				nRet = 1;	// cancelled
				break;
			}
		}
		return nRet;
	}

};

