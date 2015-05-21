// eqlzrfigView.h : interface of the CEqlzrfigView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EQLZRFIGVIEW_H__9F142440_1C0B_47E6_9F20_188334F33CF3__INCLUDED_)
#define AFX_EQLZRFIGVIEW_H__9F142440_1C0B_47E6_9F20_188334F33CF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CEqlzrfigView : public CView
{
protected: // create from serialization only
	CEqlzrfigView();
	DECLARE_DYNCREATE(CEqlzrfigView)

// Attributes
public:
	CEqlzrfigDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEqlzrfigView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void MakePoints(RECT *pRect);
	void FindMaxMin();
	int CheckEqSolns(double a, double b, double c, double *pSoln);
	int FindEqualizers();
	virtual ~CEqlzrfigView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEqlzrfigView)
	afx_msg void OnEditSetTriangle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_nScale;
	CString m_szVertexStr;
	CString m_szLabel;
	double m_verts[6];
	double m_equalizers[6][4];
	CPoint m_vertPts[3];
	CPoint m_eqEndPts[12];
	CPoint m_labelPt;
	double m_MaxX;
	double m_MinX;
	double m_MaxY;
	double m_MinY;
	int m_nEq;
};

#ifndef _DEBUG  // debug version in eqlzrfigView.cpp
inline CEqlzrfigDoc* CEqlzrfigView::GetDocument()
   { return (CEqlzrfigDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQLZRFIGVIEW_H__9F142440_1C0B_47E6_9F20_188334F33CF3__INCLUDED_)
