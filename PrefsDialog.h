#pragma once

#include "Firmware.h"
#include "afxwin.h"
#include "afxcmn.h"

// CPrefsDialog dialog

class CPrefsDialog : public CDialog
{
	DECLARE_DYNAMIC(CPrefsDialog)

public:
	CPrefsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPrefsDialog();

// Dialog Data
	enum { IDD = IDD_PREFS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_Filename;
	CFirmware *	m_pFirmware;
	BOOL m_bTipStartup;
protected:
	virtual void OnOK();
public:
	void SetFirmware(CFirmware *pFirmware);
	afx_msg void OnBnClickedChkUpdates();
	afx_msg void OnBnClickedCheck4updatesButton();
	afx_msg void OnBnClickedAssociateIpw();
	afx_msg void OnBnClickedChkShowtips();
	afx_msg void OnBnClickedChkReflashImage();
	afx_msg void OnBnClickedChkMetricsystem();
	afx_msg void OnBnClickedChkPicsystem2();
	afx_msg void OnBnClickedIpodhelpButton();
	afx_msg void OnBnClickedThemeidButton();
	afx_msg void OnBnClickedChkAllstrings();
	afx_msg void OnBnClickedChkOtfdraw();
	afx_msg void OnBnClickedChkLayoutdraw();
};