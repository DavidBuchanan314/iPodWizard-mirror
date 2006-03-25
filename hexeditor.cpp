// hexeditor.cpp

#include "stdafx.h"
#include "hexeditor.h"
#include "memdc.h"
#include ".\hexeditor.h"

#define HEXWND_CLASSNAME    _T("IpodHexWnd")  // Window class name

#define LINE_LENGTH 16

// CHexWnd

IMPLEMENT_DYNAMIC(CHexWnd, CWnd)
CHexWnd::CHexWnd()
{
	m_Buffer = NULL;
	m_BufferLen = 0;
	m_Offset = 0;

	m_CursorPos = 0;
	m_CursorHalf = 0;

	m_NumLines = 0;

	m_bFocus = FALSE;

	RegisterWindowClass();
}

CHexWnd::~CHexWnd()
{
}


BEGIN_MESSAGE_MAP(CHexWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

BOOL CHexWnd::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, HEXWND_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_PARENTDC; // | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = NULL; // (HBRUSH) (COLOR_WINDOW);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = HEXWND_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

// CHexWnd message handlers


void CHexWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// CMemDC pDC(&dc);
	CDC *pDC = &dc;

	CRect clientRect;
	GetClientRect(&clientRect);

	if (m_Buffer == NULL)
		return;

	CFont *pFont = CFont::FromHandle((HFONT)GetStockObject(ANSI_FIXED_FONT));
	CFont *pOldFont = pDC->SelectObject(pFont);

	int i;
	CString txt;
	BYTE b;

	DWORD offset = m_Offset * LINE_LENGTH;

	BOOL bFinished = FALSE;
	CRect textRect;
	CRect cursorRect;

	// calc char metrics
	m_CharRect = clientRect;
	m_CharRect.bottom = pDC->DrawText(TEXT("X"), -1, m_CharRect, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_CALCRECT);

	m_NumLines = clientRect.bottom / m_CharRect.bottom;
	
	while (clientRect.top < clientRect.bottom && !bFinished)
	{
		pDC->SetBkColor(GetSysColor(COLOR_WINDOW));

		textRect = clientRect;
		txt.Format(TEXT("%08X: "), offset);

		pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		DrawText(pDC, txt, &textRect);

		for (i = 0; i < LINE_LENGTH; i++)
		{
			pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
			if (offset + i >= m_BufferLen)
			{
				txt = "   ";
				DrawText(pDC, txt, &textRect);
			}
			else
			{
				if (m_pFirmware->CanWrite(offset + i, 1))
					pDC->SetTextColor(RGB(0, 0, 0));
				else
					pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));

				txt.Format(TEXT("%X"), m_Buffer[offset + i] / 0x10);
				if (m_CursorPos == offset + i && m_CursorHalf == 0 && m_bFocus)
					pDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
				else
					pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
				DrawText(pDC, txt, &textRect, &cursorRect);
				txt.Format(TEXT("%X"), m_Buffer[offset + i] & 0x0F);
				if (m_CursorPos == offset + i && m_CursorHalf == 1 && m_bFocus)
					pDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
				else
					pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
				DrawText(pDC, txt, &textRect, &cursorRect);
				txt = " ";
				pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
				DrawText(pDC, txt, &textRect);
			}
		}

		txt = " ";
		pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
		DrawText(pDC, txt, &textRect);

		for (i = 0; i < LINE_LENGTH; i++)
		{
			if (offset + i >= m_BufferLen)
				break;
			b = m_Buffer[offset + i];
			if (b < ' ') b = '.';
			txt = (TCHAR )b;

			if (m_pFirmware->CanWrite(offset + i, 1))
				pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
			else
				pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));

			if (m_CursorPos == offset + i && m_bFocus)
				pDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
			else
				pDC->SetBkColor(GetSysColor(COLOR_WINDOW));

			DrawText(pDC, txt, &textRect);
		}

		offset += LINE_LENGTH;
		if (offset >= m_BufferLen)
			break;

		clientRect.top += m_CharRect.bottom;
	}

	pDC->SelectObject(pOldFont);

	// Do not call CWnd::OnPaint() for painting messages
}

void CHexWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch (nSBCode)
	{
	case SB_TOP:
		m_Offset = 0;
		break;
	case SB_ENDSCROLL:
		break;
	case SB_LINEUP:
		if (m_Offset > 0)
			m_Offset--;
		break;
	case SB_LINEDOWN:
		m_Offset++;
		break;
	case SB_PAGEUP:
		if (m_Offset > m_NumLines)
			m_Offset -= m_NumLines;
		break;
	case SB_PAGEDOWN:
		m_Offset += m_NumLines;
		break;
	case SB_BOTTOM:
		break;
	// case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		// Get true 32-bit scroll position
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_TRACKPOS;
		GetScrollInfo(SB_VERT, &si);
		nPos = si.nTrackPos;
		//
		m_Offset = nPos;
		break;
	}

	if (m_Offset > (DWORD)(m_BufferLen / LINE_LENGTH) - m_NumLines - 1)
		m_Offset = m_BufferLen / LINE_LENGTH - m_NumLines - 1;

	SetScrollPos(SB_VERT, m_Offset);

	Invalidate();
}

void CHexWnd::SetFirmware(CFirmware *pFirmware)
{
	m_pFirmware = pFirmware;
	m_Buffer = pFirmware->GetFirmwareBuffer();
	m_BufferLen = pFirmware->GetFirmwareSize();
	m_Offset = 0;

	if (IsWindow(m_hWnd))
	{
		SCROLLINFO si;
	
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_ALL;
		si.nMin = 0;
		si.nMax = m_BufferLen / LINE_LENGTH - m_NumLines - 1;
		si.nPage = 13;
		si.nPos = 0;
		si.nTrackPos = 0;

		SetScrollInfo(SB_VERT, &si);
		Invalidate();
	}
}

void CHexWnd::SetOffset(DWORD offset)
{
	m_Offset = offset / LINE_LENGTH;

	m_CursorPos = offset;
	m_CursorHalf = 0;

	SCROLLINFO si;
	
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;
	si.nPos = m_Offset;
	SetScrollInfo(SB_VERT, &si);

	SetFocus();

	Invalidate();
}

int CHexWnd::DrawText(CDC *pDC, CString &s, LPRECT lpRect, LPRECT lpCalcRect)
{
	int ret = pDC->DrawText(s, lpRect, DT_SINGLELINE | DT_TOP | DT_LEFT);
	lpRect->left += s.GetLength() * m_CharRect.right;

	return ret;
}

void CHexWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pFirmware == NULL)
		return;

	BYTE val = 0;
	BOOL bWrite = FALSE;

	if (nChar >= '0' && nChar <= '9')
	{
		val = nChar - '0';
		bWrite = TRUE;
	}
	else if (nChar >= 'A' && nChar <= 'F')
	{
		val = nChar - 'A' + 10;
		bWrite = TRUE;
	}

	if (bWrite)
	{
		if (m_pFirmware->CanWrite(m_CursorPos, 1))
		{
			if (m_CursorHalf == 0)
				m_Buffer[m_CursorPos] = m_Buffer[m_CursorPos] & 0x0F | (val << 4);
			else
				m_Buffer[m_CursorPos] = m_Buffer[m_CursorPos] & 0xF0 | val;
			nChar = VK_RIGHT;
		}
		else
			MessageBeep(-1);
	}

	switch (nChar)
	{
	case VK_RIGHT:
		if (m_CursorHalf == 0)
			m_CursorHalf = 1;
		else
		{
			if (m_CursorPos < m_BufferLen - 1)
			{
				m_CursorHalf = 0;
				m_CursorPos++;
			}
		}
		break;
	case VK_LEFT:
		if (m_CursorHalf == 1)
			m_CursorHalf = 0;
		else
		{
			if (m_CursorPos > 0)
			{
				m_CursorHalf = 1;
				m_CursorPos--;
			}
		}
		break;
	case VK_UP:
		if (m_CursorPos >= LINE_LENGTH)
			m_CursorPos -= LINE_LENGTH;
		break;
	case VK_DOWN:
		if (m_CursorPos < m_BufferLen - LINE_LENGTH)
			m_CursorPos += LINE_LENGTH;
		break;
	case VK_PRIOR:
		if (m_CursorPos >= (DWORD)(m_NumLines * LINE_LENGTH))
			m_CursorPos -= (m_NumLines * LINE_LENGTH);
		else
			m_CursorPos = 0;
		break;
	case VK_NEXT:
		if (m_CursorPos < m_BufferLen - (m_NumLines * LINE_LENGTH))
			m_CursorPos += (m_NumLines * LINE_LENGTH);
		else
			m_CursorPos = m_BufferLen - 1;
		break;
	case VK_HOME:
		m_CursorPos = 0;
		m_CursorHalf = 0;
		break;
	case VK_END:
		m_CursorPos = m_BufferLen - 1;
		m_CursorHalf = 0;
		break;
	default:
		MessageBeep(-1);
		break;
	}

	// ensure visibility
	if (m_Offset * LINE_LENGTH > m_CursorPos)
		m_Offset = m_CursorPos / LINE_LENGTH;
	if ((m_Offset + m_NumLines) * LINE_LENGTH <= m_CursorPos)
		m_Offset = m_CursorPos / LINE_LENGTH - m_NumLines;

	// update scrollbar
	SCROLLINFO si;
	
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;
	si.nPos = m_Offset;
	SetScrollInfo(SB_VERT, &si);

	Invalidate();
}

UINT CHexWnd::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}

void CHexWnd::OnSetFocus(CWnd* pOldWnd)
{
	m_bFocus = TRUE;

	Invalidate();
}

void CHexWnd::OnKillFocus(CWnd* pNewWnd)
{
	//m_bFocus = FALSE;
	m_bFocus = TRUE;

	Invalidate();
}

void CHexWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();

	// position cursor
	m_CursorPos = (m_Offset + point.y / m_CharRect.bottom) * LINE_LENGTH;

	LONG column = point.x / m_CharRect.right - 10;
	if (column < 0)
		return;

	if (column >= LINE_LENGTH * 3) //text
	{
		column -= (LINE_LENGTH * 3 + 1); //space between hex and text
		if (column < 0 || column >= LINE_LENGTH) //user clicked in space or out of boundries
			return;

		m_CursorPos += column;
		m_CursorHalf = 0;
	}
	else //hex codes
	{
		m_CursorPos += column / 3;
		if (column % 3 == 1)
			m_CursorHalf = 1;
		else
			m_CursorHalf = 0;
	}

	Invalidate();
}

BOOL CHexWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CRect clientRect;
	GetClientRect(&clientRect);

	ScreenToClient(&pt);
	if (!clientRect.PtInRect(pt))
		return FALSE;

	int i;

	if (zDelta > 0)
	{
		for (i = 0; i < zDelta / WHEEL_DELTA; i++)
			SendMessage(WM_VSCROLL, SB_LINEUP, 0);
	}
	else
	{
		for (i = 0; i < -zDelta / WHEEL_DELTA; i++)
			SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
	}

	return TRUE;
}
