// hexeditor.h

#pragma once

#include "Firmware.h"
#include "iPodWizard.h"

// CHexWnd

class CHexWnd : public CWnd
{
	DECLARE_DYNAMIC(CHexWnd)

public:
	CHexWnd();
	virtual ~CHexWnd();

protected:
	DECLARE_MESSAGE_MAP()

public:
	BOOL RegisterWindowClass();
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	LPBYTE m_Buffer;
	DWORD m_BufferLen;
	CFirmware *m_pFirmware;
	DWORD m_Offset;
	void SetFirmware(CFirmware *pFirmware);
	void SetOffset(DWORD offset);
	int DrawText(CDC *pDC, CString &s, LPRECT lpRect, LPRECT lpCalcRect = NULL);
	DWORD m_CursorPos;
	int m_CursorHalf;
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	BOOL m_bFocus;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CRect m_CharRect;
	DWORD m_NumLines;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
