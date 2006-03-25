// FontsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "iPodWizard.h"
#include "FontsDialog.h"
#include ".\fontsdialog.h"


// helper routines

/*void DDX_MyText(CDataExchange *pDX, int nIDC, DWORD &value)
{
	CString s;

	if (pDX->m_bSaveAndValidate)
	{
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->GetWindowText(s);
		WORD temp;
		WORD &val=temp;
		if (_stscanf(s, TEXT("%u"), &val) != 1)
		{
			val = 0;
		}
		value=val;
	}
	else
	{
		s.Format(TEXT("%d"), value);
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->SetWindowText(s);
	}
}*/

void DDX_MyText(CDataExchange *pDX, int nIDC, WORD &value)
{
	CString s;

	if (pDX->m_bSaveAndValidate)
	{
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->GetWindowText(s);
		if (_stscanf(s, TEXT("%d"), &value) != 1)
		{
			value = 0;
		}
	}
	else
	{
		s.Format(TEXT("%d"), value);
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->SetWindowText(s);
	}
}

void DDX_MyText(CDataExchange *pDX, int nIDC, SHORT &value)
{
	CString s;

	if (pDX->m_bSaveAndValidate)
	{
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->GetWindowText(s);
		if (_stscanf(s, TEXT("%d"), &value) != 1)
		{
			value = 0;
		}
	}
	else
	{
		s.Format(TEXT("%d"), value);
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->SetWindowText(s);
	}
}

void DDX_MyTextHex(CDataExchange *pDX, int nIDC, WORD &value)
{
	CString s;

	if (pDX->m_bSaveAndValidate)
	{
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
	}
	else
	{
		s.Format(TEXT("%04X"), value);
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->SetWindowText(s);
	}
}

// CFontsDialog dialog

IMPLEMENT_DYNAMIC(CFontsDialog, CDialog)
CFontsDialog::CFontsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFontsDialog::IDD, pParent)
	, m_FontName(_T(""))
	, m_pFirmware(NULL)
	, m_FontIndex(0)
	, m_FontSize(0)
	, m_Offset1(0)
	, m_Offset2(0)
	, m_Width(0)
	, m_Ident(0)
	, m_BitDepth(0)
	, m_GroupLen(0)
	, m_GroupStart(0)
	, m_GroupOffset(0)
	, m_GroupExplain(_T(""))
{
}

CFontsDialog::~CFontsDialog()
{
}

void CFontsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FONTNAME_EDIT, m_FontName);
	DDX_Text(pDX, IDC_FONTSIZE_EDIT, m_FontSize);
	DDX_Text(pDX, IDC_FONTSTYLE_EDIT, m_FontStyle);
	DDX_Control(pDX, IDC_PICTURE, m_FontWnd);
	DDX_Control(pDX, IDC_FONTZOOM, m_ZoomWnd);
	DDX_Control(pDX, IDC_FONTIDX_COMBO, m_FontIndexCombo);
	DDX_Control(pDX, IDC_CHAR_COMBO, m_CharCombo);
	DDX_MyText(pDX, IDC_OFFSET1_EDIT, m_Offset1);
	DDX_MyText(pDX, IDC_OFFSET2_EDIT, m_Offset2);
	DDX_MyText(pDX, IDC_WIDTH_EDIT, m_Width);
	DDX_MyText(pDX, IDC_IDENT_EDIT, m_Ident);
	DDX_Text(pDX, IDC_BITDEPTH_EDIT, m_BitDepth);
	DDX_Control(pDX, IDC_GROUPIDX_COMBO, m_GroupIndexCombo);
	DDX_MyText(pDX, IDC_GROUPLEN_EDIT, m_GroupLen);
	DDX_MyTextHex(pDX, IDC_GROUPSTART_EDIT, m_GroupStart);
	DDX_MyText(pDX, IDC_GROUPOFFSET_EDIT, m_GroupOffset);
	DDX_Control(pDX, IDC_CHARMAPIDX_COMBO, m_CharMapCombo1);
	DDX_Control(pDX, IDC_CHARMAPIDX2_COMBO, m_CharMapCombo2);
	DDX_Text(pDX, IDC_GROUPEXPLAIN_STATIC, m_GroupExplain);
}


BEGIN_MESSAGE_MAP(CFontsDialog, CDialog)
	ON_BN_CLICKED(ID_PREV_PICTURE, OnBnClickedPrevPicture)
	ON_BN_CLICKED(ID_NEXT_PICTURE, OnBnClickedNextPicture)
	ON_BN_CLICKED(ID_SAVE_BITMAP, OnBnClickedSaveBitmap)
	ON_BN_CLICKED(ID_LOAD_BITMAP, OnBnClickedLoadBitmap)
	ON_CBN_SELCHANGE(IDC_FONTIDX_COMBO, OnCbnSelchangeFontidxCombo)
	ON_CBN_SELCHANGE(IDC_CHAR_COMBO, OnCbnSelchangeCharCombo)
	ON_BN_CLICKED(IDC_REFRESH_BUTTON, OnBnClickedRefreshButton)
	ON_CBN_SELCHANGE(IDC_GROUPIDX_COMBO, OnCbnSelchangeGroupidxCombo)
	ON_CBN_SELCHANGE(IDC_CHARMAPIDX2_COMBO, OnCbnSelchangeCharmapidx2Combo)
	ON_CBN_SELCHANGE(IDC_CHARMAPIDX_COMBO, OnCbnSelchangeCharmapidxCombo)
	ON_EN_CHANGE(IDC_OFFSET1_EDIT, OnEnChangeOffset1Edit)
	ON_EN_CHANGE(IDC_OFFSET2_EDIT, OnEnChangeOffset2Edit)
	ON_EN_CHANGE(IDC_WIDTH_EDIT, OnEnChangeWidthEdit)
	ON_EN_CHANGE(IDC_IDENT_EDIT, OnEnChangeIdentEdit)
	ON_EN_CHANGE(IDC_GROUPSTART_EDIT, OnEnChangeGroupstartEdit)
	ON_EN_CHANGE(IDC_GROUPLEN_EDIT, OnEnChangeGrouplenEdit)
	ON_EN_CHANGE(IDC_GROUPOFFSET_EDIT, OnEnChangeGroupoffsetEdit)
	ON_BN_CLICKED(ID_LOAD_METRICS, OnBnClickedLoadMetrics)
	ON_BN_CLICKED(ID_SAVE_METRICS, OnBnClickedSaveMetrics)
	ON_BN_CLICKED(ID_MAKE_FONT, OnBnClickedMakeFont)
	ON_BN_CLICKED(ID_ZOOMIN_BUTTON, OnBnClickedZoomIn)
	ON_BN_CLICKED(ID_ZOOMOUT_BUTTON, OnBnClickedZoomOut)
	ON_BN_CLICKED(IDC_CHKGRID, OnBnClickedShowGrid)
	ON_MESSAGE(WM_APP, OnSelectMetricsFromBitmap)
	ON_BN_CLICKED(IDC_EXPLAIN_BUTTON, OnBnClickedCharMapExplain)
	ON_BN_CLICKED(IDC_OTF_BUTTON, OnBnClickedOtfButton)
END_MESSAGE_MAP()


// CFontsDialog message handlers

BOOL CFontsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	//m_FontWnd.EnableScrollBarCtrl(SB_VERT, TRUE);
	//m_ZoomWnd.EnableScrollBarCtrl(SB_HORZ, TRUE);
	//m_FontWnd.EnableScrollBar(SB_BOTH, ESB_DISABLE_BOTH);
	//m_ZoomWnd.EnableScrollBar(SB_BOTH, ESB_DISABLE_BOTH);
	m_ZoomWnd.SetZoomMode(TRUE);

	CheckDlgButton(IDC_CHKGRID, 1);

	m_CharCombo.SendMessage(CCM_SETUNICODEFORMAT, TRUE, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CFontsDialog::OnSelectMetricsFromBitmap(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL || lParam == NULL)
		return 0;
	int mg=(int)wParam;
	if (mg!=10)
		return 0;
	DWORD px=(DWORD)lParam;
	DWORD startOffset, endOffset;
	SHORT width, ident;

	for (WORD i=0;i<m_Font.GetNumMetrics();i++)
	{
		m_Font.GetCharMetrics(m_Font.GetCharMapping(i), &startOffset, &endOffset, &width, &ident);
		if (px>=startOffset && px<=endOffset)
		{
			m_CharMapCombo1.SetCurSel(i);
			UpdateCharMap();
			break;
		}
	}
	return 0;
}

void CFontsDialog::OnBnClickedCharMapExplain()
{
	MessageBox(TEXT("Explanation:\nOffsets = Determines what pixels in the picture should be counted as that specific char.\nWidth = Determines the total width the char will occupy on the iPod screen.\nIdent = Determines how many pixels in the char from the start offset(1) will be combined with the char before on the iPod screen.\nNotes: On one hand, if you specify a width bigger then the actual width in pixels of a char, the result will be a blank space after the char on the iPod screen but on the other hand, if the width specified is smaller, this will cause in the next char to run over this char."), TEXT("What do these values mean?"));
}

void CFontsDialog::OnBnClickedShowGrid()
{
	if (IsDlgButtonChecked(IDC_CHKGRID) == 0)
		m_ZoomWnd.SetGrid(FALSE);
	else
		m_ZoomWnd.SetGrid(TRUE);
}

void CFontsDialog::OnBnClickedZoomIn()
{
	m_ZoomWnd.ChangeZoom(TRUE);
	CString temp;
	temp.Format(TEXT("x%d"), m_ZoomWnd.GetZoom());
	GetDlgItem(IDC_STATIC_ZOOM)->SetWindowText(temp);
}

void CFontsDialog::OnBnClickedZoomOut()
{
	m_ZoomWnd.ChangeZoom(FALSE);
	CString temp;
	temp.Format(TEXT("x%d"), m_ZoomWnd.GetZoom());
	GetDlgItem(IDC_STATIC_ZOOM)->SetWindowText(temp);
}

void CFontsDialog::SetFirmware(CFirmware *pFirmware)
{
	m_pFirmware = pFirmware;

	m_FontIndex = 0;

	m_FontIndexCombo.ResetContent();

	CString s;
	for (DWORD i = 0; i < m_pFirmware->GetNumFonts(); i++)
	{
		s.Format(TEXT("%d"), i);
		m_FontIndexCombo.InsertString(i, s);
	}
	m_FontIndexCombo.SetCurSel(0);

	UpdateFont();
}

void CFontsDialog::UpdateFont()
{
	DWORD i;

	LPBYTE lpBuffer = m_pFirmware->GetFont(m_FontIndex);
	if (lpBuffer == NULL)
		return;

	LPBYTE lpEnd = m_pFirmware->GetFirmwareBuffer() + m_pFirmware->GetFirmwareSize();

	if (!m_Font.Read(lpBuffer, lpEnd))
		return;

	m_FontName = m_Font.GetFontName();
	m_FontSize = m_Font.GetFontSize();
	m_BitDepth = m_Font.GetFontBitDepth();
	m_FontStyle = m_Font.GetFontStyle();

	UpdateData(FALSE);

	m_FontWnd.SetIpodFont(&m_Font);
	m_ZoomWnd.SetIpodFont(&m_Font);

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

	//

	m_CharMapCombo1.ResetContent();
	m_CharMapCombo2.ResetContent();

	for (i = 1; i <= m_Font.GetNumChars(); i++)
	{
		s.Format(TEXT("%d"), i);
		m_CharMapCombo1.AddString(s);
		m_CharMapCombo2.AddString(s);
	}
	m_CharMapCombo1.SetCurSel(0);

	//

	UpdateCharCombo();
	OnCbnSelchangeCharCombo();

	BOOL bCanWrite = m_pFirmware->CanWriteFont(m_FontIndex);

	GetDlgItem(ID_SAVE_BITMAP)->EnableWindow(TRUE);
	GetDlgItem(ID_LOAD_BITMAP)->EnableWindow(bCanWrite);
	GetDlgItem(ID_SAVE_METRICS)->EnableWindow(TRUE);
	GetDlgItem(ID_LOAD_METRICS)->EnableWindow(bCanWrite);
	GetDlgItem(ID_MAKE_FONT)->EnableWindow(bCanWrite);
	GetDlgItem(ID_PREV_PICTURE)->EnableWindow(TRUE);
	GetDlgItem(ID_NEXT_PICTURE)->EnableWindow(TRUE);
	GetDlgItem(ID_CHK_NEWMETRICS)->EnableWindow(TRUE);
	GetDlgItem(ID_ZOOMIN_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(ID_ZOOMOUT_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHKGRID)->EnableWindow(TRUE);
	GetDlgItem(IDC_OTF_BUTTON)->EnableWindow(TRUE);
}

void CFontsDialog::OnBnClickedPrevPicture()
{
	if (m_FontIndex > 0)
	{
		m_FontIndex--;

		UpdateFont();
	}
}

void CFontsDialog::OnBnClickedNextPicture()
{
	if (m_FontIndex < m_pFirmware->GetNumFonts() - 1)
	{
		m_FontIndex++;

		UpdateFont();
	}
}

void CFontsDialog::OnBnClickedSaveBitmap()
{
	CString filename;
	filename.Format(TEXT("%s.bmp"), m_Font.GetFontName());

	CFileDialog dlg(FALSE, TEXT("bmp"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("BMP Files (*.bmp)|*.bmp||"), this);

	if (dlg.DoModal() != IDOK)
		return;

	CSize size = m_Font.GetFontBitmapSize();

	CImage img;
	/*
	size.cx=m_Offset2-m_Offset1;
	size.cy--;

	int x, y;

	int hstart = -1, hend = -1;
	BOOL bLineEmpty;

	for (y = 0; y < size.cy; y++)
	{
		bLineEmpty = TRUE;
		for (x = 0; x < size.cx; x++)
		{
			if (m_Font.GetFontPixel(CPoint(x+m_Offset1, y)) != RGB(255, 255, 255))
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

	for (y = size.cy - 1; y >= 0 && hstart != -1; y--)
	{
		bLineEmpty = TRUE;
		for (x = 0; x < size.cx ; x++)
		{
			if (m_Font.GetFontPixel(CPoint(x+m_Offset1, y)) != RGB(255, 255, 255))
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

	if (!img.Create(size.cx, hend - hstart, 24))
	{
		MessageBox(TEXT("Unable to create image!"));
		return;
	}

	for (x = 0; x < size.cx; x++)
	{
		for (y = 0; y < hend - hstart; y++)
		{
			img.SetPixel(x, y, m_Font.GetFontPixel(CPoint(x+m_Offset1, y + hstart)));
		}
	}

	if (FAILED(img.Save(dlg.GetPathName())))
	{
		MessageBox(TEXT("Unable to save image!"));
		return;
	}
	return;
	*/
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
			img.SetPixel(x, y, m_Font.GetFontPixel(CPoint(x, y)));
		}
	}

	if (FAILED(img.Save(dlg.GetPathName())))
	{
		MessageBox(TEXT("Unable to save image!"));
		return;
	}
}

void CFontsDialog::OnBnClickedLoadBitmap()
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

	CSize size = m_Font.GetFontBitmapSize();

	LONG x, y;
	COLORREF color;

	for (x = 0; x < size.cx; x++)
	{
		for (y = 0; y < size.cy; y++)
		{
			if (x < img.GetWidth() && y < img.GetHeight())
				color = img.GetPixel(x, y);
			else
				color = RGB(255, 255, 255);
			m_Font.SetFontPixel(x, y, color);
		}
	}

	UpdateFont();
}

void CFontsDialog::OnCbnSelchangeFontidxCombo()
{
	m_FontIndex = m_FontIndexCombo.GetCurSel();

	UpdateFont();
}

void CFontsDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialog::OnOK();
}

void CFontsDialog::OnCbnSelchangeCharCombo()
{
	int i = m_CharCombo.GetCurSel();

	WORD group;
	WORD unicodeChar = m_Font.GetUnicodeChar(i, &group);

	// update group
	m_GroupIndexCombo.SetCurSel(group);
	ReadUnicodeGroup();

	// update char
	WORD offset = m_Font.GetUnicodeCharOffset(unicodeChar);

	m_CharMapCombo1.SetCurSel(offset);
	UpdateCharMap();
}

void CFontsDialog::UpdateZoomView()
{
	m_ZoomWnd.SetMetrics(m_Offset1, m_Offset2, m_Width, m_Ident);
}

void CFontsDialog::OnBnClickedRefreshButton()
{
	m_FontWnd.Invalidate();
}

void CFontsDialog::ReadUnicodeGroup()
{
	int i = m_GroupIndexCombo.GetCurSel();

	DWORD temp=m_GroupLen;
	m_Font.GetUnicodeGroup(i, &m_GroupStart, &temp, &m_GroupOffset);
	m_GroupLen=(WORD)temp;

	UpdateGroupExplanation();
}

void CFontsDialog::WriteUnicodeGroup()
{
	UpdateData(TRUE);

	int i = m_GroupIndexCombo.GetCurSel();

	if (i < 0)
		return;

	m_Font.SetUnicodeGroup(i, m_GroupStart, m_GroupLen, m_GroupOffset);

	//if sorting then change the index:
	DWORD glen,temp=m_GroupLen;
	WORD gstart,goffset;
	for (i=0;i<m_GroupIndexCombo.GetCount();i++)
	{
		m_Font.GetUnicodeGroup(i, &gstart, &glen, &goffset);
		if (glen==temp && gstart==m_GroupStart && goffset==m_GroupOffset)
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

void CFontsDialog::OnCbnSelchangeGroupidxCombo()
{
	ReadUnicodeGroup();

	// update char mapping

	m_CharMapCombo1.SetCurSel(m_GroupOffset - 1);
	UpdateCharMap();
}

void CFontsDialog::UpdateCharMap()
{
	m_CharMapCombo2.SetCurSel(m_Font.GetCharMapping(m_CharMapCombo1.GetCurSel()));
	ReadCharMetrics();
	UpdateZoomView();
}

void CFontsDialog::OnCbnSelchangeCharmapidx2Combo()
{
	// write changes

	m_Font.SetCharMapping(m_CharMapCombo1.GetCurSel(), m_CharMapCombo2.GetCurSel());

	//
	ReadCharMetrics();
	UpdateZoomView();
}

void CFontsDialog::OnCbnSelchangeCharmapidxCombo()
{
	UpdateCharMap();
}

void CFontsDialog::WriteCharMetrics()
{
	UpdateData(TRUE);

	m_Font.SetCharMetrics(m_CharMapCombo2.GetCurSel(), (WORD)m_Offset1, (WORD)m_Offset2, (SHORT)m_Width, (SHORT)m_Ident);

	UpdateZoomView();
}

void CFontsDialog::ReadCharMetrics()
{
	int i = m_CharMapCombo2.GetCurSel();

	DWORD offset1 = 0, offset2 = 0;
	SHORT width = 0, ident = 0;

	m_Font.GetCharMetrics(i, &offset1, &offset2, &width, &ident);

	m_Offset1 = (WORD)offset1;
	m_Offset2 = (WORD)offset2;
	m_Width = width;
	m_Ident = ident;

	BOOL bEnable = !m_Font.IsFixedWidth();

	GetDlgItem(IDC_OFFSET1_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_OFFSET2_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_WIDTH_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_IDENT_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_UPDATE_BUTTON)->EnableWindow(bEnable);

	UpdateData(FALSE);
}

void CFontsDialog::OnEnChangeOffset1Edit()
{
	WriteCharMetrics();
}

void CFontsDialog::OnEnChangeOffset2Edit()
{
	WriteCharMetrics();
}

void CFontsDialog::OnEnChangeWidthEdit()
{
	WriteCharMetrics();
}

void CFontsDialog::OnEnChangeIdentEdit()
{
	WriteCharMetrics();
}

void CFontsDialog::OnEnChangeGroupstartEdit()
{
	WriteUnicodeGroup();
}

void CFontsDialog::OnEnChangeGrouplenEdit()
{
	WriteUnicodeGroup();
}

void CFontsDialog::OnEnChangeGroupoffsetEdit()
{
	WriteUnicodeGroup();
}

void CFontsDialog::UpdateCharCombo()
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

void CFontsDialog::UpdateGroupExplanation()
{
	m_GroupExplain.Format(TEXT("Explanation: Unicode characters U+%04X to U+%04X are mapped to entries %d to %d of the character mapping table."), m_GroupStart, m_GroupStart + m_GroupLen - 1, m_GroupOffset, m_GroupOffset + m_GroupLen - 1);

	UpdateData(FALSE);
}

void CFontsDialog::OnBnClickedLoadMetrics()
{
	CFileDialog dlg(TRUE, TEXT("ifm"), 0, OFN_HIDEREADONLY, TEXT("Fonts Metrics (*.ifm)|*.ifm||"), this);

	if (dlg.DoModal() != IDOK)
		return;

	CFile file;
	if (!file.Open(dlg.GetPathName(), CFile::modeRead))
	{
		MessageBox(TEXT("Unable to load file!"));
		return;
	}

	WORD offset1, offset2;
	SHORT width, ident;
	WORD c;

	//Old Version
	if (IsDlgButtonChecked(ID_CHK_NEWMETRICS) == 1)
	{
		while (TRUE)
		{
			if (file.Read(&c, 2) < 2)
				break;

			if (file.Read(&offset1, 2) < 2)
				break;
			if (file.Read(&offset2, 2) < 2)
				break;
			if (file.Read(&width, 2) < 2)
				break;
			if (file.Read(&ident, 2) < 2)
				break;
			m_Font.SetCharMetrics(m_Font.GetCharMapping(m_Font.GetUnicodeCharOffset(c)), offset1, offset2, width, ident);
		}
	}
	else
	{
		/*
		WORD index1, index2, start, len;

		//Make file size validity check
		if ((file.GetLength() - 8*m_Font.GetNumUnicodeGroups())%14 != 0)
			goto NotValid;

		while (TRUE)
		{
			if (file.Read(&index1, 2) < 2)
				break;
			if (file.Read(&start, 2) < 2)
				break;
			if (file.Read(&len, 2) < 2)
				break;
			if (file.Read(&offset1, 2) < 2)
				break;
			m_Font.SetUnicodeGroup(index1, start, len, offset1);

			if (file.GetPosition() == (8*m_Font.GetNumUnicodeGroups()))
				break;
		}

		while (TRUE)
		{
			if (file.Read(&index1, 2) < 2)
				break;
			if (file.Read(&index2, 2) < 2)
				break;

			if (file.Read(&c, 2) < 2)
				break;

			if (file.Read(&offset1, 2) < 2)
				break;
			if (file.Read(&offset2, 2) < 2)
				break;
			if (file.Read(&width, 2) < 2)
				break;
			if (file.Read(&ident, 2) < 2)
				break;
			m_Font.SetCharMapping(index1, index2);
			m_Font.SetCharMetrics(index2, offset1, offset2, width, ident);
		}
		*/
		DWORD size=m_Font.GetMetricDataLen();
		DWORD length=(DWORD)file.GetLength();
		if (length!=size)
			goto NotValid;

		LPBYTE lpBuf;
		lpBuf=new BYTE[size];
		if (file.Read(lpBuf, size) == size)
		{
			m_Font.SetMetricData(lpBuf);
		}
	}

	file.Close();

	// update
	UpdateFont();

	return;

NotValid:
	MessageBox(TEXT("The file you chose is not a valid metrics file!"));
}

void CFontsDialog::OnBnClickedSaveMetrics()
{
	CString filename;
	CString idx;
	m_FontIndexCombo.GetWindowText(idx);
	filename.Format(TEXT("%s-%s.ifm"), m_Font.GetFontName(), idx);

	CFileDialog dlg(FALSE, TEXT("ifm"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("Fonts Metrics (*.ifm)|*.ifm||"), this);
	if (dlg.DoModal() != IDOK)
		return;

	CFile file;
	if (!file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
	{
		MessageBox(TEXT("Unable to save file!"));
		return;
	}

	DWORD offset1, offset2;
	//WORD offset3;
	SHORT width, ident;
	WORD c, group;

	//Old version
	if (IsDlgButtonChecked(ID_CHK_NEWMETRICS) == 1)
	{
		for (WORD i = 0; i < m_Font.GetNumUnicodeChars(); i++)
		{
			c = m_Font.GetUnicodeChar(i, &group);

			m_Font.GetCharMetrics(m_Font.GetCharMapping(m_Font.GetUnicodeCharOffset(c)), &offset1, &offset2, &width, &ident);

			file.Write(&c, 2);
			file.Write(&offset1, 2);
			file.Write(&offset2, 2);
			file.Write(&width, 2);
			file.Write(&ident, 2);
		}
	}
	else
	{
		/*
		WORD index1, index2;

		WORD start;
		DWORD len;

		for (WORD i = 0; i < m_Font.GetNumUnicodeGroups(); i++)
		{
			m_Font.GetUnicodeGroup(i, &start, &len, &offset3);

			file.Write(&i, 2);
			file.Write(&start, 2);
			file.Write(&len, 2);
			file.Write(&offset3, 2);
		}

		for (WORD i = 0; i < m_Font.GetNumUnicodeChars(); i++)
		{
			c = m_Font.GetUnicodeChar(i, &group);

			m_Font.GetCharMetrics(m_Font.GetCharMapping(m_Font.GetUnicodeCharOffset(c)), &offset1, &offset2, &width, &ident);

			index1 = m_Font.GetUnicodeCharOffset(c);
			index2 = m_Font.GetCharMapping(index1);

			file.Write(&index1, 2);
			file.Write(&index2, 2);
			file.Write(&c, 2);
			file.Write(&offset1, 2);
			file.Write(&offset2, 2);
			file.Write(&width, 2);
			file.Write(&ident, 2);
		}*/
		LPBYTE temp=m_Font.GetMetricData();
		DWORD size;
		memcpy((LPBYTE)&size, temp, 4);
		file.Write(&temp[4], size);
	}

	file.Close();
}

void CFontsDialog::OnBnClickedMakeFont()
{
	CFontDialog dlg;

	if (dlg.DoModal() != IDOK)
		return;

	LOGFONT lf;
	memcpy(&lf, dlg.m_cf.lpLogFont, sizeof(LOGFONT));

	if (m_Font.GetFontBitDepth() > 1)
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

	WORD offset;

	CSize size = m_Font.GetFontBitmapSize();

	for (WORD i = 0; i < (WORD)m_Font.GetNumUnicodeChars(); i++)
	{
		c[0] = m_Font.GetUnicodeChar(i, &group);

		offset = i;
		m_Font.SetCharMapping(offset, offset);
		
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
		int start = 0, end = 0;
		BOOL bLineEmpty;

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
				start = x;
				break;
			}
		}

		for (x = rect.right - 1; x >= 0; x--)
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
				end = x;
				break;
			}
		}

		end++;
		// copy
		for (x = start; x < end; x++)
		{
			for (y = 0; y < height; y++)
			{
				m_Font.SetFontPixel(x + xoffset - start, y, dc.GetPixel(x, y));
			}

			for (y = height; y < size.cy; y++)
			{
				m_Font.SetFontPixel(x + xoffset - start, y, RGB(255, 255, 255));
			}
		}
		// update metrics

		m_Font.SetCharMetrics(offset, xoffset, xoffset + end - start, abcB + abcC + abcA, abcA);

		xoffset += end - start;

		if (xoffset >= size.cx)
		{
			for (i++;i < (WORD)m_Font.GetNumUnicodeChars(); i++)
			{
				m_Font.SetCharMapping(i, i);
				m_Font.SetCharMetrics(i, 0, 0, 1, 0);
			}
			break;
		}
	}

	//

	dc.SelectObject(pOldFont);
	dc.SelectObject(pBitmap);
	
	//

	ReleaseDC(pDC);

	UpdateFont();
}
void CFontsDialog::OnBnClickedOtfButton()
{
	if (m_pFirmware->GetNumOTFFonts() > 0)
		::SendMessage(::GetParent(::GetParent(m_hWnd)), WM_APP, (WPARAM)10, (LPARAM)TRUE);
	else
		MessageBox(TEXT("No OpenType fonts found in this firmware."));
}
