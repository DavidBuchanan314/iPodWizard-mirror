// firmware.h

#pragma once

#include <Afxtempl.h>

struct IPOD_PARTITION_HEADER
{
	char	type[4];
	char	id[4]; // unique image id 
	DWORD	unused1;
	DWORD	devOffset; // byte offset of start of image
	DWORD	len; // length in bytes of the image
	DWORD	addr; // this is referred to as 'addr' but purpose still unknown
	DWORD	entryOffset; // code start location relative to devOffset
	DWORD	checksum; // checksum for entire image
	DWORD	version; // version of image
	DWORD	loadAddr; //memory location to load image (*) 
};

struct PARTITION_INFO
{
	BOOL	bCanModify;
	DWORD	checksumStartAddress;
};

struct IPOD_STRING
{
	__int32 id;
	DWORD offset;
	DWORD size;
	LPDWORD string_off; //byte pointer to DWORD of resource containing string
	LPDWORD size_off; //byte pointer to DWORD of resource containing size
};

struct IPOD_LANG
{
	CArray<IPOD_STRING*,IPOD_STRING*> iPodString;
	DWORD rsdata_end;
	DWORD rsdata_start;
};

struct RESOURCE_HEADER
{
	DWORD	z1; //always 0x03
	DWORD	length;
	DWORD	num_sections;
};

struct RESOURCE_SECTION
{
    char type[4];               // four letters identifying type of resource e.g. ' rtS' 'paMB'
    DWORD num_items;
    DWORD t1;                // either 0 or 1
    DWORD start;             // offset to beginning of block containing this resource type
};

struct RESOURCE
{
    __int32 id;
    DWORD offset;            // from beginning of resource data at end of all sections
    DWORD size;              // size of data (e.g. picture size including header)
};

struct RESOURCE_BLOCK
{
	RESOURCE_HEADER header;
	RESOURCE_SECTION *sections;
	RESOURCE *resources;
};

#define KNOWN_RESOURCE(x)	(!strncmp(x," rtS", 4) || !strncmp(x,"paMB", 4) || !strncmp(x,"uneM", 4) || !strncmp(x,"epyT", 4) || !strncmp(x,"weiV", 4) || !strncmp(x,"dmCT", 4) || !strncmp(x,"enoN", 4) || !strncmp(x,"mTDL", 4) || !strncmp(x,"stiB", 4) || !strncmp(x,"boot", 4))
//unknown: found in the firmware inside resources data and not as resource sections
// !strncmp(x,"xaMV", 4) || !strncmp(x,"SgsM", 4) || !strncmp(x,"tuAk", 4) || !strncmp(x,"H&Pk", 4) || !strncmp(x," pUk", 4) || !strncmp(x,"emit", 4) || !strncmp(x,"dpUw", 4) || !strncmp(x,"ylpt", 4) || !strncmp(x,"ltnC", 4) || !strncmp(x,"nwDk", 4) || !strncmp(x,"leeW", 4) || !strncmp(x,"GSlc", 4) || !strncmp(x,"GSlp", 4) || !strncmp(x,"GSon", 4))
#define DWORD_ALIGN(x) ((((x)+3)/4)*4)

/*typedef enum
{
    ID,
    TYPE,
    DECIMAL,
    HEX,
} PROPERTY_TYPE;

#define MAX_PROPERTY_LENGTH 16
struct PROPERTY
{
    PROPERTY_TYPE type;
    char name[MAX_PROPERTY_LENGTH];
    int data;
    char *comment;
};

#define MAX_PROPERTIES 32
struct PROPERTIES
{
    int num_props;
    PROPERTY props[MAX_PROPERTIES];
};

// list of elements, each has a set of properties
#define MAX_ELEMENTS 90
struct PROPERTY_LIST
{
	int num_elems;
	PROPERTIES elems[MAX_ELEMENTS];
};

struct PROPERTY_FIELD
{
    PROPERTY_TYPE type;
    char *name;
    int size;                   // number of double-bytes, 1 or 2
    char *help;
};

// Description of the other resources to be read into property files
static PROPERTY_FIELD resource_type_type[] =
{
    { ID, "fontID", 1, "ID of font" },    // rsrc ID of text containing font name
    { HEX, "z1",  1, NULL},
    { DECIMAL, "size",  2, "Font size"},
    { HEX, "z2",  2, NULL},
    { HEX, "z3",  2, NULL},           // must be font description of some sort
};*/


#define PARTITION_MAP_ADDRESS			0x4200

class CFirmware
{
// constructors
public:
	CFirmware();
	~CFirmware();

// implementation
private:
	LPBYTE	m_Buffer;
	DWORD	m_BufferSize;
	CString	m_Name;

	void	Reset();

	CPtrArray	m_Pictures;
	CDWordArray m_PicturesType;
	CDWordArray	m_PicturesID;
	CPtrArray	m_Fonts;
	CPtrArray	m_OTFFonts;
	CPtrArray	m_Strings;
	CArray<IPOD_LANG*,IPOD_LANG*> m_iPodLang;

	int		m_NumPartitions;
	IPOD_PARTITION_HEADER *	m_pPartitions;

	PARTITION_INFO	m_PartitionInfo[10];

	//Change string stuff
	DWORD	m_SLangOffset;
	BOOL	m_StringOrderFixed;

// operations
public:
	BOOL	SetFirmware(LPCTSTR name, LPBYTE buffer, DWORD size);
	void	ScanFirmware(CWnd *pWnd);
	LPBYTE	GetFirmwareBuffer();
	DWORD	GetFirmwareSize();
	CString	GetName();

	DWORD	ComputeChecksum(int index = 0);
	void	SyncChecksum();

	int		GetNumPartitions();
	void	GetPartitionChecksums(int index, LPDWORD pChecksum1, LPDWORD pChecksum2);

	BOOL	CanWrite(DWORD offset, DWORD len);

	BOOL	CanWritePicture(DWORD index);

	DWORD	GetNumPictures();
	LPBYTE	GetPicture(DWORD index);
	DWORD	GetPictureType(DWORD index);
	DWORD	GetPictureID(DWORD index);

	DWORD	GetNumFonts();
	LPBYTE	GetFont(DWORD index);

	DWORD	GetNumOTFFonts();
	LPBYTE	GetOTFFont(DWORD index);

	BOOL	CanWriteFont(DWORD index);
	BOOL	CanWriteOTFFont(DWORD index);

	DWORD	GetNumStrings();
	LPCTSTR	GetString(DWORD index);

	DWORD	GetNumLangs();
	DWORD	GetNumLangStrings(DWORD index);
	LPCTSTR GetLangString(DWORD index, DWORD index2);
	DWORD	GetLangStringSize(DWORD index, DWORD index2);
	void	AddLangString(DWORD index, DWORD index2, LPBYTE str, DWORD size);
	BOOL	ResetLangBlock(DWORD index, DWORD totallen);
	BOOL	SetLangString(DWORD index, DWORD index2, LPBYTE str, DWORD size);
	__int32	GetLangStringID(DWORD index, DWORD index2);

	void	SetData(DWORD offset, LPBYTE data, DWORD size);

	//Volume hack
	int isCapped(void);
	DWORD unCap(void);
	DWORD reCap(void);
};

