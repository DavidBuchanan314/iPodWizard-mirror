#pragma once

#include "Firmware.h"
#include "Picture.h"
#include "Font.h"
#include "OTFFontParser.h"
#include "EditorDialog.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ThemePreviewer.h"

#include "..\Zip\ZipArchive.h"

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
	CListCtrl m_ResourcesList;
	CString m_SCFPath;
	CComboBox m_FontIndexCombo;
	CComboBox m_OTFFontIndexCombo;
	CComboBox m_StringIndexCombo;
	CFirmware *	m_pFirmware;
	CEditorDialog * m_pEditorDialog;
	virtual BOOL OnInitDialog();
	void SetFirmware(CFirmware *pFirmware, CEditorDialog *pEditorDialog);
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
	afx_msg void OnBnClickedTloadresourcesButton();
	afx_msg void OnBnClickedTdeleteresourceButton();
	afx_msg void OnBnClickedThemepreviewerButton();
	afx_msg void OnBnClickedExplainButton();
	afx_msg void OnBnClickedMakefullthemeButton();
};