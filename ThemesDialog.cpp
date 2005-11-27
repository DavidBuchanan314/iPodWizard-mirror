#include "stdafx.h"
#include "iPodWizard.h"
#include "ThemesDialog.h"

#define NO_SCF_STR	_T("No SCF file loaded")

IMPLEMENT_DYNAMIC(CThemesDialog, CDialog)

CThemesDialog::CThemesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CThemesDialog::IDD, pParent)
	, m_pFirmware(NULL)
	, m_pStringDialog(NULL)
{
}

CThemesDialog::~CThemesDialog()
{
}

void CThemesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAPHICS_LIST, m_GraphicsList);
	DDX_Control(pDX, IDC_FONT_LIST, m_FontsList);
	DDX_Control(pDX, IDC_FONTIDX_CMB, m_FontIndexCombo);
}

BEGIN_MESSAGE_MAP(CThemesDialog, CDialog)
	ON_BN_CLICKED(IDC_LOADTHEME_BUTTON, OnBnClickedLoadTheme)
	ON_BN_CLICKED(IDC_MAKETHEME_BUTTON, OnBnClickedMakeTheme)
	ON_BN_CLICKED(IDC_LOADGRAPHICS_BUTTON, OnBnClickedLoadGraphics)
	ON_BN_CLICKED(IDC_DELETEGRAPHIC_BUTTON, OnBnClickedDeleteGraphic)
	ON_BN_CLICKED(IDC_LOADFONT_BUTTON, OnBnClickedLoadFont)
	ON_BN_CLICKED(IDC_DELETEFONT_BUTTON, OnBnClickedDeleteFont)
	ON_BN_CLICKED(IDC_LOADSCF_BUTTON, OnBnClickedLoadSCF)
	ON_BN_CLICKED(IDC_CLEARSCF_BUTTON, OnBnClickedClearSCF)
END_MESSAGE_MAP()

BOOL CThemesDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// initialize lists
	m_GraphicsList.SetExtendedStyle(m_GraphicsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_GraphicsList.InsertColumn(0, TEXT("#"), LVCFMT_LEFT, 30);
	m_GraphicsList.InsertColumn(1, TEXT("Path"), LVCFMT_LEFT, 320);

	m_FontsList.SetExtendedStyle(m_FontsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_FontsList.InsertColumn(0, TEXT("#"), LVCFMT_LEFT, 20);
	m_FontsList.InsertColumn(1, TEXT("Bitmap Path"), LVCFMT_LEFT, 250);
	m_FontsList.InsertColumn(2, TEXT("Metrics Path"), LVCFMT_LEFT, 250);

	return TRUE;
}

void CThemesDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialog::OnOK();
}

void CThemesDialog::SetFirmware(CFirmware *pFirmware, CStringDialog *pStringDialog)
{
	m_pFirmware = pFirmware;

	m_pStringDialog = pStringDialog;

	m_FontIndexCombo.ResetContent();

	CString s;
	for (DWORD i = 0; i < m_pFirmware->GetNumFonts(); i++)
	{
		s.Format(TEXT("%d"), i);
		m_FontIndexCombo.InsertString(i, s);
	}
	m_FontIndexCombo.SetCurSel(0);

	m_GraphicsList.DeleteAllItems();
	m_FontsList.DeleteAllItems();
	m_SCFPath.Empty();
	GetDlgItem(IDC_SCFPATH_STATIC)->SetWindowText(NO_SCF_STR);

	GetDlgItem(IDC_LOADTHEME_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_MAKETHEME_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOADGRAPHICS_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOADFONT_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOADSCF_BUTTON)->EnableWindow(TRUE);
}

void CThemesDialog::OnBnClickedMakeTheme()
{
	//The firmware of the desired iPod must be loaded.
	if (m_pFirmware==NULL)
	{
		MessageBox(TEXT("Can't make theme!\nYou must load the firmware you intend to put the theme on before you can make it."));
		return;
	}

	BOOL bGraphics=FALSE,bFonts=FALSE,bSCF=FALSE;
	//Check if the user loaded something to be on the theme
	if (m_GraphicsList.GetItemCount()>0)
		bGraphics=TRUE;
	if (m_FontsList.GetItemCount()>0)
		bFonts=TRUE;
	if (m_SCFPath.IsEmpty()==FALSE)
		bSCF=TRUE;
	if (!bGraphics && !bFonts && !bSCF)
	{
		MessageBox(TEXT("You need at least one type of resource to make a theme!"));
		return;
	}
	//Why bother to make a theme only for SCF file?
	if (bSCF && !bGraphics && !bFonts)
	{
		MessageBox(TEXT("You can't make a theme just for string changes, if you want string changes then just click 'Load Changes' in Strings tab in firmware editor and load the SCF file you saved earlier."));
		return;
	}

	CFileDialog dlg(FALSE, TEXT("ipw"), TEXT("MyTheme.ipw"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("iPodWizard Theme Files (*.ipw)|*.ipw||"), this);

	if (dlg.DoModal() != IDOK)
		return;

	CFile file;
	if (!file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
	{
		MessageBox(TEXT("Unable to save file!"));
		return;
	}

	CString s,s2,sMsg;
	CFile scffile;
	DWORD w,value;
	char sep[4]={83,69,67,84}; //SECTION SEPERATOR
	int x;

	//write current firmware version for later identification purposes:
	CString name;
	name=m_pFirmware->GetName();
	file.Write(name.GetBuffer(), name.GetAllocLength()*2);
	WORD z=0;
	file.Write(&z, 2);

	//write header:
	char empty[4]={"EMP"};
	if (bGraphics)
	{
		char graphics[4]={"GRP"};
		file.Write(&graphics, 4);
	}
	else
		file.Write(&empty, 4);
	if (bFonts)
	{
		char fonts[4]={"FNT"};
		file.Write(&fonts, 4);
	}
	else
		file.Write(&empty, 4);
	if (bSCF)
	{
		char strings[4]={"STR"};
		file.Write(&strings, 4);
	}
	else
		file.Write(&empty, 4);

	//write seperator
	if (bGraphics)
		file.Write(&sep, 4);

	//Prepare pictures
	for (x=0;x<m_GraphicsList.GetItemCount();x++)
	{
		s=m_GraphicsList.GetItemText(x, 0);
		if (_stscanf(s, TEXT("%d"), &value) != 1)
			value=0;
		s=m_GraphicsList.GetItemText(x, 1);
		CPicture pic;
		if (!pic.Read(m_pFirmware->GetPicture(value)))
		{
			sMsg.Format(TEXT("Error reading graphic, make sure firmware is loaded!"));
			goto readerror;
		}
		if (LoadPicture(&pic, s)==FALSE)
		{
			sMsg.Format(TEXT("Error loading graphic from specific file in list!"));
			goto readerror;
		}
		file.Write(&value, 4);
		w=pic.GetPictureRawLen();
		file.Write(&w, 4);
		file.Write(pic.GetData(), w);
	}

	//write seperator
	if (bFonts)
		file.Write(&sep, 4);

	//Prepare fonts
	for (x=0;x<m_FontsList.GetItemCount();x++)
	{
		s=m_FontsList.GetItemText(x, 0);
		if (_stscanf(s, TEXT("%d"), &value) != 1)
			value=0;
		s=m_FontsList.GetItemText(x, 1);
		CIpodFont font;
		LPBYTE lpEnd = m_pFirmware->GetFirmwareBuffer() + m_pFirmware->GetFirmwareSize();
		if (!font.Read(m_pFirmware->GetFont(value), lpEnd))
		{
			sMsg.Format(TEXT("Error reading font, make sure firmware is loaded!"));
			goto readerror;
		}
		s2=m_FontsList.GetItemText(x, 2);
		if (LoadFont(&font, s, s2)==FALSE)
		{
			sMsg.Format(TEXT("Error loading font from specific file in list! Either the bitmap or the metrics file is damaged."));
			goto readerror;
		}
		file.Write(&value, 4);
		w=font.GetFontBlockLen();
		file.Write(&w, 4);
		file.Write(m_pFirmware->GetFont(value), w);
	}

	//Prepare String Changes File
	LPBYTE buffer;
	if (!bSCF)
		goto nextstep;
	if (!scffile.Open(m_SCFPath, CFile::modeRead))
		goto nextstep;

	//write seperator
	file.Write(&sep, 4);

	w=(DWORD)scffile.GetLength();
	file.Write(&w, 4);
	buffer = new BYTE[w];
	if (scffile.Read(buffer, w) < w)
		goto readerror;
	file.Write(buffer, w);

nextstep:
	if (!bSCF)
		file.Write(&sep, 4);
	file.Close();
	MessageBox(TEXT("Successfully wrote theme file!"));
	return;

readerror:
	file.Close();
	DeleteFile(dlg.GetPathName());
	sMsg.AppendFormat(TEXT("\nAborting theme maker."));
	MessageBox(sMsg);
}

BOOL CThemesDialog::LoadFont(CIpodFont *pFont, LPCTSTR lpszBmpFilename, LPCTSTR lpszIfmFilename)
{
	//Bitmap
	CImage img;
	if (FAILED(img.Load(lpszBmpFilename)))
	{
		//MessageBox(TEXT("Unable to load image!"));
		return FALSE;
	}

	CSize size = pFont->GetFontBitmapSize();

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
			pFont->SetFontPixel(x, y, color);
		}
	}

	//Metrics
	CFile file;
	if (!file.Open(lpszIfmFilename, CFile::modeRead))
	{
		//MessageBox(TEXT("Unable to load file!"));
		return FALSE;
	}

	WORD offset1, offset2;
	SHORT width, ident;
	WORD c;

	WORD index1, index2, start, len;

	//Make file size validity check
	if ((file.GetLength() - 8*pFont->GetNumUnicodeGroups())%14 != 0)
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
		pFont->SetUnicodeGroup(index1, start, len, offset1);

		if (file.GetPosition() == (8*pFont->GetNumUnicodeGroups()))
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
		pFont->SetCharMapping(index1, index2);
		pFont->SetCharMetrics(index2, offset1, offset2, width, ident);
	}
	file.Close();

	return TRUE;

NotValid:
	return FALSE;
}

BOOL CThemesDialog::LoadPicture(CPicture *pPicture, LPCTSTR lpszFilename)
{
	CImage img;
	if (FAILED(img.Load(lpszFilename)))
	{
		return FALSE;
	}

	CSize size = pPicture->GetPictureSize();

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
			pPicture->SetPixel(x, y, color);
		}
	}

	return TRUE;
}

void CThemesDialog::OnBnClickedLoadTheme()
{
	//The firmware of the desired iPod must be loaded.
	if (m_pFirmware==NULL)
	{
		MessageBox(TEXT("Can't load theme!\nYou must load the firmware you intend to put the theme on before you can load the theme to it."));
		return;
	}

	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY, TEXT("iPodWizard Theme Files (*.ipw)|*.ipw||"), this);

	if (dlg.DoModal() != IDOK)
		return;

	CFile file;
	if (!file.Open(dlg.GetPathName(), CFile::modeRead))
	{
		MessageBox(TEXT("Can't open theme file!"));
		return;
	}

	int errid=0;
	CIpodFont font;
	DWORD index,size;
	LPBYTE buffer;
	char sep[4]={83,69,67,84}; //SECTION SEPERATOR

	//Read firmware version and check if it's good
	CString currname = m_pFirmware->GetName();
	CString name;
	LPWORD warr;
	WORD w;
	warr = new WORD[255];
	int n=0;
	while (TRUE)
	{
		if (file.Read(&w, 2) < 2)
		{
			errid=12;
			goto readerror;
		}
		warr[n]=w;
		n++;
		if (w==0)
			break;
	}
	name.Format(TEXT("%s"), (LPCTSTR)warr);

	//Check out what sections with have in this theme (graphics/fonts/strings)
	BOOL bGraphics=FALSE,bFonts=FALSE,bSCF=FALSE;
	char st[4]={0,0,0,0};
	if (file.Read(&st, 4) < 4)
	{
		errid=12;
		goto readerror;
	}
	if (!strcmp(st, "GRP"))
	{
		bGraphics=TRUE;
	}
	if (file.Read(&st, 4) < 4)
	{
		errid=12;
		goto readerror;
	}
	if (!strcmp(st, "FNT"))
	{
		bFonts=TRUE;
	}
	if (file.Read(&st, 4) < 4)
	{
		errid=12;
		goto readerror;
	}
	if (!strcmp(st, "STR"))
	{
		bSCF=TRUE;
	}
	//

	//Make validation checks to see if this theme is compatible with the current loaded firmware
	int bFirmCompare;
	if (name.Compare(currname)==0)
		bFirmCompare=0;
	else
	{
		currname = currname.Left(currname.Find(TEXT(".")));
		name = name.Left(name.Find(TEXT(".")));
		if (name.Compare(currname)==0)
			bFirmCompare=1;
		else
			bFirmCompare=2;
	}

	if (bFirmCompare==2)
	{
		file.Close();
		MessageBox(TEXT("Damage protection: not allowed to load theme!\nThe firmware you are trying to load this theme to is from a different iPod model than the iPod model the theme creator used."));
		return;
	}
	if (bSCF)
	{
		if (bFirmCompare!=0)
		{
			file.Close();
			MessageBox(TEXT("Damage protection: not allowed to load theme!\nThe theme you are about to load contains string changes but the firmware you want to load this theme to isn't the original firmware the theme creator used and it can screw you're iPod badly if you load it."));
			return;
		}
	}
	if ((bGraphics || bFonts) && bFirmCompare==1)
		if (MessageBox(TEXT("The theme you are about to load is from a different firmware revision than the current one in your updater (it's the same iPod model though). Do you still want to load this theme?"), TEXT("Warning"), MB_YESNO) != IDYES)
		{
			file.Close();
			return;
		}

	//Make sure section seperator exist and find out what section
	if (file.Read(&index, 4) < 4)
	{
		errid=6;
		goto readerror;
	}
	if (memcmp(&index, &sep, 4)!=0)
	{
		errid=7;
		goto readerror;
	}
	if (!bGraphics)
	{
		if (bFonts)
			goto fontstep;
		else if (bSCF)
			goto scfstep;
		else
			goto endstep;
	}
	
	//Parse pictures
	while (TRUE)
	{
		if (file.Read(&index, 4) < 4)
		{
			errid=5;
			goto readerror;
		}
		if (memcmp(&index, &sep, 4)==0)
			break;
		if (file.Read(&size, 4) < 4)
		{
			errid=1;
			goto readerror;
		}
		CPicture pic;
		if (!pic.Read(m_pFirmware->GetPicture(index)))
		{
			errid=2;
			goto readerror;
		}
		buffer = new BYTE[size];
		if (file.Read(buffer, size) < size)
		{
			errid=4;
			goto readerror;
		}
		pic.SetData(buffer, size);
	}

	if (!bFonts)
	{
		if (bSCF)
			goto scfstep;
		else
			goto endstep;
	}

fontstep:
	//Parse fonts
	LPBYTE lpEnd = m_pFirmware->GetFirmwareBuffer() + m_pFirmware->GetFirmwareSize();
	while (TRUE)
	{
		if (file.Read(&index, 4) < 4)
		{
			errid=5;
			goto readerror;
		}
		if (memcmp(&index, &sep, 4)==0)
			break;
		if (file.Read(&size, 4) < 4)
		{
			errid=1;
			goto readerror;
		}
		if (!font.Read(m_pFirmware->GetFont(index), lpEnd))
		{
			errid=3;
			goto readerror;
		}
		buffer = new BYTE[size];
		if (file.Read(buffer, size) < size)
		{
			errid=4;
			goto readerror;
		}
		font.SetData(buffer);
	}

	if (!bSCF)
		goto endstep;

scfstep:
	//Parse string changes file
	if (file.Read(&size, 4) < 4)
	{
		errid=10;
		goto readerror;
	}
	if (m_pStringDialog->LoadStringChanges(&file)==FALSE)
	{
		errid=11;
		goto readerror;
	}

endstep:
	file.Close();
	//reset stuff:
	m_GraphicsList.DeleteAllItems();
	m_FontsList.DeleteAllItems();
	m_SCFPath.Empty();
	//
	MessageBox(TEXT("Successfully loaded theme file!"));
	return;
	
readerror:
	file.Close();
	CString error;
	error.Format(TEXT("An error encountered while reading data from the theme file! Either the file is in use by another program or it is damaged!\nError: %d"), errid);
	MessageBox(error);
	return;
}

void CThemesDialog::OnBnClickedLoadGraphics()
{
	//The firmware of the desired iPod must be loaded.
	if (m_pFirmware==NULL)
	{
		MessageBox(TEXT("Can't load graphics!\nYou must load the firmware you intend to put the theme on before you can load graphics."));
		return;
	}

	LPITEMIDLIST il;
	TCHAR folderPath[MAX_PATH];

	BROWSEINFO br;
    br.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
    br.pidlRoot = NULL;
    br.pszDisplayName = folderPath;
    br.lpszTitle = TEXT("Select folder for graphics");
    br.ulFlags = BIF_RETURNONLYFSDIRS;
    br.lpfn = NULL;
    br.lParam = 0;
    br.iImage = 0;

	il = SHBrowseForFolder(&br);
	if (il == NULL)
		return;

	SHGetPathFromIDList(il, folderPath);

	WORD index;
	CString filename = folderPath;
	filename += "\\pic???.bmp";

	CString s,s3;
	int c=0;
	WIN32_FIND_DATA findData;
	HANDLE hFind = FindFirstFile(filename, &findData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			filename = folderPath;
			filename += "\\";
			filename += findData.cFileName;
			index = _wtoi(findData.cFileName + 3);
			
			if (index>=0 && index < m_pFirmware->GetNumPictures())
			{
				s3.Format(TEXT("%3d"), index);
				int j=-1;
				for (int x=0;x<m_GraphicsList.GetItemCount();x++)
				{
					s=m_GraphicsList.GetItemText(x, 0);
					if (s.Compare(s3)==0)
					{
						j=x;
						break;
					}
				}
				if (j==-1)
				{
					j=m_GraphicsList.GetItemCount();
					m_GraphicsList.InsertItem(j, s3);
				}
				m_GraphicsList.SetItemText(j, 1, filename);
				c++;
			}

		} while (FindNextFile(hFind, &findData) != 0);

		FindClose(hFind);

		s.Format(TEXT("Found %d valid picture(s)."), c);
		MessageBox(s);
	}
}

void CThemesDialog::OnBnClickedDeleteGraphic()
{
	POSITION pos = m_GraphicsList.GetFirstSelectedItemPosition();
	CWordArray items;
	while (pos)
	{
		items.Add((WORD)m_GraphicsList.GetNextSelectedItem(pos));
	}
	int y=0;
	for (int i=0;i<items.GetCount();i++)
	{
		int x=(int)items.GetAt(i);
		m_GraphicsList.DeleteItem(x-y);
		y++;
	}
}

void CThemesDialog::OnBnClickedLoadFont()
{
	if (m_pFirmware==NULL)
	{
		MessageBox(TEXT("Can't load font! Font index not selected.\nYou must load the firmware you intend to put the theme on before you can load a font with a corresponding index to the list."));
		return;
	}

	//Get font file
	CFileDialog dlg_b(TRUE, 0, 0, OFN_HIDEREADONLY, TEXT("Bitmap Files (*.bmp, *.gif, *.jpg)|*.bmp;*.gif;*.jpg||"), this);
	dlg_b.m_pOFN->lpstrTitle = TEXT("Open bitmap font file");

	if (dlg_b.DoModal() != IDOK)
		return;

	//Get metrics file
	CFileDialog dlg_m(TRUE, TEXT("ifm"), 0, OFN_HIDEREADONLY, TEXT("Fonts Metrics (*.ifm)|*.ifm||"), this);
	dlg_m.m_pOFN->lpstrTitle = TEXT("Open metrics file");

	if (dlg_m.DoModal() != IDOK)
		return;

	CString s,s2;
	s2.Format(TEXT("%d"), m_FontIndexCombo.GetCurSel());
	int i=-1;
	for (int x=0;x<m_FontsList.GetItemCount();x++)
	{
		s=m_GraphicsList.GetItemText(x, 0);
		if (s.Compare(s2)==0)
		{
			i=x;
			break;
		}
	}
	if (i==-1)
	{
		i=m_FontsList.GetItemCount();
		m_FontsList.InsertItem(i, s2);
	}
	s.Format(dlg_b.GetPathName());
	m_FontsList.SetItemText(i, 1, s);

	s.Format(dlg_m.GetPathName());
	m_FontsList.SetItemText(i, 2, s);
}

void CThemesDialog::OnBnClickedDeleteFont()
{
	POSITION pos = m_FontsList.GetFirstSelectedItemPosition();
	CWordArray items;
	while (pos)
	{
		items.Add((WORD)m_FontsList.GetNextSelectedItem(pos));
	}
	int y=0;
	for (int i=0;i<items.GetCount();i++)
	{
		int x=(int)items.GetAt(i);
		m_FontsList.DeleteItem(x-y);
		y++;
	}
}

void CThemesDialog::OnBnClickedLoadSCF()
{
	CFileDialog dlg(TRUE, TEXT("scf"), 0, OFN_HIDEREADONLY, TEXT("String Changes (*.scf)|*.scf||"), this);

	if (dlg.DoModal() != IDOK)
		return;

	m_SCFPath.Format(dlg.GetPathName());
	GetDlgItem(IDC_SCFPATH_STATIC)->SetWindowText(m_SCFPath);
}

void CThemesDialog::OnBnClickedClearSCF()
{
	m_SCFPath.Empty();
	GetDlgItem(IDC_SCFPATH_STATIC)->SetWindowText(NO_SCF_STR);
}