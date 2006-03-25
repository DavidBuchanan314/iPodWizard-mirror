// firmware.cpp

#include "stdafx.h"
#include "firmware.h"
#include "Picture.h"
#include "Font.h"
#include "OTFFontParser.h"
#include "iPodWizard.h"

DWORD byte_cap_pos = 0;



#define IS_STRING_CHAR(x)	((x >= ' ' && x <= '~') || x == 0x0D || x == 0x0A || x == 0x09)

/////////////////////////////
//
// CFirmware class
//
/////////////////////////////

// construction

CFirmware::CFirmware()
{
	//reset vars
	m_Buffer = NULL;
	m_BufferSize = 0;
	m_NumPartitions = 0;
	m_SLangOffset = 0;
	m_StringOrderFixed = FALSE;
}

CFirmware::~CFirmware()
{
	Reset();
}

// implementation

void CFirmware::Reset()
{
	//reset vars
	if (m_Buffer != NULL)
	{
		delete m_Buffer;
	}

	m_Buffer = NULL;
	m_BufferSize = 0;

	m_NumPartitions = 0;

	//remove all pointers
	m_Pictures.RemoveAll();
	m_PicturesType.RemoveAll();
	m_PicturesID.RemoveAll();
	m_Fonts.RemoveAll();
	m_Strings.RemoveAll();
	m_OTFFonts.RemoveAll();
	m_iPodLang.RemoveAll();

	m_SLangOffset = 0;
	m_StringOrderFixed = FALSE;
}

// operations

BOOL CFirmware::SetFirmware(LPCTSTR name, LPBYTE buffer, DWORD size)
{
	// reset
	Reset();

	m_Buffer = new BYTE[size];
	if (m_Buffer == NULL)
		return FALSE;

	//copy resource buffer to a buffer we can use
	memcpy(m_Buffer, buffer, size);

	m_BufferSize = size;

	m_Name = name;

	char ata[4]={0x21,0x41,0x54,0x41};
	WORD firmware_version=(WORD)m_Buffer[266]; //0x10A
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
			//if firmware version is 3 (4g and above models) then 512 bytes from devOffset are random and not included in checksum
			//elsewhere, if it's 2 then it's 1-3g
			if (firmware_version!=2) //it's either 3 or we don't know this firmware but heck let's try checksum anyway
				m_PartitionInfo[m_NumPartitions].checksumStartAddress += 0x200;
			if (m_pPartitions[m_NumPartitions].checksum != ComputeChecksum(m_NumPartitions))
			{
				//checksum went wrong, meaning don't edit this image
				m_PartitionInfo[m_NumPartitions].checksumStartAddress = m_pPartitions[m_NumPartitions].devOffset;
				m_PartitionInfo[m_NumPartitions].bCanModify = FALSE;
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

int CFirmware::isCapped(void)
{
	//We search for this byte combination in firmware and after it two bytes decide either cap or not
	BYTE temp[8]={0x8,0x0,0x9F,0xE5,0x38,0x1,0x90,0xE5}; //3/4G + Color + mini
	BYTE temp2[8]={0x8,0x0,0x9F,0xE5,0x94,0x0,0x90,0xE5}; //nano + 5G
	for (byte_cap_pos = 0; byte_cap_pos < (BLOCK_SIZE * 2000) - 2; byte_cap_pos++)
	{
	    if (memcmp(&m_Buffer[byte_cap_pos], &temp, 8)==0 || memcmp(&m_Buffer[byte_cap_pos], &temp2, 8)==0)
		{
			//if (byte_cap_pos > BLOCK_SIZE - 3)
            //   break;
			if ((m_Buffer[byte_cap_pos+8] == 0x1 && m_Buffer[byte_cap_pos+9] == 0x0) || (m_Buffer[byte_cap_pos+8] == 0x0 && m_Buffer[byte_cap_pos+9] == 0x1))
			{
				byte_cap_pos+=8;
				goto found;
			}
            else
				continue;
         }
    }
   
	return -1;

found: //check if the ipod is capped or not
	if(m_Buffer[byte_cap_pos] == 0x1 && m_Buffer[byte_cap_pos+1] == 0x0)
		return 1; //capped

    if(m_Buffer[byte_cap_pos] == 0x0 && m_Buffer[byte_cap_pos+1] == 0x1)
		return 0; //uncapped

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

//Scan firmware for all the resources we may find and edit
void CFirmware::ScanFirmware(CWnd *pWnd)
{
	DWORD i;
	int j;
	DWORD startOffset,endOffset;
	CPicture pic;
	CIpodFont font;
	COTFFont ofont;

	pWnd->SendMessage(WM_APP, (WPARAM)TEXT("Scanning for pictures and strings..."), 0);
	
	for (j=0;j<m_NumPartitions;j++)
	{
		if (strcmp(m_pPartitions[j].id,"soso")!=0) //this sources will be only in OS image that's why we can skip others
			continue;

		startOffset=m_pPartitions[j].devOffset; //start offset of image in firmware
		endOffset=startOffset+m_pPartitions[j].len; //end offset of image in firmware

		int iFirst=0;
		RESOURCE_HEADER	*pRsHeader;
		char type[4];
		LPBYTE lpEnd = m_Buffer + endOffset;
	
		int y=0;
		for (i = startOffset; i < endOffset; i+= 4)
		{
			pRsHeader=(RESOURCE_HEADER *)&m_Buffer[i];
			memcpy(&type,&m_Buffer[i+sizeof(RESOURCE_HEADER)],4);
			//validate resource header
			if (KNOWN_RESOURCE(type) && pRsHeader->z1==0x03 && pRsHeader->length < 0xffff && pRsHeader->length > 0 && pRsHeader->num_sections < 50 && pRsHeader->num_sections > 0)
			{
				//each resource block is built by resource sections that has the resources in them
				DWORD data_size=0,rs_offset;
				rs_offset=i;
				i+=sizeof(RESOURCE_HEADER);
				RESOURCE_SECTION *pRsSection;
				pRsSection=(RESOURCE_SECTION *)&m_Buffer[i];
				DWORD k,x,lastend=0,total=0;
				
				//calculate the total bytes of section tables
				for (k=0;k<pRsHeader->num_sections;k++)
					total+=pRsSection[k].num_items;

				i+=sizeof(RESOURCE_SECTION)*pRsHeader->num_sections+sizeof(RESOURCE)*total; //data begin

				IPOD_LANG *il=new IPOD_LANG;
				BOOL bString=FALSE;
				RESOURCE *pRs;
				for (k=0;k<pRsHeader->num_sections;k++)
				{
					pRs=(RESOURCE *)&m_Buffer[rs_offset+pRsSection[k].start];
					
					for (x=0;x<pRsSection[k].num_items;x++)
					{
						//if (lastend && pRs[x].offset != lastend)
							//	warning
						//{
						//	i+=lastend-pRs[x].offset;
							//continue;
						//}
						//lastend = DWORD_ALIGN(pRs[x].offset+pRs[x].size);
						
						if (!strncmp(pRsSection[k].type, "paMB", 4)) //pictures
						{
							if (pic.Read(&m_Buffer[i+pRs[x].offset], TRUE))
							{
								//add data pointer for picture and picture type (0=changable)
								m_Pictures.Add(&m_Buffer[i+pRs[x].offset]);
								m_PicturesType.Add(0);
								m_PicturesID.Add(pRs[x].id);
								pWnd->SendMessage(WM_APP, 0, i);
							}
						}
						else if (!strncmp(pRsSection[k].type, "boot", 4)) //boot pictures - not working because encoded
						{
							pic.SetPictureMode(1);
							if (pic.Read(&m_Buffer[i+pRs[x].offset], TRUE))
							{
								m_Pictures.Add(&m_Buffer[i+pRs[x].offset]);
								m_PicturesType.Add(1);
								m_PicturesID.Add(pRs[x].offset);
								pWnd->SendMessage(WM_APP, 0, i);
							}
						}
						else if (!strncmp(pRsSection[k].type, " rtS", 4)) //language strings
						{
							IPOD_STRING *is=new IPOD_STRING;
							is->id=pRs[x].id;
							is->offset=i+pRs[x].offset;
							is->size=pRs[x].size;
							is->string_off=&pRs[x].offset;
							is->size_off=&pRs[x].size;
							if (is->offset<m_BufferSize)
								il->iPodString.Add(is);
							bString=TRUE;
							//p=k;
						}
						else if (!strncmp(pRsSection[k].type, "weiV", 4)) //layout
						{
							//not supported
						}
						else if (!strncmp(pRsSection[k].type, "uneM", 4)) //layout
						{
							//not supported
						}
						else if (!strncmp(pRsSection[k].type, "dmCT", 4)) //layout
						{
							//not supported
						}
						//if ((pRs[x].offset+pRs[x].size) > data_size)
						//	data_size=pRs[x].offset+pRs[x].size;
						data_size+=pRs[x].size;
					}
				}
				if (bString)
				{
					IPOD_STRING *is_last;
					is_last=il->iPodString.GetAt(il->iPodString.GetCount()-1);
					il->rsdata_end=is_last->offset+is_last->size;
					il->rsdata_start=i;
					m_iPodLang.Add(il);
				}
				i+=data_size;
				pWnd->SendMessage(WM_APP, 0, i);
				i=DWORD_ALIGN(i);
			}
		}
	}
	

	WORD firmware_version=(WORD)m_Buffer[266]; //0x10A
	if (firmware_version==2) //below 4g (not encrypted apple update image
	{
		// scan firmware for boot pictures
		pWnd->SendMessage(WM_APP, (WPARAM)TEXT("Scanning for boot pictures..."), 0);

		pic.SetPictureMode(1);
		for (j=0;j<m_NumPartitions;j++)
		{
			if (strcmp(m_pPartitions[j].id,"dpua")!=0)
				continue;

			startOffset=m_pPartitions[j].devOffset;
			endOffset=startOffset+m_pPartitions[j].len;
		
			DWORD r=0;
			for (i = startOffset; i < endOffset; i+= 4)
			{
				if (pic.Read(&m_Buffer[i], TRUE))
				{
					m_Pictures.Add(&m_Buffer[i]);
					m_PicturesType.Add(1);
					m_PicturesID.Add(r);
					r++;
					i += 4;
					if ((i % 100) == 0)
						pWnd->SendMessage(WM_APP, 0, i);
				}
			}
		}
	}
	
	// scan firmware for regular fonts

	pWnd->SendMessage(WM_APP, (WPARAM)TEXT("Scanning for fonts..."), 0);

	
	for (j=0;j<m_NumPartitions;j++)
	{
		if (strcmp(m_pPartitions[j].id,"soso")!=0)
			continue;

		startOffset=m_pPartitions[j].devOffset;
		endOffset=startOffset+m_pPartitions[j].len;
		LPBYTE lpEnd = m_Buffer + endOffset;

		for (i = startOffset; i < endOffset; i+= 4)
		{
			if (font.Read(&m_Buffer[i], lpEnd))
			{
				m_Fonts.Add(&m_Buffer[i]);
				/*
				if (m_Fonts.GetCount()==30)
				{
					CFile temp;
					temp.Open(TEXT("D:\\iTunes Music\\israPod Update\\res\\nano_font.bin"), CFile::modeCreate | CFile::modeWrite);
					temp.Write(&m_Buffer[i], font.GetFontBlockLen());
					temp.Close();
				}
				*/
				i += font.GetFontBlockLen() - 4;
				if ((i % 10) == 0)
					pWnd->SendMessage(WM_APP, 0, i);
			}
		}
	}

	//scan firmware for OTF fonts

	pWnd->SendMessage(WM_APP, (WPARAM)TEXT("Scanning for OpenType fonts..."), 0);

	for (j=0;j<m_NumPartitions;j++)
	{
		if (strcmp(m_pPartitions[j].id,"crsr")!=0)
			continue;

		startOffset=m_pPartitions[j].devOffset;
		endOffset=startOffset+m_pPartitions[j].len;
	
		for (i = startOffset; i < endOffset; i+= 4)
		{
			if (ofont.Read(&m_Buffer[i], FALSE))
			{
				m_OTFFonts.Add(&m_Buffer[i]);
				i += DWORD_ALIGN(ofont.GetFontBlockLen()) - 4;
				if ((i % 10) == 0)
					pWnd->SendMessage(WM_APP, 0, i);
			}
		}
	}

	// scan firmware for strings

	pWnd->SendMessage(WM_APP, (WPARAM)TEXT("Scanning for all strings..."), 0);

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
			if ((i % 1000) == 0)
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

DWORD CFirmware::GetPictureID(DWORD index)
{
	if (index >= (DWORD)m_PicturesID.GetCount())
		return FALSE;

	return (DWORD)m_PicturesID.GetAt(index);
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

DWORD CFirmware::GetNumOTFFonts()
{
	return (DWORD)m_OTFFonts.GetCount();
}

LPBYTE CFirmware::GetOTFFont(DWORD index)
{
	if (index >= (DWORD)m_OTFFonts.GetCount())
		return NULL;

	return (LPBYTE)m_OTFFonts.GetAt(index);
}

BOOL CFirmware::CanWriteOTFFont(DWORD index)
{
	DWORD offset = (DWORD)((LPBYTE)m_OTFFonts.GetAt(index) - m_Buffer);

	return CanWrite(offset, 1);
}

DWORD CFirmware::GetNumLangs()
{
	return (DWORD)m_iPodLang.GetCount();
}

DWORD CFirmware::GetNumLangStrings(DWORD index)
{
	if (index >= (DWORD)m_iPodLang.GetCount())
		return 0;

	IPOD_LANG *il;
	il=m_iPodLang.GetAt(index);
	return (DWORD)il->iPodString.GetCount();
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

LPCTSTR CFirmware::GetLangString(DWORD index, DWORD index2)
{
	if (index >= (DWORD)m_iPodLang.GetCount())
		return FALSE;

	IPOD_LANG *il;
	il=m_iPodLang.GetAt(index);

	if (index2 >= (DWORD)il->iPodString.GetCount())
		return FALSE;

	IPOD_STRING *is;
	is=il->iPodString.GetAt(index2);
	return (LPCTSTR)&m_Buffer[is->offset];
}

DWORD CFirmware::GetLangStringSize(DWORD index, DWORD index2)
{
	if (index >= (DWORD)m_iPodLang.GetCount())
		return 0;

	IPOD_LANG *il;
	il=m_iPodLang.GetAt(index);

	if (index2 >= (DWORD)il->iPodString.GetCount())
		return 0;

	IPOD_STRING *is;
	is=il->iPodString.GetAt(index2);
	return is->size;
}

void CFirmware::AddLangString(DWORD index, DWORD index2, LPBYTE str, DWORD size)
{
	if (index >= (DWORD)m_iPodLang.GetCount())
		return;

	IPOD_LANG *il;
	il=m_iPodLang.GetAt(index);

	if (index2 >= (DWORD)il->iPodString.GetCount())
		return;

	IPOD_STRING *is;
	is=il->iPodString.GetAt(index2);

	is->size=size+1;

	LPBYTE lpPos;
	if (index2+1 != (DWORD)il->iPodString.GetCount())
	{
		is->offset=m_SLangOffset;

		lpPos=&m_Buffer[is->offset];
		if (size>0)
			memcpy(lpPos, str, size);
		lpPos[size]=0;

		m_SLangOffset+=is->size;
	}
	else
	{
		is->offset=il->rsdata_end-is->size;
		lpPos=&m_Buffer[is->offset];
		if (size>0)
			memcpy(lpPos, str, size);
		lpPos[size]=0;
	}
	*is->string_off=is->offset-il->rsdata_start;
	*is->size_off=is->size;
}

BOOL CFirmware::ResetLangBlock(DWORD index, DWORD totallen)
{
	if (index >= (DWORD)m_iPodLang.GetCount())
		return FALSE;

	IPOD_LANG *il;
	il=m_iPodLang.GetAt(index);

	IPOD_STRING *is;
	is=il->iPodString.GetAt(0);
	
	DWORD start=is->offset;
	DWORD end=il->rsdata_end;

	if (totallen>end-start+1)
		return FALSE;

	for (DWORD i=start;i<end;i++)
		m_Buffer[i]=0;

	m_SLangOffset=is->offset;
	
	m_StringOrderFixed = TRUE;

	return TRUE;
}

BOOL CFirmware::SetLangString(DWORD index, DWORD index2, LPBYTE str, DWORD size)
{
	if (index >= (DWORD)m_iPodLang.GetCount())
		return FALSE;

	IPOD_LANG *il;
	il=m_iPodLang.GetAt(index);

	DWORD total=(DWORD)il->iPodString.GetCount();
	if (index2 >= total)
		return FALSE;

	DWORD i;

	//calc total space of strings
	DWORD sum=0;
	IPOD_STRING *is;
	is=il->iPodString.GetAt(0);
	DWORD datastart=is->offset;

	for (i=0;i<total;i++)
	{
		is=il->iPodString.GetAt(i);
		sum+=is->size;
	}
	
	is=il->iPodString.GetAt(index2);
	int change=(signed)(size+1-is->size);

	sum+=change;
	DWORD original_len=il->rsdata_end-datastart;
	if (sum>original_len) //check if after the future change there is enough space in language block
		return FALSE;

	//change string
	LPBYTE lpPos,lpBuf;
	if (m_StringOrderFixed)
	{
		if (index2+1!=total)
		{
			IPOD_STRING *ist;
			
			lpBuf=new BYTE[original_len];
			memcpy(lpBuf, &m_Buffer[datastart], original_len);
			for (i=total-2;i>index2;i--)
			{
				ist=il->iPodString.GetAt(i);
				if (ist->size>0)
					memcpy(&lpBuf[ist->offset-datastart+change], &m_Buffer[ist->offset], ist->size);
				ist->offset+=change;
				*ist->string_off=ist->offset-il->rsdata_start;
			}
			memcpy(&m_Buffer[datastart], lpBuf, original_len);
			delete[] lpBuf;

			lpPos=&m_Buffer[is->offset];
			is->size=size+1;
			if (size>0)
				memcpy(lpPos, str, size);
			lpPos[size]=0;

			*is->size_off=is->size;
		}
		else
		{
			is->offset-=change;

			is->size=size+1;

			lpPos=&m_Buffer[is->offset];
			if (size>0)
				memcpy(lpPos, str, size);
			lpPos[size]=0;

			*is->string_off=is->offset-il->rsdata_start;
			*is->size_off=is->size;
		}
	}
	else
	{
		//Trim 00 spaces so will have space for changes.
		lpBuf=new BYTE[il->rsdata_end-datastart];
		memcpy(lpBuf, &m_Buffer[datastart], original_len);
		
		if (ResetLangBlock(index, sum)==FALSE)
			return FALSE;
		
		for (i=0;i<total;i++)
		{
			is=il->iPodString.GetAt(i);
			if (i==index2)
				AddLangString(index, i, str, size);
			else
				AddLangString(index, i, &lpBuf[is->offset-datastart], is->size-1);
		}

		delete[] lpBuf;
		m_StringOrderFixed = TRUE;
	}
	return TRUE;
}

void CFirmware::SetData(DWORD offset, LPBYTE data, DWORD size)
{
	if (offset < m_BufferSize)
	{
		memcpy(&m_Buffer[offset], data, size);
	}
}

__int32 CFirmware::GetLangStringID(DWORD index, DWORD index2)
{
	if (index >= (DWORD)m_iPodLang.GetCount())
		return 0;

	IPOD_LANG *il;
	il=m_iPodLang.GetAt(index);

	if (index2 >= (DWORD)il->iPodString.GetCount())
		return 0;

	IPOD_STRING *is;
	is=il->iPodString.GetAt(index2);
	return is->id;
}