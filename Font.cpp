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
