#ifndef AFX_THEMEWND_H__6D1C0A60_F616_43C6_9850_F3C0F052C0DB__INCLUDED_
#define AFX_THEMEWND_H__6D1C0A60_F616_43C6_9850_F3C0F052C0DB__INCLUDED_

#pragma once

#include "Picture.h"
#include "EditorDialog.h"
#include "Firmware.h"
#include "RDragDropFiles.h"
#include "Font.h"
#include "OTFFontParser.h"
#include "Color.h"

#include <algorithm>
#include <iterator>

class EditorDialog;

typedef struct _PicRecord PicRecord;

struct _PicRecord {
	int id;
	RECT rct;
	CPoint inner;
};

typedef vector<PicRecord *> PicTable;

typedef struct _TextRecord TextRecord;

struct _TextRecord {
	int lang;
	int id;
	CPoint inner;
	int fontid;
};

typedef vector<TextRecord *> TextTable;

// CThemeWnd

class CThemeWnd : public CWnd
{
	DECLARE_DYNAMIC(CThemeWnd)

public:
	CThemeWnd();
	virtual ~CThemeWnd();

protected:
	DECLARE_MESSAGE_MAP()
	BOOL m_bDrawCombined;
	ViewRecord *m_ViewRec;
	MenuRecord *m_MenuRec;
	void DrawImages(CDC *pDC, ViewRecord *view_rec, vector<short> *id_list);
	bool IsNotOnList(vector<short> *id_list, short id)
	{
		for (size_t i=0;i<(*id_list).size();i++)
		{
			if ((*id_list).at(i)==id)
				return false;
		}
		return true;
	}

public:
	BOOL RegisterWindowClass();
	afx_msg void OnPaint();
	CFirmware *	m_pFirmware;
	CEditorDialog * m_pEditorDialog;
	void SetData(int view_id, int menu_id);
	void SetRes(int w, int h);
	void DrawCombined(BOOL bDraw);
	void Reset(BOOL bTextOnly=FALSE);
	CPoint m_Origin;
	CSize m_ResSize;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void SetFirmware(CFirmware *pFirmware, CEditorDialog *pEditorDialog);
	PicTable PicRecList;
	TextTable TextRecList;
	PicTable PicResRecList;
	TextTable TextResRecList;
};

#endif
