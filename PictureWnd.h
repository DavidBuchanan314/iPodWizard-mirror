#pragma once

#include "Picture.h"
#include "RDragDropFiles.h"

// CPictureWnd

class CPictureWnd : public CWnd
{
	DECLARE_DYNAMIC(CPictureWnd)

public:
	CPictureWnd();
	virtual ~CPictureWnd();

protected:
	DECLARE_MESSAGE_MAP()

public:
	BOOL RegisterWindowClass();
	afx_msg void OnPaint();
	CPicture * m_pPicture;
	void SetPicture(CPicture *pPicture);
	CPoint m_Origin;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


