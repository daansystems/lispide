// appsettings.h
//
// Utility class for storing/reading profile values from the Registry
// (C) 2000 Peter Kenyon
//
// You may freely distribute this source code, providing that you do not charge for it
// and that this copyright notice remains intact.
//
// Please report any bugs to Peter@bizinf.co.nz
//
// Edited for VC 7.1 to avoid ATL_DEPRECATED warnings
//

#ifndef _APPSETTINGS_H
#define _APPSETTINGS_H

class CAppSettings
{
public:
	// Must be called once to set the name of the key under HKLM\\Software
	// or HKCU\\Software.
	// Normally this will be of the form "[company name]\\[app name]"
	void SetRegKey(LPCTSTR pszKey)
	{
		m_strRegKeyName = pszKey;
	}
	CString GetUserProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL)
	{
		CString strKey = CatKeyAndSubkeyNames(lpszSection);
		CRegKey key;

		if (key.Open(HKEY_CURRENT_USER, strKey, KEY_READ) == ERROR_SUCCESS)
		{
			DWORD dwcbNeeded = 0;
			if (key.QueryValue(lpszEntry, NULL, NULL, &dwcbNeeded) == ERROR_SUCCESS)
			{
				CString strVal;
				LPTSTR pszVal = strVal.GetBuffer(dwcbNeeded);
				if (key.QueryStringValue(lpszEntry, pszVal, &dwcbNeeded) == ERROR_SUCCESS) {
					strVal.ReleaseBuffer();
					key.Close();
					return strVal;
				}
				else {
					strVal.ReleaseBuffer(0);
				}
			}
		}
		key.Close();
		return CString(lpszDefault);
	}

	CString GetMachineProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL)
	{
		CString strKey = CatKeyAndSubkeyNames(lpszSection);
		CRegKey key;

		if (key.Open(HKEY_LOCAL_MACHINE, strKey, KEY_READ) == ERROR_SUCCESS)
		{
			DWORD dwcbNeeded = 0;
			if (key.QueryValue(lpszEntry, NULL, NULL, &dwcbNeeded) == ERROR_SUCCESS)
			{
				CString strVal;
				LPTSTR pszVal = strVal.GetBuffer(dwcbNeeded);

				if (key.QueryStringValue(lpszEntry, pszVal, &dwcbNeeded) == ERROR_SUCCESS)
				{
					strVal.ReleaseBuffer();
					key.Close();
					return strVal;
				}
				else
				{
					strVal.ReleaseBuffer(0);
				}
			}
		}
		key.Close();
		return CString(lpszDefault);
	}

	DWORD GetUserProfileDword(LPCTSTR lpszSection, LPCTSTR lpszEntry, DWORD dwDefault = 0)
	{
		CString strKey = CatKeyAndSubkeyNames(lpszSection);
		CRegKey key;
		if (key.Open(HKEY_CURRENT_USER, strKey, KEY_READ) == ERROR_SUCCESS)
		{
			DWORD dwVal;
			if (key.QueryDWORDValue(lpszEntry, dwVal) == ERROR_SUCCESS)
			{
				key.Close();
				return dwVal;
			}
		}
		key.Close();
		return dwDefault;;
	}

	DWORD GetMachineProfileDword(LPCTSTR lpszSection, LPCTSTR lpszEntry, DWORD dwDefault = 0)
	{
		CString strKey = CatKeyAndSubkeyNames(lpszSection);
		CRegKey key;
		if (key.Open(HKEY_LOCAL_MACHINE, strKey, KEY_READ) == ERROR_SUCCESS)
		{
			DWORD dwVal;
			if (key.QueryDWORDValue(lpszEntry, dwVal) == ERROR_SUCCESS)
			{
				key.Close();
				return dwVal;
			}
		}
		key.Close();
		return dwDefault;;
	}

	BOOL WriteMachineProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszVal)
	{
		CString strKey = CatKeyAndSubkeyNames(lpszSection);
		CRegKey key;
		if (key.Create(HKEY_LOCAL_MACHINE, strKey, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE) == ERROR_SUCCESS)
		{
			if (key.SetStringValue(lpszEntry, lpszVal) == ERROR_SUCCESS)
			{
				key.Close();
				return TRUE;
			}
		}
		key.Close();
		return FALSE;
	}

	BOOL WriteMachineProfileDword(LPCTSTR lpszSection, LPCTSTR lpszEntry, DWORD dwVal)
	{
		CString strKey = CatKeyAndSubkeyNames(lpszSection);
		CRegKey key;
		if (key.Create(HKEY_LOCAL_MACHINE, strKey, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE) == ERROR_SUCCESS)
		{
			if (key.SetDWORDValue(lpszEntry, dwVal) == ERROR_SUCCESS)
			{
				key.Close();
				return TRUE;
			}
		}
		key.Close();
		return FALSE;
	}

	BOOL WriteUserProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszVal)
	{
		CString strKey = CatKeyAndSubkeyNames(lpszSection);
		CRegKey key;
		if (key.Create(HKEY_CURRENT_USER, strKey, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE) == ERROR_SUCCESS)
		{
			if (key.SetStringValue(lpszEntry, lpszVal) == ERROR_SUCCESS)
			{
				key.Close();
				return TRUE;
			}
		}
		key.Close();
		return FALSE;
	}

	BOOL WriteUserProfileDword(LPCTSTR lpszSection, LPCTSTR lpszEntry, DWORD dwVal)
	{
		CString strKey = CatKeyAndSubkeyNames(lpszSection);
		CRegKey key;
		if (key.Create(HKEY_CURRENT_USER, strKey, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE) == ERROR_SUCCESS)
		{
			if (key.SetDWORDValue(lpszEntry, dwVal) == ERROR_SUCCESS)
			{
				key.Close();
				return TRUE;
			}
		}
		key.Close();
		return FALSE;
	}

private:
	CString CatKeyAndSubkeyNames(LPCTSTR lpszSection)
	{
		ATLASSERT(!m_strRegKeyName.IsEmpty());
		CString strKey = m_strRegKeyName;

		if (lpszSection)
		{
			strKey += _T("\\");
			strKey += lpszSection;
		}
		return strKey;
	}
	CString m_strRegKeyName;
};

#endif // _APPSETTINGS_H