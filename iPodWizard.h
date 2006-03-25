// iPodWizard.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CiPodWizardApp:
// See iPodWizard.cpp for the implementation of this class
//

//iPod Detection
#define BLOCK_SIZE 512
#define FIRMWARE_START 32256
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <winioctl.h>

//Firmware
#define FIRMWARE_RESOURCE_TYPE	TEXT("FIRM")

class CiPodWizardApp : public CWinApp
{
public:
	CiPodWizardApp();

// Overrides
	public:
	virtual BOOL InitInstance();
	//Extra stuff:
	CString m_iPodDRV;
	CString m_IniPath;
	CString m_DeviceSel;

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CiPodWizardApp theApp;