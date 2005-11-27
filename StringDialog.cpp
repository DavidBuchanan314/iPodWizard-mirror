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
{
}

CStringDialog::~CStringDialog()
{
}

void CStringDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STRING_LIST, m_StringList);
	DDX_Control(pDX, IDC_HEX, m_HexWnd);
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

LPCTSTR CStringDialog::GetFirmwareString(DWORD i)
{
	wchar_t *sOut;
	Utf8Decode((char *)m_pFirmware->GetString(i), &sOut);
	return (LPTSTR)sOut;
}

BOOL CStringDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_ADDR_EDIT))->LimitText(8);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CStringDialog::SetFirmware(CFirmware *pFirmware)
{
	m_pFirmware = pFirmware;

	// update strings
	m_StringList.ResetContent();
	for (DWORD i = 0; i < m_pFirmware->GetNumStrings(); i++)
	{
		m_StringList.AddString(GetFirmwareString(i));
	}
	
	m_HexWnd.SetFirmware(m_pFirmware);
	GetDlgItem(IDC_STRING_LIST)->EnableWindow(TRUE);
	GetDlgItem(IDC_FIND_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHANGE_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHKWILD)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOADCHANGES_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_SAVECHANGES_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_JUMP_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_ADDR_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHKHEX)->EnableWindow(TRUE);
	GetDlgItem(IDC_SFILTER_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_VOLUMEHACK_BUTTON)->EnableWindow(TRUE);

	//Zero string changes list
	m_StringChanges.RemoveAll();
}

void CStringDialog::OnBnClickedJumpToButton()
{
	if (m_HexAddr<=m_pFirmware->GetFirmwareSize())
		m_HexWnd.SetOffset(m_HexAddr);
}

void CStringDialog::OnBnClickedStringFilterButton()
{
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
			if (c==13)
			{
				if (s.Compare(TEXT(""))!=0)
					sfilter.Add(s);
				break;
			}
			if (c!=10)
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
	for (DWORD i = 0; i < m_pFirmware->GetNumStrings(); i++)
	{
		LPCTSTR st = GetFirmwareString(i);
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
	s.Format(TEXT("VERY IMPORTANT: Volume hacking has been tested successfuly only on iPods till nano. 5G and above are not supported!\nIn addition to the volume hacking that can be done via 'Tweaks', you can also hack the volume limit internaly by changing the firmware itself for future use.\nThe current loaded firmware's volume is %s, do you want to %s it?"), a, b);
	if (MessageBox(s, TEXT("Volume Limit Hacking"), MB_YESNO) != IDYES)
		return;
	int ret;
	if (a.Compare(TEXT("capped"))==0)
		ret=m_pFirmware->unCap();
	else
		ret=m_pFirmware->reCap();
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
	CFileDialog dlg(TRUE, TEXT("scf"), 0, OFN_HIDEREADONLY, TEXT("String Changes (*.scf)|*.scf||"), this);

	if (dlg.DoModal() != IDOK)
		return;

	CFile file;
	if (!file.Open(dlg.GetPathName(), CFile::modeRead))
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
	
	filename.Format(TEXT("%s.scf"), m_pFirmware->GetName());

	CFileDialog dlg(FALSE, TEXT("scf"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("String Changes (*.scf)|*.scf||"), this);
	if (dlg.DoModal() != IDOK)
		return;

	CFile file;
	if (!file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
	{
		MessageBox(TEXT("Unable to save file!"));
		return;
	}
	WORD i=0,j=0,c=0;
	DWORD offset;
	for (i=0;i<m_StringChanges.GetCount();i++)
	{
		templ=new STRING_CHANGE;
		templ=m_StringChanges.GetAt(i);
		offset=templ->offset;
		file.Write(&offset, 4);
		for (j=0;j<templ->b_text.GetCount();j++)
		{
			c=templ->b_text.GetAt(j);
			file.Write(&c, 2);
		}
	}
	file.Close();
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

	DWORD offset=0;
	if (m_bSFilter==FALSE)
		offset = (DWORD)((LPBYTE)m_pFirmware->GetString(i) - m_pFirmware->GetFirmwareBuffer());
	else
		offset = (DWORD)((LPBYTE)m_pFirmware->GetString(sfilter_w.GetAt(i)) - m_pFirmware->GetFirmwareBuffer());

	m_HexWnd.SetOffset(offset);
}

void CStringDialog::OnBnClickedFindnextButton()
{
	FindString(m_StringList.GetTopIndex());
}

void CStringDialog::OnBnClickedChangeTextButton()
{
	WORD idx = m_StringList.GetCurSel();
	if (idx!=65535)
	{
		if (idx>=m_pFirmware->GetNumStrings())
		{
			MessageBox(TEXT("The 'iPod' string you are about to change is probably not safe to change (there should be 'Cancel' string after it)! Consider your actions and if you still want to, edit manualy in hex editor."), TEXT("Warning"));
			return;
		}
		CString s1,s2;
		s1.Format(TEXT("%s"),GetFirmwareString(idx));
		s2.Format(TEXT("%s"),GetFirmwareString(idx+1));
		if (s1.Compare(TEXT("iPod"))==0 && s2.Compare(TEXT("Cancel"))!=0)
		{
			if (MessageBox(TEXT("The 'iPod' string you are about to change is probably not safe to change (there should be 'Cancel' string after it)! Consider your actions, do you still want to change the string? (not safe)"), TEXT("Warning"), MB_YESNO) != IDYES)
				return;
		}
	}
	int len=0;
	int i=0;
	templ = new STRING_CHANGE;
	templ->offset=m_HexWnd.m_CursorPos;
	templ->b_text.RemoveAll();
	if (IsDlgButtonChecked(IDC_CHKHEX)==0)
	{
		//String change
		TCHAR buf[4];
		char buffer[2048];
		int numofbytes = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)m_ChangeString, m_ChangeString.GetAllocLength(), buffer, sizeof(buffer), 0, 0);
		len=(int)strlen(buffer);
		for (i = 0; i < len; i++)
		{
			WORD s;
			if (buffer[i]<0)
			{
				s=256;
				s+=buffer[i];
			}
			else
				s=buffer[i];
			templ->b_text.Add(s);
			wsprintf(buf, TEXT("%02X"), s);
			m_HexWnd.OnKeyDown((UINT)buf[0], 0, 0);
			m_HexWnd.OnKeyDown((UINT)buf[1], 0, 0);
		}
	}
	else
	{
		//Hex values string
		len=m_ChangeString.GetAllocLength();
		for (i = 0; i < len; i++)
		{
			m_HexWnd.OnKeyDown((UINT)((LPCTSTR)m_ChangeString.Mid(i, 1))[0], 0, 0);
			m_HexWnd.OnKeyDown((UINT)((LPCTSTR)m_ChangeString.Mid(i, 1))[1], 0, 0);
		}
	}
	templ->b_text.Add(0);
	m_HexWnd.OnKeyDown(48, 0, 0);
	m_HexWnd.OnKeyDown(48, 0, 0);
	m_StringChanges.Add(templ);

	//Change the listbox item as well if appropriate
	int p = m_StringList.GetCurSel();
	DWORD off;
	if (m_bSFilter==FALSE)
		off = (DWORD)((LPBYTE)m_pFirmware->GetString(p) - m_pFirmware->GetFirmwareBuffer());
	else
		off = (DWORD)((LPBYTE)m_pFirmware->GetString(sfilter_w.GetAt(p)) - m_pFirmware->GetFirmwareBuffer());
	if (off==m_HexWnd.m_CursorPos)
	{
		m_StringList.DeleteString(p);
		m_StringList.InsertString(p, m_ChangeString);
		m_StringList.SetCurSel(p);
	}
	//
	MessageBox(TEXT("String changed successfully."), TEXT("String Changer"), MB_OK);
	GetDlgItem(IDC_SAVECHANGES_BUTTON)->EnableWindow(TRUE);
	return;
}

void CStringDialog::OnBnClickedCheckTextButton()
{
	if (m_HexWnd.m_CursorHalf == 1)
	{
		MessageBox(TEXT("Can't check string changing when starting in a middle of a char. Point the cursor in Hex Editor to the start of the char."), TEXT("Note"), MB_OK);
		return;
	}
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
	for (DWORD i = start + 1; i < m_pFirmware->GetNumStrings(); i++)
	{
		s = (LPCTSTR)GetFirmwareString(i);
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
			m_StringList.SetTopIndex(i);
			m_StringList.SetCurSel(i);
			OnLbnSelchangeStringList();
			break;
		}
	}
}
