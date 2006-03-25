// StringDialog.cpp : implementation file
//

#include "stdafx.h"
#include "iPodWizard.h"
#include "StringDialog.h"
#include ".\stringdialog.h"

// CStringDialog dialog

void DDX_MyTextAddrHex(CDataExchange *pDX, int nIDC, DWORD &value)
{
	CString s;

	if (pDX->m_bSaveAndValidate)
	{
		DWORD temp = 0;
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->GetWindowText(s);
		s.MakeLower();
		for (int i = 0; i < s.GetLength(); i++)
		{
			TCHAR c = ((LPCTSTR)s)[i];
			if (c >= '0' && c <= '9')
			{
				temp = (temp << 4) + (c - '0');
			}
			else if (c >= 'a' && c <= 'f')
			{
				temp = (temp << 4) + (c - 'a' + 10);
			}
			else
			{
				s.Format(TEXT("%08X"), value);
				pDX->m_pDlgWnd->GetDlgItem(nIDC)->SetWindowText(s);
				return;
			}
		}
		value=temp;
	}
	else
	{
		s.Format(TEXT("%08X"), value);
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->SetWindowText(s);
	}
}

IMPLEMENT_DYNAMIC(CStringDialog, CDialog)
CStringDialog::CStringDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CStringDialog::IDD, pParent)
	, m_pFirmware(NULL)
	, m_FindString(_T(""))
	, m_ChangeString(_T(""))
	, m_HexAddr(0)
	, m_bSFilter(FALSE)
	, m_bShowAll(FALSE)
{
}

CStringDialog::~CStringDialog()
{
}

void CStringDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STRING_LIST, m_StringList);
	DDX_Control(pDX, IDC_LANGSTRING_LIST, m_LangStringList);
	DDX_Control(pDX, IDC_HEX, m_HexWnd);
	DDX_Control(pDX, IDC_LANGIDX_COMBO, m_Langidx);
	DDX_Text(pDX, IDC_FIND_EDIT, m_FindString);
	DDX_Text(pDX, IDC_CHANGE_EDIT, m_ChangeString);
	DDX_MyTextAddrHex(pDX, IDC_ADDR_EDIT, m_HexAddr);
}


BEGIN_MESSAGE_MAP(CStringDialog, CDialog)
	ON_BN_CLICKED(IDC_FIND_BUTTON, OnBnClickedFindButton)
	ON_EN_CHANGE(IDC_FIND_EDIT, OnEnChangeFindEdit)
	ON_LBN_SELCHANGE(IDC_STRING_LIST, OnLbnSelchangeStringList)
	ON_BN_CLICKED(IDC_FINDNEXT_BUTTON, OnBnClickedFindnextButton)
	ON_BN_CLICKED(IDC_CHANGETEXT_BUTTON, OnBnClickedChangeTextButton)
	ON_EN_CHANGE(IDC_CHANGE_EDIT, OnEnChangeChangeEdit)
	ON_BN_CLICKED(IDC_CHECKTEXT_BUTTON, OnBnClickedCheckTextButton)
	ON_BN_CLICKED(IDC_SAVECHANGES_BUTTON, OnBnClickedSaveAllChangesButton)
	ON_BN_CLICKED(IDC_LOADCHANGES_BUTTON, OnBnClickedLoadChangesButton)
	ON_BN_CLICKED(IDC_JUMP_BUTTON, OnBnClickedJumpToButton)
	ON_EN_CHANGE(IDC_ADDR_EDIT, OnEnChangeAddrEdit)
	ON_BN_CLICKED(IDC_SFILTER_BUTTON, OnBnClickedStringFilterButton)
	ON_BN_CLICKED(IDC_VOLUMEHACK_BUTTON, OnBnClickedVolumeHackButton)
	ON_CBN_SELCHANGE(IDC_LANGIDX_COMBO, OnCbnSelchangeLangidxCombo)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LANGSTRING_LIST, OnLvnItemchangedLangstringList)
	//ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LANGSTRING_LIST, OnLvnItemchangedLangstringList)
	ON_BN_CLICKED(IDC_TRANSLATE_BUTTON, OnBnClickedTranslate)
END_MESSAGE_MAP()


BOOL WildMatch(CString sWild, CString sString, CString sLimitChar)
{
    BOOL bAny = FALSE;
    BOOL bNextIsOptional = FALSE;
    BOOL bAutorizedChar = TRUE;

    int i=0;
    int j=0;

    // Check all the string char by char
    while (i<sString.GetLength()) 
    {
      // Check index for array overflow
      if (j<sWild.GetLength())
      {
          // Manage '*' in the wildcard
          if (sWild[j]=='*') 
          {
          // Go to next character in the wildcard
          j++;

          // Enf of the string and wildcard end 
          // with *, only test string validity
          if (j>=sWild.GetLength()) 
          {
          // Check end of the string
          while (!sLimitChar.IsEmpty() && i<sString.GetLength()) 
          {
          // If this char is not ok, return false
          if (sLimitChar.Find(sString[i])<0)
              return FALSE;

          i++;
          }

          return TRUE;
          }

          bAny = TRUE;
          bNextIsOptional = FALSE;
          } 
          else 
          {
              // Optional char in the wildcard
              if (sWild[j]=='^')
              {
              // Go to next char in the wildcard and indicate 
              // that the next is optional
              j++;
 
              bNextIsOptional = TRUE;
              }
              else
              {
                bAutorizedChar = 
                  ((sLimitChar.IsEmpty()) || (sLimitChar.Find(sString[i])>=0));

                // IF :
                  if (// Current char match the wildcard
                    sWild[j] == sString[i] 
                    // '?' is used and current char is in autorized char list
                    || (sWild[j] == '?' && bAutorizedChar)
                    // Char is optional and it's not in the string
                    // and it's necessary to test if '*' make any 
                    // char browsing
                    || (bNextIsOptional && !(bAny && bAutorizedChar))) 
                    {
                    // If current char match wildcard, 
                    // we stop for any char browsing
                    if (sWild[j] == sString[i])
                        bAny = FALSE;

                    // If it's not an optional char who is not present,
                    // go to next
                    if (sWild[j] == sString[i] || sWild[j] == '?')
                        i++;

                    j++;

                    bNextIsOptional = FALSE;
                    } 
                    else
                    // If we are in any char browsing ('*') 
                    // and curent char is autorized
                    if (bAny && bAutorizedChar)
                        // Go to next
                        i++;
                    else
                        return FALSE;
               }
            }
        }
        else
        // End of the wildcard but not the 
        // end of the string => 
        // not matching
        return FALSE;
    }

    if (j<sWild.GetLength() && sWild[j]=='^')
    {
        bNextIsOptional = TRUE;
        j++;
    }


    // If the string is shorter than wildcard 
    // we test end of the 
    // wildcard to check matching
    while ((j<sWild.GetLength() && sWild[j]=='*') || bNextIsOptional)
    {
        j++;
        bNextIsOptional = FALSE;

        if (j<sWild.GetLength() && sWild[j]=='^')
        {
            bNextIsOptional = TRUE;
            j++;
        }
    }

    return j>=sWild.GetLength();
}

/////////////////////////////////////////////////////////////////////////////////////////
// Utf8Decode - converts UTF8-encoded string to the UCS2/MBCS format
void Utf8Decode( char* str, wchar_t** ucs2 )
{
	if ( str == NULL )
		return;

	int len = (int)strlen( str );
	if ( len < 2 ) {
		if ( ucs2 != NULL ) {
			*ucs2 = ( wchar_t* )malloc(( len+1 )*sizeof( wchar_t ));
			MultiByteToWideChar( CP_ACP, 0, str, len, *ucs2, len );
			( *ucs2 )[ len ] = 0;
		}
		return;
	}

	wchar_t* tempBuf = ( wchar_t* )alloca(( len+1 )*sizeof( wchar_t ));
	{
		wchar_t* d = tempBuf;
		BYTE* s = ( BYTE* )str;

		while( *s )
		{
			if (( *s & 0x80 ) == 0 ) {
				*d++ = *s++;
				continue;
			}

			if (( s[0] & 0xE0 ) == 0xE0 && ( s[1] & 0xC0 ) == 0x80 && ( s[2] & 0xC0 ) == 0x80 ) {
				*d++ = (( WORD )( s[0] & 0x0F) << 12 ) + ( WORD )(( s[1] & 0x3F ) << 6 ) + ( WORD )( s[2] & 0x3F );
				s += 3;
				continue;
			}

			if (( s[0] & 0xE0 ) == 0xC0 && ( s[1] & 0xC0 ) == 0x80 ) {
				*d++ = ( WORD )(( s[0] & 0x1F ) << 6 ) + ( WORD )( s[1] & 0x3F );
				s += 2;
				continue;
			}

			*d++ = *s++;
		}

		*d = 0;
	}

	if ( ucs2 != NULL ) {
		int fullLen = ( len+1 )*sizeof( wchar_t );
		*ucs2 = ( wchar_t* )malloc( fullLen );
		memcpy( *ucs2, tempBuf, fullLen );
	}

   WideCharToMultiByte( CP_ACP, 0, tempBuf, -1, str, len, NULL, NULL );
}

// CStringDialog message handlers

BOOL CStringDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_ADDR_EDIT))->LimitText(8);

	m_LangStringList.SetExtendedStyle(m_LangStringList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_LangStringList.InsertColumn(0, TEXT("ID"), LVCFMT_LEFT, 30);
	m_LangStringList.InsertColumn(1, TEXT("String Data"), LVCFMT_LEFT, 320);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CStringDialog::SetFirmware(CFirmware *pFirmware)
{
	m_pFirmware = pFirmware;

	// update strings
	m_Langidx.ResetContent();
	CString s;
	for (DWORD i=0;i<m_pFirmware->GetNumLangs();i++)
	{
		s.Format(TEXT("%d"),i);	
		m_Langidx.InsertString(i, s);
	}
	m_Langidx.InsertString(i, TEXT("Show All"));
	m_Langidx.SetCurSel(0);
	OnCbnSelchangeLangidxCombo();
	
	m_HexWnd.SetFirmware(m_pFirmware);
	GetDlgItem(IDC_STRING_LIST)->EnableWindow(TRUE);
	GetDlgItem(IDC_FIND_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHANGE_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHKWILD)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOADCHANGES_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_JUMP_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_ADDR_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHKHEX)->EnableWindow(TRUE);
	GetDlgItem(IDC_SFILTER_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_VOLUMEHACK_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_TRANSLATE_BUTTON)->EnableWindow(TRUE);

	//Zero string changes list
	m_StringChanges.RemoveAll();

	UpdateData(FALSE);
}

void CStringDialog::OnBnClickedJumpToButton()
{
	if (m_HexAddr<=m_pFirmware->GetFirmwareSize())
		m_HexWnd.SetOffset(m_HexAddr);
}

void CStringDialog::OnBnClickedStringFilterButton()
{
	if (!m_bShowAll)
	{
		MessageBox(TEXT("This option is only valid for Show All strings and not a single language block."));
		return;
	}

	CFileDialog dlg(TRUE, TEXT("txt"), 0, OFN_HIDEREADONLY, TEXT("String List Files (*.txt)|*.txt||"), this);

	if (dlg.DoModal() != IDOK)
		return;

	CFile file;
	if (!file.Open(dlg.GetPathName(), CFile::modeRead))
	{
		MessageBox(TEXT("Unable to load file!"));
		return;
	}
	BYTE c=0;
	CString s;
	CStringArray sfilter;
	sfilter.RemoveAll();
	while (TRUE)
	{
		s.Format(TEXT(""));
		while (TRUE)
		{
			c=-1;
			if (file.Read(&c, 1) < 1)
				goto endoffile;
			if (c==10)
			{
				if (s.Compare(TEXT(""))!=0)
					sfilter.Add(s);
				break;
			}
			if (c!=13)
				s.AppendChar(c);
		}
		s.Format(TEXT(""));
	}
endoffile:
	file.Close();
	if (s.Compare(TEXT(""))!=0)
		sfilter.Add(s);
	// update strings
	sfilter_w.RemoveAll();
	m_StringList.ResetContent();
	DWORD y=0;
	for (DWORD i = 0; i < GetNumStrings(m_Lang); i++)
	{
		LPCTSTR st = GetString(m_Lang, i);
		for (c=0;c<sfilter.GetCount();c++)
		{
			if (sfilter.GetAt(c).Compare(st)==0)
			{
				sfilter_w.Add(i);
				m_StringList.AddString(st);
				y++;
			}
		}
	}
	m_bSFilter=TRUE;
}

//VOLUME CAPPER START
void CStringDialog::OnBnClickedVolumeHackButton()
{
	CString s,a,b;
	int mode=m_pFirmware->isCapped();
	if (mode==0)
	{
		a.Format(TEXT("uncapped"));
		b.Format(TEXT("recap"));
	}
	else if (mode==1)
	{
		a.Format(TEXT("capped"));
		b.Format(TEXT("uncap"));
	}
	else
	{
		MessageBox(TEXT("Couldn't find volume capping mode, you're firmware might be too new and iPodWizard doesn't support it or there's a problem in it."));
		return;
	}
	s.Format(TEXT("VERY IMPORTANT: Volume hacking has been tested successfuly only on iPods 3/4/5G, Color, mini and nano, other models are not supported!\nIn addition to the volume hacking that can be done via 'Tweaks', you can also hack the volume limit internaly by changing the firmware itself for future use.\nThe current loaded firmware's volume is %s, do you want to %s it?"), a, b);
	if (MessageBox(s, TEXT("Volume Limit Hacking"), MB_YESNO) != IDYES)
		return;
	int ret;
	if (a.Compare(TEXT("capped"))==0)
		ret=m_pFirmware->unCap();
	else
		ret=m_pFirmware->reCap();
	//if you want to point to the offset in the firmware we changed the capping bytes, uncomment the two following lines
	//if (ret>0)
	//	m_HexWnd.SetOffset(ret);
}
//VOLUME CAPPER END

void CStringDialog::OnBnClickedFindButton()
{
	FindString(0);
}

void CStringDialog::OnBnClickedLoadChangesButton()
{
	CFile file;
	if (m_bShowAll)
	{
		CFileDialog dlga(TRUE, TEXT("scf"), 0, OFN_HIDEREADONLY, TEXT("String Changes (*.scf)|*.scf||"), this);

		if (dlga.DoModal() != IDOK)
			return;

		if (!file.Open(dlga.GetPathName(), CFile::modeRead))
		{
			MessageBox(TEXT("Unable to load file!"));
			return;
		}

		if (LoadStringChanges(&file)==FALSE)
		{
			file.Close();
			MessageBox(TEXT("The file you chose is not a correct String Changes File!"), TEXT("Error"));
		}
		else
		{
			file.Close();
			MessageBox(TEXT("Changed all strings successfully!"));
		}
	}
	else
	{
		CFileDialog dlgb(TRUE, TEXT("txt"), 0, OFN_HIDEREADONLY, TEXT("String List Files (*.txt)|*.txt||"), this);

		if (dlgb.DoModal() != IDOK)
			return;

		if (!file.Open(dlgb.GetPathName(), CFile::modeRead))
		{
			MessageBox(TEXT("Unable to load file!"));
			return;
		}
		
		DWORD filelen=(DWORD)file.GetLength();
		if (LoadLanguageBlock(&file, filelen,m_Lang)==FALSE)
		{
			file.Close();
			//MessageBox(TEXT("The file you chose is not a correct String List File!"), TEXT("Error"));
		}
		else
		{
			file.Close();
			MessageBox(TEXT("Loaded string block file successfully!"));
		}
		OnCbnSelchangeLangidxCombo();
	}
}

BOOL CStringDialog::LoadLanguageBlock(CFile *pFile, DWORD filelen, DWORD lang)
{
	if (lang==-1)
		return FALSE;

	LPBYTE lpBuf;
	lpBuf = new BYTE[filelen];
	if (pFile->Read(lpBuf, (UINT)filelen) < filelen)
	{
		MessageBox(TEXT("Unable to read file!"));
		return FALSE;
	}
	//pFile->Close();
	BYTE UTF8_START[3]={0xEF,0xBB,0xBF};
	if (memcmp(lpBuf, &UTF8_START, 3))
	{
		MessageBox(TEXT("The file is not a valid UTF8 text file!"));
		return FALSE;
	}
	CDWordArray lenlist,offsets;
	lenlist.RemoveAll();
	offsets.RemoveAll();
	DWORD start=3,y;
	for (y=3;y<filelen;y++)
	{
		if (y+1 < filelen)
		{
			if (*((LPWORD)&lpBuf[y])==0x0A0D)
			{
				offsets.Add(start);
				if (y>start)
					lenlist.Add(y-start);
				else
					lenlist.Add(0);
				y++;
				start=y+1;
			}
		}
	}
	offsets.Add(start);
	lenlist.Add(y-start);
	// update strings
	DWORD total=GetNumStrings(lang);
	if ((DWORD)offsets.GetCount()!=total)
	{
		MessageBox(TEXT("The number of strings in the text file must be identical to the number of strings on the iPod language block!\nIf you would change this, your iPod could freak out."));
		return FALSE;
	}
	if (m_pFirmware->ResetLangBlock(lang, filelen-(total-1))==FALSE)
	{
		MessageBox(TEXT("There is no room for all the strings combined, trim some strings and then try again."));
		return FALSE;
	}
	LPBYTE lpszBuffer,lpPos;
	DWORD z;
	for (DWORD i = 0;i < total; i++)
	{
		if (lenlist.GetAt(i)==0)
			m_pFirmware->AddLangString(lang, i, NULL, 0);
		else
		{
			lpPos = &lpBuf[offsets.GetAt(i)];
			lpszBuffer = new BYTE[lenlist.GetAt(i)];
			for (y=0,z=0;y<lenlist.GetAt(i);y++,z++)
			{
				if (y+2 < lenlist.GetAt(i))
				{
					if (*((LPWORD)&lpPos[y])==0x000A)
					{
						lpszBuffer[z]=0x0A;
						y++;
					}
					else if (*((LPWORD)&lpPos[y])==0x000D)
					{
						lpszBuffer[z]=0x0D;
						y++;
					}
					else
						lpszBuffer[z]=lpPos[y];
				}
				else
					lpszBuffer[z]=lpPos[y];
			}
			m_pFirmware->AddLangString(lang, i, lpszBuffer, lenlist.GetAt(i));
			delete[] lpszBuffer;
		}
	}
	delete[] lpBuf;
	return TRUE;
}

BOOL CStringDialog::LoadStringChanges(CFile *pFile)
{
	WORD i=0,x=0,y=0,c=-1;
	DWORD offset=-1;
	CWordArray str;
	TCHAR buf[4];
	while (TRUE)
	{
		offset=-1;
		if (pFile->Read(&offset, 4) < 4)
			break;
		else
			x++;
		str.RemoveAll();
		while (TRUE)
		{
			c=-1;
			if (pFile->Read(&c, 2) < 2)
				goto wrongfile;
			str.Add(c);
			if (c==0)
			{
				y++;			
				break;
			}
		}
		m_HexWnd.SetOffset(offset);
		for (i=0;i<str.GetCount();i++)
		{
			wsprintf(buf, TEXT("%02X"), str.GetAt(i));
			m_HexWnd.OnKeyDown((UINT)buf[0], 0, 0);
			m_HexWnd.OnKeyDown((UINT)buf[1], 0, 0);
		}
	}
	if (x!=y)
		goto wrongfile;
	return TRUE;

wrongfile:
	return FALSE;
}

void CStringDialog::OnBnClickedSaveAllChangesButton()
{
	CString filename;
	
	if (m_bShowAll)
	{
		filename.Format(TEXT("%s.scf"), m_pFirmware->GetName());

		CFileDialog dlga(FALSE, TEXT("scf"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("String Changes (*.scf)|*.scf||"), this);
		if (dlga.DoModal() != IDOK)
			return;

		CFile filea;
		if (!filea.Open(dlga.GetPathName(), CFile::modeCreate | CFile::modeWrite))
		{
			MessageBox(TEXT("Unable to save file!"));
			return;
		}
		WORD i=0,j=0,c=0;
		DWORD offset;
		for (i=0;i<m_StringChanges.GetCount();i++)
		{
			templ=m_StringChanges.GetAt(i);
			offset=templ->offset;
			filea.Write(&offset, 4);
			for (j=0;j<templ->b_text.GetCount();j++)
			{
				c=templ->b_text.GetAt(j);
				filea.Write(&c, 2);
			}
		}
		filea.Close();
	}
	else
	{
		filename.Format(TEXT("%s.txt"), m_pFirmware->GetName());

		CFileDialog dlgb(FALSE, TEXT("txt"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("String Language List (*.txt)|*.txt||"), this);
		if (dlgb.DoModal() != IDOK)
			return;

		CFile fileb;
		if (!fileb.Open(dlgb.GetPathName(), CFile::modeCreate | CFile::modeWrite))
		{
			MessageBox(TEXT("Unable to save file!"));
			return;
		}

		//CString s;
		BYTE UTF8_START[3]={0xEF,0xBB,0xBF};
		fileb.Write(UTF8_START, 3);
		/*
		BYTE UNICODE_START[2]={0xFF,0xFE};
		fileb.Write(UNICODE_START, 2);
		char buf[4]={13,0,10,0};
		*/
		DWORD num=GetNumStrings(m_Lang);
		char buf[2]={13,10};
		for (DWORD i=0;i<num;i++)
		{
			/*
			wchar_t *sOut;
			Utf8Decode((char *)m_pFirmware->GetLangString(m_Lang, i), &sOut);
			fileb.Write(sOut, (UINT)(_tcslen(sOut)*2));
			*/
			LPBYTE lpStart=(LPBYTE)m_pFirmware->GetLangString(m_Lang, i);
			LPBYTE lpPos=lpStart;
			DWORD stsize=m_pFirmware->GetLangStringSize(m_Lang, i);
			UINT size=0;
			while (lpPos[0]!=0x00 && (unsigned)(lpPos-lpStart)<stsize)
			{
				if (*((WORD *)lpPos)==0x0A0D)
				{
					size+=4;
					lpPos+=2;
				}
				else
				{
					size++;
					lpPos++;
				}
			}
			LPBYTE lpBuf;
			lpBuf = new BYTE[size];
			UINT y=0;
			lpPos=lpStart;
			while (lpPos[0]!=0x00 && (unsigned)(lpPos-lpStart)<stsize)
			{
				if (*((WORD *)lpPos)==0x0A0D)
				{
					lpBuf[y]=0x0D;
					lpBuf[y+1]=0x00;
					lpBuf[y+2]=0x0A;
					lpBuf[y+3]=0x00;
					y+=4;
					lpPos+=2;
				}
				else
				{
					lpBuf[y]=lpPos[0];
					y++;
					lpPos++;
				}
			}
			
			fileb.Write(lpBuf, size); //remove 0 feed
			delete[] lpBuf;

			if (i+1!=num)
				fileb.Write(&buf, 2);
		}
		fileb.Close();
	}
}

void CStringDialog::OnEnChangeFindEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	UpdateData(TRUE);

	BOOL bEnable = !m_FindString.IsEmpty() && m_pFirmware != NULL;
	GetDlgItem(IDC_FIND_BUTTON)->EnableWindow(bEnable);
	GetDlgItem(IDC_FINDNEXT_BUTTON)->EnableWindow(bEnable);
}

void CStringDialog::OnEnChangeChangeEdit()
{
	UpdateData(TRUE);

	BOOL bEnable = !m_ChangeString.IsEmpty() && m_pFirmware != NULL;
	GetDlgItem(IDC_CHANGETEXT_BUTTON)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECKTEXT_BUTTON)->EnableWindow(bEnable);
}

void CStringDialog::OnEnChangeAddrEdit()
{
	UpdateData(TRUE);
}

void CStringDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialog::OnOK();
}

void CStringDialog::OnLbnSelchangeStringList()
{
	int i = m_StringList.GetCurSel();

	m_HexWnd.SetOffset(GetStringOffset(i));
}

void CStringDialog::OnBnClickedFindnextButton()
{
	if (m_bShowAll)
		FindString(m_StringList.GetTopIndex());
	else
	{
		POSITION p=m_LangStringList.GetFirstSelectedItemPosition();
		FindString(m_LangStringList.GetNextSelectedItem(p));
	}
		//FindString(m_LangStringList.GetTopIndex());
}

void CStringDialog::OnBnClickedChangeTextButton()
{
	if (m_bShowAll)
	{
		WORD idx = m_StringList.GetCurSel();
		if (idx!=65535)
		{
			if (idx>=GetNumStrings(m_Lang))
			{
				MessageBox(TEXT("The 'iPod' string you are about to change is probably not safe to change (there should be 'Cancel' string after it)! Consider your actions and if you still want to, edit manualy in hex editor."), TEXT("Warning"));
				return;
			}
			CString s1,s2;
			s1.Format(TEXT("%s"),GetString(m_Lang, idx));
			s2.Format(TEXT("%s"),GetString(m_Lang, idx+1));
			if (s1.Compare(TEXT("iPod"))==0 && s2.Compare(TEXT("Cancel"))!=0)
			{
				if (MessageBox(TEXT("The 'iPod' string you are about to change is probably not safe to change (there should be 'Cancel' string after it)! Consider your actions, do you still want to change the string? (not safe)"), TEXT("Warning"), MB_YESNO) != IDYES)
					return;
			}
		}
	}

	int j,lenm;
	LPBYTE str;
	if (IsDlgButtonChecked(IDC_CHKHEX)==0)
	{
		//String change
		const int cchMultiByte = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)m_ChangeString, -1, NULL, 0, NULL, NULL);
		LPSTR lpa = new CHAR [cchMultiByte];
		if(lpa == NULL){
			return;
		}
		*lpa = '\0';
		const int nMultiCount = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)m_ChangeString, -1, lpa, cchMultiByte, NULL, NULL);
		if( nMultiCount <= 0 ){
			delete[] lpa;
			return;
		}

		lenm=nMultiCount-1; //remove 0
		str=new BYTE[lenm];
		for (j = 0; j < lenm; j++)
		{
			WORD c;
			if (lpa[j]<0)
			{
				c=256;
				c+=lpa[j];
			}
			else
				c=lpa[j];
			str[j]=(BYTE)c;
		}
	}
	else
	{
		//Hex values string
		lenm=m_ChangeString.GetLength();
		str=new BYTE[lenm/2];
		//for (int k=0,j=lenm-2;j >=0;j-=2,k++)
		for (int k=0,j=0;j<lenm;j+=2,k++)
		{
			swscanf(m_ChangeString.Mid(j, 2), TEXT("%X"), &str[k]);
		}
		lenm=lenm/2;
	}

	int p;
	if (m_bShowAll)
	{
		p = m_StringList.GetCurSel();
		templ = new STRING_CHANGE;
		templ->offset=m_HexWnd.m_CursorPos;
		templ->b_text.RemoveAll();
		for (j=0;j<lenm;j++)
			templ->b_text.Add((WORD)str[j]);
		templ->b_text.Add(0);
		m_StringChanges.Add(templ);
		m_pFirmware->SetData(templ->offset, str, lenm);
		BYTE temp=0;
		m_pFirmware->SetData(templ->offset+lenm, &temp, 1);
	}
	else
	{
		POSITION temp=m_LangStringList.GetFirstSelectedItemPosition();
		p = m_LangStringList.GetNextSelectedItem(temp);
		//p--;
		if (!m_pFirmware->SetLangString(m_Lang, p, str, lenm))
		{
			MessageBox(TEXT("There is not enough room to change this string!"));
			return;
		}
		m_HexWnd.SetOffset(GetStringOffset(p));
	}
	m_HexWnd.Invalidate();
	
	//Change the listbox item as well if appropriate
	LPBYTE str2;
	str2=new BYTE[lenm+1];
	memcpy(str2, str, lenm);
	str2[lenm]=0;
	const int cchWideChar = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)str2, -1, NULL, 0);
    LPWSTR lpw = new WCHAR [cchWideChar];
    if(lpw == NULL){
        return;
    }
    *lpw = L'\0';

    const int nUnicodeCount = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)str2, -1, lpw, cchWideChar);
    if( nUnicodeCount <= 0 ){
        delete[] lpw;
        return;
    }

	if (m_bShowAll)
	{
		DWORD off=GetStringOffset(p);
		if (off==m_HexWnd.m_CursorPos)
		{
			m_StringList.DeleteString(p);
			m_StringList.InsertString(p, (LPCTSTR)lpw);
			m_StringList.SetCurSel(p);
		}
	}
	else
	{
		m_LangStringList.SetItemText(p, 1, (LPCTSTR)lpw);
	}
	UpdateData(FALSE);
	//
	delete[] str;
	MessageBox(TEXT("String changed successfully."), TEXT("String Changer"), MB_OK);
	if (m_bShowAll)
		GetDlgItem(IDC_SAVECHANGES_BUTTON)->EnableWindow(TRUE);
}

void CStringDialog::OnBnClickedCheckTextButton()
{
	if (m_HexWnd.m_CursorHalf == 1)
	{
		MessageBox(TEXT("Can't check string changing when starting in a middle of a char. Point the cursor in Hex Editor to the start of the char."), TEXT("Note"), MB_OK);
		return;
	}
	if (m_bShowAll)
	{
		DWORD i;
		int cnt=0;
		i = m_HexWnd.m_CursorPos;
		if (m_HexWnd.m_Buffer[i] == 0 && m_HexWnd.m_Buffer[i-1] != 0)
		{
			MessageBox(TEXT("Can't change a string starting from this position because it will ruin the string before (joins it with the one you're about to add)."), TEXT("String Checker"), MB_OK);
			return;
		}
		while (TRUE)
		{
			if (m_HexWnd.m_Buffer[i] == 0)
				break;
			cnt++;
			i++;
		}
		int len=0;
		if (IsDlgButtonChecked(IDC_CHKHEX)==0)
		{
			char buffer[2048];
			int numofbytes = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)m_ChangeString, m_ChangeString.GetAllocLength(), buffer, sizeof(buffer), 0, 0);
			len=(int)strlen(buffer);
		}
		else
		{
			len=m_ChangeString.GetAllocLength()/2;
		}
		if (cnt == len)
			MessageBox(TEXT("String can be changed exactly, just click 'Change String'"), TEXT("String Checker"), MB_OK);
		else if (len < cnt)
			MessageBox(TEXT("String can be changed and you will be left with extra space (which you cannot really use)."), TEXT("String Checker"), MB_OK);
		else
			MessageBox(TEXT("String cannot be changed without ruining the next string(s) but it's still possible."), TEXT("String Checker"), MB_OK);
	}
	else
	{
		MessageBox(TEXT("Try changing the string using 'Change String' and it will auto tell you if the string is too big."), TEXT("Note"), MB_OK);
	}
}

void CStringDialog::FindString(int start)
{
	UpdateData(TRUE);

	if (m_FindString.IsEmpty())
		return;

	m_FindString.MakeLower();

	// search

	CWaitCursor wait;

	CString s;
	BOOL bFound;
	for (DWORD i = start + 1; i < GetNumStrings(m_Lang); i++)
	{
		s = (LPCTSTR)GetString(m_Lang, i);
		s.MakeLower();

		bFound=FALSE;
		if (IsDlgButtonChecked(IDC_CHKWILD) == 0)
		{
			if (s.Find(m_FindString) != -1)
				bFound=TRUE;
		}
		else
		{
			if (WildMatch(m_FindString, s, TEXT("")))
				bFound=TRUE;
		}
		if (bFound==TRUE)
		{
			if (m_bShowAll)
			{
				m_StringList.SetTopIndex(i);
				m_StringList.SetCurSel(i);
				OnLbnSelchangeStringList();
			}
			else
			{
				m_LangStringList.SetSelectionMark(i);
				LVITEM lim;
				lim.mask=LVIF_STATE;
				lim.iItem=i;
				lim.stateMask=LVIS_SELECTED;
				lim.state=LVIS_SELECTED;
				lim.iSubItem=0;
				m_LangStringList.SetItem(&lim);
				m_LangStringList.EnsureVisible(i, FALSE);
			}
			break;
		}
	}
}

void CStringDialog::OnCbnSelchangeLangidxCombo()
{
	int i=m_Langidx.GetCurSel();
	if (i+1==m_Langidx.GetCount())
		m_bShowAll=TRUE;
	else
		m_bShowAll=FALSE;
	m_Lang=i;
	if (m_bShowAll)
	{
		GetDlgItem(IDC_SAVECHANGES_BUTTON)->SetWindowText(TEXT("Save Changes"));
		GetDlgItem(IDC_LOADCHANGES_BUTTON)->SetWindowText(TEXT("Load Changes"));
		if (m_StringChanges.GetCount()==0)
			GetDlgItem(IDC_SAVECHANGES_BUTTON)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_SAVECHANGES_BUTTON)->SetWindowText(TEXT("Save List"));
		GetDlgItem(IDC_LOADCHANGES_BUTTON)->SetWindowText(TEXT("Load List"));
		GetDlgItem(IDC_SAVECHANGES_BUTTON)->EnableWindow(TRUE);
	}
	
	DWORD j;
	if (m_bShowAll)
	{
		m_StringList.ResetContent();
		
		for (j = 0; j < GetNumStrings(m_Lang); j++)
		{
			m_StringList.AddString(GetString(m_Lang, j));
		}
		m_LangStringList.ShowWindow(FALSE);
		m_StringList.ShowWindow(TRUE);
	}
	else
	{
		m_LangStringList.DeleteAllItems();
		CString temp;
		for (j = 0; j < GetNumStrings(m_Lang); j++)
		{
			temp.Format(TEXT("%d"), m_pFirmware->GetLangStringID(m_Lang, j));
			m_LangStringList.InsertItem(j, temp);
			m_LangStringList.SetItemText(j, 1, GetString(m_Lang, j));
		}
		m_StringList.ShowWindow(FALSE);
		m_LangStringList.ShowWindow(TRUE);
	}
}

LPCTSTR CStringDialog::GetString(DWORD lang, DWORD index)
{
	wchar_t *sOut;
	CString s;
	if (m_bShowAll)
	{
		s.Format(TEXT("%s"), m_pFirmware->GetString(index));
	}
	else
	{
		s.Format(TEXT("%s"), m_pFirmware->GetLangString(lang, index));
	}
	Utf8Decode((char *)s.GetBuffer(), &sOut);
	return (LPTSTR)sOut;
}

DWORD CStringDialog::GetNumStrings(DWORD lang)
{
	if (m_bShowAll)
	{
		return m_pFirmware->GetNumStrings();
	}
	else
	{
		return m_pFirmware->GetNumLangStrings(lang);
	}
}

DWORD CStringDialog::GetStringOffset(DWORD index)
{
	DWORD offset=0;
	if (!m_bSFilter)
	{
		if (m_bShowAll)
			offset = (DWORD)((LPBYTE)m_pFirmware->GetString(index) - m_pFirmware->GetFirmwareBuffer());
		else
			offset=(DWORD)((LPBYTE)m_pFirmware->GetLangString(m_Lang, index) - m_pFirmware->GetFirmwareBuffer());
	}
	else
		offset = (DWORD)((LPBYTE)m_pFirmware->GetString(sfilter_w.GetAt(index)) - m_pFirmware->GetFirmwareBuffer());
	return offset;
}

void CStringDialog::OnLvnItemchangedLangstringList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	//LPNMITEMACTIVATE pNMLV = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (pNMLV->uOldState!=0)
		return;

	//DWORD i = (DWORD)m_LangStringList.GetFirstSelectedItemPosition();
	//if (i==0)
	//	return;
	//i--;

	m_HexWnd.SetOffset(GetStringOffset(pNMLV->iItem));

	if (IsDlgButtonChecked(IDC_CHKHEX)==0)
	{
		m_ChangeString=GetString(m_Lang, pNMLV->iItem);
	}
	else
	{
		DWORD size=m_pFirmware->GetLangStringSize(m_Lang, pNMLV->iItem);
		LPBYTE lpPos=(LPBYTE)m_pFirmware->GetLangString(m_Lang, pNMLV->iItem);
		CString temp;
		temp.Format(TEXT(""));
		for (DWORD i=0;i<(size-1);i++)
		{
			temp.AppendFormat(TEXT("%2X"), lpPos[i]);
		}
		m_ChangeString.Format(temp);
		//
		GetDlgItem(IDC_CHANGETEXT_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECKTEXT_BUTTON)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);

	*pResult = 0;
}

void CStringDialog::OnBnClickedTranslate()
{
	if (m_Lang==m_pFirmware->GetNumLangs()-1)
	{
		MessageBox(TEXT("Choose another language than English"));
		return;
	}

	MessageBox(TEXT("This function compares the selected language with the English language and creates a text language file intended for that language but with English words so it will be easier to translate.\nTranslation will replace selected langauge.\nAfter you've translated the file, just use Load List."));

	CString filename;

	filename.Format(TEXT("%s.txt"), m_pFirmware->GetName());

	CFileDialog dlgb(FALSE, TEXT("txt"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("String Language List (*.txt)|*.txt||"), this);
	if (dlgb.DoModal() != IDOK)
		return;

	CFile fileb;
	if (!fileb.Open(dlgb.GetPathName(), CFile::modeCreate | CFile::modeWrite))
	{
		MessageBox(TEXT("Unable to save file!"));
		return;
	}

	BYTE UTF8_START[3]={0xEF,0xBB,0xBF};
	fileb.Write(UTF8_START, 3);
	char buf[2]={13,10};

	DWORD endid,firstid=m_pFirmware->GetNumLangs()-1,secid=m_Lang,i,j,sum1,sum2;
	sum1=GetNumStrings(firstid);
	sum2=GetNumStrings(secid);

	endid=0xFFFFFFFF;
	for (i=sum1-1;i>0;i--)
	{
		for (j=sum2-1;j>0;j--)
		{
			 if (m_pFirmware->GetLangStringID(firstid, i)==m_pFirmware->GetLangStringID(secid, j))
			 {
				 endid=m_pFirmware->GetLangStringID(firstid, i);
				 break;
			 }
		}
		if (endid!=0xFFFFFFFF)
				 break;
	}
	
	for (i=0;i<sum1;i++)
	{
		for (j=0;j<sum2;j++)
		{
			 if (m_pFirmware->GetLangStringID(firstid, i)==m_pFirmware->GetLangStringID(secid, j))
			 {
					LPBYTE lpStart=(LPBYTE)m_pFirmware->GetLangString(firstid, i);
					LPBYTE lpPos=lpStart;
					DWORD stsize=m_pFirmware->GetLangStringSize(firstid, i);
					UINT size=0;
					while (lpPos[0]!=0x00 && (unsigned)(lpPos-lpStart)<stsize)
					{
						if (*((WORD *)lpPos)==0x0A0D)
						{
							size+=4;
							lpPos+=2;
						}
						else
						{
							size++;
							lpPos++;
						}
					}
					LPBYTE lpBuf;
					lpBuf = new BYTE[size];
					UINT y=0;
					lpPos=lpStart;
					while (lpPos[0]!=0x00 && (unsigned)(lpPos-lpStart)<stsize)
					{
						if (*((WORD *)lpPos)==0x0A0D)
						{
							lpBuf[y]=0x0D;
							lpBuf[y+1]=0x00;
							lpBuf[y+2]=0x0A;
							lpBuf[y+3]=0x00;
							y+=4;
							lpPos+=2;
						}
						else
						{
							lpBuf[y]=lpPos[0];
							y++;
							lpPos++;
						}
					}
					
					fileb.Write(lpBuf, size); //remove 0 feed
					delete[] lpBuf;

					if (endid!=m_pFirmware->GetLangStringID(firstid, i))
						fileb.Write(&buf, 2);
			 }
		}
	}
	
	//__int32 temp;
	//for (DWORD i=0;i<GetNumStrings(13);i++)
	//{
	//	temp=m_pFirmware->GetLangStringID(13, i);
	//	fileb.Write(&temp, 4);
	//}

	fileb.Close();
}
