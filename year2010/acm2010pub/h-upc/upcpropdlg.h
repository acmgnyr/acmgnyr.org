#if !defined(AFX_UPCPROPDLG_H__080C5D86_72B5_4CC4_8667_24D0A5E77EFC__INCLUDED_)
#define AFX_UPCPROPDLG_H__080C5D86_72B5_4CC4_8667_24D0A5E77EFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UpcPropDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUpcPropDlg dialog

class CUpcPropDlg : public CDialog
{
// Construction
public:
	CUpcPropDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUpcPropDlg)
	enum { IDD = IDD_UPC_PROPS_DLG };
	int		m_nBarXDim;
	int		m_nExpXDim;
	CString	m_szUpcCode;
	int		m_nBarHeight;
	int		m_nExpHt;
	int		m_nFontHt;
	CString	m_szFontName;
	BOOL	m_bExpand;
	int		m_nFontWt;
	BOOL	m_bInvert;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpcPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUpcPropDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPCPROPDLG_H__080C5D86_72B5_4CC4_8667_24D0A5E77EFC__INCLUDED_)
