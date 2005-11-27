#pragma once

#include "Firmware.h"
#include "ResourceManager.h"
#include "afxwin.h"
#include "afxcmn.h"

// CUpdaterDialog dialog

class CUpdaterDialog : public CDialog
{
	DECLARE_DYNAMIC(CUpdaterDialog)

public:
	CUpdaterDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUpdaterDialog();

// Dialog Data
	enum { IDD = IDD_UPDATER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void GenerateList(CResourceManager *pRsrcMgr, CString Filename);
	CListCtrl m_FirmwareList;
	CString m_Filename;
	CFirmware *	m_pFirmware;
	CResourceManager *	m_pRsrcMgr;
	CPtrArray m_LoadedFirm;
	CStringArray m_LoadedFirmStr;
	CDWordArray m_LoadedFirmSize;
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedExtractFirmware();
	afx_msg void OnBnClickedLoadFirmware();
	afx_msg void OnBnClickedDeleteFirmware();
	afx_msg void OnBnClickedWriteAllChanges();
};