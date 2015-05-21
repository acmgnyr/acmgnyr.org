// upcpicView.h : interface of the CUpcpicView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPCPICVIEW_H__EB6FF14C_A9CD_4AD7_812C_9005585D75F1__INCLUDED_)
#define AFX_UPCPICVIEW_H__EB6FF14C_A9CD_4AD7_812C_9005585D75F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CUpcpicView : public CView
{
protected: // create from serialization only
	CUpcpicView();
	DECLARE_DYNCREATE(CUpcpicView)

// Attributes
public:
	CUpcpicDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpcpicView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void MakeUPCBits(char *upcVals);
	void MakeRandUPC(char *upcVals);
	char CompCheckDigit(char *upcDigits);
	virtual ~CUpcpicView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CUpcpicView)
	afx_msg void OnEditSetUpcProperties();
	afx_msg void OnEditGenUpcTests();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DrawUPC(CDC *pDC, CRect *pRect);
	int m_BarXDim;
	int m_ExpXDim;
	int m_BarHt;
	int m_ExpHt;
	char m_upcCode[13];
	char m_upcBits[95];
	BOOL m_bHaveUPC;
	int m_FontHt;
	int m_FontWt;
	CString m_szFontName;
	CFont m_font;
	BOOL m_bFontCreated;
	BOOL m_bExpand;
	BOOL m_bInvert;

};

#ifndef _DEBUG  // debug version in upcpicView.cpp
inline CUpcpicDoc* CUpcpicView::GetDocument()
   { return (CUpcpicDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPCPICVIEW_H__EB6FF14C_A9CD_4AD7_812C_9005585D75F1__INCLUDED_)
