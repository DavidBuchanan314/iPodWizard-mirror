// firmware.cpp

#include "stdafx.h"
#include "firmware.h"
#include "Picture.h"
#include "Font.h"
#include "iPodWizard.h"

DWORD byte_cap_pos = 0;

#define PARTITION_MAP_ADDRESS			0x4200

#define IS_STRING_CHAR(x)	((x >= ' ' && x <= '~') || x == 0x0D || x == 0x0A || x == 0x09)

/////////////////////////////
//
// CFirmware class
//
/////////////////////////////

// construction

CFirmware::CFirmware()
{
	m_Buffer = NULL;
	m_BufferSize = 0;
	m_NumPartitions = 0;
}

CFirmware::~CFirmware()
{
	Reset();
}

// implementation

void CFirmware::Reset()
{
	if (m_Buffer != NULL)
	{
		delete m_Buffer;
	}

	m_Buffer = NULL;
	m_BufferSize = 0;

	m_NumPartitions = 0;

	m_Pictures.RemoveAll();
	m_PicturesType.RemoveAll();
	m_Fonts.RemoveAll();
	m_Strings.RemoveAll();
}

// operations

BOOL CFirmware::SetFirmware(LPCTSTR name, LPBYTE buffer, DWORD size)
{
	// reset
	Reset();

	m_Buffer = new BYTE[size];
	if (m_Buffer == NULL)
		return FALSE;

	memcpy(m_Buffer, buffer, size);

	m_BufferSize = size;

	m_Name = name;

	char ata[4]={0x21,0x41,0x54,0x41}; 
	m_pPartitions = (IPOD_PARTITION_HEADER *)&m_Buffer[PARTITION_MAP_ADDRESS];
	while (memcmp(&m_pPartitions[m_NumPartitions].type, &ata, 4) == 0)
	{
		m_PartitionInfo[m_NumPartitions].checksumStartAddress = m_pPartitions[m_NumPartitions].devOffset;

		if (strcmp(m_pPartitions[m_NumPartitions].id, "soso")==0 || strcmp(m_pPartitions[m_NumPartitions].id, "crsr")==0)
		{
			//Known images so far:
			//soso: OS Code with initial resources such as pictures and bitmap fonts
			//crsr: Resources image, 5g and above which contain OpenType Bitmap fonts and video stuff
			m_PartitionInfo[m_NumPartitions].bCanModify = TRUE;

			// check checksum
			if (m_pPartitions[m_NumPartitions].checksum != ComputeChecksum(m_NumPartitions))
			{
				// hack
				//Checksum works from devOffset+0x200 so let's check if it's really is
				m_PartitionInfo[m_NumPartitions].checksumStartAddress += 0x200;
				if (m_pPartitions[m_NumPartitions].checksum != ComputeChecksum(m_NumPartitions))
				{
					m_PartitionInfo[m_NumPartitions].checksumStartAddress = m_pPartitions[m_NumPartitions].devOffset;
					m_PartitionInfo[m_NumPartitions].bCanModify = FALSE;
				}
			}
		}
		else if (strcmp(m_pPartitions[m_NumPartitions].id, "dpua")==0)
		{
			//do not let user edit software flash update, it's risky and no resources or strings are there
			m_PartitionInfo[m_NumPartitions].bCanModify = FALSE;
		}
		else
		{
			//we don't know what it is so we better not write it
			m_PartitionInfo[m_NumPartitions].bCanModify = FALSE;
		}

		m_NumPartitions++;
	}

	return TRUE;
}

//VOLUME HACK
int CFirmware::seekChaine (void)
{
	BYTE temp[8]={0x8,0x0,0x9F,0xE5,0x38,0x1,0x90,0xE5};
	BYTE temp2[8]={0x8,0x0,0x9F,0xE5,0x94,0x0,0x90,0xE5};
	for (byte_cap_pos = 0; byte_cap_pos < (BLOCK_SIZE * 2000) - 2; byte_cap_pos++)
    {
	    if (memcmp(&m_Buffer[byte_cap_pos], &temp, 8)==0 || memcmp(&m_Buffer[byte_cap_pos], &temp2, 8)==0)
		{
			//if (byte_cap_pos > BLOCK_SIZE - 3)
            //   break;
			if ((m_Buffer[byte_cap_pos+8] == 0x1 && m_Buffer[byte_cap_pos+9] == 0x0) || (m_Buffer[byte_cap_pos+8] == 0x0 && m_Buffer[byte_cap_pos+9] == 0x1))
			{
				byte_cap_pos+=8;
				return (1);
			}
            else
				continue;
         }
    }
   
   return (0);
}

int CFirmware::isCapped(void)
{
   if(seekChaine())
   {
      if(m_Buffer[byte_cap_pos] == 0x1 && m_Buffer[byte_cap_pos+1] == 0x0)
      {
         return 1;
      }

      if(m_Buffer[byte_cap_pos] == 0x0 && m_Buffer[byte_cap_pos+1] == 0x1)
      {
         return 0;
      }
   }

   return -1;
}

DWORD CFirmware::unCap(void)
{
   m_Buffer[byte_cap_pos]=0x0;
   m_Buffer[byte_cap_pos+1]=0x1;
   
   if(!(m_Buffer[byte_cap_pos] == 0x0 && m_Buffer[byte_cap_pos+1] == 0x1))
   {
      MessageBox(0, TEXT("Failed to uncap your iPod.\nPlease do not write this modded firmware unless you want to screw your iPod."), TEXT("Failed to uncap"), MB_OK);
	  return 0;
   }
   else
   {
      MessageBox(0, TEXT("Successfully uncapped your iPod!"), TEXT("Success"), MB_OK);
	  return byte_cap_pos;
   }
   return -1;
}

DWORD CFirmware::reCap(void)
{
   m_Buffer[byte_cap_pos]=0x1;
   m_Buffer[byte_cap_pos+1]=0x0;
  
   if(m_Buffer[byte_cap_pos] == 0x1 && m_Buffer[byte_cap_pos+1] == 0x0)
   {
      MessageBox(0, TEXT("Successfully recapped your iPod!"), TEXT("Success"), MB_OK);
	  return byte_cap_pos;
   }
   else
   {
      MessageBox(0, TEXT("Failed to cap your iPod.\nPlease do not write this modded firmware unless you want to screw your iPod."), TEXT("Failed to cap"), MB_OK);
	  return 0;
   }
   return -1;
}
//VOLUME HACK

int CFirmware::PType()
{
	if (m_Name.Find(TEXT("13."))!=-1)
		return 1;
	else if (m_Name.Find(TEXT("14."))!=-1)
		return 0;
	else
		return -1;
}

void CFirmware::ScanFirmware(CWnd *pWnd)
{
	DWORD i,j;
	DWORD startOffset,endOffset;


	// scan firmware for regular pictures
	pWnd->SendMessage(WM_APP, (WPARAM)TEXT("Scanning for pictures..."), 0);

	CPicture pic;

	if (PType()==1)
		pic.SetPictureMode(2);
	else if (PType()==0)
	{
		pic.SetPictureMode(0);
	}
	for (j=0;j<m_NumPartitions;j++)
	{
		startOffset=m_pPartitions[j].devOffset;
		endOffset=startOffset+m_pPartitions[j].len;
	
		for (i = startOffset; i < endOffset; i+= 4)
		{
			if (pic.Read(&m_Buffer[i], TRUE))
			{
				m_Pictures.Add(&m_Buffer[i]);
				m_PicturesType.Add(0);
				i += 4;
				pWnd->SendMessage(WM_APP, 0, i);
			}
		}
	}

	// scan firmware for boot pictures
	pWnd->SendMessage(WM_APP, (WPARAM)TEXT("Scanning for boot pictures..."), 0);

	pic.SetPictureMode(1);
	for (j=0;j<m_NumPartitions;j++)
	{
		startOffset=m_pPartitions[j].devOffset;
		endOffset=startOffset+m_pPartitions[j].len;
	
		for (i = startOffset; i < endOffset; i+= 4)
		{
			if (pic.Read(&m_Buffer[i], TRUE))
			{
				m_Pictures.Add(&m_Buffer[i]);
				m_PicturesType.Add(1);
				i += 4;
				if ((i % 100) == 0)
					pWnd->SendMessage(WM_APP, 0, i);
			}
		}
	}

	// scan firmware for fonts

	pWnd->SendMessage(WM_APP, (WPARAM)TEXT("Scanning for fonts..."), 0);

	LPBYTE lpEnd = m_Buffer + m_BufferSize;

	CIpodFont font;
	for (j=0;j<m_NumPartitions;j++)
	{
		startOffset=m_pPartitions[j].devOffset;
		endOffset=startOffset+m_pPartitions[j].len;
	
		for (i = startOffset; i < endOffset; i+= 4)
		{
			if (font.Read(&m_Buffer[i], lpEnd))
			{
				m_Fonts.Add(&m_Buffer[i]);
				i += font.GetFontBlockLen() - 4;
				if ((i % 100) == 0)
					pWnd->SendMessage(WM_APP, 0, i);
			}
		}
	}

	// scan firmware for strings

	pWnd->SendMessage(WM_APP, (WPARAM)TEXT("Scanning for strings..."), 0);

	BYTE c;
	BOOL bInString = FALSE;
	DWORD stringStart;

	for (j=0;j<m_NumPartitions;j++)
	{
		startOffset=m_pPartitions[j].devOffset;
		endOffset=startOffset+m_pPartitions[j].len;
	
		for (i = startOffset; i < endOffset; i++)
		{
			c = m_Buffer[i];
			if (bInString)
			{
				if (c == 0)
				{
					if ((i - stringStart) >= 3) // at least 3 chars
					{
						m_Strings.Add(&m_Buffer[stringStart]);
					}
					bInString = FALSE;
				}
				else if (!IS_STRING_CHAR(c))
				{
					bInString = FALSE;
				}
			}
			else
			{
				if (IS_STRING_CHAR(c))
				{
					bInString = TRUE;
					stringStart = i;
				}
			}
			if ((i % 100) == 0)
				pWnd->SendMessage(WM_APP, 0, i);
		}
	}
}

LPBYTE CFirmware::GetFirmwareBuffer()
{
	return m_Buffer;
}

DWORD CFirmware::GetFirmwareSize()
{
	return m_BufferSize;
}

CString CFirmware::GetName()
{
	return m_Name;
}

DWORD CFirmware::ComputeChecksum(int index)
{
	DWORD i;

	if (m_Buffer == NULL)
		return 0;

	DWORD checksum = 0;
	DWORD blockLen = m_pPartitions[index].len;

	DWORD startAddress = m_PartitionInfo[index].checksumStartAddress;

	for (i = startAddress; i < startAddress + blockLen; i++)
	{
		checksum += m_Buffer[i];
	}

	/*
	if (index == 1)
	{
		DWORD j;

		for (i = 0; i < m_BufferSize; i+= 4)
		{
			checksum = 0;
			for (j = i; j < m_BufferSize; j++)
			{
				checksum += m_Buffer[j];
				if (checksum >= m_pPartitions[index].checksum)
					break;
			}

			if (checksum == m_pPartitions[index].checksum && ((j - i + 1) % 4) == 0)
			{
				TRACE("Address 0x%X length 0x%X\r\n", i, j - i + 1);
			}
		}
	}
	*/
	
	return checksum;
}

void CFirmware::SyncChecksum()
{
	int i;

	for (i = 0; i < m_NumPartitions; i++)
	{
		if (m_PartitionInfo[i].bCanModify)
		{
			m_pPartitions[i].checksum = ComputeChecksum(i);
		}
	}
}

int CFirmware::GetNumPartitions()
{
	return m_NumPartitions;
}

void CFirmware::GetPartitionChecksums(int index, LPDWORD pChecksum1, LPDWORD pChecksum2)
{
	if (pChecksum1 == NULL || pChecksum2 == NULL || index >= m_NumPartitions)
		return;

	*pChecksum1 = m_pPartitions[index].checksum;
	*pChecksum2 = ComputeChecksum(index);
}

BOOL CFirmware::CanWrite(DWORD offset, DWORD len)
{
	for (int i = 0; i < m_NumPartitions; i++)
	{
		if (offset >= m_PartitionInfo[i].checksumStartAddress &&
			offset + len <= (m_PartitionInfo[i].checksumStartAddress + m_pPartitions[i].len) &&
			m_PartitionInfo[i].bCanModify)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CFirmware::CanWritePicture(DWORD index)
{
	DWORD offset = (DWORD)((LPBYTE)m_Pictures.GetAt(index) - m_Buffer);

	return CanWrite(offset, 1);
}

DWORD CFirmware::GetNumPictures()
{
	return (DWORD)m_Pictures.GetCount();
}

LPBYTE CFirmware::GetPicture(DWORD index)
{
	if (index >= (DWORD)m_Pictures.GetCount())
		return NULL;

	return (LPBYTE)m_Pictures.GetAt(index);
}

DWORD CFirmware::GetPictureType(DWORD index)
{
	if (index >= (DWORD)m_PicturesType.GetCount())
		return FALSE;

	return (DWORD)m_PicturesType.GetAt(index);
}

DWORD CFirmware::GetNumFonts()
{
	return (DWORD)m_Fonts.GetCount();
}

LPBYTE CFirmware::GetFont(DWORD index)
{
	if (index >= (DWORD)m_Fonts.GetCount())
		return NULL;

	return (LPBYTE)m_Fonts.GetAt(index);
}

BOOL CFirmware::CanWriteFont(DWORD index)
{
	DWORD offset = (DWORD)((LPBYTE)m_Fonts.GetAt(index) - m_Buffer);

	return CanWrite(offset, 1);
}

DWORD CFirmware::GetNumStrings()
{
	return (DWORD)m_Strings.GetCount();
}



LPCTSTR CFirmware::GetString(DWORD index)
{
	if (index >= (DWORD)m_Strings.GetCount())
		return FALSE;

	return (LPCTSTR)m_Strings.GetAt(index);
}