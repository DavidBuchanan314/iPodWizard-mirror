#pragma once

struct IPOD_FONT_HEADER1
{
	BYTE d1[10];
	WORD numChars;
	WORD numMappingEntries;
	WORD w2;
};

struct IPOD_FONT_UNICODE_GROUP
{
	WORD code;
	WORD offset;
	WORD length;
	WORD w1;
};

struct IPOD_FONT_HEADER2
{
	WORD	w1;
	char	name[64];
	WORD	w2;
	WORD	size;
	WORD	w3;
	BYTE	style;
	BYTE	bitdepth;
	WORD	numChars;
	WORD	flags; //fixed width - either 0 or 1 depending on if characters have different widths
	WORD	width; 
    WORD	line_height;
    WORD	descent; // pixels below baseline
    DWORD	w4;
    DWORD	finfo_size;
	DWORD	charMapEnd;
	DWORD	metricsEnd;
};

struct IPOD_FONT_CHARINFO
{
	WORD	offset1;
	WORD	offset2;
	SHORT	width;
	SHORT	ident;
};

struct IPOD_FONT_CHARINFO_V2
{
	DWORD	offset1;
	DWORD	offset2;
	SHORT	width;
	SHORT	ident;
};

struct IPOD_FONT_BITMAP_HEADER
{
	DWORD w1;
	WORD height;
	WORD rowSizePixels;
	WORD rowLenghBytes;
	WORD bitDepth;
	DWORD w2;
	DWORD length;
};

struct IPOD_FONT_BITMAP_HEADER_V2
{
	BYTE d1[8];
	DWORD height;
	DWORD rowSizePixels;
	DWORD rowLenghBytes;
	BYTE d2[12];
};

class CIpodFont
{
// construction
public:
	CIpodFont(void);
	~CIpodFont(void);

// implementation
private:
	IPOD_FONT_HEADER1 *			m_pHeader1;
	IPOD_FONT_HEADER2 *			m_pHeader2;

	IPOD_FONT_UNICODE_GROUP *	m_pUnicodeGroups;

	WORD *						m_pCharMapping;

	IPOD_FONT_CHARINFO *		m_pCharInfo;

	IPOD_FONT_BITMAP_HEADER 	m_BitmapHeader;

	LPBYTE						m_pBitmapData;

	WORD						m_NumMetrics;

// operations
public:
	BOOL						Read(LPBYTE lpBuffer, LPBYTE lpEnd);

	LPCTSTR						GetFontName();
	DWORD						GetFontBlockLen();
	WORD						GetFontBitDepth();
	WORD						GetFontSize();
	BOOL						IsFixedWidth();
	void						SetData(LPBYTE buffer);

	CSize						GetFontBitmapSize();
	COLORREF					GetFontPixel(CPoint point);
	void						SetFontPixel(LONG x, LONG y, COLORREF color);

	WORD						GetNumUnicodeGroups();
	void						GetUnicodeGroup(WORD index, LPWORD start, LPWORD len, LPWORD offset);
	void						SetUnicodeGroup(WORD index, WORD start, WORD len, WORD offset);
	WORD						GetNumChars();
	WORD						GetNumMetrics();
	WORD						GetCharMapping(WORD index);
	void						SetCharMapping(WORD index, WORD index2);

	WORD						GetNumUnicodeChars();
	WORD						GetUnicodeChar(WORD index, LPWORD groupIndex);
	WORD						GetUnicodeCharOffset(WORD c);
	void						GetCharMetrics(WORD c, LPWORD offset1, LPWORD offset2, SHORT *width, SHORT *ident);
	void						SetCharMetrics(WORD c, WORD offset1, WORD offset2, SHORT width, SHORT ident);

	void						Draw(CDC *pDC, CRect rect);
};
