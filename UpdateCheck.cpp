// UpdateCheck.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UpdateCheck.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUpdateCheck::CUpdateCheck(BOOL bNotifyNone)
{
	m_bNotifyNoMsg = bNotifyNone;
}

CUpdateCheck::~CUpdateCheck()
{
}

BOOL CUpdateCheck::GetFileVersion(DWORD &dwMS, DWORD &dwLS)
{
	TCHAR szModuleFileName[MAX_PATH];

    LPBYTE  lpVersionData; 

	if (GetModuleFileName(AfxGetInstanceHandle(), szModuleFileName, sizeof(szModuleFileName)) == 0) return FALSE;

    DWORD dwHandle;     
    DWORD dwDataSize = ::GetFileVersionInfoSize(szModuleFileName, &dwHandle); 
    if ( dwDataSize == 0 ) 
        return FALSE;

    lpVersionData = new BYTE[dwDataSize]; 
    if (!::GetFileVersionInfo(szModuleFileName, dwHandle, dwDataSize, (void**)lpVersionData) )
    {
		delete [] lpVersionData;
        return FALSE;
    }

    ASSERT(lpVersionData != NULL);

    UINT nQuerySize;
	VS_FIXEDFILEINFO* pVsffi;
    if ( ::VerQueryValue((void **)lpVersionData, _T("\\"),
                         (void**)&pVsffi, &nQuerySize) )
    {
		dwMS = pVsffi->dwFileVersionMS;
		dwLS = pVsffi->dwFileVersionLS;
		delete [] lpVersionData;
        return TRUE;
    }

	delete [] lpVersionData;
    return FALSE;

}

void CUpdateCheck::Check(UINT uiURL)
{
	CString strURL(MAKEINTRESOURCE(uiURL));
	Check(strURL);
}

void CUpdateCheck::Check(const CString& strURL)
{
	DWORD dwMS, dwLS;
	if (!GetFileVersion(dwMS, dwLS))
	{
		ASSERT(FALSE);
		return;
	}

	CWaitCursor wait;
	HINTERNET hInet = InternetOpen(UPDATECHECK_BROWSER_STRING, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	HINTERNET hUrl = InternetOpenUrl(hInet, strURL, NULL, -1L,
										 INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE |
										 INTERNET_FLAG_NO_CACHE_WRITE |WININET_API_FLAG_ASYNC, NULL);
	if (hUrl)
	{
		char szBuffer[512];
		DWORD dwRead;
		if (InternetReadFile(hUrl, szBuffer, sizeof(szBuffer), &dwRead))
		{
			if (dwRead > 0)
			{
				szBuffer[dwRead] = 0;
				CString strSubMS1;
				CString strSubMS2;
				CString strSubLS1;
				CString strSubLS2;
				CString strSub;
				DWORD dwMSWeb;
				DWORD dwLSWeb;
				TCHAR szBufferW[1026];
				MultiByteToWideChar(CP_ACP, 0, szBuffer, sizeof(szBuffer), szBufferW, sizeof(szBufferW));
				AfxExtractSubString(strSubMS1, szBufferW, 0, '|');
				AfxExtractSubString(strSubMS2, szBufferW, 1, '|');
				AfxExtractSubString(strSubLS1, szBufferW, 3, '|');
				AfxExtractSubString(strSubLS2, szBufferW, 4, '|');
				dwMSWeb = MAKELONG((WORD) _ttol(strSubMS2), (WORD) _ttol(strSubMS1));
				dwLSWeb = MAKELONG((WORD) _ttol(strSubLS2), (WORD) _ttol(strSubLS1));

				WORD major=HIWORD(dwMS);
				WORD minor=LOWORD(dwMS);
				if (dwMSWeb > dwMS)
				{
					AfxExtractSubString(strSub, szBufferW, 2, '|');
					MsgUpdateAvailable(dwMS, dwLS, dwMSWeb, 0, strSub);
				}
				else if (dwMSWeb == dwMS)
				{
					if (dwLSWeb > dwLS)
					{
						AfxExtractSubString(strSub, szBufferW, 2, '|');
						MsgUpdateAvailable(dwMS, dwLS, dwMSWeb, dwLSWeb, strSub);
					}
					else
						MsgUpdateNotAvailable(dwMS, dwLS);
				}
				else
					MsgUpdateNotAvailable(dwMS, dwLS);
			}
			else
				MsgUpdateNoCheck(dwMS, dwLS);

		}
		InternetCloseHandle(hUrl);
	}
	else
		MsgUpdateNoCheck(dwMS, dwLS);

	InternetCloseHandle(hInet);
}

HINSTANCE CUpdateCheck::GotoURL(LPCTSTR url, int showcmd)
{
    TCHAR key[MAX_PATH + MAX_PATH];

    // First try ShellExecute()
    HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);

    // If it failed, get the .htm regkey and lookup the program
    if ((UINT)result <= HINSTANCE_ERROR) 
	{

        if (GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) 
		{
            lstrcat(key, _T("\\shell\\open\\command"));

            if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) 
			{
                TCHAR *pos;
                pos = _tcsstr(key, _T("\"%1\""));
                if (pos == NULL) {                     // No quotes found
                    pos = _tcsstr(key, _T("%1"));      // Check for %1, without quotes 
                    if (pos == NULL)                   // No parameter at all...
                        pos = key+lstrlen(key)-1;
                    else
                        *pos = '\0';                   // Remove the parameter
                }
                else
                    *pos = '\0';                       // Remove the parameter

                lstrcat(pos, _T(" "));
                lstrcat(pos, url);

				char key_a[520];
				WideCharToMultiByte(CP_ACP, 0, key, 520, key_a, 520, 0, 0);
                result = (HINSTANCE) WinExec(key_a,showcmd);
            }
        }
    }

    return result;
}

LONG CUpdateCheck::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS) 
	{
        long datasize = MAX_PATH;
        TCHAR data[MAX_PATH];
        RegQueryValue(hkey, NULL, data, &datasize);
        lstrcpy(retdata,data);
        RegCloseKey(hkey);
    }

    return retval;
}


void CUpdateCheck::MsgUpdateAvailable(DWORD dwMSlocal, DWORD dwLSlocal, DWORD dwMSWeb, DWORD dwLSWeb, const CString& strURL)
{
	CString strMessage;
	strMessage.Format(IDS_UPDATE_AVAILABLE, HIWORD(dwMSlocal), LOWORD(dwMSlocal), HIWORD(dwMSWeb), LOWORD(dwMSWeb), HIWORD(dwLSWeb), LOWORD(dwLSWeb));

	if (AfxMessageBox(strMessage, MB_YESNO|MB_ICONINFORMATION) == IDYES)
		GotoURL(strURL, SW_SHOW);
}

void CUpdateCheck::MsgUpdateNotAvailable(DWORD dwMSlocal, DWORD dwLSlocal)
{
	if (m_bNotifyNoMsg)
		AfxMessageBox(IDS_UPDATE_NO, MB_OK|MB_ICONINFORMATION);
}

void CUpdateCheck::MsgUpdateNoCheck(DWORD dwMSlocal, DWORD dwLSlocal)
{
	AfxMessageBox(IDS_UPDATE_NOCHECK, MB_OK|MB_ICONINFORMATION);
}
