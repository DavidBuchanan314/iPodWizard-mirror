#pragma once

//static const char FONT_MAGIC[] = { 0x04, 0x00, 0x21, 0x00 };
//it tunes out that the 0x04 is not always that value (there was 0x05 too)

struct IPOD_FONT_HEADER1
{
	DWORD z0; //char magic[4];
	WORD first_char;
	WORD last_char;
	WORD z1; //seems to be always 0x01
	WORD numChars;
	DWORD numMappingEntries;
};

struct IPOD_FONT_UNICODE_GROUP
{
	WORD code;
	WORD offset;
	DWORD length;
};

struct IPOD_FONT_HEADER2
{
	WORD	w1; //seems to be always 0x04
	char	name[66];
	DWORD	size;
	BYTE	style;
	BYTE	bitdepth;
	WORD	numChars;
	WORD	flags; //fixed width - either 0 or 1 depending on if characters have different widths
	WORD	width; 
    WORD	line_height;
    WORD	descent; // pixels below baseline
    DWORD	w4; //0 or 1
    DWORD	finfo_size; //might be size of this struct
	DWORD	charMapEnd; // from beginning of this struct to end of char map
	DWORD	metricsEnd; // from beginning of this struct to end of char metrics
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
	DWORD						GetFontSize();
	LPCTSTR						GetFontStyle();
	BOOL						IsFixedWidth();
	void						SetData(LPBYTE buffer);
	LPBYTE						GetMetricData();
	DWORD						GetMetricDataLen();
	void						SetMetricData(LPBYTE lpBuf);

	CSize						GetFontBitmapSize();
	COLORREF					GetFontPixel(CPoint point);
	void						SetFontPixel(LONG x, LONG y, COLORREF color);

	DWORD						GetNumUnicodeGroups();
	void						GetUnicodeGroup(WORD index, LPWORD start, LPDWORD len, LPWORD offset);
	void						SetUnicodeGroup(WORD index, WORD start, DWORD len, WORD offset);
	WORD						GetNumChars();
	WORD						GetNumMetrics();
	WORD						GetCharMapping(WORD index);
	void						SetCharMapping(WORD index, WORD index2);

	DWORD						GetNumUnicodeChars();
	WORD						GetUnicodeChar(WORD index, LPWORD groupIndex);
	WORD						GetUnicodeCharOffset(WORD c);
	void						GetCharMetrics(WORD c, LPDWORD offset1, LPDWORD offset2, SHORT *width, SHORT *ident);
	void						SetCharMetrics(WORD c, WORD offset1, WORD offset2, SHORT width, SHORT ident);
};
