#pragma once
#include "afxcmn.h"
#include "Firmware.h"
#include "HyperLink.h"

//iPod Detection
#include <winioctl.h>
#include <ntddscsi.h>
#define SENSE_BUFF_LEN 32       /* Arbitrary, could be larger */
#define DEF_TIMEOUT 60000       /* 60,000 millisecs == 60 seconds */

#define INQUIRY_CMD     0x12
#define INQUIRY_CMDLEN  6
#define DEV_ID_VPD  0x83
#define DEF_ALLOC_LEN 252
#define MX_ALLOC_LEN 4096

#define EBUFF_SZ 256

struct SCSI_PASS_THROUGH_WITH_BUFFERS
{
	SCSI_PASS_THROUGH spt;
	__int32 filler;
	unsigned char sense_b[SENSE_BUFF_LEN];
	unsigned char rsp_buff[MX_ALLOC_LEN + 1];
};

static DWORD GetiPodSCSIInfo(char devstring[], int pagecode, unsigned char *bresult)
{
	//memcpy(bresult, "12345678901", 12);
	//return 0;

	DWORD returned;
	HANDLE h = CreateFileA(devstring, GENERIC_READ | GENERIC_WRITE, FILE_ANY_ACCESS, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SCSI_PASS_THROUGH_DIRECT *request;
	request=(SCSI_PASS_THROUGH_DIRECT *)malloc(sizeof(SCSI_PASS_THROUGH_DIRECT)*2);
	//SCSI_PASS_THROUGH request;
	memset(request, 0, sizeof(SCSI_PASS_THROUGH_DIRECT));
	request->Length = sizeof(SCSI_PASS_THROUGH_DIRECT);
	int mx_resp_len=DEF_ALLOC_LEN;
	unsigned char rsp_buff[MX_ALLOC_LEN + 1];
	unsigned  int pg_op=pagecode;
	unsigned char inqCmdBlk[INQUIRY_CMDLEN] = {INQUIRY_CMD, 0, 0, 0, 0, 0};
	inqCmdBlk[1] |= 1;
	inqCmdBlk[2] = (unsigned char)pg_op;
	inqCmdBlk[3] = (unsigned char)((mx_resp_len >> 8) & 0xff);
	inqCmdBlk[4] = (unsigned char)(mx_resp_len & 0xff);
	
	request->TimeOutValue = 5; //5 seconds //DEF_TIMEOUT;
	request->DataIn = SCSI_IOCTL_DATA_IN;
	request->CdbLength = INQUIRY_CMDLEN;
	memcpy(request->Cdb, inqCmdBlk, sizeof(inqCmdBlk));
	request->DataBuffer = (void *)rsp_buff;
	request->DataTransferLength = sizeof(rsp_buff);
	//ULONG lent=request.spt.DataBufferOffset + request.spt.DataTransferLength;

	if (DeviceIoControl(h, IOCTL_SCSI_PASS_THROUGH_DIRECT, request, sizeof(SCSI_PASS_THROUGH_DIRECT), request, sizeof(SCSI_PASS_THROUGH_DIRECT), &returned, NULL))
	{
		CloseHandle(h);
		if (rsp_buff[1]!=pagecode)
			return -1;
		memcpy(bresult,rsp_buff,rsp_buff[3]+4);
		return 0;
	}
	else
	{
		returned=GetLastError();
		CloseHandle(h);
		return returned;
	}
}

// CHelpDialog dialog

class CHelpDialog : public CDialog
{
	DECLARE_DYNAMIC(CHelpDialog)

public:
	CHelpDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHelpDialog();

// Dialog Data
	enum { IDD = IDD_HELP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL m_bFindGen;
	LRESULT OnClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedFindGen();
	void DisableFind();
	CStringArray m_iPodNames;
	CStringArray m_iPodNamesAdvanced1;
	CStringArray m_iPodNamesAdvanced2;
	CComboBox m_iPodCombo;
	void OnCbnSelchangeiPodCombo();
	CHyperLink	m_HyperLink;
};
