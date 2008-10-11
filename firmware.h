// firmware.h

#pragma once

#include <Afxtempl.h>
#include <vector>

using namespace std;

struct IPOD_PARTITION_HEADER
{
	char	type[4];
	char	id[4]; // unique image id 
	DWORD	flash;
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
    //__int32 id;
	short id;
	WORD unk1;
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

#define IPOD_ADVANCED_USER

#define NUM_ELEM(x) ((int)(sizeof(x) / sizeof(x[0])))

struct resource_type
{
    char type[5];               // internal type like 'paMB'
    char name[5];               // easier to read name like 'bmap'
    void *decode;     // function to take raw resource to usable form
    void *encode;     // go back to raw form
    void *info;         // print info
    char *extension;            // default extension for decoded resources
};

// don't know what all these actually mean yet, so some just reverse the type for the name
static struct resource_type RESOURCE_TYPES[] = 
{
    {" rtS", "text", NULL, NULL, NULL, "txt"}, //text
    {"paMB", "bmap", NULL, NULL, NULL, "pgm"}, // picture
    {"uneM", "menu", NULL, NULL, NULL, "prop"}, /* menus */
    {"epyT", "type", NULL, NULL, NULL, "prop"}, /* font selector */

    // don't fully understand these files yet, this is just for my testing
#ifdef IPOD_ADVANCED_USER
    {"weiV", "view", NULL, NULL, NULL, "prop"}, /* layout */
    {"dmCT", "tcmd", NULL, NULL, NULL, "prop"}, /* command table */

    {"enoN", "none", NULL, NULL, NULL, NULL}, /* resource linker */
    {"tStS", "stst", NULL, NULL, NULL, NULL},

    // should not enable this, otherwise can get screwed up on 
    // some firmwares finding incorrect resource block
#if 0
    // the following are found elsewhere in the firmware (not dumped from resource block)
    {"xaMV", "vmax", NULL, NULL, NULL, NULL},
    {"SgsM", "msgs", NULL, NULL, NULL, NULL},
    {"tuAk", "kaut", NULL, NULL, NULL, NULL},
    {"H&Pk", "kp&h", NULL, NULL, NULL, NULL},
    {" pUk", "kup ", NULL, NULL, NULL, NULL},
    {"emit", "time", NULL, NULL, NULL, NULL},
    {"dpUw", "wupd", NULL, NULL, NULL, NULL},
    {"ylpt", "tply", NULL, NULL, NULL, NULL},
    {"ltnC", "cntl", NULL, NULL, NULL, NULL}, //
    {"nwDk", "kdwn", NULL, NULL, NULL, NULL},
    {"leeW", "weel", NULL, NULL, NULL, NULL},
    {"GSlc", "clsg", NULL, NULL, NULL, NULL},
    {"GSlp", "plsg", NULL, NULL, NULL, NULL},
    {"GSon", "nosg", NULL, NULL, NULL, NULL},
#endif

#else
    {"weiV", "view", NULL, NULL, NULL, NULL},
    {"dmCT", "tcmd", NULL, NULL, NULL, NULL},
#endif
    {"mTDL", "ldtm", NULL, NULL, NULL, NULL}, /* language something */
    {"stiB", "bits", NULL, NULL, NULL, NULL},

    // these are pseudo resources. These were just chosen to not match existing ones
    {"boot", "boot", NULL, NULL, NULL, "pgm"}, /* for pictures in the bootloader */
};

static struct resource_type unknown_resource_type = { "????", "????", NULL, NULL, NULL, NULL };

static struct resource_type *get_resource_type(char *type)
{
    int i;
    for (i = 0; i < NUM_ELEM(RESOURCE_TYPES); i++)
    {
        if ((memcmp(RESOURCE_TYPES[i].type, type, 4) == 0) ||
            (memcmp(RESOURCE_TYPES[i].name, type, 4) == 0))
            return &RESOURCE_TYPES[i];
    }
    //warning("Unknown type '%.4s'", type);
    return &unknown_resource_type;
}

typedef enum property_type
{
    U_ID,
    U_TYPE,
    U_DECIMAL,
    U_HEX,
} property_type;

#define MAX_PROPERTY_LENGTH 16
struct property
{
    property_type type;
    char name[MAX_PROPERTY_LENGTH];
    int data;
    char *comment;
};

#define MAX_PROPERTIES 32
struct properties
{
    int num_props;
    struct property props[MAX_PROPERTIES];
};

// list of elements, each has a set of properties
#define MAX_ELEMENTS 900
struct property_list
{
    int num_elems;
    struct properties elems[MAX_ELEMENTS];
};

struct property_field
{
    property_type type;
    char *name;
    int size;                   // number of double-bytes, 1 or 2
    char *help;
};

struct resource_type_type_elem
{
	short fontID;
	unsigned short z1;
	int size;
	unsigned int color;
	unsigned char style; //bold or regular?
	unsigned char alignment;
	unsigned short z2;
};

struct resource_type_menu_elem
{
	int size;
	unsigned int z1;
	unsigned int z2;
	unsigned int cmd;
	short submenuID;
	unsigned short z3;
	short textID;
	short z4;
	unsigned short z5;
	unsigned short textID3;
	unsigned int z6;
	short textID2;
	unsigned short z7;
	int typeID;
};

// Description of the other resources to be read into property files
static struct property_field resource_type_type[] =
{
    { U_ID, "fontID", 1, "ID of font" },    /* rsrc ID of text containing font name */
    { U_HEX, "z1",  1, NULL},
    { U_DECIMAL, "size",  2, "Font size"},
    { U_HEX, "fontColor",  2, NULL}, //color in RGB of font
    { U_HEX, "textAlign",  2, NULL},           /* font text align on screen */
};

static struct property_field resource_type_menu[] =
{
    { U_DECIMAL, "size", 2, "Size of this struct. Always 36." },
    { U_HEX, "z1", 2, NULL },           /* 32,34,36,38 */
    { U_HEX, "z2", 2, NULL }, /* 7434,7438,9218,9342,9398. quite different across firmwares */
    { U_HEX, "cmd", 2, NULL }, /* varied numbers around 7000-9000. maybe command to execute on press. byte offset into a table? they are all separated by dwords */
    { U_ID, "submenuID", 1, "ID of submenu" },
    { U_HEX, "z3", 1, "Zero" },
    { U_ID, "textID", 1, "text displayed by menu" },
    { U_HEX, "z4", 1, NULL }, /* numbers increase from 775 within one menu. same across firmwares */
    { U_HEX, "z5", 2, NULL },           /* mostly unique numbers */
    { U_HEX, "z6", 2, NULL }, /* similar numbers across very different menus */
    { U_ID, "textID2", 1, "Always equal to textID? unknown function" },
    { U_HEX, "z7", 1, "Zero" },
    { U_ID, "typeID", 2, "Zero indicates to use default type (font)" },
};


struct resource_view_temp
{
    unsigned int a1; //usauly 0
    unsigned short a2;
    unsigned short a3;
    int val;                 /* width, height */
};
/*
#define FIX_RESOURCE_TYPE_VIEW_TEMP(x) do { flip32(&(x)->a1, &(x)->a4, NULL); \
    flip16(&(x)->a2, &(x)->a3, NULL);\
} while (0)
*/
typedef union
{
	//for 5g
	unsigned int color1; //color1 to start gradient from
	struct {
	//for nano:
	unsigned short string_id;
	unsigned short font_id;
	} nano;
}view_var1;
typedef union
{
int temp;
}view_var2;

struct resource_type_view_elem
{
    unsigned int size;              /* size of rest of resource */
    unsigned int z0;
    char type[4];               /* resource type */
    short id;                 /* resource ID */
	unsigned short unk1;
    int z1;
    int z2;

    // appear to be bit fields
    unsigned short z3;
    unsigned short z4;

	//Aligning stuff:
    // text - left value is length?
	// text - bottom value is color?
    // height and width for bitmap
	struct resource_view_temp top;
    struct resource_view_temp left;
	struct resource_view_temp bottom; //height
	struct resource_view_temp right; //width

	view_var1 vvar1;
	unsigned int color1;
	unsigned int color2;
   /* // some number of extra values
	//for 5g
	unsigned int color1; //color1 to start gradient from
	unsigned int color2; //color2 to end gradient to
	//for nano:
	unsigned short string_id;
	unsigned short font_id;
	unsigned int unk2;
	*/
	short z7;
	//only good for rtS, otherwise it is 0
	short font_id; //appears to be font id of some kind for text
};

/*
#define FIX_RESOURCE_TYPE_VIEW(x) do { flip32(&(x)->size, &(x)->z0, &(x)->id, &(x)->z1, &(x)->z2, NULL); \
    flip16(&(x)->z3, &(x)->z4, NULL);\
} while (0)
*/
struct resource_type_view
{
    int num_elem;
    struct resource_type_view_elem *elements;
};

struct resource_type_tcmd_elem
{
	//this is of unknown size
	short id;
	unsigned short z0;
	unsigned short z1;
	unsigned short num_subs;
	unsigned short subcmd_len;
	unsigned char *subcmd; //sub commands set, subcmd_len bytes
	unsigned short z2;
};

struct resource_type_tcmd
{
    unsigned int *size_unk;
    unsigned short *z1;                /* zero */
    unsigned short *header_size; // multiply by 4
    int *elems;             /* num_elems elements */
    unsigned short *num_offsets;             /* number of offsets, each offset points to a location inside tcmd buffer which includes a command set */
	unsigned short *t_len; //number of t's
    unsigned short *tlist;    /* unknown numbers - lengths? - size of t_len */
	//do word align, meaning that if t_len is iizugi, skip +2
    unsigned int *offsets;             /* offsets - num_offsets of them. first one is 0. last offset is not used, in order to know the size of each element just substract offset[n+1]-offset[n] */
    vector <struct resource_type_tcmd_elem *>elements;
    //groups of bytes each starting with 0x51000000
};

/*
struct resource_type_tcmd_elem
{
	//this is of unknown size
	int id;
	unsigned short z0;
	unsigned short num_subs;
	unsigned short subcmd_len;
	unsigned char *subcmd; //sub commands set, subcmd_len bytes
	unsigned short z1;
};

struct resource_type_tcmd
{
    unsigned int size_unk;
    unsigned short z1;                // zero
    unsigned short header_size; // multiply by 4
    int *elems;             // num_elems elements
    unsigned short num_offsets;             // number of offsets, each offset points to a location inside tcmd buffer which includes a command set
	unsigned short t_len; //number of t's
    unsigned short *tlist;    // unknown numbers - lengths? - size of t_len
	//do word align, meaning that if t_len is iizugi, skip +2
    unsigned int *offsets;             // offsets - num_offsets of them. first one is 0. last offset is not used, in order to know the size of each element just substract offset[n+1]-offset[n]
    struct resource_type_tcmd_elem *elements;
    //groups of bytes each starting with 0x51000000
}; */

typedef struct _ViewRecord  ViewRecord;
typedef struct _ViewElem  ViewElem;

struct _ViewElem {
	struct resource_type_view_elem *element;
	ViewRecord 	*next;
};


struct _ViewRecord {
	int num_elems;
	short id;
	vector<ViewElem *> ViewElemList;
};

typedef vector<ViewRecord *> ViewTable;

typedef struct _MenuRecord MenuRecord;
typedef struct _MenuElem MenuElem;

struct _MenuElem
{
	struct resource_type_menu_elem *element;
};

struct _MenuRecord {
	int num_elems;
	int id;
	vector<MenuElem *> MenuElemList;
};

typedef vector<MenuRecord *> MenuTable;

typedef struct _TypeRecord TypeRecord;

struct _TypeRecord {
	int id;
	struct resource_type_type_elem *element;
};

typedef vector<TypeRecord *> TypeTable;

typedef struct _TCMDRecord TCMDRecord;

struct _TCMDRecord {
	int id;
	struct resource_type_tcmd element;
};

typedef vector<TCMDRecord *> TCMDTable;



//#define theApp.PARTITION_MAP_ADDRESS			0x4200
//#define theApp.PARTITION_MAP_ADDRESS			0x4800

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
	vector<DWORD> m_PicturesID;
	CPtrArray	m_Fonts;
	CPtrArray	m_OTFFonts;
	CPtrArray	m_Strings;
	CArray<IPOD_LANG*,IPOD_LANG*> m_iPodLang;
	ViewTable ViewRecList;
	MenuTable MenuRecList;
	TypeTable TypeRecList;
	TCMDTable TCMDRecList;

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
	DWORD	GetFirmwareVersion();
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
	DWORD	GetPictureIndexFromID(DWORD id);

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

	ViewTable *GetViewList();
	MenuTable *GetMenuList();
	TypeTable *GetTypeList();
	TCMDTable *GetTCMDList();
	bool FindResourceID(int i, int id);

	void	SetData(DWORD offset, LPBYTE data, DWORD size);
	void	FixChecksum(DWORD imageID);

	//Volume hack
	int isCapped(void);
	DWORD unCap(void);
	DWORD reCap(void);

	//AUPD decryption
	void DecryptAUPD(int partition);
	LPBYTE GetAUPDBuffer();
	void ResetImageAdresses();
};

static int offset[8]={0x5,0x25,0x6f,0x69,0x15,0x4d,0x40,0x34};
static unsigned short SWAPWORD_L(unsigned short x)
{
		return MAKEWORD(HIBYTE(x), LOBYTE(x));
};
static unsigned long SWAPLONG_L(unsigned long x)
{
	return MAKELONG(SWAPWORD_L(HIWORD(x)), SWAPWORD_L(LOWORD(x)));
};

class SecurityBlock {
public:
	unsigned char *data;
	int key;
	bool fileIsProtected;

	SecurityBlock(unsigned char *rawData);

	void GetSecurityBlockKey(unsigned char *rawData);

	static int readWord(unsigned char *buffer,int pos);

	bool testMarker(int marker);
}; //class


class FileDecrypt {
private:
	int *cipherBox;
	int *cipherKeyArray;

	unsigned char *doRC4Decrypt(unsigned char *rawData, unsigned long data_len);

	void doRC4keyExpansion(unsigned char *thisKey);

public:
	unsigned char *decryptFirmwareFile(unsigned char *fileData, int key, unsigned long data_len);
};