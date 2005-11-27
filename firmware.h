// firmware.h

#pragma once

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
	CPtrArray	m_Fonts;
	CPtrArray	m_Strings;

	int		m_NumPartitions;
	IPOD_PARTITION_HEADER *	m_pPartitions;

	PARTITION_INFO	m_PartitionInfo[10];

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

	DWORD	GetNumFonts();
	LPBYTE	GetFont(DWORD index);

	BOOL	CanWriteFont(DWORD index);

	DWORD	GetNumStrings();
	LPCTSTR	GetString(DWORD index);

	int PType();

	//Volume hack
	int seekChaine (void);
	int isCapped(void);
	DWORD unCap(void);
	DWORD reCap(void);
};
