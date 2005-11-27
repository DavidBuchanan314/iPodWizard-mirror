// ------------------------------------------------------------
//  CDialogMinHelpBtn template class
//  MFC CDialog with help button (0.02)
//  Supports WinXP styles (thanks to David Yuheng Zhao for CVisualStylesXP - yuheng_zhao@yahoo.com)
// ------------------------------------------------------------
//  DialogMinHelpBtn.hpp
//  Original DialogMinTrayBtn by zegzav - 2002,2003 - eMule project (http://www.emule-project.net)
//  Modded into help button - Kingstone 2005
// ------------------------------------------------------------
#include "stdafx.h"
#include "DialogMinHelpBtn.h"
#include "VisualStylesXP.h"
//#include "ResizableLib\ResizableDialog.h"
#include "AfxBeginMsgMapTemplate.h"
//#include "OtherFunctions.h"
#include "iPodWizard.h"

#if 0
// define this to use that source file as template
#define	TEMPLATE	template <class BASE>
#else
// define this to instantiate functions for class 'BASE' right in this CPP module
#if _MSC_VER >= 1310
#define	TEMPLATE	template <>
#else
#define	TEMPLATE
#endif
//#define BASE		CResizableDialog
#define BASE		CDialog
#endif

// ------------------------------
//  constants
// ------------------------------

#define CAPTION_BUTTONSPACE      (2)
#define CAPTION_MINHEIGHT        (8)

#define TIMERMINHELPBTN_ID       0x76617a67
#define TIMERMINHELPBTN_PERIOD   200    // ms

#define WP_HELPBUTTON WP_MINBUTTON

BEGIN_TM_PART_STATES(HLPBUTTON)
    TM_STATE(1, HLPBS, NORMAL)
    TM_STATE(2, HLPBS, HOT)
    TM_STATE(3, HLPBS, PUSHED)
    TM_STATE(4, HLPBS, DISABLED)
	// Inactive
    TM_STATE(5, HLPBS, INORMAL)	
    TM_STATE(6, HLPBS, IHOT)
    TM_STATE(7, HLPBS, IPUSHED)
    TM_STATE(8, HLPBS, IDISABLED)
END_TM_PART_STATES()

#define BMP_HELPBTN_WIDTH		(21)
#define BMP_HELPBTN_HEIGHT		(21)
#define BMP_HELPBTN_BLUE		_T("HELPBTN_BLUE")
#define BMP_HELPBTN_METALLIC	_T("HELPBTN_METALLIC")
#define BMP_HELPBTN_HOMESTEAD	_T("HELPBTN_HOMESTEAD")
#define BMP_HELPBTN_TRANSCOLOR	(RGB(255,0,255))
#define BMP_HELPBTN_CLASSIC_WIDTH		(6)
#define BMP_HELPBTN_CLASSIC_HEIGHT		(9)
#define BMP_HELPBTN_CLASSIC_TRANSCOLOR	(RGB(255,255,255))
#define BMP_HELPBTN_CLASSIC		_T("HELPBTN_CLASSIC")

TEMPLATE const TCHAR *CDialogMinHelpBtn<BASE>::m_pszMinHelpBtnBmpName[] = { BMP_HELPBTN_BLUE, BMP_HELPBTN_METALLIC, BMP_HELPBTN_HOMESTEAD, BMP_HELPBTN_CLASSIC };
TEMPLATE int CDialogMinHelpBtn<BASE>::m_pszMinHelpBtnBmpId[] = { HELPBTN_BLUE, HELPBTN_METALLIC, HELPBTN_HOMESTEAD, HELPBTN_CLASSIC };

#define VISUALSTYLESXP_DEFAULTFILE		L"LUNA.MSSTYLES"
#define VISUALSTYLESXP_BLUE				0
#define VISUALSTYLESXP_METALLIC			1
#define VISUALSTYLESXP_HOMESTEAD		2
#define VISUALSTYLESXP_NAMEBLUE			L"NORMALCOLOR"
#define VISUALSTYLESXP_NAMEMETALLIC		L"METALLIC"
#define VISUALSTYLESXP_NAMEHOMESTEAD	L"HOMESTEAD"

// _WIN32_WINNT >= 0x0501 (XP only)
#define _WM_THEMECHANGED                0x031A	
#define _ON_WM_THEMECHANGED()														\
	{	_WM_THEMECHANGED, 0, 0, 0, AfxSig_l,										\
		(AFX_PMSG)(AFX_PMSGW)														\
		(static_cast< LRESULT (AFX_MSG_CALL CWnd::*)(void) > (_OnThemeChanged))		\
	},


BEGIN_MESSAGE_MAP_TEMPLATE(TEMPLATE, CDialogMinHelpBtn<BASE>, CDialogMinHelpBtn, BASE)
    ON_WM_NCPAINT()
    ON_WM_NCACTIVATE()
    ON_WM_NCHITTEST()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_NCRBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
	_ON_WM_THEMECHANGED()
END_MESSAGE_MAP()


TEMPLATE CDialogMinHelpBtn<BASE>::CDialogMinHelpBtn() :
    m_MinHelpBtnPos(0,0), m_MinHelpBtnSize(0,0), m_bMinHelpBtnEnabled(TRUE), m_bMinHelpBtnVisible(TRUE), 
    m_bMinHelpBtnUp(TRUE), m_bMinHelpBtnCapture(FALSE), m_bMinHelpBtnActive(FALSE), m_bMinHelpBtnHitTest(FALSE)
{
    MinHelpBtnInit();
}

TEMPLATE CDialogMinHelpBtn<BASE>::CDialogMinHelpBtn(LPCTSTR lpszTemplateName, CWnd* pParentWnd) : BASE(lpszTemplateName, pParentWnd),
    m_MinHelpBtnPos(0,0), m_MinHelpBtnSize(0,0), m_bMinHelpBtnEnabled(TRUE), m_bMinHelpBtnVisible(TRUE), 
    m_bMinHelpBtnUp(TRUE), m_bMinHelpBtnCapture(FALSE), m_bMinHelpBtnActive(FALSE), m_bMinHelpBtnHitTest(FALSE)
{
    MinHelpBtnInit();
}

TEMPLATE CDialogMinHelpBtn<BASE>::CDialogMinHelpBtn(UINT nIDTemplate, CWnd* pParentWnd) : BASE(nIDTemplate, pParentWnd),
    m_MinHelpBtnPos(0,0), m_MinHelpBtnSize(0,0), m_bMinHelpBtnEnabled(TRUE), m_bMinHelpBtnVisible(TRUE), 
    m_bMinHelpBtnUp(TRUE), m_bMinHelpBtnCapture(FALSE), m_bMinHelpBtnActive(FALSE), m_bMinHelpBtnHitTest(FALSE)
{
    MinHelpBtnInit();
}

TEMPLATE void CDialogMinHelpBtn<BASE>::MinHelpBtnInit()
{
    m_nMinHelpBtnTimerId = 0;
	BOOL bBmpResult = MinHelpBtnInitBitmap();
	// - Never use the 'TransparentBlt' function under Win9x (read SDK)
	// - Load the 'MSIMG32.DLL' only, if it's really needed.
	if (bBmpResult && !_TransparentBlt)
	{
		HMODULE hMsImg32 = LoadLibrary(_T("MSIMG32.DLL"));
		if (hMsImg32)
		{
			(FARPROC &)_TransparentBlt = GetProcAddress(hMsImg32, "TransparentBlt");
			if (!_TransparentBlt)
				FreeLibrary(hMsImg32);
		}
	}
}

TEMPLATE BOOL CDialogMinHelpBtn<BASE>::OnInitDialog()
{
	BOOL bReturn = BASE::OnInitDialog();
	InitWindowStyles(this);
    m_nMinHelpBtnTimerId = SetTimer(TIMERMINHELPBTN_ID, TIMERMINHELPBTN_PERIOD, NULL);
    return bReturn;
}

TEMPLATE void CDialogMinHelpBtn<BASE>::OnNcPaint()
{
	BASE::OnNcPaint();
    MinHelpBtnUpdatePosAndSize();
    MinHelpBtnDraw();
}

TEMPLATE BOOL CDialogMinHelpBtn<BASE>::OnNcActivate(BOOL bActive)
{
    MinHelpBtnUpdatePosAndSize();
    BOOL bResult = BASE::OnNcActivate(bActive);
    m_bMinHelpBtnActive = bActive;
    MinHelpBtnDraw();
    return bResult;
}

TEMPLATE UINT CDialogMinHelpBtn<BASE>::OnNcHitTest(CPoint point)
{
    BOOL bPreviousHitTest = m_bMinHelpBtnHitTest;
    m_bMinHelpBtnHitTest = MinHelpBtnHitTest(point);
    if (!IsWindowsClassicStyle() && m_bMinHelpBtnHitTest != bPreviousHitTest)
        MinHelpBtnDraw(); // Windows XP Style (hot button)
    if (m_bMinHelpBtnHitTest)
       return HTMINHELPBUTTON;
    return BASE::OnNcHitTest(point);
}

TEMPLATE void CDialogMinHelpBtn<BASE>::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
    if ((GetStyle() & WS_DISABLED) || !MinHelpBtnIsEnabled() || !MinHelpBtnIsVisible() || !MinHelpBtnHitTest(point))
    {
        BASE::OnNcLButtonDown(nHitTest, point);
        return;
    }
    SetCapture();
    m_bMinHelpBtnCapture = TRUE;
    MinHelpBtnSetDown();
}

TEMPLATE void CDialogMinHelpBtn<BASE>::OnNcRButtonDown(UINT nHitTest, CPoint point) 
{
    if ((GetStyle() & WS_DISABLED) || !MinHelpBtnIsVisible() || !MinHelpBtnHitTest(point))
        BASE::OnNcRButtonDown(nHitTest, point);
}

TEMPLATE void CDialogMinHelpBtn<BASE>::OnMouseMove(UINT nFlags, CPoint point)
{
    if ((GetStyle() & WS_DISABLED) || !m_bMinHelpBtnCapture)
    { 
        BASE::OnMouseMove(nFlags, point);
        return;
    }

    ClientToScreen(&point);
    m_bMinHelpBtnHitTest = MinHelpBtnHitTest(point);
    if (m_bMinHelpBtnHitTest)
    {
        if (m_bMinHelpBtnUp)
            MinHelpBtnSetDown();
    }
    else
    {
        if (!m_bMinHelpBtnUp)
            MinHelpBtnSetUp();
    }
}

TEMPLATE void CDialogMinHelpBtn<BASE>::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if ((GetStyle() & WS_DISABLED) || !m_bMinHelpBtnCapture)
    {
        BASE::OnLButtonUp(nFlags, point);
        return;
    }

    ReleaseCapture();
    m_bMinHelpBtnCapture = FALSE;
    MinHelpBtnSetUp();

    ClientToScreen(&point);
    if (MinHelpBtnHitTest(point))
       SendMessage(WM_SYSCOMMAND, SC_MINIMIZEHELP, MAKELONG(point.x, point.y));
}

TEMPLATE void CDialogMinHelpBtn<BASE>::OnTimer(UINT_PTR nIDEvent)
{
    if (!IsWindowsClassicStyle() && nIDEvent == m_nMinHelpBtnTimerId)
    {
        // Visual XP Style (hot button)
        CPoint point;
        GetCursorPos(&point);
        BOOL bHitTest = MinHelpBtnHitTest(point);
        if (m_bMinHelpBtnHitTest != bHitTest)
        {
            m_bMinHelpBtnHitTest = bHitTest;
            MinHelpBtnDraw();
        }
    }
}

TEMPLATE LRESULT CDialogMinHelpBtn<BASE>::_OnThemeChanged()
{
	// BASE::OnThemeChanged();
	MinHelpBtnInitBitmap();
	return 0;
}

TEMPLATE void CDialogMinHelpBtn<BASE>::MinHelpBtnUpdatePosAndSize()
{
    DWORD dwStyle = GetStyle();
    DWORD dwExStyle = GetExStyle();

    INT cyCaption = ((dwExStyle & WS_EX_TOOLWINDOW) == 0) ? GetSystemMetrics(SM_CYCAPTION) - 1 : GetSystemMetrics(SM_CYSMCAPTION) - 1;
    if (cyCaption < CAPTION_MINHEIGHT)
		cyCaption = CAPTION_MINHEIGHT;

    CSize borderfixed(-GetSystemMetrics(SM_CXFIXEDFRAME), GetSystemMetrics(SM_CYFIXEDFRAME));
    CSize bordersize(-GetSystemMetrics(SM_CXSIZEFRAME), GetSystemMetrics(SM_CYSIZEFRAME));

    CRect rcWnd;
    GetWindowRect(&rcWnd);

	// get Windows' frame window button width/height (this may not always be a square!)
    CSize szBtn;
    szBtn.cy = cyCaption - (CAPTION_BUTTONSPACE * 2);
	if (IsWindowsClassicStyle())
		szBtn.cx = GetSystemMetrics(SM_CXSIZE) - 2;
	else
		szBtn.cx = GetSystemMetrics(SM_CXSIZE) - 4;

	// set our frame window button width/height...
	if (IsWindowsClassicStyle()){
		// ...this is same as Windows' buttons for non WinXP
		m_MinHelpBtnSize = szBtn;
	}
	else{
		// ...this is a square for WinXP
		m_MinHelpBtnSize.cx = szBtn.cy;
		m_MinHelpBtnSize.cy = szBtn.cy;
	}

	m_MinHelpBtnPos.x = rcWnd.Width() - (CAPTION_BUTTONSPACE + m_MinHelpBtnSize.cx + CAPTION_BUTTONSPACE + szBtn.cx);
    m_MinHelpBtnPos.y = CAPTION_BUTTONSPACE;

    if ((dwStyle & WS_THICKFRAME) != 0)
    {
        // resizable window
        m_MinHelpBtnPos += bordersize;
    }
    else
    {
        // fixed window
        m_MinHelpBtnPos += borderfixed;
    }

    if ( ((dwExStyle & WS_EX_TOOLWINDOW) == 0) && (((dwStyle & WS_MINIMIZEBOX) != 0) || ((dwStyle & WS_MAXIMIZEBOX) != 0)) )
    {
        if (IsWindowsClassicStyle())
            m_MinHelpBtnPos.x -= (szBtn.cx * 2) + CAPTION_BUTTONSPACE;
        else
            m_MinHelpBtnPos.x -= (szBtn.cx + CAPTION_BUTTONSPACE) * 2;
    }
}

TEMPLATE void CDialogMinHelpBtn<BASE>::MinHelpBtnShow()
{
    if (MinHelpBtnIsVisible())
       return;

    m_bMinHelpBtnVisible = TRUE;
    if (IsWindowVisible())
        RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
}

TEMPLATE void CDialogMinHelpBtn<BASE>::MinHelpBtnHide()
{
    if (!MinHelpBtnIsVisible())
       return;

    m_bMinHelpBtnVisible = FALSE;
    if (IsWindowVisible())
        RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
}

TEMPLATE void CDialogMinHelpBtn<BASE>::MinHelpBtnEnable()
{
    if (MinHelpBtnIsEnabled())
       return;

    m_bMinHelpBtnEnabled = TRUE;
    MinHelpBtnSetUp();
}

TEMPLATE void CDialogMinHelpBtn<BASE>::MinHelpBtnDisable()
{
    if (!MinHelpBtnIsEnabled())
       return;

    m_bMinHelpBtnEnabled = FALSE;
    if (m_bMinHelpBtnCapture)
    {
       ReleaseCapture();
       m_bMinHelpBtnCapture = FALSE;
    }
    MinHelpBtnSetUp();
}

TEMPLATE void CDialogMinHelpBtn<BASE>::MinHelpBtnDraw()
{
    if (!MinHelpBtnIsVisible())
       return;

    CDC *pDC= GetWindowDC();
    if (!pDC)
       return; // panic!

    if (IsWindowsClassicStyle())
    {
        CBrush black(GetSysColor(COLOR_BTNTEXT));
        CBrush gray(GetSysColor(COLOR_GRAYTEXT));
        CBrush gray2(GetSysColor(COLOR_BTNHILIGHT));

        // button
        if (m_bMinHelpBtnUp)
           pDC->DrawFrameControl(MinHelpBtnGetRect(), DFC_BUTTON, DFCS_BUTTONPUSH);
        else
           pDC->DrawFrameControl(MinHelpBtnGetRect(), DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_PUSHED);

		if (m_bmMinHelpBtnBitmap.m_hObject && _TransparentBlt)
		{
			CBitmap *pBmpOld;
			CDC dcMem;
			CRect btn = MinHelpBtnGetRect();
			if (dcMem.CreateCompatibleDC(pDC) && (pBmpOld = dcMem.SelectObject(&m_bmMinHelpBtnBitmap)) != NULL)
			{
				int ix=0,iy=0;
				if (m_bMinHelpBtnHitTest && m_bMinHelpBtnCapture)
				{
					//hot
					ix=btn.Width()/11;
					iy=btn.Height()/10;
				} //else = normal
				int w=(int)(btn.Width()*0.4);
				int h=(int)(btn.Height()*0.7);
				btn.left+=(btn.Width()-w)/2+ix;
				btn.top+=(btn.Height()-h)/2+iy;
				_TransparentBlt(pDC->m_hDC, btn.left, btn.top, w, h, dcMem.m_hDC, 0, 0, BMP_HELPBTN_CLASSIC_WIDTH, BMP_HELPBTN_CLASSIC_HEIGHT, BMP_HELPBTN_CLASSIC_TRANSCOLOR);
				dcMem.SelectObject(pBmpOld);
			}
		}
    }
	else
	{
		// VisualStylesXP
		CRect btn = MinHelpBtnGetRect();
		int iState;
		if (!m_bMinHelpBtnEnabled)
			iState = HLPBS_DISABLED;
		else if (GetStyle() & WS_DISABLED)
			iState = MINBS_NORMAL;
		else if (m_bMinHelpBtnHitTest)
			iState = (m_bMinHelpBtnCapture) ? MINBS_PUSHED : MINBS_HOT;
		else
			iState = MINBS_NORMAL;
		// inactive
		if (!m_bMinHelpBtnActive)
			iState += 4; // inactive state HELPBS_Ixxx

		if (m_bmMinHelpBtnBitmap.m_hObject && _TransparentBlt)
		{
			// known theme (bitmap)
			CBitmap *pBmpOld;
			CDC dcMem;
			if (dcMem.CreateCompatibleDC(pDC) && (pBmpOld = dcMem.SelectObject(&m_bmMinHelpBtnBitmap)) != NULL)
			{
				_TransparentBlt(pDC->m_hDC, btn.left, btn.top, btn.Width(), btn.Height(), dcMem.m_hDC, 0, BMP_HELPBTN_HEIGHT * (iState - 1), BMP_HELPBTN_WIDTH, BMP_HELPBTN_HEIGHT, BMP_HELPBTN_TRANSCOLOR);
				dcMem.SelectObject(pBmpOld);
			}
		}
		else
		{
			// unknown theme (ThemeData)
			HTHEME hTheme = g_xpStyle.OpenThemeData(m_hWnd, L"Window");
			if (hTheme)
			{
				btn.top += btn.Height() / 8;
				g_xpStyle.DrawThemeBackground(hTheme, pDC->m_hDC, WP_HELPBUTTON, iState, &btn, NULL);
				g_xpStyle.CloseThemeData(hTheme);
			}
		}
	}

    ReleaseDC(pDC);
}

TEMPLATE BOOL CDialogMinHelpBtn<BASE>::MinHelpBtnHitTest(CPoint ptScreen) const
{
	CRect rcWnd;
	GetWindowRect(&rcWnd);
	// adjust 'ptScreen' with possible RTL window layout
	CRect rcWndOrg(rcWnd);
	CPoint ptScreenOrg(ptScreen);
	if (::MapWindowPoints(HWND_DESKTOP, m_hWnd, &rcWnd.TopLeft(), 2) == 0 || 
		::MapWindowPoints(HWND_DESKTOP, m_hWnd, &ptScreen, 1) == 0)
	{
		// several bug reports about not working on NT SP6 (?). in case of any problems with
		// 'MapWindowPoints' we fall back to old code (does not work for RTL window layout though)
		rcWnd = rcWndOrg;
		ptScreen = ptScreenOrg;
	}
	ptScreen.Offset(-rcWnd.TopLeft());

    CRect rcBtn = MinHelpBtnGetRect();
    rcBtn.InflateRect(0, CAPTION_BUTTONSPACE);
    return rcBtn.PtInRect(ptScreen);
}

TEMPLATE void CDialogMinHelpBtn<BASE>::MinHelpBtnSetUp()
{
    m_bMinHelpBtnUp = TRUE;
    MinHelpBtnDraw();
}

TEMPLATE void CDialogMinHelpBtn<BASE>::MinHelpBtnSetDown()
{
    m_bMinHelpBtnUp = FALSE;
    MinHelpBtnDraw();
}

TEMPLATE BOOL CDialogMinHelpBtn<BASE>::IsWindowsClassicStyle() const
{
	return m_bMinHelpBtnWindowsClassicStyle;
}

TEMPLATE void CDialogMinHelpBtn<BASE>::SetWindowText(LPCTSTR lpszString)
{
	BASE::SetWindowText(lpszString);
	MinHelpBtnDraw();
}

TEMPLATE INT CDialogMinHelpBtn<BASE>::GetVisualStylesXPColor()
{
	if (IsWindowsClassicStyle())
		return -1;

	WCHAR szwThemeFile[MAX_PATH];
	WCHAR szwThemeColor[256];
	if (g_xpStyle.GetCurrentThemeName(szwThemeFile, MAX_PATH, szwThemeColor, 256, NULL, 0) != S_OK)
		return -12;
	WCHAR* p;
	if ((p = wcsrchr(szwThemeFile, _T('\\'))) == NULL)
		return -13;
	p++;
	if (_wcsicmp(p, VISUALSTYLESXP_DEFAULTFILE) != 0)
		return -14;
	if (_wcsicmp(szwThemeColor, VISUALSTYLESXP_NAMEBLUE) == 0)
		return VISUALSTYLESXP_BLUE;
	if (_wcsicmp(szwThemeColor, VISUALSTYLESXP_NAMEMETALLIC) == 0)
		return VISUALSTYLESXP_METALLIC;
	if (_wcsicmp(szwThemeColor, VISUALSTYLESXP_NAMEHOMESTEAD) == 0)
		return VISUALSTYLESXP_HOMESTEAD;
	return -15;
}

TEMPLATE BOOL CDialogMinHelpBtn<BASE>::MinHelpBtnInitBitmap()
{
	BOOL bActiveTheme, bAppThemed;
	bActiveTheme=g_xpStyle.IsThemeActive();
	bAppThemed=g_xpStyle.IsAppThemed();
	m_bMinHelpBtnWindowsClassicStyle = !(bActiveTheme && bAppThemed);

	INT nColor;
	m_bmMinHelpBtnBitmap.DeleteObject();
	nColor = GetVisualStylesXPColor();
	if (nColor < 0)
		nColor=3;
	//return m_bmMinHelpBtnBitmap.LoadBitmap(m_pszMinHelpBtnBmpName[nColor]);
	return m_bmMinHelpBtnBitmap.LoadBitmap(m_pszMinHelpBtnBmpId[nColor]);
}
