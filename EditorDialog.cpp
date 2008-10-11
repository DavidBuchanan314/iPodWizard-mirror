#include "stdafx.h"
#include "iPodWizard.h"
#include "EditorDialog.h"

IMPLEMENT_DYNAMIC(CEditorDialog, CDialog)

CEditorDialog::CEditorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorDialog::IDD, pParent)
	, bFonts(FALSE)
	, bOTFCreate(FALSE)
	, m_pThemePreview(NULL)
{
}

CEditorDialog::~CEditorDialog()
{
}

void CEditorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROP_TAB, m_PropsTab);
}

BEGIN_MESSAGE_MAP(CEditorDialog, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_PROP_TAB, OnTcnSelchangePropTab)
	ON_MESSAGE(WM_APP, OnSwitchFontSystem)
END_MESSAGE_MAP()

BOOL CEditorDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	// initialize pages

	CRect rect;
	m_PropsTab.InsertItem(0, TEXT("Pictures"));
	m_PropsTab.InsertItem(1, TEXT("Fonts"));
	m_PropsTab.InsertItem(2, TEXT("Strings"));
	m_PropsTab.InsertItem(3, TEXT("Layout"));

	m_PicsDialog.Create(m_PicsDialog.IDD, &m_PropsTab);
	m_FontsDialog.Create(m_FontsDialog.IDD, &m_PropsTab);
	m_StringDialog.Create(m_StringDialog.IDD, &m_PropsTab);
	m_OTFDialog.Create(m_OTFDialog.IDD, &m_PropsTab);
	m_LayoutDialog.Create(m_LayoutDialog.IDD, &m_PropsTab);

	m_PropsTab.GetClientRect(&rect);
	m_PropsTab.AdjustRect(FALSE, &rect);

	m_PicsDialog.MoveWindow(rect);
	m_FontsDialog.MoveWindow(rect);
	m_StringDialog.MoveWindow(rect);
	m_OTFDialog.MoveWindow(rect);
	m_LayoutDialog.MoveWindow(rect);

	UpdatePages(FALSE);

	return TRUE;
}

void CEditorDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialog::OnOK();
}

void CEditorDialog::SetPreviewButton(CWnd *pWnd)
{
	m_pThemePreview = pWnd;
}

//Switch page when user clicked OTF Fonts/Bitmap Fonts
LRESULT CEditorDialog::OnSwitchFontSystem(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL)
		return 0;
	if (m_pFirmware==NULL)
		return 0;
	int mg=(int)wParam;
	CString temp;
	CIpodFont fnt;
	COTFFont ofnt;
	bool found;
	switch(mg)
	{
	case 10:
		bFonts=(BOOL)lParam;
		UpdatePages(bFonts);
		break;
	case 11:
		m_StringDialog.m_Langidx.SetCurSel(m_StringDialog.m_Langidx.GetCount()-2);
		m_StringDialog.OnCbnSelchangeLangidxCombo();
		LRESULT Result;
		NMLISTVIEW dNMHDR;
		dNMHDR.uOldState = 0;
		temp.Format(_T("%d"), (int)lParam);
		int i;
		for (i=0;i<m_StringDialog.m_LangStringList.GetItemCount();i++)
		{
			if (m_StringDialog.m_LangStringList.GetItemText(i, 0).Compare(temp)==0)
				break;
		}
		if (i!=m_StringDialog.m_LangStringList.GetItemCount())
		{
			m_StringDialog.m_LangStringList.SetSelectionMark(i);
			LVITEM lim;
			lim.mask=LVIF_STATE;
			lim.iItem=i;
			lim.stateMask=LVIS_SELECTED;
			lim.state=LVIS_SELECTED;
			lim.iSubItem=0;
			m_StringDialog.m_LangStringList.SetItem(&lim);
			m_StringDialog.m_LangStringList.EnsureVisible(i, FALSE);

			dNMHDR.iItem = i;
			m_StringDialog.OnLvnItemchangedLangstringList((NMHDR *)&dNMHDR, &Result);
			m_PropsTab.SetCurSel(2);
			UpdatePages(bFonts);
		}
		else
			MessageBox(_T("String not found in firmware, it maybe generated on the fly on iPod program."));
		break;
	case 12:
		//if (m_PicsDialog.m_PicIndexCombo.GetCurSel()!=(int)lParam)
		if ((DWORD)lParam==0xFFFFFFFF)
			MessageBox(_T("Picture not found in firmware, it maybe generated on the fly on iPod program."));
		else
		{
			m_PicsDialog.m_PicIndexCombo.SetCurSel((int)lParam);
			m_PicsDialog.OnCbnSelchangePicidxCombo();
			m_PropsTab.SetCurSel(0);
			UpdatePages(bFonts);
		}
		break;
	case 13:
		resource_type_type_elem *type;
		type=(resource_type_type_elem *)lParam;
		DWORD j;
		int k;
		LPBYTE lpEnd;
		lpEnd=m_pFirmware->GetFirmwareBuffer()+m_pFirmware->GetFirmwareSize();
		found=false;
		unsigned char fstyle;
		fstyle=type->style;
		for (j=0;j<m_pFirmware->GetNumLangStrings(m_pFirmware->GetNumLangs()-1);j++)
		{
			if (m_pFirmware->GetLangStringID(m_pFirmware->GetNumLangs()-1, j)==type->fontID)
			{
				unsigned char style=0;
				wchar_t *sOut;
				Utf8Decode((char *)m_pFirmware->GetLangString(m_pFirmware->GetNumLangs()-1, j), &sOut);
				for (k=0;k<m_pFirmware->GetNumOTFFonts();k++)
				{
					if (ofnt.Read(m_pFirmware->GetOTFFont(k), TRUE))
					{
						if (!wcscmp(ofnt.GetFontStyle(), _T("Bold")))
							style=0;
						else if (!wcscmp(ofnt.GetFontStyle(), _T("Regular")))
							style=1;
						if (!wcscmp(ofnt.GetFontName(), sOut) && ofnt.GetFontSize()==type->size && fstyle==style)
						{
							bFonts=TRUE;
							m_OTFDialog.m_FontIndexCombo.SetCurSel(k);
							m_OTFDialog.OnCbnSelchangeFontidxCombo();
							m_PropsTab.SetCurSel(1);
							UpdatePages(bFonts);
							found=true;
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
							m_FontsDialog.m_FontIndexCombo.SetCurSel(k);
							m_FontsDialog.OnCbnSelchangeFontidxCombo();
							m_PropsTab.SetCurSel(1);
							UpdatePages(bFonts);
							found=true;
							break;
						}
					}
				}
				delete sOut;
				break;
			}
		}
		if (found==false)
			MessageBox(_T("Font not found in firmware, it maybe generated on the fly on iPod program."));
		break;
	case 21:
		//show layouts dialog
		m_PropsTab.SetCurSel(3);
		UpdatePages(bFonts);
		break;
	default:
		break;
	}
	return 0;
}

void CEditorDialog::UpdatePages(BOOL bFonts)
{
	int i = m_PropsTab.GetCurSel();

	switch (i)
	{
	case 0: //Pictures
		m_PicsDialog.ShowWindow(SW_SHOW);
		m_FontsDialog.ShowWindow(SW_HIDE);
		m_StringDialog.ShowWindow(SW_HIDE);
		m_OTFDialog.ShowWindow(SW_HIDE);
		m_LayoutDialog.ShowWindow(SW_HIDE);
		break;
	case 1: //Fonts
		m_PicsDialog.ShowWindow(SW_HIDE);
		m_StringDialog.ShowWindow(SW_HIDE);
		m_LayoutDialog.ShowWindow(SW_HIDE);
		if (bFonts==FALSE) //Either OTF or not
		{
			m_FontsDialog.ShowWindow(SW_SHOW);
			m_OTFDialog.ShowWindow(SW_HIDE);
		}
		else
		{
			m_FontsDialog.ShowWindow(SW_HIDE);
			m_OTFDialog.ShowWindow(SW_SHOW);
		}
		break;
	case 2: //Strings
		m_PicsDialog.ShowWindow(SW_HIDE);
		m_FontsDialog.ShowWindow(SW_HIDE);
		m_StringDialog.ShowWindow(SW_SHOW);
		m_OTFDialog.ShowWindow(SW_HIDE);
		m_LayoutDialog.ShowWindow(SW_HIDE);
		break;
	case 3: //layouts
		m_PicsDialog.ShowWindow(SW_HIDE);
		m_FontsDialog.ShowWindow(SW_HIDE);
		m_StringDialog.ShowWindow(SW_HIDE);
		m_OTFDialog.ShowWindow(SW_HIDE);
		m_LayoutDialog.ShowWindow(SW_SHOW);
		break;
	}
}

void CEditorDialog::SetFirmware(CFirmware *pFirmware)
{
	//Initialize firmware on all sections
	m_PicsDialog.SetFirmware(pFirmware, &m_LayoutDialog, m_pThemePreview);
	m_FontsDialog.SetFirmware(pFirmware, &m_LayoutDialog);
	m_OTFDialog.SetFirmware(pFirmware, &m_LayoutDialog);
	m_StringDialog.SetFirmware(pFirmware);
	m_LayoutDialog.SetFirmware(pFirmware);
	m_pFirmware=pFirmware;
	if (pFirmware->GetNumOTFFonts()==0) //if no OTF fonts then show only bitmap (even if before user was on OTF)
	{
		bFonts=FALSE;
		UpdatePages(bFonts);
	}
}

void CEditorDialog::OnTcnSelchangePropTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	//show page according to user selection
	UpdatePages(bFonts);

	*pResult = 0;
}