#pragma once

#include "Firmware.h"
#include "Picture.h"
#include "Font.h"
#include "OTFFontParser.h"
#include "StringDialog.h"
#include "afxwin.h"
#include "afxcmn.h"

// CThemesDialog dialog

class CThemesDialog : public CDialog
{
	DECLARE_DYNAMIC(CThemesDialog)

public:
	CThemesDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CThemesDialog();

// Dialog Data
	enum { IDD = IDD_THEMES_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_GraphicsList;
	CListCtrl m_FontsList;
	CString m_SCFPath;
	CComboBox m_FontIndexCombo;
	CComboBox m_OTFFontIndexCombo;
	CComboBox m_StringIndexCombo;
	CFirmware *	m_pFirmware;
	CStringDialog *	m_pStringDialog;
	virtual BOOL OnInitDialog();
	void SetFirmware(CFirmware *pFirmware, CStringDialog *pStringDialog);
	BOOL LoadPicture(CPicture *pPicture, LPCTSTR lpszFilename);
	BOOL LoadFont(CIpodFont *pFont, LPCTSTR lpszBmpFilename, LPCTSTR lpszIfmFilename);
	BOOL LoadOTFFont(COTFFont *pFont, DWORD index, LPCTSTR lpszFilename);
	void LoadTheme(CString filename);
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedMakeTheme();
	afx_msg void OnBnClickedLoadTheme();
	afx_msg void OnBnClickedLoadGraphics();
	afx_msg void OnBnClickedDeleteGraphic();
	afx_msg void OnBnClickedLoadFont();
	afx_msg void OnBnClickedDeleteFont();
	afx_msg void OnBnClickedLoadSCF();
	afx_msg void OnBnClickedClearSCF();
	afx_msg void OnBnClickedOtfChk();
	afx_msg void OnBnClickedAssociateIpw();
};