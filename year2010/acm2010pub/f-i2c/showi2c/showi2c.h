// ShowI2C.h : main header file for the SHOWI2C application
//

#if !defined(AFX_SHOWI2C_H__1EF6BB53_E21B_4699_9D14_1E0C27E41FF2__INCLUDED_)
#define AFX_SHOWI2C_H__1EF6BB53_E21B_4699_9D14_1E0C27E41FF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CShowI2CApp:
// See ShowI2C.cpp for the implementation of this class
//

class CShowI2CApp : public CWinApp
{
public:
	CShowI2CApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowI2CApp)
	public:
	virtual BOOL InitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CShowI2CApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWI2C_H__1EF6BB53_E21B_4699_9D14_1E0C27E41FF2__INCLUDED_)
