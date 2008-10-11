// TweaksDialog.cpp : implementation file
//

#include "stdafx.h"
#include "iPodWizard.h"
#include "TweaksDialog.h"
#include ".\tweaksdialog.h"

int dev = 0, byte_cap = 0;
unsigned char buffer[512];


// CTweaksDialog dialog

IMPLEMENT_DYNAMIC(CTweaksDialog, CDialog)
CTweaksDialog::CTweaksDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTweaksDialog::IDD, pParent)
{
}

CTweaksDialog::~CTweaksDialog()
{
}

void CTweaksDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTweaksDialog, CDialog)
	ON_MESSAGE(WM_CLOSE, OnClose)
	ON_BN_CLICKED(ID_APPLY_BUTTON, OnBnClickedApply)
	ON_BN_CLICKED(ID_VOLUMECAP_BUTTON, OnBnClickedVolumeCap)
END_MESSAGE_MAP()

//Volume Capping stuff:
//Taken from goPod source code, do not mess this up as this could cause hard damage to an iPod.
//Be very carefull!!! when editing below
//START
int CTweaksDialog::isCapped(void)
{
	int chaine = 0, block_cap = 0;

	BYTE temp[8]={0x8,0x0,0x9F,0xE5,0x38,0x1,0x90,0xE5};
	BYTE temp2[8]={0x8,0x0,0x9F,0xE5,0x94,0x0,0x90,0xE5};
	   
	_lseek (dev, theApp.BLOCK_SIZE * 1000, SEEK_SET);
	   
	while ((block_cap < theApp.BLOCK_SIZE * 2000) && _read (dev, buffer, theApp.BLOCK_SIZE) != -1)
	{
		block_cap += theApp.BLOCK_SIZE;

		for (byte_cap = 7; byte_cap < theApp.BLOCK_SIZE - 2; byte_cap++)
		{
			if (memcmp(&buffer[byte_cap-7], &temp, 8)==0 || memcmp(&buffer[byte_cap-7], &temp2, 8)==0)
			{
				if (byte_cap > theApp.BLOCK_SIZE - 3)
				break;

				if ((buffer[byte_cap+1] == 0x1 && buffer[byte_cap+2] == 0x0) || (buffer[byte_cap+1] == 0x0 && buffer[byte_cap+2] == 0x1))
					goto found;
				else
					break;
			}
		}
	}
	   
	return -1;
found:
	if(buffer[byte_cap+1] == 0x1 && buffer[byte_cap+2] == 0x0)
		return 1;

	if(buffer[byte_cap+1] == 0x0 && buffer[byte_cap+2] == 0x1)
		return 0;

   return -1;
}

//Taken from goPod source code, do not mess this up as this could cause hard damage to an iPod.
int CTweaksDialog::unCap(void)
{
   buffer[byte_cap+1]=0x0;
   buffer[byte_cap+2]=0x1;
   _lseek(dev, -theApp.BLOCK_SIZE, SEEK_CUR);
   _write(dev, buffer, theApp.BLOCK_SIZE);
   _lseek(dev, -theApp.BLOCK_SIZE, SEEK_CUR);
   _read(dev, buffer, theApp.BLOCK_SIZE);
   
   if(!(buffer[byte_cap+1] == 0x0 && buffer[byte_cap+2] == 0x1))
   {
      MessageBox(TEXT("Failed to uncap your iPod.\nYou may have to restore your firmware."), TEXT("Failed to uncap"), MB_OK);
	  return 0;
   }
   else
   {
      MessageBox(TEXT("Successfully uncapped your iPod!"), TEXT("Success"), MB_OK);
	  return 1;
   }
   return -1;
}

//Taken from goPod source code, do not mess this up as this could cause hard damage to an iPod.
int CTweaksDialog::reCap(void)
{
   buffer[byte_cap+1]=0x1;
   buffer[byte_cap+2]=0x0;
   _lseek(dev, -theApp.BLOCK_SIZE, SEEK_CUR);
   _write(dev, buffer, theApp.BLOCK_SIZE);
   _lseek(dev, -theApp.BLOCK_SIZE, SEEK_CUR);
   _read(dev, buffer, theApp.BLOCK_SIZE);
   if(buffer[byte_cap+1] == 0x1 && buffer[byte_cap+2] == 0x0)
   {
      MessageBox(TEXT("Successfully capped your iPod!"), TEXT("Success"), MB_OK);
	  return 1;
   }
   else
   {
      MessageBox(TEXT("Failed to cap your iPod.\nYou may have to restore your firmware."), TEXT("Failed to cap"), MB_OK);
	  return 0;
   }
   return -1;
}

void CTweaksDialog::InitCapping()
{
	//Be carefull when changing below!
	TCHAR devstring[25];
	wsprintf (devstring, TEXT("%s"), theApp.m_DeviceSel);
	dev = _wopen (devstring, O_RDWR | _O_RAW);
	if (dev!=-1)
	{
		_lseek(dev, theApp.FIRMWARE_START, SEEK_SET);
		_read(dev, buffer, theApp.BLOCK_SIZE);
		int ret;
		ret=isCapped();
		if (ret==0)
			GetDlgItem(ID_VOLUMECAP_BUTTON)->SetWindowText(TEXT("ReCap Volume"));
	}
	else
		GetDlgItem(ID_VOLUMECAP_BUTTON)->EnableWindow(FALSE);
}
//END

// CTweaksDialog message handlers

BOOL CTweaksDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString path;
	path.SetString(theApp.m_iPodDRV);
	path.AppendFormat(TEXT("\\_show_voltage"));
	if (PathFileExists(path)==TRUE)
		CheckDlgButton(ID_VOLTAGE_CHKBOX, 1);
	path.SetString(theApp.m_iPodDRV);
	path.AppendFormat(TEXT("\\_show_speed"));
	if (PathFileExists(path)==TRUE)
		CheckDlgButton(ID_CPU_CHKBOX, 1);
	path.SetString(theApp.m_iPodDRV);
	path.AppendFormat(TEXT("\\_short_deepsleep"));
	if (PathFileExists(path)==TRUE)
		CheckDlgButton(ID_DEEPSLEEP_CHKBOX, 1);
	path.SetString(theApp.m_iPodDRV);
	path.AppendFormat(TEXT("\\_disable_cache"));
	if (PathFileExists(path)==TRUE)
		CheckDlgButton(ID_CACHE_CHKBOX, 1);
	path.SetString(theApp.m_iPodDRV);
	path.AppendFormat(TEXT("\\_no_deepsleep"));
	if (PathFileExists(path)==TRUE)
		CheckDlgButton(ID_NODEEPSLEEP_CHKBOX, 1);
	//Capping
	InitCapping();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CTweaksDialog::OnClose(WPARAM wParam, LPARAM lParam)
{
	if (dev!=-1)
		_close(dev);
	this->EndDialog(IDOK);
	return 0;
}

void CTweaksDialog::OnBnClickedApply()
{
	CString path;
	CFile file;
	path.SetString(theApp.m_iPodDRV);
	path.AppendFormat(TEXT("\\_show_voltage"));
	if (IsDlgButtonChecked(ID_VOLTAGE_CHKBOX)==0)
		DeleteFile(path);
	else if (file.Open(path, CFile::modeCreate))
		file.Close();
	path.SetString(theApp.m_iPodDRV);
	path.AppendFormat(TEXT("\\_show_speed"));
	if (IsDlgButtonChecked(ID_CPU_CHKBOX)==0)
		DeleteFile(path);
	else if (file.Open(path, CFile::modeCreate))
		file.Close();
	path.SetString(theApp.m_iPodDRV);
	path.AppendFormat(TEXT("\\_short_deepsleep"));
	if (IsDlgButtonChecked(ID_DEEPSLEEP_CHKBOX)==0)
		DeleteFile(path);
	else if (file.Open(path, CFile::modeCreate))
		file.Close();
	path.SetString(theApp.m_iPodDRV);
	path.AppendFormat(TEXT("\\_disable_cache"));
	if (IsDlgButtonChecked(ID_CACHE_CHKBOX)==0)
		DeleteFile(path);
	else if (file.Open(path, CFile::modeCreate))
		file.Close();
	path.SetString(theApp.m_iPodDRV);
	path.AppendFormat(TEXT("\\_no_deepsleep"));
	if (IsDlgButtonChecked(ID_NODEEPSLEEP_CHKBOX)==0)
		DeleteFile(path);
	else if (file.Open(path, CFile::modeCreate))
		file.Close();
	if (dev!=-1)
		_close(dev);
	this->EndDialog(IDOK);
}

void CTweaksDialog::OnBnClickedVolumeCap()
{
	CString temp;
	GetDlgItem(ID_VOLUMECAP_BUTTON)->GetWindowText(temp);
	if (dev!=-1)
	{
		if (temp.Compare(TEXT("ReCap Volume"))==0)
		{
			if (reCap()==1)
				GetDlgItem(ID_VOLUMECAP_BUTTON)->SetWindowText(TEXT("UnCap Volume"));
		}
		else
		{
			if (unCap()==1)
				GetDlgItem(ID_VOLUMECAP_BUTTON)->SetWindowText(TEXT("ReCap Volume"));
		}
	}
	else
		MessageBox(TEXT("Couldn't access the iPod drive. Check no other program is directly accessing the device."));
}
