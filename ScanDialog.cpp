// ScanDialog.cpp : implementation file
//

#include "stdafx.h"
#include "iPodWizard.h"
#include "ScanDialog.h"
#include ".\scandialog.h"


// CScanDialog dialog

IMPLEMENT_DYNAMIC(CScanDialog, CDialog)
CScanDialog::CScanDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CScanDialog::IDD, pParent)
{
}

CScanDialog::~CScanDialog()
{
}

void CScanDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCAN_PROGRESS, m_ProgressCtrl);
}


BEGIN_MESSAGE_MAP(CScanDialog, CDialog)
	ON_MESSAGE(WM_APP, OnScanProgress)
END_MESSAGE_MAP()


// CScanDialog message handlers

BOOL CScanDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CScanDialog::ScanFirmware(CFirmware *pFirmware, DWORD size)
{
	if (pFirmware)
	{
		m_ProgressCtrl.SetRange32(0, pFirmware->GetFirmwareSize());
		m_ProgressCtrl.SetPos(0);

		pFirmware->ScanFirmware(this);
	}
	else
	{
		m_ProgressCtrl.SetRange32(0, size);
		m_ProgressCtrl.SetPos(0);
	}
}

LRESULT CScanDialog::OnScanProgress(WPARAM wParam, LPARAM lParam)
{
	if (wParam != NULL)
	{
		SetWindowText((LPCTSTR)wParam);
	}

	UpdateDisplay();

	m_ProgressCtrl.SetPos((int)lParam);

	m_ProgressCtrl.UpdateWindow();

	return 0;
}

void CScanDialog::UpdateDisplay()
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
