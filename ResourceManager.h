#pragma once

class CResourceManager
{
// constructors
public:
	CResourceManager(void);
	~CResourceManager(void);

// implementation
private:
	HMODULE	m_hModule;

	static BOOL CALLBACK	enumProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam);
	void	AddResourceName(LPCTSTR lpszName);

	CStringArray	m_ResourceNames;

	LPBYTE	m_ResourceBuffer;
	DWORD	m_ResourceSize;
	HGLOBAL	m_hGlobal;

// operations
public:
	void	Reset();
	CString m_Filename;
	BOOL	Open(LPCTSTR lpszFilename);
	BOOL	Enum(LPCTSTR type);
	INT_PTR	GetNumResources();
	CString	GetResourceName(INT_PTR nIndex);
	BOOL	LoadResource(LPCTSTR type, LPCTSTR name);
	LPBYTE	GetResourceBuffer();
	DWORD	GetResourceSize();
	BOOL	WriteResource(LPCTSTR lpszFilename, LPCTSTR lpType, LPCTSTR lpName, LPVOID lpData, DWORD cbData);
	
	DWORD	UpdGetResourceSize(LPCTSTR lpType, LPCTSTR lpName);
	BOOL	UpdChangeResources(LPCTSTR lpszFilename, LPCTSTR lpType, CPtrArray *lpBuffers, CStringArray *lpNames, CDWordArray *lpSizes);
	LPBYTE	UpdLoadResource(LPCTSTR type, LPCTSTR name);
	void	UpdFreeResource();
};
