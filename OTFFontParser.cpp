#include "StdAfx.h"
#include <atlconv.h>
#include ".\OTFFontParser.h"

COTFFont::COTFFont(void):
	m_pHeader(NULL),
	m_pName(NULL),
	m_pEBLC(NULL),
	m_pCMap(NULL),
	m_pEBDT(NULL),
	m_BigLen(0),
	m_EBDTLen(0)
{
}

COTFFont::~COTFFont(void)
{
}

// operations

ULONG	CalcTableChecksum(ULONG *Table, ULONG Length)
{
	ULONG Sum = 0L;
	ULONG *EndPtr = Table+((Length+3) & ~3) / sizeof(ULONG);

	while (Table < EndPtr)
	{
		Sum += SWAPLONG(*Table);
		Table++;
	}
	return Sum;
}


void COTFFont::SyncChecksums()
{
	ULONG *lpPos;

	//CMap checksum calculation:
	lpPos=(ULONG *)m_pHeader;
	lpPos+=m_CMapChecksum->offset/sizeof(ULONG);
	m_CMapChecksum->checkSum=CalcTableChecksum(lpPos, m_CMapChecksum->length);

	//EBDT checksum calculation:
	lpPos=(ULONG *)m_pHeader;
	lpPos+=m_EBDTChecksum->offset/sizeof(ULONG);
	m_EBDTChecksum->checkSum=CalcTableChecksum(lpPos, m_EBDTChecksum->length);

	//EBLC checksum calculation:
	lpPos=(ULONG *)m_pHeader;
	lpPos+=m_EBLCChecksum->offset/sizeof(ULONG);
	m_EBLCChecksum->checkSum=CalcTableChecksum(lpPos, m_EBLCChecksum->length);
}

BOOL COTFFont::Read(LPBYTE lpPos, BOOL bParse)
{
	if (lpPos == NULL)
		return FALSE;

	LPBYTE lpStart=lpPos;

	m_pHeader = (TABLES_HEADER *)lpPos;

	//reset
	m_pName = NULL;
	m_pEBLC = NULL;
	m_pCMap = NULL;
	m_pEBDT = NULL;
	m_BigLen = 0;
	m_EBDTLen = 0;
	//

	//BE_DWORD a;
	//a=(BE_DWORD *)lpPos;
	//DWORD temp=a;
	//if (a!=5)
	//	m_BigLen=0;

	if (m_pHeader->uMajorVersion != 1 || m_pHeader->uMinorVersion != 0)
		return FALSE;


	//validate header
	if (m_pHeader->numTables != 16 || m_pHeader->searchRange!=256 || m_pHeader->entrySelector!=4 || m_pHeader->rangeShift!=0)
		return FALSE;

	lpPos+=sizeof(TABLES_HEADER);

	DWORD i;
	TABLE_DIRECTORY *			m_pTable;
	m_BigLen=sizeof(TABLES_HEADER)+sizeof(TABLE_DIRECTORY)*m_pHeader->numTables;
	//Find the offsets for each table
	for (i=0;i<m_pHeader->numTables;i++)
	{
		m_pTable=(TABLE_DIRECTORY *)lpPos;
		if (!strncmp(m_pTable->name, "cmap", 4))
		{
			m_pCMap = (CMAP_HEADER *)(&lpStart[m_pTable->offset]);
			m_CMapChecksum=m_pTable;
		}
		else if (!strncmp(m_pTable->name, "EBDT", 4))
		{
			m_pEBDT = (EBDT_HEADER *)(&lpStart[m_pTable->offset]);
			m_EBDTLen = m_pTable->length-4; //DON'T FORGER THE -1!!!! IT WILL RUIN YOUR IPOD IF YOU WILL
			m_EBDTChecksum=m_pTable;
		}
		else if (!strncmp(m_pTable->name, "EBLC", 4))
		{
			m_pEBLC = (EBLC_HEADER *)(&lpStart[m_pTable->offset]);
			m_EBLCChecksum=m_pTable;
		}
		else if (!strncmp(m_pTable->name, "name", 4))
		{
			m_pName = (NAME_HEADER *)(&lpStart[m_pTable->offset]);
		}
		m_BigLen+=m_pTable->length;
		lpPos+=sizeof(TABLE_DIRECTORY);
	}

	if (m_pCMap==NULL || m_pName==NULL || m_pEBDT == NULL || m_pEBLC==NULL)
		return FALSE;

	//Parse the font tables in the following order:
	if (bParse==TRUE)
	{
		if (ParseName()==FALSE)
			return FALSE;
		if (ParseCMap()==FALSE)
			return FALSE;
		if (ParseEBDT()==FALSE)
			return FALSE;
		if (ParseEBLC()==FALSE)
			return FALSE;
	}

	lpStart+=m_BigLen;
	lpPos=lpStart;
	while (SWAPLONG(*((DWORD *)lpPos))!=0x12121212 && lpPos-lpStart<60)
	{
		lpPos++;
		m_BigLen++;
	}
	m_BigLen+=6;

	return TRUE;
}

DWORD COTFFont::GetFontBlockLen()
{
	if (m_pHeader == NULL)
		return 0;

	return m_BigLen;
}

WORD COTFFont::GetFontSize()
{
	if (m_pHeader == NULL)
		return 0;

	return m_FontSize;
}

WORD COTFFont::GetFontBitDepth()
{
	if (m_pHeader == NULL)
		return 0;

	return m_BitDepth;
}

LPCTSTR	COTFFont::GetFontName()
{
	if (m_pHeader==NULL)
		return NULL;

	wchar_t* lpwStr;
	LPTSTR lpStr = m_FontName.GetBuffer( m_FontName.GetLength() );
	int nLen = MultiByteToWideChar(CP_ACP, 0,(LPCSTR)lpStr, -1, NULL, NULL);
	lpwStr = ( wchar_t* )malloc(( nLen )*sizeof( wchar_t ));
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)lpStr, -1, lpwStr, nLen);
	return (LPTSTR)lpwStr;
}

LPCTSTR	COTFFont::GetFontStyle()
{
	if (m_pHeader==NULL)
		return NULL;

	wchar_t* lpwStr;
	LPTSTR lpStr = m_FontStyle.GetBuffer( m_FontStyle.GetLength() );
	int nLen = MultiByteToWideChar(CP_ACP, 0,(LPCSTR)lpStr, -1, NULL, NULL);
	lpwStr = ( wchar_t* )malloc(( nLen )*sizeof( wchar_t ));
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)lpStr, -1, lpwStr, nLen);
	return (LPTSTR)lpwStr;
}

BOOL COTFFont::ParseName()
{
	if (m_pName == NULL)
		return FALSE;

	LPBYTE lpPos=(LPBYTE)m_pName;
	LPBYTE lpTemp=lpPos;

	if (m_pName->format!=0)
		return FALSE;

	lpPos+=sizeof(NAME_HEADER);
	for (DWORD i=0;i<m_pName->count;i++)
	{
		NameRecord *nrd;
		nrd=(NameRecord *)lpPos;
		if (nrd->platformID == 1 && nrd->encodingID == 0)
		//macinttosh font (let's get this and not windows because the windows name is in UTF (Bleh))
		//nameid 6 = postscript name
		{
			char *strname = new char[nrd->length+1];
			strncpy(strname, (char*)(&lpTemp[m_pName->stringOffset+nrd->offset]), nrd->length);
			strname[nrd->length]='\0';
			if (nrd->nameID == 1)
				m_FontName.Format(TEXT("%s"), strname);
			else if (nrd->nameID == 2)
				m_FontStyle.Format(TEXT("%s"), strname);
		}
		lpPos+=sizeof(NameRecord);
	}
	return TRUE;
}

BOOL COTFFont::ParseCMap()
{
	LPBYTE lpPos=(LPBYTE)m_pCMap;
	LPBYTE lpStart=lpPos;

	if (m_pCMap->version!=0) //we only know version 0
		return FALSE;

	lpPos+=sizeof(CMAP_HEADER);

	DWORD i;
	LPBYTE lpTemp;
	ENCODING_RECORD	*m_pEncodeRec;
	//ENC_FORMAT0		*m_pEncFormat0;
	ENC_FORMAT4		*m_pEncFormat4;
	m_UnicodeGroups.RemoveAll();
	m_UnicodeGlyphs.RemoveAll();
	for (i=0;i<m_pCMap->numTables;i++)
	{
		m_pEncodeRec=(ENCODING_RECORD *)lpPos;
		lpTemp=lpPos;
		//lpPos+=sizeof(USHORT);
		//USHORT length=*((USHORT*)lpPos);
		//lpPos+=length;
		if ((m_pEncodeRec->encodingID==1 && m_pEncodeRec->platformID==3) || (m_pEncodeRec->encodingID==0 && m_pEncodeRec->platformID==1)) //microsoft unicode encoding
		{
			lpPos=&lpStart[m_pEncodeRec->offset];

			BE_US format=*((USHORT*)lpPos);
			USHORT j;
			if (format==4) //unicode ranges
			{
				m_pEncFormat4=(ENC_FORMAT4 *)lpPos;
				lpPos+=sizeof(ENC_FORMAT4);
				USHORT segCount=m_pEncFormat4->segCount/2;
				
				LPBYTE lpRS,lpRE,lpDA,lpRO;

				//get endcount array
				lpRE=lpPos;
				USHORT *endCount=new USHORT[sizeof(USHORT)*segCount];
				for (j=0;j<segCount;j++)
				{
					endCount[j]=SWAPWORD(*((USHORT *)lpPos));
					lpPos+=sizeof(USHORT);
				}
				lpPos+=sizeof(USHORT);  //reserved pad = 0
				//get startcount array
				lpRS=lpPos;
				USHORT *startCount=new USHORT[sizeof(USHORT)*segCount];
				for (j=0;j<segCount;j++)
				{
					startCount[j]=SWAPWORD(*((USHORT *)lpPos));
					lpPos+=sizeof(USHORT);
				}
				//get iddelta array
				lpDA=lpPos;
				SHORT *idDelta=new SHORT[sizeof(SHORT)*segCount];
				for (j=0;j<segCount;j++)
				{
					idDelta[j]=SWAPWORD(*((SHORT *)lpPos));
					lpPos+=sizeof(SHORT);
				}
				//get rangeoffset array
				lpRO=lpPos;
				USHORT *idRangeOffset=new USHORT[sizeof(USHORT)*segCount];
				for (j=0;j<segCount;j++)
				{
					idRangeOffset[j]=SWAPWORD(*((USHORT *)lpPos));
					lpPos+=sizeof(USHORT);
				}

				//*(idRangeOffset[i]/2 + (c - startCount[i]) + &idRangeOffset[i])
				USHORT leng=m_pEncFormat4->length;
				leng=leng-8*sizeof(USHORT)-4*segCount*sizeof(USHORT);
				USHORT *glyphs=new USHORT[sizeof(USHORT)*leng];
				//get glyphs
				for (j=0;j<leng/2;j++)
				{
					glyphs[j]=SWAPWORD(*((USHORT *)lpPos));
					lpPos+=sizeof(USHORT);
				}

				//file up groups
				for (j=0;j<segCount;j++) //we don't need the 0xffff group
				{
					UNICODE_GROUP *ugp=new UNICODE_GROUP;
					ugp->format=4;
					ugp->byteoffset_s=(USHORT *)(lpRS+sizeof(USHORT)*j);
					ugp->byteoffset_e=(USHORT *)(lpRE+sizeof(USHORT)*j);
					ugp->byteoffset_d=(SHORT *)(lpDA+sizeof(SHORT)*j);
					ugp->byteoffset_r=(USHORT *)(lpRO+sizeof(USHORT)*j);
					ugp->startOffset=startCount[j];
					ugp->endOffset=endCount[j];
					ugp->glyphIndex.RemoveAll();
					USHORT k=0;
					if ( idRangeOffset[j]==0 && endCount[j]==0xffff )
						/*done*/;
					else if (idRangeOffset[j]==0)
					{
						//one after one
						for (k=0;k<endCount[j]-startCount[j]+1;k++)
						{
							ugp->glyphIndex.Add(idDelta[j]+startCount[j]+k);
							m_UnicodeGlyphs.Add(idDelta[j]+startCount[j]+k);
						}
					}
					else if (idRangeOffset[j] != 0xffff)
					{
						//positions may vary
						for (k=0;k<endCount[j]-startCount[j]+1;k++)
						{
							ugp->glyphIndex.Add(glyphs[j+k-segCount+idRangeOffset[j]/2]+idDelta[j]);
							m_UnicodeGlyphs.Add(glyphs[j+k-segCount+idRangeOffset[j]/2]+idDelta[j]);
						}
					}
					if (k!=0)
						m_UnicodeGroups.Add(ugp);
				}
				delete[] startCount;
				delete[] endCount;
				delete[] idDelta;
				delete[] idRangeOffset;
				delete[] glyphs;
			}
			else if (format==0) //ascii char map
			{
				/*m_pEncFormat0=(ENC_FORMAT0 *)lpPos;
				lpPos+=sizeof(ENC_FORMAT0);
				j=0;
				while (j<256)
				{
					UNICODE_GROUP *ugp=new UNICODE_GROUP;
					ugp->format=0;
					for (;j<256;j++)
					{
						if (m_pEncFormat0->glyphIdArray[j]!=0)
							break;
					}
					ugp->startOffset=j;
					ugp->glyhpIndexStart=m_pEncFormat0->glyphIdArray[j];
					for (j++;j<256;j++)
					{
						if (m_pEncFormat0->glyphIdArray[j]==0)
							break;
					}
					ugp->endOffset=j-1;
					//if (ugp->startOffset!=0x20)
						m_UnicodeGroups.Add(ugp);
				}*/
			}
			lpPos=lpTemp;
		}
		lpPos+=sizeof(ENCODING_RECORD);
	}
	if (m_UnicodeGroups.GetCount()==0 || m_UnicodeGlyphs.GetCount() == 0)
		 return FALSE;

	return TRUE;
}

BOOL COTFFont::ParseEBDT()
{
	if (m_pEBDT==NULL)
		return FALSE;
	LPBYTE lpPos=(LPBYTE)m_pEBDT;
	if (m_pEBDT->version!=0x00020000)
		return FALSE;
	return TRUE;
}

BOOL COTFFont::ParseEBLC()
{
	if (m_pEBLC==NULL)
		return FALSE;
	LPBYTE lpPos=(LPBYTE)m_pEBLC;
	if (m_pEBLC->version!=0x00020000)
		return FALSE;

	LPBYTE lpStart,lpSubArr,lpSubTable;
	lpStart=lpPos;

	lpPos+=sizeof(EBLC_HEADER);

	//fix glyphs array
	m_GlyphTable.RemoveAll();

	//UNICODE_GROUP *ugp=new UNICODE_GROUP;
	//int y,z;
	int y;
	INT_PTR sum=m_UnicodeGlyphs.GetCount();
	int indexformat, imageformat, k;
	//int num, size;
    __int32 offset, *glyphoffsets;
	m_Glyphs.RemoveAll();
	for (DWORD i=0;i<m_pEBLC->numSizes;i++)
	{
		bitmapSizeTable *bst;
		bst=(bitmapSizeTable *)lpPos;
		m_BitDepth=(WORD)bst->bitDepth;
		m_FontSize=(WORD)bst->ppemX;
		lpSubArr=&lpStart[bst->indexSubTableArrayOffset];
		for (DWORD j=0;j<bst->numberOfIndexSubTables;j++)
		{
			indexSubTableArray *ista;
			ista=(indexSubTableArray *)lpSubArr;
			lpSubTable=&lpSubArr[ista->additionalOffsetToIndexSubtable];
			//check out what format is the subtable and parse it
			indexformat=SWAPWORD(*((USHORT*)lpSubTable));
			lpSubTable+=sizeof(USHORT);
			imageformat=SWAPWORD(*((USHORT*)lpSubTable));
			lpSubTable+=sizeof(USHORT);
			offset=SWAPLONG(*((ULONG*)lpSubTable));
			lpSubTable+=sizeof(ULONG);
			USHORT last=ista->lastGlyphIndex;
			USHORT first=ista->firstGlyphIndex;
			m_GlyphsOffsets.format=indexformat;
			m_GlyphsOffsets.offset_start=lpSubTable;
			m_GlyphsOffsets.ebdt_offset=offset;
			m_GlyphsOffsets.first=first;
			switch (indexformat) {
				case 1: case 3:
					glyphoffsets=new __int32[last-first+2];
					for ( k=0; k<(last-first+2); k++ ) {
						if (indexformat==3)
						{
							glyphoffsets[k]=SWAPWORD(*((USHORT*)lpSubTable));
							lpSubTable+=sizeof(USHORT);
						}
						else
						{
							glyphoffsets[k]=SWAPLONG(*((ULONG*)lpSubTable));
							lpSubTable+=sizeof(ULONG);
						}
					}
					if ( indexformat==3 && ((last-first)&1) )
						lpSubTable+=sizeof(USHORT);
					//
					m_GlyphTable.SetSize(sum);
					//
					GLYPH_INFO*	temp;
					m_Glyphs.SetSize(last-first+1);
					m_GlyphsOffsets.final_offset=glyphoffsets[last-first+1];
					for ( k=0; k<=last-first; k++ ) {
						temp=new GLYPH_INFO;
						temp->format=imageformat;
						temp->index=k+first;
						temp->offset=offset+glyphoffsets[k];
						temp->size=glyphoffsets[k+1]-glyphoffsets[k];
						//get metrics for glyph
						LPBYTE lpGlyph=(LPBYTE)m_pEBDT;
						smallGlyphMetrics *sgm;
						if (temp->format == 1)
						{
							sgm=(smallGlyphMetrics *)&lpGlyph[temp->offset];
							temp->width = sgm->width;
							temp->height = sgm->height;
							temp->bearingX = sgm->BearingX;
							temp->bearingY = sgm->BearingY;
							temp->Advance = sgm->Advance;
						}
						m_Glyphs.SetAt(k, temp);

						//
						for (y=0;y<sum;y++)
						{
							if (temp->index==m_UnicodeGlyphs.GetAt(y))
								m_GlyphTable.SetAt(y,k);
						}
					}
					delete[] glyphoffsets;
					break;
				case 2:
					break;
				case 4:
					break;
				case 5:
					break;
			}
			lpSubArr+=sizeof(indexSubTableArray);
		}
		lpPos+=sizeof(bitmapSizeTable);
	}
	if (m_Glyphs.GetCount() == 0 || m_GlyphTable.GetCount() == 0)
		return FALSE;
	return TRUE;
}

void COTFFont::ResetGlyphData()
{
	LPBYTE lpPos=(LPBYTE)m_pEBDT;
	lpPos+=sizeof(EBDT_HEADER);
	DWORD i=0;
	for (i=0;i<m_EBDTLen;i++)
		lpPos[i]=0;
	m_Offset=sizeof(EBDT_HEADER);

	smallGlyphMetrics *sgm;
	sgm=(smallGlyphMetrics *)lpPos;
	sgm->Advance = 0;
	sgm->width = 0;
	sgm->height = 0;
	sgm->BearingX = 0;
	sgm->BearingY = 0;

	GLYPH_INFO *gi;
	INT_PTR j;
	for (j=0;j<m_Glyphs.GetCount();j++)
	{
		gi=m_Glyphs.GetAt(j);
		gi->offset=m_Offset;
	}

	m_Offset+=sizeof(smallGlyphMetrics);
}

DWORD COTFFont::GetGlyphDataLen()
{
	if (m_pHeader==NULL)
		return 0;

	DWORD sum=0;
	sum=m_EBDTLen-sizeof(EBDT_HEADER);
	return sum;
}

COLORREF COTFFont::GetFontPixel(WORD index, CPoint point)
{
	GLYPH_INFO *gi;
	gi=m_Glyphs.GetAt(index);

	if (point.x > gi->width || point.y > gi->height)
		return RGB(0, 0, 0);

	LPBYTE lpPos=(LPBYTE)m_pEBDT;
	lpPos=&lpPos[gi->offset+sizeof(smallGlyphMetrics)];

	BYTE r, g, b;
	WORD d=0;
	LONG x;
	if (gi->width%2==0)
		lpPos+=point.y*gi->width/2;
	else
		lpPos+=point.y*(gi->width+1)/2;
	x=point.x;
	switch (m_BitDepth)
	{
	case 1:
		d = lpPos[x / 8];
		d = d >> (7 - (x % 8));
		d = d & 0x01;
		r = 255 - (d * 255);
		g = r;
		b = r;
		break;
	case 2:
		d = lpPos[x / 4];
		d = d >> (6 - (x % 4) * 2);
		d = d & 0x03;
		r = (3 - d) * 85;
		g = r;
		b = r;
		break;
	case 4:
		d = lpPos[x / 2];
		d = d >> (4 - (x % 2) * 4);
		d = d & 0x0F;
		r = (15 - d) * 17;
		g = r;
		b = r;
		break;
	}

	return RGB(r,g,b);
}

void COTFFont::SetFontPixel(WORD index, LONG x, LONG y, COLORREF color)
{
	GLYPH_INFO *gi;
	gi=m_Glyphs.GetAt(index);

	if (x > gi->width || y > gi->height)
		return;
	
	LPBYTE lpPos=(LPBYTE)m_pEBDT;
	lpPos=&lpPos[gi->offset+sizeof(smallGlyphMetrics)];

	if (gi->width%2==0)
		lpPos+=y*gi->width/2;
	else
		lpPos+=y*(gi->width+1)/2;

	WORD d, mask;

	switch (m_BitDepth)
	{
	case 1:
		if (color == RGB(0, 0, 0))
			d = 1;
		else
			d = 0;
		lpPos += x / 8;
		d = d << (7 - x % 8);
		mask = 1 << (7 - x % 8);
		*lpPos &= 255 - mask;
		*lpPos |= d;
		break;
	case 2:
		d = GetRValue(color) + GetGValue(color) + GetBValue(color);
		d = d / 3;
		d = (255 - d) * 3 / 255;

		lpPos += x / 4;
		d = d << (6 - (x % 4) * 2);
		mask = 3 << (6 - (x % 4) * 2);
		*lpPos &= 255 - mask;
		*lpPos |= d;
		break;
	case 4:
		d = GetRValue(color) + GetGValue(color) + GetBValue(color);
		d = d / 3;
		d = (255 - d) * 15 / 255;

		lpPos += x / 2;
		d = d << (4 - (x % 2) * 4);
		mask = 15 << (4 - (x % 2) * 4);
		*lpPos &= 255 - mask;
		*lpPos |= d;
		break;
	}
}

WORD COTFFont::GetGlyphIndex(WORD index)
{
	if (index+1>m_GlyphTable.GetCount())
		return 1;
	return m_GlyphTable.GetAt(index);
}

WORD COTFFont::GetNumGlyphs()
{
	if (m_pHeader == NULL)
		return 0;

	return (WORD)m_GlyphTable.GetCount();
}

WORD COTFFont::GetNumUnicodeGroups()
{
	if (m_pHeader == NULL)
		return 0;

	return (WORD)m_UnicodeGroups.GetCount();
}

void COTFFont::GetUnicodeGroup(WORD index, LPWORD start, LPWORD end, LPWORD offset)
{
	if (index >= GetNumUnicodeGroups())
	{
		*start = 0;
		*end = 0;
		*offset = 0;
		return;
	}

	UNICODE_GROUP *ugp;
	ugp=m_UnicodeGroups.GetAt(index);
	*start = ugp->startOffset;
	*end = ugp->endOffset;

	*offset=0;
	GLYPH_INFO *gi;
	for (int i=0;i<m_GlyphTable.GetCount();i++)
	{
		gi=m_Glyphs.GetAt(m_GlyphTable.GetAt(i));
		if (ugp->glyphIndex.GetAt(0)==gi->index)
		{
			*offset = i+1;
		}
	}
}

CSize COTFFont::GetFontBitmapSize(WORD index)
{
	GLYPH_INFO *gi;
	gi=m_Glyphs.GetAt(index);
	return CSize(gi->width,gi->height);
}

WORD COTFFont::GetUnicodeChar(WORD index, LPWORD groupIndex)
{
	WORD i;

	DWORD sum = 0;
	UNICODE_GROUP *ugp;

	for (i = 0; i < m_UnicodeGroups.GetCount(); i++)
	{
		ugp=m_UnicodeGroups.GetAt(i);
		if (index >= sum && index < (sum + ugp->endOffset - ugp->startOffset + 1))
		{
			*groupIndex = i;
			return (WORD)(ugp->startOffset + index - sum);
		}

		sum += ugp->endOffset - ugp->startOffset + 1;
	}

	return 0;
}

WORD COTFFont::GetNumUnicodeChars()
{
	//WORD i;

	//WORD sum = 0;

	if (m_pHeader == NULL)
		return 0;

	return (WORD)m_UnicodeGlyphs.GetCount();

	/*
	UNICODE_GROUP *ugp=new UNICODE_GROUP;

	for (i = 0; i < m_UnicodeGroups.GetCount(); i++)
	{
		ugp=m_UnicodeGroups.GetAt(i);
		sum += ugp->endOffset - ugp->startOffset + 1;
	}

	return sum;*/
}

WORD COTFFont::GetUnicodeCharOffset(WORD c)
{
	WORD i;

	UNICODE_GROUP *ugp;
	for (i = 0; i < m_UnicodeGroups.GetCount(); i++)
	{
		ugp=m_UnicodeGroups.GetAt(i);
		if (c >= ugp->startOffset && c <= ugp->endOffset)
		{
			GLYPH_INFO *gi;
			for (int j=0;j<m_Glyphs.GetCount();j++)
			{
				gi=m_Glyphs.GetAt(j);
				if (ugp->glyphIndex.GetAt(c - ugp->startOffset)==gi->index)
					return j;
			}
		}
	}

	return 0;
}

void COTFFont::GetCharMetrics(WORD index, LPBYTE width, LPBYTE height, CHAR *bearingx, CHAR *bearingy, LPBYTE advance)
{
	if (m_pHeader == NULL)
	{
		*width = 0;
		*height = 0;
		*bearingx = 0;
		*bearingy = 0;
		*advance = 0;
		return;
	}

	GLYPH_INFO *gi;
	gi=m_Glyphs.GetAt(index);
	*width = gi->width;
	*height = gi->height;
	*bearingx = gi->bearingX;
	*bearingy = gi->bearingY;
	*advance = gi->Advance;
}

void COTFFont::SetCharMetrics(WORD index, BYTE width, BYTE height, CHAR bearingx, CHAR bearingy, BYTE advance, BOOL bMakeFont)
{
	if (m_pHeader == NULL || index > (m_Glyphs.GetCount()+1))
		return;

	GLYPH_INFO *gi;
	gi=m_Glyphs.GetAt(index);

	//

	DWORD tempsize=gi->size;
	gi->size=sizeof(smallGlyphMetrics);
	if (width%2==0)
		gi->size+=(width*height)/(8/m_BitDepth);
	else
		gi->size+=((width+1)*height)/(8/m_BitDepth);

	LPBYTE lpPos;
	smallGlyphMetrics *sgm;

	if ((index+1)<m_Glyphs.GetCount())
	{
		GLYPH_INFO *gi2;
		gi2=m_Glyphs.GetAt(index+1);

		if ((gi->offset+width*height)>gi2->offset)
		{
			BOOL bEnd=FALSE;
			int i=0;
			while ((gi->offset+gi->size)>gi2->offset)
			{
				lpPos=m_GlyphsOffsets.offset_start;

				if (m_GlyphsOffsets.format==3)
				{
					lpPos+=sizeof(USHORT)*(gi2->index-m_GlyphsOffsets.first);
					USHORT soff=(USHORT)(gi->offset-m_GlyphsOffsets.ebdt_offset+gi->size+i*sizeof(smallGlyphMetrics));
					*((USHORT *)lpPos)=SWAPWORD(soff);
					gi2->offset=m_GlyphsOffsets.ebdt_offset+soff;
				}
				else
				{
					lpPos+=sizeof(ULONG)*(gi2->index-m_GlyphsOffsets.first);
					ULONG loff=gi->offset-m_GlyphsOffsets.ebdt_offset+gi->size+i*sizeof(smallGlyphMetrics);
					*((ULONG *)lpPos)=SWAPLONG(loff);
					gi2->offset=m_GlyphsOffsets.ebdt_offset+loff;
				}
				//
				gi2->width=0;
				gi2->height=0;
				gi2->bearingX=0;
				gi2->bearingY=0;
				gi2->Advance=0;
				lpPos=(LPBYTE)m_pEBDT;
				lpPos=&lpPos[gi2->offset];
				sgm=(smallGlyphMetrics *)lpPos;
				sgm->width=0;
				sgm->height=0;
				sgm->Advance=0;
				sgm->BearingY=0;
				sgm->BearingX=0;
				//

				i++;
				if (index+1+i<m_Glyphs.GetCount())
					gi2=m_Glyphs.GetAt(index+1+i);
				else
				{
					bEnd=TRUE;
					break;
				}
			}

			/*
			if (!bEnd)
			{
				lpPos=(LPBYTE)m_pEBDT;
				DWORD k=gi->offset+gi->size+i*sizeof(smallGlyphMetrics);
				for (;k<gi2->offset;k++)
					lpPos[k]=0;
			}
			*/
		}
	}
	else
	{
		if (gi->size>(m_GlyphsOffsets.final_offset-gi->offset))
		{
			gi->size=tempsize;
			return;
		}
	}

	lpPos=(LPBYTE)m_pEBDT;
	lpPos=&lpPos[gi->offset];
	sgm=(smallGlyphMetrics *)lpPos;
	sgm->Advance = advance;
	sgm->width = width;
	sgm->height = height;
	sgm->BearingX = bearingx;
	sgm->BearingY = bearingy;
	//
	gi->width = width;
	gi->height = height;
	gi->bearingX = bearingx;
	gi->bearingY = bearingy;
	gi->Advance = advance;
	//
}

void COTFFont::FillEndGlyphs(int i)
{
	WORD glyphIndex;
	WORD group;
	GLYPH_INFO *gi;
	LPBYTE lpPos;
	for (;i<GetNumUnicodeChars();i++)
	{
		glyphIndex=GetUnicodeCharOffset(GetUnicodeChar(i, &group));
		gi=m_Glyphs.GetAt(glyphIndex);

		lpPos=m_GlyphsOffsets.offset_start;
		if (m_GlyphsOffsets.format==3)
		{
			lpPos+=sizeof(USHORT)*(gi->index-m_GlyphsOffsets.first);
			*((USHORT *)lpPos)=SWAPWORD((USHORT)(sizeof(EBDT_HEADER)-m_GlyphsOffsets.ebdt_offset));
		}
		else
		{
			lpPos+=sizeof(ULONG)*(gi->index-m_GlyphsOffsets.first);
			*((ULONG *)lpPos)=SWAPLONG((sizeof(EBDT_HEADER)-m_GlyphsOffsets.ebdt_offset));
		}
	}
}

void COTFFont::AddCharMetrics(WORD index, BYTE width, BYTE height, CHAR bearingx, CHAR bearingy, BYTE advance)
{
	if (m_pHeader == NULL || index > (m_Glyphs.GetCount()+1))
		return;

	GLYPH_INFO *gi;
	gi=m_Glyphs.GetAt(index);

	//
	smallGlyphMetrics *sgm;
	LPBYTE lpPos=(LPBYTE)m_pEBDT;
	lpPos=&lpPos[m_Offset];
	sgm=(smallGlyphMetrics *)lpPos;
	sgm->Advance = advance;
	sgm->width = width;
	sgm->height = height;
	sgm->BearingX = bearingx;
	sgm->BearingY = bearingy;

	//
	gi->width = width;
	gi->height = height;
	gi->bearingX = bearingx;
	gi->bearingY = bearingy;
	gi->Advance = advance;
	gi->offset=m_Offset;
	gi->size=sizeof(smallGlyphMetrics);
	if (width%2==0)
		gi->size+=(width*height)/(8/m_BitDepth);
	else
		gi->size+=((width+1)*height)/(8/m_BitDepth);
	//

	lpPos=m_GlyphsOffsets.offset_start;
	if (m_GlyphsOffsets.format==3)
	{
		lpPos+=sizeof(USHORT)*(gi->index-m_GlyphsOffsets.first);
		*((USHORT *)lpPos)=SWAPWORD((USHORT)(gi->offset-m_GlyphsOffsets.ebdt_offset));
	}
	else
	{
		lpPos+=sizeof(ULONG)*(gi->index-m_GlyphsOffsets.first);
		*((ULONG *)lpPos)=SWAPLONG((gi->offset-m_GlyphsOffsets.ebdt_offset));
	}

	m_Offset+=gi->size;
}

BOOL COTFFont::GetUnicodeGroupGlyphState(WORD index)
{
	if (index >= GetNumUnicodeGroups())
		return FALSE;

	UNICODE_GROUP *ugp;
	ugp=m_UnicodeGroups.GetAt(index);

	if (SWAPWORD(*ugp->byteoffset_r)==0)
		return TRUE;
	else if (SWAPWORD(*ugp->byteoffset_r)!=0xffff)
		return FALSE;
	else
		return TRUE;
}

void COTFFont::SetUnicodeGroup(WORD index, WORD start, WORD end, WORD offset)
{
	if (index >= GetNumUnicodeGroups())
		return;

	GLYPH_INFO *gi;
	UNICODE_GROUP *ugp;
	ugp=m_UnicodeGroups.GetAt(index);

	if (ugp->format==4)
	{
		*ugp->byteoffset_s=SWAPWORD(start);
		*ugp->byteoffset_e=SWAPWORD(end);
		if (SWAPWORD(*ugp->byteoffset_r)==0)
		{
			//change only iddelta
			if (start!=ugp->startOffset) //unicode offsets changed
				*ugp->byteoffset_d=SWAPWORD(SWAPWORD(*ugp->byteoffset_d)-((signed)start-(signed)ugp->startOffset));
			else //glyph index changed
			{
				gi=m_Glyphs.GetAt(m_GlyphTable.GetAt(offset));
				*ugp->byteoffset_d=SWAPWORD(SWAPWORD(*ugp->byteoffset_d)+((signed)gi->index-(signed)ugp->glyphIndex.GetAt(0)));
			}
		}
		else if (SWAPWORD(*ugp->byteoffset_r)!=0xffff)
		{
			//glyphs[j+k-segCount+idRangeOffset[j]/2]+idDelta[j]
			if (start!=ugp->startOffset) //unicode offsets changed
			{
				//we need to do nothing here..
			}
			else  //glyph index changed
			{
				//still have to think about what to do here
			}
		}
	}
	
	if (start==ugp->startOffset)
	{
		for (int i=0;i<ugp->glyphIndex.GetCount();i++)
		{
			gi=m_Glyphs.GetAt(m_GlyphTable.GetAt(offset+i));
			ugp->glyphIndex.SetAt(i, gi->index);
		}
	}

	ugp->startOffset = start;
	ugp->endOffset = end;


	//We must sort the unicode groups array or else it might cause some chars to be represented as missing glyphs although they exist
	DWORD i,j,indexOfMin;
	UNICODE_GROUP *curr;

	//swap variables:
	WORD endOffset,startOffset,format;
	USHORT e_val,s_val,r_val;
	SHORT d_val;
	CWordArray glyphIndex;

	DWORD arrsize=(DWORD)m_UnicodeGroups.GetCount();

	for (i=0;i<arrsize;i++)
	{
		indexOfMin=i;
		for (j=i+1;j<arrsize;j++)
		{
			curr=m_UnicodeGroups.GetAt(indexOfMin);
			ugp=m_UnicodeGroups.GetAt(j);
			if (ugp->endOffset<curr->endOffset)
				indexOfMin=j;
		}

		//swap
		if (indexOfMin!=i)
		{
			ugp=m_UnicodeGroups.GetAt(i);
			e_val=*ugp->byteoffset_e;
			s_val=*ugp->byteoffset_s;
			r_val=*ugp->byteoffset_r;
			d_val=*ugp->byteoffset_d;
			startOffset=ugp->startOffset;
			endOffset=ugp->endOffset;
			format=ugp->format;

			
			glyphIndex.RemoveAll();
			//glyphIndex.SetSize(ugp->glyphIndex.GetCount());
			//memcpy(&glyphIndex, &ugp->glyphIndex, ugp->glyphIndex.GetCount()*sizeof(WORD));
			glyphIndex.Copy(ugp->glyphIndex);

			curr=m_UnicodeGroups.GetAt(indexOfMin);
			*ugp->byteoffset_d=*curr->byteoffset_d;
			*ugp->byteoffset_r=*curr->byteoffset_r;
			*ugp->byteoffset_s=*curr->byteoffset_s;
			*ugp->byteoffset_e=*curr->byteoffset_e;
			ugp->startOffset=curr->startOffset;
			ugp->endOffset=curr->endOffset;
			ugp->format=curr->format;

			ugp->glyphIndex.RemoveAll();
			//ugp->glyphIndex.SetSize(curr->glyphIndex.GetCount());
			//memcpy(ugp->glyphIndex.GetData(), curr->glyphIndex.GetData(), curr->glyphIndex.GetCount()*sizeof(WORD));
			ugp->glyphIndex.Copy(curr->glyphIndex);
			
			*curr->byteoffset_d=d_val;
			*curr->byteoffset_r=r_val;
			*curr->byteoffset_s=s_val;
			*curr->byteoffset_e=e_val;
			curr->startOffset=startOffset;
			curr->endOffset=endOffset;
			curr->format=format;

			curr->glyphIndex.RemoveAll();
			//curr->glyphIndex.SetSize(glyphIndex.GetCount());
			//memcpy(curr->glyphIndex.GetData(), glyphIndex.GetData(), glyphIndex.GetCount()*sizeof(WORD));
			curr->glyphIndex.Copy(glyphIndex);
		}
	}
}