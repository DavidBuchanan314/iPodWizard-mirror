#pragma once
#include "afxcmn.h"
#include "Firmware.h"

// CScanDialog dialog

class CScanDialog : public CDialog
{
	DECLARE_DYNAMIC(CScanDialog)

public:
	CScanDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScanDialog();

// Dialog Data
	enum { IDD = IDD_SCAN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_ProgressCtrl;
	virtual BOOL OnInitDialog();
	void ScanFirmware(CFirmware *pFirmware);
	LRESULT OnScanProgress(WPARAM wParam, LPARAM lParam);
};
