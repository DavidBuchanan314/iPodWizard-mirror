// PictureWnd.cpp : implementation file
//

#include "stdafx.h"
#include "iPodWizard.h"
#include "PictureWnd.h"

#define PICTUREWND_CLASSNAME    _T("IpodPictureWnd")  // Window class name

// CPictureWnd

IMPLEMENT_DYNAMIC(CPictureWnd, CWnd)
CPictureWnd::CPictureWnd()
{
	m_pPicture = NULL;

	m_Origin = CPoint(0, 0);

	RegisterWindowClass();
}

CPictureWnd::~CPictureWnd()
{
}


BEGIN_MESSAGE_MAP(CPictureWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_CREATE()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

BOOL CPictureWnd::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, PICTUREWND_CLASSNAME, &wndcls)))
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
        wndcls.lpszClassName    = PICTUREWND_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

// CPictureWnd message handlers

int CPictureWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	DragAcceptFiles(TRUE);
	return 0;
}

void CPictureWnd::OnDropFiles(HDROP hDropInfo)
{
	rjc::RDragDropFiles myFiles (hDropInfo);
    CString buf;

	int iL = 0;
	int filesRead = 0;
	while (myFiles ()) {
		myFiles.sNextFile (buf);
		iL++;
		if (!(GetFileAttributes(buf.GetBuffer()) & FILE_ATTRIBUTE_DIRECTORY))
		{
			GetParent()->SendMessage(WM_APP, 11, (LPARAM)buf.GetBuffer());
			filesRead++;
			if (filesRead==1)
				GetParent()->SendMessage(WM_APP, 10, (LPARAM)buf.Left(buf.ReverseFind('\\')).GetBuffer());
		}
		else
			GetParent()->SendMessage(WM_APP, 10, (LPARAM)buf.GetBuffer());
	}
}

void CPictureWnd::OnPaint()
{
	if (theApp.m_LoadingFirmware==TRUE || theApp.m_SavingFirmware==TRUE)
		return;

	CPaintDC dc(this); // device context for painting
	
	CRect clientRect;
	GetClientRect(&clientRect);

	if (m_pPicture != NULL)
	{
		m_pPicture->Draw(&dc, m_Origin);
	}

	// Do not call CWnd::OnPaint() for painting messages
}

void CPictureWnd::SetPicture(CPicture *pPicture)
{
	CRect clientRect;
	GetClientRect(&clientRect);

	m_pPicture = pPicture;

	if (m_pPicture != NULL)
	{
		m_Origin = CPoint(0, 0);

		CSize size = m_pPicture->GetPictureSize();
		if (size.cx > clientRect.Width())
		{
			EnableScrollBar(SB_HORZ, ESB_ENABLE_BOTH);
			SetScrollRange(SB_HORZ, 0, size.cx - clientRect.Width());
			SetScrollPos(SB_HORZ, 0);
		}
		else
			EnableScrollBar(SB_HORZ, ESB_DISABLE_BOTH);

		if (size.cy > clientRect.Height())
		{
			EnableScrollBar(SB_VERT, ESB_ENABLE_BOTH);
			SetScrollRange(SB_VERT, 0, size.cy - clientRect.Height());
			SetScrollPos(SB_VERT, 0);
		}
		else
			EnableScrollBar(SB_VERT, ESB_DISABLE_BOTH);
	}

	Invalidate();
}

void CPictureWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	CSize size = m_pPicture->GetPictureSize();

	LONG maxx = size.cx - clientRect.Width();

	switch (nSBCode)
	{
	case SB_LEFT:
		m_Origin.x = 0;
		break;
	case SB_ENDSCROLL:
		break;
	case SB_LINELEFT:
		m_Origin.x++;
		break;
	case SB_LINERIGHT:
		m_Origin.x--;
		break;
	case SB_PAGELEFT:
		m_Origin.x += 10;
		break;
	case SB_PAGERIGHT:
		m_Origin.x -= 10;
		break;
	case SB_RIGHT:
		m_Origin.x = -maxx;
		break;
	case SB_THUMBPOSITION:
		m_Origin.x = -(LONG)nPos;
		break;
	case SB_THUMBTRACK:
		m_Origin.x = -(LONG)nPos;
		break;
	}

	if (m_Origin.x > 0) m_Origin.x = 0;
	if (m_Origin.x < -maxx) m_Origin.x = -maxx;

	SetScrollPos(SB_HORZ, -m_Origin.x);

	Invalidate();
}

void CPictureWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	CSize size = m_pPicture->GetPictureSize();

	LONG maxy = size.cy - clientRect.Height();

	switch (nSBCode)
	{
	case SB_LEFT:
		m_Origin.y = 0;
		break;
	case SB_ENDSCROLL:
		break;
	case SB_LINELEFT:
		m_Origin.y++;
		break;
	case SB_LINERIGHT:
		m_Origin.y--;
		break;
	case SB_PAGELEFT:
		m_Origin.y += 10;
		break;
	case SB_PAGERIGHT:
		m_Origin.y -= 10;
		break;
	case SB_RIGHT:
		m_Origin.y = -maxy;
		break;
	case SB_THUMBPOSITION:
		m_Origin.y = -(LONG)nPos;
		break;
	case SB_THUMBTRACK:
		m_Origin.y = -(LONG)nPos;
		break;
	}

	if (m_Origin.y > 0) m_Origin.y = 0;
	if (m_Origin.y < -maxy) m_Origin.y = -maxy;

	SetScrollPos(SB_VERT, -m_Origin.y);

	Invalidate();
}
