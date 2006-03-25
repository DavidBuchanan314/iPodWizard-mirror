#pragma once

#include "Firmware.h"
#include "PicturesDialog.h"
#include "FontsDialog.h"
#include "StringDialog.h"
#include "OTFDialog.h"
#include "afxwin.h"
#include "afxcmn.h"

// CEditorDialog dialog

class CEditorDialog : public CDialog
{
	DECLARE_DYNAMIC(CEditorDialog)

public:
	CEditorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditorDialog();

// Dialog Data
	enum { IDD = IDD_EDITOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTabCtrl m_PropsTab;
	void SetFirmware(CFirmware *pFirmware);
	void UpdatePages(BOOL bFonts = FALSE);
	CPicturesDialog m_PicsDialog;
	CFontsDialog m_FontsDialog;
	CStringDialog m_StringDialog;
	COTFDialog m_OTFDialog;
	BOOL bFonts,bOTFCreate;
protected:
	virtual void OnOK();
public:
	afx_msg void OnTcnSelchangePropTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnSwitchFontSystem(WPARAM wParam, LPARAM lParam);
};