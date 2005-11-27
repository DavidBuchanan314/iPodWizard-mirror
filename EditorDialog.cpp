#include "stdafx.h"
#include "iPodWizard.h"
#include "EditorDialog.h"

IMPLEMENT_DYNAMIC(CEditorDialog, CDialog)

CEditorDialog::CEditorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorDialog::IDD, pParent)
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

	m_PropsTab.GetClientRect(&rect);
	m_PropsTab.AdjustRect(FALSE, &rect);

	m_PicsDialog.MoveWindow(rect);
	m_FontsDialog.MoveWindow(rect);
	m_StringDialog.MoveWindow(rect);

	UpdatePages();

	return TRUE;
}

void CEditorDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialog::OnOK();
}


void CEditorDialog::UpdatePages()
{
	int i = m_PropsTab.GetCurSel();

	switch (i)
	{
	case 0:
		m_PicsDialog.ShowWindow(SW_SHOW);
		m_FontsDialog.ShowWindow(SW_HIDE);
		m_StringDialog.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_PicsDialog.ShowWindow(SW_HIDE);
		m_FontsDialog.ShowWindow(SW_SHOW);
		m_StringDialog.ShowWindow(SW_HIDE);
		break;
	case 2:
		m_PicsDialog.ShowWindow(SW_HIDE);
		m_FontsDialog.ShowWindow(SW_HIDE);
		m_StringDialog.ShowWindow(SW_SHOW);
		break;
	}
}

void CEditorDialog::SetFirmware(CFirmware *pFirmware)
{
	m_PicsDialog.SetFirmware(pFirmware);
	m_FontsDialog.SetFirmware(pFirmware);
	m_StringDialog.SetFirmware(pFirmware);
}

void CEditorDialog::OnTcnSelchangePropTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdatePages();

	*pResult = 0;
}
