// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Include the ATL headers needed:
#include <atlimage.h> //for CImage
//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#define ARRSIZE(x)	(sizeof(x)/sizeof(x[0]))

static char *Unicode2MB(CString& sinput)
{
	LPTSTR lpszInput=sinput.GetBuffer( sinput.GetLength() );
	int nLen=WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)lpszInput, -1, NULL, NULL, NULL, NULL);
	char *snuser=new char[nLen*2];
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)lpszInput, -1, snuser, nLen, NULL, NULL);
	sinput.ReleaseBuffer();
	return snuser;
}

static LPCTSTR MB2Unicode(char *input)
{
	int nLen=MultiByteToWideChar(CP_ACP, 0, input, -1, NULL, NULL);
	LPWSTR lpwStr=new WCHAR[nLen*2];
	MultiByteToWideChar(CP_ACP, 0, input, -1, lpwStr, nLen);
	return (LPCTSTR)lpwStr;
}