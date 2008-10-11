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
	m_PicturesID.clear();
	m_Fonts.RemoveAll();
	m_Strings.RemoveAll();
	m_OTFFonts.RemoveAll();

	for (INT_PTR i=0;i<m_iPodLang.GetCount();i++)
	{
		for (INT_PTR j=0;j<m_iPodLang.GetAt(i)->iPodString.GetCount();j++)
			if (m_iPodLang.GetAt(i)->iPodString.GetAt(j)!=NULL)
				delete m_iPodLang.GetAt(i)->iPodString.GetAt(j);
		if (m_iPodLang.GetAt(i)!=NULL)
			delete m_iPodLang.GetAt(i);
	}
	m_iPodLang.RemoveAll();

	for (size_t i=0;i<ViewRecList.size();i++)
	{
		for (size_t j=0;j<ViewRecList.at(i)->ViewElemList.size();j++)
			if (ViewRecList.at(i)->ViewElemList.at(j)!=NULL)
				delete ViewRecList.at(i)->ViewElemList.at(j);
		if (ViewRecList.at(i)!=NULL)
			delete ViewRecList.at(i);
	}
	ViewRecList.clear();
	for (size_t i=0;i<MenuRecList.size();i++)
	{
		for (size_t j=0;j<MenuRecList.at(i)->MenuElemList.size();j++)
			if (MenuRecList.at(i)->MenuElemList.at(j)!=NULL)
				delete MenuRecList.at(i)->MenuElemList.at(j);
		if (MenuRecList.at(i)!=NULL)
			delete MenuRecList.at(i);
	}
	MenuRecList.clear();
	for (size_t i=0;i<TypeRecList.size();i++)
		if (TypeRecList.at(i)!=NULL)
			delete TypeRecList.at(i);
	TypeRecList.clear();
	for (size_t i=0;i<TCMDRecList.size();i++)
	{
		//for (size_t j=0;j<TCMDRecList.at(i)->element.elements.size();j++)
		//	if (TCMDRecList.at(i)->element.elements.at(j)!=NULL)
		//		delete TCMDRecList.at(i)->element.elements.at(j);
		if (TCMDRecList.at(i)!=NULL)
			delete TCMDRecList.at(i);
	}
	TCMDRecList.clear();

	m_SLangOffset = 0;
	m_StringOrderFixed = FALSE;
}

DWORD CFirmware::GetFirmwareVersion()
{
	if (m_pPartitions)
		return m_pPartitions[0].version;
	else
		return -1;
}

// operations

BOOL CFirmware::SetFirmware(LPCTSTR name, LPBYTE buffer, DWORD size)
{
	// reset
	Reset();

	m_Buffer = new BYTE[size+524288];
	if (m_Buffer == NULL)
		return FALSE;

	//copy resource buffer to a buffer we can use
	memcpy(m_Buffer, buffer, size);

	m_BufferSize = size;

	m_Name = name;

	//char ata[4]={0x21,0x41,0x54,0x41};
	//char ata[4]={0x44,0x4E,0x41,0x4E};
	theApp.CalcPartMapAddr(m_Buffer);
	WORD firmware_version=(WORD)m_Buffer[266]; //0x10A
	m_pPartitions = (IPOD_PARTITION_HEADER *)&m_Buffer[theApp.PARTITION_MAP_ADDRESS];
	while (memcmp(&m_pPartitions[m_NumPartitions].type, &ata, 4) == 0 || memcmp(&m_pPartitions[m_NumPartitions].type, &nand, 4) == 0)
	{
		m_PartitionInfo[m_NumPartitions].checksumStartAddress = m_pPartitions[m_NumPartitions].devOffset;

		if (strncmp(m_pPartitions[m_NumPartitions].id, "soso", 4)==0 || strncmp(m_pPartitions[m_NumPartitions].id, "crsr", 4)==0)
		{
			//Known images so far:
			//soso: OS Code with initial resources such as pictures and bitmap fonts
			//crsr: Resources image, 5g and above which contain OpenType Bitmap fonts and video stuff
			m_PartitionInfo[m_NumPartitions].bCanModify = TRUE;

			// check checksum
			//if firmware version is 3 (4g and above models) then 512 bytes from devOffset are random and not included in checksum
			//elsewhere, if it's 2 then it's 1-3g
			if (firmware_version!=2) //it's either 3 or we don't know this firmware but heck let's try checksum anyway
				m_PartitionInfo[m_NumPartitions].checksumStartAddress -= theApp.BLOCK_SIZE * 2;

			//
			if (strncmp(m_pPartitions[m_NumPartitions].id, "soso", 4)==0)
			{
				DecryptAUPD(m_NumPartitions);
			}
			//

			if (m_pPartitions[m_NumPartitions].checksum != ComputeChecksum(m_NumPartitions))
			{
				//checksum went wrong, meaning don't edit this image
				//m_PartitionInfo[m_NumPartitions].checksumStartAddress = m_pPartitions[m_NumPartitions].devOffset;
				m_PartitionInfo[m_NumPartitions].bCanModify = FALSE;
			}
		}
		else if (strncmp(m_pPartitions[m_NumPartitions].id, "dpua", 4)==0)
		{
			//do not let user edit software flash update, it's risky and no resources or strings are there
			//m_PartitionInfo[m_NumPartitions].bCanModify = FALSE;

			//enable decryption
			if (firmware_version!=2) //it's either 3 or we don't know this firmware but heck let's try checksum anyway
			{
				//theApp.BLOCK_SIZE=512;
				m_PartitionInfo[m_NumPartitions].checksumStartAddress += theApp.BLOCK_SIZE;
				m_PartitionInfo[m_NumPartitions].bCanModify = TRUE;
				DecryptAUPD(m_NumPartitions);
			}
		}
		else if (strncmp(m_pPartitions[m_NumPartitions].id, "ebih", 4)==0)
		{
			//LPBYTE temp=m_Buffer+m_pPartitions[m_NumPartitions].devOffset;
		}
		else
		{
			//we don't know what it is so we better not write it
			m_PartitionInfo[m_NumPartitions].bCanModify = FALSE;
		}
		//LPBYTE teemp=m_Buffer+m_pPartitions[m_NumPartitions].devOffset;

		m_NumPartitions++;
	}

	return TRUE;
}

void CFirmware::ResetImageAdresses()
{
	int i;
	/*DWORD savedAddr=0;

	for (i = 0; i < m_NumPartitions; i++)
	{
		if (!strncmp(m_pPartitions[i].id, "soso", 4))
			savedAddr=m_pPartitions[i].loadAddr;
	}*/

	//if (savedAddr!=0)
	//{
		for (i = 0; i < m_NumPartitions; i++)
		{
			if (!strncmp(m_pPartitions[i].id, "dpua", 4) && m_PartitionInfo[i].bCanModify)
			{
				m_pPartitions[i].flash=0;
				break;
			}
		}
	//}
}

void CFirmware::DecryptAUPD(int partition)
{
	LPBYTE offset_start;
	offset_start=m_Buffer+m_pPartitions[partition].devOffset;
	SecurityBlock sb(offset_start);
	sb.GetSecurityBlockKey(offset_start);

	if (sb.fileIsProtected==true)
	{
		DWORD size=m_pPartitions[partition].len;
		//size-=theApp.BLOCK_SIZE;
		//unsigned char *decrypted_data=new unsigned char[size];
		//memset(decrypted_data, 0, size);
		unsigned char *decrypted_data;

		FileDecrypt fd;
		sb.key=SWAPLONG_L((unsigned long)sb.key);
		decrypted_data=fd.decryptFirmwareFile(&offset_start[theApp.BLOCK_SIZE], sb.key, size);

		//memcpy(&offset_start[theApp.BLOCK_SIZE], decrypted_data, size);
	}
}

void CFirmware::FixChecksum(DWORD imageID)
{
	if (!strncmp(m_pPartitions[imageID].id, "dpua", 4))
		return;

	if (m_PartitionInfo[imageID].bCanModify==TRUE)
		//no need to fix
		return;

	m_pPartitions[imageID].checksum=ComputeChecksum(imageID);
}

//VOLUME HACK

int CFirmware::isCapped(void)
{
	//We search for this byte combination in firmware and after it two bytes decide either cap or not
	BYTE temp[8]={0x8,0x0,0x9F,0xE5,0x38,0x1,0x90,0xE5}; //3/4G + Color + mini
	BYTE temp2[8]={0x8,0x0,0x9F,0xE5,0x94,0x0,0x90,0xE5}; //nano + 5G
	for (byte_cap_pos = 0; byte_cap_pos < (theApp.BLOCK_SIZE * 2000) - 2; byte_cap_pos++)
	{
	    if (memcmp(&m_Buffer[byte_cap_pos], &temp, 8)==0 || memcmp(&m_Buffer[byte_cap_pos], &temp2, 8)==0)
		{
			//if (byte_cap_pos > theApp.BLOCK_SIZE - 3)
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

	// scan firmware for regular fonts

	pWnd->SendMessage(WM_APP, (WPARAM)TEXT("Scanning for fonts..."), 0);

	
	for (j=0;j<m_NumPartitions;j++)
	{
		if (strncmp(m_pPartitions[j].id,"soso",4)!=0 && strncmp(m_pPartitions[j].id,"crsr",4)!=0)
			continue;

		startOffset=m_pPartitions[j].devOffset;
		endOffset=startOffset+m_pPartitions[j].len;
		LPBYTE lpEnd = m_Buffer + endOffset;

		for (i = startOffset; i < endOffset; i+= 4)
		{
			if (font.Read(&m_Buffer[i], lpEnd))
			{
				m_Fonts.Add(&m_Buffer[i]);
				
				/*if (m_Fonts.GetCount()==39)
				{
					CFile temp;
					temp.Open(TEXT("D:\\iTunes Music\\israPod Update\\res\\color_font.bin"), CFile::modeCreate | CFile::modeWrite);
					temp.Write(&m_Buffer[i], font.GetFontBlockLen());
					temp.Close();
				}*/
				
				i += font.GetFontBlockLen() - 4;
				if ((i % 10) == 0)
					pWnd->SendMessage(WM_APP, 0, i);
			}
		}
	}

	pWnd->SendMessage(WM_APP, (WPARAM)TEXT("Scanning for pictures and strings..."), 0);
	
	for (j=0;j<m_NumPartitions;j++)
	{
		if (strncmp(m_pPartitions[j].id,"soso",4)!=0) //this sources will be only in OS image that's why we can skip others
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

				//########################################################
				/*
				DWORD pic_section=-1;
				for (k=0;k<pRsHeader->num_sections;k++)
				{
					if (!strncmp(pRsSection[k].type, "paMB", 4))
						pic_section=k;
				}
				if (pic_section!=-1)
				{
					int fntid=1;
					DWORD ppindex=pRsSection[pic_section].num_items-1;
					ppindex=140;
					pRs=(RESOURCE *)&m_Buffer[rs_offset+pRsSection[pic_section].start];

					LPBYTE lpEnd=m_Buffer+m_BufferSize;
					DWORD change=0;
					LPBYTE target;
					
					pRs[ppindex].offset=(DWORD)((LPBYTE)m_Fonts.GetAt(fntid)-m_Buffer)-i;

					
					//DWORD im_size=0;
					
					//create image buffer:
					DWORD im_width=320;
					DWORD im_height=240;
					DWORD im_size=sizeof(IPOD_PICTURE_HEADER2)+im_width*im_height*16/8;
					pRs[ppindex].size=im_size;
					LPBYTE image_buff=new BYTE[im_size];
					IPOD_PICTURE_HEADER2 *im_header=(IPOD_PICTURE_HEADER2 *)image_buff;
					memset(im_header, 0, im_size);
					im_header->texture_format=0x2565;
					im_header->d5=0;
					im_header->bytes_per_row=im_width*16/8;
					im_header->bitDepth=16;
					im_header->height=im_height;
					im_header->width=im_width;
					im_header->blockLen=im_width*im_height*16/8;

					change=im_size;
					target=(LPBYTE)m_Fonts.GetAt(fntid);
					memcpy(target, image_buff, im_size);
					//m_BufferSize+=change;
					//lpEnd=m_Buffer+m_BufferSize;
				}
				*/
				//########################################################
				

				for (k=0;k<pRsHeader->num_sections;k++)
				{
					pRs=(RESOURCE *)&m_Buffer[rs_offset+pRsSection[k].start];
					//TRACE(_T("Section %d, type %S\n"), k, pRsSection[k].type);
					for (x=0;x<pRsSection[k].num_items;x++)
					{
						//if (lastend && pRs[x].offset != lastend)
							//	warning
						//{
						//	i+=lastend-pRs[x].offset;
							//continue;
						//}
						//lastend = DWORD_ALIGN(pRs[x].offset+pRs[x].size);
						//TRACE(_T("ID %d, offset %d, size %d\n"), pRs[x].id, pRs[x].offset, pRs[x].size);
						int *num_elem;
						int q;
						struct resource_type *res_type;
						
						if (!strncmp(pRsSection[k].type, "paMB", 4)) //pictures
						{
							if (pic.Read(&m_Buffer[i+pRs[x].offset], TRUE))
							{
								//add data pointer for picture and picture type (0=changable)
								m_Pictures.Add(&m_Buffer[i+pRs[x].offset]);
								m_PicturesType.Add(0);
								m_PicturesID.push_back(pRs[x].id);
								//if (m_PicturesType.GetCount()==264)
								//{
								//	TCHAR bb[10];
								//	wsprintf(bb, _T("%d"), i+pRs[x].offset);
									//MessageBox(0, bb, _T("a"), 0);
								//}
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
								m_PicturesID.push_back(pRs[x].offset);
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
							struct resource_type_view_elem *view;
							LPBYTE lpViewPos=&m_Buffer[i+pRs[x].offset];
							ViewElem *view_elem;
							ViewRecord *view_item;

							num_elem=(int *)lpViewPos;
							lpViewPos+=4;
							//if (*num_elem > MAX_ELEMENTS)
							//	error("Too many elements (%d) in %s", num_elem, file);


							view_item=new ViewRecord;
							view_item->num_elems=*num_elem;
							view_item->id=pRs[x].id;

							for (q = 0; q < *num_elem; q++)
							{
								view=(resource_type_view_elem *)lpViewPos;

								//res_type = get_resource_type(view->type);

								// some have extra fields, different number doesn't seem to be any
								// consistent pattern*/

								//lpViewPos+=sizeof(resource_type_view_elem);
								lpViewPos+=view->size+4;

								// +4 for the length of the size field which isn't included in the size
								//for (q = 0; q < (int)(view->size+4-sizeof(view))/4; q++)
								//{
								//	t1=(unsigned int *)lpViewPos;
								//	lpViewPos+=4;
								//	//add_property(props, item, HEX, t1, NULL);
								//}
								
								view_elem=new ViewElem;
								view_elem->next=NULL;
								view_elem->element=view;
								view_item->ViewElemList.push_back(view_elem);
							}
							ViewRecList.push_back(view_item);
						}
						else if (!strncmp(pRsSection[k].type, "uneM", 4)) //layout
						{
							struct resource_type_menu_elem *menu;
							LPBYTE lpMenuPos=&m_Buffer[i+pRs[x].offset];
							MenuElem *menu_elem;
							MenuRecord *menu_item;

							num_elem=(int *)lpMenuPos;
							lpMenuPos+=4;
							//if (*num_elem > MAX_ELEMENTS)
							//	error("Too many elements (%d) in %s", num_elem, file);


							menu_item=new MenuRecord;
							menu_item->num_elems=*num_elem;
							menu_item->id=pRs[x].id;

							for (q = 0; q < *num_elem; q++)
							{
								menu=(resource_type_menu_elem *)lpMenuPos;

								//res_type = get_resource_type(view->type);

								// some have extra fields, different number doesn't seem to be any
								// consistent pattern*/

								//lpViewPos+=sizeof(resource_type_view_elem);
								lpMenuPos+=menu->size+4;

								// +4 for the length of the size field which isn't included in the size
								//for (q = 0; q < (int)(view->size+4-sizeof(view))/4; q++)
								//{
								//	t1=(unsigned int *)lpViewPos;
								//	lpViewPos+=4;
								//	//add_property(props, item, HEX, t1, NULL);
								//}
								
								menu_elem=new MenuElem;
								//menu_elem->next=NULL;
								menu_elem->element=menu;
								menu_item->MenuElemList.push_back(menu_elem);
							}
							MenuRecList.push_back(menu_item);
						}
						else if (!strncmp(pRsSection[k].type, "epyT", 4)) //layout
						{
							struct resource_type_type_elem *type;
							LPBYTE lpTypePos=&m_Buffer[i+pRs[x].offset];
							TypeRecord *type_item;

							type_item=new TypeRecord;
							type_item->id=pRs[x].id;

							type=(resource_type_type_elem *)lpTypePos;

							lpTypePos+=type->size+4;

							type_item->element=type;
								
							TypeRecList.push_back(type_item);
						}
						else if (!strncmp(pRsSection[k].type, "dmCT", 4)) //layout
						{
							struct resource_type_tcmd tcmd;
							LPBYTE lpTCMDPos=&m_Buffer[i+pRs[x].offset];
							TCMDRecord *tcmd_item;

							tcmd_item=new TCMDRecord;
							tcmd_item->id=pRs[x].id;

							tcmd.size_unk=(unsigned int *)lpTCMDPos;
							lpTCMDPos+=4;
							tcmd.z1=(unsigned short *)lpTCMDPos;
							if (*tcmd.z1==0)
							{
								lpTCMDPos+=2;
								tcmd.header_size=(unsigned short *)lpTCMDPos;
								lpTCMDPos+=2;
								tcmd.elems=(int *)lpTCMDPos;
								lpTCMDPos+=(*tcmd.header_size)*4;
								tcmd.num_offsets=(unsigned short *)lpTCMDPos;
								lpTCMDPos+=2;
								tcmd.t_len=(unsigned short *)lpTCMDPos;
								lpTCMDPos+=2;
								tcmd.tlist=(unsigned short *)lpTCMDPos;
								lpTCMDPos+=(*tcmd.t_len)*2;
								if ((*tcmd.t_len)%2!=0)
									lpTCMDPos+=2;
								tcmd.offsets=(unsigned int *)lpTCMDPos;
								lpTCMDPos+=(*tcmd.num_offsets)*4;
								lpTCMDPos+=4; //add last offset (not used but for aligning)
								LPBYTE save;
								for (unsigned short tidx=0;tidx<(*tcmd.num_offsets);tidx++)
								{
									save=lpTCMDPos+tcmd.offsets[tidx];
									tcmd.elements.push_back((resource_type_tcmd_elem *)save);
								}

								tcmd_item->element=tcmd;
								
								TCMDRecList.push_back(tcmd_item);
							}
							else
							{

							}
						}
						else if (!strncmp(pRsSection[k].type, "mTDL", 4)) //layout
						{
							//not supported
							//looks boring
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

	//if (m_BufferSize%theApp.BLOCK_SIZE!=0)
	//	m_BufferSize=(m_BufferSize/theApp.BLOCK_SIZE)*theApp.BLOCK_SIZE+theApp.BLOCK_SIZE;
	
	//Link connections between view elements:
	DWORD k;
	bool foundid;
	for (i=0;i<ViewRecList.size();i++)
	{
		for (j=0;j<ViewRecList[i]->num_elems;j++)
		{
			foundid=false;
			for (k=0;k<ViewRecList.size();k++)
				if (ViewRecList[k]->id == ViewRecList[i]->ViewElemList[j]->element->id)
				{
					ViewRecList[i]->ViewElemList[j]->next=ViewRecList[k];
					foundid=true;
				}
			if (foundid==false)
				ViewRecList[i]->ViewElemList[j]->next=NULL;
		}
	}

	WORD firmware_version=(WORD)m_Buffer[266]; //0x10A
	//if (firmware_version==2) //below 4g (not encrypted apple update image
	//{
	
		// scan firmware for boot pictures
		pWnd->SendMessage(WM_APP, (WPARAM)TEXT("Scanning for boot pictures..."), 0);

		pic.SetPictureMode(1);
		for (j=0;j<m_NumPartitions;j++)
		{
			if (strncmp(m_pPartitions[j].id,"dpua", 4))
				continue;

			startOffset=m_pPartitions[j].devOffset;
			endOffset=startOffset+m_pPartitions[j].len;
			LPBYTE lpImageOffset=m_Buffer+startOffset+theApp.BLOCK_SIZE;
		
			DWORD r=0;
			for (i = startOffset; i < endOffset; i+= 2)
			{
				if (pic.Read(&m_Buffer[i], TRUE, lpImageOffset))
				{
					m_Pictures.Add(&m_Buffer[i]);
					m_PicturesType.Add(1);
					m_PicturesID.push_back(r);
					r++;
					i += 4;
					if ((i % 100) == 0)
						pWnd->SendMessage(WM_APP, 0, i);
				}
			}
		}
	//}

	//scan firmware for OTF fonts

	pWnd->SendMessage(WM_APP, (WPARAM)TEXT("Scanning for OpenType fonts..."), 0);

	for (j=0;j<m_NumPartitions;j++)
	{
		if (strncmp(m_pPartitions[j].id,"crsr",4)!=0)
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
	if (theApp.m_AllStringsRender)
		return;

	pWnd->SendMessage(WM_APP, (WPARAM)TEXT("Scanning for all strings..."), 0);

	BYTE c;
	BOOL bInString = FALSE;
	DWORD stringStart;

	for (j=0;j<m_NumPartitions;j++)
	{
		if (!strncmp(m_pPartitions[j].id,"ebih",4)!=0)
			continue;

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

LPBYTE CFirmware::GetAUPDBuffer()
{
	int i;
	for (i = 0; i < m_NumPartitions; i++)
	{
		if (!strncmp(m_pPartitions[i].id, "dpua", 4))
			return m_Buffer+m_pPartitions[i].devOffset+theApp.BLOCK_SIZE;
	}
	return NULL;
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
		if (!strncmp(m_pPartitions[i].id, "dpua", 4))
			DecryptAUPD(i);
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
	if (index >= (DWORD)m_PicturesID.size())
		return FALSE;

	return m_PicturesID[index];
}

DWORD CFirmware::GetPictureIndexFromID(DWORD id)
{
	for (int i=0;i<m_PicturesID.size();i++)
	{
		if (id==m_PicturesID[i])
			return (WORD)i;
	}
	return 0xFFFFFFFF;
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

ViewTable *CFirmware::GetViewList()
{
	if (ViewRecList.size()>0)
	{
		return &ViewRecList;
	}
	else
		return NULL;
}

MenuTable *CFirmware::GetMenuList()
{
	if (MenuRecList.size()>0)
	{
		return &MenuRecList;
	}
	else
		return NULL;
}

TypeTable *CFirmware::GetTypeList()
{
	if (TypeRecList.size()>0)
	{
		return &TypeRecList;
	}
	else
		return NULL;
}

TCMDTable *CFirmware::GetTCMDList()
{
	if (TCMDRecList.size()>0)
	{
		return &TCMDRecList;
	}
	else
		return NULL;
}

bool CFirmware::FindResourceID(int i, int id)
{
	bool found=false;
	int j;
	switch(i)
	{
	case 0: //view
		for (j = 0; j < ViewRecList.size(); j++)
		{
			if (ViewRecList.at(j)->id==id)
			{
				found=true;
				break;
			}
		}
		break;
	case 1: //menu
		for (j = 0; j < MenuRecList.size(); j++)
		{
			if (MenuRecList.at(j)->id==id)
			{
				found=true;
				break;
			}
		}
		break;
	case 2: //type
		for (j = 0; j < TypeRecList.size(); j++)
		{
			if (TypeRecList.at(j)->id==id)
			{
				found=true;
				break;
			}
		}
		break;
	case 3: //tcmd
		for (j = 0; j < TCMDRecList.at(0)->element.elements.size(); j++)
		{
			if (TCMDRecList.at(0)->element.elements.at(j)->id==id)
			{
				found=true;
				break;
			}
		}
		break;
	}
	return found;
}

void SecurityBlock::GetSecurityBlockKey(unsigned char *rawData){
	int constant = 0x54c3a298;
	int key=0;
	data=rawData;
	int aMarker=0;
	int pos=0;
	for (int c=0;c<8;c++){
		pos =offset[c]*4;
		aMarker=readWord(rawData,pos);
		bool result=testMarker(aMarker);
		//System.out.println("Marker ="+Integer.toHexString(aMarker)+" "+result);
		if (result)
		{ // This marker is enable
			fileIsProtected=true;
			// pos of nextblock
			if (c<7)
				pos =(offset[c+1]*4)+4;
			else
				pos =(offset[0]*4)+4;
			key=0;
			int temp1=aMarker;
			for (int count=0;count<2;count++)
			{
				int word=readWord(data,pos);
				temp1=aMarker;
				temp1=temp1^word;
				temp1=temp1^constant;
				key=temp1;
				pos=pos+4;
			}
			int r1=0x6f;
			int r2=0;
			int r12;
			int r14;
			unsigned int r_tmp;
			for (int count=2;count<theApp.BLOCK_SIZE/4;count=count+2)
			{
				r2=readWord(data,count*4);
				r12=readWord(data,(count*4)+4);
				r_tmp=(unsigned int)r12>>16;
				r14=r2 | ((int)r_tmp);
				r2=r2&0xffff;
				r2=r2 | r12;
				r1=r1^r14;
				r1=r1+r2;
			}
			key=key^r1;
			// Invert key, little endian
			r_tmp=((unsigned int)(key&0xff000000))>>24;
			unsigned int r_tmp2=((unsigned int)(key&0xff0000))>>8;
			this->key = ((key&0xff)<<24)|((key&0xff00)<<8)|(r_tmp2)|(r_tmp);
		}
	}
}

int SecurityBlock::readWord(unsigned char *buffer,int pos){
	int p1=buffer[pos];
	int p2=buffer[pos+1];
	int p3=buffer[pos+2];
	int p4=buffer[pos+3];
	if (p1<0)p1=p1+256;
	if (p2<0)p2=p2+256;
	if (p3<0)p3=p3+256;
	if (p4<0)p4=p4+256;
	return p1+(p2<<8)+(p3<<16)+(p4<<24);
}

bool SecurityBlock::testMarker(int marker){
	int mask = (marker&0xff)|((marker&0xff)<<8)|((marker&0xff)<<16)|((marker&0xff)<<24);
	int decrypt = marker ^ mask;
	int temp1=(int)((unsigned int)decrypt>>24);
	int temp2=decrypt<<8;
	if (temp1==0) return false;
	temp2=(int)((unsigned int)temp2>>24);
	decrypt=decrypt<<16;
	decrypt=(int)((unsigned int)decrypt>>24);
	if ((temp1<temp2)&&(temp2<decrypt)){
		temp1=temp1&0xf;
		temp2=temp2&0xf;
		decrypt=decrypt&0xf;
		if ((temp1>temp2)&&(temp2>decrypt)){
			if (decrypt!=0) return true; // This marker is enabled!
		}
	}
	return false;
}

SecurityBlock::SecurityBlock(unsigned char *rawData)
{
	fileIsProtected=false;
}


unsigned char *FileDecrypt::decryptFirmwareFile(unsigned char *fileData, int key, unsigned long data_len){
		unsigned char *data=NULL;
		cipherBox=new int [256];
		cipherKeyArray=new int [256];
		//try{
		unsigned char* theKey = (unsigned char*)malloc(sizeof(key));
		memset(theKey, 0, sizeof(key));
		memcpy(theKey, &key, sizeof(key));
		doRC4keyExpansion(theKey);
		data= doRC4Decrypt(fileData, data_len);
		//}catch(e){
		//e.printStackTrace();
		//}
		return data;
	}

void FileDecrypt::doRC4keyExpansion(unsigned char *thisKey)
	{
		int keyLength =0;
		int dataSwap;
		int b;
		int keyByte;
		keyLength = 4;

		for(int a = 0; a < 256; a++)
		{
			keyByte = thisKey[a % keyLength];
			if (keyByte<0)keyByte=keyByte+256;
			cipherKeyArray[a] = keyByte;
			cipherBox[a]=a;
		}
		b = 0;
		for(int a = 0; a < 256; a++)
		{
			b = (b + cipherBox[a] + cipherKeyArray[a]) & 255;
			dataSwap = cipherBox[a];
			cipherBox[a] = cipherBox[b];
			cipherBox[b] = dataSwap;
		}
	}

unsigned char *FileDecrypt::doRC4Decrypt(unsigned char *rawData, unsigned long data_len){
		int z = 0;
		int t = 0;
		int i = 0;
		int tempInt=0;
		byte cipherByte;
		int cipherBy = 0;
		for(int a = 0; a < data_len; a++)
		{
			i = (i + 1) & 255;
			t = (t + cipherBox[i]) & 255;
			tempInt = cipherBox[i];
			cipherBox[i] = cipherBox[t];
			cipherBox[t]= tempInt;
			z = cipherBox[(cipherBox[i] + cipherBox[t]) & 255];
			cipherByte = rawData[a];
			cipherBy = (int) cipherByte ^z;
			rawData[a]=(byte)cipherBy;
		}
		return rawData;
	}