// UpcPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "upcpic.h"
#include "UpcPropDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpcPropDlg dialog


CUpcPropDlg::CUpcPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpcPropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUpcPropDlg)
	m_nBarXDim = 0;
	m_nExpXDim = 0;
	m_szUpcCode = _T("");
	m_nBarHeight = 0;
	m_nExpHt = 0;
	m_nFontHt = 0;
	m_szFontName = _T("");
	m_bExpand = FALSE;
	m_nFontWt = 0;
	m_bInvert = FALSE;
	//}}AFX_DATA_INIT
}


void CUpcPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpcPropDlg)
	DDX_Text(pDX, IDC_BAR_X_DIM, m_nBarXDim);
	DDV_MinMaxInt(pDX, m_nBarXDim, 1, 6);
	DDX_Text(pDX, IDC_EXP_X_DIM, m_nExpXDim);
	DDV_MinMaxInt(pDX, m_nExpXDim, 4, 10);
	DDX_Text(pDX, IDC_UPC_CODE, m_szUpcCode);
	DDX_Text(pDX, IDC_BAR_HT, m_nBarHeight);
	DDV_MinMaxInt(pDX, m_nBarHeight, 20, 500);
	DDX_Text(pDX, IDC_EXP_HT, m_nExpHt);
	DDV_MinMaxInt(pDX, m_nExpHt, 20, 300);
	DDX_Text(pDX, IDC_FONT_HT, m_nFontHt);
	DDX_Text(pDX, IDC_FONT_NAME, m_szFontName);
	DDX_Check(pDX, IDC_EXPAND, m_bExpand);
	DDX_Text(pDX, IDC_FONT_WT, m_nFontWt);
	DDX_Check(pDX, IDC_INVERT, m_bInvert);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUpcPropDlg, CDialog)
	//{{AFX_MSG_MAP(CUpcPropDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpcPropDlg message handlers
