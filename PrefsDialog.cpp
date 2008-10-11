#include "stdafx.h"
#include "iPodWizard.h"
#include "PrefsDialog.h"
#include "HelpDialog.h"
#include "cgfiltyp.h"

IMPLEMENT_DYNAMIC(CPrefsDialog, CDialog)

CPrefsDialog::CPrefsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPrefsDialog::IDD, pParent)
{
	m_pFirmware = NULL;
}

CPrefsDialog::~CPrefsDialog()
{
}

void CPrefsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHK_SHOWTIPS, m_bTipStartup);
	DDX_Check(pDX, IDC_CHK_PICSYSTEM2, theApp.m_OldPicIndex);
	DDX_Check(pDX, IDC_CHK_METRICSYSTEM, theApp.m_OldFontMetricsSystem);
	DDX_Check(pDX, IDC_CHK_UPDATES, theApp.m_CheckUpdates);
	DDX_Check(pDX, IDC_CHK_REFLASHIMAGE, theApp.m_ReflashBootImage);
	DDX_Check(pDX, IDC_CHK_ALLSTRINGS, theApp.m_AllStringsRender);
	DDX_Check(pDX, IDC_CHK_LAYOUTDRAW, theApp.m_LayoutRender);
	DDX_Check(pDX, IDC_CHK_OTFDRAW, theApp.m_OTFRender);
}

BEGIN_MESSAGE_MAP(CPrefsDialog, CDialog)
	ON_BN_CLICKED(IDC_CHK_UPDATES, &CPrefsDialog::OnBnClickedChkUpdates)
	ON_BN_CLICKED(IDC_CHECK4UPDATES_BUTTON, &CPrefsDialog::OnBnClickedCheck4updatesButton)
	ON_BN_CLICKED(IDC_ASSOCIATE_IPW, &CPrefsDialog::OnBnClickedAssociateIpw)
	ON_BN_CLICKED(IDC_CHK_SHOWTIPS, &CPrefsDialog::OnBnClickedChkShowtips)
	ON_BN_CLICKED(IDC_CHK_REFLASHIMAGE, &CPrefsDialog::OnBnClickedChkReflashImage)
	ON_BN_CLICKED(IDC_CHK_METRICSYSTEM, &CPrefsDialog::OnBnClickedChkMetricsystem)
	ON_BN_CLICKED(IDC_CHK_PICSYSTEM2, &CPrefsDialog::OnBnClickedChkPicsystem2)
	ON_BN_CLICKED(IDC_IPODHELP_BUTTON, &CPrefsDialog::OnBnClickedIpodhelpButton)
	ON_BN_CLICKED(IDC_THEMEID_BUTTON, &CPrefsDialog::OnBnClickedThemeidButton)
	ON_BN_CLICKED(IDC_CHK_ALLSTRINGS, &CPrefsDialog::OnBnClickedChkAllstrings)
	ON_BN_CLICKED(IDC_CHK_OTFDRAW, &CPrefsDialog::OnBnClickedChkOtfdraw)
	ON_BN_CLICKED(IDC_CHK_LAYOUTDRAW, &CPrefsDialog::OnBnClickedChkLayoutdraw)
END_MESSAGE_MAP()

BOOL CPrefsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CheckDlgButton(IDC_CHK_UPDATES, theApp.m_CheckUpdates);
	CheckDlgButton(IDC_CHK_PICSYSTEM2, theApp.m_OldPicIndex);
	CheckDlgButton(IDC_CHK_METRICSYSTEM, theApp.m_OldFontMetricsSystem);
	CheckDlgButton(IDC_CHK_REFLASHIMAGE, theApp.m_ReflashBootImage);
	CheckDlgButton(IDC_CHK_SHOWTIPS, !theApp.GetProfileIntW(_T("Tip"), _T("StartUp"), FALSE));
	CheckDlgButton(IDC_CHK_OTFDRAW, theApp.m_OTFRender);
	CheckDlgButton(IDC_CHK_LAYOUTDRAW, theApp.m_LayoutRender);
	CheckDlgButton(IDC_CHK_ALLSTRINGS, theApp.m_AllStringsRender);

	return TRUE;
}

void CPrefsDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialog::OnOK();
}
void CPrefsDialog::OnBnClickedChkUpdates()
{
	UpdateData(TRUE);
	theApp.WriteProfileInt(_T("Main"), _T("CheckForUpdates"), theApp.m_CheckUpdates);
}

void CPrefsDialog::OnBnClickedCheck4updatesButton()
{
	
	CUpdateCheck checkUpdate;
	checkUpdate.Check(IDS_UPDATE);
}

void CPrefsDialog::OnBnClickedAssociateIpw()
{
	CGCFileTypeAccess TheFTA;

	// get full file path to program executable file
	TCHAR	szProgPath[MAX_PATH * 2];
	::GetModuleFileName(NULL, szProgPath, sizeof(szProgPath)/sizeof(TCHAR));

	CString csTempText;

	TheFTA.SetExtension(_TEXT("iPW"));

	// just pass file path in quotes on command line
	//csTempText  = szProgPath;
	//csTempText += _TEXT(" \"%1\"");
	csTempText.Format(_TEXT("\"%s\" -theme %s"),szProgPath,_TEXT("\"%1\""));
	TheFTA.SetShellOpenCommand(csTempText);
	TheFTA.SetDocumentShellOpenCommand(csTempText);

	TheFTA.SetDocumentClassName(_TEXT("iPodWizard.Document"));

	// use first icon in program
	csTempText  = szProgPath;
	csTempText += _TEXT(",0");
	//csTempText.Format(_TEXT("%s%s"),szProgPath,_TEXT(",0"));
	TheFTA.SetDocumentDefaultIcon(csTempText);

	// set the necessary registry entries	
	TheFTA.RegSetAllInfo();

	MessageBox(_TEXT("Associated successfully!"));
}

void CPrefsDialog::OnBnClickedChkShowtips()
{
	UpdateData(TRUE);

	theApp.WriteProfileInt(_T("Tip"), _T("StartUp"), !m_bTipStartup);
}

void CPrefsDialog::OnBnClickedChkReflashImage()
{
	UpdateData(TRUE);
	theApp.WriteProfileInt(_T("Main"), _T("ReflashBootImage"), theApp.m_ReflashBootImage);
}

void CPrefsDialog::OnBnClickedChkMetricsystem()
{
	UpdateData(TRUE);
	theApp.WriteProfileInt(_T("Main"), _T("FontMetricSystem"), theApp.m_OldFontMetricsSystem);
}

void CPrefsDialog::OnBnClickedChkPicsystem2()
{
	UpdateData(TRUE);
	theApp.WriteProfileInt(_T("Main"), _T("PicIndexSystem"), theApp.m_OldPicIndex);
}

void CPrefsDialog::OnBnClickedIpodhelpButton()
{
	CHelpDialog dlg(this);
	//if (CheckiPod(TRUE)==-1)
	//	dlg.DisableFind();
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

void CPrefsDialog::OnBnClickedThemeidButton()
{
	if (!m_pFirmware)
	{
		MessageBox(_T("Please load a firmware before making a theme id file!"));
		return;
	}

	CFileDialog dlg(FALSE, TEXT(""), TEXT("firmware-id"), OFN_HIDEREADONLY, TEXT("Firmware theme fingerprint|firmware-id||"), this);
	MO_LOAD_RESOURCES_PATH(dlg)

	if (dlg.DoModal() != IDOK)
		return;
	MO_SAVE_RESOURCES_PATH(dlg)

	CFile file;
	if (file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
	{
		DWORD fm_ver=m_pFirmware->GetFirmwareVersion();
		file.Write(&fm_ver, 4);
		file.Close();
	}
	else
	{
		MessageBox(_T("Unable to save file!"));
	}
}

void CPrefsDialog::OnBnClickedChkAllstrings()
{
	UpdateData(TRUE);
	theApp.WriteProfileInt(_T("Main"), _T("AllStringsRender"), theApp.m_AllStringsRender);
}

void CPrefsDialog::OnBnClickedChkOtfdraw()
{
	UpdateData(TRUE);
	theApp.WriteProfileInt(_T("Main"), _T("OTFRender"), theApp.m_OTFRender);
}

void CPrefsDialog::OnBnClickedChkLayoutdraw()
{
	UpdateData(TRUE);
	theApp.WriteProfileInt(_T("Main"), _T("LayoutRender"), theApp.m_LayoutRender);
}

void CPrefsDialog::SetFirmware(CFirmware *pFirmware)
{
	m_pFirmware=pFirmware;
}
