// iPodWizard.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "UpdateCheck.h"

// CiPodWizardApp:
// See iPodWizard.cpp for the implementation of this class
//

//iPod Detection
//nano:
//#define BLOCK_SIZE 2048
//#define FIRMWARE_START 129024
//regular:
#define OBLOCK_SIZE 512
#define OFIRMWARE_START 32256
#define OPARTITION_MAP_ADDRESS 0x4200
#define SECTOR_SIZE_PTR 11 //0x0b
#define RELATIVE_SECTOR 454 //0x1c6
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <winioctl.h>

//Firmware
#define FIRMWARE_RESOURCE_TYPE	TEXT("FIRM")

//static char nand[]={"DNAN"};
//static char ata[]={"!ATA"};
static char nand[4]={0x44,0x4E,0x41,0x4E};
static char ata[4]={0x21,0x41,0x54,0x41};

//iPod Detector and handler:
#import "progid:IPodService.iPodManager.1" no_namespace, named_guids, raw_interfaces_only, raw_native_types, no_smart_pointers
//[ module(name="iPodEventReceiver") ];, embedded_idl

//File paths definitions:
#define MO_LOAD_FIRMWARE_PATH(dlg) CString temppath; \
	temppath=theApp.GetProfileString(TEXT("FilePaths"), TEXT("Firmware")); \
	dlg.m_pOFN->lpstrInitialDir=temppath;
#define MO_LOAD_RESOURCES_PATH(dlg) CString temppath; \
	temppath=theApp.GetProfileString(TEXT("FilePaths"), TEXT("Resources")); \
	dlg.m_pOFN->lpstrInitialDir=temppath;
#define MO_SAVE_FIRMWARE_PATH(dlg) theApp.WriteProfileString(TEXT("FilePaths"), TEXT("Firmware"), dlg.GetPathName().Left(dlg.GetPathName().ReverseFind('\\')));
#define MO_SAVE_RESOURCES_PATH(dlg) theApp.WriteProfileString(TEXT("FilePaths"), TEXT("Resources"), dlg.GetPathName().Left(dlg.GetPathName().ReverseFind('\\')));

static BOOL WildMatch(CString sWild, CString sString, CString sLimitChar)
{
    BOOL bAny = FALSE;
    BOOL bNextIsOptional = FALSE;
    BOOL bAutorizedChar = TRUE;

    int i=0;
    int j=0;

    // Check all the string char by char
    while (i<sString.GetLength()) 
    {
      // Check index for array overflow
      if (j<sWild.GetLength())
      {
          // Manage '*' in the wildcard
          if (sWild[j]=='*') 
          {
          // Go to next character in the wildcard
          j++;

          // Enf of the string and wildcard end 
          // with *, only test string validity
          if (j>=sWild.GetLength()) 
          {
          // Check end of the string
          while (!sLimitChar.IsEmpty() && i<sString.GetLength()) 
          {
          // If this char is not ok, return false
          if (sLimitChar.Find(sString[i])<0)
              return FALSE;

          i++;
          }

          return TRUE;
          }

          bAny = TRUE;
          bNextIsOptional = FALSE;
          } 
          else 
          {
              // Optional char in the wildcard
              if (sWild[j]=='^')
              {
              // Go to next char in the wildcard and indicate 
              // that the next is optional
              j++;
 
              bNextIsOptional = TRUE;
              }
              else
              {
                bAutorizedChar = 
                  ((sLimitChar.IsEmpty()) || (sLimitChar.Find(sString[i])>=0));

                // IF :
                  if (// Current char match the wildcard
                    sWild[j] == sString[i] 
                    // '?' is used and current char is in autorized char list
                    || (sWild[j] == '?' && bAutorizedChar)
                    // Char is optional and it's not in the string
                    // and it's necessary to test if '*' make any 
                    // char browsing
                    || (bNextIsOptional && !(bAny && bAutorizedChar))) 
                    {
                    // If current char match wildcard, 
                    // we stop for any char browsing
                    if (sWild[j] == sString[i])
                        bAny = FALSE;

                    // If it's not an optional char who is not present,
                    // go to next
                    if (sWild[j] == sString[i] || sWild[j] == '?')
                        i++;

                    j++;

                    bNextIsOptional = FALSE;
                    } 
                    else
                    // If we are in any char browsing ('*') 
                    // and curent char is autorized
                    if (bAny && bAutorizedChar)
                        // Go to next
                        i++;
                    else
                        return FALSE;
               }
            }
        }
        else
        // End of the wildcard but not the 
        // end of the string => 
        // not matching
        return FALSE;
    }

    if (j<sWild.GetLength() && sWild[j]=='^')
    {
        bNextIsOptional = TRUE;
        j++;
    }


    // If the string is shorter than wildcard 
    // we test end of the 
    // wildcard to check matching
    while ((j<sWild.GetLength() && sWild[j]=='*') || bNextIsOptional)
    {
        j++;
        bNextIsOptional = FALSE;

        if (j<sWild.GetLength() && sWild[j]=='^')
        {
            bNextIsOptional = TRUE;
            j++;
        }
    }

    return j>=sWild.GetLength();
}

/////////////////////////////////////////////////////////////////////////////////////////
// Utf8Decode - converts UTF8-encoded string to the UCS2/MBCS format
static void Utf8Decode( char* str, wchar_t** ucs2 )
{
	if ( str == NULL )
		return;

	int len = (int)strlen( str );
	if ( len < 2 ) {
		if ( ucs2 != NULL ) {
			*ucs2 = ( wchar_t* )malloc(( len+1 )*sizeof( wchar_t ));
			MultiByteToWideChar( CP_ACP, 0, str, len, *ucs2, len );
			( *ucs2 )[ len ] = 0;
		}
		return;
	}

	wchar_t* tempBuf = ( wchar_t* )alloca(( len+1 )*sizeof( wchar_t ));
	{
		wchar_t* d = tempBuf;
		BYTE* s = ( BYTE* )str;

		while( *s )
		{
			if (( *s & 0x80 ) == 0 ) {
				*d++ = *s++;
				continue;
			}

			if (( s[0] & 0xE0 ) == 0xE0 && ( s[1] & 0xC0 ) == 0x80 && ( s[2] & 0xC0 ) == 0x80 ) {
				*d++ = (( WORD )( s[0] & 0x0F) << 12 ) + ( WORD )(( s[1] & 0x3F ) << 6 ) + ( WORD )( s[2] & 0x3F );
				s += 3;
				continue;
			}

			if (( s[0] & 0xE0 ) == 0xC0 && ( s[1] & 0xC0 ) == 0x80 ) {
				*d++ = ( WORD )(( s[0] & 0x1F ) << 6 ) + ( WORD )( s[1] & 0x3F );
				s += 2;
				continue;
			}

			*d++ = *s++;
		}

		*d = 0;
	}

	if ( ucs2 != NULL ) {
		int fullLen = ( len+1 )*sizeof( wchar_t );
		*ucs2 = ( wchar_t* )malloc( fullLen );
		memcpy( *ucs2, tempBuf, fullLen );
	}

   WideCharToMultiByte( CP_ACP, 0, tempBuf, -1, str, len, NULL, NULL );
}

static bool GetUserColor(unsigned int base_color, unsigned int *ret_color, HWND hWnd)
{
	CHOOSECOLOR cc;
	static COLORREF acrCustClr[16];
	DWORD rgbCurrent;
	rgbCurrent=base_color;
	ZeroMemory(&cc, sizeof(cc));
	cc.Flags = CC_FULLOPEN | CC_RGBINIT | CC_SOLIDCOLOR;
	cc.hwndOwner = hWnd;
	cc.rgbResult = rgbCurrent;
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.lStructSize = sizeof(cc);
	if (ChooseColor(&cc)==TRUE)
	{
		*ret_color=cc.rgbResult;
		return true;
	}
	else
		return false;
}

#define WUNKNOWN	0

#define W9XFIRST	1
#define W95			1
#define W95SP1		2
#define W95OSR2		3
#define W98			4
#define W98SP1		5
#define W98SE		6
#define WME			7
#define W9XLAST		99

#define WNTFIRST	101
#define WNT351		101
#define WNT4		102
#define W2K			103
#define WXP			104
#define W2003		105
#define WVISTA		106
#define WNTLAST		199

#define WCEFIRST	201
#define WCE			201
#define WCELAST		299

#ifndef VER_PLATFORM_WIN32_CE
#define VER_PLATFORM_WIN32_CE           3
#endif

struct OSVERSIONINFOEXX
	{  
		DWORD dwOSVersionInfoSize;  
		DWORD dwMajorVersion;  
		DWORD dwMinorVersion;  
		DWORD dwBuildNumber;  
		DWORD dwPlatformId;  
		TCHAR szCSDVersion[128];  
		WORD  wServicePackMajor;  
		WORD  wServicePackMinor;  
		WORD  wSuiteMask;  
		BYTE  wProductType;  
		BYTE  wReserved;
	};

static int GetWinVersion()
{
	OSVERSIONINFOEXX m_osinfo;

	ZeroMemory(&m_osinfo, sizeof(m_osinfo));

	m_osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if (GetVersionEx((LPOSVERSIONINFO) &m_osinfo))
	{

		if ((m_osinfo.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
			(m_osinfo.dwMajorVersion >= 5))
		{
			// get extended version info for 2000 and later

			ZeroMemory(&m_osinfo, sizeof(m_osinfo));

			m_osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

			GetVersionEx((LPOSVERSIONINFO) &m_osinfo);
		}
	}

	int nVersion = WUNKNOWN;

	DWORD dwPlatformId   = m_osinfo.dwPlatformId;
	DWORD dwMinorVersion = m_osinfo.dwMinorVersion;
	DWORD dwMajorVersion = m_osinfo.dwMajorVersion;
	DWORD dwBuildNumber  = m_osinfo.dwBuildNumber & 0xFFFF;	// Win 9x needs this

	if ((dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (dwMajorVersion == 4))
	{
		if ((dwMinorVersion < 10) && (dwBuildNumber == 950))
		{
			nVersion = W95;
		}
		else if ((dwMinorVersion < 10) &&
				((dwBuildNumber > 950) && (dwBuildNumber <= 1080)))
		{
			nVersion = W95SP1;
		}
		else if ((dwMinorVersion < 10) && (dwBuildNumber > 1080))
		{
			nVersion = W95OSR2;
		}
		else if ((dwMinorVersion == 10) && (dwBuildNumber == 1998))
		{
			nVersion = W98;
		}
		else if ((dwMinorVersion == 10) &&
				((dwBuildNumber > 1998) && (dwBuildNumber < 2183)))
		{
			nVersion = W98SP1;
		}
		else if ((dwMinorVersion == 10) && (dwBuildNumber >= 2183))
		{
			nVersion = W98SE;
		}
		else if (dwMinorVersion == 90)
		{
			nVersion = WME;
		}
	}
	else if (dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if ((dwMajorVersion == 3) && (dwMinorVersion == 51))
		{
			nVersion = WNT351;
		}
		else if ((dwMajorVersion == 4) && (dwMinorVersion == 0))
		{
			nVersion = WNT4;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 0))
		{
			nVersion = W2K;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 1))
		{
			nVersion = WXP;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 2))
		{
			nVersion = W2003;
		}
		else if ((dwMajorVersion == 6) && (dwMinorVersion == 0))
		{
			nVersion = WVISTA;
		}
	}
	else if (dwPlatformId == VER_PLATFORM_WIN32_CE)
	{
		nVersion = WCE;
	}

	return nVersion;
}

static void RecursiveDelete(CString szPath)
{
	CFileFind ff;
	CString path = szPath;
	
	if(path.Right(1) != "\\")
		path += "\\";

	path += "*.*";

	BOOL res = ff.FindFile(path);

	while(res)
	{
		res = ff.FindNextFile();
		if (!ff.IsDots() && !ff.IsDirectory())
			DeleteFile(ff.GetFilePath());
		else if (ff.IsDirectory() && !ff.IsDots())
		{
			path = ff.GetFilePath();
			RecursiveDelete(path);
			RemoveDirectory(path);
		}
	}
}

static int _pdread(int _FileHandle, void *DstBuf, unsigned int _MaxCountChar, int BLK_SIZE)
{
	//unsigned int mark=0x03ff7ffe; //+1
	unsigned int mark=50000000;
	unsigned int j;
	for (j=mark;j%BLK_SIZE!=0;j--);
	mark=j;
	if (_MaxCountChar<=mark)
		return _read(_FileHandle, DstBuf, _MaxCountChar);
	else
	{
		unsigned int i,num_reads;
		unsigned long pointer=0;
		num_reads=_MaxCountChar/mark+1;
		unsigned int diff;
		diff=_MaxCountChar%mark;
		int sizeret=0;
		int totalsizeret=0;
		for (i=0;i<num_reads;i++)
		{
			if (i==num_reads-1)
			{
				sizeret=_read(_FileHandle, (void *)((unsigned char *)DstBuf+pointer), diff);
				if (sizeret!=diff)
					return sizeret;
				else
					totalsizeret+=sizeret;
			}
			else
			{
				sizeret=_read(_FileHandle, (void *)((unsigned char *)DstBuf+pointer), mark);
				if (sizeret!=mark)
					return sizeret;
				else
					totalsizeret+=sizeret;
				pointer+=sizeret;
			}
		}
		return totalsizeret;
	}
}

static int _pdwrite(int _FileHandle, const void *_Buf, unsigned int _MaxCountChar, int BLK_SIZE)
{
	//unsigned int mark=0x03ff7ffe; //+1
	unsigned int mark=50000000;
	unsigned int j;
	for (j=mark;j%BLK_SIZE!=0;j--);
	mark=j;
	if (_MaxCountChar<=mark)
		return _write(_FileHandle, _Buf, _MaxCountChar);
	else
	{
		unsigned int i,num_writes;
		unsigned long pointer=0;
		num_writes=_MaxCountChar/mark+1;
		unsigned int diff;
		diff=_MaxCountChar%mark;
		int sizeret=0;
		int totalsizeret=0;
		for (i=0;i<num_writes;i++)
		{
			if (i==num_writes-1)
			{
				sizeret=_write(_FileHandle, (void *)((unsigned char *)_Buf+pointer), diff);
				if (sizeret!=diff)
					return sizeret;
				else
					totalsizeret+=sizeret;
			}
			else
			{
				sizeret=_write(_FileHandle, (void *)((unsigned char *)_Buf+pointer), mark);
				if (sizeret!=mark)
					return sizeret;
				else
					totalsizeret+=sizeret;
				pointer+=sizeret;
			}
		}
		return totalsizeret;
	}
}

#define APPLE_WARN "{{~~  /-----\\   {{~~ /       \\  {{~~|         | {{~~| S T O P | {{~~|         | {{~~ \\       /  {{~~  \\-----/   Copyright(C) 2001 Apple Computer, Inc.---------------------------------------------------------------------------------------------------------"

class CiPodWizardApp : public CWinApp
{
public:
	CiPodWizardApp();

// Overrides
	public:
	virtual BOOL InitInstance();
	//Extra stuff:
	CString m_iPodDRV;
	CString m_IniPath;
	CString m_DeviceSel;
	BOOL m_LoadingFirmware;
	BOOL m_SavingFirmware;
	void InitSectorSize(TCHAR *devstring, bool quiet = false);
	void InitHDDValues(int fd);
	void CalcPartMapAddr(unsigned char *buf);
	int BLOCK_SIZE;
	long FIRMWARE_START;
	long PARTITION_MAP_ADDRESS;
	BOOL m_BootPicChanged;
	//Preferences
	BOOL m_OldPicIndex;
	BOOL m_OldFontMetricsSystem;
	BOOL m_ReflashBootImage;
	BOOL m_OTFRender;
	BOOL m_LayoutRender;
	BOOL m_AllStringsRender;
	BOOL m_CheckUpdates;
	void Check4Updates();
	void LoadPreferences();

	BOOL m_bAdminPrivilege;
	TCHAR *pszName;
	TCHAR *pszPwd;
	void RunAsAdmin();
	void EndRunAsAdmin();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CiPodWizardApp theApp;
