// iPodWizardDlg.h : header file
//

#pragma once

#include "ResourceManager.h"
#include "Firmware.h"
#include "EditorDialog.h"
#include "ThemesDialog.h"
#include "UpdaterDialog.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "DialogMinHelpBtn.h"


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
	BOOL m_iPodFirm;
	CString m_ThemeFirmware;
	CString m_ThemeFile;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	void ThemeChecker();
	DECLARE_MESSAGE_MAP()
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
	void UpdatePages();
	afx_msg void OnTcnSelchangeOptionTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedWriteFirmwareButton();
	void UpdateChecksums();
	void RefreshiPodDrives();
	CListCtrl m_FirmwareList;
	CStringArray m_iPodDevices;
	afx_msg void OnBnClickedAbout();
	afx_msg void OnBnClickedUpdateSysInfo();
	//afx_msg void OnBnClickedRefreshDrives();
	afx_msg void OnBnClickedTweaks();
	void GetAppPath(CString &app_path);
	int CheckiPod(BOOL bSilent=FALSE);
	afx_msg void OnCbnSelChangeiPodDriveCombo();
	afx_msg void OnBnClickedLoadipodfwButton();
	afx_msg void OnCbnSelchangeModeCombo();
};
