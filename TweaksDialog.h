#pragma once
#include "afxcmn.h"
#include "Firmware.h"

// CTweaksDialog dialog

class CTweaksDialog : public CDialog
{
	DECLARE_DYNAMIC(CTweaksDialog)

public:
	CTweaksDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTweaksDialog();

// Dialog Data
	enum { IDD = IDD_TWEAKS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	LRESULT OnClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedVolumeCap();
	int seekChaine (void);
	int isCapped(void);
	int unCap(void);
	int reCap(void);
	void InitCapping();
};
