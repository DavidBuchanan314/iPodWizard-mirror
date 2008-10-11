#include "StdAfx.h"
#include ".\picture.h"
#define SWAPWORD(x) MAKEWORD(HIBYTE(x), LOBYTE(x))
#define SWAPLONG(x) MAKELONG(SWAPWORD(HIWORD(x)), SWAPWORD(LOWORD(x)))


// CPicture class

CPicture::CPicture(void)
{
	m_pHeader = NULL;
	m_pHeader2 = NULL;
	m_pFooter = NULL;
	m_pFooter2 = NULL;
	m_pFooter3=NULL;
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
	switch (m_iPictureMode)
	{
	case 3: //boot version 2
		return 1;
	case 4: //boot version 3
		return 1;
	default:
		return m_iPictureMode;
	}
}

BOOL CPicture::Read(LPBYTE lpBuffer, BOOL bScan, LPBYTE lpImageOffset)
{
	if (lpBuffer == NULL)
		return FALSE;

	switch (m_iPictureMode)
	{
	case 1:
	case 3:
	case 4:
		m_pData=NULL;

		m_pFooter = (IPOD_BOOT_PICTURE_FOOTER *)lpBuffer;

		if (m_pFooter->d1!=0)
			return FALSE;

		// validity checks
		if (m_pFooter->d1 != 0 || 
			m_pFooter->height == 0 || m_pFooter->height >= 256 || 
			m_pFooter->width == 0 || m_pFooter->width >= 256 ||
			m_pFooter->lineSizeBytes >= 512 || m_pFooter->texture_format != 0 ||
			(m_pFooter->bitDepth != 1 && m_pFooter->bitDepth != 2 && m_pFooter->bitDepth != 4  && m_pFooter->bitDepth != 8))
		{
			m_pFooter = NULL;
		}
		else
		{
			if (m_pFooter->b1==0x10 && m_pFooter->image_offset[2]<=0x01)
			{
				m_pData = lpImageOffset + (*((LPDWORD)m_pFooter->image_offset) & 0x00FFFFFF);
			}
			else if (m_pFooter->b1!=0xff)
			{
				m_pData = lpBuffer - m_pFooter->lineSizeBytes * m_pFooter->height;
			}
		}

		if (m_pData==NULL)
		{
			m_pFooter2 = (IPOD_BOOT_PICTURE_FOOTER_V2 *)lpBuffer;

			if (m_pFooter2->d1 != 0 || 
				m_pFooter2->height == 0 || m_pFooter2->height >= 256 || 
				m_pFooter2->width == 0 || m_pFooter2->width >= 256 ||
				m_pFooter2->lineSizeBytes >= 512 || m_pFooter2->lineSizeBytes==0 ||
				m_pFooter2->length==0 || *((LPDWORD)m_pFooter2->image_offset)==0)
			{
				m_pFooter2=NULL;
			}
			else
			{
				//m_pData = lpBuffer - m_pFooter2->lineSizeBytes * m_pFooter2->height;
				WORD bitdepth = m_pFooter2->lineSizeBytes * 8 / m_pFooter2->width;
				if (bitdepth==1 || bitdepth==2 || bitdepth==4 || bitdepth==16)
				{
					if (m_pFooter2->b1==0x10 && m_pFooter2->image_offset[2]<=0x01)
						m_pData = lpImageOffset + (*((LPDWORD)m_pFooter2->image_offset) & 0x00FFFFFF);
					else if (m_pFooter2->b1!=0xff)
					{
						
						m_pData = lpBuffer - m_pFooter2->lineSizeBytes * m_pFooter2->height;
					}
					m_iPictureMode = 3;
				}
				else
				{
						m_pFooter2=NULL;
				}
			}

			if (!m_pData)
			{
				m_pFooter3 = (IPOD_BOOT_PICTURE_FOOTER_V3 *)lpBuffer;

				if (m_pFooter3->d1 != 0 || 
					m_pFooter3->height == 0 || m_pFooter3->height >= 256 || 
					m_pFooter3->width == 0 || m_pFooter3->width >= 256 ||
					m_pFooter3->lineSizeBytes >= 512 ||
					m_pFooter3->w2 != 52 || m_pFooter3->height*m_pFooter3->lineSizeBytes!=m_pFooter3->length)
				{
					m_pFooter3=NULL;
				}
				else
				{
					WORD bitdepth3 = m_pFooter3->lineSizeBytes * 8 / m_pFooter3->width;
					if (bitdepth3==1 || bitdepth3==2 || bitdepth3==4 || bitdepth3==16)
					{
						m_pData = lpBuffer + sizeof(IPOD_BOOT_PICTURE_FOOTER_V3);
						m_iPictureMode = 4;
					}
					else
					{
						m_pFooter3=NULL;
					}
				}
			}
		}

		if (m_pData==NULL)
			return FALSE;
		break;
	default:
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

			//5g pictures are different header than others and nano has different color scheme
			//try other header type
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
			m_iPictureMode=2;
			break;
		}
		m_pData = lpBuffer + sizeof(IPOD_PICTURE_HEADER);
		m_iPictureMode=0;
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
	case 3:
		if (m_pFooter2 == NULL)
			return 0;

		return sizeof(IPOD_BOOT_PICTURE_FOOTER_V2) + m_pFooter2->lineSizeBytes * m_pFooter2->height;
		break;
	case 4:
		if (m_pFooter3 == NULL)
			return 0;

		return sizeof(IPOD_BOOT_PICTURE_FOOTER_V3) + m_pFooter3->lineSizeBytes * m_pFooter3->height;
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
	case 3:
		if (m_pFooter2 == NULL)
			return 0;

		return m_pFooter2->lineSizeBytes * m_pFooter2->height;
		break;
	case 4:
		if (m_pFooter3 == NULL)
			return 0;

		return m_pFooter3->lineSizeBytes * m_pFooter3->height;
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
	case 3:
		size.cx = m_pFooter2->width;
		size.cy = m_pFooter2->height;
		break;
	case 4:
		size.cx = m_pFooter3->width;
		size.cy = m_pFooter3->height;
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
	case 3:
		return m_pFooter2->lineSizeBytes * 8 / m_pFooter2->width;
		break;
	case 4:
		return m_pFooter3->lineSizeBytes * 8 / m_pFooter3->width;
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

void CPicture::Draw(CDC *pDC, CPoint point, CSize psize)
{
	DWORD x, y;

	CSize size;
	if (psize.cx<=0)
		size.cx = GetPictureSize().cx;
	else
		size.cx = psize.cx;

	if (psize.cy<=0)
		size.cy = GetPictureSize().cy;
	else
		size.cy = psize.cy;

	if (point.x<=0)
		point.x=0;
	if (point.y<=0)
		point.y=0;

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
	WORD tf;

	bitDepth = GetPictureBitDepth();

	switch (m_iPictureMode)
	{
	case 1:
		linePointer = m_pData + (m_pFooter->lineSizeBytes) * y;
		tf=0x5652;
		break;
	case 3:
		linePointer = m_pData + (m_pFooter2->lineSizeBytes) * y;
		tf=0x5652;
		break;
	case 4:
		linePointer = m_pData + (m_pFooter3->lineSizeBytes) * y;
		tf=0x5652;
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
		if (tf==0x2565)
		{
			pos1 = x * 2;
			pos2 = pos1 + 1;
		}
		else
		{
			pos2 = x * 2;
			pos1 = pos2 + 1;
		}
		if (tf!=0x1444)
		{
			d = (linePointer[pos1] & 0xF8) >> 3;
			r = d * 255 / 31;

			d = (linePointer[pos1] & 0x07);
			d = d << 3;
			d += (linePointer[pos2] & 0xE0) >> 5;
			g = d * 255 / 63;

			d = linePointer[pos2] & 0x1F;
			b = d * 255 / 31;
		}
		else
		{
			/*unsigned short rgba=SWAPWORD(*((LPWORD)linePointer));
			unsigned char alpha = ((rgba & 0x00F0) >> 4);
			r = ((rgba & 0xF000) >> 16) * alpha / 255;
			g = ((rgba & 0x0F00) >> 8) * alpha / 255;
			b = (rgba & 0x000F)  * alpha / 255;*/

			//last
			/*
			d = (linePointer[x*2+1] & 0xF0) >> 4;
			unsigned short alpha=d;
			if (alpha==0)
				alpha=15;
			//alpha bits are not treated here
			d = (linePointer[x*2] & 0xF0) >> 4;
			//r = d * 255 / 15;
			r = d * 255 / alpha;

			d = (linePointer[x*2] & 0x0F);
			//g = d * 255 / 15;
			g = d * 255 / alpha;

			d = (linePointer[x*2+1] & 0x0F);
			//b = d * 255 / 15;
			b = d * 255 / alpha;
			*/
			//last


			/*d = (linePointer[x*2+1] & 0x78) >> 3;
			r = d * 255 / 15;

			d = (linePointer[x*2+1] & 0x07) << 3;
			d += (linePointer[x*2] & 0x80) >> 7;
			g = d * 255 / 15;

			d = (linePointer[x*2] & 0x78) >> 3;
			b = d * 255 / 15;*/

			/*d = (linePointer[x*2] & 0x1E) >> 1;
			r = d * 255 / 15;

			d = (linePointer[x*2] & 0x01);
			d += (linePointer[x*2+1] & 0x07) >> 5;
			g = d * 255 / 15;

			d = (linePointer[x*2+1] & 0x1E) >> 1;
			b = d * 255 / 15;*/

			d=*((LPWORD)(&linePointer[x*2]));
			r = REDBYTEA(d);
			g = GREENBYTEA(d);
			b = BLUEBYTEA(d);
		}
		break;
	}

	return RGB(r, g, b);
}

void CPicture::SetPixel(DWORD x, DWORD y, COLORREF color)
{
	WORD d, mask;
	LPBYTE linePointer;
	WORD bitDepth;
	WORD tf;

	CSize size = GetPictureSize();

	if (x >= (DWORD)size.cx || y >= (DWORD)size.cy)
		return;

	bitDepth = GetPictureBitDepth();

	switch (m_iPictureMode)
	{
	case 1:
		linePointer = m_pData + (m_pFooter->lineSizeBytes) * y;
		tf=0x5652;
		break;
	case 3:
		linePointer = m_pData + (m_pFooter2->lineSizeBytes) * y;
		tf=0x5652;
		break;
	case 4:
		linePointer = m_pData + (m_pFooter3->lineSizeBytes) * y;
		tf=0x5652;
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
		if (tf!=0x1444)
		{
			d = GetRValue(color) * 31 / 255;
			d = d << 6;
			d += GetGValue(color) * 63 / 255;
			d = d << 5;
			d += GetBValue(color) * 31 / 255;
			if (tf==0x2565)
			{
				*((LPWORD)linePointer) = ((d << 8) | (d >> 8));
			}
			else
			{
				*((LPWORD)linePointer) = d;
			}
		}
		else
		{
			/*
			d = GetRValue(color) * 15 / 255;
			d = d << 4;
			d += GetGValue(color) * 15 / 255;
			d = d << 4;
			d += (linePointer[1] & 0xF0) >> 4;
			d = d << 4;
			d += GetBValue(color) * 15 / 255;
			
			*((LPWORD)linePointer) = ((d << 8) | (d >> 8)); //swap it since the firmware is little endian and when we assembled the value we went raw to number which is the opposite way
			*/

			d=*((LPWORD)(&linePointer));
			BYTE a=ALPHABYTEA(d);
			ARGBFORM(a,GetRValue(color),GetGValue(color),GetBValue(color));
		}
		break;
	}
}