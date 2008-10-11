// ThemePreviewDialog.cpp : implementation file
//

#include "stdafx.h"
#include "iPodWizard.h"
#include "ThemePreviewer.h"


// CThemePreviewDialog dialog

IMPLEMENT_DYNAMIC(CThemePreviewDialog, CDialog)
CThemePreviewDialog::CThemePreviewDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CThemePreviewDialog::IDD, pParent)
{
}

CThemePreviewDialog::~CThemePreviewDialog()
{
}

void CThemePreviewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_ThemeWnd);
	DDX_Control(pDX, IDC_RESOLUTION_COMBO, m_ResCombo);
	DDX_Control(pDX, IDC_LAYOUT_COMBO, m_LayoutCombo);
	DDX_Control(pDX, IDC_MENU_COMBO, m_MenuCombo);
}


BEGIN_MESSAGE_MAP(CThemePreviewDialog, CDialog)
	ON_BN_CLICKED(IDC_MAINMENU_RADIO, &CThemePreviewDialog::OnBnClickedMainmenuRadio)
	ON_BN_CLICKED(IDC_NOWPLAYING_RADIO, &CThemePreviewDialog::OnBnClickedNowplayingRadio)
	ON_CBN_SELCHANGE(IDC_MENU_COMBO, &CThemePreviewDialog::OnCbnSelchangeMenuCombo)
	ON_CBN_SELCHANGE(IDC_LAYOUT_COMBO, &CThemePreviewDialog::OnCbnSelchangeLayoutCombo)
	ON_CBN_SELCHANGE(IDC_RESOLUTION_COMBO, &CThemePreviewDialog::OnCbnSelchangeResoultionCombo)
	ON_BN_CLICKED(IDC_LOADSCREEN_BUTTON, &CThemePreviewDialog::OnBnClickedLoadscreenButton)
	ON_BN_CLICKED(IDC_REFRESH_BUTTON, &CThemePreviewDialog::OnBnClickedRefreshButton)
	ON_BN_CLICKED(IDC_DRAWCOMBINE_CHECK, &CThemePreviewDialog::OnBnClickedDrawcombineCheck)
END_MESSAGE_MAP()


// CThemePreviewDialog message handlers

BOOL CThemePreviewDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ThemeWnd.SetFirmware(m_pFirmware, m_pEditorDialog);

	CString sID;
	for (int i=0;i<m_pEditorDialog->m_LayoutDialog.ViewList->size();i++)
	{
		sID.Format(_T("%d"), m_pEditorDialog->m_LayoutDialog.ViewList->at(i)->id);
		m_LayoutCombo.InsertString(m_LayoutCombo.GetCount(), sID);
	}

	for (int i=0;i<m_pEditorDialog->m_LayoutDialog.MenuList->size();i++)
	{
		sID.Format(_T("%d"), m_pEditorDialog->m_LayoutDialog.MenuList->at(i)->id);
		m_MenuCombo.InsertString(m_MenuCombo.GetCount(), sID);
	}

	for (int i=0;i<NUM_ELEM(RES_SIZES);i++)
		m_ResCombo.InsertString(m_ResCombo.GetCount(), RES_SIZES[i].res_str);

	m_ResCombo.SetCurSel(0);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CThemePreviewDialog::SetFirmware(CFirmware *pFirmware, CEditorDialog *pEditorDialog)
{
	m_pFirmware = pFirmware;

	m_pEditorDialog = pEditorDialog;
}

void CThemePreviewDialog::UpdateDisplay()
{
	this->UpdateWindow();
	MSG message;
	if (::PeekMessage(&message, this->m_hWnd, 0, 0, PM_REMOVE)) {
		// periodically (every 10000 cycles) check for messages
		// in particular the Timer message
		// Notice this is the message pump  we say earlier in WinMain loop!
            ::TranslateMessage(&message);
            ::DispatchMessage(&message);
    }

	this->GetParent()->UpdateWindow();
	if (::PeekMessage(&message, this->GetParent()->m_hWnd, 0, 0, PM_REMOVE)) {
		// periodically (every 10000 cycles) check for messages
		// in particular the Timer message
		// Notice this is the message pump  we say earlier in WinMain loop!
            ::TranslateMessage(&message);
            ::DispatchMessage(&message);
    }
}

void CThemePreviewDialog::OnBnClickedMainmenuRadio()
{
	// TODO: Add your control notification handler code here
}

void CThemePreviewDialog::OnBnClickedNowplayingRadio()
{
	// TODO: Add your control notification handler code here
}

void CThemePreviewDialog::OnCbnSelchangeMenuCombo()
{
	m_ThemeWnd.Reset(TRUE);
	CString sID;
	m_MenuCombo.GetLBText(m_MenuCombo.GetCurSel(), sID);
	int sel_id=_ttoi(sID.GetBuffer());
	sID.ReleaseBuffer();
	bool first=true;
	for (int i=0;i<m_pEditorDialog->m_LayoutDialog.MenuList->size();i++)
	{
		if (m_pEditorDialog->m_LayoutDialog.MenuList->at(i)->id==sel_id)
		{
			for (int j=0;j<m_pEditorDialog->m_LayoutDialog.MenuList->at(i)->num_elems;j++)
			{
				DWORD lang=m_pFirmware->GetNumLangs()-1;
				for (DWORD x=0;x<m_pFirmware->GetNumLangStrings(lang);x++)
				{
					if (m_pFirmware->GetLangStringID(lang, x)==m_pEditorDialog->m_LayoutDialog.MenuList->at(i)->MenuElemList.at(j)->element->textID2)
					{
						TextRecord *text_rec=new TextRecord;
						text_rec->lang=lang;
						text_rec->id=m_pFirmware->GetLangStringID(lang, x);
						text_rec->fontid=22196;
						text_rec->inner.x=-1;
						if (first)
						{
							text_rec->inner.y=30;
							first=false;
						}
						else
							text_rec->inner.y=-1;
						m_ThemeWnd.TextRecList.push_back(text_rec);
						break;
					}	
				}
			}
			break;
		}
	}
	m_ThemeWnd.Invalidate();
}

void CThemePreviewDialog::OnCbnSelchangeLayoutCombo()
{
	CString sID;
	m_LayoutCombo.GetLBText(m_LayoutCombo.GetCurSel(), sID);
	int sel_id=_ttoi(sID.GetBuffer());
	sID.ReleaseBuffer();
	m_ThemeWnd.SetData(sel_id, 0);
	/*
	for (int i=0;i<m_pEditorDialog->m_LayoutDialog.ViewList->size();i++)
	{
		if (m_pEditorDialog->m_LayoutDialog.ViewList->at(i)->id==sel_id)
		{
			m_FontWnd.SetData(sel_id, 0);
		}
	}
	*/
}

void CThemePreviewDialog::OnCbnSelchangeResoultionCombo()
{
	m_ThemeWnd.SetRes(RES_SIZES[m_ResCombo.GetCurSel()].resx, RES_SIZES[m_ResCombo.GetCurSel()].resy);
	m_ThemeWnd.Invalidate();
}

void CThemePreviewDialog::OnBnClickedLoadscreenButton()
{
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY, TEXT("iPod Theme Screen Files (*.txt)|*.txt||"), this);
	MO_LOAD_RESOURCES_PATH(dlg)

	if (dlg.DoModal() != IDOK)
		return;
	MO_SAVE_RESOURCES_PATH(dlg)
	
	ifstream inFile;
    
    inFile.open(dlg.GetPathName());
    if (!inFile) {
        MessageBox(_T("Can't open file!"));
		return;
    }

	m_ThemeWnd.Reset();
    
	char sLine[2000];
    while (inFile >> sLine) {
		if (!strcmp(sLine, "") || !strncmp(sLine, "#", 1))
			continue;

		char *pch = strtok (sLine,",");
		if (!strcmp(pch, "PIC"))
		{
			pch = strtok (NULL, ",");
			PicRecord *pic_rec=new PicRecord;
			for (int m=0;m<7 && pch != NULL;m++)
			{
				switch(m)
				{
				case 0:
					pic_rec->id=atoi(pch);
					break;
				case 1:
					pic_rec->rct.left=atoi(pch);
					break;
				case 2:
					pic_rec->rct.top=atoi(pch);
					break;
				case 3:
					pic_rec->rct.right=atoi(pch);
					break;
				case 4:
					pic_rec->rct.bottom=atoi(pch);
					break;
				case 5:
					pic_rec->inner.x=atoi(pch);
					break;
				case 6:
					pic_rec->inner.y=atoi(pch);
					break;
				}
				pch = strtok (NULL, ",");
			}
			m_ThemeWnd.PicRecList.push_back(pic_rec);
		}
		else if (!strcmp(pch, "TEXT"))
		{
			pch = strtok (NULL, ",");
			TextRecord *text_rec=new TextRecord;
			for (int m=0;m<5 && pch != NULL;m++)
			{
				switch(m)
				{
				case 0:
					text_rec->lang=atoi(pch);
					break;
				case 1:
					text_rec->id=atoi(pch);
					break;
				case 2:
					text_rec->inner.x=atoi(pch);
					break;
				case 3:
					text_rec->inner.y=atoi(pch);
					break;
				case 4:
					text_rec->fontid=atoi(pch);
					break;
				}
				pch = strtok (NULL, ",");
			}
			m_ThemeWnd.TextRecList.push_back(text_rec);
		}
    }
    
    inFile.close();

	m_ThemeWnd.Invalidate();
}

void CThemePreviewDialog::OnBnClickedRefreshButton()
{
	m_ThemeWnd.Invalidate();
}

void CThemePreviewDialog::OnBnClickedDrawcombineCheck()
{
	m_ThemeWnd.DrawCombined(IsDlgButtonChecked(IDC_DRAWCOMBINE_CHECK));
}
