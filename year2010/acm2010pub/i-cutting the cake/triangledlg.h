#if !defined(AFX_TRIANGLEDLG_H__9931D7AE_A74B_4EA7_B639_C546146045AF__INCLUDED_)
#define AFX_TRIANGLEDLG_H__9931D7AE_A74B_4EA7_B639_C546146045AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TriangleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTriangleDlg dialog

class CTriangleDlg : public CDialog
{
// Construction
public:
	CTriangleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTriangleDlg)
	enum { IDD = IDD_TRIANGLE_DIALOG };
	int		m_nScale;
	CString	m_szVertexStr;
	CString	m_szLabel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTriangleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTriangleDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRIANGLEDLG_H__9931D7AE_A74B_4EA7_B639_C546146045AF__INCLUDED_)
