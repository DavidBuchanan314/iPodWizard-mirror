// PicturesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "iPodWizard.h"
#include "PicturesDialog.h"

// CPicturesDialog dialog

IMPLEMENT_DYNAMIC(CPicturesDialog, CDialog)
CPicturesDialog::CPicturesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPicturesDialog::IDD, pParent)
	, m_PictureWidth(0)
	, m_PictureHeight(0)
	, m_PictureBitdepth(0)
	, m_PictureIndex(0)
	, m_PictureType(0)
	, m_PictureID(0)
	, m_pFirmware(NULL)
{
}

CPicturesDialog::~CPicturesDialog()
{
}

void CPicturesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WIDTH_EDIT, m_PictureWidth);
	DDX_Text(pDX, IDC_HEIGHT_EDIT, m_PictureHeight);
	DDX_Text(pDX, IDC_BITDEPTH_EDIT, m_PictureBitdepth);
	DDX_Text(pDX, IDC_TYPE_EDIT, m_PictureType);
	DDX_Text(pDX, IDC_ID_EDIT, m_PictureID);
	DDX_Control(pDX, IDC_PICTURE, m_PictureWnd);
	DDX_Control(pDX, IDC_PICIDX_COMBO, m_PicIndexCombo);
}


BEGIN_MESSAGE_MAP(CPicturesDialog, CDialog)
	ON_BN_CLICKED(ID_PREV_PICTURE, OnBnClickedPrevPicture)
	ON_BN_CLICKED(ID_NEXT_PICTURE, OnBnClickedNextPicture)
	ON_BN_CLICKED(ID_LOAD_BITMAP, OnBnClickedLoadBitmap)
	ON_BN_CLICKED(ID_SAVE_BITMAP, OnBnClickedSaveBitmap)
	ON_CBN_SELCHANGE(IDC_PICIDX_COMBO, OnCbnSelchangePicidxCombo)
	ON_BN_CLICKED(ID_SAVE_ALL, OnBnClickedSaveAll)
	ON_BN_CLICKED(ID_LOAD_ALL, OnBnClickedLoadAll)
	ON_BN_CLICKED(ID_PASTE_IMAGE, &CPicturesDialog::OnBnClickedPasteImage)
	ON_BN_CLICKED(ID_COPY_IMAGE, &CPicturesDialog::OnBnClickedCopyImage)
	ON_MESSAGE(WM_APP, OnDropPictureFiles)
	ON_BN_CLICKED(ID_FIND_BITMAP_RSRC, &CPicturesDialog::OnBnClickedFindBitmapRsrc)
	ON_BN_CLICKED(IDC_THEMEPREVIEW_BUTTON, OnBnClickedThemePreview)
END_MESSAGE_MAP()


// CPicturesDialog message handlers

BOOL CPicturesDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_PictureWnd.EnableScrollBar(SB_BOTH, ESB_DISABLE_BOTH);
	m_PictureWnd.OnCreate(NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPicturesDialog::OnBnClickedThemePreview()
{
	//CThemePreviewDialog dlg;
	
	//dlg.SetFirmware(m_pFirmware, m_pEditorDialog);
	//dlg.DoModal();
	m_pThemePreview->SendMessage(BM_CLICK, 0, 0);
}

LRESULT CPicturesDialog::OnDropPictureFiles(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL)
		return 0;
	if (m_pFirmware==NULL)
		return 0;
	int mg=(int)wParam;
	
	if (mg==10)
	{
		LoadPictures((TCHAR *)lParam);
	}
	else if (mg==11)
	{
		LoadPicture(&m_Picture, (TCHAR *)lParam);
	}
	return 0;
}

void CPicturesDialog::OnBnClickedPrevPicture()
{
	if (m_PictureIndex > 0)
	{
		m_PictureIndex--;

		UpdatePicture();
	}
}

void CPicturesDialog::OnBnClickedNextPicture()
{
	if (m_PictureIndex < m_pFirmware->GetNumPictures() - 1)
	{
		m_PictureIndex++;

		UpdatePicture();
	}
}

void CPicturesDialog::OnBnClickedLoadBitmap()
{
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY, TEXT("Bitmap Files (*.bmp, *.gif, *.jpg)|*.bmp;*.gif;*.jpg||"), this);
	MO_LOAD_RESOURCES_PATH(dlg)

	if (dlg.DoModal() != IDOK)
		return;
	MO_SAVE_RESOURCES_PATH(dlg)
	if (!LoadPicture(&m_Picture, dlg.GetPathName()))
	{
		MessageBox(TEXT("Unable to load image!"));
		return;
	}

	UpdatePicture();
}

void CPicturesDialog::UpdatePicture()
{
	BOOL bEnable;
	LPBYTE lpBuffer = m_pFirmware->GetPicture(m_PictureIndex);
	if (lpBuffer == NULL)
		return;

	DWORD off_set=lpBuffer-m_pFirmware->GetFirmwareBuffer();

	if (m_pFirmware->GetPictureType(m_PictureIndex) != 0)
		m_Picture.SetPictureMode(1);
	else
		m_Picture.SetPictureMode(0);

	// check if partition can be written
	bEnable = m_pFirmware->CanWritePicture(m_PictureIndex);

	if (!m_Picture.Read(lpBuffer, FALSE, m_pFirmware->GetAUPDBuffer()))
		return;

	CSize picSize = m_Picture.GetPictureSize();
	m_PictureID = m_pFirmware->GetPictureID(m_PictureIndex);
	m_PictureWidth = picSize.cx;
	m_PictureHeight = picSize.cy;
	m_PictureBitdepth = m_Picture.GetPictureBitDepth();

	if (m_Picture.GetPictureMode()==1)
		m_PictureType = 2;
	else
		m_PictureType = 1;

	UpdateData(FALSE);

	m_PictureWnd.SetPicture(&m_Picture);

	m_PicIndexCombo.SetCurSel(m_PictureIndex);

	GetDlgItem(ID_LOAD_BITMAP)->EnableWindow(bEnable);
	GetDlgItem(ID_SAVE_BITMAP)->EnableWindow(TRUE);
	GetDlgItem(ID_SAVE_ALL)->EnableWindow(TRUE);
	GetDlgItem(ID_LOAD_ALL)->EnableWindow(TRUE);
	GetDlgItem(ID_PREV_PICTURE)->EnableWindow(TRUE);
	GetDlgItem(ID_NEXT_PICTURE)->EnableWindow(TRUE);
	GetDlgItem(ID_COPY_IMAGE)->EnableWindow(TRUE);
	GetDlgItem(ID_PASTE_IMAGE)->EnableWindow(TRUE);
	GetDlgItem(ID_FIND_BITMAP_RSRC)->EnableWindow(TRUE);
	GetDlgItem(IDC_THEMEPREVIEW_BUTTON)->EnableWindow(TRUE);
}

void CPicturesDialog::SetFirmware(CFirmware *pFirmware, CLayoutDialog *pLayoutDialog, CWnd *pThemePreview)
{
	m_pFirmware = pFirmware;

	m_pLayoutDialog = pLayoutDialog;

	m_pThemePreview = pThemePreview;

	m_PictureIndex = 0;

	m_PicIndexCombo.ResetContent();

	CString s;
	for (DWORD i = 0; i < m_pFirmware->GetNumPictures(); i++)
	{
		s.Format(TEXT("%d"), i);
		m_PicIndexCombo.InsertString(i, s);
	}
	m_PicIndexCombo.SetCurSel(0);

	UpdatePicture();
}

void CPicturesDialog::OnBnClickedSaveBitmap()
{
	CString filename;
	filename.Format(TEXT("pic%05d.bmp"), m_pFirmware->GetPictureID(m_PictureIndex));

	CFileDialog dlg(FALSE, TEXT("bmp"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("BMP Files (*.bmp)|*.bmp||"), this);
	MO_LOAD_RESOURCES_PATH(dlg)

	if (dlg.DoModal() != IDOK)
		return;
	MO_SAVE_RESOURCES_PATH(dlg)
	if (!SavePicture(&m_Picture, dlg.GetPathName()))
	{
		MessageBox(TEXT("Unable to save image!"));
	}
}

void CPicturesDialog::OnCbnSelchangePicidxCombo()
{
	m_PictureIndex = m_PicIndexCombo.GetCurSel();

	UpdatePicture();
}

void CPicturesDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialog::OnOK();
}

BOOL CPicturesDialog::SavePicture(CPicture *pPicture, LPCTSTR lpszFilename)
{
	if (pPicture == NULL || lpszFilename == NULL)
		return FALSE;

	CImage img;
	CSize size = pPicture->GetPictureSize();

	if (!img.Create(size.cx, size.cy, 24))
	{
		return FALSE;
	}

	int x, y;

	for (x = 0; x < size.cx; x++)
	{
		for (y = 0; y < size.cy; y++)
		{
			img.SetPixel(x, y, pPicture->GetPixel(x, y));
		}
	}

	if (FAILED(img.Save(lpszFilename)))
	{
		return FALSE;
	}

	return TRUE;
}

void CPicturesDialog::OnBnClickedSaveAll()
{
	CWaitCursor wait;
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

	SavePictures(folderPath);
}

void CPicturesDialog::SavePictures(LPCTSTR folderPath)
{
	CString filename;

	if (theApp.m_OldPicIndex==TRUE)
	{
		for (DWORD i = 0; i < m_pFirmware->GetNumPictures(); i++)
		{
			filename.Format(TEXT("%s\\pic%03d.bmp"), folderPath, i);
			CPicture pic;
			if (m_pFirmware->GetPictureType(m_PictureIndex) != 0)
				pic.SetPictureMode(1);
			else
				pic.SetPictureMode(0);
			if (pic.Read(m_pFirmware->GetPicture(i)))
			{
				if (!SavePicture(&pic, filename))
				{
					MessageBox(TEXT("Unable to save pictures!"));
					return;
				}
			}
		}
	}
	else
	{
		for (DWORD i = 0; i < m_pFirmware->GetNumPictures(); i++)
		{
			filename.Format(TEXT("%s\\pic%05d.bmp"), folderPath, m_pFirmware->GetPictureID(i));
			CPicture pic;
			if (m_pFirmware->GetPictureType(m_PictureIndex) != 0)
				pic.SetPictureMode(1);
			else
				pic.SetPictureMode(0);
			if (pic.Read(m_pFirmware->GetPicture(i)))
			{
				if (!SavePicture(&pic, filename))
				{
					MessageBox(TEXT("Unable to save pictures!"));
					return;
				}
			}
		}
	}
}

void CPicturesDialog::OnBnClickedLoadAll()
{
	CWaitCursor wait;
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

	CScanDialog dlg;
	dlg.Create(dlg.IDD);
	dlg.SetWindowText(_T("Loading pictures into firmware..."));
	dlg.ShowWindow(SW_SHOW);

	LoadPictures(folderPath);

	dlg.DestroyWindow();
}

void CPicturesDialog::LoadPictures(LPCTSTR folderPath)
{
	DWORD index;
	CString filename = folderPath;
	
	if (theApp.m_OldPicIndex==FALSE)
	{
		filename += "\\pic?????.bmp";

		WIN32_FIND_DATA findData;
		HANDLE hFind = FindFirstFile(filename, &findData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				filename = folderPath;
				filename += "\\";
				filename += findData.cFileName;
				index = m_pFirmware->GetPictureIndexFromID(_wtoi(findData.cFileName + 3));

				if (index!=0xFFFFFFFF)
				{
					CPicture pic;
					LPBYTE lpBuffer = m_pFirmware->GetPicture(index);
					if (lpBuffer != NULL)
					{
						if (m_pFirmware->GetPictureType(index) != 0)
							pic.SetPictureMode(1);
						else
							pic.SetPictureMode(0);

						if (pic.Read(lpBuffer) && m_pFirmware->CanWritePicture(index)==TRUE)
							LoadPicture(&pic, filename);
					}
				}
			} while (FindNextFile(hFind, &findData) != 0);

			FindClose(hFind);

			UpdatePicture();
		}
	}
	else
	{
		filename += "\\pic???.bmp";

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

				CPicture pic;
				LPBYTE lpBuffer = m_pFirmware->GetPicture(index);
				if (lpBuffer != NULL)
				{
					if (m_pFirmware->GetPictureType(index) != 0)
						pic.SetPictureMode(1);
					else
						pic.SetPictureMode(0);

					if (pic.Read(lpBuffer) && m_pFirmware->CanWritePicture(index)==TRUE)
						LoadPicture(&pic, filename);
				}
			} while (FindNextFile(hFind, &findData) != 0);

			FindClose(hFind);

			UpdatePicture();
		}
	}
}

BOOL CPicturesDialog::LoadPicture(CPicture *pPicture, LPCTSTR lpszFilename)
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

	if (pPicture->GetPictureMode()==1)
		theApp.m_BootPicChanged=TRUE;

	return TRUE;
}

void CPicturesDialog::OnBnClickedPasteImage()
{
	if (!OpenClipboard())
		return;
	HBITMAP hBm;
	CImage img;
	hBm = (HBITMAP)GetClipboardData(CF_BITMAP);
	if (hBm==NULL)
		return;
	img.Attach(hBm);
	CSize size = m_Picture.GetPictureSize();

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
			m_Picture.SetPixel(x, y, color);
		}
	}
	CloseClipboard();
	UpdatePicture();
}

void CPicturesDialog::OnBnClickedCopyImage()
{
	
	/*HBITMAP hBm;
	CImage img;
	

	if (!img.Create(size.cx, size.cy, 24))
		return;

	int x, y;
	COLORREF c;

	for (x = 0; x < size.cx; x++)
	{
		for (y = 0; y < size.cy; y++)
		{
			c = m_Picture.GetPixel(x, y);
			img.SetPixel(x, y, c);
		}
	}*/

	//CDC *dc;
	//CBitmap bm;
	//img.ReleaseDC();
	//dc=m_PictureWnd.GetDC();
	//dc.Attach(hdc);
	//bm.CreateCompatibleBitmap(m_PictureWnd.GetDC(), size.cx, size.cy);
	CSize size = m_Picture.GetPictureSize();
	HDC hdc=::GetDC(m_PictureWnd.m_hWnd);
	CDC dc;
	dc.Attach(hdc);
	CDC memDC;
    memDC.CreateCompatibleDC(&dc);
	CBitmap bm;
	bm.CreateCompatibleBitmap(&dc, size.cx, size.cy);
	CBitmap * oldbm = memDC.SelectObject(&bm);
    memDC.BitBlt(0, 0, size.cx, size.cy, &dc, 0, 0, SRCCOPY);

	this->OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, bm.m_hObject);
	CloseClipboard();
	memDC.SelectObject(oldbm);
     bm.Detach();  // make sure bitmap not deleted with CBitmap object
}

void CPicturesDialog::OnBnClickedFindBitmapRsrc()
{
	CString sID;
	sID.Format(_T("%d"), m_pFirmware->GetPictureID(m_PictureIndex));
	m_pLayoutDialog->GetDlgItem(IDC_FIND_EDIT)->SetWindowText(sID);
	m_pLayoutDialog->OnBnClickedFindButton();
	::SendMessage(::GetParent(::GetParent(m_hWnd)), WM_APP, (WPARAM)21, (LPARAM)0);
}
