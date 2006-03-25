#pragma once

#include "Firmware.h"
#include "Font.h"
#include "FontWnd.h"
#include "afxwin.h"

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
	void SetFirmware(CFirmware *pFirmware);
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
};
