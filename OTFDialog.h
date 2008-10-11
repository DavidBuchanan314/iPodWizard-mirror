#pragma once

#include "Firmware.h"
#include "OTFFontParser.h"
#include "OTFFontWnd.h"
#include "afxwin.h"
#include "LayoutDialog.h"

// COTFDialog dialog

class COTFDialog : public CDialog
{
	DECLARE_DYNAMIC(COTFDialog)

public:
	COTFDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~COTFDialog();

// Dialog Data
	enum { IDD = IDD_OTF_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	COTFFont m_Font;
	CFirmware * m_pFirmware;
	CLayoutDialog *m_pLayoutDialog;
	void SetFirmware(CFirmware *pFirmware, CLayoutDialog *pLayoutDialog);
	void UpdateFont();
	DWORD m_FontIndex;
	COTFFontWnd m_FontWnd;
	COTFFontWnd m_ZoomWnd;
	CComboBox m_FontIndexCombo;
	CComboBox m_GlyphCombo;
	CComboBox m_CharCombo;
	CComboBox m_GroupIndexCombo;
	CString m_FontName;
	CString m_FontStyle;
	int m_FontSize;
	int m_BitDepth;
	BYTE m_Width;
	BYTE m_Height;
	CHAR m_BearingX;
	CHAR m_BearingY;
	BYTE m_Advance;
	WORD m_GroupOffset;
	WORD m_GroupEnd;
	WORD m_GroupStart;
	CString m_GroupExplain;
	WORD m_GlyphIndex;

	CHAR m_ALeft;
	BYTE m_ARight;
	BYTE m_BaseLine;
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedPrevPicture();
	afx_msg void OnBnClickedNextPicture();
	afx_msg void OnCbnSelchangeFontidxCombo();
	afx_msg void OnBnClickedRefreshButton();
	afx_msg void OnBnClickedMakeFont();
	afx_msg void OnCbnSelchangeCharCombo();
	afx_msg void OnCbnSelchangeGroupidxCombo();
	afx_msg void OnBnClickedZoomoutButton();
	afx_msg void OnBnClickedZoominButton();
	afx_msg void OnCbnSelchangeGlyphidxCombo();

	void UpdateCharCombo();
	void UpdateGroupExplanation();
	void ReadUnicodeGroup();
	void ReadCharMetrics();
	void UpdateZoomView();
	void WriteUnicodeGroup();
	void WriteCharMetrics();
	void LoadFullFont(CString path, DWORD size, DWORD fidx=-1);
	void LoadFullFontsDir(CString folderpath);
	afx_msg void OnBnClickedBmpButton();
	afx_msg void OnBnClickedLoadGlyph();
	afx_msg void OnBnClickedSaveGlyph();
	afx_msg void OnBnClickedChkgrid();
	afx_msg void OnEnChangeAdvanceEdit();
	afx_msg void OnEnChangeBearingyEdit();
	afx_msg void OnEnChangeBearingxEdit();
	afx_msg void OnEnChangeGroupstartEdit();
	afx_msg void OnEnChangeGroupendEdit();
	afx_msg void OnEnChangeGroupoffsetEdit();
	afx_msg void OnBnClickedLoadFont();
	afx_msg void OnBnClickedSaveFont();
	afx_msg void OnBnClickedMulloadGlyph();
	afx_msg LRESULT OnSelectMetricsFromBitmap(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedChangeFontColor();
	afx_msg void OnBnClickedMulsaveGlyph();
};