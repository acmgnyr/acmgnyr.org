// ShowI2CDoc.h : interface of the CShowI2CDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOWI2CDOC_H__0FFD21F5_B023_4452_ADD1_05E2F7DC5F5C__INCLUDED_)
#define AFX_SHOWI2CDOC_H__0FFD21F5_B023_4452_ADD1_05E2F7DC5F5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CShowI2CDoc : public CDocument
{
protected: // create from serialization only
	CShowI2CDoc();
	DECLARE_DYNCREATE(CShowI2CDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowI2CDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShowI2CDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CShowI2CDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWI2CDOC_H__0FFD21F5_B023_4452_ADD1_05E2F7DC5F5C__INCLUDED_)
