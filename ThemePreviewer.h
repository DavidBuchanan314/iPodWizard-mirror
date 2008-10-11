#ifndef AFX_THEMEPREVDIALOG_H__6D1C0A60_F616_43C6_9850_F3C0F052C0DB__INCLUDED_
#define AFX_THEMEPREVDIALOG_H__6D1C0A60_F616_43C6_9850_F3C0F052C0DB__INCLUDED_

#pragma once
#include "afxcmn.h"
#include "Firmware.h"
#include "ThemeWnd.h"
#include "EditorDialog.h"

#include <fstream>
using namespace std;

class CEditorDialog;

#define NUM_ELEM(x) ((int)(sizeof(x) / sizeof(x[0])))

struct res_size
{
	int resx,resy;
	TCHAR *res_str;
};

static struct res_size RES_SIZES[] =
{
	{320, 240, _T("320x240 - 5G")},
	{220, 176, _T("220x176 - 4G")},
	{220, 154, _T("220x154 - Photo")},
	{176, 132, _T("176x132 - Nano")},
};

// CThemePreviewDialog dialog

class CThemePreviewDialog : public CDialog
{
	DECLARE_DYNAMIC(CThemePreviewDialog)

public:
	CThemePreviewDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CThemePreviewDialog();

// Dialog Data
	enum { IDD = IDD_THEMEPREVIEWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CFirmware *m_pFirmware;
	CEditorDialog *m_pEditorDialog;
	CThemeWnd m_ThemeWnd;
	CComboBox m_ResCombo;
	CComboBox m_LayoutCombo;
	CComboBox m_MenuCombo;
public:
	virtual BOOL OnInitDialog();
	void SetFirmware(CFirmware *pFirmware, CEditorDialog *pEditorDialog);
	void UpdateDisplay();
	afx_msg void OnBnClickedMainmenuRadio();
	afx_msg void OnBnClickedNowplayingRadio();
	afx_msg void OnCbnSelchangeMenuCombo();
	afx_msg void OnCbnSelchangeLayoutCombo();
	afx_msg void OnCbnSelchangeResoultionCombo();
	afx_msg void OnBnClickedLoadscreenButton();
	afx_msg void OnBnClickedRefreshButton();
	afx_msg void OnBnClickedDrawcombineCheck();
};

#endif