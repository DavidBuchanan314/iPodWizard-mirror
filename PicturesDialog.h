#ifndef AFX_PICTURESDIALOG_H__6D1C0A60_F616_43C6_9850_F3C0F052C0DB__INCLUDED_
#define AFX_PICTURESDIALOG_H__6D1C0A60_F616_43C6_9850_F3C0F052C0DB__INCLUDED_

#pragma once

#include "Firmware.h"
#include "PictureWnd.h"
#include "LayoutDialog.h"
#include "afxwin.h"
#include "ScanDialog.h"

class CLayoutDialog;

// CPicturesDialog dialog

class CPicturesDialog : public CDialog
{
	DECLARE_DYNAMIC(CPicturesDialog)

public:
	CPicturesDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPicturesDialog();

// Dialog Data
	enum { IDD = IDD_PICTURES_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedPrevPicture();
	afx_msg void OnBnClickedNextPicture();
	afx_msg void OnBnClickedLoadBitmap();
	CPicture m_Picture;
	CPictureWnd m_PictureWnd;
	CLayoutDialog * m_pLayoutDialog;
	void UpdatePicture();
	CFirmware * m_pFirmware;
	DWORD m_PictureIndex;
	int m_PictureWidth;
	int m_PictureHeight;
	int m_PictureBitdepth;
	int m_PictureID;
	void SetFirmware(CFirmware *pFirmware, CLayoutDialog *pLayoutDialog, CWnd *pThemePreview);
	afx_msg void OnBnClickedSaveBitmap();
	int m_PictureType;
	CComboBox m_PicIndexCombo;
	afx_msg void OnCbnSelchangePicidxCombo();
	BOOL SavePicture(CPicture *pPicture, LPCTSTR lpszFilename);
	CWnd *m_pThemePreview;
protected:
	virtual void OnOK();
public:
	afx_msg LRESULT OnDropPictureFiles(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedSaveAll();
	afx_msg void OnBnClickedLoadAll();
	void LoadPictures(LPCTSTR folderPath);
	void SavePictures(LPCTSTR folderPath);
	BOOL LoadPicture(CPicture *pPicture, LPCTSTR lpszFilename);
public:
	afx_msg void OnBnClickedPasteImage();
public:
	afx_msg void OnBnClickedCopyImage();
	afx_msg void OnBnClickedFindBitmapRsrc();
	afx_msg void OnBnClickedThemePreview();
};

#endif