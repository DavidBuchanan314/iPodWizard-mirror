#if !defined(AFX_MFCSINK_H__49141D3B_3B6A_423D_8183_5C4396C2AED1__INCLUDED_)
#define AFX_MFCSINK_H__49141D3B_3B6A_423D_8183_5C4396C2AED1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MFCSink.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MFCSink command target

class MFCSink : public CCmdTarget
{
	DECLARE_DYNCREATE(MFCSink)

	MFCSink();           // protected constructor used by dynamic creation

// Attributes
public:
	CWnd  *m_pHandler;
	void SetHandler(CWnd *pHandler)
	{
		m_pHandler=pHandler;
	}
// Operations
public:
	virtual ~MFCSink();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MFCSink)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	

	// Generated message map functions
	//{{AFX_MSG(MFCSink)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(MFCSink)
	afx_msg void OnDeviceStateChanged(long hDevice, enum EItunesDeviceClass devClass, _EDeviceStatus lNewState);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCSINK_H__49141D3B_3B6A_423D_8183_5C4396C2AED1__INCLUDED_)
