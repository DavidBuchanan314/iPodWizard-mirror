#pragma once

#include "Firmware.h"
#include "Font.h"
#include "FontWnd.h"
#include "afxwin.h"
#include "LayoutDialog.h"

static unsigned short redirectionTable[][2]={
0x0627, 0x27,
0x0628, 0x28,
0x0629, 0x29,
0x062a, 0x2a,
0x062b, 0x2b,
0x062c, 0x2c,
0x062d, 0x2d,
0x062e, 0x2e,
0x062f, 0x2f,
0x0630, 0x30,
0x0631, 0x31,
0x0632, 0x32,
0x0633, 0x33,
0x0634, 0x34,
0x0635, 0x35,
0x0636, 0x36,
0x0637, 0x37,
0x0638, 0x38,
0x0639, 0x39,
0x063a, 0x3a,
0xfefb, 0x3b,
0xfe8e, 0x3c,
0xfe92, 0x3d,
0xfe94, 0x3e,
0xfe98, 0x3f,
0x0640, 0x40,
0x0641, 0x41,
0x0642, 0x42,
0x0643, 0x43,
0x0644, 0x44,
0x0645, 0x45,
0x0646, 0x46,
0x0647, 0x47,
0x0648, 0x48,
0x0649, 0x49,
0x064a, 0x4a,
0xfe9c, 0x4b,
0xfea0, 0x4c,
0xfe9e, 0x4d,
0xfea4, 0x4e,
0xfea2, 0x4f,
0xfea8, 0x50,
0xfea6, 0x51,
0xfea9, 0x52,
0xfeab, 0x53,
0xfead, 0x54,
0xfeaf, 0x55,
0xfeb4, 0x56,
0xfeb8, 0x57,
0xfebc, 0x58,
0xfec0, 0x59,
0xfeca, 0x5a,
0xfecb, 0x5b,
0xfecc, 0x5c,
0xfece, 0x5d,
0xfecf, 0x5e,
0xfed0, 0x5f,
0xfed3, 0x60,
0xfed4, 0x61,
0xfed7, 0x62,
0xfed8, 0x63,
0xfedc, 0x64,
0xfedf, 0x65,
0xfee4, 0x66,
0xfee8, 0x67,
0xfeea, 0x68,
0xfeeb, 0x69,
0xfeec, 0x6a,
0xfef2, 0x6b,
0xfef4, 0x6c,
0xfefc, 0x6d,
0xfef0, 0x6e,
0xfe80, 0x6f,
0xfe85, 0x70,
0xfe86, 0x71,
0xfe89, 0x72,
0xfe8a, 0x73,
0xfe8c, 0x74
};

static const int redirectionTableCount = 78;

// CFontsDialog dialog

class CFontsDialog : public CDialog
{
	DECLARE_DYNAMIC(CFontsDialog)

public:
	CFontsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFontsDialog();

// Dialog Data
	enum { IDD = IDD_FONTS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_FontName;
	CString m_FontStyle;
	CIpodFont m_Font;
	CFirmware * m_pFirmware;
	CLayoutDialog * m_pLayoutDialog;
	void SetFirmware(CFirmware *pFirmware, CLayoutDialog *pLayoutDialog);
	void UpdateFont();
	afx_msg void OnBnClickedPrevPicture();
	afx_msg void OnBnClickedNextPicture();
	DWORD m_FontIndex;
	CFontWnd m_FontWnd;
	CFontWnd m_ZoomWnd;
	afx_msg void OnBnClickedSaveBitmap();
	int m_FontSize;
	afx_msg void OnBnClickedLoadBitmap();
	CComboBox m_FontIndexCombo;
	afx_msg void OnCbnSelchangeFontidxCombo();
	void LoadFullFont(CString path, DWORD size, DWORD fidx=-1);
	void LoadFullFontsDir(CString folderpath);
protected:
	virtual void OnOK();
public:
	CComboBox m_CharCombo;
	afx_msg void OnCbnSelchangeCharCombo();
	void UpdateZoomView();
	WORD m_Offset1;
	WORD m_Offset2;
	SHORT m_Width;
	SHORT m_Ident;
	afx_msg void OnBnClickedRefreshButton();
	int m_BitDepth;
	CComboBox m_GroupIndexCombo;
	WORD m_GroupLen;
	WORD m_GroupStart;
	void ReadUnicodeGroup();
	void WriteUnicodeGroup();
	afx_msg void OnCbnSelchangeGroupidxCombo();
	WORD m_GroupOffset;
	CComboBox m_CharMapCombo1;
	CComboBox m_CharMapCombo2;
	void UpdateCharMap();
	afx_msg void OnCbnSelchangeCharmapidx2Combo();
	afx_msg void OnCbnSelchangeCharmapidxCombo();
	CString m_GroupExplain;
	afx_msg void OnEnChangeOffset1Edit();
	void WriteCharMetrics();
	void ReadCharMetrics();
	afx_msg void OnEnChangeOffset2Edit();
	afx_msg void OnEnChangeWidthEdit();
	afx_msg void OnEnChangeIdentEdit();
	afx_msg void OnEnChangeGroupstartEdit();
	afx_msg void OnEnChangeGrouplenEdit();
	afx_msg void OnEnChangeGroupoffsetEdit();
	void UpdateCharCombo();
	void UpdateGroupExplanation();
	afx_msg void OnBnClickedLoadMetrics();
	afx_msg void OnBnClickedSaveMetrics();
	afx_msg void OnBnClickedMakeFont();
	afx_msg void OnBnClickedZoomIn();
	afx_msg void OnBnClickedZoomOut();
	afx_msg void OnBnClickedShowGrid();
	afx_msg void OnBnClickedCharMapExplain();
	afx_msg LRESULT OnSelectMetricsFromBitmap(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedOtfButton();
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnEnChangeFontnameEdit();
public:
	afx_msg void OnEnChangeFontstyleEdit();
public:
	afx_msg void OnEnChangeFontsizeEdit();
public:
	afx_msg void OnBnClickedMultisaveGlyph();
public:
	afx_msg void OnBnClickedLoadfontButton();
	afx_msg void OnBnClickedChangeFontColor();
	afx_msg void OnBnClickedSaveIfont();
};
