#pragma once
#include "Font.h"

// CFontWnd

class CFontWnd : public CWnd
{
	DECLARE_DYNAMIC(CFontWnd)

public:
	CFontWnd();
	virtual ~CFontWnd();

protected:
	DECLARE_MESSAGE_MAP()

public:
	BOOL RegisterWindowClass();
	afx_msg void OnPaint();
	CIpodFont * m_pFont;
	void SetIpodFont(CIpodFont *pFont);
	CPoint m_Origin;
	int m_FontLine,m_TotalLines;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	BOOL m_ZoomMode, m_Grid;
	void SetZoomMode(BOOL bZoomMode);
	void ChangeZoom(BOOL bMode);
	void SetGrid(BOOL bGridMode);
	int GetZoom();
	int m_Marker1, m_Marker2, m_Width, m_Ident;
	void SetMetrics(int marker1, int marker2, int width, int ident);
};


