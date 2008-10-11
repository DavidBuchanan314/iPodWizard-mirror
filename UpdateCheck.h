// UpdateCheck.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPDATECHECK_H__6D1C0A60_F616_43C6_9850_F3C0F052C0DB__INCLUDED_)
#define AFX_UPDATECHECK_H__6D1C0A60_F616_43C6_9850_F3C0F052C0DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Wininet.h"

#define UPDATECHECK_BROWSER_STRING _T("Update search")
class CUpdateCheck  
{
public:
	virtual void Check(UINT uiURL);
	virtual void Check(const CString& strURL);
	CUpdateCheck(BOOL bNotifyNone = TRUE);
	virtual ~CUpdateCheck();

	static HINSTANCE GotoURL(LPCTSTR url, int showcmd);
	static BOOL GetFileVersion(DWORD &dwMS, DWORD &dwLS);
	static LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);

protected:
	BOOL m_bNotifyNoMsg;
	virtual void MsgUpdateAvailable(DWORD dwMSlocal, DWORD dwLSlocal, DWORD dwMSWeb, DWORD dwLSWeb, const CString& strURL);
	virtual void MsgUpdateNotAvailable(DWORD dwMSlocal, DWORD dwLSlocal);
	virtual void MsgUpdateNoCheck(DWORD dwMSlocal, DWORD dwLSlocal);
};

#endif // !defined(AFX_UPDATECHECK_H__6D1C0A60_F616_43C6_9850_F3C0F052C0DB__INCLUDED_)
