// LispIDEView.h : interface of the CLispIDEView class
//
////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>

using namespace std;

class CLispIDEView :
	public CWindowImpl<CLispIDEView, CScintillaCtrl>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, "Scintilla")

	CString m_FilePath;
	CFont	m_Font;

	BEGIN_MSG_MAP_EX(CLispIDEView)
		MSG_WM_ERASEBKGND(OnEraseBackGround)
		MSG_WM_CHAR(OnChar)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		NOTIFY_CODE_HANDLER_EX(SCN_UPDATEUI, OnUpdateUI)
		END_MSG_MAP()

	BOOL PreTranslateMessage(MSG* pMsg) {
		pMsg;
		return FALSE;
	}

	LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) {
		GetTopLevelParent().SendMessage(uMsg, wParam, lParam);
		return 1;
	}

	void Init() {
		CScintillaCtrl::Init();
		ClearCmdKey(SCK_RETURN + (SCMOD_NORM << 16));
		ClearCmdKey(SCK_RETURN + (SCMOD_SHIFT << 16));
	}

	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (nChar == VK_RETURN) {
			if (::GetKeyState(VK_SHIFT) < 0 || nFlags & 0x0100) {
				SetMsgHandled(TRUE);
				CString pipetext = GetPipeString();
				if (!pipetext.IsEmpty()) {
					GetTopLevelParent().SendMessage(WM_USER + 2, (WPARAM)&pipetext, 0);
				}
			}
			else {
				SetMsgHandled(FALSE);
				Call(SCI_NEWLINE, 0, 0);
			}
		}
		else {
			SetMsgHandled(FALSE);
		}
	}

	CString GetPipeString() {
		CString text = GetSelText();
		if (text.IsEmpty()) {
			Sci_Position lStart = GetCurrentPos() - 1;
			int chBr = GetCharAt(lStart);
			if (chBr == ')') {
				Sci_Position lEnd = BraceMatch(lStart, 0);
				// if there is a matching brace highlight it
				if (lEnd >= 0) {
					Sci_TextRange tr;
					tr.chrg.cpMin = lEnd;
					tr.chrg.cpMax = lStart + 1;
					int len = tr.chrg.cpMax - tr.chrg.cpMin + 1;
					char *buf = new char[len];
					tr.lpstrText = buf;
					GetTextRange(&tr);
					text = tr.lpstrText;
					delete[] buf;
				}
			}
		}
		return text;
	}

	LRESULT OnEraseBackGround(HDC hdc) {
		return 1;
	}

	/////////////////////////////////////
	// @mfunc Try to load a new file
	// @rvalue BOOL | FALSE on error - TRUE on success
	//
	BOOL LoadFile(LPCSTR szPath) //@parm filename of to load
	{
		// if pathname is empty do nothing
		if (szPath == NULL || *szPath == '\0')
			return TRUE;
		// try to get extension and figure out what lexer to use
		CString strFile(szPath);
		BOOL bReturn = TRUE;
		// ty to open file in sharing mode
		std::ifstream file;
		streamoff len = 0L;
		streamsize nTotal;
		TCHAR *szBuffer = NULL;
		file.open(szPath, ios::in | ios::binary);
		// ok success - try to get length of file
		if (file.is_open())
		{
			file.seekg(0, ios::end);
			len = file.tellg();
			if (len > 0)
			{
				//       alloc new buffer of sie = filesize+1 for termination NULL
				szBuffer = new TCHAR[(UINT)len + 1];
				if (szBuffer != NULL)
				{
					file.seekg(0, ios::beg);
					file.read(szBuffer, len);
					nTotal = file.gcount();
					if (nTotal > 0 && nTotal <= len)
						szBuffer[nTotal] = '\0';
					//          read error
					if (file.bad())
					{
						file.close();
						bReturn = FALSE;
					}
				}
				else
				{
					file.close();
					bReturn = FALSE;
				}
			}
			file.close();
			//    set text to control
			SetText(szBuffer);
			// Respect the users EOL settings
			ConvertEOLs(GetEOLMode());
			//    tell scintilla that we have an unmodified document
			SetSavePoint();
			GotoPos(0);
			EmptyUndoBuffer();
			m_FilePath = szPath;
		}
		// file open error - return
		else {
			bReturn = FALSE;
		}
		// clean up
		if (szBuffer != NULL) {
			delete[] szBuffer;
		}
		return bReturn;
	}
	
	BOOL SaveFile(LPCSTR szPath) //@parm filename to save to
	{
		std::ofstream file;
		file.open(szPath, ios_base::out | ios_base::binary);
		if (file.fail())
		{
			MessageBox("Save failed", "LispIDE", MB_OK | MB_ICONERROR);
			return FALSE;
		}
		int buflen = GetLength() + 1; //last NULL
		CString text = this->GetText(buflen);
		file.write(text, buflen - 1);
		file.close();
		SetSavePoint();
		m_FilePath = szPath;
		return TRUE;
	}
};
