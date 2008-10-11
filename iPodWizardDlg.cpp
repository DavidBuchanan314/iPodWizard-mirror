// iPodWizardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "iPodWizard.h"
#include "iPodWizardDlg.h"
#include ".\ipodwizarddlg.h"
#include <afxctl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "ResourceManager.h"
#include "Picture.h"
#include "ScanDialog.h"
#include "TweaksDialog.h"
#include "HelpDialog.h"
#include "TipDlg.h"

#define BUFSIZE 512

#include <windows.h>

//Help button
BOOL (WINAPI *_TransparentBlt)(HDC, int, int, int, int, HDC, int, int, int, int, UINT) = NULL;

//#import "Zip.lib"

class MyITunesServiceCallback : public IItunesServiceCallback
{
public:
	CiPodWizardDlg *ipw;
	UINT refCount;
	DWORD cookie;

	MyITunesServiceCallback(CiPodWizardDlg *pipw)
	{
		ipw=pipw;
		refCount = 1;
		cookie = 0;
	}

	STDMETHOD(QueryInterface)(REFIID iid, void** ppvObject)
	{		
		if (iid == IID_IUnknown)
		{
			*ppvObject = static_cast<IUnknown*>(this);
		}
		else if (iid == IID_IDispatch)
		{
			*ppvObject = static_cast<IDispatch*>(this);
		}
		else if (iid == IID_IItunesServiceCallback)
		{
			*ppvObject = static_cast<IItunesServiceCallback*>(this);
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
		static_cast<IUnknown*>(*ppvObject)->AddRef();
		return S_OK;
	}
	STDMETHOD_(ULONG,AddRef)(void)
	{
		refCount++;
		return S_OK;
	}
	STDMETHOD_(ULONG,Release)(void)
	{
		refCount--;
		if (refCount == 0)
			delete this;
		return S_OK;
	}

	STDMETHOD(GetTypeInfoCount)(UINT* pctinfo)
	{
		return E_FAIL;
	}
	STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
	{
		return E_FAIL;
	}
	STDMETHOD(GetIDsOfNames)(const IID &riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
	{
		return E_FAIL;
	}
	STDMETHOD(Invoke)(DISPID dispIdMember, const IID &riid, LCID lcid, WORD wFlags,
					  DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
	{
		return E_FAIL;
	}
	STDMETHOD(OnDeviceStateChanged)(long hDevice, EItunesDeviceClass devClass, _EDeviceStatus lNewState )
	{
		ipw->RefreshiPodDrives();
		return S_OK;
	}
    STDMETHOD(OnRequestClientCookie)(unsigned long * pdwCookie)
	{
		if (pdwCookie)
		{
			*pdwCookie = cookie;
			return S_OK;
		}
		else
			return E_FAIL;
	}

};

//Identify iPod
IItunesService *itsv;
IItunesDevices *itdv;
//IItunesServiceCallback *itcb;

//Downgrade
BOOL bEnumSuccess;
int iTotal;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CiPodWizardDlg dialog



CiPodWizardDlg::CiPodWizardDlg(CWnd* pParent /*=NULL*/)
	: CExDialog(CiPodWizardDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_FirmwareFileName.Empty();
	m_FirmwareiPSWFileName.Empty();
	m_TipFirst = TRUE;
}

void CiPodWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CExDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FIRMWARE_COMBO, m_FirmwareCombo);
	DDX_Control(pDX, IDC_MODE_COMBO, m_EditModeCombo);
	DDX_Control(pDX, IDC_OPT_TAB, m_OptionsTab);
	DDX_Control(pDX, IDC_FIRMWARE_LIST, m_FirmwareList);
	DDX_Control(pDX, IDC_IPODDRIVE_COMBO, m_iPodDriveCombo);
}

BEGIN_MESSAGE_MAP(CiPodWizardDlg, CExDialog)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_OPEN, OnBnClickedOpen)
	ON_BN_CLICKED(ID_LOAD_FIRMWARE, OnBnClickedLoadFirmware)
	ON_NOTIFY(TCN_SELCHANGE, IDC_OPT_TAB, OnTcnSelchangeOptionTab)
	ON_BN_CLICKED(IDC_WRITE_FIRMWARE_BUTTON, OnBnClickedWriteFirmwareButton)
	ON_BN_CLICKED(ID_ABOUT, OnBnClickedAbout)
	ON_BN_CLICKED(IDC_USYSINFO_BUTTON, OnBnClickedUpdateSysInfo)
	//ON_BN_CLICKED(IDC_REFRESHDRV_BUTTON, OnBnClickedRefreshDrives)
	ON_BN_CLICKED(ID_TWEAKS, OnBnClickedTweaks)
	ON_CBN_SELCHANGE(IDC_IPODDRIVE_COMBO, OnCbnSelChangeiPodDriveCombo)
	ON_BN_CLICKED(IDC_LOADIPODFW_BUTTON, OnBnClickedLoadipodfwButton)
	ON_CBN_SELCHANGE(IDC_MODE_COMBO, OnCbnSelchangeModeCombo)
	ON_BN_CLICKED(ID_EJECT_BUTTON, &CiPodWizardDlg::OnBnClickedEjectButton)
	ON_MESSAGE(WM_APP, OniPodEvent)
	ON_BN_CLICKED(ID_OPEN_FFILE, &CiPodWizardDlg::OnBnClickedOpenFfile)
	ON_BN_CLICKED(IDC_WRITE2IPOD_FIRMWARE_BUTTON, &CiPodWizardDlg::OnBnClickedWrite2ipodFirmwareButton)
	ON_NOTIFY(NM_RCLICK, IDC_FIRMWARE_LIST, &CiPodWizardDlg::OnLvnItemRightClickedFirmwareList)
END_MESSAGE_MAP()

// CiPodWizardDlg message handlers

BOOL CiPodWizardDlg::OnInitDialog()
{
	CExDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//initialize modes
	m_EditModeCombo.ResetContent();
	m_EditModeCombo.AddString(_T("Updater"));
	m_EditModeCombo.AddString(_T("iPod"));
	m_EditModeCombo.AddString(_T("Firmware File"));
	m_EditModeCombo.AddString(_T("iPodSoftware File"));
	m_EditModeCombo.SetCurSel(0);

	// initialize list
	m_FirmwareList.SetExtendedStyle(m_FirmwareList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_FirmwareList.InsertColumn(0, TEXT("#"), LVCFMT_LEFT, 20);
	m_FirmwareList.InsertColumn(1, TEXT("Image Checksum"), LVCFMT_LEFT, 95);
	m_FirmwareList.InsertColumn(2, TEXT("Table Checksum"), LVCFMT_LEFT, 95);
	m_FirmwareList.InsertColumn(3, TEXT("Status"), LVCFMT_LEFT, 65);

	//Initialize pages

	CRect rect;

	m_OptionsTab.InsertItem(0, TEXT("Firmware editor"));
	m_OptionsTab.InsertItem(1, TEXT("Themes"));
	m_OptionsTab.InsertItem(2, TEXT("Updater"));
	m_OptionsTab.InsertItem(3, TEXT("Preferences"));

	m_EditorDialog.Create(m_EditorDialog.IDD, &m_OptionsTab);
	m_ThemesDialog.Create(m_ThemesDialog.IDD, &m_OptionsTab);
	m_UpdaterDialog.Create(m_UpdaterDialog.IDD, &m_OptionsTab);
	m_PrefsDialog.Create(m_PrefsDialog.IDD, &m_OptionsTab);

	m_OptionsTab.GetClientRect(&rect);
	m_OptionsTab.AdjustRect(FALSE, &rect);

	m_EditorDialog.MoveWindow(rect);
	m_ThemesDialog.MoveWindow(rect);
	m_UpdaterDialog.MoveWindow(rect);
	m_PrefsDialog.MoveWindow(rect);

	UpdatePages();

	CString inipath;
	GetAppPath(inipath);
	inipath.AppendFormat(TEXT("\\iPW.ini"));
	theApp.m_IniPath.SetString(inipath);

	// try to read last things
	//
	CWinApp *pApp = AfxGetApp();

	m_Filename = pApp->GetProfileString(TEXT("Main"), TEXT("Updater"));
	if (!m_Filename.IsEmpty())
	{
		OpenUpdater(TRUE);
	}

	int iChosenMode=0;
	iChosenMode=_ttoi(pApp->GetProfileString(TEXT("Main"), TEXT("Mode")));
	m_EditModeCombo.SetCurSel(iChosenMode);
	OnCbnSelchangeModeCombo();

	CString s;
	CString firmware = pApp->GetProfileString(TEXT("Main"), TEXT("Firmware"));
	if (!firmware.IsEmpty())
	{
		for (int i = 0; i < m_FirmwareCombo.GetCount(); i++)
		{
			//m_FirmwareCombo.GetLBText(i, s);
			s=m_FirmwareNames.GetAt(i);
			if (s == firmware)
			{
				m_FirmwareCombo.SetCurSel(i);
				break;
			}
		}
	}

	//ipod detector
	CoInitialize(NULL);

	const char * gds={"iTunes"};
	_bstr_t ghh(gds);

	//m_pSink=new MFCSink;
	//m_pSink->SetHandler((CWnd *)this);
	
	//HRESULT gb=CoCreateInstance(CLSID_ItunesService, pUnkOuter,CLSCTX_LOCAL_SERVER,DIID_IItunesService,(void**) &itsv);
	HRESULT gb=CoCreateInstance(CLSID_ItunesService, NULL,0x17,IID_IItunesService,(void**) &itsv);
	
	if (gb != S_OK)
	{
		itsv=NULL;
		CoUninitialize();
		m_COMFindiPod=FALSE;
		goto refresh;
	}

	m_COMFindiPod=TRUE;
	
	IGlobalInterfaceTable *gpGIT;
	HRESULT csdfdsf=CoCreateInstance(CLSID_StdGlobalInterfaceTable,NULL,CLSCTX_INPROC_SERVER,IID_IGlobalInterfaceTable,(void **)&gpGIT);
	m_dwCookieGlobal=0;
	//ServiceCallbackImp *y=new ServiceCallbackImp;
	MyITunesServiceCallback *itcb=new MyITunesServiceCallback(this);
	HRESULT dfbc=gpGIT->RegisterInterfaceInGlobal((IUnknown *)itcb, IID_IItunesServiceCallback, &m_dwCookieGlobal);
	gpGIT->Release();
	csdfdsf=CoCreateInstance(CLSID_StdGlobalInterfaceTable,NULL,CLSCTX_INPROC_SERVER,IID_IGlobalInterfaceTable,(void **)&gpGIT);
	HRESULT ibb=gpGIT->GetInterfaceFromGlobal(m_dwCookieGlobal, IID_IItunesServiceCallback, (void **)&itcb);

	HRESULT gdf=itsv->Login(4, ghh, (IItunesServiceCallback *)itcb, &m_dwCookie);

	itsv->GetItunesDevices(m_dwCookie, (IItunesDevices **)&itdv);

	HRESULT cvnn=gpGIT->RegisterInterfaceInGlobal((IUnknown *)itdv, IID_IItunesDevices, &m_dwCookieGlobal);
	
	/*LPUNKNOWN m_pUnk=m_pSink->GetIDispatch(FALSE);
	if(!AfxConnectionAdvise(y,IID_IItunesServiceCallback,m_pUnk,FALSE,&m_dwCookieSink))
	{
		itcb=NULL;
		CoUninitialize();
		goto refresh;
	}*/
	
refresh:
	RefreshiPodDrives();
	//

	//check automatic theme loader
	ThemeChecker();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

LRESULT CiPodWizardDlg::OniPodEvent(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL || lParam == NULL)
		return 0;
	int mg=(int)wParam;
	if (mg!=14)
		return 0;
	HIPOD ipodid=(HIPOD)lParam;
	RefreshiPodDrives();
	return 0;
}

void CiPodWizardDlg::ThemeChecker()
{
	m_ThemeFirmware.Empty();
	m_ThemeFile.Empty();

	CString m_strCommandLine;
	CString strParam;
	CString strFlag;
	int nParam = 0;
	TCHAR seps1[] = _T("\"");
	TCHAR seps2[] = _T(" ");
	BOOL bRet = FALSE;
	
	strFlag = _T("");
	strParam = _T("");

	m_strCommandLine = ::GetCommandLine();
	m_strCommandLine.TrimLeft();
	m_strCommandLine.TrimRight();
	//MessageBox(m_strCommandLine);
	
	int pos1=0,pos2=0;
	strParam = m_strCommandLine.Tokenize(seps1, pos1);
	strParam = m_strCommandLine.Tokenize(seps1, pos1);
	strFlag = strParam.Tokenize(seps2, pos2);
	if (strFlag.Compare(_TEXT("-theme"))==0)
	{
		strParam = m_strCommandLine.Tokenize(seps1, pos1);
		if (PathFileExists(strParam)==TRUE)
		{
			m_ThemeFile = strParam;

			CFile file;
			if (!file.Open(m_ThemeFile, CFile::modeRead))
			{
				MessageBox(TEXT("Can't open theme file!"));
				return;
			}
			WORD w;
			WORD warr[255];
			int n=0;
			while (TRUE)
			{
				if (file.Read(&w, 2) < 2)
				{
					MessageBox(_T("Error reading theme file!"));
					file.Close();
					return;
				}
				warr[n]=w;
				n++;
				if (w==0)
					break;
			}
			m_ThemeFirmware.Format(TEXT("%s"), (LPCTSTR)warr);
			for (int n=0;n<m_FirmwareNames.GetCount();n++)
			{
				if (m_FirmwareNames.GetAt(n).Compare(m_ThemeFirmware)==0)
				{
					m_FirmwareCombo.SetCurSel(n);
					UpdateData(FALSE);
					file.Close();
					OnBnClickedLoadFirmware();
					return;
				}
			}
			file.Close();
		}
		else
		{
			MessageBox(_TEXT("Theme file not found!"));
		}
	}
}

int CiPodWizardDlg::CheckiPod(BOOL bSilent)
{
	if (GetDlgItem(IDC_STATIC_NOTFOUND)->IsWindowVisible()==TRUE)
	{
		if (bSilent==FALSE)
			MessageBox(TEXT("No iPod is connected to the computer!"));
		return -1;
	}
	return 0;
	CString temp, path;
	m_iPodDriveCombo.GetLBText(m_iPodDriveCombo.GetCurSel(), temp);
	path.SetString(temp.Mid(0,2));
	path.Append(TEXT("\\iPod_Control\\Device"));
	theApp.m_iPodDRV.SetString(path);
	if (PathFileExists(path)==FALSE)
	{
		if (bSilent==FALSE)
			MessageBox(TEXT("The iPod drive you chose from the list below is not a valid iPod!"));
		return -1;
	}
	return 0;
}

void CiPodWizardDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == SC_MINIMIZEHELP)
	{
		CHelpDialog dlg(this);
		if (CheckiPod(TRUE)==-1)
			dlg.DisableFind();
		INT_PTR nRet = -1;
		nRet = dlg.DoModal();
		switch (nRet)
		{
		case -1:
			MessageBox(TEXT("Could not open help dialog!"));
			break;
		default:
			dlg.DestroyWindow();
			break;
		}
	}
	else
	{
		CExDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
int CiPodWizardDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CExDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CiPodWizardDlg::ShowTipAtStartup(void)
{
	// CG: This function added by 'Tip of the Day' component.

	CCommandLineInfo cmdInfo;
	theApp.ParseCommandLine(cmdInfo);
	if (cmdInfo.m_bShowSplash)
	{
		CTipDlg dlg;
		if (dlg.m_bStartup)
			dlg.DoModal();
	}

}

void CiPodWizardDlg::ShowTipOfTheDay(void)
{
	// CG: This function added by 'Tip of the Day' component.

	CTipDlg dlg;
	dlg.DoModal();

}

void CiPodWizardDlg::OnDestroy()
{
	if (itsv)
	{
		itsv->Logout(m_dwCookie);
		//LPUNKNOWN m_pUnk=m_pSink->GetIDispatch(FALSE);
		//AfxConnectionUnadvise(itcb,IID_IItunesServiceCallback,m_pUnk,FALSE,m_dwCookieSink);

		//if(m_pSink!=NULL)
		//{
		//	delete m_pSink;
		//	m_pSink=NULL;
			itsv=NULL;
			//itcb=NULL;
			itdv=NULL;
		//}
		CoUninitialize();
	}
	CExDialog::OnDestroy();
}

void CiPodWizardDlg::OnTimer(UINT nIDEvent)
{
	CDialogMinHelpBtn<CDialog>::OnTimer(nIDEvent);
}

void CiPodWizardDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else if (m_TipFirst)
	{
		m_TipFirst=FALSE;
		ShowTipAtStartup();
	}
	else
	{
		CExDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CiPodWizardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CiPodWizardDlg::OnBnClickedOpen()
{
	CFileDialog dlg(TRUE, TEXT("exe"), TEXT("iPod Updater*.exe"), OFN_HIDEREADONLY, TEXT("EXE files (*.exe)|*.exe||"), this);
	MO_LOAD_FIRMWARE_PATH(dlg)

	if (dlg.DoModal() != IDOK)
		return;
	MO_SAVE_FIRMWARE_PATH(dlg)

	m_Filename = dlg.GetPathName();

	OpenUpdater(FALSE);
}

void CiPodWizardDlg::OnBnClickedTweaks()
{
	if (CheckiPod(FALSE)==-1)
		return;
	CTweaksDialog dlg(this);
	INT_PTR nRet = -1;
	nRet = dlg.DoModal();
	switch (nRet)
	{
	case -1:
		MessageBox(TEXT("Could not open tweaks dialog!"));
		break;
	default:
		dlg.DestroyWindow();
		break;
	}
}

/*void CiPodWizardDlg::OnBnClickedRefreshDrives()
{
	RefreshiPodDrives();
}*/

LPCTSTR BSTR2TCHAR(_bstr_t st)
{
	const int cchWideChar = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)st, -1, NULL, 0);
	LPWSTR lpw = new WCHAR [cchWideChar];
	if(lpw == NULL){
		return NULL;
	}
	*lpw = L'\0';

	const int nUnicodeCount = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)st, -1, lpw, cchWideChar);
	if( nUnicodeCount <= 0 ){
		delete[] lpw;
		return NULL;
	}
	return (LPCTSTR)lpw;
}

CString CiPodWizardDlg::GetPhysicalDiskFromDevice(LPCTSTR lpDrive, BOOL macFormatted)
{
	CString ipod_list;
	CString sDrive;
	TCHAR drive[9] = TEXT("\\\\.\\A:"); 
	DWORD returned;
	STORAGE_DEVICE_NUMBER sdn_drive;
	drive[4]=*lpDrive;
	TCHAR szName[12];

	HANDLE hDrive = CreateFile(drive, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);  
	if (hDrive != (HANDLE)-1)  
	{   
		if (DeviceIoControl(hDrive, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn_drive, sizeof(sdn_drive), &returned, NULL))  
			sdn_drive.PartitionNumber=1;
		else
			sdn_drive.PartitionNumber=0;
		CloseHandle(hDrive);
	}

	for (int y=1;y<10;y++)
	{
		TCHAR devstring[25];
		wsprintf (devstring, TEXT("\\\\.\\PHYSICALDRIVE%i"), y);
		HANDLE h = CreateFile(devstring, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (h != (HANDLE)-1)  
		{  
			STORAGE_DEVICE_NUMBER sdn;  
		
			if (DeviceIoControl(h, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &returned, NULL))  
			{
				if (sdn_drive.DeviceNumber == sdn.DeviceNumber && sdn_drive.DeviceType == sdn.DeviceType)
				{
					if (macFormatted)
					{
							sDrive.Format(TEXT(" [Mac Formatted]"));
					}
					else
					{
						sDrive.Format(TEXT("%s:"), lpDrive);
						if (GetVolumeInformation(sDrive, szName, 12, NULL, NULL, NULL, NULL, NULL))
						{
							if (!_tcscmp(szName, TEXT("")))
								sDrive.Format(TEXT(" (No Name)"));
							else
								sDrive.Format(TEXT(" (%s)"), szName);
						}
					}
					ipod_list.Format(TEXT("%s: %s|"), lpDrive, sDrive);
					m_iPodDevices.Add(devstring);
					if (lpDrive)
						delete[] lpDrive;
					CloseHandle(h);
					break;
				}
			}
			CloseHandle(h);
		}
	}
	return ipod_list;
}

void CiPodWizardDlg::RefreshiPodDrives()
{
	//clean combo
	m_iPodDriveCombo.ResetContent();
	m_iPodDevices.RemoveAll();
	m_iPodHandles.RemoveAll();
	CString sDrive;

	CString ipod_list; //ipod drives list

	if (m_COMFindiPod)
	{
		//#########COM WAY##############
		long ipodid=-1;
		int cn=1;
		itdv->FindFirstDevice(&ipodid);

		while (ipodid>0)
		{
			VARIANT_BOOL lMnt;
			itdv->IsMounted(ipodid, &lMnt);
			m_iPodHandles.Add((DWORD)ipodid);
			EFormatState kFormat;
			if (lMnt==FALSE) //check if mounted
			{
				unsigned short ver=0;
				itdv->GetFirmwareVersion(ipodid, &ver);
				//ipod->GetDeviceUniqueID(appid, ipodid)

				//check for mac ipod
				itdv->GetDriveFormat(ipodid, &kFormat);
				if (kFormat==kFormatMac || kFormat==kFormatUnknown || kFormat==kFormatMacNoFirmware)
				{
					_bstr_t abc2("DDSDGFGFDSGFGDF");
					
					itdv->GetDriveLetter(ipodid, abc2.GetAddress());
					
					LPCTSTR lpDrive2=BSTR2TCHAR(abc2);

					ipod_list.Append(GetPhysicalDiskFromDevice(lpDrive2, TRUE));
				}
				else
				{
					ipod_list.AppendFormat(TEXT("Unmounted%d (v%d)|"), cn, ver);
					cn++;
					m_iPodDevices.Add(TEXT(""));
				}
			}
			else if (lMnt==-1)
			{
				itdv->GetDriveFormat(ipodid, &kFormat);
				if (kFormat==kFormatFat32) //only format of fat32 is valid
				{
					//get physical disk
					_bstr_t abc("DDSDGFGFDSGFGDF");
					
					itdv->GetDriveLetter(ipodid, abc.GetAddress());
					
					LPCTSTR lpDrive=BSTR2TCHAR(abc);
					
					ipod_list.Append(GetPhysicalDiskFromDevice(lpDrive, FALSE));
				}
			}
			itdv->FindNextDevice(&ipodid);
		}
		//#########COM WAY##############
	}
	else
	{
		//Find the iPod devices (check hidden partition == firmware)
		unsigned char buffer[512];
		int x,list[10]={0},y=0;
		for (x = 1; x < 10; x++)
		{
			TCHAR devstring[25];
			int j,dev=-1;
		       
			wsprintf (devstring, TEXT("\\\\.\\PHYSICALDRIVE%i"), x);
			theApp.InitSectorSize(devstring, true);
			dev = _wopen (devstring, O_RDONLY | _O_RAW);

			if (dev == -1)
				continue;
			
			theApp.InitHDDValues(dev);

			_lseek(dev, theApp.FIRMWARE_START, SEEK_SET);
			_read(dev, buffer, theApp.BLOCK_SIZE);

			for (j = 0; j < theApp.BLOCK_SIZE - 6; j++)
			{

				if (buffer [j] == 'S' && buffer [j+2] == 'T' && buffer [j+4] == 'O' && buffer [j+6] == 'P')
				{
					y++;			
					list[y]=x;
				}
			}
		      
			_close(dev);
		}
		if (y==0) //Why process all the code if there is no iPod device found?
			goto ShowStatus;

		//Get all volumes devicenumber/devicetype in order to compare with our ipod devices.
		int idx;
		STORAGE_DEVICE_NUMBER sdn_drives[26];
		TCHAR drive[9] = TEXT("\\\\.\\A:");   
	 
		DWORD driveMask = GetLogicalDrives();  
		for(idx = 0; idx < 26; idx++, driveMask >>= 1)  
		{  
			if (idx==0)
				continue;
			BOOL b = (driveMask & 1);   
			if (b)  
			{  
				drive[4] = 'A' + idx;
				HANDLE hDrive = CreateFile(drive, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);  
	 
				if (hDrive != (HANDLE)-1)  
				{   
					DWORD returned2;  

					if (DeviceIoControl(hDrive, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn_drives[idx], sizeof(sdn_drives[idx]), &returned2, NULL))  
						sdn_drives[idx].PartitionNumber=1;
					else
						sdn_drives[idx].PartitionNumber=0;
					CloseHandle(hDrive);
				}
			}
		}


		//Check what volumes are compatible with the iPod devices we've found.
		for (y=1;y<10;y++)
			if (list[y]!=0)
			{
				TCHAR devstring2[25];
				wsprintf (devstring2, TEXT("\\\\.\\PHYSICALDRIVE%i"), list[y]);
				HANDLE h = CreateFile(devstring2, 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);  
		 
				if (h != (HANDLE)-1)  
				{  
					STORAGE_DEVICE_NUMBER sdn;  
					DWORD returned;  

					if (DeviceIoControl(h, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &returned, NULL))  
					{
						for (idx=0;idx<26;idx++)
							if (sdn_drives[idx].DeviceNumber == sdn.DeviceNumber && sdn_drives[idx].DeviceType == sdn.DeviceType)
							{
								ipod_list.AppendFormat(TEXT("%c:|"), 'A' + idx);
								m_iPodDevices.Add(devstring2);
								break;
							}
					}
					CloseHandle(h);
				}
				y++;
			}	
	}

	if (ipod_list.Right(1).Compare(TEXT("|"))==0)
			ipod_list.Truncate(ipod_list.GetLength()-1);

	if (!m_COMFindiPod)
		goto ShowStatus;
	
	//Find all iPods	
	//TCHAR szName[12];
	WORD wlast=0;
	if (ipod_list.IsEmpty()==TRUE)
	{
		GetDlgItem(IDC_STATIC_FOUND)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_IPODDRIVE_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_NOTFOUND)->ShowWindow(SW_SHOW);
		GetDlgItem(ID_EJECT_BUTTON)->EnableWindow(FALSE);
		if (m_FirmMode==1)
			GetDlgItem(IDC_WRITE_FIRMWARE_BUTTON)->EnableWindow(FALSE);
		theApp.m_DeviceSel.Empty();
	}
	else
	{
		GetDlgItem(IDC_STATIC_FOUND)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_IPODDRIVE_COMBO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_NOTFOUND)->ShowWindow(SW_HIDE);
		GetDlgItem(ID_EJECT_BUTTON)->EnableWindow(TRUE);
		if (ipod_list.Find(TEXT("|"),0)>0)
		{
			WORD i;
			for (i=0;i<ipod_list.GetLength();i++)
				if (ipod_list.Mid(i,1).Compare(TEXT("|"))==0)
				{
					m_iPodDriveCombo.AddString(ipod_list.Mid(wlast, i-wlast));
					wlast=i+1;
				}
			m_iPodDriveCombo.AddString(ipod_list.Mid(wlast, i-wlast));
		}
		else
		{
			m_iPodDriveCombo.AddString(ipod_list);
		}
		sDrive.Format(TEXT("Found %d iPod drives:"), m_iPodDriveCombo.GetCount());
		GetDlgItem(IDC_STATIC_FOUND)->SetWindowText(sDrive);
		m_iPodDriveCombo.SetCurSel(0);
		OnCbnSelChangeiPodDriveCombo();
		//add something that if editing ipod firm, make sure the ipod editing isn't out
	}
	return;

ShowStatus:
	//Find all iPods	
	TCHAR szName[12];
	WORD last=0;
	if (ipod_list.IsEmpty()==TRUE)
	{
		GetDlgItem(IDC_STATIC_FOUND)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_IPODDRIVE_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_NOTFOUND)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LOADIPODFW_BUTTON)->EnableWindow(FALSE);
		if (m_FirmMode==1)
			GetDlgItem(IDC_WRITE_FIRMWARE_BUTTON)->EnableWindow(FALSE);
		theApp.m_DeviceSel.Empty();
	}
	else
	{
		GetDlgItem(IDC_STATIC_FOUND)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_IPODDRIVE_COMBO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_NOTFOUND)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LOADIPODFW_BUTTON)->EnableWindow(TRUE);
		CString sDrive;
		if (ipod_list.Find(TEXT("|"),0)>0)
		{
			for (WORD i=0;i<ipod_list.GetLength();i++)
				if (ipod_list.Mid(i,1).Compare(TEXT("|"))==0)
				{
					if (GetVolumeInformation(ipod_list.Mid(last, 3), szName, 12, NULL, NULL, NULL, NULL, NULL))
					{
						sDrive.Format(TEXT("%s"),ipod_list.Mid(last, 3));
						if (!_tcscmp(szName, TEXT("")))
							sDrive.AppendFormat(TEXT(" (No Name)"));
						else
							sDrive.AppendFormat(TEXT(" (%s)"), szName);
					}
					m_iPodDriveCombo.AddString(sDrive);
					last=i+1;
				}
			if (GetVolumeInformation(ipod_list.Mid(last, 3), szName, 12, NULL, NULL, NULL, NULL, NULL))
			{
				sDrive.Format(TEXT("%s"),ipod_list.Mid(last, 3));
				if (!_tcscmp(szName, TEXT("")))
					sDrive.AppendFormat(TEXT(" (No Name)"));
				else
					sDrive.AppendFormat(TEXT(" (%s)"), szName);
			}
			m_iPodDriveCombo.AddString(sDrive);
		}
		else
		{
			if (GetVolumeInformation(ipod_list, szName, 12, NULL, NULL, NULL, NULL, NULL))
				if (!_tcscmp(szName, TEXT("")))
					ipod_list.AppendFormat(TEXT(" (No Name)"));
				else
					ipod_list.AppendFormat(TEXT(" (%s)"), szName);
			m_iPodDriveCombo.AddString(ipod_list);
		}
		sDrive.Format(TEXT("Found %d iPod drives:"), m_iPodDriveCombo.GetCount());
		GetDlgItem(IDC_STATIC_FOUND)->SetWindowText(sDrive);
		m_iPodDriveCombo.SetCurSel(0);
		theApp.m_DeviceSel=m_iPodDevices.GetAt(m_iPodDriveCombo.GetCurSel());
	}
}

void CiPodWizardDlg::OnCbnSelChangeiPodDriveCombo()
{
	if (m_FirmMode==1)
		if (theApp.m_DeviceSel != m_iPodDevices.GetAt(m_iPodDriveCombo.GetCurSel()))
		{
			if (MessageBox(TEXT("Are you sure you want to select another iPod?\nYou are currently editing an iPod's firmware.\nIf you change now selection, you won't be able to write the changes you've made to the firmware.\nAre you sure you want to continue?"), TEXT("Warning"), MB_YESNO) != IDYES)
				return;
		}

	theApp.m_DeviceSel=m_iPodDevices.GetAt(m_iPodDriveCombo.GetCurSel());
	CString checkmac;
	m_iPodDriveCombo.GetLBText(m_iPodDriveCombo.GetCurSel(), checkmac);
	
	if (checkmac.Right(15).Compare(_T("[Mac Formatted]"))==0)
	{
		GetDlgItem(ID_EJECT_BUTTON)->SetWindowTextW(TEXT("Mount"));
		GetDlgItem(ID_EJECT_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_LOADIPODFW_BUTTON)->EnableWindow(TRUE);
	}
	else
	{
		VARIANT_BOOL lMnt;
		itdv->IsMounted(m_iPodHandles.GetAt(m_iPodDriveCombo.GetCurSel()), &lMnt);
		if (lMnt==FALSE)
		{
			GetDlgItem(ID_EJECT_BUTTON)->SetWindowTextW(TEXT("Mount"));
			GetDlgItem(IDC_LOADIPODFW_BUTTON)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(ID_EJECT_BUTTON)->SetWindowTextW(TEXT("Eject"));
			GetDlgItem(IDC_LOADIPODFW_BUTTON)->EnableWindow(TRUE);
		}
	}
}

static BOOL CALLBACK myWinFunc(HWND handle, LPARAM lParam)
{
	//int len=1;
	//len=GetWindowTextLength(handle);
	wchar_t s[14];
	GetWindowText(handle, s, 14);
	wchar_t s2[7];
	GetClassName(handle, s2, 7);
	CString sr(s);
	CString sr2(s2);
	if (sr2.Compare(_T("#32770")) == 0 && sr.Left(13).Compare(_T("iPod Updater ")) == 0)
	{
		EnumChildWindows(handle, myChildWinFunc, 0);
		return FALSE;
	}
	return TRUE;
}

static BOOL CALLBACK myChildWinFunc(HWND handle, LPARAM lParam)
{
	//int len=1;
	//len=GetWindowTextLength(handle);
	wchar_t s[7];
	GetWindowText(handle, s, 7);
	wchar_t s2[7];
	GetClassName(handle, s2, 7);
	CString sr(s);
	CString sr2(s2);
	//if (sr2.Compare(_T("Button")) == 0 && sr.Compare(_T("Update")) == 0)
	if (sr2.Compare(_T("Button")) == 0)
	{
		EnableWindow(handle, TRUE);
		iTotal++;
		if (iTotal==2)
		{
			bEnumSuccess=TRUE;
			MessageBox(0, _T("Successfully resetted 'Update' button state!"), _T("Success"), 0);
			return FALSE;
		}
	}
	return TRUE;
}

void CiPodWizardDlg::OnBnClickedUpdateSysInfo()
{
	bEnumSuccess=FALSE;
	iTotal=0;
	MessageBox(_T("NOTE: THIS FOR OLD UPDATERS ONLY, MEANING iTunes BELOW VERSION 7!\nFirst make sure the updater is opened and detected your iPod.\nAfter you see the 'Update' button disabled, click OK on this message."), _T("Note"), 0);
	EnumWindows(myWinFunc, 0);
	if (bEnumSuccess==TRUE)
	{
		MessageBox(_T("iPodWizard will now exit, because you can't update your iPod while iPodWizard is opened. After it is closed, switch to updater window and update as usual."), _T("Note!"), 0);
		PostQuitMessage(0);
	}
	else
	{
		MessageBox(_T("Failed finding the Update button!\nMake sure the updater is opened and an iPod is plugged in and the updater detects it!"), _T("Error"), MB_ICONERROR | MB_OK);
	}
	/*
	if (GetDlgItem(IDC_STATIC_NOTFOUND)->IsWindowVisible()==TRUE)
	{
		MessageBox(TEXT("No iPod is connected to the computer!"));
		return;
	}
	CFile file;
	CString temp, path;
	m_iPodDriveCombo.GetLBText(m_iPodDriveCombo.GetCurSel(), temp);
	path.SetString(temp.Mid(0,2));
	path.Append(TEXT("\\iPod_Control\\Device\\SysInfo"));
	if (!file.Open(path, CFile::modeRead ))
	{
		MessageBox(TEXT("Unable to load SysInfo file! Make sure you chose the right drive."));
		return;
	}
	BYTE b;
	DWORD len, i;
	len=(DWORD)file.GetLength();
	char *buffer = new char[len];
	i=0;
	while (TRUE)
	{
		if (file.Read(&b, 1) < 1)
			break;
		buffer[i]=(char)b;
		i++;
	}
	DWORD pos;
	CString s(buffer);
	CString s2;
	CString buildID;
	buildID.Format(TEXT("buildID changes:\nBefore - "));
	for (i=0; i<len; i++)
	{
		if (buffer[i]==10 && (len-i)>11)
		{
			s2.SetString(s.Mid(i+1,11));
			if (s2.Compare(TEXT("buildID: 0x")) == 0)
			{
				pos=i+12;
				buildID.Append(s.Mid(pos-2,10));
				break;
			}
		}
	}
	buffer[pos+3]='0';
	if (buffer[pos+2] > '0')
		buffer[pos+2]--;
	else if (buffer[pos+1] > '0')
	{
		buffer[pos+2]='9';
		buffer[pos+1]--;
	}
	LPWSTR lpszW = new WCHAR[2048];
	int nLen = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)buffer, -1, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)buffer, -1, lpszW, nLen);
	s.Format(TEXT("%s"),lpszW);
	buildID.AppendFormat(TEXT("\nAfter - %s"), s.Mid(pos-2,10));
	file.Close();
	if (!file.Open(path, CFile::modeWrite))
	{
		MessageBox(TEXT("There was an error accessing the file for writing! Make sure no other program uses it."));
		return;
	}
	file.Flush();
	file.Write(buffer,len);
	file.Close();
	s.Format(TEXT("Your iPod's SysInfo file was changed successfully!\n"));
	s.Append(buildID);
	MessageBox(s, TEXT("Success"));
	*/
}

void CiPodWizardDlg::GetAppPath(CString &app_path)
{
	TCHAR szName[MAX_PATH];
	GetModuleFileName(NULL, szName, MAX_PATH);
	app_path.Format(TEXT("%s"), szName);
	app_path = app_path.Left(app_path.ReverseFind('\\'));
}

void CiPodWizardDlg::OpenUpdater(BOOL firstTime)
{
	INT_PTR numResources;

	GetDlgItem(ID_LOAD_FIRMWARE)->EnableWindow(FALSE);

	if (!m_RsrcMgr.Open(m_Filename))
	{
		DWORD err = GetLastError();
		if (err == 126 && firstTime == TRUE)
			return;
		CString s;
		s.Format(TEXT("Unable to open file! Code=%d"), err);
		MessageBox(s);
		return;
	}

	if (!m_RsrcMgr.Enum(FIRMWARE_RESOURCE_TYPE) || (numResources = m_RsrcMgr.GetNumResources()) == 0)
	{
		MessageBox(TEXT("Unable to find resources!"));
		return;
	}

	m_UpdaterDialog.GenerateList(&m_RsrcMgr, m_Filename);

	// fill combo
	m_FirmwareCombo.ResetContent();
	m_FirmwareNames.RemoveAll();
	TCHAR szTemp[BUFSIZE];

	for (int i = 0; i < numResources; i++)
	{
		if (m_RsrcMgr.GetResourceName(i).Find(TEXT("128."))!=-1 || m_RsrcMgr.GetResourceName(i).Find(TEXT("129."))!=-1)
			continue;
		m_FirmwareNames.Add(m_RsrcMgr.GetResourceName(i));
		if (GetPrivateProfileString(TEXT("FirmwareDisplay"), m_RsrcMgr.GetResourceName(i).Left(m_RsrcMgr.GetResourceName(i).Find(_T("."), m_RsrcMgr.GetResourceName(i).Find(_T(".")) + 1)), NULL, szTemp, BUFSIZE, theApp.m_IniPath)>0)
		{
			CString s(szTemp);
			s.AppendFormat(_T(" (v%s)"), m_RsrcMgr.GetResourceName(i).Right(m_RsrcMgr.GetResourceName(i).GetLength() - 13));
			m_FirmwareCombo.AddString(s);
		}
		else
			m_FirmwareCombo.AddString(m_RsrcMgr.GetResourceName(i));
	}
	m_FirmwareCombo.SetCurSel(0);

	GetDlgItem(ID_LOAD_FIRMWARE)->EnableWindow(TRUE);
}

void CiPodWizardDlg::OnBnClickedLoadFirmware()
{
	CScanDialog dlg;
	CString name;
	theApp.m_BootPicChanged=FALSE;
	if (m_FirmMode==3 && m_EditModeCombo.GetCurSel()==3)
	{
		name.Format(_T("A"));
		dlg.Create(dlg.IDD, this);

		dlg.SendMessage(WM_APP, (WPARAM)TEXT("Extracting iPod Software file..."), 0);
		CZipArchive ipsw;
		ipsw.Open(m_iPSWFile, CZipArchive::openReadOnly);
		CFileHeader finfo;
		if (!ipsw.OpenFile(m_iPSW_FID))
			return;
		if (!ipsw.GetFileInfo(finfo, m_iPSW_FID))
		{
			//error
		}
		dlg.ScanFirmware(NULL, finfo.m_uUncomprSize);
		DWORD iRead;
		DWORD nBufSize = 65535;
		DWORD ptr=0;
		unsigned char *buf=new unsigned char[finfo.m_uUncomprSize];
		CAutoBuffer buffer(nBufSize);
		do
		{
			iRead = ipsw.ReadFile(buffer, buffer.GetSize());
			if (iRead)
			{
				memcpy(&buf[ptr], buffer, iRead);
				ptr+=iRead;
				dlg.SendMessage(WM_APP, NULL, ptr);
			}
		}
		while (iRead == buffer.GetSize());

		ipsw.Close();
		//ipsw.ExtractFile(0, app_pth)

		theApp.BLOCK_SIZE=2048;
		if (finfo.m_uUncomprSize%theApp.BLOCK_SIZE!=0)
			theApp.BLOCK_SIZE=512;

		if (!m_Firmware.SetFirmware(name, buf, finfo.m_uUncomprSize))
		{
			MessageBox(TEXT("Unable to allocate memory!"));
			delete buf;
			return;
		}

		delete buf;

		UpdateChecksums();

		//dlg.Create(dlg.IDD, this);
		dlg.ScanFirmware(&m_Firmware);
		dlg.SetWindowTextW(_T("Loading iPodWizard screens..."));

		m_PrefsDialog.SetFirmware(&m_Firmware);
		m_ThemesDialog.SetFirmware(&m_Firmware, &m_EditorDialog);
		m_EditorDialog.SetFirmware(&m_Firmware);

		dlg.DestroyWindow();
		
		GetDlgItem(IDC_WRITE_FIRMWARE_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_WRITE2IPOD_FIRMWARE_BUTTON)->EnableWindow(TRUE);
	}
	else
	{
		m_FirmwareFileName.Empty();
		m_FirmwareiPSWFileName.Empty();

		//Load the firmware resource into memory
		name = m_FirmwareNames.GetAt(m_FirmwareCombo.GetCurSel());
		if (!m_RsrcMgr.LoadResource(FIRMWARE_RESOURCE_TYPE, name))
		{
			MessageBox(TEXT("Unable to load resource!"));
			return;
		}

		theApp.BLOCK_SIZE=2048;
		if (m_RsrcMgr.GetResourceSize()%theApp.BLOCK_SIZE!=0)
			theApp.BLOCK_SIZE=512;

		if (!m_Firmware.SetFirmware(name, m_RsrcMgr.GetResourceBuffer(), m_RsrcMgr.GetResourceSize()))
		{
			MessageBox(TEXT("Unable to allocate memory!"));
			return;
		}

		UpdateChecksums();

		dlg.Create(dlg.IDD, this);
		dlg.ScanFirmware(&m_Firmware);

		dlg.SetWindowTextW(_T("Loading iPodWizard screens..."));

		m_PrefsDialog.SetFirmware(&m_Firmware);
		m_ThemesDialog.SetFirmware(&m_Firmware, &m_EditorDialog);
		m_EditorDialog.SetFirmware(&m_Firmware);

		dlg.DestroyWindow();
		
		m_FirmMode=0;
		GetDlgItem(IDC_WRITE_FIRMWARE_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_WRITE2IPOD_FIRMWARE_BUTTON)->EnableWindow(FALSE);

		// save settings
		CWinApp *pApp = AfxGetApp();
		pApp->WriteProfileString(TEXT("Main"), TEXT("Updater"), m_Filename);
		pApp->WriteProfileString(TEXT("Main"), TEXT("Firmware"), name);

		//Check if we need to auto load a theme:
		if (m_ThemeFirmware.IsEmpty()==FALSE)
		{
			m_ThemeFirmware.Empty();
			m_ThemesDialog.LoadTheme(m_ThemeFile);
		}
	}
}

void CiPodWizardDlg::OnTcnSelchangeOptionTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdatePages();

	*pResult = 0;
}

void CiPodWizardDlg::UpdatePages()
{
	int i = m_OptionsTab.GetCurSel();

	switch (i)
	{
	case 0: //Editor
		m_EditorDialog.ShowWindow(SW_SHOW);
		m_ThemesDialog.ShowWindow(SW_HIDE);
		m_UpdaterDialog.ShowWindow(SW_HIDE);
		m_PrefsDialog.ShowWindow(SW_HIDE);
		break;
	case 1: //Themes
		m_EditorDialog.ShowWindow(SW_HIDE);
		m_ThemesDialog.ShowWindow(SW_SHOW);
		m_UpdaterDialog.ShowWindow(SW_HIDE);
		m_PrefsDialog.ShowWindow(SW_HIDE);
		break;
	case 2: //Updater
		m_EditorDialog.ShowWindow(SW_HIDE);
		m_ThemesDialog.ShowWindow(SW_HIDE);
		m_PrefsDialog.ShowWindow(SW_HIDE);
		m_UpdaterDialog.ShowWindow(SW_SHOW);
		break;
	case 3: //Preferences
		m_EditorDialog.ShowWindow(SW_HIDE);
		m_ThemesDialog.ShowWindow(SW_HIDE);
		m_UpdaterDialog.ShowWindow(SW_HIDE);
		m_PrefsDialog.ShowWindow(SW_SHOW);
		break;
	}
}

void CiPodWizardDlg::OnBnClickedWriteFirmwareButton()
{
	if (m_FirmMode==1)
	{
		if (MessageBox(TEXT("Are you sure you want to write the modified firmware to your iPod?\nBefore continuing please make sure iPodWizard is running in front and don't switch to any other program while writing the firmware to the iPod!!!"), TEXT("Warning"), MB_YESNO) != IDYES)
			return;

		if (theApp.m_BootPicChanged)
			if (theApp.m_ReflashBootImage)
				m_Firmware.ResetImageAdresses();
			else if (MessageBox(_T("You've changed the boot flash image (boot images).\nDo you want to reflash it so it updates the image upon iPod reset?\nWarning: Make sure you don't disconnect your iPod during the flash update (bar filling)!"), _T("Question"), MB_YESNO) == IDYES)
				m_Firmware.ResetImageAdresses();

		if (m_Firmware.GetNumOTFFonts()>0)
		{
			COTFFont m_Font;
			for (DWORD i=0;i<m_Firmware.GetNumOTFFonts();i++)
			{
				if (!m_Font.Read(m_Firmware.GetOTFFont(i), FALSE))
				{
					MessageBox(TEXT("One of the OTF fonts is damages and therefor iPodWizard cannot write the update as it may cause damage to iPod.\nPlease reload the firmware and make the changes again."));
					return;
				}
				m_Font.SyncChecksums();
			}
		}

		theApp.m_SavingFirmware=TRUE;

		m_Firmware.SyncChecksum();

		UpdateChecksums();

		if (CheckiPod(FALSE)==-1)
			return;

		TCHAR devstring[25];
		wsprintf (devstring, TEXT("%s"), theApp.m_DeviceSel);
		int dev = _wopen (devstring, O_WRONLY | _O_RAW);
		if (dev==-1)
		{
			MessageBox(TEXT("Unable to access iPod! Make sure programs who use the iPod like iTunes are closed."));
			return;
		}

		DWORD size=m_Firmware.GetFirmwareSize();
		LPBYTE lpBuf=m_Firmware.GetFirmwareBuffer();
		_lseek(dev, theApp.FIRMWARE_START, SEEK_SET);
		
		CScanDialog dlg;
		dlg.Create(dlg.IDD, this);

		dlg.SendMessage(WM_APP, (WPARAM)_T("Writing iPod firmware to disk"), 0);
		dlg.m_ProgressCtrl.SetRange32(0, size);
		dlg.m_ProgressCtrl.SetPos(0);
		int m_UpdatesDone=0,ret;
		DWORD i;
		/*
		for (i=0;i<size;i+=theApp.BLOCK_SIZE)
		{
			ret=_write(dev, &lpBuf[i], theApp.BLOCK_SIZE);
			if (ret==-1 && m_UpdatesDone==0)
			{
				_close(dev);
				MessageBox(TEXT("Can't write the modded firmware to your iPod.\r\nYour iPod is remained untouched.\r\nRestarting your computer may help to solve this problem."), TEXT("Error"));
				return;
			}
			else if (ret!=theApp.BLOCK_SIZE)
			{
				_close(dev);
				MessageBox(TEXT("A severe writing error occured to the iPod and the firmware might be damaged.\nIn order to fix this, you must go into disk mode (see more info on our website www.iPodWizard.net) and restore or update using original Apple updater."), TEXT("Error"));
				return;
			}
			m_UpdatesDone++;
			if (i%(theApp.BLOCK_SIZE*2000)==0)
			{
				dlg.m_ProgressCtrl.SetPos(i);
				dlg.m_ProgressCtrl.UpdateWindow();
			}
		}*/

		DWORD szret=_pdwrite(dev, &lpBuf[0], size, theApp.BLOCK_SIZE);
		if (szret!=size)
		{
			WCHAR cc[200];
			wsprintf(cc, _T("ERROR %d"), szret);
			MessageBox(cc);
			return;
		}

		dlg.m_ProgressCtrl.SetPos(size);
		dlg.m_ProgressCtrl.UpdateWindow();
		_close(dev);
		dlg.DestroyWindow();

		theApp.m_SavingFirmware=FALSE;

		MessageBox(TEXT("Successfully written the modded firmware to your iPod!\r\nWarning: According to Apple's EULA, uploading firmware to 3rd party websites is prohibited! Use this firmware for personal use only!\r\nNow in order for changed to take order, you need to safe remove your iPod from your computer and the iPod will auto reset itself."));
	}
	else if (m_FirmMode==0)
	{
		if (MessageBox(TEXT("Are you sure you want to write the modified firmware to the updater?"), TEXT("Warning"), MB_YESNO) != IDYES)
			return;

		//Check for diskspace:
		int f=_wopen(m_Filename, O_RDONLY | _O_RAW);
		__int64	m_uliFreeBytesAvailable,m_uliTotalNumberOfBytes;
		if( f==-1 || !GetDiskFreeSpaceEx(
			m_Filename.Left(3),                  // directory name
			(PULARGE_INTEGER)&m_uliFreeBytesAvailable,         // bytes available to caller
			(PULARGE_INTEGER)&m_uliTotalNumberOfBytes,         // bytes on disk
			NULL) )   // free bytes on disk
		{
			MessageBox(TEXT("Unable to access hard disk!"));
			return;
		}
		
		if (m_uliFreeBytesAvailable < (_filelength(f) + 0xFFFF))
		{
			MessageBox(TEXT("You don't have enough disk space to write the updater file!"));
			return;
		}
		_close(f);

		CString title, title_new;
		GetWindowText(title);
		title_new.Format(TEXT("%s - Writing firmware, please wait..."), title);
		SetWindowText(title_new);

		if (m_Firmware.GetNumOTFFonts()>0)
		{
			COTFFont m_Font;
			for (DWORD i=0;i<m_Firmware.GetNumOTFFonts();i++)
			{
				if (!m_Font.Read(m_Firmware.GetOTFFont(i), FALSE))
				{
					MessageBox(TEXT("One of the OTF fonts is damages and therefor iPodWizard cannot write the update as it may cause damage to iPod.\nPlease reload the firmware and make the changes again."));
					return;
				}
				m_Font.SyncChecksums();
			}
		}

		theApp.m_SavingFirmware=TRUE;

		m_Firmware.SyncChecksum();

		UpdateChecksums();

		if (!m_RsrcMgr.WriteResource(m_Filename, FIRMWARE_RESOURCE_TYPE, m_Firmware.GetName(), m_Firmware.GetFirmwareBuffer(), m_Firmware.GetFirmwareSize()))
		{
			SetWindowText(title);
			MessageBox(TEXT("Unable to write modified firmware!"));
		}
		else
		{
			CString s;
			s.Format(TEXT("1"));
			if (!m_RsrcMgr.Open(m_Filename))
			{
				DWORD err = GetLastError();
				s.Format(TEXT("Unable to reopen file! Code=%d"), err);
			}
			SetWindowText(title);
			MessageBox(TEXT("Updater modified successfully!\nWarning: According to Apple's EULA, uploading firmware to 3rd party websites is prohibited! Use this firmware for personal use only!"), TEXT("Success"));
			if (s.Compare(TEXT("1")))
				MessageBox(s);
		}

		theApp.m_SavingFirmware=FALSE;
	}
	else if (m_FirmMode==2)
	{
		if (MessageBox(TEXT("Are you sure you want to write the modified firmware to the file?"), TEXT("Warning"), MB_YESNO) != IDYES)
			return;

		CString title, title_new;
		GetWindowText(title);
		title_new.Format(TEXT("%s - Writing firmware, please wait..."), title);
		SetWindowText(title_new);

		if (m_Firmware.GetNumOTFFonts()>0)
		{
			COTFFont m_Font;
			for (DWORD i=0;i<m_Firmware.GetNumOTFFonts();i++)
			{
				if (!m_Font.Read(m_Firmware.GetOTFFont(i), FALSE))
				{
					MessageBox(TEXT("One of the OTF fonts is damages and therefor iPodWizard cannot write the update as it may cause damage to iPod.\nPlease reload the firmware and make the changes again."));
					return;
				}
				m_Font.SyncChecksums();
			}
		}

		theApp.m_SavingFirmware=TRUE;

		m_Firmware.SyncChecksum();

		UpdateChecksums();

		CFile f_wr;
		if (!f_wr.Open(m_FirmwareFile, CFile::modeCreate | CFile::modeWrite))
		{
			//error
			SetWindowText(title);
			MessageBox(TEXT("Unable to write modified firmware!"));
		}
		f_wr.Write(m_Firmware.GetFirmwareBuffer(), m_Firmware.GetFirmwareSize());
		f_wr.Close();

		theApp.m_SavingFirmware=FALSE;
		
		SetWindowText(title);
		MessageBox(TEXT("Firmware file modified successfully!\nWarning: According to Apple's EULA, uploading firmware to 3rd party websites is prohibited! Use this firmware for personal use only!"), TEXT("Success"));
	}
	else if (m_FirmMode==3)
	{
		if (MessageBox(TEXT("Are you sure you want to write the modified firmware to the file?"), TEXT("Warning"), MB_YESNO) != IDYES)
			return;

		//CString filename;
		//filename.Format(TEXT("firmware_file.bin"));

		//CFileDialog dlg2(FALSE, TEXT("bin"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("Binary Files (*.bin)|*.bin||"), this);

		//if (dlg2.DoModal() != IDOK)
		//	return;

		CString title, title_new;
		GetWindowText(title);
		title_new.Format(TEXT("%s - Writing firmware, please wait..."), title);
		SetWindowText(title_new);

		if (m_Firmware.GetNumOTFFonts()>0)
		{
			COTFFont m_Font;
			for (DWORD i=0;i<m_Firmware.GetNumOTFFonts();i++)
			{
				if (!m_Font.Read(m_Firmware.GetOTFFont(i), FALSE))
				{
					MessageBox(TEXT("One of the OTF fonts is damages and therefor iPodWizard cannot write the update as it may cause damage to iPod.\nPlease reload the firmware and make the changes again."));
					return;
				}
				m_Font.SyncChecksums();
			}
		}

		theApp.m_SavingFirmware=TRUE;

		m_Firmware.SyncChecksum();

		UpdateChecksums();

		///////
		CZipArchive ipsw2;
		ipsw2.Open(m_iPSWFile, CZipArchive::open);
		/*
		CFile fw;
		BOOL bRet = fw.Open(_T("C:\\Documents and Settings\\Administrator\\Application Data\\Apple Computer\\iTunes\\iPod Software Updates\\Firmware-5.4.2.1"), CFile::modeRead | CFile::shareDenyWrite);
		if (!bRet)
			return;
		
		DWORD iRead;
		DWORD nBufSize = 65535;
		CAutoBuffer buf(nBufSize);
		do
		{
			iRead = fw.Read(buf, nBufSize);
			if (iRead)
				ipsw2.WriteNewFile(buf, iRead);
		}
		while (iRead == buf.GetSize());*/
		CFileHeader finfo;
		if (!ipsw2.OpenFile(m_iPSW_FID))
			return;
		if (!ipsw2.GetFileInfo(finfo, m_iPSW_FID))
		{
			//error
		}
		ipsw2.CloseFile();

		LPTSTR lpszInput=finfo.m_szFileName.GetBuffer( finfo.m_szFileName.GetLength() );
		int nLen=MultiByteToWideChar(CP_ACP, 0, (LPCSTR)lpszInput, -1, NULL, NULL);
		LPWSTR lpwStr=new WCHAR[nLen*2];
		nLen=MultiByteToWideChar(CP_ACP, 0, (LPCSTR)lpszInput, -1, lpwStr, nLen);
		finfo.m_szFileName.ReleaseBuffer();
		lpwStr[nLen/2]=0;
		CString filen(lpwStr);
		
		DWORD nBufSize = 65535;

		CFile f_wr;
		CString papp;
		GetAppPath(papp);
		papp.AppendFormat(_T("\\%s"), filen);
		delete lpwStr;
		if (!f_wr.Open(papp, CFile::modeCreate | CFile::modeWrite))
		{
			//error
			SetWindowText(title);
			MessageBox(TEXT("Unable to write modified firmware!"));
		}
		f_wr.Write(m_Firmware.GetFirmwareBuffer(), m_Firmware.GetFirmwareSize());
		f_wr.Close();
	
		if (!ipsw2.DeleteFileW(m_iPSW_FID))
		{
			//error
		}
		//_T("C:\\Documents and Settings\\Administrator\\Application Data\\Apple Computer\\iTunes\\iPod Software Updates\\Firmware-5.4.2.1")
		if (!ipsw2.AddNewFile(papp))
		{
			//error
		}
		DeleteFile(papp);
		
		ipsw2.Close();
		//////

		/*
		CFile f_wr;
		if (!f_wr.Open(dlg2.GetPathName(), CFile::modeCreate | CFile::modeWrite))
		{
			//error
			SetWindowText(title);
			MessageBox(TEXT("Unable to write modified firmware!"));
		}
		f_wr.Write(m_Firmware.GetFirmwareBuffer(), m_Firmware.GetFirmwareSize());
		f_wr.Close();*/

		theApp.m_SavingFirmware=FALSE;
		
		SetWindowText(title);
		MessageBox(TEXT("Firmware file modified successfully!\nWarning: According to Apple's EULA, uploading firmware to 3rd party websites is prohibited! Use this firmware for personal use only!"), TEXT("Success"));
	}
}

void CiPodWizardDlg::UpdateChecksums()
{
	DWORD checksum1, checksum2;

	m_FirmwareList.DeleteAllItems();
	CString s;

	for (int i = 0; i < m_Firmware.GetNumPartitions(); i++)
	{
		s.Format(TEXT("%d"), i);
		m_FirmwareList.InsertItem(i, s);

		m_Firmware.GetPartitionChecksums(i, &checksum1, &checksum2);

		s.Format(TEXT("0x%X"), checksum1);
		m_FirmwareList.SetItemText(i, 1, s);
		s.Format(TEXT("0x%X"), checksum2);
		m_FirmwareList.SetItemText(i, 2, s);

		if (checksum1 == checksum2)
		{
			s = "Ok";
		}
		else
		{
			s = "Read-only";
		}
		m_FirmwareList.SetItemText(i, 3, s);
	}
}

void CiPodWizardDlg::OnBnClickedAbout()
{
	CAboutDlg dlg;

	/*CFile f1,f2;
	f1.Open(_T("E:\\1.avi"), CFile::modeWrite);
	f2.Open(_T("E:\\2.avi"), CFile::modeRead);
	f1.SeekToEnd();
	UINT size=(UINT)f2.GetLength();
	unsigned char *buf=new unsigned char[size];
	f2.Read(buf, size);
	f1.Write(buf, size);
	f1.Close();
	f2.Close();*/

	dlg.DoModal();
}

void CiPodWizardDlg::OnBnClickedLoadipodfwButton()
{
	if (CheckiPod(FALSE)==-1)
		return;

	theApp.m_BootPicChanged=FALSE;

	TCHAR devstring[25];
	wsprintf (devstring, TEXT("%s"), theApp.m_DeviceSel);

	theApp.InitSectorSize(devstring);

	int dev = _wopen (devstring, O_RDONLY | _O_RAW);
	if (dev==-1)
	{
		MessageBox(TEXT("Unable to access iPod! Make sure programs who use the iPod like iTunes are closed."));
		return;
	}

	m_FirmwareFileName.Empty();
	m_FirmwareiPSWFileName.Empty();

ReadStart:
	theApp.InitHDDValues(dev);

	DWORD size=0;
	LPBYTE partitions;
	long pos=theApp.FIRMWARE_START + theApp.PARTITION_MAP_ADDRESS;
	pos%=theApp.BLOCK_SIZE;
	_lseek(dev, theApp.FIRMWARE_START + theApp.PARTITION_MAP_ADDRESS - pos, SEEK_SET);
	partitions = new BYTE[theApp.BLOCK_SIZE*20];
	_read(dev, partitions, theApp.BLOCK_SIZE*20);
	//theApp.CalcPartMapAddr(partitions);
	IPOD_PARTITION_HEADER *	m_pParts;
	int num=0;
	m_pParts = (IPOD_PARTITION_HEADER *)(&partitions[pos]);
	//CFile f;
	//f.Open(_T("C:\\b.bin"), CFile::modeCreate | CFile::modeWrite);
	//f.Write(partitions, theApp.BLOCK_SIZE*20);
	//f.Close();
	//TCHAR aa[500];
	//wsprintf(aa, _T("%d %d %d"), theApp.FIRMWARE_START, theApp.PARTITION_MAP_ADDRESS, theApp.BLOCK_SIZE);
	//wsprintf(aa, _T("%s"), m_pParts[num].type);
	//wsprintf(aa, _T("%d"), b);
	//MessageBox(aa, _T("num"), 0);
	//MessageBoxA(0, m_pParts[num].type, "hi", 0);
	while (memcmp(m_pParts[num].type, ata, 4) == 0 || memcmp(m_pParts[num].type, nand, 4) == 0)
	{
		if (m_pParts[num].devOffset+m_pParts[num].len > size)
			size=m_pParts[num].devOffset+m_pParts[num].len;
		num++;
	}
	delete[] partitions;

	if (num==0)
	{
		if (theApp.BLOCK_SIZE==512)
		{
			theApp.BLOCK_SIZE=2048;
			goto ReadStart;
		}
		_close(dev);
		MessageBox(_T("Error finding firmware images"));
		return;
	}
	
	LPBYTE buffer;
	DWORD i=0;
	buffer=NULL;
	for (i=0;i<size;i+=theApp.BLOCK_SIZE);
	size=i+theApp.BLOCK_SIZE; //extra read
	buffer = new BYTE[size+1];
	if (buffer==NULL)
	{
		_close(dev);
		MessageBox(_T("Couldn't allocate memory!!!"));
		return;
	}
	memset(buffer, 0, size);
	long retpos=_lseek(dev, theApp.FIRMWARE_START, SEEK_SET);
	if (retpos!=theApp.FIRMWARE_START)
	{
		_close(dev);
		MessageBox(_T("Couldn't seek for data!!!"));
		return;
	}

	CScanDialog dlg;
	dlg.Create(dlg.IDD, this);

	dlg.SendMessage(WM_APP, (WPARAM)_T("Reading iPod firmware from disk"), 0);
	dlg.m_ProgressCtrl.SetRange32(0, size);
	dlg.m_ProgressCtrl.SetPos(0);
	//for (i=0;i<size;i+=theApp.BLOCK_SIZE)
	//{
	//	_read(dev, &buffer[i], theApp.BLOCK_SIZE);
	//	if (i%2500==0)
	//		dlg.m_ProgressCtrl.SetPos(i);
	//}
	DWORD szret=_pdread(dev, buffer, size, theApp.BLOCK_SIZE);
	if (szret!=size)
	{
		if (theApp.BLOCK_SIZE==512)
		{
			theApp.BLOCK_SIZE=2048;
			goto ReadStart;
		}
		else
		{
			WCHAR cmsg[200];
			wsprintf(cmsg, _T("Error reading firmware from iPod, error value: %d, %d, %d\nTry rebooting your PC or restoring the iPod (warning: restoring deletes all the iPod contents!)"), szret, errno, size);
			MessageBox(cmsg);
			return;
		}
	}

	theApp.m_LoadingFirmware=TRUE;

	CString name = _T("iPod Firmware");
	if (!m_Firmware.SetFirmware(name, buffer, size))
	{
		delete[] buffer;
		MessageBox(TEXT("Unable to allocate memory!"));
		return;
	}

	delete[] buffer;
	_close(dev);

	UpdateChecksums();

	dlg.ScanFirmware(&m_Firmware);
	dlg.SetWindowTextW(_T("Loading iPodWizard screens..."));

	m_PrefsDialog.SetFirmware(&m_Firmware);
	m_ThemesDialog.SetFirmware(&m_Firmware, &m_EditorDialog);
	m_EditorDialog.SetFirmware(&m_Firmware);

	dlg.DestroyWindow();
	
	m_FirmMode=1;
	GetDlgItem(IDC_WRITE_FIRMWARE_BUTTON)->EnableWindow(TRUE);

	theApp.m_LoadingFirmware=FALSE;
}

void CiPodWizardDlg::OnCbnSelchangeModeCombo()
{
	switch(m_EditModeCombo.GetCurSel())
	{
	case 0: //updater
		GetDlgItem(IDC_LOADIPODFW_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(ID_OPEN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC)->SetWindowTextW(_T("Firmware:"));
		GetDlgItem(IDC_FIRMWARE_COMBO)->ShowWindow(SW_SHOW);
		GetDlgItem(ID_LOAD_FIRMWARE)->ShowWindow(SW_SHOW);
		GetDlgItem(ID_OPEN_FFILE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_WRITE2IPOD_FIRMWARE_BUTTON)->ShowWindow(SW_HIDE);
		break;
	case 1: //ipod
		GetDlgItem(ID_OPEN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FIRMWARE_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(ID_LOAD_FIRMWARE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LOADIPODFW_BUTTON)->ShowWindow(SW_SHOW);
		GetDlgItem(ID_OPEN_FFILE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_WRITE2IPOD_FIRMWARE_BUTTON)->ShowWindow(SW_HIDE);
		break;
	case 2: //firmware file
		GetDlgItem(ID_OPEN_FFILE)->SetWindowTextW(_T("Open Firmware"));
		GetDlgItem(IDC_LOADIPODFW_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(ID_OPEN)->ShowWindow(SW_HIDE);
		GetDlgItem(ID_OPEN_FFILE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC)->SetWindowTextW(m_FirmwareFileName);
		GetDlgItem(IDC_FIRMWARE_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(ID_LOAD_FIRMWARE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_WRITE2IPOD_FIRMWARE_BUTTON)->ShowWindow(SW_SHOW);
		break;
	case 3: //ipsw file
		GetDlgItem(ID_OPEN_FFILE)->SetWindowTextW(_T("Open iPSW"));
		GetDlgItem(IDC_LOADIPODFW_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(ID_OPEN)->ShowWindow(SW_HIDE);
		GetDlgItem(ID_OPEN_FFILE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC)->SetWindowTextW(m_FirmwareiPSWFileName);
		GetDlgItem(IDC_FIRMWARE_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(ID_LOAD_FIRMWARE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_WRITE2IPOD_FIRMWARE_BUTTON)->ShowWindow(SW_SHOW);
		break;
	}
	CWinApp *pApp = AfxGetApp();
	CString sMode;
	sMode.Format(_T("%d"), m_EditModeCombo.GetCurSel());
	pApp->WriteProfileString(_T("Main"), _T("Mode"), sMode);
}

void CiPodWizardDlg::OnBnClickedEjectButton()
{
	
	long ipodid=m_iPodHandles.GetAt(m_iPodDriveCombo.GetCurSel());
	VARIANT_BOOL lMnt;
	itdv->IsMounted(ipodid, &lMnt);
	if (lMnt==-1)
	{
		//itdv->ReleaseDevice(ipodid);
		itdv->Unmount(ipodid);
	}
	else if (lMnt==FALSE)
	{
		//itdv->RetainDevice(ipodid);
		itdv->Mount(ipodid);
	}
}

void CiPodWizardDlg::OnBnClickedOpenFfile()
{
	CScanDialog dlg;
	CFile f;

	if (m_EditModeCombo.GetCurSel()==2)
	{
		CFileDialog dlg2(TRUE, TEXT("*"), TEXT("*"), OFN_HIDEREADONLY, TEXT("Binary files (*)|*||"), this);
		MO_LOAD_FIRMWARE_PATH(dlg2)

		if (dlg2.DoModal() != IDOK)
			return;
		
		MO_SAVE_FIRMWARE_PATH(dlg2);

		theApp.m_LoadingFirmware = TRUE;
		m_FirmwareFile.SetString(dlg2.GetPathName());

		f.Open(dlg2.GetPathName(), CFile::modeRead);
		unsigned char *buf=new unsigned char[(DWORD)f.GetLength()];
		f.Read(buf, f.GetLength());
		theApp.BLOCK_SIZE = 512;
		CString name(_T("A"));

		if (!m_Firmware.SetFirmware(name, buf, (DWORD)f.GetLength()))
		{
			MessageBox(TEXT("Unable to allocate memory!"));
			theApp.m_LoadingFirmware = FALSE;
			delete buf;
			return;
		}
		delete buf;

		m_FirmwareFileName=dlg2.GetPathName().Right(dlg2.GetPathName().GetLength()-dlg2.GetPathName().ReverseFind('\\')-1);
		GetDlgItem(IDC_STATIC)->SetWindowText(m_FirmwareFileName);

		UpdateChecksums();

		dlg.Create(dlg.IDD, this);
		dlg.ScanFirmware(&m_Firmware);
		dlg.SetWindowTextW(_T("Loading iPodWizard screens..."));

		m_PrefsDialog.SetFirmware(&m_Firmware);
		m_ThemesDialog.SetFirmware(&m_Firmware, &m_EditorDialog);
		m_EditorDialog.SetFirmware(&m_Firmware);

		dlg.DestroyWindow();
		
		m_FirmMode=2;
		GetDlgItem(IDC_WRITE_FIRMWARE_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_WRITE2IPOD_FIRMWARE_BUTTON)->EnableWindow(TRUE);
	}
	else
	{
		CFileDialog dlg3(TRUE, TEXT("*.ipsw"), TEXT("*.ipsw"), OFN_HIDEREADONLY, TEXT("iPod Software files (*.ipsw)|*.ipsw||"), this);
		MO_LOAD_FIRMWARE_PATH(dlg3)

		if (dlg3.DoModal() != IDOK)
			return;
		MO_SAVE_FIRMWARE_PATH(dlg3)

		m_iPSWFile.SetString(dlg3.GetPathName());

		CZipArchive ipsw;
		ipsw.Open(m_iPSWFile, CZipArchive::openReadOnly);
		CFileHeader finfo;
		int file_id=ipsw.FindFile(_T("manifest.plist"));
		if (!ipsw.OpenFile(file_id))
		{
			MessageBox(_T("iPSW file corrupted!"));
			return;
		}
		if (!ipsw.GetFileInfo(finfo, file_id))
		{
			//error
			MessageBox(_T("iPSW file corrupted!"));
			return;
		}
		m_iPSW_FID=1-file_id;
		DWORD iRead;
		DWORD nBufSize = 65535;
		DWORD ptr=0;
		unsigned char *buf=new unsigned char[finfo.m_uUncomprSize];
		CAutoBuffer buffer(nBufSize);
		do
		{
			iRead = ipsw.ReadFile(buffer, buffer.GetSize());
			if (iRead)
			{
				memcpy(&buf[ptr], buffer, iRead);
				ptr+=iRead;
			}
		}
		while (iRead == buffer.GetSize());
		ipsw.Close();
		
		char str[] = "FamilyID</key>\r\n\t<integer>";
		char str2[] = "VisibleBuildID</key>\r\n\t<integer>";
		char *p=(char *)memmem(buf, finfo.m_uUncomprSize, str, strlen(str));
		char *p2=(char *)memmem(buf, finfo.m_uUncomprSize, str2, strlen(str2));
		int i=0;
		if (p && p2)
		{
			p+=strlen(str);
			i=0;
			while (p[i]!='<')
				i++;
			p[i]=0;
			int genid=-1;
			genid=atoi((char *)p);
			
			p2+=strlen(str2);
			i=0;
			while (p2[i]!='<')
				i++;
			p2[i]=0;
			long vbuildid=atoi(p2);
			TCHAR vbuild[20];
			wsprintf(vbuild, _T("%X"), vbuildid);

			CString final_s;
			final_s.Format(_T("  Firmware: "));
			switch (genid)
			{
			case 1:
				final_s.AppendFormat(_T("1st/2nd Generation iPod v"));
				break;
			case 2:
				final_s.AppendFormat(_T("3rd Generation iPod v"));
				break;
			case 3:
				final_s.AppendFormat(_T("iPod mini 2nd Generation v"));
				break;
			case 4:
				final_s.AppendFormat(_T("4th Generation iPod v"));
				break;
			case 5:
				final_s.AppendFormat(_T("iPod Color v"));
				break;
			case 7:
				final_s.AppendFormat(_T("iPod nano 1st Generation v"));
				break;
			case 13:
				final_s.AppendFormat(_T("5th Generation iPod v"));
				break;
			case 19:
				final_s.AppendFormat(_T("iPod nano 2nd Generation v"));
				break;
			}
			for (i=0;i<_tcslen(vbuild)-4;i++)
			{
				final_s.AppendFormat(_T("%c"), vbuild[i]);
				final_s.AppendFormat(_T("%c"), '.');
			}
			if (final_s.Right(1).Compare(_T("."))==0)
				final_s.Truncate(final_s.GetLength()-1);
			m_FirmwareiPSWFileName=final_s;
			GetDlgItem(IDC_STATIC)->SetWindowText(m_FirmwareiPSWFileName);
		}

		delete buf;
		
		
		m_FirmMode=3;
		GetDlgItem(ID_LOAD_FIRMWARE)->EnableWindow(TRUE);

		theApp.m_LoadingFirmware = TRUE;

		OnBnClickedLoadFirmware();
	}
	theApp.m_LoadingFirmware = FALSE;
}

void CiPodWizardDlg::OnBnClickedWrite2ipodFirmwareButton()
{
	if (MessageBox(TEXT("Are you sure you want to write the modified firmware to your iPod?\nBefore continuing please make sure iPodWizard is running in front and don't switch to any other program while writing the firmware to the iPod!!!"), TEXT("Warning"), MB_YESNO) != IDYES)
		return;

	if (theApp.m_BootPicChanged)
		if (theApp.m_ReflashBootImage)
			m_Firmware.ResetImageAdresses();
		else if (MessageBox(_T("You've changed the boot flash image (boot images).\nDo you want to reflash it so it updates the image upon iPod reset?\nWarning: Make sure you don't disconnect your iPod during the flash update (bar filling)!"), _T("Question"), MB_YESNO) == IDYES)
			m_Firmware.ResetImageAdresses();

	if (m_Firmware.GetNumOTFFonts()>0)
	{
		COTFFont m_Font;
		for (DWORD i=0;i<m_Firmware.GetNumOTFFonts();i++)
		{
			if (!m_Font.Read(m_Firmware.GetOTFFont(i), FALSE))
			{
				MessageBox(TEXT("One of the OTF fonts is damaged and therefore iPodWizard cannot write the update as it may cause damage to iPod.\nPlease reload the firmware and make the changes again."));
				return;
			}
			m_Font.SyncChecksums();
		}
	}

	theApp.m_SavingFirmware=TRUE;

	m_Firmware.SyncChecksum();

	UpdateChecksums();

	if (CheckiPod(FALSE)==-1)
		return;

	TCHAR devstring[25];
	wsprintf (devstring, TEXT("%s"), theApp.m_DeviceSel);
	int dev = _wopen (devstring, O_RDWR | O_RAW);
	if (dev==-1)
	{
		MessageBox(TEXT("Unable to access iPod! Make sure programs who use the iPod like iTunes are closed."));
		return;
	}

	theApp.InitHDDValues(dev);

	DWORD size=m_Firmware.GetFirmwareSize();
	LPBYTE lpBuf=m_Firmware.GetFirmwareBuffer();
	_lseek(dev, theApp.FIRMWARE_START, SEEK_SET);
	
	CScanDialog dlg;
	dlg.Create(dlg.IDD, this);

	dlg.SendMessage(WM_APP, (WPARAM)_T("Writing iPod firmware to disk"), 0);
	dlg.m_ProgressCtrl.SetRange32(0, size);
	dlg.m_ProgressCtrl.SetPos(0);
	int m_UpdatesDone=0,ret;
	DWORD i;
	
	/*for (i=0;i<size;i+=theApp.BLOCK_SIZE)
	{
		ret=_write(dev, &lpBuf[i], theApp.BLOCK_SIZE);
		if (ret==-1 && m_UpdatesDone==0)
		{
			_close(dev);
			MessageBox(TEXT("Can't write the modded firmware to your iPod.\r\nYour iPod is remained untouched.\r\nRestarting your computer may help to solve this problem."), TEXT("Error"));
			return;
		}
		else if (ret!=theApp.BLOCK_SIZE)
		{
			_close(dev);
			MessageBox(TEXT("A severe writing error occured to the iPod and the firmware might be damaged.\nIn order to fix this, you must go into disk mode (see more info on our website www.iPodWizard.net) and restore or update using original Apple updater."), TEXT("Error"));
			return;
		}
		m_UpdatesDone++;
		if (i%(theApp.BLOCK_SIZE*2000)==0)
		{
			dlg.m_ProgressCtrl.SetPos(i);
			dlg.m_ProgressCtrl.UpdateWindow();
		}
	}*/

	DWORD szret=_pdwrite(dev, lpBuf, size, theApp.BLOCK_SIZE);
	if (szret!=size)
	{
		int temp=errno;
		WCHAR cc[200];
		wsprintf(cc, _T("Error writing firmware! Code: %d, %d, %d"), szret, temp, size);
		MessageBox(cc);
		return;
	}

	dlg.m_ProgressCtrl.SetPos(size);
	dlg.m_ProgressCtrl.UpdateWindow();
	_close(dev);
	dlg.DestroyWindow();

	theApp.m_SavingFirmware=FALSE;
	GetDlgItem(IDC_WRITE2IPOD_FIRMWARE_BUTTON)->EnableWindow(FALSE);

	MessageBox(TEXT("Successfully written the modded firmware to your iPod!\r\nWarning: According to Apple's EULA, uploading firmware to 3rd party websites is prohibited! Use this firmware for personal use only!\r\nNow in order for changed to take order, you need to safe remove your iPod from your computer and the iPod will auto reset itself."));
}

void CiPodWizardDlg::OnLvnItemRightClickedFirmwareList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV->iItem==-1)
		return;
	
	HMENU hMenu = LoadMenu (NULL, MAKEINTRESOURCE (IDR_MENU1));
	HMENU hPopupMenu = GetSubMenu (hMenu, 0);
	POINT pt;
	SetMenuDefaultItem (hPopupMenu, -1, TRUE);
	GetCursorPos (&pt);
	SetForegroundWindow();
	HMENU hSel=(HMENU)TrackPopupMenu (hPopupMenu, 
	  TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, this->m_hWnd, NULL);
	if (hSel!=0)
	{
		m_Firmware.FixChecksum(pNMLV->iItem);
		UpdateChecksums();
	}
	SetForegroundWindow();
	DestroyMenu (hPopupMenu);
	DestroyMenu (hMenu);

	*pResult = 0;
}
