#pragma once

#include "Firmware.h"
#include "afxwin.h"
#include <Afxtempl.h>
#include "HexEditor.h"
#include "HScrollListBox.h"

struct STRING_CHANGE
{
	CWordArray b_text;
	DWORD offset;
};

// CStringDialog dialog

class CStringDialog : public CDialog
{
	DECLARE_DYNAMIC(CStringDialog)

public:
	CStringDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStringDialog();

// Dialog Data
	enum { IDD = IDD_STRINGS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CFirmware * m_pFirmware;
	void SetFirmware(CFirmware *pFirmware);
	CHScrollListBox m_StringList;
	CListCtrl m_LangStringList;
	CString m_FindString;
	CString m_ChangeString;
	CComboBox m_Langidx;
	DWORD m_HexAddr;
	CDWordArray sfilter_w;
	BOOL m_bSFilter;
	BOOL m_bShowAll;
	int m_Lang;
	afx_msg void OnBnClickedFindButton();
	afx_msg void OnEnChangeFindEdit();
	afx_msg void OnEnChangeChangeEdit();
	afx_msg void OnEnChangeAddrEdit();
	CHexWnd m_HexWnd;
protected:
	virtual void OnOK();
public:
	afx_msg void OnLbnSelchangeStringList();
	afx_msg void OnBnClickedFindnextButton();
	afx_msg void OnBnClickedChangeTextButton();
	afx_msg void OnBnClickedCheckTextButton();
	afx_msg void OnBnClickedSaveAllChangesButton();
	afx_msg void OnBnClickedLoadChangesButton();
	afx_msg void OnBnClickedJumpToButton();
	afx_msg void OnBnClickedStringFilterButton();
	afx_msg void OnBnClickedVolumeHackButton();
	void FindString(int start);
	BOOL LoadStringChanges(CFile *pFile);
	BOOL LoadLanguageBlock(CFile *pFile, DWORD filelen, DWORD lang=-1);
	STRING_CHANGE* templ;
	CArray<STRING_CHANGE*,STRING_CHANGE*> m_StringChanges;
	LPCTSTR GetString(DWORD lang, DWORD index);
	DWORD GetNumStrings(DWORD lang);
	DWORD GetStringOffset(DWORD index);
	afx_msg void OnCbnSelchangeLangidxCombo();
	afx_msg void OnLvnItemchangedLangstringList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedTranslate();
};
