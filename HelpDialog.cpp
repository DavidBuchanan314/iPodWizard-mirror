// HelpDialog.cpp : implementation file
//

#include "stdafx.h"
#include "iPodWizard.h"
#include "HelpDialog.h"
#include ".\helpdialog.h"
#include <windows.h>
#include <tchar.h>
#include <string.h>


// CHelpDialog dialog

IMPLEMENT_DYNAMIC(CHelpDialog, CDialog)
CHelpDialog::CHelpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpDialog::IDD, pParent)
{
}

CHelpDialog::~CHelpDialog()
{
}

void CHelpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPOD_COMBO, m_iPodCombo);
	DDX_Control(pDX, IDC_HYPERLINK, m_HyperLink);
}


BEGIN_MESSAGE_MAP(CHelpDialog, CDialog)
	ON_MESSAGE(WM_CLOSE, OnClose)
	ON_BN_CLICKED(ID_FINDGEN_BUTTON, OnBnClickedFindGen)
	ON_CBN_SELCHANGE(IDC_IPOD_COMBO, OnCbnSelchangeiPodCombo)
END_MESSAGE_MAP()


// CHelpDialog message handlers

BOOL CHelpDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_HyperLink.SetURL(_T("http://www.iPodWizard.net"));

	if (m_bFindGen==TRUE)
		GetDlgItem(ID_FINDGEN_BUTTON)->EnableWindow(FALSE);

	m_iPodCombo.ResetContent();
	m_iPodNames.RemoveAll();
	TCHAR szTemp[2048];
	szTemp[0]='\0';
	GetPrivateProfileSection(TEXT("GenInfo"), szTemp, 2048, theApp.m_IniPath);
	TCHAR* p = szTemp;
	TCHAR* p2;
	int i=0;
	CString s_new;
	do
	{
		p2=p;
		i=0;
		while(*p2++)
			i++;
		CString key(p, i);
		s_new.SetString(key.Left(key.Find(TEXT("="))));
		s_new.AppendChar('\0');
		m_iPodNames.Add(s_new); 
		s_new.SetString(key.Right(key.GetLength()-key.Find(TEXT("="))-1));
		s_new.AppendChar('\0');
		m_iPodCombo.AddString(s_new);
		while (*p++);
	} while (*p);
	m_iPodCombo.SetCurSel(0);
	OnCbnSelchangeiPodCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CHelpDialog::OnCbnSelchangeiPodCombo()
{
	CString s;
	s.Format(TEXT("%d"), m_iPodCombo.GetCurSel()+1);
	TCHAR szTemp[512];
	if (GetPrivateProfileString(TEXT("FirmwareVersion"), s, NULL, szTemp, 512, theApp.m_IniPath)>0)
	{
		GetDlgItem(IDC_STATIC_FIRM)->SetWindowText(szTemp);
	}
	if (GetPrivateProfileString(TEXT("MainFont"), s, NULL, szTemp, 512, theApp.m_IniPath)>0)
	{
		GetDlgItem(IDC_STATIC_FONT)->SetWindowText(szTemp);
	}
}

LRESULT CHelpDialog::OnClose(WPARAM wParam, LPARAM lParam)
{
	this->EndDialog(IDOK);
	return 0;
}

void CHelpDialog::OnBnClickedFindGen()
{
	CFile file;
	CString temp, path;
	path.SetString(theApp.m_iPodDRV);
	path.AppendFormat(TEXT("\\SysInfo"));
	if (!file.Open(path, CFile::modeRead ))
	{
		MessageBox(TEXT("Unable to load SysInfo file! Make sure you chose the right drive."));
		return;
	}
	BYTE b;
	DWORD len, i;
	len=(DWORD)file.GetLength();
	char *buffer = new char[len];
	i=0;
	while (TRUE)
	{
		if (file.Read(&b, 1) < 1)
			break;
		buffer[i]=(char)b;
		i++;
	}
	DWORD pos;
	CString s(buffer);
	CString s2;
	CString iPodGen;
	for (i=0; i<len; i++)
	{
		if ((len-i)>15)
		{
			s2.SetString(s.Mid(i,13));
			if (s2.Compare(TEXT("BoardHwName: ")) == 0)
			{
				pos=i+13;
				iPodGen.Append(s.Mid(pos,s.Find(TEXT("\n"), pos)-pos));
				break;
			}
			else
			{
				s2.SetString(s.Mid(i,16));
				if (s2.Compare(TEXT("pszBoardHwName: ")) == 0)
				{
					pos=i+16;
					iPodGen.Append(s.Mid(pos,s.Find(TEXT("\n"), pos)-pos));
					break;
				}
			}
		}
	}
	WORD j, k, last=0;
	BOOL bFound=FALSE;
	CString str;
	BOOL bCmp=FALSE;
	for (j=0;j < m_iPodCombo.GetCount();j++)
	{
		bCmp=FALSE;
		str.Format(TEXT("%s"), m_iPodNames.GetAt(j));
		if (str.Find(TEXT("|"),0)>0)
		{
			for (k=0,last=0;k<str.GetLength();k++)
			{
				if (str.Mid(k,1).Compare(TEXT("|"))==0)
				{
						if (iPodGen.Compare(str.Mid(last, k-last))==0)
							bCmp=TRUE;
						last=k+1;
				}
			}
			if (iPodGen.Compare(str.Mid(last, k-last+1))==0)
				bCmp=TRUE;
		}
		else if (iPodGen.Compare(str)==0)
			bCmp=TRUE;
		if (bCmp==TRUE)
		{
			CString final;
			m_iPodCombo.GetLBText(j, s2);
			final.Format(TEXT("Your iPod generation is: %s."), s2);
			MessageBox(final, TEXT("Information"));
			bFound=TRUE;
		}
	}
	if (bFound==FALSE)
		MessageBox(TEXT("Either your iPod is from an unknown generation or iPodWizard doesn't know about it. Check www.iPodWizard.net website for updates concerning new iPods."));
	file.Close();
}

void CHelpDialog::DisableFind()
{
	m_bFindGen=TRUE;
}
