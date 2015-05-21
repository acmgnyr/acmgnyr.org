// upcpicDoc.h : interface of the CUpcpicDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPCPICDOC_H__2804A38C_B7BC_4DE3_86AD_E96FC13F26B9__INCLUDED_)
#define AFX_UPCPICDOC_H__2804A38C_B7BC_4DE3_86AD_E96FC13F26B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CUpcpicDoc : public CDocument
{
protected: // create from serialization only
	CUpcpicDoc();
	DECLARE_DYNCREATE(CUpcpicDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpcpicDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUpcpicDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CUpcpicDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPCPICDOC_H__2804A38C_B7BC_4DE3_86AD_E96FC13F26B9__INCLUDED_)
