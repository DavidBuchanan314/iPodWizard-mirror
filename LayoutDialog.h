#ifndef AFX_LAYOUTDIALOG_H__6D1C0A60_F616_43C6_9850_F3C0F052C0DB__INCLUDED_
#define AFX_LAYOUTDIALOG_H__6D1C0A60_F616_43C6_9850_F3C0F052C0DB__INCLUDED_

#pragma once

#include "Firmware.h"
#include "afxwin.h"
#include <Afxtempl.h>
#include "HexEditor.h"
#include "PicturesDialog.h"
#include "StringDialog.h"
#include "ToolTipListCtrl.h"
#include <algorithm>
#include <iterator>

class CPicturesDialog;

// CLayoutDialog dialog

class CLayoutDialog : public CDialog
{
	DECLARE_DYNAMIC(CLayoutDialog)

public:
	CLayoutDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLayoutDialog();

// Dialog Data
	enum { IDD = IDD_LAYOUT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();
	CFirmware * m_pFirmware;
	CToolTipListCtrl m_ResourceList;
	CTreeCtrl m_ResourceTree;
	CString m_FindString;
	CString m_ChangeString;
	DWORD m_HexAddr;
	CDWordArray sfilter_w;
	BOOL m_bSFilter;
	DWORD m_ValSize;
	DWORD m_ValOffset;
	int m_ResourceType;
	CHexWnd m_HexWnd;
	void Create_ViewTree(ViewRecord *view_rec, HTREEITEM hItem, vector<short> *id_list);
	bool IsNotOnList(vector<short> *id_list, short id)
	{
		for (size_t i=0;i<(*id_list).size();i++)
		{
			if ((*id_list).at(i)==id)
				return false;
		}
		return true;
	}
protected:
	virtual void OnOK();
public:
	void SaveRAWResource(CString filename, LPBYTE lpBuf, UINT nCount);
	CComboBox m_ResourceTypeIdx;
	void SetFirmware(CFirmware *pFirmware);
	afx_msg void OnBnClickedFindButton();
	afx_msg void OnEnChangeFindEdit();
	afx_msg void OnEnChangeChangeEdit();
	afx_msg void OnEnChangeAddrEdit();
	afx_msg void OnBnClickedFindnextButton();
	afx_msg void OnBnClickedChangeTextButton();
	afx_msg void OnBnClickedSaveResourceButton();
	afx_msg void OnBnClickedLoadChangesButton();
	afx_msg void OnBnClickedJumpToButton();
	afx_msg void OnBnClickedSelectorBarButton();
	void FindString(HTREEITEM hStartItem);
	bool FindResource(HTREEITEM hParentItem, bool next, HTREEITEM hMarkItem, bool *startsearch);
	LPCTSTR GetString(DWORD lang, DWORD index);
	DWORD GetNumStrings(DWORD lang);
	DWORD GetStringOffset(DWORD index);
	afx_msg void OnCbnSelchangeResourceTypeidxCombo();
	afx_msg void OnLvnItemchangedResourceList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemDblClickedResourceList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemClickedResourceList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedResourceTree(NMHDR *pNMHDR, LRESULT *pResult);
	ViewTable *ViewList;
	MenuTable *MenuList;
	TypeTable *TypeList;
	TCMDTable *TCMDList;
	void LoadAllResources(LPCTSTR folderPath, DWORD res_type, BOOL silent=FALSE);
public:
	afx_msg void OnBnClickedSaveallresourcesButton();
public:
	afx_msg void OnBnClickedLoadallresourcesButton();
};

#endif