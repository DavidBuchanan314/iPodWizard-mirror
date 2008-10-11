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
	ON_BN_CLICKED(IDC_EXTRACTIPOD_BUTTON, OnBnClickedExtractiPodFirmware)
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
	MO_LOAD_FIRMWARE_PATH(dlg)

	if (dlg.DoModal() != IDOK)
		return;
	MO_SAVE_FIRMWARE_PATH(dlg)	
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
			MessageBox(TEXT("Extracted firmware successfully!\nWarning: According to Apple's EULA, uploading firmware to 3rd party websites is prohibited! Use this firmware for personal use only!"));
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

	//warning
	MessageBox(TEXT("Be aware that the binary file you are about to load must be a valid firmware!\nIf not, you can severly screw your iPod and you can pray for something to fix it.\nEverything should be ok if you extracted the firmware using iPodWizard."));

	CFileDialog dlg(TRUE, NULL, name, OFN_HIDEREADONLY, TEXT("All Files (*.*)|*.*||"), this);
	MO_LOAD_FIRMWARE_PATH(dlg)

	if (dlg.DoModal() != IDOK)
		return;
	MO_SAVE_FIRMWARE_PATH(dlg)
	CFile file;
	if (!file.Open(dlg.GetPathName(), CFile::modeRead))
	{
		MessageBox(TEXT("Unable to load file!"));
		return;
	}

	/*if (size!=file.GetLength())
	{
		file.Close();
		MessageBox(TEXT("The firmware binary size you are about to load has to be exactly like the firmware in the updater!"));
		return;
	}*/
	size=(DWORD)file.GetLength();

	//make firmware checkups
	LPBYTE buf=m_pRsrcMgr->UpdLoadResource(FIRMWARE_RESOURCE_TYPE, m_pRsrcMgr->GetResourceName(nItem));
	if (buf==NULL)
		MessageBox(TEXT("Couldn't load firmware data to memory for checkup!"));

	LPBYTE buffer;
	buffer = new BYTE[size];
	if (file.Read(buffer, size) < size)
	{
		MessageBox(TEXT("Error reading file!"));
		return;
	}

	char ata[4]={0x21,0x41,0x54,0x41};
	IPOD_PARTITION_HEADER *part;
	IPOD_PARTITION_HEADER *part2;
	part=(IPOD_PARTITION_HEADER *)&buffer[theApp.PARTITION_MAP_ADDRESS];
	part2=(IPOD_PARTITION_HEADER *)&buf[theApp.PARTITION_MAP_ADDRESS];
	int num=0;
	DWORD loadAddr=0,loadAddr2=0,offset=0,offset2=0;
	while (memcmp(part[num].type, &ata, 4) == 0 || memcmp(part2[num].type, &nand, 4) == 0)
	{
		if (strcmp(part2[num].id, "soso")==0) // file
		{
			//load address is 0xffff after the firmware is loaded and therefore we need to change it
			offset2=theApp.PARTITION_MAP_ADDRESS + sizeof(IPOD_PARTITION_HEADER) * (num+1) - 4;
			loadAddr2=part2[num].loadAddr;
		}
		if (strcmp(part[num].id, "soso")==0) //real
		{
			//if load address is 0xffff meaning the flash update occured but we need a workign load addr in the firmware so the next time the ipod will flash the firmware and therefore we need to change it
			offset=theApp.PARTITION_MAP_ADDRESS + sizeof(IPOD_PARTITION_HEADER) * (num+1) - 4;
			loadAddr=part[num].loadAddr;
		}
		num++;
	}
	if (offset==0 || offset2==0)
	{
		m_pRsrcMgr->UpdFreeResource();
		file.Close();
		MessageBox(TEXT("Error finding load address offset! It's danagerous to load this kind of firmware and therefore operation is cancelled!"));
		return;
	}
	if (loadAddr2==0xFFFFFFFF)
	{
		//that's an iPod extracted firmware
		memcpy(&buffer[offset2],&loadAddr,4);
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

void CUpdaterDialog::OnBnClickedExtractiPodFirmware()
{
	if (theApp.m_DeviceSel.IsEmpty() == TRUE)
	{
		MessageBox(TEXT("iPod not connected or unmounted!"));
		return;
	}

	CFileDialog dlg(FALSE, NULL,NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("All Files (*.*)|*.*||"), this);
	MO_LOAD_FIRMWARE_PATH(dlg)

	if (dlg.DoModal() != IDOK)
		return;
	MO_SAVE_FIRMWARE_PATH(dlg)
	TCHAR devstring[25];
	wsprintf (devstring, TEXT("%s"), theApp.m_DeviceSel);
	int dev = _wopen (devstring, O_RDONLY | _O_RAW);
	if (dev==-1)
	{
		MessageBox(TEXT("Unable to access iPod!"));
		return;
	}

	CFile file;
	if (!file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
	{
		_close(dev);
		MessageBox(TEXT("Unable to save file!"));
		return;
	}

	theApp.InitHDDValues(dev);

	DWORD size=0;
	LPBYTE partitions;
	long pos=theApp.FIRMWARE_START + theApp.PARTITION_MAP_ADDRESS;
	pos%=theApp.BLOCK_SIZE;
	_lseek(dev, theApp.FIRMWARE_START + theApp.PARTITION_MAP_ADDRESS - pos, SEEK_SET);
	partitions = new BYTE[theApp.BLOCK_SIZE];
	_read(dev, partitions, theApp.BLOCK_SIZE);
	IPOD_PARTITION_HEADER *	m_pParts;
	int num=0;
	//char ata[4]={0x21,0x41,0x54,0x41}; 
	m_pParts = (IPOD_PARTITION_HEADER *)(&partitions[pos]);
	while (memcmp(&m_pParts[num].type, &ata, 4) == 0 || memcmp(&m_pParts[num].type, &nand, 4) == 0)
	{
		if (m_pParts[num].devOffset+m_pParts[num].len > size)
			size=m_pParts[num].devOffset+m_pParts[num].len;
		num++;
	}
	delete[] partitions;
	
	LPBYTE buffer;
	DWORD i=0;
	for (i=0;i<size;i+=theApp.BLOCK_SIZE);
	size=i+theApp.BLOCK_SIZE; //extra read
	buffer = new BYTE[size];
	_lseek(dev, theApp.FIRMWARE_START, SEEK_SET);
	for (i=0;i<size;i+=theApp.BLOCK_SIZE)
		_read(dev, &buffer[i], theApp.BLOCK_SIZE);
	file.Write(buffer, size);
	delete[] buffer;
	_close(dev);
	file.Close();

	MessageBox(TEXT("Successfully saved iPod firmware!\nWarning: According to Apple's EULA, uploading firmware to 3rd party websites is prohibited! Use this firmware for personal use only!"));
}