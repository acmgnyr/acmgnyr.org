// ShowI2CView.h : interface of the CShowI2CView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOWI2CVIEW_H__E0D9B504_22D3_4B84_B2C3_DCAA892BF4AD__INCLUDED_)
#define AFX_SHOWI2CVIEW_H__E0D9B504_22D3_4B84_B2C3_DCAA892BF4AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CShowI2CView : public CFormView
{
protected: // create from serialization only
	CShowI2CView();
	DECLARE_DYNCREATE(CShowI2CView)

public:
	//{{AFX_DATA(CShowI2CView)
	enum { IDD = IDD_SHOWI2C_FORM };
	CSpinButtonCtrl	m_spinSkip;
	CSliderCtrl	m_slZoom;
	CComboBox	m_cbForms;
	int		m_nWaveForm;
	int		m_nZoom;
	int		m_nSkip;
	//}}AFX_DATA

// Attributes
public:
	CShowI2CDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowI2CView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	void OpenFile(LPCTSTR szFile);
	virtual ~CShowI2CView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CShowI2CView)
	afx_msg void OnSelchangeComboWave();
	afx_msg void OnReleasedcaptureSliderZoom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChangeEditSkip();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CStringArray m_arForms;
	BOOL m_bInit;
};

#ifndef _DEBUG  // debug version in ShowI2CView.cpp
inline CShowI2CDoc* CShowI2CView::GetDocument()
   { return (CShowI2CDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWI2CVIEW_H__E0D9B504_22D3_4B84_B2C3_DCAA892BF4AD__INCLUDED_)
