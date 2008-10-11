#include "StdAfx.h"
#include <atlconv.h>
#include ".\font.h"

CIpodFont::CIpodFont(void):
	m_pHeader1(NULL),
	m_pHeader2(NULL),
	m_pUnicodeGroups(NULL),
	m_pCharMapping(NULL),
	m_pCharInfo(NULL),
	m_pBitmapData(NULL)
{
}

CIpodFont::~CIpodFont(void)
{
}

// operations

BOOL CIpodFont::Read(LPBYTE lpBuffer, LPBYTE lpEnd)
{
	if (lpBuffer == NULL)
		return FALSE;

	LPBYTE lpPos = lpBuffer;

	if (lpPos + sizeof(IPOD_FONT_HEADER1) >= lpEnd)
		return FALSE;

	m_pHeader1 = (IPOD_FONT_HEADER1 *)lpPos;
	lpPos += sizeof(IPOD_FONT_HEADER1);

	//if (memcmp(&m_pHeader1->magic, FONT_MAGIC, 4))
	//	return FALSE;

	// validate

	if (m_pHeader1->numChars == 0 || m_pHeader1->numMappingEntries == 0 || 
		m_pHeader1->numMappingEntries > 0x2000)
		return FALSE;

	//
	DWORD i;

	m_pUnicodeGroups = (IPOD_FONT_UNICODE_GROUP *)lpPos;

	for (i = 0; i < m_pHeader1->numMappingEntries; i++)
	{
		if (lpPos + sizeof(IPOD_FONT_UNICODE_GROUP) >= lpEnd)
			return FALSE;
		lpPos += sizeof(IPOD_FONT_UNICODE_GROUP);
	}

	if (lpPos + sizeof(IPOD_FONT_HEADER2) >= lpEnd)
		return FALSE;

	m_pHeader2 = (IPOD_FONT_HEADER2 *)lpPos;
	lpPos += sizeof(IPOD_FONT_HEADER2);

	// validate

	if (m_pHeader2->numChars != m_pHeader1->numChars)
		return FALSE;

	if (m_pHeader2->bitdepth != 1 && m_pHeader2->bitdepth != 2 && m_pHeader2->bitdepth != 4 && m_pHeader2->bitdepth != 8)
		return FALSE;

	BOOL bName = TRUE;
	BYTE c;

	for (i = 0; i < sizeof(m_pHeader2->name); i++)
	{
		c = m_pHeader2->name[i];
		if (bName = TRUE)
		{
			if (c == 0)
			{
				bName = FALSE;
			}
			else if ((c < '!' || c > 'z') && c != ' ')
				return FALSE;
		}
		else
		{
			if (c != 0)
				return FALSE;
		}
	}

	// read tables

	WORD charValue;

	m_pCharMapping = (LPWORD)lpPos;

	for (i = 0; i < m_pHeader2->numChars; i++)
	{
		charValue = *(LPWORD)lpPos;
		lpPos += sizeof(WORD);
	}

	if (m_pHeader2->bitdepth == 2)
		m_NumMetrics = (WORD)((m_pHeader2->metricsEnd - m_pHeader2->charMapEnd) / sizeof(IPOD_FONT_CHARINFO_V2));
	else
		m_NumMetrics = (WORD)((m_pHeader2->metricsEnd - m_pHeader2->charMapEnd) / sizeof(IPOD_FONT_CHARINFO));

	lpPos = ((lpPos - lpBuffer + 3) / 4) * 4 + lpBuffer;

	if (IsFixedWidth())
	{
		// fixed width
		m_pCharInfo = NULL;
		m_NumMetrics = m_pHeader2->numChars;
	}
	else
	{
		// variable width
		m_pCharInfo = (IPOD_FONT_CHARINFO *)lpPos;

		for (i = 0; i < m_NumMetrics; i++)
		{
			if (m_pHeader2->bitdepth == 2)
			{
				lpPos += sizeof(IPOD_FONT_CHARINFO_V2);
			}
			else
			{
				lpPos += sizeof(IPOD_FONT_CHARINFO);
			}
		}
	}

	// bitmap header
	if (m_pHeader2->bitdepth == 2)
	{
		IPOD_FONT_BITMAP_HEADER_V2 bitmapHeader;
		memcpy(&bitmapHeader, lpPos, sizeof(IPOD_FONT_BITMAP_HEADER_V2));

		lpPos += sizeof(IPOD_FONT_BITMAP_HEADER_V2);

		m_BitmapHeader.height = (WORD)bitmapHeader.height;
		m_BitmapHeader.rowLenghBytes = (WORD)bitmapHeader.rowLenghBytes;
		m_BitmapHeader.rowSizePixels = (WORD)bitmapHeader.rowSizePixels;
	}
	else
	{
		memcpy(&m_BitmapHeader, lpPos, sizeof(IPOD_FONT_BITMAP_HEADER));
		lpPos += sizeof(IPOD_FONT_BITMAP_HEADER);
	}

	// verify bitmap header
	//
	if (m_BitmapHeader.height == 0 || m_BitmapHeader.height > 80)
		return FALSE;

	// store bitmap data
	m_pBitmapData = lpPos;

	return TRUE;
}

DWORD CIpodFont::GetMetricDataLen()
{
	DWORD ugpblock=sizeof(IPOD_FONT_UNICODE_GROUP)*m_pHeader1->numMappingEntries;
	DWORD mapblock=m_pHeader2->numChars*sizeof(WORD);
	WORD m_NumMetrics;
	DWORD charblock;
	if (m_pHeader2->bitdepth == 2)
	{
		m_NumMetrics = (WORD)((m_pHeader2->metricsEnd - m_pHeader2->charMapEnd) / sizeof(IPOD_FONT_CHARINFO_V2));
		charblock=m_NumMetrics*sizeof(IPOD_FONT_CHARINFO_V2);
	}
	else
	{
		m_NumMetrics = (WORD)((m_pHeader2->metricsEnd - m_pHeader2->charMapEnd) / sizeof(IPOD_FONT_CHARINFO));
		charblock=m_NumMetrics*sizeof(IPOD_FONT_CHARINFO);
	}
	return ugpblock+mapblock+charblock;
}

LPBYTE CIpodFont::GetMetricData()
{
	LPBYTE lpBuf;
	DWORD ugpblock=sizeof(IPOD_FONT_UNICODE_GROUP)*m_pHeader1->numMappingEntries;
	DWORD mapblock=m_pHeader2->numChars*sizeof(WORD);
	WORD m_NumMetrics;
	DWORD charblock;
	if (m_pHeader2->bitdepth == 2)
	{
		m_NumMetrics = (WORD)((m_pHeader2->metricsEnd - m_pHeader2->charMapEnd) / sizeof(IPOD_FONT_CHARINFO_V2));
		charblock=m_NumMetrics*sizeof(IPOD_FONT_CHARINFO_V2);
	}
	else
	{
		m_NumMetrics = (WORD)((m_pHeader2->metricsEnd - m_pHeader2->charMapEnd) / sizeof(IPOD_FONT_CHARINFO));
		charblock=m_NumMetrics*sizeof(IPOD_FONT_CHARINFO);
	}
	
	DWORD total=ugpblock+mapblock+charblock+4;
	lpBuf=new BYTE[total];
	DWORD size=total-4;
	memcpy(lpBuf, (LPBYTE)&size, 4);
	LPBYTE lpPos=lpBuf+4;
	memcpy(lpPos, (LPBYTE)m_pUnicodeGroups, ugpblock);
	lpPos+=ugpblock;
	memcpy(lpPos, (LPBYTE)m_pCharMapping, mapblock);
	lpPos+=mapblock;
	memcpy(lpPos, (LPBYTE)m_pCharInfo, charblock);
	return lpBuf;
}

void CIpodFont::SetMetricData(LPBYTE lpBuf)
{
	DWORD ugpblock=sizeof(IPOD_FONT_UNICODE_GROUP)*m_pHeader1->numMappingEntries;
	DWORD mapblock=m_pHeader2->numChars*sizeof(WORD);
	WORD m_NumMetrics;
	DWORD charblock;
	if (m_pHeader2->bitdepth == 2)
	{
		m_NumMetrics = (WORD)((m_pHeader2->metricsEnd - m_pHeader2->charMapEnd) / sizeof(IPOD_FONT_CHARINFO_V2));
		charblock=m_NumMetrics*sizeof(IPOD_FONT_CHARINFO_V2);
	}
	else
	{
		m_NumMetrics = (WORD)((m_pHeader2->metricsEnd - m_pHeader2->charMapEnd) / sizeof(IPOD_FONT_CHARINFO));
		charblock=m_NumMetrics*sizeof(IPOD_FONT_CHARINFO);
	}
	LPBYTE lpPos=lpBuf;
	memcpy((LPBYTE)m_pUnicodeGroups, lpPos, ugpblock);
	lpPos+=ugpblock;
	memcpy((LPBYTE)m_pCharMapping, lpPos, mapblock);
	lpPos+=mapblock;
	memcpy((LPBYTE)m_pCharInfo, lpPos, charblock);
}

LPCTSTR	CIpodFont::GetFontStyle()
{
	if (m_pHeader1==NULL)
		return NULL;

	char ststyle[32];
	if (m_pHeader2->style & 0x01)
		strcpy(ststyle,"Bold");
	else
		strcpy(ststyle,"Regular");

	wchar_t* lpwStr;
	int nLen = MultiByteToWideChar(CP_ACP, 0, ststyle, -1, NULL, NULL);
	lpwStr = ( wchar_t* )malloc(( nLen )*sizeof( wchar_t ));
	MultiByteToWideChar(CP_ACP, 0,  ststyle, -1, lpwStr, nLen);
	return (LPTSTR)lpwStr;
}

void CIpodFont::SetFontName(CString sNewName)
{
	if (m_pHeader1==NULL)
		return;

	char *stname;
	stname=Unicode2MB(sNewName);
	if (strlen(stname)>63)
		return;

	strcpy(m_pHeader2->name, stname);
}

void CIpodFont::SetFontSize(DWORD iNewSize)
{
	if (m_pHeader2 == NULL)
		return;

	m_pHeader2->size=iNewSize;
}

void CIpodFont::SetFontStyle(CString sNewStyle)
{
	if (m_pHeader1==NULL)
		return;

	char *ststyle;
	ststyle=Unicode2MB(sNewStyle);
	if (!strcmp(ststyle, "Bold"))
		m_pHeader2->style|=0x01;
	else if (!strcmp(ststyle, "Regular"))
		m_pHeader2->style&=0x0;
}

void CIpodFont::SetData(LPBYTE buffer)
{
	memcpy((LPBYTE)m_pHeader1, buffer, GetFontBlockLen());
}

LPCTSTR CIpodFont::GetFontName()
{
	if (m_pHeader2 == NULL)
		return NULL;

	wchar_t* lpwStr;
	int nLen = MultiByteToWideChar(CP_ACP, 0, m_pHeader2->name, -1, NULL, NULL);
	lpwStr = ( wchar_t* )malloc(( nLen )*sizeof( wchar_t ));
	MultiByteToWideChar(CP_ACP, 0, m_pHeader2->name, -1, lpwStr, nLen);
	return (LPTSTR)lpwStr;
}

DWORD CIpodFont::GetFontBlockLen()
{
	return (DWORD)(m_pBitmapData - (LPBYTE)m_pHeader1 + (m_BitmapHeader.height * m_BitmapHeader.rowLenghBytes));
}

WORD CIpodFont::GetFontBitDepth()
{
	if (m_pHeader2 == NULL)
		return 0;

	return m_pHeader2->bitdepth;
}

DWORD CIpodFont::GetFontSize()
{
	if (m_pHeader2 == NULL)
		return 0;

	return m_pHeader2->size;
}

BOOL CIpodFont::IsFixedWidth()
{
	if (m_pHeader2 == NULL)
		return FALSE;

	if (m_pHeader2->flags & 0x01)
		return TRUE;

	return FALSE;
}

CSize CIpodFont::GetFontBitmapSize()
{
	return CSize(m_BitmapHeader.rowSizePixels, m_BitmapHeader.height);
}

void CIpodFont::ReMakeBitmap()
{
	pNewBitmap=new BYTE[GetFontBlockLen()];
	memset(pNewBitmap, 0, GetFontBlockLen());
	unsigned long i,j,k,m,start,y;
	WORD c, group;

	CSize size = GetFontBitmapSize();

	DWORD offset1, offset2;
	SHORT width, ident;
	WORD w;

	LPBYTE pNewMetrics;
	DWORD len;
	if (m_pHeader2->bitdepth==2)
	{
		len=sizeof(IPOD_FONT_CHARINFO_V2)*GetNumUnicodeChars();
	}
	else
	{
		len=sizeof(IPOD_FONT_CHARINFO)*GetNumUnicodeChars();
	}
	pNewMetrics=new BYTE[len];

	LPBYTE saveBitmap;
	saveBitmap=m_pBitmapData;
	CPoint p;
	COLORREF cr;
	CWordArray wlist;

	m=0;
	y=(unsigned)-1;
	bool end=false;
	for (i = 0; i < GetNumUnicodeChars(); i++)
	{
		c = GetUnicodeChar(i, &group);

		w = GetCharMapping(GetUnicodeCharOffset(c));

		for (j=0;j<wlist.GetCount();j++)
			if (wlist.GetAt(j)==w)
			{
				end=true;
				break;
			}
		if (end)
		{
			SetCharMapping(GetUnicodeCharOffset(c), j);
			end=false;
			continue;
		}
		wlist.Add(w);
		y++;

		GetCharMetrics(w, &offset1, &offset2, &width, &ident);

		SetCharMapping(GetUnicodeCharOffset(c), y);

		if (width>30 || (offset2-offset1)>30 || (offset2-offset1)<0)
		{
			if (m_pHeader2->bitdepth==2)
			{
				((IPOD_FONT_CHARINFO_V2 *)pNewMetrics)[y].ident = ident;
				((IPOD_FONT_CHARINFO_V2 *)pNewMetrics)[y].offset1 = offset1;
				((IPOD_FONT_CHARINFO_V2 *)pNewMetrics)[y].offset2 = offset2;
				((IPOD_FONT_CHARINFO_V2 *)pNewMetrics)[y].width = width;
			}
			else
			{
				((IPOD_FONT_CHARINFO *)pNewMetrics)[y].ident = ident;
				((IPOD_FONT_CHARINFO *)pNewMetrics)[y].offset1 = offset1;
				((IPOD_FONT_CHARINFO *)pNewMetrics)[y].offset2 = offset2;
				((IPOD_FONT_CHARINFO *)pNewMetrics)[y].width = width;
			}
			continue;
		}

		start=m;
		for (k=offset1;k<offset2;k++,m++)
		{
			for (j=0;j<size.cy;j++)
			{
				p.SetPoint(k, j);
				cr=GetFontPixel(p);
				m_pBitmapData=pNewBitmap;
				if (m>=size.cx)
				{
					//out of space!
					//m_pBitmapData=saveBitmap;
					end=true;
					break;
				}
				SetFontPixel(m, j, cr);
				m_pBitmapData=saveBitmap;
			}
			if (end)
				break;
		}
		if (end)
			break;

		if (m_pHeader2->bitdepth==2)
		{
			((IPOD_FONT_CHARINFO_V2 *)pNewMetrics)[y].ident = ident;
			((IPOD_FONT_CHARINFO_V2 *)pNewMetrics)[y].offset1 = start;
			((IPOD_FONT_CHARINFO_V2 *)pNewMetrics)[y].offset2 = m;
			((IPOD_FONT_CHARINFO_V2 *)pNewMetrics)[y].width = width;
		}
		else
		{
			((IPOD_FONT_CHARINFO *)pNewMetrics)[y].ident = ident;
			((IPOD_FONT_CHARINFO *)pNewMetrics)[y].offset1 = start;
			((IPOD_FONT_CHARINFO *)pNewMetrics)[y].offset2 = m;
			((IPOD_FONT_CHARINFO *)pNewMetrics)[y].width = width;
		}
	}
finish:
	if (m_pHeader2->bitdepth==2)
	{
		len=sizeof(IPOD_FONT_CHARINFO_V2)*y;
	}
	else
	{
		len=sizeof(IPOD_FONT_CHARINFO)*y;
	}
	memcpy(m_pCharInfo, pNewMetrics, len);
	delete pNewMetrics;
	m_pBitmapData=pNewBitmap;
	return;
}

LPBYTE CIpodFont::MakeFont()
{
	LPBYTE lpBuffer;
	LPBYTE lpPos;
	DWORD len=GetFontBlockLen();
	DWORD unicode_group_num=2;
	DWORD num_chars=145;
	BYTE bitdepth=4;
	BYTE ststyle=0; //0 for regular, 1 for bold
	DWORD fsize=18;
	lpBuffer=new BYTE[len];
	lpPos=lpBuffer;
	memset(lpBuffer, 0, len);
	
	IPOD_FONT_HEADER1 *header1=m_pHeader1;
	IPOD_FONT_HEADER2 *header2=m_pHeader2;

	header1->first_char=0x6000; //602a
	header1->last_char=0xfeff; //6069
	header1->numChars=num_chars;
	header1->numMappingEntries=unicode_group_num;

	header2->bitdepth=bitdepth;
	header2->line_height=32;
	strcpy(header2->name, "Podium Sans_0600_0700");
	header2->numChars=num_chars;
	header2->size=fsize;
	header2->style=ststyle;
	header2->width=fsize;
	header2->flags=0;
	
	memcpy(lpPos, header1, sizeof(IPOD_FONT_HEADER1));
	lpPos += sizeof(IPOD_FONT_HEADER1);

	//
	DWORD i;

	IPOD_FONT_UNICODE_GROUP *UnicodeGroups = new IPOD_FONT_UNICODE_GROUP[unicode_group_num];

	/*for (i=0;i<unicode_group_num;i++)
	{
		UnicodeGroups[i].code=0x0020;
		UnicodeGroups[i].length=1;
		UnicodeGroups[i].offset=1;
	}*/
	UnicodeGroups[0].code=0x0627;
	UnicodeGroups[0].length=36;
	UnicodeGroups[0].offset=1;
	UnicodeGroups[1].code=0xfe8e;
	UnicodeGroups[1].length=114;
	UnicodeGroups[1].offset=37;
	
	memcpy(lpPos, UnicodeGroups, sizeof(IPOD_FONT_UNICODE_GROUP)*unicode_group_num);
	lpPos+=sizeof(IPOD_FONT_UNICODE_GROUP)*unicode_group_num;
	
	memcpy(lpPos, header2, sizeof(IPOD_FONT_HEADER2));
	header2=(IPOD_FONT_HEADER2 *)lpPos;
	lpPos += sizeof(IPOD_FONT_HEADER2);

	WORD *CharMapping=new WORD[num_chars];

	for (i = 0; i < num_chars; i++)
	{
		CharMapping[i]=i;
	}

	memcpy(lpPos, CharMapping, sizeof(WORD)*num_chars);
	lpPos+=sizeof(WORD)*num_chars;

	// read tables


	header2->charMapEnd=(DWORD)(lpPos-(LPBYTE)header2);

	
	//m_NumMetrics is equal for us to num chars because we don't use the same metric twice
	//m_NumMetrics = num_chars;
	if (bitdepth == 2)
		header2->metricsEnd=header2->charMapEnd+sizeof(IPOD_FONT_CHARINFO_V2)*num_chars;
	else
		header2->metricsEnd=header2->charMapEnd+sizeof(IPOD_FONT_CHARINFO)*num_chars;

	lpPos = ((lpPos - lpBuffer + 3) / 4) * 4 + lpBuffer;

	// variable width
	IPOD_FONT_CHARINFO *c1=new IPOD_FONT_CHARINFO[num_chars];
	IPOD_FONT_CHARINFO_V2 *c2=new IPOD_FONT_CHARINFO_V2[num_chars];

	for (i = 0; i < num_chars; i++)
	{
		if (bitdepth == 2)
		{
			c2[i].ident=0;
			c2[i].width=0;
			c2[i].offset1=0;
			c2[i].offset2=0;
		}
		else
		{
			c1[i].ident=0;
			c1[i].width=0;
			c1[i].offset1=0;
			c1[i].offset2=0;
		}
	}

	if (bitdepth == 2)
	{
		memcpy(lpPos, c2, sizeof(IPOD_FONT_CHARINFO_V2)*num_chars);
		lpPos+=sizeof(IPOD_FONT_CHARINFO_V2)*num_chars;
	}
	else
	{
		memcpy(lpPos, c1, sizeof(IPOD_FONT_CHARINFO)*num_chars);
		lpPos+=sizeof(IPOD_FONT_CHARINFO)*num_chars;
	}

	DWORD bmplen;
	// bitmap header
	if (bitdepth == 2)
	{
		IPOD_FONT_BITMAP_HEADER_V2 bitmapHeader2;
		bmplen=len-(lpPos-lpBuffer+sizeof(IPOD_FONT_BITMAP_HEADER_V2));
		bitmapHeader2.height=header2->line_height+2;
		bitmapHeader2.rowLenghBytes=bmplen/bitmapHeader2.height;
		bitmapHeader2.rowSizePixels=bitmapHeader2.rowLenghBytes*8/bitdepth;
		memcpy(&bitmapHeader2.d2,&bmplen,4);

		memcpy(lpPos, &bitmapHeader2, sizeof(IPOD_FONT_BITMAP_HEADER_V2));

		lpPos += sizeof(IPOD_FONT_BITMAP_HEADER_V2);
	}
	else
	{
		IPOD_FONT_BITMAP_HEADER bitmapHeader;
		bmplen=len-(lpPos-lpBuffer+sizeof(IPOD_FONT_BITMAP_HEADER));
		bitmapHeader.w1=m_BitmapHeader.w1;
		bitmapHeader.w2=m_BitmapHeader.w2;
		bitmapHeader.bitDepth=bitdepth;
		bitmapHeader.height=header2->line_height+2;
		bitmapHeader.rowLenghBytes=bmplen/bitmapHeader.height;
		bitmapHeader.rowSizePixels=bitmapHeader.rowLenghBytes*8/bitdepth;
		bitmapHeader.length=bmplen;

		memcpy(lpPos, &bitmapHeader, sizeof(IPOD_FONT_BITMAP_HEADER));

		lpPos += sizeof(IPOD_FONT_BITMAP_HEADER);
	}
	memcpy(m_pHeader1, lpBuffer, len);
	return lpBuffer;
}

COLORREF CIpodFont::GetFontPixel(CPoint point)
{
	CSize size = GetFontBitmapSize();

	if (point.x >= size.cx || point.y >= size.cy)
		return RGB(0, 0, 0);

	LPBYTE linePointer;
	BYTE r, g, b;
	WORD d;
	LONG x;

	linePointer = m_pBitmapData + point.y * m_BitmapHeader.rowLenghBytes;

	x = point.x;

	switch (GetFontBitDepth())
	{
	case 1:
		d = linePointer[x / 8];
		d = d >> (7 - (x % 8));
		d = d & 0x01;
		r = 255 - (d * 255);
		g = r;
		b = r;
		break;
	case 2:
		d = linePointer[x / 4];
		d = d >> (6 - (x % 4) * 2);
		d = d & 0x03;
		r = (3 - d) * 85;
		g = r;
		b = r;
		break;
	case 4:
		d = linePointer[x / 2];
		d = d >> (4 - (x % 2) * 4);
		d = d & 0x0F;
		r = (15 - d) * 17;
		g = r;
		b = r;
		break;
	case 16:
		d = (linePointer[x * 2 + 1] & 0xF8) >> 3;
		r = d * 255 / 31;

		d = (linePointer[x * 2 + 1] & 0x07);
		d = d << 3;
		d += (linePointer[x * 2] & 0xE0) >> 5;
		g = d * 255 / 63;

		d = linePointer[x * 2] & 0x1F;
		b = d * 255 / 31;
		
		break;
	}

	return RGB(r, g, b);
}

void CIpodFont::SetFontPixel(LONG x, LONG y, COLORREF color)
{
	WORD d, mask;
	LPBYTE linePointer;
	WORD bitDepth;

	CSize size = GetFontBitmapSize();

	if (x >= size.cx || y >= size.cy)
		return;

	bitDepth = GetFontBitDepth();

	linePointer = m_pBitmapData + y * m_BitmapHeader.rowLenghBytes;

	switch (bitDepth)
	{
	case 1:
		if (color == RGB(0, 0, 0))
			d = 1;
		else
			d = 0;
		linePointer += x / 8;
		d = d << (7 - x % 8);
		mask = 1 << (7 - x % 8);
		*linePointer &= 255 - mask;
		*linePointer |= d;
		break;
	case 2:
		d = GetRValue(color) + GetGValue(color) + GetBValue(color);
		d = d / 3;
		d = (255 - d) * 3 / 255;

		linePointer += x / 4;
		d = d << (6 - (x % 4) * 2);
		mask = 3 << (6 - (x % 4) * 2);
		*linePointer &= 255 - mask;
		*linePointer |= d;
		break;
	case 4:
		d = GetRValue(color) + GetGValue(color) + GetBValue(color);
		d = d / 3;
		d = (255 - d) * 15 / 255;

		linePointer += x / 2;
		d = d << (4 - (x % 2) * 4);
		mask = 15 << (4 - (x % 2) * 4);
		*linePointer &= 255 - mask;
		*linePointer |= d;
		break;
	case 16:
		linePointer += x * 2;
		d = GetRValue(color) * 31 / 255;
		d = d << 6;
		d += GetGValue(color) * 63 / 255;
		d = d << 5;
		d += GetBValue(color) * 31 / 255;
		*((LPWORD)linePointer) = d;
		break;
	}
}

DWORD CIpodFont::GetNumUnicodeGroups()
{
	if (m_pHeader1 == NULL)
		return 0;

	return m_pHeader1->numMappingEntries;
}

void CIpodFont::GetUnicodeGroup(WORD index, LPWORD start, LPDWORD len, LPWORD offset)
{
	if (index >= GetNumUnicodeGroups())
	{
		*start = 0;
		*len = 0;
		*offset = 0;
		return;
	}

	*start = m_pUnicodeGroups[index].code;
	*len = m_pUnicodeGroups[index].length;
	*offset = m_pUnicodeGroups[index].offset;
}


void CIpodFont::SetUnicodeGroup(WORD index, WORD start, DWORD len, WORD offset)
{
	if (index >= GetNumUnicodeGroups())
		return;

	m_pUnicodeGroups[index].code = start;
	m_pUnicodeGroups[index].length = len;
	m_pUnicodeGroups[index].offset = offset;

	
	//We must sort the unicode groups array or else it might cause some chars to be represented as missing glyphs although they exist
	DWORD i,j,indexOfMin;
	IPOD_FONT_UNICODE_GROUP temp,curr;
	for (i=0;i<m_pHeader1->numMappingEntries;i++)
	{
		indexOfMin=i;
		for (j=i+1;j<m_pHeader1->numMappingEntries;j++)
		{
			curr=m_pUnicodeGroups[indexOfMin];
			temp=m_pUnicodeGroups[j];
			if (temp.code<curr.code)
				indexOfMin=j;
		}

		//swap
		temp=m_pUnicodeGroups[i];
		m_pUnicodeGroups[i]=m_pUnicodeGroups[indexOfMin];
		m_pUnicodeGroups[indexOfMin]=temp;
	}
}

WORD CIpodFont::GetNumChars()
{
	return m_pHeader2->numChars;
}

WORD CIpodFont::GetNumMetrics()
{
	return m_NumMetrics;
}

WORD CIpodFont::GetCharMapping(WORD index)
{
	return m_pCharMapping[index];
}

void CIpodFont::SetCharMapping(WORD index, WORD index2)
{
	if (index < m_pHeader2->numChars)
		m_pCharMapping[index] = index2;
}

DWORD CIpodFont::GetNumUnicodeChars()
{
	DWORD i;

	DWORD sum = 0;

	if (m_pHeader1 == NULL)
		return 0;

	for (i = 0; i < m_pHeader1->numMappingEntries; i++)
	{
		sum += m_pUnicodeGroups[i].length;
	}

	return sum;
}

WORD CIpodFont::GetUnicodeChar(WORD index, LPWORD groupIndex)
{
	WORD i;

	DWORD sum = 0;

	for (i = 0; i < m_pHeader1->numMappingEntries; i++)
	{
		if (index >= sum && index < (sum + m_pUnicodeGroups[i].length))
		{
			*groupIndex = i;
			return (WORD)(m_pUnicodeGroups[i].code + index - sum);
		}

		sum += m_pUnicodeGroups[i].length;
	}

	return 0;
}

WORD CIpodFont::GetUnicodeCharOffset(WORD c)
{
	WORD i;

	for (i = 0; i < m_pHeader1->numMappingEntries; i++)
	{
		if (c >= m_pUnicodeGroups[i].code && c < m_pUnicodeGroups[i].code + m_pUnicodeGroups[i].length)
		{
			return m_pUnicodeGroups[i].offset + (c - m_pUnicodeGroups[i].code) - 1;
		}
	}

	return 0;
}

void CIpodFont::GetCharMetrics(WORD c, LPDWORD offset1, LPDWORD offset2, SHORT *width, SHORT *ident)
{
	if (m_pHeader2 == NULL)
	{
		*offset1 = 0;
		*offset2 = 0;
		*width = 0;
		*ident = 0;
		return;
	}

	if (IsFixedWidth())
	{
		// fixed width
		*offset1 = c * m_pHeader2->size;
		*offset2 = *offset1 + m_pHeader2->size;
		*width = (SHORT)(*offset2 - *offset1); // ??
		*ident = 0; // ??
	}
	else
	{
		// variable width
		if (m_pHeader2->bitdepth == 2)
		{
			IPOD_FONT_CHARINFO_V2 *pCharInfo = (IPOD_FONT_CHARINFO_V2 *)m_pCharInfo;

			*offset1 = (WORD)pCharInfo[c].offset1;
			*offset2 = (WORD)pCharInfo[c].offset2;
			*width = pCharInfo[c].width;
			*ident = pCharInfo[c].ident;
		}
		else
		{
			*offset1 = m_pCharInfo[c].offset1;
			*offset2 = m_pCharInfo[c].offset2;
			*width = m_pCharInfo[c].width;
			*ident = m_pCharInfo[c].ident;
		}
	}
}

void CIpodFont::SetCharMetrics(WORD c, WORD offset1, WORD offset2, SHORT width, SHORT ident)
{
	if (m_pHeader2 == NULL || IsFixedWidth() || c >= m_NumMetrics)
		return;

	if (m_pHeader2->bitdepth == 2)
	{
		IPOD_FONT_CHARINFO_V2 *pCharInfo = (IPOD_FONT_CHARINFO_V2 *)m_pCharInfo;

		pCharInfo[c].offset1 = offset1;
		pCharInfo[c].offset2 = offset2;
		pCharInfo[c].width = width;
		pCharInfo[c].ident = ident;
	}
	else
	{
		m_pCharInfo[c].offset1 = offset1;
		m_pCharInfo[c].offset2 = offset2;
		m_pCharInfo[c].width = width;
		m_pCharInfo[c].ident = ident;
	}
}
