// iPodWizard.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "iPodWizard.h"
#include "iPodWizardDlg.h"
#include <wincred.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CiPodWizardApp

BEGIN_MESSAGE_MAP(CiPodWizardApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CiPodWizardApp construction

CiPodWizardApp::CiPodWizardApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bAdminPrivilege=FALSE;
}


// The one and only CiPodWizardApp object

CiPodWizardApp theApp;


// CiPodWizardApp initialization

BOOL CiPodWizardApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	//In case you use MFC shared DLL, uncomment those
	//HWND p_WindowHandle = CreateWindowEx(0, "STATIC", "IPW_TEMP", 0, 0, 0, 0, 0, 0, 0, m_hInstance, 0);
	//DestroyWindow(p_WindowHandle);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("iPodWizard"));

	LoadPreferences();

	Check4Updates();

	//Check for .NET Framework existance:
	LPWSTR lpw = new WCHAR [MAX_PATH];
	GetSystemDirectory(lpw, MAX_PATH);
	CString sPath(lpw);
	sPath.AppendFormat(TEXT("\\gdi32.dll"));
	if (PathFileExists(sPath)==FALSE)
	{
		if (MessageBox(0, TEXT("Your computer doesn't have .NET Framework v1.0+ installed.\nYou need this piece of software in order for iPodWizard to work.\nPlease download and install it from Windows Update website.\nAlternatively, if you click yes, an Internet Explorer will pop up."), TEXT("Error"), MB_YESNO) == IDYES)
		{
			HINSTANCE result = ShellExecute(NULL, _T("open"), TEXT("http://www.microsoft.com/downloads/details.aspx?familyid=0856eacb-4362-4b0d-8edd-aab15c5e04f5&displaylang=en"), NULL,NULL, SW_SHOW);
		}
	}
	else
	{
		CiPodWizardDlg dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with OK
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with Cancel
		}
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CiPodWizardApp::Check4Updates()
{
	if (theApp.m_CheckUpdates)
	{
		CUpdateCheck checkUpdate(FALSE);
		checkUpdate.Check(IDS_UPDATE);
	}
}

void CiPodWizardApp::InitSectorSize(TCHAR *devstring, bool quiet)
{
	DISK_GEOMETRY gdg;
	DWORD ret;
	CString err_t;
	HANDLE h = CreateFile(devstring, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == (HANDLE)ERROR_INVALID_HANDLE || h==(HANDLE)0xFFFFFFFF)
	{
		if (!quiet)
		{
			ret=GetLastError();
			CString err_t;
			if (ret==5) //access denied, make sure admin rights is enabled
			{
				err_t.Format(_T("Error accessing iPod hard drive parameters!\nIf you're using Windows Vista, make sure you're running using administrative rights."));
				MessageBox(0, err_t, _T("Error"), 0);
				return;
			}
			else
			{
				err_t.Format(_T("Error accessing iPod hard drive parameters!\nCode1: %d\nTrying to load firmware anyway."), ret);
				MessageBox(0, err_t, _T("Error"), 0);
			}
		}
	}
	if (!DeviceIoControl((HANDLE)h, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &gdg, sizeof(gdg), &ret, NULL))
	{
		if (!quiet)
		{
			ret=GetLastError();
			err_t.Format(_T("Error accessing iPod hard drive parameters!\nCode2: %d\nTrying to load firmware anyway."), ret);
			MessageBox(0, err_t, _T("Error"), 0);
			theApp.BLOCK_SIZE = 512;
			//return;
		}
	}
	else
	{
		theApp.BLOCK_SIZE = (int)gdg.BytesPerSector;
	}
	CloseHandle(h);
}

void CiPodWizardApp::InitHDDValues(int fd)
{
	if (fd==-1)
		return;
	
	unsigned char mbr[16384];
	_lseek(fd, 0, SEEK_SET);
	_read(fd, mbr, 16384);
	unsigned char macsig[2]={'E', 'R'};

	if (memcmp(mbr, macsig, 2))
	{
		//memcpy(&BLOCK_SIZE, &mbr[SECTOR_SIZE_PTR], 2);
		DWORD rs;
		memcpy(&rs, &mbr[RELATIVE_SECTOR], 4);
		FIRMWARE_START=rs*BLOCK_SIZE;
		//FIRMWARE_START=78*BLOCK_SIZE;
	}
	else
	{
		FIRMWARE_START=63*BLOCK_SIZE;
	}

	_lseek(fd, FIRMWARE_START, SEEK_SET);
	unsigned char *buf=new unsigned char[BLOCK_SIZE*100];
	_read(fd, buf, BLOCK_SIZE*100);

	int size=sizeof(APPLE_WARN);
	char strwarn[] = APPLE_WARN;
	char *warn=new char[size+2];
	memset(warn, 0, size+1);
	memcpy(warn, buf, size);
	if (strncmp(warn, strwarn, size))
	{
		if (BLOCK_SIZE==512)
			BLOCK_SIZE==2048;
		else
			BLOCK_SIZE=512;
		FIRMWARE_START=63*BLOCK_SIZE;
	}
	
	CalcPartMapAddr(buf);
	delete buf;
}

void CiPodWizardApp::LoadPreferences()
{
	m_OldPicIndex=(BOOL)theApp.GetProfileIntW(_T("Main"), _T("PicIndexSystem"), FALSE);
	m_OldFontMetricsSystem=(BOOL)theApp.GetProfileIntW(_T("Main"), _T("FontMetricSystem"), FALSE);
	m_ReflashBootImage=(BOOL)theApp.GetProfileIntW(_T("Main"), _T("ReflashBootImage"), FALSE);
	m_CheckUpdates=(BOOL)theApp.GetProfileIntW(_T("Main"), _T("CheckForUpdates"), FALSE);
	m_LayoutRender=(BOOL)theApp.GetProfileIntW(_T("Main"), _T("LayoutRender"), FALSE);
	m_OTFRender=(BOOL)theApp.GetProfileIntW(_T("Main"), _T("OTFRender"), FALSE);
	m_AllStringsRender=(BOOL)theApp.GetProfileIntW(_T("Main"), _T("AllStringsRender"), FALSE);
}

void CiPodWizardApp::CalcPartMapAddr(unsigned char *buf)
{
	unsigned short firmver=*((unsigned short *)&buf[266]);
	switch(firmver)
	{
	case 1:
		PARTITION_MAP_ADDRESS=0x4000;
		break;
	case 2:
	case 3:
		long diraddr=*((long *)&buf[260]);
		/*
		PARTITION_MAP_ADDRESS=diraddr;
		int z=0;
		while (memcmp(&buf[PARTITION_MAP_ADDRESS], &ata, 4) != 0 && memcmp(&buf[PARTITION_MAP_ADDRESS], &nand, 4) != 0 && z<50)
		{
			PARTITION_MAP_ADDRESS+=512;
			z++;
		}*/
		//PARTITION_MAP_ADDRESS=diraddr+1024;
		PARTITION_MAP_ADDRESS=diraddr+BLOCK_SIZE;
		if (memcmp(&buf[PARTITION_MAP_ADDRESS], &ata, 4) != 0 && memcmp(&buf[PARTITION_MAP_ADDRESS], &nand, 4) != 0)
		{
			PARTITION_MAP_ADDRESS=diraddr+512;
			while (memcmp(&buf[PARTITION_MAP_ADDRESS], &ata, 4) != 0 && memcmp(&buf[PARTITION_MAP_ADDRESS], &nand, 4) != 0 && PARTITION_MAP_ADDRESS<(diraddr+16384))
				PARTITION_MAP_ADDRESS+=512;
		}

		long pos=PARTITION_MAP_ADDRESS+BLOCK_SIZE;
		long sum=0;
		for (long i=0;i<16;sum+=buf[pos],pos++,i++);
		if (sum==0)
		{
			if (BLOCK_SIZE==512)
				BLOCK_SIZE=2048;
			else
				BLOCK_SIZE=512;
		}
		break;
	}
}

void CiPodWizardApp::RunAsAdmin()
{
	if (m_bAdminPrivilege)
		return;

	CREDUI_INFO cui;
	pszName=new TCHAR[CREDUI_MAX_USERNAME_LENGTH+1];
	pszPwd=new TCHAR[CREDUI_MAX_PASSWORD_LENGTH+1];
	BOOL fSave;
	DWORD dwErr;

	cui.cbSize = sizeof(CREDUI_INFO);
	cui.hwndParent = NULL;
	//  Ensure that MessageText and CaptionText identify what credentials
	//  to use and which application requires them.
	cui.pszMessageText = TEXT("You need to enter administrator account information in order to load firmware off the iPod");
	cui.pszCaptionText = TEXT("iPod Firmware Loading");
	cui.hbmBanner = NULL;
	fSave = FALSE;
	SecureZeroMemory(pszName, sizeof(pszName));
	SecureZeroMemory(pszPwd, sizeof(pszPwd));
	dwErr = CredUIPromptForCredentials( 
	&cui,                         // CREDUI_INFO structure
	TEXT("TheServer"),            // Target for credentials
								  //   (usually a server)
	NULL,                         // Reserved
	0,                            // Reason
	pszName,                      // User name
	CREDUI_MAX_USERNAME_LENGTH+1, // Max number of char for user name
	pszPwd,                       // Password
	CREDUI_MAX_PASSWORD_LENGTH+1, // Max number of char for password
	&fSave,                       // State of save check box
	CREDUI_FLAGS_GENERIC_CREDENTIALS |  // flags
	CREDUI_FLAGS_ALWAYS_SHOW_UI |
	CREDUI_FLAGS_DO_NOT_PERSIST |
	CREDUI_FLAGS_REQUEST_ADMINISTRATOR);  

	if(!dwErr)
	{
	//  Put code that uses the credentials here.

	
		m_bAdminPrivilege=TRUE;
	}
}

void CiPodWizardApp::EndRunAsAdmin()
{
	if (m_bAdminPrivilege)
	{
		m_bAdminPrivilege=FALSE;

		//  When you have finished using the credentials,
	//  erase them from memory.
		SecureZeroMemory(pszName, sizeof(pszName));
		SecureZeroMemory(pszPwd, sizeof(pszPwd));
	}
}