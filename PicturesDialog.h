#pragma once

#include "Firmware.h"
#include "PictureWnd.h"
#include "afxwin.h"

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
	void UpdatePicture();
	CFirmware * m_pFirmware;
	DWORD m_PictureIndex;
	int m_PictureWidth;
	int m_PictureHeight;
	int m_PictureBitdepth;
	int m_PictureID;
	void SetFirmware(CFirmware *pFirmware);
	afx_msg void OnBnClickedSaveBitmap();
	int m_PictureType;
	CComboBox m_PicIndexCombo;
	afx_msg void OnCbnSelchangePicidxCombo();
	BOOL SavePicture(CPicture *pPicture, LPCTSTR lpszFilename);
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedSaveAll();
	afx_msg void OnBnClickedLoadAll();
	BOOL LoadPicture(CPicture *pPicture, LPCTSTR lpszFilename);
};
