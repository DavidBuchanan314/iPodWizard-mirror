#pragma once
#include "afxcmn.h"
#include "Firmware.h"
#include "HyperLink.h"

// CHelpDialog dialog

class CHelpDialog : public CDialog
{
	DECLARE_DYNAMIC(CHelpDialog)

public:
	CHelpDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHelpDialog();

// Dialog Data
	enum { IDD = IDD_HELP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL m_bFindGen;
	LRESULT OnClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedFindGen();
	void DisableFind();
	CStringArray m_iPodNames;
	CComboBox m_iPodCombo;
	void OnCbnSelchangeiPodCombo();
	CHyperLink	m_HyperLink;
};
