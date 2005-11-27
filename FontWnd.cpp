// FontWnd.cpp : implementation file
//

#include "stdafx.h"
#include "iPodWizard.h"
#include "FontWnd.h"

#define FONTWND_CLASSNAME    _T("IpodFontWnd")  // Window class name

int ADD_PIXELS=5;
int ZOOM_FACTOR=8;

// CFontWnd

IMPLEMENT_DYNAMIC(CFontWnd, CWnd)
CFontWnd::CFontWnd()
{
	m_pFont = NULL;
	m_ZoomMode = FALSE;
	m_Grid = TRUE;

	m_Origin = CPoint(0, 0);

	m_Marker1 = 0;
	m_Marker2 = 0;
	m_Width = 0;
	m_Ident = 0;

	m_FontLine = 0;
	m_TotalLines = 0;

	ZOOM_FACTOR = 8;

	RegisterWindowClass();
}

CFontWnd::~CFontWnd()
{
}

BEGIN_MESSAGE_MAP(CFontWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CFontWnd::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, FONTWND_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = FONTWND_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

// CFontWnd message handlers

void CFontWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (nFlags==MK_LBUTTON && m_ZoomMode==TRUE)
	{
		LONG px=(-m_Origin.x+point.x)/ZOOM_FACTOR;
		::SendMessage(::GetParent(m_hWnd), WM_APP, (WPARAM)10, (LPARAM)px);
		//m_pFontsDialog.SelectMetricsFromBitmap(point.x/ZOOM_FACTOR+4);
	}
}

void CFontWnd::ChangeZoom(BOOL bMode)
{
	if (bMode==TRUE) //Zoom In
	{
		if (ZOOM_FACTOR < 12)
		{
			ZOOM_FACTOR += 2;
			ADD_PIXELS += 1;
		}
	}
	else //Zoom Out
	{
		if (ZOOM_FACTOR > 2)
		{
			ZOOM_FACTOR -= 2;
			ADD_PIXELS -= 1;
		}
	}
	m_Origin.x = -(m_Marker1 - 4) * ZOOM_FACTOR;
	SetIpodFont(m_pFont);
}

void CFontWnd::SetGrid(BOOL bGridMode)
{
	m_Grid=bGridMode;
	Invalidate();
}

int CFontWnd::GetZoom()
{
	return ZOOM_FACTOR;
}

void CFontWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect clientRect;
	GetClientRect(&clientRect);

	if (m_pFont != NULL)
	{
		CSize size = m_pFont->GetFontBitmapSize();

		if (m_ZoomMode)
		{
			LONG marker1, marker2;

			marker1 = m_Marker1 * ZOOM_FACTOR + m_Origin.x - 1;
			marker2 = m_Marker2 * ZOOM_FACTOR + m_Origin.x - 1;

			LONG x, y, px, py;

			int zf=ZOOM_FACTOR;
			if (m_Grid==TRUE)
				zf-=1;

			for (x = 0; x < size.cx; x++)
			{
				for (y = 0; y < size.cy; y++)
				{
					COLORREF color = m_pFont->GetFontPixel(CPoint(x, y));

					// if (ZOOM_FACTOR == 1)
					//	dc.SetPixel(x + m_Origin.x, y + m_Origin.y, color);
					// else
					px = x * ZOOM_FACTOR + m_Origin.x;
					py = y * ZOOM_FACTOR + m_Origin.y;

					if (px < marker1 || px > marker2)
					{
						color = RGB(GetRValue(color) / 2 + 64, GetGValue(color) / 2 + 64, GetBValue(color) / 2 + 64); 
					}

					dc.FillSolidRect(px, py, zf, zf, color);
				}
			}

			// draw markers
			LONG height = size.cy * ZOOM_FACTOR;
			CPen redPen(PS_SOLID, 0, RGB(255, 0, 0));
			CPen *pOldPen = dc.SelectObject(&redPen);

			if (height>clientRect.Height())
			{
				LONG maxy = height - clientRect.Height();
				if (m_Origin.y < -maxy)
					height=clientRect.Height()+(m_Origin.y+maxy); //ultimate formula ;p
			}
			CPen greenPen(PS_SOLID, 0, RGB(0, 255, 0));
			CPen bluePen(PS_SOLID, 0, RGB(0, 0, 255));

			dc.MoveTo(marker1, 0);
			dc.LineTo(marker1, height);
			dc.LineTo(marker1 - ADD_PIXELS, height + ADD_PIXELS);
			dc.LineTo(marker1 + ADD_PIXELS, height + ADD_PIXELS);
			dc.LineTo(marker1, height);

			dc.MoveTo(marker2, 0);
			dc.LineTo(marker2, height);
			dc.LineTo(marker2 - ADD_PIXELS, height + ADD_PIXELS);
			dc.LineTo(marker2 + ADD_PIXELS, height + ADD_PIXELS);
			dc.LineTo(marker2, height);

			// draw width indicator
			if (m_Width != 0)
			{
				dc.SelectObject(&greenPen);
				dc.MoveTo(marker1, height + ADD_PIXELS*2);
				dc.LineTo(marker1, height + ADD_PIXELS*3);
				dc.LineTo(marker1 + m_Width * ZOOM_FACTOR, height + ADD_PIXELS*3);
				dc.LineTo(marker1 + m_Width * ZOOM_FACTOR, height + ADD_PIXELS*2);
			}

			// draw ident indicator
			if (m_Ident != 0)
				{
				dc.SelectObject(&bluePen);
				dc.MoveTo(marker1 - 1, height + ADD_PIXELS*2);
				dc.LineTo(marker1 - 1, height + ADD_PIXELS*3);
					dc.LineTo(marker1 - m_Ident * ZOOM_FACTOR, height + ADD_PIXELS*3);
				dc.LineTo(marker1 - m_Ident * ZOOM_FACTOR, height + ADD_PIXELS*2);
			}

			dc.SelectObject(pOldPen);
		}
		else
		{
			LONG xoffset = clientRect.left;
			LONG yoffset = clientRect.top;

			WORD startOffset, endOffset, charwidth;
			SHORT width, ident;

			int z=0; //font line counter
			for (int c = 0; c < m_pFont->GetNumMetrics(); c++)
			{
				int x, y;

				m_pFont->GetCharMetrics(c, &startOffset, &endOffset, &width, &ident);

				charwidth = endOffset - startOffset;

				if (xoffset + (charwidth + 2) > clientRect.right)
				{
					z++;
					xoffset = clientRect.left;
					if (z>m_FontLine)
						yoffset += size.cy + 2;
				}

				if (z>=m_FontLine)
				{
					for (y = 0; y < size.cy; y++)
					{
						for (x = startOffset; x < endOffset; x++)
						{
							COLORREF c = m_pFont->GetFontPixel(CPoint(x, y));
							// if (c != RGB(255, 255, 255))
							dc.SetPixel(x - startOffset + xoffset /* + ident */, y + yoffset, c);
						}
					}
				}
				xoffset += charwidth + 2;
				// xoffset += width;
			}
		}
	}

	// Do not call CWnd::OnPaint() for painting messages
}

void CFontWnd::SetIpodFont(CIpodFont *pFont)
{
	m_pFont = pFont;

	if (m_pFont == NULL)
	{
		Invalidate();
		return;
	}

	CRect clientRect;
	GetClientRect(&clientRect);
	CSize size = m_pFont->GetFontBitmapSize();

	if (!m_ZoomMode)
	{
		LONG xoffset = clientRect.left;
		int z=0,y=0;
		WORD startOffset, endOffset, charwidth;
		SHORT width, ident;
		for (int c = 0; c < m_pFont->GetNumMetrics(); c++)
		{
			m_pFont->GetCharMetrics(c, &startOffset, &endOffset, &width, &ident);
			charwidth = endOffset - startOffset;
			if (xoffset + (charwidth + 2) > clientRect.right)
			{
				z++;
				xoffset = clientRect.left;
				y+=size.cy+2;
			}
			xoffset += charwidth + 2;
		}
		m_TotalLines=z;

		m_FontLine=0;
		if (y>(clientRect.bottom-clientRect.top))
		{
			EnableScrollBarCtrl(SB_VERT, TRUE);
			SCROLLINFO si;

			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;
			si.nMin = 0;
			si.nMax = z+3;
			si.nPage = 4;
			si.nPos = 0;
			si.nTrackPos = 0;

			SetScrollInfo(SB_VERT, &si);
			SetScrollPos(SB_VERT, 0);
		}
		else
			EnableScrollBarCtrl(SB_VERT, FALSE);
	}
	else
	{
		size.cx *= ZOOM_FACTOR;
		size.cy *= ZOOM_FACTOR;
		m_Origin.y = 0;

		if (size.cx > clientRect.Width())
		{
			EnableScrollBar(SB_HORZ, ESB_ENABLE_BOTH);
			EnableScrollBarCtrl(SB_HORZ, TRUE);
			SetScrollRange(SB_HORZ, 0, size.cx - clientRect.Width());
			SetScrollPos(SB_HORZ, 0);
		}
		else
		{
			EnableScrollBar(SB_HORZ, ESB_DISABLE_BOTH);
			EnableScrollBarCtrl(SB_HORZ, FALSE);
			m_Origin.x = 0;
		}

		if (size.cy > clientRect.Height())
		{
			EnableScrollBar(SB_VERT, ESB_ENABLE_BOTH);
			EnableScrollBarCtrl(SB_VERT, TRUE);
			SetScrollRange(SB_VERT, 0, size.cy - clientRect.Height() + ADD_PIXELS*4);
			SetScrollPos(SB_VERT, 0);
		}
		else
		{
			EnableScrollBar(SB_VERT, ESB_DISABLE_BOTH);
			EnableScrollBarCtrl(SB_VERT, FALSE);
		}

		SetScrollPos(SB_HORZ, -m_Origin.x);
	}

	Invalidate();
}

void CFontWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	CSize size = m_pFont->GetFontBitmapSize();

	size.cx *= ZOOM_FACTOR;
	size.cy *= ZOOM_FACTOR;

	LONG maxx = size.cx - clientRect.Width() + 4*ZOOM_FACTOR;

	SCROLLINFO si;

	switch (nSBCode)
	{
	case SB_LEFT:
		m_Origin.x = 4*ZOOM_FACTOR;
		break;
	case SB_ENDSCROLL:
		break;
	case SB_LINELEFT:
		m_Origin.x += (ZOOM_FACTOR*2);
		break;
	case SB_LINERIGHT:
		m_Origin.x -= (ZOOM_FACTOR*2);
		break;
	case SB_PAGELEFT:
		m_Origin.x += clientRect.Width();
		break;
	case SB_PAGERIGHT:
		m_Origin.x -= clientRect.Width();
		break;
	case SB_RIGHT:
		m_Origin.x = -maxx;
		break;
	case SB_THUMBPOSITION:
		// Get true 32-bit scroll position
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_TRACKPOS;
		GetScrollInfo(SB_HORZ, &si);
		nPos = si.nTrackPos;
		//
		m_Origin.x = -(LONG)nPos;
		break;
	case SB_THUMBTRACK:
		// Get true 32-bit scroll position
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_TRACKPOS;
		GetScrollInfo(SB_HORZ, &si);
		nPos = si.nTrackPos;
		//
		m_Origin.x = -(LONG)nPos;
		break;
	}

	if (m_Origin.x > 4*ZOOM_FACTOR) m_Origin.x = 4*ZOOM_FACTOR;
	if (m_Origin.x < -maxx) m_Origin.x = -maxx;

	SetScrollPos(SB_HORZ, -m_Origin.x);

	Invalidate();
}

void CFontWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	CSize size = m_pFont->GetFontBitmapSize();

	SCROLLINFO si;

	if (m_ZoomMode)
	{
		size.cx *= ZOOM_FACTOR;
		size.cy *= ZOOM_FACTOR;

		LONG maxy = size.cy - clientRect.Height() + ADD_PIXELS*4;
		//LONG maxt = maxy - ADD_PIXELS*4;

		switch (nSBCode)
		{
		case SB_BOTTOM:
			m_Origin.y = 0;
			break;
		case SB_ENDSCROLL:
			break;
		case SB_LINEDOWN:
			m_Origin.y -= ZOOM_FACTOR;
			break;
		case SB_LINEUP:
			m_Origin.y += ZOOM_FACTOR;
			break;
		case SB_PAGEDOWN:
			m_Origin.y -= ZOOM_FACTOR*3;
			break;
		case SB_PAGEUP:
			m_Origin.y += ZOOM_FACTOR*3;
			break;
		case SB_TOP:
			m_Origin.y = -maxy;
			break;
		case SB_THUMBPOSITION:
			// Get true 32-bit scroll position
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_TRACKPOS;
			GetScrollInfo(SB_VERT, &si);
			nPos = si.nTrackPos;
			//
			m_Origin.y = -(LONG)nPos;
			break;
		case SB_THUMBTRACK:
			// Get true 32-bit scroll position
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_TRACKPOS;
			GetScrollInfo(SB_VERT, &si);
			nPos = si.nTrackPos;
			//
			m_Origin.y = -(LONG)nPos;
			break;
		}

		if (m_Origin.y > 0) m_Origin.y = 0;
		if (m_Origin.y < -maxy) m_Origin.y = -maxy;

		SetScrollPos(SB_VERT, -m_Origin.y);
	}
	else //Font picture
	{
		switch (nSBCode)
		{
		case SB_BOTTOM:
			m_FontLine=0;
			break;
		case SB_ENDSCROLL:
			break;
		case SB_LINEDOWN:
			if (m_FontLine!=m_TotalLines)
				m_FontLine++;
			break;
		case SB_LINEUP:
			if (m_FontLine!=0)
				m_FontLine--;
			break;
		case SB_PAGEDOWN:
			m_FontLine += 4;
			if (m_FontLine>m_TotalLines)
				m_FontLine=m_TotalLines;
			break;
		case SB_PAGEUP:
			m_FontLine -= 4;
			if (m_FontLine<0)
				m_FontLine=0;
			break;
		case SB_TOP:
			m_FontLine = m_TotalLines;
			break;
		case SB_THUMBPOSITION:
			// Get true 32-bit scroll position
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_TRACKPOS;
			GetScrollInfo(SB_VERT, &si);
			nPos = si.nTrackPos;
			//
			m_FontLine = (LONG)nPos;
			break;
		case SB_THUMBTRACK:
			// Get true 32-bit scroll position
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_TRACKPOS;
			GetScrollInfo(SB_VERT, &si);
			nPos = si.nTrackPos;
			//
			m_FontLine = (LONG)nPos;
			break;
		}
		SetScrollPos(SB_VERT, m_FontLine);
	}

	Invalidate();
}

void CFontWnd::SetZoomMode(BOOL bZoomMode)
{
	m_ZoomMode = bZoomMode;

	if (IsWindow(m_hWnd))
		Invalidate();
}

void CFontWnd::SetMetrics(int marker1, int marker2, int width, int ident)
{
	m_Marker1 = marker1;
	m_Marker2 = marker2;
	m_Width = width;
	m_Ident = ident;

	m_Origin.x = -(m_Marker1 - 4) * ZOOM_FACTOR;

	SetScrollPos(SB_HORZ, -m_Origin.x);

	if (IsWindow(m_hWnd))
		Invalidate();
}
