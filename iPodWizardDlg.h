// iPodWizardDlg.h : header file
//

#pragma once 

#include "ResourceManager.h"
#include "Firmware.h"
#include "EditorDialog.h"
#include "ThemesDialog.h"
#include "UpdaterDialog.h"
#include "PrefsDialog.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "DialogMinHelpBtn.h"
//#include "MFCSink.h"
#include "..\Zip\ZipArchive.h"

BOOL CALLBACK myWinFunc(HWND handle, LPARAM lParam);
BOOL CALLBACK myChildWinFunc(HWND handle, LPARAM lParam);
// CiPodWizardDlg dialog
//class CiPodWizardDlg : public CDialog
class CiPodWizardDlg : public CDialogMinHelpBtn<CDialog>
{
protected:
	typedef CDialogMinHelpBtn<CDialog> CExDialog;
// Construction
public:
	CiPodWizardDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_IPODWIZARD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	CString	m_Filename;
	CResourceManager m_RsrcMgr;
	CFirmware m_Firmware;
	int m_FirmMode;
	CString m_ThemeFirmware;
	CString m_ThemeFile;
	CString m_iPSWFile;
	int m_iPSW_FID;
	BOOL m_COMFindiPod;
	CString m_FirmwareFile;
	CString m_FirmwareFileName; //file name for presentation purpose
	CString m_FirmwareiPSWFileName;  //file name for presentation purpose

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	void ThemeChecker();
	CZipArchive m_zip;
	DECLARE_MESSAGE_MAP()
private:
	//MFCSink *m_pSink;
	DWORD m_dwCookie;
	//DWORD m_dwCookieSink;
	DWORD m_dwCookieGlobal;
	CString GetPhysicalDiskFromDevice(LPCTSTR lpDrive, BOOL macFormatted);
private:
	void ShowTipAtStartup(void);
	BOOL m_TipFirst;
private:
	void ShowTipOfTheDay(void);
public:
	afx_msg void OnBnClickedOpen();
	void OpenUpdater(BOOL firstTime);
	CComboBox m_EditModeCombo;
	CComboBox m_FirmwareCombo;
	CComboBox m_iPodDriveCombo;
	CStringArray m_FirmwareNames; //non visible
	afx_msg void OnBnClickedLoadFirmware();
	CTabCtrl m_OptionsTab;
	CEditorDialog m_EditorDialog;
	CThemesDialog m_ThemesDialog;
	CUpdaterDialog m_UpdaterDialog;
	CPrefsDialog m_PrefsDialog;
	void UpdatePages();
	afx_msg void OnTcnSelchangeOptionTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedWriteFirmwareButton();
	void UpdateChecksums();
	void RefreshiPodDrives();
	CListCtrl m_FirmwareList;
	CStringArray m_iPodDevices;
	CDWordArray m_iPodHandles;
	afx_msg void OnBnClickedAbout();
	afx_msg void OnBnClickedUpdateSysInfo();
	//afx_msg void OnBnClickedRefreshDrives();
	afx_msg void OnBnClickedTweaks();
	void GetAppPath(CString &app_path);
	int CheckiPod(BOOL bSilent=FALSE);
	afx_msg void OnCbnSelChangeiPodDriveCombo();
	afx_msg void OnBnClickedLoadipodfwButton();
	afx_msg void OnCbnSelchangeModeCombo();
	afx_msg LRESULT OniPodEvent(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedEjectButton();
public:
	afx_msg void OnBnClickedOpenFfile();
public:
	afx_msg void OnBnClickedWrite2ipodFirmwareButton();
public:
	afx_msg void OnLvnItemRightClickedFirmwareList(NMHDR *pNMHDR, LRESULT *pResult);
};

static void *memmem(const void *haystack, size_t haystack_len, const void *needle, size_t needle_len)
{
    const char *begin;
    const char *const last_possible
	= (const char *) haystack + haystack_len - needle_len;

    if (needle_len == 0)
	/* The first occurrence of the empty string is deemed to occur at
	   the beginning of the string.  */
	return (void *) haystack;

    /* Sanity check, otherwise the loop might search through the whole
       memory.  */
    if (haystack_len < needle_len)
		return NULL;

    for (begin = (const char *) haystack; begin <= last_possible; ++begin)
	if (begin[0] == ((const char *) needle)[0] &&
	    !memcmp((const void *) &begin[1],
		    (const void *) ((const char *) needle + 1),
		    needle_len - 1))
	    return (void *) begin;

    return NULL;
}
