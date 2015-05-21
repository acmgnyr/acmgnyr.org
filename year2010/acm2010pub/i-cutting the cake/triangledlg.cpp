// TriangleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eqlzrfig.h"
#include "TriangleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTriangleDlg dialog


CTriangleDlg::CTriangleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTriangleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTriangleDlg)
	m_nScale = 0;
	m_szVertexStr = _T("");
	m_szLabel = _T("");
	//}}AFX_DATA_INIT
}


void CTriangleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTriangleDlg)
	DDX_Text(pDX, IDC_SCALE, m_nScale);
	DDX_Text(pDX, IDC_VERTEX_STR, m_szVertexStr);
	DDX_Text(pDX, IDC_LABEL, m_szLabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTriangleDlg, CDialog)
	//{{AFX_MSG_MAP(CTriangleDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTriangleDlg message handlers
