#include "stdafx.h"
#include "iPodWizard.h"
#include "EditorDialog.h"

IMPLEMENT_DYNAMIC(CEditorDialog, CDialog)

CEditorDialog::CEditorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorDialog::IDD, pParent)
	, bFonts(FALSE)
	, bOTFCreate(FALSE)
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

	m_PicsDialog.Create(m_PicsDialog.IDD, &m_PropsTab);
	m_FontsDialog.Create(m_FontsDialog.IDD, &m_PropsTab);
	m_StringDialog.Create(m_StringDialog.IDD, &m_PropsTab);
	m_OTFDialog.Create(m_OTFDialog.IDD, &m_PropsTab);

	m_PropsTab.GetClientRect(&rect);
	m_PropsTab.AdjustRect(FALSE, &rect);

	m_PicsDialog.MoveWindow(rect);
	m_FontsDialog.MoveWindow(rect);
	m_StringDialog.MoveWindow(rect);
	m_OTFDialog.MoveWindow(rect);

	UpdatePages(FALSE);

	return TRUE;
}

void CEditorDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialog::OnOK();
}

//Switch page when user clicked OTF Fonts/Bitmap Fonts
LRESULT CEditorDialog::OnSwitchFontSystem(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL)
		return 0;
	int mg=(int)wParam;
	if (mg!=10)
		return 0;
	bFonts=(BOOL)lParam;
	UpdatePages(bFonts);
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
		break;
	case 1: //Fonts
		m_PicsDialog.ShowWindow(SW_HIDE);
		m_StringDialog.ShowWindow(SW_HIDE);
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
		break;
	}
}

void CEditorDialog::SetFirmware(CFirmware *pFirmware)
{
	//Initialize firmware on all sections
	m_PicsDialog.SetFirmware(pFirmware);
	m_FontsDialog.SetFirmware(pFirmware);
	m_OTFDialog.SetFirmware(pFirmware);
	m_StringDialog.SetFirmware(pFirmware);
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