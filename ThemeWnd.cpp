// ThemeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "iPodWizard.h"
#include "ThemeWnd.h"

#define THEMEWND_CLASSNAME    _T("IpodThemeWnd")  // Window class name

#define TRANSPARENT_COLOR	RGB(255,255,255)

// CThemeWnd

IMPLEMENT_DYNAMIC(CThemeWnd, CWnd)
CThemeWnd::CThemeWnd()
{
	m_pFirmware = NULL;

	m_Origin = CPoint(0, 0);

	m_ResSize = CSize(320, 240);

	m_ViewRec = NULL;

	m_MenuRec = NULL;

	m_bDrawCombined = FALSE;

	RegisterWindowClass();
}

void CThemeWnd::Reset(BOOL bTextOnly)
{
	for (int i=0;i<TextRecList.size();i++)
		delete TextRecList.at(i);
	TextRecList.clear();

	if (bTextOnly)
		return;

	for (int i=0;i<PicRecList.size();i++)
		delete PicRecList.at(i);
	PicRecList.clear();

	for (int i=0;i<PicResRecList.size();i++)
		delete PicResRecList.at(i);
	PicResRecList.clear();

	for (int i=0;i<TextResRecList.size();i++)
		delete TextResRecList.at(i);
	TextResRecList.clear();

	m_ViewRec = NULL;
	m_MenuRec = NULL;
}

CThemeWnd::~CThemeWnd()
{
	for (int i=0;i<PicRecList.size();i++)
		delete PicRecList.at(i);
	PicRecList.clear();

	for (int i=0;i<TextRecList.size();i++)
		delete TextRecList.at(i);
	TextRecList.clear();
}

void CThemeWnd::DrawCombined(BOOL bDraw)
{
	m_bDrawCombined=bDraw;
}


BEGIN_MESSAGE_MAP(CThemeWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_CREATE()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

BOOL CThemeWnd::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, THEMEWND_CLASSNAME, &wndcls)))
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
        wndcls.lpszClassName    = THEMEWND_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

void CThemeWnd::SetFirmware(CFirmware *pFirmware, CEditorDialog *pEditorDialog)
{
	m_pFirmware = pFirmware;

	m_pEditorDialog = pEditorDialog;
}

// CThemeWnd message handlers

int CThemeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	DragAcceptFiles(TRUE);
	return 0;
}

void CThemeWnd::OnDropFiles(HDROP hDropInfo)
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

void CThemeWnd::OnPaint()
{
	if (theApp.m_LoadingFirmware==TRUE || theApp.m_SavingFirmware==TRUE)
		return;

	CPaintDC dc(this); // device context for painting
	
	CRect clientRect;
	GetClientRect(&clientRect);

	if (m_pFirmware != NULL)
	{
		PicTable PicFullRecList;
		TextTable TextFullRecList;

		for (int i=0;i<PicResRecList.size();i++)
			delete PicResRecList.at(i);
		PicResRecList.clear();

		for (int i=0;i<TextResRecList.size();i++)
			delete TextResRecList.at(i);
		TextResRecList.clear();

		dc.FillSolidRect(0, 0, m_ResSize.cx, m_ResSize.cy, TRANSPARENT_COLOR);

		if (m_ViewRec != NULL)
		{
			vector<short> id_list;
			DrawImages(&dc, m_ViewRec, &id_list);
		}

		copy(PicResRecList.begin(), PicResRecList.end(), back_inserter(PicFullRecList));
		copy(TextResRecList.begin(), TextResRecList.end(), back_inserter(TextFullRecList));
		copy(PicRecList.begin(), PicRecList.end(), back_inserter(PicFullRecList));
		copy(TextRecList.begin(), TextRecList.end(), back_inserter(TextFullRecList));
		
		if (PicFullRecList.size()>0 || TextFullRecList.size()>0)
		{
			//draw pictures first
			CPicture pic;
			for (int i=0;i<PicFullRecList.size();i++)
			{
				if (pic.Read(m_pFirmware->GetPicture(m_pFirmware->GetPictureIndexFromID(PicFullRecList.at(i)->id)), FALSE)==TRUE)
				{
					DWORD x,y;
					if (PicFullRecList.at(i)->rct.bottom<=0)
						PicFullRecList.at(i)->rct.bottom=pic.GetPictureSize().cy;
					if (PicFullRecList.at(i)->rct.right<=0)
						PicFullRecList.at(i)->rct.right=pic.GetPictureSize().cx;
					if (PicFullRecList.at(i)->rct.top<=0)
						PicFullRecList.at(i)->rct.top=0;
					if (PicFullRecList.at(i)->rct.left<=0)
						PicFullRecList.at(i)->rct.left=0;
					for (y = PicFullRecList.at(i)->inner.y; y < (DWORD)PicFullRecList.at(i)->rct.bottom; y++)
					{
						for (x = PicFullRecList.at(i)->inner.x; x < (DWORD)PicFullRecList.at(i)->rct.right; x++)
						{
							if (pic.GetPixel(x, y)!=TRANSPARENT_COLOR)
								dc.SetPixel(x + PicFullRecList.at(i)->rct.left, y + PicFullRecList.at(i)->rct.top, pic.GetPixel(x, y));
						}
					}
				}
			}

			//draw text
			CIpodFont fnt;
			COTFFont ofnt;
			DWORD save_yoffset=0;
			for (int i=0;i<TextFullRecList.size();i++)
			{
				//find string
				LPCTSTR lpStr=NULL;
				for (DWORD a=0;a<m_pFirmware->GetNumLangStrings(TextFullRecList.at(i)->lang);a++)
				{
					if (TextFullRecList.at(i)->id==m_pFirmware->GetLangStringID(TextFullRecList.at(i)->lang, a))
					{
						lpStr=m_pFirmware->GetLangString(TextFullRecList.at(i)->lang, a);
						break;
					}
				}
				
				if (!lpStr)
					return;
				wchar_t *sText;
				Utf8Decode((char *)lpStr, &sText);

				DWORD j;
				int k;
				bool found=false;
				resource_type_type_elem *type;
				for (j=0;j<m_pEditorDialog->m_LayoutDialog.TypeList->size();j++)
				{
					if (m_pEditorDialog->m_LayoutDialog.TypeList->at(j)->id==TextFullRecList.at(i)->fontid)
					{
						type=m_pEditorDialog->m_LayoutDialog.TypeList->at(j)->element;
						found=true;
						break;
					}
				}
				if (!found)
					return;
				
				LPBYTE lpEnd;
				lpEnd=m_pFirmware->GetFirmwareBuffer()+m_pFirmware->GetFirmwareSize();
				found=false;
				unsigned char fstyle;
				fstyle=type->style;
				bool otf=false;
				for (j=0;j<m_pFirmware->GetNumLangStrings(m_pFirmware->GetNumLangs()-1);j++)
				{
					
					if (m_pFirmware->GetLangStringID(m_pFirmware->GetNumLangs()-1, j)==type->fontID)
					{
						wchar_t *sOut;
						Utf8Decode((char *)m_pFirmware->GetLangString(m_pFirmware->GetNumLangs()-1, j), &sOut);
						unsigned char style=0;
						for (k=0;k<m_pFirmware->GetNumOTFFonts();k++)
						{
							if (ofnt.Read(m_pFirmware->GetOTFFont(k), FALSE))
							{
								if (!wcscmp(ofnt.GetFontStyle(), _T("Bold")))
									style=0;
								else if (!wcscmp(ofnt.GetFontStyle(), _T("Regular")))
									style=1;
								if (!wcscmp(ofnt.GetFontName(), sOut) && ofnt.GetFontSize()==type->size && fstyle==style)
								{
									ofnt.Read(m_pFirmware->GetOTFFont(k), TRUE);
									found=true;
									otf=true;
									break;
								}
							}
						}
						for (k=0;k<m_pFirmware->GetNumFonts();k++)
						{
							if (fnt.Read(m_pFirmware->GetFont(k), lpEnd)==TRUE)
							{
								if (!wcscmp(fnt.GetFontStyle(), _T("Bold")))
									style=1;
								else if (!wcscmp(fnt.GetFontStyle(), _T("Regular")))
									style=0;
								if (!wcscmp(fnt.GetFontName(), sOut) && fnt.GetFontSize()==type->size && fstyle==style)
								{
									found=true;
									break;
								}
							}
						}
						delete sOut;
						break;
					}
				}

				if (!found)
					return;

				float h_dif,s_dif,l_dif;
				COLORREF fcolor=(COLORREF)type->color;
				//fcolor=RGB(237, 41, 14);
				if (otf)
					fcolor=RGB(0,0,0);
				CColor test_color=fcolor;
				CColor black_color=RGB(0, 0, 0);
				h_dif=test_color.GetHue()-black_color.GetHue();
				s_dif=test_color.GetSaturation()-black_color.GetSaturation();
				l_dif=test_color.GetLuminance()-black_color.GetLuminance();

				DWORD xoffset=TextFullRecList.at(i)->inner.x;

				DWORD yoffset;
				if (TextFullRecList.at(i)->inner.y==-1)
					yoffset=save_yoffset;
				else
					yoffset=TextFullRecList.at(i)->inner.y;

				WORD offset;
				WORD unicodeChar;

				CSize size;
				WORD total_width=0;
				if (otf)
				{
					BYTE width=0,height=0,advance=0;
					CHAR bearingx=0,bearingy=0;
					BYTE max_height=0;
					for (int j=0;j<wcslen(sText);j++)
					{
						unicodeChar=(WORD)sText[j];
						offset = ofnt.GetUnicodeCharOffset(unicodeChar);

						ofnt.GetCharMetrics(offset, &width, &height, &bearingx, &bearingy, &advance);

						if (height>max_height)
							max_height=height;

						total_width+=advance;
					}

					if (TextRecList.at(i)->inner.x==-1)
					{
						switch(type->alignment)
						{
						case 0:
							xoffset=3;
							break;
						case 1:
							xoffset=m_ResSize.cx/2-total_width/2;
							break;
						case 2:
							xoffset=m_ResSize.cx-3-total_width;
							break;
						}
					}

					for (int j=0;j<wcslen(sText);j++)
					{
						unicodeChar=(WORD)sText[j];
						offset = ofnt.GetUnicodeCharOffset(unicodeChar);

						ofnt.GetCharMetrics(offset, &width, &height, &bearingx, &bearingy, &advance);

						size=ofnt.GetFontBitmapSize(offset);

						for (int p = 0; p < (int)size.cy; p++)
						{
							for (int q = 0; q < (int)size.cx; q++)
							{
								COLORREF cl = ofnt.GetFontPixel(offset, CPoint(q, p));

								CColor mcl=cl;
								
								if (cl != TRANSPARENT_COLOR)
								{
									if (mcl.GetLuminance()>test_color.GetLuminance())
									{
										mcl.SetHue(h_dif-mcl.GetHue());
										mcl.SetSaturation(s_dif-mcl.GetSaturation());
									}
									else
										mcl=fcolor;
									//float ln=l_dif-mcl.GetLuminance();
									//if (ln<0)
									//	ln*=(-1);
									//mcl.SetLuminance(ln);

									//
									//	cl = RGB(GetRValue(cl)+r_dif, GetGValue(cl)+g_dif, GetBValue(cl)+b_dif);
									dc.SetPixel(q + xoffset + bearingx, max_height - bearingy + p + yoffset, mcl);
								}
							}
						}
						xoffset+=advance;
					}
					yoffset+=max_height+8;
				}
				else
				{
					WORD c;
					DWORD offset1 = 0, offset2 = 0;
					SHORT width = 0, ident = 0;
					size=fnt.GetFontBitmapSize();
					for (int j=0;j<wcslen(sText);j++)
					{
						unicodeChar=(WORD)sText[j];
						offset=fnt.GetUnicodeCharOffset(unicodeChar);
						c=fnt.GetCharMapping(offset);
						fnt.GetCharMetrics(c, &offset1, &offset2, &width, &ident);
						total_width+=width;
					}
					if (TextRecList.at(i)->inner.x==-1)
					{
						switch(type->alignment)
						{
						case 0:
							xoffset=3;
							break;
						case 1:
							xoffset=m_ResSize.cx/2-total_width/2;
							break;
						case 2:
							xoffset=m_ResSize.cx-3-total_width;
							break;
						}
					}
					for (int j=0;j<wcslen(sText);j++)
					{
						unicodeChar=(WORD)sText[j];
						offset=fnt.GetUnicodeCharOffset(unicodeChar);
						c=fnt.GetCharMapping(offset);
						fnt.GetCharMetrics(c, &offset1, &offset2, &width, &ident);
						
						for (int p = 0; p < size.cy; p++)
						{
							for (int q = (int)offset1; q < (int)offset2; q++)
							{
								COLORREF cl = fnt.GetFontPixel(CPoint(q, p));

								CColor mcl=cl;
								
								if (cl != TRANSPARENT_COLOR)
								{
									if (mcl.GetLuminance()>test_color.GetLuminance())
									{
										mcl.SetHue(h_dif-mcl.GetHue());
										mcl.SetSaturation(s_dif-mcl.GetSaturation());
									}
									else
										mcl=fcolor;
									//float ln=l_dif-mcl.GetLuminance();
									//if (ln<0)
									//	ln*=(-1);
									//mcl.SetLuminance(ln);

									//
									//	cl = RGB(GetRValue(cl)+r_dif, GetGValue(cl)+g_dif, GetBValue(cl)+b_dif);
									dc.SetPixel(q - offset1 + xoffset + ident, p + yoffset, mcl);
								}
							}
						}
						xoffset+=width;
					}
					yoffset+=size.cy+8;
				}
				delete sText;
				save_yoffset=yoffset;
			}
		}
	
		PicFullRecList.clear();
		TextFullRecList.clear();
	}

	// Do not call CWnd::OnPaint() for painting messages
}

void CThemeWnd::DrawImages(CDC *pDC, ViewRecord *view_rec, vector<short> *id_list)
{
	if (view_rec==NULL)
		return;

	(*id_list).push_back(view_rec->id);

	CPicture pic;
	CPoint point;
	for (int i=0;i<view_rec->num_elems;i++)
	{
		if (IsNotOnList(id_list, view_rec->ViewElemList.at(i)->element->id))
		{
			if (!strncmp(view_rec->ViewElemList.at(i)->element->type, "paMB", 4)) //draw bitmap
			{
				if (pic.Read(m_pFirmware->GetPicture(m_pFirmware->GetPictureIndexFromID(view_rec->ViewElemList.at(i)->element->id)), FALSE)==TRUE)
				{
					point.SetPoint(view_rec->ViewElemList.at(i)->element->left.val, view_rec->ViewElemList.at(i)->element->top.val);
					if (m_bDrawCombined)
					{
						PicRecord *pic_rec=new PicRecord;
						pic_rec->id=view_rec->ViewElemList.at(i)->element->id;
						pic_rec->rct.left=view_rec->ViewElemList.at(i)->element->left.val;
						pic_rec->rct.top=view_rec->ViewElemList.at(i)->element->top.val;
						pic_rec->rct.bottom=view_rec->ViewElemList.at(i)->element->bottom.val;
						pic_rec->rct.right=view_rec->ViewElemList.at(i)->element->right.val;
						pic_rec->inner.SetPoint(0, 0);
						PicResRecList.push_back(pic_rec);
					}
					else
						pic.Draw(pDC, point, CSize(view_rec->ViewElemList.at(i)->element->right.val, view_rec->ViewElemList.at(i)->element->bottom.val));
					//pic.Draw(pDC, CPoint(0, 0));
				}
			}
			else if (!strncmp(view_rec->ViewElemList.at(i)->element->type, " rtS", 4)) //draw string
			{
			}
			else if (!strncmp(view_rec->ViewElemList.at(i)->element->type, "enoN", 4)) //draw string
			{
				vector<short> new_id_list;
				new_id_list.push_back(0);
				//allocate v2 to size of v1 or use back_inserter
				copy((*id_list).begin(), (*id_list).end(), back_inserter(new_id_list));
				DrawImages(pDC, view_rec->ViewElemList.at(i)->next, &new_id_list);
			}
		}
	}
}

void CThemeWnd::SetData(int view_id, int menu_id)
{
	CRect clientRect;
	GetClientRect(&clientRect);

	m_ViewRec = NULL;
	m_MenuRec = NULL;

	for (int i=0;i<m_pEditorDialog->m_LayoutDialog.ViewList->size();i++)
	{
		if (m_pEditorDialog->m_LayoutDialog.ViewList->at(i)->id==view_id)
		{
			m_ViewRec = m_pEditorDialog->m_LayoutDialog.ViewList->at(i);
		}
	}

	for (int i=0;i<m_pEditorDialog->m_LayoutDialog.MenuList->size();i++)
	{
		if (m_pEditorDialog->m_LayoutDialog.MenuList->at(i)->id==menu_id)
		{
			m_MenuRec = m_pEditorDialog->m_LayoutDialog.MenuList->at(i);
		}
	}

	/*
	if (m_pFirmware != NULL)
	{
		m_Origin = CPoint(0, 0);

		CSize size = m_ResSize;
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
*/
	Invalidate();
}

void CThemeWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	CSize size = m_ResSize;

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

void CThemeWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	CSize size = m_ResSize;

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

void CThemeWnd::SetRes(int w, int h)
{
	m_ResSize = CSize(w, h);
}