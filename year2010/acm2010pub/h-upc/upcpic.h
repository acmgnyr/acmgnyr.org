// upcpic.h : main header file for the UPCPIC application
//

#if !defined(AFX_UPCPIC_H__58505938_A478_4D77_92E5_DD051D8FFDA4__INCLUDED_)
#define AFX_UPCPIC_H__58505938_A478_4D77_92E5_DD051D8FFDA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CUpcpicApp:
// See upcpic.cpp for the implementation of this class
//

class CUpcpicApp : public CWinApp
{
public:
	CUpcpicApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpcpicApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CUpcpicApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPCPIC_H__58505938_A478_4D77_92E5_DD051D8FFDA4__INCLUDED_)
