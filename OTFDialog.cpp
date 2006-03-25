// OTFDialog.cpp : implementation file
//

#include "stdafx.h"
#include "iPodWizard.h"
#include "OTFDialog.h"
#include ".\otfdialog.h"

// helper routines

void DDX_OTFText(CDataExchange *pDX, int nIDC, BYTE &value)
{
	CString s;

	if (pDX->m_bSaveAndValidate)
	{
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->GetWindowText(s);
		int temp=_wtoi(s.GetBuffer());
		if (temp>=0 && temp<=255)
			value=temp;
		else
			value=0;
	}
	else
	{
		s.Format(TEXT("%d"), value);
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->SetWindowText(s);
	}
}

void DDX_OTFText(CDataExchange *pDX, int nIDC, CHAR &value)
{
	CString s;

	if (pDX->m_bSaveAndValidate)
	{
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->GetWindowText(s);
		int temp=_wtoi(s.GetBuffer());
		if (temp>-128 && temp<128)
			value=temp;
		else
			value=0;
	}
	else
	{
		s.Format(TEXT("%d"), value);
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->SetWindowText(s);
	}
}

void DDX_OTFText(CDataExchange *pDX, int nIDC, WORD &value)
{
	CString s;

	if (pDX->m_bSaveAndValidate)
	{
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->GetWindowText(s);
		int temp=_wtoi(s.GetBuffer());
		if (temp>=0 && temp <=0xffff)
			value=temp;
		else
			value=0;
	}
	else
	{
		s.Format(TEXT("%d"), value);
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->SetWindowText(s);
	}
}

void DDX_OTFTextHex(CDataExchange *pDX, int nIDC, int nIDC2, WORD &value, WORD &value2)
{
	CString s;

	if (pDX->m_bSaveAndValidate)
	{
		WORD temp=value,temp2=value2;
		WORD len=value2-value;
		value = 0;
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->GetWindowText(s);
		s.MakeLower();
		for (int i = 0; i < min(4, s.GetLength()); i++)
		{
			TCHAR c = ((LPCTSTR)s)[i];
			if (c >= '0' && c <= '9')
			{
				value = (value << 4) + (c - '0');
			}
			else if (c >= 'a' && c <= 'f')
			{
				value = (value << 4) + (c - 'a' + 10);
			}
			else
				return;
		}
		//
		value2 = 0;
		pDX->m_pDlgWnd->GetDlgItem(nIDC2)->GetWindowText(s);
		s.MakeLower();
		for (int i = 0; i < min(4, s.GetLength()); i++)
		{
			TCHAR c = ((LPCTSTR)s)[i];
			if (c >= '0' && c <= '9')
			{
				value2 = (value2 << 4) + (c - '0');
			}
			else if (c >= 'a' && c <= 'f')
			{
				value2 = (value2 << 4) + (c - 'a' + 10);
			}
			else
				return;
		}
		if (temp!=value)
		{
			if (value+len>0xfffe) //0xffff is a reserved code in font
			{
				value2=0xfffe;
				value=value2-len;
			}
			else
			{
				value2=value+len;
			}
		}
		else if (temp2!=value2)
		{
			if (value2<len+0x20)
			{
				value=0x20; //it shouldn't be wise to put 0x0 as there are control chars from 0x0 to 0x20 such as tab and new line
				value2=len;
			}
			else
			{
				value=value2-len;
			}
		}
	}
	else
	{
		s.Format(TEXT("%04X"), value);
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->SetWindowText(s);
		s.Format(TEXT("%04X"), value2);
		pDX->m_pDlgWnd->GetDlgItem(nIDC2)->SetWindowText(s);
	}
}

// COTFDialog dialog

IMPLEMENT_DYNAMIC(COTFDialog, CDialog)
COTFDialog::COTFDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COTFDialog::IDD, pParent)
	, m_FontName(_T(""))
	, m_pFirmware(NULL)
	, m_FontIndex(0)
	, m_FontSize(0)
	, m_BitDepth(0)
	, m_Width(0)
	, m_BearingX(0)
	, m_BearingY(0)
	, m_Advance(0)
	, m_GroupEnd(0)
	, m_GroupStart(0)
	, m_GroupOffset(0)
	, m_GroupExplain(_T(""))
	, m_ALeft(0)
	, m_ARight(0)
	, m_BaseLine(0)
{
}

COTFDialog::~COTFDialog()
{
}

void COTFDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FONTNAME_EDIT, m_FontName);
	DDX_Text(pDX, IDC_FONTSIZE_EDIT, m_FontSize);
	DDX_Text(pDX, IDC_FONTSTYLE_EDIT, m_FontStyle);
	DDX_Control(pDX, IDC_PICTURE, m_FontWnd);
	DDX_Control(pDX, IDC_FONTZOOM, m_ZoomWnd);
	DDX_Control(pDX, IDC_FONTIDX_COMBO, m_FontIndexCombo);
	DDX_Text(pDX, IDC_BITDEPTH_EDIT, m_BitDepth);
	DDX_Control(pDX, IDC_CHAR_COMBO, m_CharCombo);
	DDX_Control(pDX, IDC_GROUPIDX_COMBO, m_GroupIndexCombo);
	DDX_OTFTextHex(pDX, IDC_GROUPSTART_EDIT, IDC_GROUPEND_EDIT, m_GroupStart, m_GroupEnd);
	DDX_OTFText(pDX, IDC_GROUPOFFSET_EDIT, m_GroupOffset);
	DDX_OTFText(pDX, IDC_GINDEX_EDIT, m_GlyphIndex);
	DDX_OTFText(pDX, IDC_WIDTH_EDIT, m_Width);
	DDX_OTFText(pDX, IDC_HEIGHT_TEXT, m_Height);
	DDX_OTFText(pDX, IDC_BEARINGX_EDIT, m_BearingX);
	DDX_OTFText(pDX, IDC_BEARINGY_EDIT, m_BearingY);
	DDX_OTFText(pDX, IDC_ADVANCE_EDIT, m_Advance);
	DDX_Control(pDX, IDC_GLYPHIDX_COMBO, m_GlyphCombo);
	DDX_Text(pDX, IDC_GROUPEXPLAIN_STATIC, m_GroupExplain);

	DDX_OTFText(pDX, IDC_ALEFT_EDIT, m_ALeft);
	DDX_OTFText(pDX, IDC_ARIGHT_EDIT, m_ARight);
	DDX_OTFText(pDX, IDC_BASELINE_EDIT, m_BaseLine);
}


BEGIN_MESSAGE_MAP(COTFDialog, CDialog)
	ON_BN_CLICKED(ID_PREV_PICTURE, OnBnClickedPrevPicture)
	ON_BN_CLICKED(ID_NEXT_PICTURE, OnBnClickedNextPicture)
	ON_CBN_SELCHANGE(IDC_FONTIDX_COMBO, OnCbnSelchangeFontidxCombo)
	ON_BN_CLICKED(IDC_REFRESH_BUTTON, OnBnClickedRefreshButton)
	ON_BN_CLICKED(ID_MAKE_FONT, OnBnClickedMakeFont)
	ON_CBN_SELCHANGE(IDC_CHAR_COMBO, OnCbnSelchangeCharCombo)
	ON_CBN_SELCHANGE(IDC_GROUPIDX_COMBO, OnCbnSelchangeGroupidxCombo)
	ON_BN_CLICKED(ID_ZOOMOUT_BUTTON, OnBnClickedZoomoutButton)
	ON_BN_CLICKED(ID_ZOOMIN_BUTTON, OnBnClickedZoominButton)
	ON_CBN_SELCHANGE(IDC_GLYPHIDX_COMBO, OnCbnSelchangeGlyphidxCombo)
	ON_BN_CLICKED(IDC_BMP_BUTTON, OnBnClickedBmpButton)
	ON_BN_CLICKED(ID_LOAD_GLYPH, OnBnClickedLoadGlyph)
	ON_BN_CLICKED(ID_SAVE_GLYPH, OnBnClickedSaveGlyph)
	ON_BN_CLICKED(IDC_CHKGRID, OnBnClickedChkgrid)
	ON_EN_CHANGE(IDC_ADVANCE_EDIT, OnEnChangeAdvanceEdit)
	ON_EN_CHANGE(IDC_BEARINGY_EDIT, OnEnChangeBearingyEdit)
	ON_EN_CHANGE(IDC_BEARINGX_EDIT, OnEnChangeBearingxEdit)
	ON_EN_CHANGE(IDC_GROUPSTART_EDIT, OnEnChangeGroupstartEdit)
	ON_EN_CHANGE(IDC_GROUPEND_EDIT, OnEnChangeGroupendEdit)
	ON_EN_CHANGE(IDC_GROUPOFFSET_EDIT, OnEnChangeGroupoffsetEdit)
	ON_BN_CLICKED(ID_LOAD_FONT, OnBnClickedLoadFont)
	ON_BN_CLICKED(ID_SAVE_FONT, OnBnClickedSaveFont)
	ON_BN_CLICKED(ID_MULLOAD_GLYPH, OnBnClickedMulloadGlyph)
END_MESSAGE_MAP()


// COTFDialog message handlers

BOOL COTFDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_ZoomWnd.SetZoomMode(TRUE);

	//CheckDlgButton(IDC_CHKGRID, 1);

	m_CharCombo.SendMessage(CCM_SETUNICODEFORMAT, TRUE, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void COTFDialog::SetFirmware(CFirmware *pFirmware)
{
	m_pFirmware = pFirmware;

	m_FontIndex = 0;

	m_FontIndexCombo.ResetContent();

	CString s;
	for (DWORD i = 0; i < m_pFirmware->GetNumOTFFonts(); i++)
	{
		s.Format(TEXT("%d"), i);
		m_FontIndexCombo.InsertString(i, s);
	}
	m_FontIndexCombo.SetCurSel(0);

	UpdateFont();
}

void COTFDialog::UpdateFont()
{
	WORD i;

	LPBYTE lpBuffer = m_pFirmware->GetOTFFont(m_FontIndex);
	if (lpBuffer == NULL)
		return;

	if (!m_Font.Read(lpBuffer))
		return;

	m_FontName = m_Font.GetFontName();
	m_FontSize = m_Font.GetFontSize();
	m_BitDepth = m_Font.GetFontBitDepth();
	m_FontStyle = m_Font.GetFontStyle();

	UpdateData(FALSE);

	m_FontWnd.SetOTFFont(&m_Font);
	m_ZoomWnd.SetOTFFont(&m_Font);

	m_FontIndexCombo.SetCurSel(m_FontIndex);

	//

	CString s;
	m_GroupIndexCombo.ResetContent();
	for (i = 0; i < m_Font.GetNumUnicodeGroups(); i++)
	{
		s.Format(TEXT("%d"), i);
		m_GroupIndexCombo.AddString(s);
	}
	m_GroupIndexCombo.SetCurSel(0);
	ReadUnicodeGroup();

	m_GlyphCombo.ResetContent();
	for (i = 1; i <= m_Font.GetNumGlyphs(); i++)
	{
		s.Format(TEXT("%d"), i);
		m_GlyphCombo.AddString(s);
	}

	m_GlyphCombo.SetCurSel(m_GroupOffset-1);

	//

	UpdateCharCombo();
	OnCbnSelchangeGlyphidxCombo();

	BOOL bCanWrite = m_pFirmware->CanWriteOTFFont(m_FontIndex);

	GetDlgItem(ID_SAVE_FONT)->EnableWindow(TRUE);
	GetDlgItem(ID_LOAD_FONT)->EnableWindow(bCanWrite);
	GetDlgItem(ID_SAVE_GLYPH)->EnableWindow(TRUE);
	GetDlgItem(ID_LOAD_GLYPH)->EnableWindow(bCanWrite);
	GetDlgItem(ID_MAKE_FONT)->EnableWindow(bCanWrite);
	GetDlgItem(ID_PREV_PICTURE)->EnableWindow(TRUE);
	GetDlgItem(ID_NEXT_PICTURE)->EnableWindow(TRUE);
	GetDlgItem(ID_ZOOMIN_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(ID_ZOOMOUT_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHKGRID)->EnableWindow(TRUE);
	GetDlgItem(IDC_BMP_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_REFRESH_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(ID_MULLOAD_GLYPH)->EnableWindow(TRUE);
}

void COTFDialog::UpdateZoomView()
{
	m_ZoomWnd.SetMetrics(m_Height, m_Width, m_BearingX, m_BearingY, m_Advance);
}

void COTFDialog::ReadUnicodeGroup()
{
	int i = m_GroupIndexCombo.GetCurSel();

	m_Font.GetUnicodeGroup(i, &m_GroupStart, &m_GroupEnd, &m_GroupOffset);

	GetDlgItem(IDC_GROUPOFFSET_EDIT)->EnableWindow(m_Font.GetUnicodeGroupGlyphState(i));

	UpdateGroupExplanation();
}

void COTFDialog::UpdateCharCombo()
{
	WORD i;

	m_CharCombo.ResetContent();

	wchar_t w[64];
	WORD c, group;
	for (i = 0; i < m_Font.GetNumUnicodeChars(); i++)
	{
		c = m_Font.GetUnicodeChar(i, &group);
		swprintf(w, L"U+%04X (%c)", c, c);
		::SendMessageW(m_CharCombo.m_hWnd, CB_ADDSTRING, 0, (LPARAM)w);
	}

	m_CharCombo.SetCurSel(0);
}

void COTFDialog::UpdateGroupExplanation()
{
	m_GroupExplain.Format(TEXT("Explanation: Unicode characters U+%04X to U+%04X are mapped to entries %d to %d of the character mapping table."), m_GroupStart, m_GroupEnd, m_GroupOffset, m_GroupOffset + m_GroupEnd - m_GroupStart);

	UpdateData(FALSE);
}

void COTFDialog::OnBnClickedPrevPicture()
{
	if (m_FontIndex > 0)
	{
		m_FontIndex--;

		UpdateFont();
	}
}

void COTFDialog::OnBnClickedNextPicture()
{
	if (m_FontIndex < m_pFirmware->GetNumOTFFonts() - 1)
	{
		m_FontIndex++;

		UpdateFont();
	}
}

void COTFDialog::OnBnClickedRefreshButton()
{
	m_FontWnd.Invalidate();
}

void COTFDialog::OnCbnSelchangeFontidxCombo()
{
	m_FontIndex = m_FontIndexCombo.GetCurSel();

	UpdateFont();
}

void COTFDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialog::OnOK();
}

void COTFDialog::OnBnClickedMakeFont()
{
	CFontDialog dlg;

	if (dlg.DoModal() != IDOK)
		return;

	LOGFONT lf;
	memcpy(&lf, dlg.m_cf.lpLogFont, sizeof(LOGFONT));

	if (m_BitDepth > 1)
		lf.lfQuality = ANTIALIASED_QUALITY;

	CFont font;
	VERIFY(font.CreateFontIndirect(&lf));

	//

	CDC dc;
	CDC *pDC = GetWindowDC();
	dc.CreateCompatibleDC(pDC);
	
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, 60, 60);

	CBitmap *pBitmap = dc.SelectObject(&bmp);
	CFont *pOldFont = dc.SelectObject(&font);

	dc.SetTextColor(RGB(0, 0, 0));
	dc.SetBkColor(RGB(255 ,255, 255));

	//

	WORD xoffset = 0;

	int height;
	wchar_t c[2];
	WORD group;

	c[1] = 0;

	DWORD imagedatalen,totalbytes=0;
	imagedatalen=m_Font.GetGlyphDataLen();

	TEXTMETRIC tm;
	GetTextMetrics(dc.m_hDC, &tm);

	m_Font.ResetGlyphData();

	for (int i = 0; i < m_Font.GetNumUnicodeChars(); i++)
	{
		c[0] = m_Font.GetUnicodeChar(i, &group);
		
		CRect rect(0, 0, 60, 60);
		::DrawTextW(dc.m_hDC, c, 1, &rect, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOPREFIX | DT_CALCRECT);
		height = ::DrawTextW(dc.m_hDC, c, 1, &rect, DT_LEFT | DT_TOP | DT_NOPREFIX | DT_SINGLELINE);

		ABCFLOAT abc;
		GetCharABCWidthsFloat(dc.m_hDC, c[0], c[0], &abc);

		SHORT abcA = (SHORT)abc.abcfA;
		SHORT abcB = (SHORT)abc.abcfB;
		SHORT abcC = (SHORT)abc.abcfC;

		// analyze

		int x, y;
		int wstart = -1, wend = -1, hstart = -1, hend = -1;
		BOOL bLineEmpty;

		//gets the start and end line for the actual char inside the 60 by 60 rect
		for (x = 0; x < rect.right; x++)
		{
			bLineEmpty = TRUE;
			for (y = 0; y < height; y++)
			{
				if (dc.GetPixel(x, y) != RGB(255, 255, 255))
				{
					bLineEmpty = FALSE;
					break;
				}
			}
			if (!bLineEmpty)
			{
				wstart = x;
				break;
			}
		}

		for (x = rect.right - 1; x >= 0 && wstart != -1; x--)
		{
			bLineEmpty = TRUE;
			for (y = 0; y < height; y++)
			{
				if (dc.GetPixel(x, y) != RGB(255, 255, 255))
				{
					bLineEmpty = FALSE;
					break;
				}
			}
			if (!bLineEmpty)
			{
				wend = x;
				break;
			}
		}
		if (wstart!=-1)
			wend++;

		for (y = 0; y < rect.bottom; y++)
		{
			bLineEmpty = TRUE;
			for (x = wstart; x < wend; x++)
			{
				if (dc.GetPixel(x, y) != RGB(255, 255, 255))
				{
					bLineEmpty = FALSE;
					break;
				}
			}
			if (!bLineEmpty)
			{
				hstart = y;
				break;
			}
		}

		for (y = rect.bottom - 1; y >= 0 && hstart != -1; y--)
		{
			bLineEmpty = TRUE;
			for (x = wstart; x < wend ; x++)
			{
				if (dc.GetPixel(x, y) != RGB(255, 255, 255))
				{
					bLineEmpty = FALSE;
					break;
				}
			}
			if (!bLineEmpty)
			{
				hend = y;
				break;
			}
		}
		if (hstart!=-1)
			hend++;

		WORD glyphidx=m_Font.GetUnicodeCharOffset(c[0]);

		//check that we don't override and if the size is too big, stop
		totalbytes += sizeof(smallGlyphMetrics);
		if (wstart!=-1 && hstart!=-1)
		{
			if ((wend - wstart)%2==0)
				totalbytes += (hend - hstart)*(wend - wstart)/(8/m_BitDepth);
			else
				totalbytes += (hend - hstart)*(wend - wstart + 1)/(8/m_BitDepth);
		}

		if (totalbytes >= imagedatalen)
		{
			m_Font.FillEndGlyphs(i);
			break;
		}

		// update metrics
		//bearingx = abcA
		//advance = abcA + abcB + abcC
		//width = abcB
		//bearingy = tm.tmAscent - hstart
		//height = hend - hstart
		if (hstart==-1 || wstart==-1)
			m_Font.AddCharMetrics(glyphidx, 0, 0, (CHAR)abcA, 0, (BYTE)(abcA + abcB + abcC));
		else
			m_Font.AddCharMetrics(glyphidx, wend - wstart, hend - hstart, (CHAR)abcA, (CHAR)(tm.tmAscent - hstart), (BYTE)(abcA + abcB + abcC));

		// copy
		if (abcB!=0)
		{
			for (x = wstart; x < wend; x++)
			{
				for (y = hstart; y < hend; y++)
				{
					m_Font.SetFontPixel(glyphidx, x - wstart, y - hstart, dc.GetPixel(x, y));
				}
			}
		}
	}

	//

	dc.SelectObject(pOldFont);
	dc.SelectObject(pBitmap);
	
	//

	ReleaseDC(pDC);

	UpdateFont();
	//UpdateData(FALSE);
}
void COTFDialog::OnCbnSelchangeCharCombo()
{
	int i = m_CharCombo.GetCurSel();

	WORD group;
	WORD unicodeChar = m_Font.GetUnicodeChar(i, &group);

	// update group
	m_GroupIndexCombo.SetCurSel(group);
	ReadUnicodeGroup();

	// update char
	WORD offset = m_Font.GetUnicodeCharOffset(unicodeChar);

	for (i=0;i<m_Font.GetNumGlyphs();i++)
	{
		if (m_Font.GetGlyphIndex(i)==offset)
		{
			m_GlyphCombo.SetCurSel(i);
			break;
		}
	}
	ReadCharMetrics();
	UpdateZoomView();
}

void COTFDialog::ReadCharMetrics()
{
	int i = m_GlyphCombo.GetCurSel();

	m_GlyphIndex = m_Font.GetGlyphIndex(i);

	BYTE width=0,height=0,advance=0;
	CHAR bearingx=0,bearingy=0;

	m_Font.GetCharMetrics(m_GlyphIndex, &width, &height, &bearingx, &bearingy, &advance);

	m_Width = width;
	m_Height = height;
	m_BearingX =  bearingx;
	m_BearingY = bearingy;
	m_Advance = advance;

	m_ZoomWnd.SetGlyph(m_GlyphIndex);

	GetDlgItem(IDC_WIDTH_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BEARINGX_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BEARINGY_EDIT)->EnableWindow(TRUE);
	//GetDlgItem(IDC_UPDATE_BUTTON)->EnableWindow(bEnable);

	UpdateData(FALSE);
}

void COTFDialog::WriteUnicodeGroup()
{
	UpdateData(TRUE);

	int i = m_GroupIndexCombo.GetCurSel();

	if (i < 0)
		return;

	m_Font.SetUnicodeGroup(i, m_GroupStart, m_GroupEnd, m_GroupOffset-1);

	//if sorting then change the index:
	WORD gstart,gend,goffset;
	for (i=0;i<m_GroupIndexCombo.GetCount();i++)
	{
		m_Font.GetUnicodeGroup(i, &gstart, &gend, &goffset);
		if (gend==m_GroupEnd && gstart==m_GroupStart && goffset==m_GroupOffset)
		{
			m_GroupIndexCombo.SetCurSel(i);
			break;
		}
	}
	//

	UpdateGroupExplanation();

	UpdateCharCombo();
	UpdateZoomView();
}

void COTFDialog::WriteCharMetrics()
{
	UpdateData(TRUE);

	int i=m_GlyphCombo.GetCurSel();

	m_Font.SetCharMetrics(m_Font.GetGlyphIndex(i), m_Width, m_Height, m_BearingX, m_BearingY, m_Advance);

	UpdateZoomView();
}

void COTFDialog::OnCbnSelchangeGroupidxCombo()
{
	ReadUnicodeGroup();

	// update char mapping

	m_GlyphCombo.SetCurSel(m_GroupOffset-1);
	ReadCharMetrics();
	UpdateZoomView();
}

void COTFDialog::OnBnClickedZoomoutButton()
{
	m_ZoomWnd.ChangeZoom(FALSE);
	CString temp;
	temp.Format(TEXT("x%d"), m_ZoomWnd.GetZoom());
	GetDlgItem(IDC_STATIC_ZOOM)->SetWindowText(temp);
}

void COTFDialog::OnBnClickedZoominButton()
{
	m_ZoomWnd.ChangeZoom(TRUE);
	CString temp;
	temp.Format(TEXT("x%d"), m_ZoomWnd.GetZoom());
	GetDlgItem(IDC_STATIC_ZOOM)->SetWindowText(temp);
}

void COTFDialog::OnCbnSelchangeGlyphidxCombo()
{
	ReadCharMetrics();
	UpdateZoomView();
}

void COTFDialog::OnBnClickedBmpButton()
{
	::SendMessage(::GetParent(::GetParent(m_hWnd)), WM_APP, (WPARAM)10, (LPARAM)FALSE);
}

void COTFDialog::OnBnClickedLoadGlyph()
{
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY, TEXT("Bitmap Files (*.bmp, *.gif, *.jpg)|*.bmp;*.gif;*.jpg||"), this);

	if (dlg.DoModal() != IDOK)
		return;

	CImage img;
	if (FAILED(img.Load(dlg.GetPathName())))
	{
		MessageBox(TEXT("Unable to load image!"));
		return;
	}

	//CSize size = m_Font.GetFontBitmapSize(m_GlyphIndex);

	m_Width = img.GetWidth();
	m_Height = img.GetHeight();
	m_Font.SetCharMetrics(m_GlyphIndex, m_Width, m_Height, 0, m_Height, m_Width);

	LONG x, y;
	COLORREF color;

	for (x = 0; x < img.GetWidth(); x++)
	{
		for (y = 0; y < img.GetHeight(); y++)
		{
			color = img.GetPixel(x, y);
			m_Font.SetFontPixel(m_GlyphIndex, x, y, color);
		}
	}

	UpdateData(FALSE);

	ReadCharMetrics();
	UpdateZoomView();
}

void COTFDialog::OnBnClickedSaveGlyph()
{
	CString filename;
	filename.Format(TEXT("%s-%d.bmp"), m_Font.GetFontName(), m_GlyphIndex);

	CFileDialog dlg(FALSE, TEXT("bmp"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("BMP Files (*.bmp)|*.bmp||"), this);

	if (dlg.DoModal() != IDOK)
		return;

	CSize size = m_Font.GetFontBitmapSize(m_GlyphIndex);

	CImage img;

	if (!img.Create(size.cx, size.cy, 24))
	{
		MessageBox(TEXT("Unable to create image!"));
		return;
	}

	int x, y;

	for (x = 0; x < size.cx; x++)
	{
		for (y = 0; y < size.cy; y++)
		{
			img.SetPixel(x, y, m_Font.GetFontPixel(m_GlyphIndex, CPoint(x, y)));
		}
	}

	if (FAILED(img.Save(dlg.GetPathName())))
	{
		MessageBox(TEXT("Unable to save image!"));
		return;
	}
}

void COTFDialog::OnBnClickedChkgrid()
{
	if (IsDlgButtonChecked(IDC_CHKGRID) == 0)
		m_ZoomWnd.SetGrid(FALSE);
	else
		m_ZoomWnd.SetGrid(TRUE);
}

void COTFDialog::OnEnChangeAdvanceEdit()
{
	WriteCharMetrics();
}

void COTFDialog::OnEnChangeBearingyEdit()
{
	WriteCharMetrics();
}

void COTFDialog::OnEnChangeBearingxEdit()
{
	WriteCharMetrics();
}

void COTFDialog::OnEnChangeGroupstartEdit()
{
	WriteUnicodeGroup();
}

void COTFDialog::OnEnChangeGroupendEdit()
{
	WriteUnicodeGroup();
}

void COTFDialog::OnEnChangeGroupoffsetEdit()
{
	WriteUnicodeGroup();
}

void COTFDialog::OnBnClickedLoadFont()
{
	CFileDialog dlg(TRUE, TEXT("otf"), 0, OFN_HIDEREADONLY, TEXT("OpenType Bitmap Fonts (*.otf)|*.otf||"), this);

	if (dlg.DoModal() != IDOK)
		return;

	CFile file;
	if (!file.Open(dlg.GetPathName(), CFile::modeRead))
	{
		MessageBox(TEXT("Unable to load file!"));
		return;
	}

	LPBYTE lpBuffer = m_pFirmware->GetOTFFont(m_FontIndex);
	if (file.Read(lpBuffer, m_Font.GetFontBlockLen()) < m_Font.GetFontBlockLen())
	{
		MessageBox(TEXT("Error reading font file!"));
		return;
	}

	UpdateFont();
}

void COTFDialog::OnBnClickedSaveFont()
{
	CString filename;
	filename.Format(TEXT("%s.otf"), m_Font.GetFontName());

	CFileDialog dlg(FALSE, TEXT("otf"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("OpenType Bitmap Font (*.otf)|*.otf||"), this);

	if (dlg.DoModal() != IDOK)
		return;

	CFile file;
	if (!file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
	{
		MessageBox(TEXT("Unable to save file!"));
		return;
	}

	LPBYTE lpBuffer = m_pFirmware->GetOTFFont(m_FontIndex);
	file.Write(lpBuffer, m_Font.GetFontBlockLen());
	file.Close();
}

void COTFDialog::OnBnClickedMulloadGlyph()
{
	UpdateData(TRUE);

	LPITEMIDLIST il;
	TCHAR folderPath[MAX_PATH];

	BROWSEINFO br;
    br.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
    br.pidlRoot = NULL;
    br.pszDisplayName = folderPath;
    br.lpszTitle = TEXT("Select folder for files");
    br.ulFlags = BIF_RETURNONLYFSDIRS;
    br.lpfn = NULL;
    br.lParam = 0;
    br.iImage = 0;

	il = SHBrowseForFolder(&br);
	if (il == NULL)
		return;

	SHGetPathFromIDList(il, folderPath);

	WORD index=0;
	CString filename = folderPath;
	filename += "\\????.bmp";

	BYTE width,height,advance;
	CHAR bearingy;
	WORD gidx;
	WIN32_FIND_DATA findData;
	HANDLE hFind = FindFirstFile(filename, &findData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			filename = folderPath;
			filename += "\\";
			filename += findData.cFileName;
			_stscanf(findData.cFileName, TEXT("%04X"), &index);
			//index = _wtoi(findData.cFileName + 4);

			if (index>=m_GroupStart && index<=m_GroupEnd)
			{
				CImage img;
				if (FAILED(img.Load(filename)))
				{
					CString msg;
					msg.Format(TEXT("Unable to load image %d!"), index);
					MessageBox(msg);
					return;
				}

				width = img.GetWidth();
				height = img.GetHeight();
				gidx=m_Font.GetGlyphIndex(m_GroupOffset-1+index-m_GroupStart);

				//calculate bearingy
				int x, y;

				int hstart = -1, hend = -1;
				BOOL bLineEmpty;

				for (y = 0; y < height; y++)
				{
					bLineEmpty = TRUE;
					for (x = 0; x < width; x++)
					{
						if (img.GetPixel(x, y) != RGB(255, 255, 255))
						{
							bLineEmpty = FALSE;
							break;
						}
					}
					if (!bLineEmpty)
					{
						hstart = y;
						break;
					}
				}

				for (y = height - 1; y >= 0 && hstart != -1; y--)
				{
					bLineEmpty = TRUE;
					for (x = 0; x < width ; x++)
					{
						if (img.GetPixel(x, y) != RGB(255, 255, 255))
						{
							bLineEmpty = FALSE;
							break;
						}
					}
					if (!bLineEmpty)
					{
						hend = y;
						break;
					}
				}
				if (hstart!=-1)
					hend++;
				//
				height=hend-hstart;
				bearingy=m_BaseLine-hstart;
				//bearingy=height;
				//if (hend>m_BaseLine)
				//	bearingy-=hend-m_BaseLine;
				advance=m_ALeft + width + m_ARight;
				m_Font.SetCharMetrics(gidx, width, height, m_ALeft, bearingy, advance);

				//LONG j, k;
				COLORREF color;

				for (x = 0; x < img.GetWidth(); x++)
				{
					for (y = 0; y < height; y++)
					{
						color = img.GetPixel(x, y + hstart);
						m_Font.SetFontPixel(gidx, x, y, color);
					}
				}
			}
		} while (FindNextFile(hFind, &findData) != 0);

		FindClose(hFind);

		UpdateData(FALSE);

		ReadCharMetrics();
		UpdateZoomView();
	}
	index=0;
}
