#include "StdAfx.h"
#include ".\picture.h"


// CPicture class

CPicture::CPicture(void)
{
	m_pHeader = NULL;
	m_pHeader2 = NULL;
	m_pFooter = NULL;
	m_pData = NULL;

	m_iPictureMode = 0;
}

CPicture::~CPicture(void)
{
}

// operations

void CPicture::SetPictureMode(WORD iMode)
{
	m_iPictureMode = iMode;
}

WORD CPicture::GetPictureMode()
{
	return m_iPictureMode;
}

BOOL CPicture::Read(LPBYTE lpBuffer, BOOL bScan)
{
	if (lpBuffer == NULL)
		return FALSE;

	switch (m_iPictureMode)
	{
	case 1:
		m_pFooter = (IPOD_BOOT_PICTURE_FOOTER *)lpBuffer;

		// validity checks
		if (m_pFooter->d1 != 0 || 
			m_pFooter->height == 0 || m_pFooter->height >= 96 || 
			m_pFooter->width == 0 || m_pFooter->width >= 128 ||
			m_pFooter->lineSizeBytes >= 32 || m_pFooter->texture_format != 0 || m_pFooter->w3 != 0 ||
			(m_pFooter->bitDepth != 1 && m_pFooter->bitDepth != 2 && m_pFooter->bitDepth != 4))
		{
			m_pFooter = NULL;
			return FALSE;
		}

		m_pData = lpBuffer - m_pFooter->lineSizeBytes * m_pFooter->height;
		break;
	case 0:
		m_pHeader = (IPOD_PICTURE_HEADER *)lpBuffer;
		// validity checks
		if (m_pHeader->width > 511 || m_pHeader->height > 511 ||
			m_pHeader->width == 0 || m_pHeader->height == 0 ||
			m_pHeader->blockLen < (m_pHeader->height * m_pHeader->width * m_pHeader->bitDepth / 8) ||
			(m_pHeader->bitDepth != 1 && m_pHeader->bitDepth != 2 &&
			m_pHeader->bitDepth != 4 && m_pHeader->bitDepth != 16) || *((__int64*)m_pHeader->b1)!=0)
		{
			// bad image
			m_pHeader = NULL;
			return FALSE;
		}
		m_pData = lpBuffer + sizeof(IPOD_PICTURE_HEADER);
		break;
	case 2:
		m_pHeader2 = (IPOD_PICTURE_HEADER2 *)lpBuffer;
		// validity checks
		if (m_pHeader2->texture_format == 0 || m_pHeader2->width > 511 || m_pHeader2->height > 511 ||
			m_pHeader2->width == 0 || m_pHeader2->height == 0 ||
			m_pHeader2->blockLen < (m_pHeader2->height * m_pHeader2->width * m_pHeader2->bitDepth / 8) ||
			(m_pHeader2->bitDepth != 1 && m_pHeader2->bitDepth != 2 &&
			m_pHeader2->bitDepth != 4 && m_pHeader2->bitDepth != 16) || *((__int64*)m_pHeader2->b1)!=0)
		{
			// bad image
			m_pHeader2 = NULL;
			return FALSE;
		}

		m_pData = lpBuffer + sizeof(IPOD_PICTURE_HEADER2);
		break;
	}

	return TRUE;
}

DWORD CPicture::GetPictureBlockLen()
{
	switch (m_iPictureMode)
	{
	case 1:
		if (m_pFooter == NULL)
			return 0;

		return sizeof(IPOD_BOOT_PICTURE_FOOTER) + m_pFooter->lineSizeBytes * m_pFooter->height;
		break;
	case 0:
		if (m_pHeader == NULL)
			return 0;

		return sizeof(IPOD_PICTURE_HEADER) + m_pHeader->blockLen;
		break;
	case 2:
		if (m_pHeader2 == NULL)
			return 0;

		return sizeof(IPOD_PICTURE_HEADER2) + m_pHeader2->blockLen;
		break;
	}
	return 0;
}

LPBYTE CPicture::GetData()
{
	return m_pData;
}

void CPicture::SetData(LPBYTE buffer, DWORD size)
{
	memcpy(m_pData, buffer, size);
}

DWORD CPicture::GetPictureRawLen()
{
	switch (m_iPictureMode)
	{
	case 1:
		if (m_pFooter == NULL)
			return 0;

		return m_pFooter->lineSizeBytes * m_pFooter->height;
		break;
	case 0:
		if (m_pHeader == NULL)
			return 0;

		return m_pHeader->blockLen;
		break;
	case 2:
		if (m_pHeader2 == NULL)
			return 0;

		return m_pHeader2->blockLen;
		break;
	}
	return 0;
}

CSize CPicture::GetPictureSize()
{
	CSize size;
	
	switch (m_iPictureMode)
	{
	case 1:
		size.cx = m_pFooter->width;
		size.cy = m_pFooter->height;
		break;
	case 0:
		size.cx = m_pHeader->width;
		size.cy = m_pHeader->height;
		break;
	case 2:
		size.cx = m_pHeader2->width;
		size.cy = m_pHeader2->height;
		break;
	}

	return size;
}

int CPicture::GetPictureBitDepth()
{
	switch (m_iPictureMode)
	{
	case 1:
		return m_pFooter->bitDepth;
		break;
	case 0:
		return m_pHeader->bitDepth;
		break;
	case 2:
		return m_pHeader2->bitDepth;
		break;
	}
	return 0;
}
//

void CPicture::Draw(CDC *pDC, CPoint point)
{
	DWORD x, y;

	CSize size = GetPictureSize();

	for (y = 0; y < (DWORD)size.cy; y++)
	{
		for (x = 0; x < (DWORD)size.cx; x++)
		{
			pDC->SetPixel(x + point.x, y + point.y, GetPixel(x, y));
		}
	}
}

COLORREF CPicture::GetPixel(DWORD x, DWORD y)
{
	WORD bitDepth;

	LPBYTE linePointer;
	BYTE r, g, b;
	WORD d;
	DWORD tf;

	switch (m_iPictureMode)
	{
	case 1:
		bitDepth = m_pFooter->bitDepth;
		linePointer = m_pData + (m_pFooter->lineSizeBytes) * y;
		break;
	case 0:
		bitDepth = m_pHeader->bitDepth;
		linePointer = m_pData + m_pHeader->bytes_per_row * y;
		tf = m_pHeader->texture_format;
		break;
	case 2:
		bitDepth = m_pHeader2->bitDepth;
		linePointer = m_pData + m_pHeader2->bytes_per_row * y;
		tf = m_pHeader2->texture_format;
		break;
	}

	switch (bitDepth)
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
		DWORD pos1,pos2;
		if (tf==0x25650000)
		{
			pos1 = x * 2;
			pos2 = pos1 + 1;
		}
		else
		{
			pos2 = x * 2;
			pos1 = pos2 + 1;
		}
		d = (linePointer[pos1] & 0xF8) >> 3;
		r = d * 255 / 31;

		d = (linePointer[pos1] & 0x07);
		d = d << 3;
		d += (linePointer[pos2] & 0xE0) >> 5;
		g = d * 255 / 63;

		d = linePointer[pos2] & 0x1F;
		b = d * 255 / 31;
		break;
	}

	return RGB(r, g, b);
}

void CPicture::SetPixel(DWORD x, DWORD y, COLORREF color)
{
	WORD d, mask;
	LPBYTE linePointer;
	WORD bitDepth;
	DWORD tf;

	CSize size = GetPictureSize();

	if (x >= (DWORD)size.cx || y >= (DWORD)size.cy)
		return;

	bitDepth = GetPictureBitDepth();

	switch (m_iPictureMode)
	{
	case 1:
		linePointer = m_pData + (m_pFooter->lineSizeBytes) * y;
		break;
	case 0:
		linePointer = m_pData + m_pHeader->bytes_per_row * y;
		tf = m_pHeader->texture_format;
		break;
	case 2:
		linePointer = m_pData + m_pHeader2->bytes_per_row * y;
		tf = m_pHeader2->texture_format;
		break;
	}

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
		if (tf==0x25650000)
		{
			*((LPWORD)linePointer) = ((d << 8) | (d >> 8));
		}
		else
		{
			*((LPWORD)linePointer) = d;
		}
		break;
	}
}