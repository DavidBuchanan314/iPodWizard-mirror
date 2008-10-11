// LayoutDialog.cpp : implementation file
//

#include "stdafx.h"
#include "iPodWizard.h"
#include "LayoutDialog.h"
#include ".\LayoutDialog.h"

// CLayoutDialog dialog

void DDX_MyTextAddrHex2(CDataExchange *pDX, int nIDC, DWORD &value)
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

IMPLEMENT_DYNAMIC(CLayoutDialog, CDialog)
CLayoutDialog::CLayoutDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLayoutDialog::IDD, pParent)
	, m_pFirmware(NULL)
	, m_FindString(_T(""))
	, m_ChangeString(_T(""))
	, m_HexAddr(0)
	, m_bSFilter(FALSE)
{
}

CLayoutDialog::~CLayoutDialog()
{
}

void CLayoutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESOURCE_LIST, m_ResourceList);
	DDX_Control(pDX, IDC_RESOURCE_TREE, m_ResourceTree);
	DDX_Control(pDX, IDC_HEX, m_HexWnd);
	DDX_Control(pDX, IDC_LANGIDX_COMBO, m_ResourceTypeIdx);
	DDX_Text(pDX, IDC_FIND_EDIT, m_FindString);
	DDX_Text(pDX, IDC_CHANGE_EDIT, m_ChangeString);
	DDX_MyTextAddrHex2(pDX, IDC_ADDR_EDIT, m_HexAddr);
}


BEGIN_MESSAGE_MAP(CLayoutDialog, CDialog)
	ON_BN_CLICKED(IDC_FIND_BUTTON, OnBnClickedFindButton)
	ON_EN_CHANGE(IDC_FIND_EDIT, OnEnChangeFindEdit)
	ON_BN_CLICKED(IDC_FINDNEXT_BUTTON, OnBnClickedFindnextButton)
	ON_BN_CLICKED(IDC_CHANGETEXT_BUTTON, OnBnClickedChangeTextButton)
	ON_EN_CHANGE(IDC_CHANGE_EDIT, OnEnChangeChangeEdit)
	ON_BN_CLICKED(IDC_SAVECHANGES_BUTTON, OnBnClickedSaveResourceButton)
	ON_BN_CLICKED(IDC_LOADCHANGES_BUTTON, OnBnClickedLoadChangesButton)
	ON_BN_CLICKED(IDC_JUMP_BUTTON, OnBnClickedJumpToButton)
	ON_EN_CHANGE(IDC_ADDR_EDIT, OnEnChangeAddrEdit)
	ON_CBN_SELCHANGE(IDC_LANGIDX_COMBO, OnCbnSelchangeResourceTypeidxCombo)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_RESOURCE_LIST, OnLvnItemchangedResourceList)
	ON_NOTIFY(NM_DBLCLK, IDC_RESOURCE_LIST, OnLvnItemDblClickedResourceList)
	ON_NOTIFY(NM_CLICK, IDC_RESOURCE_LIST, OnLvnItemClickedResourceList)
	//ON_NOTIFY(LVN_ITEMACTIVATE, IDC_RESOURCE_LIST, OnLvnItemchangedResourceList)
	ON_NOTIFY(TVN_SELCHANGED, IDC_RESOURCE_TREE, &CLayoutDialog::OnTvnSelchangedResourceTree)
	ON_BN_CLICKED(IDC_SAVEALLRESOURCES_BUTTON, &CLayoutDialog::OnBnClickedSaveallresourcesButton)
	ON_BN_CLICKED(IDC_LOADALLRESOURCES_BUTTON, &CLayoutDialog::OnBnClickedLoadallresourcesButton)
	ON_BN_CLICKED(IDC_SELECTORBAR_BUTTON, &CLayoutDialog::OnBnClickedSelectorBarButton)
END_MESSAGE_MAP()

// CLayoutDialog message handlers

BOOL CLayoutDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	reinterpret_cast<CEdit*>(GetDlgItem(IDC_ADDR_EDIT))->LimitText(8);

	m_ResourceList.SetExtendedStyle(m_ResourceList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	this->CheckDlgButton(IDC_RADIO_DECVAL, 1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLayoutDialog::SetFirmware(CFirmware *pFirmware)
{
	m_pFirmware = pFirmware;

	// update strings
	m_ResourceTypeIdx.ResetContent();
	CString s;
	
	ViewList=m_pFirmware->GetViewList();
	MenuList=m_pFirmware->GetMenuList();
	TypeList=m_pFirmware->GetTypeList();
	TCMDList=m_pFirmware->GetTCMDList();
	m_ResourceTypeIdx.InsertString(0, _T("Scheme Layouts"));
	m_ResourceTypeIdx.InsertString(1, _T("Menus"));
	m_ResourceTypeIdx.InsertString(2, _T("Font Types"));
	m_ResourceTypeIdx.InsertString(3, TEXT("Command Table"));
	m_ResourceTypeIdx.SetCurSel(0);

	//MessageBox(_T("S"));

	if (ViewList==NULL || MenuList==NULL || TypeList==NULL)
		goto disable;
	if (ViewList->size()<1 || MenuList->size()<1 || TypeList->size()<1)
		goto disable;

	if (theApp.m_LayoutRender==FALSE)
		OnCbnSelchangeResourceTypeidxCombo();
	else
		m_ResourceTree.DeleteAllItems();
	
	m_HexWnd.SetFirmware(m_pFirmware);
	GetDlgItem(IDC_FIND_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHANGE_EDIT)->EnableWindow(TRUE);
	//GetDlgItem(IDC_CHKWILD)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOADCHANGES_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_JUMP_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_ADDR_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_STRINGVAL)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_HEXVAL)->EnableWindow(TRUE);
	GetDlgItem(IDC_LANGIDX_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_SAVEALLRESOURCES_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOADALLRESOURCES_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_SELECTORBAR_BUTTON)->EnableWindow(TRUE);

	UpdateData(FALSE);
	return;

disable:
	GetDlgItem(IDC_FIND_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHANGE_EDIT)->EnableWindow(FALSE);
	//GetDlgItem(IDC_CHKWILD)->EnableWindow(FALSE);
	GetDlgItem(IDC_LOADCHANGES_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_JUMP_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_ADDR_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_STRINGVAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_HEXVAL)->EnableWindow(FALSE);
	GetDlgItem(IDC_LANGIDX_COMBO)->EnableWindow(FALSE);

	UpdateData(FALSE);
}

void CLayoutDialog::OnBnClickedSelectorBarButton()
{
	MessageBox(_T("The selector bar on iPod is a gradient color which consists of two colors, you have to choose those two colors to proceed.\nNote that the selector bar color on all the iPod will change!\nAlbeit, you can change manualy each selector bar color of each menu by changing the color1 and color2 values."), _T("Selector Bar Color Changer"));
	unsigned int color1,color2;
	if (ViewList->size()>=1)
	{
		color1=ViewList->at(0)->ViewElemList[0]->element->color1;
		color2=ViewList->at(0)->ViewElemList[0]->element->color2;
	}

	unsigned int ret_color1,ret_color2;
	if (GetUserColor(color1, &ret_color1, GetParent()->GetParent()->m_hWnd)==true)
	{
		//assign the new color
		color1=ret_color1;
	}
	if (GetUserColor(color2, &ret_color2, GetParent()->GetParent()->m_hWnd)==true)
	{
		//assign the new color
		color2=ret_color2;
	}

	DWORD j,k;
	unsigned int temp;
	for (j = 0; j < ViewList->size(); j++)
	{
		for (k=0; k < ViewList->at(j)->num_elems; k++)
		{
			temp=ViewList->at(j)->ViewElemList[k]->element->color1;
			if (temp == 0x5E920000)
			{
				ViewList->at(j)->ViewElemList[k]->element->color1=color1;
				//ViewList->at(j)->ViewElemList[k]->element->color2=color2;
			}
		}
	}

	HTREEITEM it;
	it=m_ResourceTree.GetSelectedItem();
	LRESULT res;
	NMTREEVIEW nm;
	nm.itemNew.hItem=it;
	OnTvnSelchangedResourceTree((NMHDR *)&nm, &res);
}

void CLayoutDialog::OnBnClickedJumpToButton()
{
	if (m_HexAddr<=m_pFirmware->GetFirmwareSize())
		m_HexWnd.SetOffset(m_HexAddr);
}

void CLayoutDialog::OnBnClickedFindButton()
{
	//FindString(m_ResourceTree.GetNextItem(m_ResourceTree.GetRootItem(), TVGN_NEXT));
	bool startsearch=true;
	FindResource(m_ResourceTree.GetRootItem(), false, NULL, &startsearch);
}

void CLayoutDialog::OnBnClickedLoadChangesButton()
{
	if (m_ResourceTree.GetSelectedItem()==NULL)
		return;
	CString filename;
	CString sID=m_ResourceTree.GetItemText(m_ResourceTree.GetSelectedItem());
	CString temp;
	DWORD j;
	int k;
	int sel_id;
	bool found=false;
	sel_id=_ttoi(sID.GetBuffer());
	LPBYTE lpBuf;
	UINT nCount;
	
	switch (m_ResourceType)
	{
	case 0: //view
		filename.Format(TEXT("view%d.rsrc"), sel_id);
		for (j = 0; j < ViewList->size(); j++)
		{
			if (ViewList->at(j)->id==sel_id)
			{
				nCount=4;
				for (k = 0; k < ViewList->at(j)->num_elems; k++)
				{
					nCount+=ViewList->at(j)->ViewElemList[k]->element->size+4;
				}
				lpBuf=(LPBYTE)ViewList->at(j)->ViewElemList[0]->element-4;
				found=true;
				break;
			}
		}
		break;
	case 1: //menu
		filename.Format(TEXT("menu%d.rsrc"), sel_id);
		for (j = 0; j < MenuList->size(); j++)
		{
			if (MenuList->at(j)->id==sel_id)
			{
				nCount=4;
				for (k = 0; k < MenuList->at(j)->num_elems; k++)
				{
					nCount+=MenuList->at(j)->MenuElemList[k]->element->size+4;
				}
				lpBuf=(LPBYTE)MenuList->at(j)->MenuElemList[0]->element-4;
				found=true;
				break;
			}
		}
		break;
	case 2: //type
		filename.Format(TEXT("type%d.rsrc"), sel_id);
		for (j = 0; j < TypeList->size(); j++)
		{
			if (TypeList->at(j)->id==sel_id)
			{
				nCount=sizeof(*TypeList->at(j)->element);
				lpBuf=(LPBYTE)TypeList->at(j)->element;
				found=true;
				break;
			}
		}
		break;
	case 3: //tcmd
		filename.Format(TEXT("tcmd%d.rsrc"), sel_id);
		for (j = 0; j < TCMDList->at(0)->element.elements.size(); j++)
		{
			if (TCMDList->at(0)->element.elements.at(j)->id==sel_id)
			{
				nCount=sizeof(*TCMDList->at(0)->element.elements.at(j));
				lpBuf=(LPBYTE)TCMDList->at(0)->element.elements.at(j);
				found=true;
				break;
			}
		}
		break;
	}
		
	if (found==false)
	{
		MessageBox(_T("This is not a valid resource for saving!"));
		return;
	}
	CFile file;
	
	CFileDialog dlgb(TRUE, TEXT("rsrc"), 0, OFN_HIDEREADONLY, TEXT("iPod Resource Files (*.rsrc)|*.rsrc||"), this);
	MO_LOAD_RESOURCES_PATH(dlgb)

	if (dlgb.DoModal() != IDOK)
		return;
	MO_SAVE_RESOURCES_PATH(dlgb)

	if (!file.Open(dlgb.GetPathName(), CFile::modeRead))
	{
		MessageBox(TEXT("Unable to load file!"));
		return;
	}
	
	DWORD filelen=(DWORD)file.GetLength();
	if (filelen!=nCount)
	{
		MessageBox(_T("The resource file you chose doesn't fit this resource!"));
		return;
	}
	else
	{
		file.Read(lpBuf, nCount);
		file.Close();
		MessageBox(TEXT("Loaded resource file successfully!"));
		
		LRESULT res;
		NMTREEVIEW nm;
		nm.itemNew.hItem=m_ResourceTree.GetSelectedItem();
		OnTvnSelchangedResourceTree((NMHDR *)&nm, &res);
		m_HexWnd.Invalidate();
	}	
}

void CLayoutDialog::OnBnClickedSaveResourceButton()
{
	/*LPITEMIDLIST il;
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

	SHGetPathFromIDList(il, folderPath);*/

	if (m_ResourceTree.GetSelectedItem()==NULL)
		return;

	CString filename;
	CString sID=m_ResourceTree.GetItemText(m_ResourceTree.GetSelectedItem());
	filename.Format(TEXT("iPod Resource Files (*.rsrc)|%s.rsrc||"), sID);
	int sel_id;
	sel_id=_ttoi(sID.GetBuffer());
	sID.AppendFormat(_T(".rsrc"));
	switch (m_ResourceType)
	{
	case 0:
		sID.Format(_T("view%s"), sID);
		break;
	case 1:
		sID.Format(_T("menu%s"), sID);
		break;
	case 2:
		sID.Format(_T("type%s"), sID);
		break;
	case 3:
		sID.Format(_T("tcmd%s"), sID);
		break;
	}
	CFileDialog dlg(FALSE, TEXT("rsrc"), sID, OFN_HIDEREADONLY, filename, this);
	MO_LOAD_RESOURCES_PATH(dlg)

	if (dlg.DoModal()!=IDOK)
		return;
	MO_SAVE_RESOURCES_PATH(dlg)
	filename.SetString(dlg.GetPathName());
	
	CString temp;
	DWORD j,k;
	bool found=false;
	LPBYTE lpBuf;
	UINT nCount;

	//filename.Format(TEXT("menu%d.rsrc"), folderPath, sel_id);
	
	switch (m_ResourceType)
	{
	case 0: //view
		for (j = 0; j < ViewList->size(); j++)
		{
			if (ViewList->at(j)->id==sel_id)
			{
				nCount=4;
				for (k = 0; k < ViewList->at(j)->num_elems; k++)
				{
					nCount+=ViewList->at(j)->ViewElemList[k]->element->size+4;
				}
				lpBuf=(LPBYTE)ViewList->at(j)->ViewElemList[0]->element-4;
				found=true;
				break;
			}
		}
		break;
	case 1: //menu
		for (j = 0; j < MenuList->size(); j++)
		{
			if (MenuList->at(j)->id==sel_id)
			{
				nCount=4;
				for (k = 0; k < MenuList->at(j)->num_elems; k++)
				{
					nCount+=MenuList->at(j)->MenuElemList[k]->element->size+4;
				}
				lpBuf=(LPBYTE)MenuList->at(j)->MenuElemList[0]->element-4;
				found=true;
				break;
			}
		}
		break;
	case 2: //type
		for (j = 0; j < TypeList->size(); j++)
		{
			if (TypeList->at(j)->id==sel_id)
			{
				nCount=sizeof(*TypeList->at(j)->element);
				lpBuf=(LPBYTE)TypeList->at(j)->element;
				found=true;
				break;
			}
		}
		break;
	case 3:
		for (j = 0; j < TCMDList->at(0)->element.elements.size(); j++)
		{
			if (TCMDList->at(0)->element.elements.at(j)->id==sel_id)
			{
				nCount=sizeof(*TCMDList->at(0)->element.elements.at(j));
				lpBuf=(LPBYTE)TCMDList->at(0)->element.elements.at(j);
				found=true;
				break;
			}
		}
		break;
	}

	if (found)
	{
		SaveRAWResource(filename, lpBuf, nCount);
		MessageBox(_T("Saved resource successfully!"));
	}
	else
	{
		MessageBox(_T("Make sure you chose a resource!"));
	}
}

void CLayoutDialog::OnEnChangeFindEdit()
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

void CLayoutDialog::OnEnChangeChangeEdit()
{
	UpdateData(TRUE);

	BOOL bEnable = !m_ChangeString.IsEmpty() && m_pFirmware != NULL;
	GetDlgItem(IDC_CHANGETEXT_BUTTON)->EnableWindow(bEnable);
}

void CLayoutDialog::OnEnChangeAddrEdit()
{
	UpdateData(TRUE);
}

void CLayoutDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialog::OnOK();
}

void CLayoutDialog::OnBnClickedFindnextButton()
{
	//FindString(m_ResourceTree.GetSelectedItem());
	bool startsearch=false;
	FindResource(m_ResourceTree.GetSelectedItem(), true, m_ResourceTree.GetSelectedItem(), &startsearch);
}

void CLayoutDialog::OnBnClickedChangeTextButton()
{
	int j,lenm;
	LPBYTE str;
	if (IsDlgButtonChecked(IDC_RADIO_STRINGVAL)==1)
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
		delete lpa;
	}
	else if (IsDlgButtonChecked(IDC_RADIO_HEXVAL)==1)
	{
		//Hex value string
		lenm=m_ChangeString.GetLength();
		str=new BYTE[lenm/2];
		//for (int k=0,j=lenm-2;j >=0;j-=2,k++)
		for (int k=0,j=0;j<lenm;j+=2,k++)
		{
			swscanf(m_ChangeString.Mid(j, 2), TEXT("%X"), &str[k]);
		}
		lenm=lenm/2;
	}
	else if (IsDlgButtonChecked(IDC_RADIO_DECVAL)==1)
	{
		//Decimal value string
		//Max is a 4 byte number
		lenm=2;
		str=new BYTE[8];
		swscanf(m_ChangeString, TEXT("%d"), str);
	}

	int p;
	POSITION temp=m_ResourceList.GetFirstSelectedItemPosition();
	p = m_ResourceList.GetNextSelectedItem(temp);

	HTREEITEM it;
	
	it=m_ResourceTree.GetSelectedItem();
	int sel_id=_ttoi(m_ResourceTree.GetItemText(it));

	DWORD bsize;
	switch (m_ResourceType)
	{
	case 0: //view
		bsize=ViewList->size();
		for (j=0;j<bsize;j++)
		{
			if (ViewList->at(j)->id==sel_id)
				break;
		}
		break;
	case 1:
		bsize=MenuList->size();
		for (j=0;j<bsize;j++)
		{
			if (MenuList->at(j)->id==sel_id)
				break;
		}
		break;
	case 2:
		bsize=TypeList->size();
		for (j=0;j<bsize;j++)
		{
			if (TypeList->at(j)->id==sel_id)
				break;
		}
		break;
	case 3:
		break;
	}

	if (j!=bsize)
	{

		if (m_ValSize==0)
		{
			MessageBox(_T("Not allowed to changed that value. Change it manually if you want."));
			return;
		}
		else if (lenm>m_ValSize)
		{
			MessageBox(_T("The value you typed is too long!"));
			return;
		}
	
		m_pFirmware->SetData(m_ValOffset, str, m_ValSize);
		if (IsDlgButtonChecked(IDC_RADIO_HEXVAL)==0)
			delete[] str;
		m_HexWnd.SetOffset(m_ValOffset);
	}
	
	m_HexWnd.Invalidate();
	
	LRESULT res;
	NMTREEVIEW nm;
	nm.itemNew.hItem=it;
	OnTvnSelchangedResourceTree((NMHDR *)&nm, &res);
	
	UpdateData(FALSE);
	//
	MessageBox(TEXT("Value changed successfully."), TEXT("Value Changer"), MB_OK);
}

bool CLayoutDialog::FindResource(HTREEITEM hParentItem, bool next, HTREEITEM hMarkItem, bool *startsearch)
{
	HTREEITEM hCurrItem;
	hCurrItem=hParentItem;
	if (hCurrItem==NULL)
		return false;

	UpdateData(TRUE);

	if (m_FindString.IsEmpty())
		return false;

	m_FindString.MakeLower();

	// search

	CWaitCursor wait;

	if (next)
	{
		HTREEITEM hCurrItem2;
		HTREEITEM hRootItem;
		hRootItem=m_ResourceTree.GetRootItem();
		hCurrItem2=m_ResourceTree.GetNextItem(hCurrItem, TVGN_PARENT);
		while (hCurrItem2!=NULL)
		{
			hCurrItem=hCurrItem2;
			hCurrItem2=m_ResourceTree.GetNextItem(hCurrItem, TVGN_PARENT);
		}
	}
	
	bool found=false;
	while (hCurrItem!=NULL && !found)
	{
		if (*startsearch==true)
		{
			if (m_ResourceTree.GetItemText(hCurrItem).Find(m_FindString)!=-1)
			{
				m_ResourceTree.SelectItem(hCurrItem);
				RECT rct;
				m_ResourceTree.GetItemRect(hCurrItem, &rct, FALSE);
				m_ResourceTree.SetCaretPos(CPoint(rct.left, rct.top));
				found=true;
				break;
			}
		}
		if (hCurrItem==hMarkItem)
			*startsearch=true;
		found=FindResource(m_ResourceTree.GetNextItem(hCurrItem, TVGN_CHILD), false, hMarkItem, startsearch);
		hCurrItem=m_ResourceTree.GetNextItem(hCurrItem, TVGN_NEXT);
	}
	return found;
}

void CLayoutDialog::FindString(HTREEITEM hStartItem)
{
	UpdateData(TRUE);

	if (m_FindString.IsEmpty())
		return;

	m_FindString.MakeLower();

	// search

	CWaitCursor wait;

	HTREEITEM hCurrItem;
	hCurrItem=m_ResourceTree.GetNextItem(m_ResourceTree.GetRootItem(), TVGN_NEXT);
	DWORD k;
	k=0;
	int sel_id;
	sel_id=_ttoi(m_FindString);
	bool found;
	bool start=false;
	while (hCurrItem!=NULL)
	{
		if (start)
		{
			found=false;
			switch (m_ResourceType)
			{
			case 0:
				if (ViewList->at(k)->id==sel_id)
				{
					m_ResourceTree.SelectItem(m_ResourceTree.GetPrevSiblingItem(hCurrItem));
					found=true;
				}
				break;
			case 1:
				if (MenuList->at(k)->id==sel_id)
				{
					m_ResourceTree.SelectItem(m_ResourceTree.GetPrevSiblingItem(hCurrItem));
					found=true;
				}
				break;
			case 2:
				if (TypeList->at(k)->id==sel_id)
				{
					m_ResourceTree.SelectItem(m_ResourceTree.GetPrevSiblingItem(hCurrItem));
					found=true;
				}
				break;
			}
			if (found)
				break;
		}
		if (hCurrItem==hStartItem)
			start=true;
		k++;
		hCurrItem=m_ResourceTree.GetNextItem(hCurrItem, TVGN_NEXT);
	}
}

void CLayoutDialog::Create_ViewTree(ViewRecord *view_rec, HTREEITEM hItem, vector<short> *id_list)
{
	DWORD k;
	CString sID;
	HTREEITEM hParentItem;
	ViewRecord *new_rec;

	if (view_rec==NULL)
		return;

	(*id_list).push_back(view_rec->id);

	for (k = 0; k < view_rec->num_elems; k++)
	{
		if (IsNotOnList(id_list, view_rec->ViewElemList[k]->element->id))
		{
			sID.Format(_T("%d"), view_rec->ViewElemList[k]->element->id);
			hParentItem=m_ResourceTree.InsertItem(sID, hItem, TVI_LAST);
			new_rec=view_rec->ViewElemList[k]->next;
			if (new_rec!=NULL)
			{
				vector<short> new_id_list;
				new_id_list.push_back(0);
				//allocate v2 to size of v1 or use back_inserter
				copy((*id_list).begin(), (*id_list).end(), back_inserter(new_id_list));
				Create_ViewTree(new_rec, hParentItem, &new_id_list);
			}
		}
	}
}

void CLayoutDialog::OnCbnSelchangeResourceTypeidxCombo()
{
	m_ResourceType=m_ResourceTypeIdx.GetCurSel();

	HTREEITEM hRootItem;
	int firstid=-1;
	DWORD j;
	int k;
	CString sID;
	
	switch(m_ResourceType)
	{
		case 0: //view
			while (m_ResourceList.DeleteColumn(0)==TRUE);
			m_ResourceList.InsertColumn(0, TEXT("Block size"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(1, TEXT("z0"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(2, TEXT("Resource type"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(3, TEXT("ID"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(4, TEXT("unk1"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(5, TEXT("z1"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(6, TEXT("z2"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(7, TEXT("z3"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(8, TEXT("z4"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(9, TEXT("left"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(10, TEXT("top"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(11, TEXT("right"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(12, TEXT("bottom"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(13, TEXT("Color1"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(14, TEXT("Color2"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(15, TEXT("z7"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(16, TEXT("font_id"), LVCFMT_LEFT, 40);

			m_ResourceTree.DeleteAllItems();

			for (j = 0; j < ViewList->size(); j++)
			{
				sID.Format(_T("%d"), ViewList->at(j)->id);
				firstid=ViewList->at(j)->id;
				hRootItem=m_ResourceTree.InsertItem(sID, TVI_ROOT, TVI_LAST);
				vector<short> id_list;
				Create_ViewTree(ViewList->at(j), hRootItem, &id_list);
			}
	
			if (firstid!=-1)
			{

			}
			break;
		case 1: //menu
			while (m_ResourceList.DeleteColumn(0)==TRUE);
			m_ResourceList.InsertColumn(0, TEXT("Block size"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(1, TEXT("z1"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(2, TEXT("z2"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(3, TEXT("Command"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(4, TEXT("submenuID"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(5, TEXT("z3"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(6, TEXT("textID"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(7, TEXT("z4"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(8, TEXT("z5"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(9, TEXT("z6"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(10, TEXT("textID2"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(11, TEXT("z7"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(12, TEXT("typeID (Font used)"), LVCFMT_LEFT, 40);

			m_ResourceTree.DeleteAllItems();

			for (j = 0; j < MenuList->size(); j++)
			{
				sID.Format(_T("%d"), MenuList->at(j)->id);
				hRootItem=m_ResourceTree.InsertItem(sID, TVI_ROOT, TVI_LAST);
				
				for (k=0;k<MenuList->at(j)->num_elems;k++)
				{
					if (MenuList->at(j)->MenuElemList[k]->element->submenuID != 0)
					{
						sID.Format(_T("%d"), MenuList->at(j)->MenuElemList[k]->element->submenuID);
						m_ResourceTree.InsertItem(sID, hRootItem, TVI_LAST);
					}
				}
			}

			break;
		case 2: //type
			while (m_ResourceList.DeleteColumn(0)==TRUE);
			m_ResourceList.InsertColumn(0, TEXT("fontID (text string)"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(1, TEXT("z1"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(2, TEXT("Font Size"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(3, TEXT("RGB Color"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(4, TEXT("Text Style"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(5, TEXT("Text Align"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(6, TEXT("z2"), LVCFMT_LEFT, 40);

			m_ResourceTree.DeleteAllItems();

			for (j = 0; j < TypeList->size(); j++)
			{
				sID.Format(_T("%d"), TypeList->at(j)->id);
				hRootItem=m_ResourceTree.InsertItem(sID, TVI_ROOT, TVI_LAST);
			}
			break;
		case 3: //tcmd
			while (m_ResourceList.DeleteColumn(0)==TRUE);
			m_ResourceList.InsertColumn(0, TEXT("commandID"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(1, TEXT("z0"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(2, TEXT("z1"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(3, TEXT("num_subCommands"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(4, TEXT("subCommand"), LVCFMT_LEFT, 40);
			m_ResourceList.InsertColumn(5, TEXT("z2"), LVCFMT_LEFT, 40);

			m_ResourceTree.DeleteAllItems();

			for (j = 0; j < TCMDList->at(0)->element.elements.size(); j++)
			{
				sID.Format(_T("%d"), TCMDList->at(0)->element.elements.at(j)->id);
				hRootItem=m_ResourceTree.InsertItem(sID, TVI_ROOT, TVI_LAST);
			}
			break;
	}
	
	GetDlgItem(IDC_SAVECHANGES_BUTTON)->EnableWindow(TRUE);
}

void CLayoutDialog::OnLvnItemDblClickedResourceList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMLV = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (m_ResourceList.GetFirstSelectedItemPosition()<=0 && *pResult!=1)
		return;
	if (pNMLV->iItem==-1)
		return;

	CString sID=m_ResourceTree.GetItemText(m_ResourceTree.GetSelectedItem());
	int iItem=pNMLV->iItem;
	DWORD j,k;
	int sel_id;
	sel_id=_ttoi(sID.GetBuffer());
	bool found=false;
	DWORD iOffset;
	LPBYTE lpBuf=m_pFirmware->GetFirmwareBuffer();

	switch(m_ResourceType)
	{
	case 0: //view
		
		for (j = 0; j < ViewList->size(); j++)
		{
			if (ViewList->at(j)->id==sel_id)
			{
				found=true;
				break;
			}
		}
		if (found==false) //it's not a view resource, try find the link
		{
			//MessageBox(_T("TEST"));
			return;
		}

		switch (pNMLV->iSubItem)
		{
		case 0:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->size-lpBuf;
			break;
		case 1:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->z0-lpBuf;
			break;
		case 2:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->type-lpBuf;
			if (!strncmp(ViewList->at(j)->ViewElemList[iItem]->element->type, " rtS", 4))
			{
				//let's find that string!
				::SendMessage(::GetParent(::GetParent(m_hWnd)), WM_APP, (WPARAM)11, (LPARAM)ViewList->at(j)->ViewElemList[iItem]->element->id);
			}
			else if (!strncmp(ViewList->at(j)->ViewElemList[iItem]->element->type, "paMB", 4))
			{
				//let's find that bitmap!
				::SendMessage(::GetParent(::GetParent(m_hWnd)), WM_APP, (WPARAM)12, (LPARAM)m_pFirmware->GetPictureIndexFromID((DWORD)ViewList->at(j)->ViewElemList[iItem]->element->id));
			}
			else if (!strncmp(ViewList->at(j)->ViewElemList[iItem]->element->type, "eypT", 4))
			{
				//let's find that bitmap!
				::SendMessage(::GetParent(::GetParent(m_hWnd)), WM_APP, (WPARAM)13, (LPARAM)ViewList->at(j)->ViewElemList[iItem]->element->id);
			}
			break;
		case 3:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->id-lpBuf;
			break;
		case 4:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->unk1-lpBuf;
			break;
		case 5:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->z1-lpBuf;
			break;
		case 6:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->z2-lpBuf;
			break;
		case 7:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->z3-lpBuf;
			break;
		case 8:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->z4-lpBuf;
			break;
		case 9:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->left.val-lpBuf;
			break;
		case 10:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->top.val-lpBuf;
			break;
		case 11:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->right.val-lpBuf;
			break;
		case 12:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->bottom.val-lpBuf;
			break;
		case 13:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->color1-lpBuf;
			//popup color selector
			if (MessageBox(_T("Do you want to change this color by picking a color from the palette?"), _T("Color changer"), MB_YESNO)!=IDYES)
				break;

			unsigned int ret_color1;
			if (GetUserColor(ViewList->at(j)->ViewElemList[iItem]->element->color1, &ret_color1, GetParent()->GetParent()->GetParent()->m_hWnd)==true)
			{
				//assign the new color
				ViewList->at(j)->ViewElemList[iItem]->element->color1=ret_color1;

				HTREEITEM it;
				it=m_ResourceTree.GetSelectedItem();
				LRESULT res;
				NMTREEVIEW nm;
				nm.itemNew.hItem=it;
				OnTvnSelchangedResourceTree((NMHDR *)&nm, &res);
			}
			break;
		case 14:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->color2-lpBuf;

			//popup color selector
			if (MessageBox(_T("Do you want to change this color by picking a color from the palette?"), _T("Color changer"), MB_YESNO)!=IDYES)
				break;

			unsigned int ret_color2;
			if (GetUserColor(ViewList->at(j)->ViewElemList[iItem]->element->color2, &ret_color2, GetParent()->GetParent()->GetParent()->m_hWnd)==true)
			{
				//assign the new color
				ViewList->at(j)->ViewElemList[iItem]->element->color2=ret_color2;

				HTREEITEM it;
				it=m_ResourceTree.GetSelectedItem();
				LRESULT res;
				NMTREEVIEW nm;
				nm.itemNew.hItem=it;
				OnTvnSelchangedResourceTree((NMHDR *)&nm, &res);
			}
			break;
		case 15:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->z7-lpBuf;
			break;
		case 16:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->font_id-lpBuf;

			if (ViewList->at(j)->ViewElemList[iItem]->element->font_id==0)
				break;

			//link to font editor
			int mysel;
			mysel=ViewList->at(j)->ViewElemList[iItem]->element->font_id;
			found=false;
			DWORD mi;
			for (mi = 0; mi < TypeList->size(); mi++)
			{
				if (TypeList->at(mi)->id==mysel)
				{
					found=true;
					break;
				}
			}
			if (found==false) //didn't find that font id
				break;
			::SendMessage(::GetParent(::GetParent(m_hWnd)), WM_APP, (WPARAM)13, (LPARAM)TypeList->at(mi)->element);
			break;
		}
		//m_HexWnd.SetOffset(iOffset);
		break;
	case 1:
		for (j = 0; j < MenuList->size(); j++)
		{
			if (MenuList->at(j)->id==sel_id)
			{
				found=true;
				break;
			}
		}
		if (found==false) //it's not a view resource, try find the link
		{
			//MessageBox(_T("TEST"));
			return;
		}

		switch (pNMLV->iSubItem)
		{
		case 0:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->size-lpBuf;
			break;
		case 1:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->z1-lpBuf;
			break;
		case 2:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->z2-lpBuf;
			break;
		case 3:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->cmd-lpBuf;
			//find command in command table
			break;
		case 4:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->submenuID-lpBuf;
			break;
		case 5:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->z3-lpBuf;
			break;
		case 6:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->textID-lpBuf;
			//let's find that string!
			::SendMessage(::GetParent(::GetParent(m_hWnd)), WM_APP, (WPARAM)11, (LPARAM)MenuList->at(j)->MenuElemList[iItem]->element->textID);
			break;
		case 7:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->z4-lpBuf;
			break;
		case 8:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->z5-lpBuf;
			break;
		case 9:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->z6-lpBuf;
			break;
		case 10:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->textID2-lpBuf;
			//let's find that string!
			::SendMessage(::GetParent(::GetParent(m_hWnd)), WM_APP, (WPARAM)11, (LPARAM)MenuList->at(j)->MenuElemList[iItem]->element->textID2);
			break;
		case 11:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->z7-lpBuf;
			break;
		case 12:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->typeID-lpBuf;
			//let's find that font type!
			m_ResourceTypeIdx.SetCurSel(2);
			OnCbnSelchangeResourceTypeidxCombo();
			HTREEITEM hCurrItem;
			hCurrItem=m_ResourceTree.GetNextItem(m_ResourceTree.GetRootItem(), TVGN_NEXT);
			k=0;
			while (hCurrItem!=NULL)
			{
				if (TypeList->at(k)->id==MenuList->at(j)->MenuElemList[iItem]->element->typeID)
				{
					m_ResourceTree.SelectItem(m_ResourceTree.GetPrevSiblingItem(hCurrItem));
					break;
				}
				k++;
				hCurrItem=m_ResourceTree.GetNextItem(hCurrItem, TVGN_NEXT);
			}
			
			//::SendMessage(::GetParent(::GetParent(m_hWnd)), WM_APP, (WPARAM)11, (LPARAM)MenuList->at(j)->MenuElemList[iItem]->element->textID);
			break;
		}
		//m_HexWnd.SetOffset(iOffset);
		break;
	case 2:
		for (j = 0; j < TypeList->size(); j++)
		{
			if (TypeList->at(j)->id==sel_id)
			{
				found=true;
				break;
			}
		}
		if (found==false) //it's not a view resource, try find the link
		{
			//MessageBox(_T("TEST"));
			return;
		}

		switch (pNMLV->iSubItem)
		{
		case 0:
			iOffset=(LPBYTE)&TypeList->at(j)->element->fontID-lpBuf;
			//link to string dialog
			::SendMessage(::GetParent(::GetParent(m_hWnd)), WM_APP, (WPARAM)11, (LPARAM)TypeList->at(j)->element->fontID);
			break;
		case 1:
			iOffset=(LPBYTE)&TypeList->at(j)->element->z1-lpBuf;
			
			break;
		case 2:
			iOffset=(LPBYTE)&TypeList->at(j)->element->size-lpBuf;
			//link to font editor
			::SendMessage(::GetParent(::GetParent(m_hWnd)), WM_APP, (WPARAM)13, (LPARAM)TypeList->at(j)->element);
			break;
		case 3:
			iOffset=(LPBYTE)&TypeList->at(j)->element->color-lpBuf;
			//popup color selector
			if (MessageBox(_T("Do you want to change this color by picking a color from the palette?"), _T("Color changer"), MB_YESNO)!=IDYES)
				break;

			unsigned int ret_color;
			if (GetUserColor(TypeList->at(j)->element->color, &ret_color, GetParent()->GetParent()->GetParent()->m_hWnd)==true)
			{
				//assign the new color
				TypeList->at(j)->element->color=ret_color;

				HTREEITEM it;
				it=m_ResourceTree.GetSelectedItem();
				LRESULT res;
				NMTREEVIEW nm;
				nm.itemNew.hItem=it;
				OnTvnSelchangedResourceTree((NMHDR *)&nm, &res);
			}
			break;
		case 4:
			iOffset=(LPBYTE)&TypeList->at(j)->element->style-lpBuf;
			
			break;
		case 5:
			iOffset=(LPBYTE)&TypeList->at(j)->element->alignment-lpBuf;
			
			break;
		case 6:
			iOffset=(LPBYTE)&TypeList->at(j)->element->z2-lpBuf;
			
			break;
		}
		m_HexWnd.SetOffset(iOffset);
		break;
	}

	UpdateData(FALSE);
}

void CLayoutDialog::OnLvnItemClickedResourceList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMLV = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (m_ResourceList.GetFirstSelectedItemPosition()<=0)
		return;
	if (pNMLV->iItem==-1)
		return;

	CString sID=m_ResourceTree.GetItemText(m_ResourceTree.GetSelectedItem());
	int iItem=pNMLV->iItem;
	DWORD j;
	int sel_id;
	sel_id=_ttoi(sID.GetBuffer());
	bool found=false;
	DWORD iOffset;
	LPBYTE lpBuf=m_pFirmware->GetFirmwareBuffer();

	switch(m_ResourceType)
	{
	case 0: //view
		for (j = 0; j < ViewList->size(); j++)
		{
			if (ViewList->at(j)->id==sel_id)
			{
				found=true;
				break;
			}
		}
		if (found==false) //it's not a view resource, try find the link
		{
			//MessageBox(_T("TEST"));
			return;
		}

		switch (pNMLV->iSubItem)
		{
		case 0:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->size-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 1:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->z0-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 2:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->type-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 3:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->id-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 4:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->unk1-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 5:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->z1-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 6:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->z2-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 7:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->z3-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 8:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->z4-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 9:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->left.val-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 10:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->top.val-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 11:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->right.val-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 12:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->bottom.val-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 13:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->color1-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 14:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->color2-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 15:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->z7-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 16:
			iOffset=(LPBYTE)&ViewList->at(j)->ViewElemList[iItem]->element->font_id-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		}
		m_HexWnd.SetOffset(iOffset);
		break;
	case 1:
		for (j = 0; j < MenuList->size(); j++)
		{
			if (MenuList->at(j)->id==sel_id)
			{
				found=true;
				break;
			}
		}
		if (found==false) //it's not a view resource, try find the link
		{
			//MessageBox(_T("TEST"));
			return;
		}

		switch (pNMLV->iSubItem)
		{
		case 0:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->size-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 1:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->z1-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 2:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->z2-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 3:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->cmd-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 4:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->submenuID-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 5:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->z3-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 6:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->textID-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 7:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->z4-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 8:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->z5-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 9:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->z6-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 10:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->textID2-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 11:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->z7-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 12:
			iOffset=(LPBYTE)&MenuList->at(j)->MenuElemList[iItem]->element->typeID-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		}
		m_HexWnd.SetOffset(iOffset);
		break;
	case 2:
		for (j = 0; j < TypeList->size(); j++)
		{
			if (TypeList->at(j)->id==sel_id)
			{
				found=true;
				break;
			}
		}
		if (found==false) //it's not a view resource, try find the link
		{
			//MessageBox(_T("TEST"));
			return;
		}

		switch (pNMLV->iSubItem)
		{
		case 0:
			iOffset=(LPBYTE)&TypeList->at(j)->element->fontID-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 1:
			iOffset=(LPBYTE)&TypeList->at(j)->element->z1-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 2:
			iOffset=(LPBYTE)&TypeList->at(j)->element->size-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 3:
			iOffset=(LPBYTE)&TypeList->at(j)->element->color-lpBuf;
			m_ValSize=4;
			m_ValOffset=iOffset;
			break;
		case 4:
			iOffset=(LPBYTE)&TypeList->at(j)->element->style-lpBuf;
			m_ValSize=1;
			m_ValOffset=iOffset;
			break;
		case 5:
			iOffset=(LPBYTE)&TypeList->at(j)->element->alignment-lpBuf;
			m_ValSize=1;
			m_ValOffset=iOffset;
			break;
		case 6:
			iOffset=(LPBYTE)&TypeList->at(j)->element->z2-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		}
		m_HexWnd.SetOffset(iOffset);
		break;
	case 3: //tcmd
		for (j = 0; j < TCMDList->at(0)->element.elements.size(); j++)
		{
			if (TCMDList->at(0)->element.elements.at(j)->id==sel_id)
			{
				found=true;
				break;
			}
		}
		if (found==false) //it's not a view resource, try find the link
		{
			//MessageBox(_T("TEST"));
			return;
		}

		switch (pNMLV->iSubItem)
		{
		case 0:
			iOffset=(LPBYTE)&TCMDList->at(0)->element.elements.at(j)->id-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 1:
			iOffset=(LPBYTE)&TCMDList->at(0)->element.elements.at(j)->z0-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 2:
			iOffset=(LPBYTE)&TCMDList->at(0)->element.elements.at(j)->z1-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 3:
			iOffset=(LPBYTE)&TCMDList->at(0)->element.elements.at(j)->num_subs-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 4:
			iOffset=(LPBYTE)&TCMDList->at(0)->element.elements.at(j)->subcmd_len-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		case 5:
			iOffset=(LPBYTE)&TCMDList->at(0)->element.elements.at(j)->z2-lpBuf;
			m_ValSize=2;
			m_ValOffset=iOffset;
			break;
		}
		m_HexWnd.SetOffset(iOffset);
		break;
	}

	UpdateData(FALSE);
}

void CLayoutDialog::OnLvnItemchangedResourceList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	//LPNMITEMACTIVATE pNMLV = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString sID=m_ResourceTree.GetItemText(m_ResourceTree.GetSelectedItem());
	if (pNMLV->uOldState!=0)
		return;


	
	UpdateData(FALSE);

	*pResult = 0;
}

void CLayoutDialog::OnTvnSelchangedResourceTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CString sID=m_ResourceTree.GetItemText(pNMTreeView->itemNew.hItem);
	CString temp;
	DWORD j;
	int k;
	int sel_id;
	bool found=false;
	sel_id=_ttoi(sID.GetBuffer());
	m_ResourceList.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_LOADINGLIST)->ShowWindow(SW_SHOW);
	m_ResourceList.DeleteAllItems();
	
	switch(m_ResourceType)
	{
	case 0: //view
		char mytype[5];
		for (j = 0; j < ViewList->size(); j++)
		{
			if (ViewList->at(j)->id==sel_id)
			{
				for (k = 0; k < ViewList->at(j)->num_elems; k++)
				{
					temp.Format(TEXT("%d"), ViewList->at(j)->ViewElemList[k]->element->size);
					m_ResourceList.InsertItem(k, temp);
					temp.Format(TEXT("%d"), ViewList->at(j)->ViewElemList[k]->element->z0);
					m_ResourceList.SetItemText(k, 1, temp);
					strncpy(mytype, ViewList->at(j)->ViewElemList[k]->element->type, 4);
					mytype[4]=0;
					temp.Format(TEXT("%S"), mytype);
					m_ResourceList.SetItemText(k, 2, temp);
					m_ResourceList.SetItemToolTipText(k, 2, _T("Double click to link to resource!"));
					temp.Format(TEXT("%d"), ViewList->at(j)->ViewElemList[k]->element->id);
					m_ResourceList.SetItemText(k, 3, temp);
					temp.Format(TEXT("%d"), ViewList->at(j)->ViewElemList[k]->element->unk1);
					m_ResourceList.SetItemText(k, 4, temp);
					temp.Format(TEXT("%d"), ViewList->at(j)->ViewElemList[k]->element->z1);
					m_ResourceList.SetItemText(k, 5, temp);
					temp.Format(TEXT("%d"), ViewList->at(j)->ViewElemList[k]->element->z2);
					m_ResourceList.SetItemText(k, 6, temp);
					temp.Format(TEXT("%d"), ViewList->at(j)->ViewElemList[k]->element->z3);
					m_ResourceList.SetItemText(k, 7, temp);
					temp.Format(TEXT("%d"), ViewList->at(j)->ViewElemList[k]->element->z4);
					m_ResourceList.SetItemText(k, 8, temp);
					temp.Format(TEXT("%d"), ViewList->at(j)->ViewElemList[k]->element->left.val); //maybe string
					m_ResourceList.SetItemText(k, 9, temp);
					temp.Format(TEXT("%d"), ViewList->at(j)->ViewElemList[k]->element->top.val);
					m_ResourceList.SetItemText(k, 10, temp);
					temp.Format(TEXT("%d"), ViewList->at(j)->ViewElemList[k]->element->right.val);
					m_ResourceList.SetItemText(k, 11, temp);
					temp.Format(TEXT("%d"), ViewList->at(j)->ViewElemList[k]->element->bottom.val); //bitmap
					m_ResourceList.SetItemText(k, 12, temp);
					//temp.Format(TEXT("0x%-4X"), ViewList->at(j)->ViewElemList[k]->element->color1);
					m_ResourceList.SetItemText(k, 13, temp);
					m_ResourceList.SetItemToolTipText(k, 13, _T("Double click to change selector bar color 1!"));
					temp.Format(TEXT("0x%-4X"), ViewList->at(j)->ViewElemList[k]->element->color2);
					m_ResourceList.SetItemText(k, 14, temp);
					m_ResourceList.SetItemToolTipText(k, 14, _T("Double click to change selector bar color 2!"));
					temp.Format(TEXT("0x%-2X"), ViewList->at(j)->ViewElemList[k]->element->z7);
					m_ResourceList.SetItemText(k, 15, temp);
					temp.Format(TEXT("%d"), ViewList->at(j)->ViewElemList[k]->element->font_id);
					m_ResourceList.SetItemText(k, 16, temp);
					m_ResourceList.SetItemToolTipText(k, 16, _T("Double click to link to font associated with this string!"));
				}
				found=true;
				break;
			}
		}
		if (found==false) //it's not a view resource, try find the link
		{
			sID=m_ResourceTree.GetItemText(m_ResourceTree.GetParentItem(pNMTreeView->itemNew.hItem));
			int sel_id_parent=_ttoi(sID.GetBuffer());
			for (j = 0; j < ViewList->size(); j++)
			{
				if (ViewList->at(j)->id==sel_id_parent)
				{
					for (k = 0; k < ViewList->at(j)->num_elems; k++)
					{
						if (ViewList->at(j)->ViewElemList[k]->element->id==sel_id)
						{
							if (!strncmp(ViewList->at(j)->ViewElemList[k]->element->type, " rtS", 4))
							{
								//let's find that string!
								::SendMessage(::GetParent(::GetParent(m_hWnd)), WM_APP, (WPARAM)11, (LPARAM)ViewList->at(j)->ViewElemList[k]->element->id);
							}
							else if (!strncmp(ViewList->at(j)->ViewElemList[k]->element->type, "paMB", 4))
							{
								//let's find that bitmap!
								::SendMessage(::GetParent(::GetParent(m_hWnd)), WM_APP, (WPARAM)12, (LPARAM)m_pFirmware->GetPictureIndexFromID((DWORD)ViewList->at(j)->ViewElemList[k]->element->id));
							}
							else if (!strncmp(ViewList->at(j)->ViewElemList[k]->element->type, "eypT", 4))
							{
								//let's find that bitmap!
								::SendMessage(::GetParent(::GetParent(m_hWnd)), WM_APP, (WPARAM)13, (LPARAM)ViewList->at(j)->ViewElemList[k]->element->id);
							}
						}
					}
				}
			}
		}
		
		break;
	case 1: //menu
		for (j = 0; j < MenuList->size(); j++)
		{
			if (MenuList->at(j)->id==sel_id)
			{
				for (k = 0; k < MenuList->at(j)->num_elems; k++)
				{
					temp.Format(TEXT("%d"), MenuList->at(j)->MenuElemList[k]->element->size);
					m_ResourceList.InsertItem(k, temp);
					temp.Format(TEXT("0x%-4X"), MenuList->at(j)->MenuElemList[k]->element->z1);
					m_ResourceList.SetItemText(k, 1, temp);
					temp.Format(TEXT("0x%-4X"), MenuList->at(j)->MenuElemList[k]->element->z2);
					m_ResourceList.SetItemText(k, 2, temp);
					temp.Format(TEXT("%d"), MenuList->at(j)->MenuElemList[k]->element->cmd);
					m_ResourceList.SetItemText(k, 3, temp);
					temp.Format(TEXT("%d"), MenuList->at(j)->MenuElemList[k]->element->submenuID);
					m_ResourceList.SetItemText(k, 4, temp);
					temp.Format(TEXT("0x%-2X"), MenuList->at(j)->MenuElemList[k]->element->z3);
					m_ResourceList.SetItemText(k, 5, temp);
					temp.Format(TEXT("%d"), MenuList->at(j)->MenuElemList[k]->element->textID);
					m_ResourceList.SetItemText(k, 6, temp);
					temp.Format(TEXT("0x%-2X"), MenuList->at(j)->MenuElemList[k]->element->z4);
					m_ResourceList.SetItemText(k, 7, temp);
					temp.Format(TEXT("0x%-4X"), MenuList->at(j)->MenuElemList[k]->element->z5);
					m_ResourceList.SetItemText(k, 8, temp);
					temp.Format(TEXT("0x%-4X"), MenuList->at(j)->MenuElemList[k]->element->z6);
					m_ResourceList.SetItemText(k, 9, temp);
					temp.Format(TEXT("%d"), MenuList->at(j)->MenuElemList[k]->element->textID2);
					m_ResourceList.SetItemText(k, 10, temp);
					m_ResourceList.SetItemToolTipText(k, 10, _T("Double click to link to the string!"));
					temp.Format(TEXT("0x%-2X"), MenuList->at(j)->MenuElemList[k]->element->z7);
					m_ResourceList.SetItemText(k, 11, temp);
					temp.Format(TEXT("%d"), MenuList->at(j)->MenuElemList[k]->element->typeID);
					m_ResourceList.SetItemText(k, 12, temp);
					m_ResourceList.SetItemToolTipText(k, 12, _T("Double click to link to font used by this menu!"));
				}
				break;
			}
		}
		break;
	case 2: //type
		for (j = 0; j < TypeList->size(); j++)
		{
			if (TypeList->at(j)->id==sel_id)
			{
				
				temp.Format(TEXT("%d"), TypeList->at(j)->element->fontID);
				m_ResourceList.InsertItem(0, temp);
				temp.Format(TEXT("0x%-2X"), TypeList->at(j)->element->z1);
				m_ResourceList.SetItemText(0, 1, temp);
				temp.Format(TEXT("%d"), TypeList->at(j)->element->size);
				m_ResourceList.SetItemText(0, 2, temp);
				temp.Format(TEXT("0x%-4X"), TypeList->at(j)->element->color);
				m_ResourceList.SetItemText(0, 3, temp);
				m_ResourceList.SetItemToolTipText(0, 3, _T("Double click to change font color!"));
				temp.Format(TEXT("0x%-1X"), TypeList->at(j)->element->style);
				m_ResourceList.SetItemText(0, 4, temp);
				m_ResourceList.SetItemToolTipText(0, 4, _T("Tip for style: Value 1 means bold, 0 means Regular."));
				temp.Format(TEXT("0x%-1X"), TypeList->at(j)->element->alignment);
				m_ResourceList.SetItemText(0, 5, temp);
				m_ResourceList.SetItemToolTipText(0, 5, _T("Double click to change alignment of font!"));
				temp.Format(TEXT("0x%-2X"), TypeList->at(j)->element->z2);
				m_ResourceList.SetItemText(0, 6, temp);
				break;
			}
		}
		break;
	case 3: //tcmd
		for (j = 0; j < TCMDList->at(0)->element.elements.size(); j++)
		{
			if (TCMDList->at(0)->element.elements.at(j)->id==sel_id)
			{
				
				temp.Format(TEXT("%d"), TCMDList->at(0)->element.elements.at(j)->id);
				m_ResourceList.InsertItem(0, temp);
				temp.Format(TEXT("0x%-2X"), TCMDList->at(0)->element.elements.at(j)->z0);
				m_ResourceList.SetItemText(0, 1, temp);
				temp.Format(TEXT("0x%-2X"), TCMDList->at(0)->element.elements.at(j)->z1);
				m_ResourceList.SetItemText(0, 2, temp);
				temp.Format(TEXT("%d"), TCMDList->at(0)->element.elements.at(j)->num_subs);
				m_ResourceList.SetItemText(0, 3, temp);
				temp.Format(TEXT("%d"), TCMDList->at(0)->element.elements.at(j)->subcmd_len);
				m_ResourceList.SetItemText(0, 4, temp);
				temp.Format(TEXT("0x%-2X"), TCMDList->at(0)->element.elements.at(j)->z1);
				m_ResourceList.SetItemText(0, 5, temp);
				break;
			}
		}
		break;
	}

	m_ResourceList.ShowWindow(SW_SHOW);
	UpdateData(FALSE);
	GetDlgItem(IDC_STATIC_LOADINGLIST)->ShowWindow(SW_HIDE);
	*pResult = 0;
}

void CLayoutDialog::OnBnClickedSaveallresourcesButton()
{
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

	CString filename;
	CString temp;
	DWORD j;
	int k;
	int sel_id;
	bool found=false;
	//sel_id=_ttoi(sID.GetBuffer());
	sel_id=0;
	LPBYTE lpBuf;
	UINT nCount;

	int rs_count=0;
	
	switch (m_ResourceType)
	{
	case 0: //view
		for (j = 0; j < ViewList->size(); j++)
		{
			nCount=4;
			for (k = 0; k < ViewList->at(j)->num_elems; k++)
			{
				nCount+=ViewList->at(j)->ViewElemList[k]->element->size+4;
			}
			lpBuf=(LPBYTE)ViewList->at(j)->ViewElemList[0]->element-4;
			filename.Format(TEXT("%s\\view%d.rsrc"), folderPath, ViewList->at(j)->id);
			SaveRAWResource(filename, lpBuf, nCount);
			rs_count++;
		}
		break;
	case 1: //menu
		for (j = 0; j < MenuList->size(); j++)
		{
			nCount=4;
			for (k = 0; k < MenuList->at(j)->num_elems; k++)
			{
				nCount+=MenuList->at(j)->MenuElemList[k]->element->size+4;
			}
			lpBuf=(LPBYTE)MenuList->at(j)->MenuElemList[0]->element-4;
			filename.Format(TEXT("%s\\menu%d.rsrc"), folderPath, MenuList->at(j)->id);
			SaveRAWResource(filename, lpBuf, nCount);
			rs_count++;
		}
		break;
	case 2: //type
		filename.Format(TEXT("type%d.rsrc"), folderPath, sel_id);
		for (j = 0; j < TypeList->size(); j++)
		{
			nCount=sizeof(*TypeList->at(j)->element);
			lpBuf=(LPBYTE)TypeList->at(j)->element;
			filename.Format(TEXT("%s\\type%d.rsrc"), folderPath, TypeList->at(j)->id);
			SaveRAWResource(filename, lpBuf, nCount);
			rs_count++;
		}
		break;
	case 3: //type
		filename.Format(TEXT("tcmd%d.rsrc"), folderPath, sel_id);
		for (j = 0; j < TCMDList->at(0)->element.elements.size(); j++)
		{
			nCount=sizeof(*TCMDList->at(0)->element.elements.at(j));
			lpBuf=(LPBYTE)TCMDList->at(0)->element.elements.at(j);
			filename.Format(TEXT("%s\\tcmd%d.rsrc"), folderPath, TCMDList->at(0)->element.elements.at(j)->id);
			SaveRAWResource(filename, lpBuf, nCount);
			rs_count++;
		}
		break;
	}

	CString sMsg;
	sMsg.Format(_T("Save %d resources successfully!"), rs_count);
	MessageBox(sMsg);
}

void CLayoutDialog::SaveRAWResource(CString filename, LPBYTE lpBuf, UINT nCount)
{
	CFile file;

	if (!file.Open(filename, CFile::modeCreate | CFile::modeWrite))
	{
		MessageBox(_T("Can't create resource file for saving!"));
		return;
	}

	file.Write(lpBuf, nCount);
	file.Close();
}

void CLayoutDialog::OnBnClickedLoadallresourcesButton()
{
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
	LoadAllResources(folderPath, m_ResourceType);
}

void CLayoutDialog::LoadAllResources(LPCTSTR folderPath, DWORD res_type, BOOL silent)
{
	DWORD index;
	CString filename = folderPath;
	
	switch (res_type)
	{
	case 0: //view
		filename += "\\view??????.rsrc";
		break;
	case 1: //menu
		filename += "\\menu??????.rsrc";
		break;
	case 2: //type =font
		filename += "\\type??????.rsrc";
		break;
	}

	bool found=false;
	LPBYTE lpBuf;
	UINT nCount;
	int rs_count=0;

	WIN32_FIND_DATA findData;
	HANDLE hFind = FindFirstFile(filename, &findData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			filename = folderPath;
			filename += "\\";
			filename += findData.cFileName;
			index = 0xFFFFFFFF;

			found=false;
			int sel_id=_wtoi(findData.cFileName + 4);
			DWORD j;
			int k;
			switch (res_type)
			{
			case 0: //view
				for (j = 0; j < ViewList->size(); j++)
				{
					if (ViewList->at(j)->id==sel_id)
					{
						nCount=4;
						for (k = 0; k < ViewList->at(j)->num_elems; k++)
						{
							nCount+=ViewList->at(j)->ViewElemList[k]->element->size+4;
						}
						lpBuf=(LPBYTE)ViewList->at(j)->ViewElemList[0]->element-4;
						found=true;
						break;
					}
				}
				break;
			case 1: //menu
				for (j = 0; j < MenuList->size(); j++)
				{
					if (MenuList->at(j)->id==sel_id)
					{
						nCount=4;
						for (k = 0; k < MenuList->at(j)->num_elems; k++)
						{
							nCount+=MenuList->at(j)->MenuElemList[k]->element->size+4;
						}
						lpBuf=(LPBYTE)MenuList->at(j)->MenuElemList[0]->element-4;
						found=true;
						break;
					}
				}
				break;
			case 2: //type
				for (j = 0; j < TypeList->size(); j++)
				{
					if (TypeList->at(j)->id==sel_id)
					{
						nCount=sizeof(*TypeList->at(j)->element);
						lpBuf=(LPBYTE)TypeList->at(j)->element;
						found=true;
						break;
					}
				}
				break;
			case 3: //tcmd
				for (j = 0; j < TCMDList->at(0)->element.elements.size(); j++)
				{
					if (TCMDList->at(0)->element.elements.at(j)->id==sel_id)
					{
						nCount=sizeof(*TCMDList->at(0)->element.elements.at(j));
						lpBuf=(LPBYTE)TCMDList->at(0)->element.elements.at(j);
						found=true;
						break;
					}
				}
				break;
			}

			if (found==true)
			{
				index=sel_id;
				CFile file;
	
				if (!file.Open(filename, CFile::modeRead))
				{
					MessageBox(TEXT("Unable to load a resource file!"));
					return;
				}
				
				DWORD filelen=(DWORD)file.GetLength();
				if (filelen!=nCount)
				{
					MessageBox(_T("One of the resource file you chose doesn't fit this resource!"));
					return;
				}
				else
				{
					file.Read(lpBuf, nCount);
					file.Close();
					rs_count++;
				}	
			}
		} while (FindNextFile(hFind, &findData) != 0);

		FindClose(hFind);

	}

	if (rs_count>0)
	{
		if (!silent)
		{
			CString sMsg;
			sMsg.Format(TEXT("Loaded %d resources file successfully!"), rs_count);
			MessageBox(sMsg);
		}

		LRESULT res;
		NMTREEVIEW nm;
		nm.itemNew.hItem=m_ResourceTree.GetRootItem();
		OnTvnSelchangedResourceTree((NMHDR *)&nm, &res);
		m_HexWnd.Invalidate();
	}
	else if (!silent)
	{
		MessageBox(_T("No resources saved!"));
	}
}