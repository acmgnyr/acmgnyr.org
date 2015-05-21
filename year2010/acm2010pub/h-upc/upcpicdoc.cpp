// upcpicDoc.cpp : implementation of the CUpcpicDoc class
//

#include "stdafx.h"
#include "upcpic.h"

#include "upcpicDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpcpicDoc

IMPLEMENT_DYNCREATE(CUpcpicDoc, CDocument)

BEGIN_MESSAGE_MAP(CUpcpicDoc, CDocument)
	//{{AFX_MSG_MAP(CUpcpicDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpcpicDoc construction/destruction

CUpcpicDoc::CUpcpicDoc()
{
	// TODO: add one-time construction code here

}

CUpcpicDoc::~CUpcpicDoc()
{
}

BOOL CUpcpicDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CUpcpicDoc serialization

void CUpcpicDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CUpcpicDoc diagnostics

#ifdef _DEBUG
void CUpcpicDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUpcpicDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUpcpicDoc commands
