// ------------------------------------------------------------
//  CDialogMinHelpBtn template class
//  MFC CDialog with help button (0.02)
//  Supports WinXP styles (thanks to David Yuheng Zhao for CVisualStylesXP - yuheng_zhao@yahoo.com)
// ------------------------------------------------------------
//  DialogMinHelpBtn.h
//  Original DialogMinTrayBtn by zegzav - 2002,2003 - eMule project (http://www.emule-project.net)
//  Modded into help button - Kingstone 2005
// ------------------------------------------------------------
#pragma once
#define HTMINHELPBUTTON         65
#define SC_MINIMIZEHELP         0xE000

//bluecow/sony: moved out of class for VC 2003 compatiblity; zegzav: made extern for proper look (thanks)
extern BOOL (WINAPI *_TransparentBlt)(HDC, int, int, int, int, HDC, int, int, int, int, UINT);

template <class BASE= CDialog> class CDialogMinHelpBtn : public BASE
{
public:
    // constructor
    CDialogMinHelpBtn();
    CDialogMinHelpBtn(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
    CDialogMinHelpBtn(UINT nIDTemplate, CWnd* pParentWnd = NULL);

    // methods
    void MinHelpBtnShow();
    void MinHelpBtnHide();
	__inline BOOL MinHelpBtnIsVisible() const { return m_bMinHelpBtnVisible; }

    void MinHelpBtnEnable();
    void MinHelpBtnDisable();
	__inline BOOL MinHelpBtnIsEnabled() const { return m_bMinHelpBtnEnabled; }

	void SetWindowText(LPCTSTR lpszString);

protected:
    // messages
    virtual BOOL OnInitDialog();
    afx_msg void OnNcPaint();
    afx_msg BOOL OnNcActivate(BOOL bActive);
    afx_msg LRESULT OnNcHitTest(CPoint point);
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT _OnThemeChanged();
    DECLARE_MESSAGE_MAP()

private:
    // internal methods
    void MinHelpBtnInit();
    void MinHelpBtnDraw();
    BOOL MinHelpBtnHitTest(CPoint point) const;
    void MinHelpBtnUpdatePosAndSize();

    void MinHelpBtnSetUp();
    void MinHelpBtnSetDown();

	__inline const CPoint &MinHelpBtnGetPos() const { return m_MinHelpBtnPos; }
	__inline const CSize &MinHelpBtnGetSize() const { return m_MinHelpBtnSize; }
	__inline CRect MinHelpBtnGetRect() const { return CRect(MinHelpBtnGetPos(), MinHelpBtnGetSize()); }

    BOOL IsWindowsClassicStyle() const;
	INT GetVisualStylesXPColor();

	BOOL MinHelpBtnInitBitmap();

    // data members
    CPoint m_MinHelpBtnPos;
    CSize  m_MinHelpBtnSize;
    BOOL   m_bMinHelpBtnVisible; 
    BOOL   m_bMinHelpBtnEnabled; 
    BOOL   m_bMinHelpBtnUp;
    BOOL   m_bMinHelpBtnCapture;
    BOOL   m_bMinHelpBtnActive;
    BOOL   m_bMinHelpBtnHitTest;
    UINT_PTR m_nMinHelpBtnTimerId;
	CBitmap m_bmMinHelpBtnBitmap;
	BOOL	m_bMinHelpBtnWindowsClassicStyle;
	static const TCHAR *m_pszMinHelpBtnBmpName[];
	static int m_pszMinHelpBtnBmpId[];
};
