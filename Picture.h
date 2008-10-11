#pragma once

#define ARGBFORM(a,r,g,b) (WORD)(((( a & 0xf0 ) << 8)  |(( r & 0xf0 ) << 4)  |(( g & 0xf0 )     )  |(( b & 0xf0 ) >> 4))) 


#define RGBFORM(r,g,b)		(WORD)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3)) 
  
#define REDBYTE(rgb)        (((WORD)rgb & 0xF800) >> 8) 
#define GREENBYTE(rgb)      (((WORD)rgb & 0x07E0) >> 3) 
#define BLUEBYTE(rgb)       (((WORD)rgb & 0x001F) << 3) 
#define REDBYTEA(rgba)      (((WORD)rgba & 0x0f00) >> 4) 
#define GREENBYTEA(rgba)    (((WORD)rgba & 0x00f0)) 
#define BLUEBYTEA(rgba)     (((WORD)rgba & 0x000f) << 4) 
#define ALPHABYTEA(rgba)    (((WORD)rgba & 0xf000) >> 8)


struct IPOD_PICTURE_HEADER
{
	BYTE b1[8];
	DWORD height;
	DWORD width;
	WORD bytes_per_row;
	WORD bitDepth;
	WORD d5;
	WORD texture_format; //0x2565 or 0x4000 or 0x4414 or 0x565 or 0x4
	DWORD blockLen;
};

struct IPOD_PICTURE_HEADER2
{
	WORD texture_format;
	WORD d5;
	WORD bytes_per_row;
	WORD bitDepth;
	BYTE b1[8];
	DWORD height;
	DWORD width;
	DWORD blockLen;
};

struct IPOD_BOOT_PICTURE_FOOTER
{
	DWORD d1;
	WORD height;
	WORD width;
	WORD lineSizeBytes;
	WORD bitDepth;
	DWORD texture_format;
	DWORD length;
	BYTE image_offset[3];
	BYTE b1;
};

struct IPOD_BOOT_PICTURE_FOOTER_V2
{
	//footer 2bit
	//DWORD d4;
	//IPOD_BOOT_PICTURE_FOOTER f1;
	//footer 16bit
	DWORD d1; //must be 0
	WORD height;
	WORD width;
	WORD lineSizeBytes; //bytes per line
	WORD w1;
	DWORD d2;
	DWORD length; //length of whole picture in bytes
	BYTE image_offset[3];
	BYTE b1;
};

struct IPOD_BOOT_PICTURE_FOOTER_V3
{
	//footer 16bit
	DWORD d1; //must be 0
	WORD height;
	WORD width;
	WORD lineSizeBytes; //bytes per line
	WORD w2; 
	DWORD d2;
	DWORD length; //length of whole picture in bytes
	DWORD d3;
};

///

class CPicture
{
// constructors
public:
	CPicture(void);
	~CPicture(void);

// implementation
private:
	IPOD_PICTURE_HEADER	*m_pHeader;
	IPOD_PICTURE_HEADER2	*m_pHeader2;
	IPOD_BOOT_PICTURE_FOOTER *m_pFooter;
	IPOD_BOOT_PICTURE_FOOTER_V2 *m_pFooter2;
	IPOD_BOOT_PICTURE_FOOTER_V3 *m_pFooter3;
	LPBYTE				m_pData;

	int			m_iPictureMode;

// operations
public:
	void		SetPictureMode(WORD iMode);
	WORD		GetPictureMode();

	BOOL		Read(LPBYTE lpBuffer, BOOL bScan = FALSE, LPBYTE lpImageOffset = 0);
	DWORD		GetPictureBlockLen();
	DWORD		GetPictureRawLen();
	LPBYTE		GetData();
	void		SetData(LPBYTE buffer, DWORD size);

	CSize		GetPictureSize();
	int			GetPictureBitDepth();

	void		Draw(CDC *pDC, CPoint point, CSize psize = CSize(0, 0));

	COLORREF	GetPixel(DWORD x, DWORD y);
	void		SetPixel(DWORD x, DWORD y, COLORREF color);
};
