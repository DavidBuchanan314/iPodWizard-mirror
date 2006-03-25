#pragma once
#include "OTFFontParser.h"

// CFontWnd

class COTFFontWnd : public CWnd
{
	DECLARE_DYNAMIC(COTFFontWnd)

public:
	COTFFontWnd();
	virtual ~COTFFontWnd();

protected:
	DECLARE_MESSAGE_MAP()

public:
	BOOL RegisterWindowClass();
	afx_msg void OnPaint();
	COTFFont * m_pFont;
	void SetOTFFont(COTFFont *pFont);
	CPoint m_Origin;
	int m_FontLine,m_TotalLines;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	BOOL m_ZoomMode, m_Grid;
	void SetZoomMode(BOOL bZoomMode);
	void ChangeZoom(BOOL bMode);
	void UpdateDefaultOrigin();
	void SetGrid(BOOL bGridMode);
	int GetZoom();
	WORD m_Glyph;
	void SetGlyph(WORD glyph);
	BYTE m_Width, m_Height, m_Advance;
	CHAR m_BearingX, m_BearingY;
	void SetMetrics(BYTE height, BYTE width, CHAR bearingx, CHAR bearingy, BYTE advance);
};


