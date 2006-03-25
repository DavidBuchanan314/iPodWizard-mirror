// FontWnd.cpp : implementation file
//

#include "stdafx.h"
#include "iPodWizard.h"
#include "OTFFontWnd.h"

#define FONTWND_CLASSNAME    _T("IpodOTFFontWnd")  // Window class name

int OTFADD_PIXELS=5;
int OTFZOOM_FACTOR=6;

// COTFFontWnd

IMPLEMENT_DYNAMIC(COTFFontWnd, CWnd)
COTFFontWnd::COTFFontWnd()
{
	m_pFont = NULL;
	m_ZoomMode = FALSE;
	m_Grid = FALSE;

	m_Origin = CPoint(0, 0);

	m_BearingX = 0;
	m_BearingY = 0;
	m_Width = 0;
	m_Height = 0;

	OTFZOOM_FACTOR = 6;

	RegisterWindowClass();
}

COTFFontWnd::~COTFFontWnd()
{
}

BEGIN_MESSAGE_MAP(COTFFontWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

BOOL COTFFontWnd::RegisterWindowClass()
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

// COTFFontWnd message handlers

void COTFFontWnd::ChangeZoom(BOOL bMode)
{
	if (bMode==TRUE) //Zoom In
	{
		if (OTFZOOM_FACTOR < 12)
		{
			OTFZOOM_FACTOR += 2;
			OTFADD_PIXELS += 1;
		}
	}
	else //Zoom Out
	{
		if (OTFZOOM_FACTOR > 2)
		{
			OTFZOOM_FACTOR -= 2;
			OTFADD_PIXELS -= 1;
		}
	}

	SetOTFFont(m_pFont);
}

void COTFFontWnd::UpdateDefaultOrigin()
{
	CRect clientRect;
	GetClientRect(&clientRect);
	int zf=OTFZOOM_FACTOR;
	if (m_Grid==TRUE)
		zf-=1;
	m_Origin.x=clientRect.Width()/2-m_Width*zf/2;
	m_Origin.y=clientRect.Height()/2-m_Height*zf/2;
}

void COTFFontWnd::SetGrid(BOOL bGridMode)
{
	m_Grid=bGridMode;
	Invalidate();
}

int COTFFontWnd::GetZoom()
{
	return OTFZOOM_FACTOR;
}

void COTFFontWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect clientRect;
	GetClientRect(&clientRect);

	if (m_pFont != NULL)
	{
		if (m_ZoomMode)
		{
			LONG x, y, px, py;

			int zf=OTFZOOM_FACTOR;
			if (m_Grid==TRUE)
				zf-=1;

			//m_Origin.x=clientRect.Width()/2-m_Width*zf/2;
			//m_Origin.y=clientRect.Height()/2-m_Height*zf/2;

			while (m_Origin.x%OTFZOOM_FACTOR!=0)
				m_Origin.x++;

			while (m_Origin.y%OTFZOOM_FACTOR!=0)
				m_Origin.y++;

			for (x=0;x<clientRect.Width();x++)
				for (y=0;y<clientRect.Height();y++)
				{
					px = x * OTFZOOM_FACTOR;
					py = y * OTFZOOM_FACTOR;
					dc.FillSolidRect(px, py, zf, zf, RGB(191,191,191));
				}

			for (x = 0; x < m_Width; x++)
			{
				for (y = 0; y < m_Height; y++)
				{
					COLORREF color = m_pFont->GetFontPixel(m_Glyph, CPoint(x, y));
		
					px = x * OTFZOOM_FACTOR + m_Origin.x;
					py = y * OTFZOOM_FACTOR + m_Origin.y;

					color = RGB(GetRValue(color) / 2 + 64, GetGValue(color) / 2 + 64, GetBValue(color) / 2 + 64); 

					dc.FillSolidRect(px, py, zf, zf, color);
				}
			}

			// draw markers
			CPen redPen(PS_SOLID, 0, RGB(255, 0, 0));
			CPen bluePen(PS_SOLID, 0, RGB(0, 0, 255));

			CPen *pOldPen = dc.SelectObject(&redPen);
			int marker1,marker2,hmarker;
			marker1=m_Origin.x-m_BearingX*OTFZOOM_FACTOR-1;
			marker2=m_Origin.x+m_Width*OTFZOOM_FACTOR+(m_Advance-(m_BearingX+m_Width))*OTFZOOM_FACTOR-1;
			hmarker=m_Origin.y+m_BearingY*OTFZOOM_FACTOR-1;

			dc.MoveTo(marker1, 0);
			dc.LineTo(marker1, clientRect.Height());

			dc.MoveTo(marker2, 0);
			dc.LineTo(marker2, clientRect.Height());

			dc.MoveTo(0, hmarker);
			dc.LineTo(clientRect.Width(), hmarker);

			// draw width indicator
			if (m_Width != 0)
			{
				dc.SelectObject(&bluePen);
				dc.MoveTo(m_Origin.x-1, m_Origin.y-2);
				dc.LineTo(m_Origin.x-1, m_Origin.y - OTFADD_PIXELS*2 - 1);
				dc.LineTo(m_Origin.x+m_Width*OTFZOOM_FACTOR-1, m_Origin.y - OTFADD_PIXELS*2 - 1);
				dc.LineTo(m_Origin.x+m_Width*OTFZOOM_FACTOR-1, m_Origin.y-1);
			}

			// draw height indicator
			if (m_Height != 0)
			{
				dc.SelectObject(&bluePen);
				dc.MoveTo(m_Origin.x+m_Width*OTFZOOM_FACTOR, m_Origin.y-1);
				dc.LineTo(m_Origin.x+m_Width*OTFZOOM_FACTOR+OTFADD_PIXELS*2, m_Origin.y-1);
				dc.LineTo(m_Origin.x+m_Width*OTFZOOM_FACTOR+OTFADD_PIXELS*2, m_Origin.y+m_Height*OTFZOOM_FACTOR-1);
				dc.LineTo(m_Origin.x+m_Width*OTFZOOM_FACTOR, m_Origin.y+m_Height*OTFZOOM_FACTOR-1);
			}

			dc.SelectObject(pOldPen);
		}
		else
		{
			LONG xoffset = clientRect.left;
			LONG yoffset = clientRect.top;

			BYTE width=0,height=0,advance=0;
			CHAR bearingx=0,bearingy=0;

			BYTE maxh=0;

			int z=0; //font line counter
			for (int c = 0; c < m_pFont->GetNumGlyphs(); c++)
			{
				int x, y;

				m_pFont->GetCharMetrics(c, &width, &height, &bearingx, &bearingy, &advance);

				if (height>maxh)
					maxh=height;

				if (xoffset + (width + 2) > clientRect.right)
				{
					z++;
					xoffset = clientRect.left;
					if (z>m_FontLine)
					{
						yoffset += maxh + 2;
						maxh=0;
					}
				}

				if (yoffset + height > clientRect.bottom)
					break;

				if (z>=m_FontLine)
				{
					for (y = 0; y < height; y++)
					{
						for (x = 0; x < width; x++)
						{
							COLORREF cr = m_pFont->GetFontPixel(c, CPoint(x, y));
							// if (c != RGB(255, 255, 255))
							dc.SetPixel(x + xoffset /* + ident */, y + yoffset, cr);
						}
					}
				}
				xoffset += width + 2;
			}
		}
	}

	// Do not call CWnd::OnPaint() for painting messages
}

void COTFFontWnd::SetOTFFont(COTFFont *pFont)
{
	m_pFont = pFont;

	if (m_pFont == NULL)
	{
		Invalidate();
		return;
	}

	CRect clientRect;
	GetClientRect(&clientRect);

	UpdateDefaultOrigin();

	if (!m_ZoomMode)
	{
		LONG xoffset = clientRect.left;
		int z=0,y=0;
		BYTE width=0,height=0,advance=0;
		CHAR bearingx=0,bearingy=0;
		BYTE maxh=0;

		for (int c = 0; c < m_pFont->GetNumGlyphs(); c++)
		{
			m_pFont->GetCharMetrics(c, &width, &height, &bearingx, &bearingy, &advance);

			if (height>maxh)
					maxh=height;

			if (xoffset + (width + 2) > clientRect.right)
			{
				z++;
				xoffset = clientRect.left;
				y+=maxh+2;
				maxh=0;
			}
			xoffset += width + 2;
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
		CSize size;
		size.cx = m_Width;
		size.cy = m_Height;
		size.cx *= OTFZOOM_FACTOR;
		size.cy *= OTFZOOM_FACTOR;

		if (size.cx > clientRect.Width())
		{
			EnableScrollBar(SB_HORZ, ESB_ENABLE_BOTH);
			EnableScrollBarCtrl(SB_HORZ, TRUE);
			SetScrollRange(SB_HORZ, clientRect.Width() - size.cx, size.cx - clientRect.Width());
			SetScrollPos(SB_HORZ, m_Origin.x);
		}
		else
		{
			EnableScrollBar(SB_HORZ, ESB_DISABLE_BOTH);
			EnableScrollBarCtrl(SB_HORZ, FALSE);
			//m_Origin.x = 0;
		}

		if (size.cy > clientRect.Height())
		{
			EnableScrollBar(SB_VERT, ESB_ENABLE_BOTH);
			EnableScrollBarCtrl(SB_VERT, TRUE);
			SetScrollRange(SB_VERT, clientRect.Height() - size.cy, size.cy - clientRect.Height());
			SetScrollPos(SB_VERT, m_Origin.y);
		}
		else
		{
			EnableScrollBar(SB_VERT, ESB_DISABLE_BOTH);
			EnableScrollBarCtrl(SB_VERT, FALSE);
		}
	}

	Invalidate();
}

void COTFFontWnd::SetGlyph(WORD glyph)
{
	m_Glyph = glyph;
	Invalidate();
}

void COTFFontWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	CSize size;
	size.cx = m_Width;
	size.cy = m_Height;

	size.cx *= OTFZOOM_FACTOR;
	size.cy *= OTFZOOM_FACTOR;

	LONG maxx = clientRect.Width() - size.cx;
	LONG minx = size.cx - clientRect.Width();

	SCROLLINFO si;

	switch (nSBCode)
	{
	case SB_LEFT:
		m_Origin.x = minx;
		break;
	case SB_ENDSCROLL:
		break;
	case SB_LINELEFT:
		m_Origin.x += OTFZOOM_FACTOR;
		break;
	case SB_LINERIGHT:
		m_Origin.x -= OTFZOOM_FACTOR;
		break;
	case SB_PAGELEFT:
		m_Origin.x += OTFZOOM_FACTOR*2;
		break;
	case SB_PAGERIGHT:
		m_Origin.x -= OTFZOOM_FACTOR*2;
		break;
	case SB_RIGHT:
		m_Origin.x = maxx;
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

	if (m_Origin.x > minx) m_Origin.x = minx;
	if (m_Origin.x < maxx) m_Origin.x = maxx;

	SetScrollPos(SB_HORZ, -m_Origin.x);

	Invalidate();
}

void COTFFontWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO si;

	if (m_ZoomMode)
	{
		CRect clientRect;
		GetClientRect(&clientRect);
		CSize size;
		if (m_Advance > m_Width)
			size.cx = m_Advance;
		else
			size.cx = m_Width;
		size.cy = m_Height;

		size.cx *= OTFZOOM_FACTOR;
		size.cy *= OTFZOOM_FACTOR;

		LONG miny = clientRect.Height() - size.cy;
		LONG maxy = size.cy - clientRect.Height();

		switch (nSBCode)
		{
		case SB_BOTTOM:
			m_Origin.y = miny;
			break;
		case SB_ENDSCROLL:
			break;
		case SB_LINEDOWN:
			m_Origin.y -= OTFZOOM_FACTOR;
			break;
		case SB_LINEUP:
			m_Origin.y += OTFZOOM_FACTOR;
			break;
		case SB_PAGEDOWN:
			m_Origin.y -= OTFZOOM_FACTOR*2;
			break;
		case SB_PAGEUP:
			m_Origin.y += OTFZOOM_FACTOR*2;
			break;
		case SB_TOP:
			m_Origin.y = maxy;
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

		if (m_Origin.y < miny) m_Origin.y = miny;
		if (m_Origin.y > maxy) m_Origin.y = maxy;

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

void COTFFontWnd::SetZoomMode(BOOL bZoomMode)
{
	m_ZoomMode = bZoomMode;

	if (IsWindow(m_hWnd))
		Invalidate();
}

void COTFFontWnd::SetMetrics(BYTE height, BYTE width, CHAR bearingx, CHAR bearingy, BYTE advance)
{
	m_BearingX = bearingx;
	m_BearingY = bearingy;
	m_Width = width;
	m_Height = height;
	m_Advance = advance;

	UpdateDefaultOrigin();

	SetScrollPos(SB_HORZ, -m_Origin.x);

	if (IsWindow(m_hWnd))
		Invalidate();
}
