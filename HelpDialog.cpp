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

	//
	szTemp[0]='\0';
	GetPrivateProfileSection(TEXT("GenInfoAdvanced"), szTemp, 2048, theApp.m_IniPath);
	p = szTemp;
	i=0;
	do
	{
		p2=p;
		i=0;
		while(*p2++)
			i++;
		CString skey(p, i);
		s_new.SetString(skey.Left(skey.Find(TEXT("="))));
		s_new.AppendChar('\0');
		m_iPodNamesAdvanced1.Add(s_new); 
		s_new.SetString(skey.Right(skey.GetLength()-skey.Find(TEXT("="))-1));
		s_new.AppendChar('\0');
		m_iPodNamesAdvanced2.Add(s_new);
		while (*p++);
	} while (*p);

	OnCbnSelchangeiPodCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CHelpDialog::OnCbnSelchangeiPodCombo()
{
	CString s,s2;
	s.Format(TEXT("%d"), m_iPodCombo.GetCurSel()+1);
	TCHAR szTemp[512];
	if (GetPrivateProfileString(TEXT("FirmwareVersion"), s, NULL, szTemp, 512, theApp.m_IniPath)>0)
	{
		GetDlgItem(IDC_STATIC_FIRM)->SetWindowText(szTemp);
	}
	if (!_tcsncmp(szTemp, TEXT("13.6."), 5) || !_tcsncmp(szTemp, TEXT("14.5."), 5))
	{
		s2.Format(TEXT("0 = Slovak, 1 = Danish, 2 = German (Dates && Time), 3 = German\n4 = Russian (Dates && Times), 5 = Spanish, 6 = Finnish\n7 = French (Dates && Times), 8 = French (Dates && Times), 9 = French\n10 = Hungarian, 11 = Italian, 12 = Japanese\n13 = Korean, 14 = Dutch (Dates && Times), 15 = Dutch\n16 = Norwegian, 17 = Polish, 18 = Spanish (Dates && Times)\n19 = Russian, 20 = Swedish, 21 = Turkish\n22 = Simplified Chinese, 23 = Traditional Chinese, 24 = English"));
	}
	else
	{
		s2.Format(TEXT("0 = Danish, 1 = German (Dates && Time), 2 = German\n3 = Spanish, 4 = Finnish, 5 = French (Dates && Times)\n6 = French (Dates && Times), 7 = French, 8 = Italian\n9 = Japanese, 10 = Korean, 11 = Dutch (Dates && Times)\n12 = Dutch, 13 = Norwegian, 14 = Spanish (Dates && Times)\n15 = Swedish, 16 = Simplified Chinese, 17 = Traditional Chinese\n18 = English"));
	}
	GetDlgItem(IDC_STATIC_LANG)->SetWindowText(s2);
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
	int curr_genid=-1; //need to set this according to proper generation id of the iPod
	unsigned long i=0; //id representd the language block id that will be replaced with Hebrew

	//Get info about the iPod such as: Generation ID, Firmware Version
	//Try SCSI Inquiry method first
	//If it doesn't work, check SysInfo file (should be for 3G and below)

	unsigned char bresult[1024];
	memset(bresult, 0, sizeof(bresult));
	char *devstring=Unicode2MB(theApp.m_DeviceSel);
	//Get XML VPD (Vital Product Data - See SCSI Inquiry Documention) Data
	int retc=GetiPodSCSIInfo(devstring, 0x0c0, (unsigned char *)bresult);
	if (retc==0 && bresult[3]!=0)
	{
		//We got the data, now translate it and find what we need
		int num_pages=bresult[3];
		int start_pagecode=bresult[4];
		unsigned char device_data[20000];
		int pos=0;
		int w;
		for (w=start_pagecode;w<start_pagecode+num_pages;w++)
		{
			retc=GetiPodSCSIInfo(devstring, w, (unsigned char *)bresult);
			if (retc==0)
			{
				memcpy(&device_data[pos], &bresult[4], bresult[3]);
				pos+=bresult[3];
			}
		}
		device_data[pos]=0;
		char geninfo[3];
		int t=0;
		int tlen=pos-15;
		char *data=new char[tlen];
		memcpy(data, &device_data, tlen);
		for (w=0;w<tlen;w++)
		{
			if (strncmp(&data[w], "<key>OEMV</key>", 15)==0)
			{
				w+=25;
				pos=w;
				while (data[w]!='<')
				{
					geninfo[t]=data[w];
					w++;
					t++;
				}
				break;
			}
			else if (strncmp(&data[w], "<key>OEMU</key>", 15)==0)
			{
				w+=25;
				pos=w;
				while (data[w]!='<')
				{
					geninfo[t]=data[w];
					w++;
					t++;
				}
				break;
			}
		}
		//We found generation ID and stored it in geninfo
		geninfo[t]=0;
		curr_genid=atoi(geninfo);
	}
	delete devstring;

	BOOL bFound=FALSE;
	if (curr_genid>0)
	{
		CString sgen;
		sgen.Format(_T("%d"), curr_genid);
		WORD m;
		for (m=0;m < m_iPodNamesAdvanced1.GetCount();m++)
		{
			if (!m_iPodNamesAdvanced1.GetAt(m).Compare(sgen))
			{
				CString sfinal;
				sfinal.Format(TEXT("Your iPod generation is: %s."), m_iPodNamesAdvanced2.GetAt(m));
				MessageBox(sfinal, TEXT("Information"));
				bFound=TRUE;
			}
		}
		if (bFound==FALSE)
			MessageBox(TEXT("Either your iPod is from an unknown generation or iPodWizard doesn't know about it. Check www.iPodWizard.net website for updates concerning new iPods."));
		return;
	}

	CFile file;
	CString path;
	path.SetString(theApp.m_iPodDRV);
	path.AppendFormat(TEXT("\\SysInfo"));
	if (!file.Open(path, CFile::modeRead ))
	{
		MessageBox(TEXT("Unable to load SysInfo file! Make sure you chose the right drive."));
		return;
	}
	BYTE b;
	DWORD len;
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
	delete buffer;
	if (bFound==FALSE)
		MessageBox(TEXT("Either your iPod is from an unknown generation or iPodWizard doesn't know about it. Check www.iPodWizard.net website for updates concerning new iPods."));
	file.Close();
}

void CHelpDialog::DisableFind()
{
	m_bFindGen=TRUE;
}
