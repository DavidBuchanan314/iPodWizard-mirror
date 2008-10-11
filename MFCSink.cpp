// MFCSink.cpp : implementation file
//

#include "stdafx.h"
#include "iPodWizard.h"
#include "MFCSink.h"
//#include "VBEvents_i.c"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

////////////////////////////////////////////////////////////////////////////
//	Copyright : Amit Dey 2001
//
//	Email :amitdey@joymail.com
//
//	This code may be used in compiled form in any way you desire. This
//	file may be redistributed unmodified by any means PROVIDING it is 
//	not sold for profit without the authors written consent, and 
//	providing that this notice and the authors name is included.
//
//	This file is provided 'as is' with no expressed or implied warranty.
//	The author accepts no liability if it causes any damage to your computer.
//
//	Do expect bugs.
//	Please let me know of any bugs/mods/improvements.
//	and I will try to fix/incorporate them into this file.
//	Enjoy!
//
//	Description :VBActiveX Events with VC
////////////////////////////////////////////////////////////////////////////
#endif

/////////////////////////////////////////////////////////////////////////////
// MFCSink

IMPLEMENT_DYNCREATE(MFCSink, CCmdTarget)

MFCSink::MFCSink()
{
	EnableAutomation();
}

MFCSink::~MFCSink()
{
}


void MFCSink::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(MFCSink, CCmdTarget)
	//{{AFX_MSG_MAP(MFCSink)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(MFCSink, CCmdTarget)
	//{{AFX_DISPATCH_MAP(MFCSink)
	DISP_FUNCTION_ID(MFCSink, "OnDeviceStateChanged",0x60020000,OnDeviceStateChanged, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMFCSink to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {5DD593E8-3242-4FE1-8986-D34F90560855}
static const IID IID_IMFCSink =
{ 0x5dd593e8, 0x3242, 0x4fe1, { 0x89, 0x86, 0xd3, 0x4f, 0x90, 0x56, 0x8, 0x55 } };

BEGIN_INTERFACE_MAP(MFCSink, CCmdTarget)
	INTERFACE_PART(MFCSink, IID_IItunesServiceCallback, Dispatch)
END_INTERFACE_MAP()


//static const IID IID_iPodEvents= __uuidof(_IiPodManagerEvents)

/////////////////////////////////////////////////////////////////////////////
// MFCSink message handlers

void MFCSink::OnDeviceStateChanged(long hDevice, enum EItunesDeviceClass devClass, _EDeviceStatus lNewState)
{
	// TODO: Add your dispatch handler code here
	if (lNewState != kDeviceStatusUnknown)
	{
		m_pHandler->SendMessage(WM_APP, (WPARAM)14, (LPARAM)hDevice);
	}
}