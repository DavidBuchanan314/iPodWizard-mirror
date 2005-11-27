#include "stdafx.h"
#include "iPodWizard.h"
#include "UpdaterDialog.h"

IMPLEMENT_DYNAMIC(CUpdaterDialog, CDialog)

CUpdaterDialog::CUpdaterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdaterDialog::IDD, pParent)
{
}

CUpdaterDialog::~CUpdaterDialog()
{
}

void CUpdaterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FIRMWARE_LIST, m_FirmwareList);
}

BEGIN_MESSAGE_MAP(CUpdaterDialog, CDialog)
	ON_BN_CLICKED(IDC_EXTRACT_BUTTON, OnBnClickedExtractFirmware)
	ON_BN_CLICKED(IDC_LOAD_BUTTON, OnBnClickedLoadFirmware)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, OnBnClickedDeleteFirmware)
	ON_BN_CLICKED(IDC_WRITEALL_BUTTON, OnBnClickedWriteAllChanges)
END_MESSAGE_MAP()

BOOL CUpdaterDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_FirmwareList.SetExtendedStyle(m_FirmwareList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_FirmwareList.InsertColumn(0, TEXT("Resource Name"), LVCFMT_LEFT, 200);
	m_FirmwareList.InsertColumn(1, TEXT("Length"), LVCFMT_LEFT, 100);

	m_LoadedFirm.RemoveAll();
	m_LoadedFirmStr.RemoveAll();

	return TRUE;
}

void CUpdaterDialog::GenerateList(CResourceManager *pRsrcMgr, CString Filename)
{
	m_pRsrcMgr = pRsrcMgr;
	m_Filename.Format(Filename);

	m_FirmwareList.DeleteAllItems();
	int numResources = (int)m_pRsrcMgr->GetNumResources();
	int x=0;
	CString s;

	for (int i = 0; i < numResources; i++)
	{
		m_FirmwareList.InsertItem(x, m_pRsrcMgr->GetResourceName(i));
		s.Format(TEXT("%d"), m_pRsrcMgr->UpdGetResourceSize(FIRMWARE_RESOURCE_TYPE, m_pRsrcMgr->GetResourceName(i)));
		m_FirmwareList.SetItemText(x, 1, s);
		x++;
	}
}

void CUpdaterDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	// CDialog::OnOK();
}

void CUpdaterDialog::OnBnClickedExtractFirmware()
{
	if (m_FirmwareList.GetFirstSelectedItemPosition()==NULL)
	{
		MessageBox(TEXT("You need to select the firmware you want to extract!"));
		return;
	}

	POSITION p=m_FirmwareList.GetFirstSelectedItemPosition();
	int nItem=m_FirmwareList.GetNextSelectedItem(p);
	CString filename;
	filename=m_FirmwareList.GetItemText(nItem, 0);
	CFileDialog dlg(FALSE, NULL, filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("All Files (*.*)|*.*||"), this);

	if (dlg.DoModal() != IDOK)
		return;
	
	LPBYTE buffer=m_pRsrcMgr->UpdLoadResource(FIRMWARE_RESOURCE_TYPE, m_pRsrcMgr->GetResourceName(nItem));
	if (buffer==NULL)
		MessageBox(TEXT("Couldn't load firmware data to memory for extraction!"));
	else
	{
		CFile file;
		if (!file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
		{
			MessageBox(TEXT("Unable to save file!"));
			return;
		}

		DWORD size;
		CString s;
		s=m_FirmwareList.GetItemText(nItem, 1);
		if (_stscanf(s, TEXT("%d"), &size) == 1)
		{
			file.Write(buffer, size);
			file.Close();
			m_pRsrcMgr->UpdFreeResource();
			MessageBox(TEXT("Extracted firmware successfully!"));
		}
		else
		{
			file.Close();
			MessageBox(TEXT("Error writing file buffer!"));
		}
	}
}

void CUpdaterDialog::OnBnClickedLoadFirmware()
{
	if (m_FirmwareList.GetFirstSelectedItemPosition()==NULL)
	{
		MessageBox(TEXT("You need to select the firmware the new firmware binary will be loaded too!"));
		return;
	}
	
	CString name;
	POSITION p=m_FirmwareList.GetFirstSelectedItemPosition();
	int nItem=m_FirmwareList.GetNextSelectedItem(p);
	name = m_FirmwareList.GetItemText(nItem, 0);
	DWORD size;
	CString s;
	s=m_FirmwareList.GetItemText(nItem, 1);
	if (_stscanf(s, TEXT("%d"), &size) != 1)
	{
		MessageBox(TEXT("Internal memory error!"));
		return;
	}

	for (INT_PTR i=0;i<m_LoadedFirmStr.GetCount();i++)
		if (m_LoadedFirmStr.GetAt(i).Compare(name)==0)
		{
			MessageBox(TEXT("You've already loaded a binary to this firmware!"));
			return;
		}

	CFileDialog dlg(TRUE, NULL, name, OFN_HIDEREADONLY, TEXT("All Files (*.*)|*.*||"), this);

	if (dlg.DoModal() != IDOK)
		return;

	CFile file;
	if (!file.Open(dlg.GetPathName(), CFile::modeRead))
	{
		MessageBox(TEXT("Unable to load file!"));
		return;
	}

	LPBYTE buffer;
	buffer = new BYTE[size];
	if (file.Read(buffer, size) < size)
	{
		MessageBox(TEXT("Error reading file!"));
		return;
	}
	file.Close();
	
	m_LoadedFirm.Add(buffer);
	m_LoadedFirmStr.Add(name);
	m_LoadedFirmSize.Add(size);

	MessageBox(TEXT("Successfully loaded firmware binary."));
}

void CUpdaterDialog::OnBnClickedDeleteFirmware()
{
	//TODO:
	//Delete resource binary from PE header because now it only deletes the reference.
	MessageBox(TEXT("This feature is currently not fully supported and therefore disabled."));
	return;
	//

	if (m_FirmwareList.GetFirstSelectedItemPosition()==NULL)
		return;

	CString name;
	POSITION p=m_FirmwareList.GetFirstSelectedItemPosition();
	int nItem=m_FirmwareList.GetNextSelectedItem(p);
	name = m_FirmwareList.GetItemText(nItem, 0);
	
	INT_PTR i=-1;
	for (i=0;i<m_LoadedFirmStr.GetCount();i++)
		if (m_LoadedFirmStr.GetAt(i).Compare(name)==0)
		{
			if (MessageBox(TEXT("You've already loaded a binary to this firmware you're about to delete.\nIf you choose to delete it, than it won't be loaded like you wanted to in the first place.\nAre you sure you want to delete?"), TEXT("Warning"), MB_YESNO) != IDYES)
				return;
			else
				break;
		}

	if (i!=-1)
	{
		m_LoadedFirm.Add(NULL);
		m_LoadedFirmSize.Add(0);
		m_LoadedFirmStr.Add(name);
	}
	else
	{
		delete[] m_LoadedFirm.GetAt(i);
		m_LoadedFirm.SetAt(i, NULL);
		m_LoadedFirmSize.SetAt(i, 0);
	}
	m_FirmwareList.DeleteItem(nItem);
}

void CUpdaterDialog::OnBnClickedWriteAllChanges()
{
	if (m_LoadedFirm.GetCount()<=0)
	{
		MessageBox(TEXT("You made no changes to the updater and therefore no need to write it."));
		return;
	}
	if (m_pRsrcMgr->UpdChangeResources(m_Filename, FIRMWARE_RESOURCE_TYPE, &m_LoadedFirm, &m_LoadedFirmStr, &m_LoadedFirmSize)==FALSE)
	{
		MessageBox(TEXT("Error writing updater!"));
		return;
	}

	for (INT_PTR i=0;i<m_LoadedFirm.GetCount();i++)
		if (m_LoadedFirm.GetAt(i)!=NULL)
			delete[] m_LoadedFirm.GetAt(i);

	m_LoadedFirm.RemoveAll();
	m_LoadedFirmStr.RemoveAll();

	MessageBox(TEXT("Changes written to the updater file successfully!"));

	if (!m_pRsrcMgr->Open(m_Filename))
	{
		DWORD err = GetLastError();
		CString s;
		s.Format(TEXT("Unable to reopen file! Code=%d"), err);
		MessageBox(s);
	}
}