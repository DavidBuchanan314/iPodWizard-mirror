#pragma once

struct IPOD_PICTURE_HEADER
{
	BYTE b1[8];
	DWORD height;
	DWORD width;
	WORD bytes_per_row;
	WORD bitDepth;
	DWORD texture_format; //627376128 or 262144 or 1381 or 4
	DWORD blockLen;
};

struct IPOD_PICTURE_HEADER2
{
	DWORD texture_format;
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
	WORD w2;
	WORD w3;
	WORD w4;
	WORD w5;
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
	LPBYTE				m_pData;

	int			m_iPictureMode;

// operations
public:
	void		SetPictureMode(WORD iMode);
	WORD		GetPictureMode();

	BOOL		Read(LPBYTE lpBuffer, BOOL bScan = FALSE);
	DWORD		GetPictureBlockLen();
	DWORD		GetPictureRawLen();
	LPBYTE		GetData();
	void		SetData(LPBYTE buffer, DWORD size);

	CSize		GetPictureSize();
	int			GetPictureBitDepth();

	void		Draw(CDC *pDC, CPoint point);

	COLORREF	GetPixel(DWORD x, DWORD y);
	void		SetPixel(DWORD x, DWORD y, COLORREF color);
};
