// eqlzrfigDoc.h : interface of the CEqlzrfigDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EQLZRFIGDOC_H__5F9BE987_F9B4_41A8_A4DB_A4707320AB0E__INCLUDED_)
#define AFX_EQLZRFIGDOC_H__5F9BE987_F9B4_41A8_A4DB_A4707320AB0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CEqlzrfigDoc : public CDocument
{
protected: // create from serialization only
	CEqlzrfigDoc();
	DECLARE_DYNCREATE(CEqlzrfigDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEqlzrfigDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEqlzrfigDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEqlzrfigDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQLZRFIGDOC_H__5F9BE987_F9B4_41A8_A4DB_A4707320AB0E__INCLUDED_)
