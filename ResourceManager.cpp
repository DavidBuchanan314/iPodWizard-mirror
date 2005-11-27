#include "StdAfx.h"
#include ".\resourcemanager.h"

CResourceManager::CResourceManager(void)
{
	m_hModule = NULL;

	m_ResourceBuffer = NULL;
	m_ResourceSize = 0;
	m_hGlobal = NULL;
}

CResourceManager::~CResourceManager(void)
{
	Reset();
}

// implementation

BOOL CALLBACK CResourceManager::enumProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam)
{
	((CResourceManager *)lParam)->AddResourceName(lpszName);

	return TRUE;
}

void CResourceManager::AddResourceName(LPCTSTR lpszName)
{
	m_ResourceNames.Add(lpszName);
}

// operations

void CResourceManager::Reset()
{
	if (m_hModule != NULL)
	{
		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}

	m_ResourceBuffer = NULL;
	m_ResourceSize = 0;
	m_hGlobal = NULL;
}

BOOL CResourceManager::Open(LPCTSTR lpszFilename)
{
	Reset();

	m_Filename.Format(lpszFilename);

	m_hModule = LoadLibrary(m_Filename);
	if (m_hModule == NULL)
		return FALSE;

	return TRUE;
}

BOOL CResourceManager::Enum(LPCTSTR type)
{
	if (m_hModule == NULL)
		return FALSE;

	m_ResourceNames.RemoveAll();

	return EnumResourceNames(m_hModule, type, enumProc, (LONG_PTR)this);
}

INT_PTR CResourceManager::GetNumResources()
{
	return m_ResourceNames.GetCount();
}

CString CResourceManager::GetResourceName(INT_PTR nIndex)
{
	return m_ResourceNames.GetAt(nIndex);
}

BOOL CResourceManager::LoadResource(LPCTSTR type, LPCTSTR name)
{
	if (name == NULL || m_hModule == NULL)
		return FALSE;

	//If it's not our first load then free the old buffer...
	//We don't want the user complaining about iPW using too much memory!
	if (m_ResourceBuffer!=NULL)
	{
		if (m_hGlobal!=NULL)
			::FreeResource(m_hGlobal);
		m_hGlobal=NULL;
		if (Open(m_Filename)==FALSE)
			return FALSE;
	}

	HRSRC hRsrc = FindResource(m_hModule, name, type);
	if (hRsrc == NULL)
		return FALSE;

	m_ResourceSize = SizeofResource(m_hModule, hRsrc);
	if (m_ResourceSize == NULL)
		return FALSE;

	HGLOBAL hGlobal = ::LoadResource(m_hModule, hRsrc);
	if (hGlobal == NULL)
		return FALSE;
	m_hGlobal=hGlobal;

	m_ResourceBuffer = (LPBYTE)LockResource(hGlobal);
	if (m_ResourceBuffer == NULL)
		return FALSE;

	return TRUE;
}

LPBYTE CResourceManager::GetResourceBuffer()
{
	return m_ResourceBuffer;
}

DWORD CResourceManager::GetResourceSize()
{
	return m_ResourceSize;
}

BOOL CResourceManager::WriteResource(LPCTSTR lpszFilename, LPCTSTR lpType, LPCTSTR lpName, LPVOID lpData, DWORD cbData)
{
	BOOL bRes = TRUE;

	Reset();

	HANDLE hUpdate = BeginUpdateResource(lpszFilename, FALSE);
	if (hUpdate == NULL)
		return FALSE;

	bRes = UpdateResource(hUpdate, lpType, lpName, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), lpData, cbData);

	bRes = bRes & EndUpdateResource(hUpdate, !bRes);

	return bRes;
}

//Updater manager
BOOL CResourceManager::UpdChangeResources(LPCTSTR lpszFilename, LPCTSTR lpType, CPtrArray *lpBuffers, CStringArray *lpNames, CDWordArray *lpSizes)
{
	BOOL bRes = TRUE;

	Reset();

	HANDLE hUpdate = BeginUpdateResource(lpszFilename, FALSE);
	if (hUpdate == NULL)
		return FALSE;

	for (INT_PTR i=0;i<lpBuffers->GetCount();i++)
	{
		bRes = UpdateResource(hUpdate, lpType, lpNames->GetAt(i), MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), lpBuffers->GetAt(i), lpSizes->GetAt(i));
		if (bRes==FALSE)
		{
			EndUpdateResource(hUpdate, TRUE);
			return FALSE;
		}
	}

	bRes = bRes & EndUpdateResource(hUpdate, !bRes);

	return bRes;
}

DWORD CResourceManager::UpdGetResourceSize(LPCTSTR lpType, LPCTSTR lpName)
{
	if (lpName == NULL || m_hModule == NULL)
		return -1;

	HRSRC hRsrc = FindResource(m_hModule, lpName, lpType);
	if (hRsrc == NULL)
		return FALSE;

	DWORD size;
	size = SizeofResource(m_hModule, hRsrc);
	if (size == NULL)
		return -1;

	return size;
}

LPBYTE CResourceManager::UpdLoadResource(LPCTSTR type, LPCTSTR name)
{
	if (name == NULL || m_hModule == NULL)
		return NULL;

	HRSRC hRsrc = FindResource(m_hModule, name, type);
	if (hRsrc == NULL)
		return NULL;

	HGLOBAL hGlobal = ::LoadResource(m_hModule, hRsrc);
	if (hGlobal == NULL)
		return NULL;

	LPBYTE buffer;
	buffer=(LPBYTE)LockResource(hGlobal);
	return buffer;
}

void CResourceManager::UpdFreeResource()
{
	if (m_hGlobal!=NULL)
		::FreeResource(m_hGlobal);
	m_hGlobal=NULL;
	Open(m_Filename);
}
