// ShowI2CDoc.cpp : implementation of the CShowI2CDoc class
//

#include "stdafx.h"
#include "ShowI2C.h"

#include "ShowI2CDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowI2CDoc

IMPLEMENT_DYNCREATE(CShowI2CDoc, CDocument)

BEGIN_MESSAGE_MAP(CShowI2CDoc, CDocument)
	//{{AFX_MSG_MAP(CShowI2CDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowI2CDoc construction/destruction

CShowI2CDoc::CShowI2CDoc()
{
	// TODO: add one-time construction code here

}

CShowI2CDoc::~CShowI2CDoc()
{
}

BOOL CShowI2CDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CShowI2CDoc serialization

void CShowI2CDoc::Serialize(CArchive& ar)
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
// CShowI2CDoc diagnostics

#ifdef _DEBUG
void CShowI2CDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CShowI2CDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShowI2CDoc commands
