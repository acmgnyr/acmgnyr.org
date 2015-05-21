// eqlzrfigDoc.cpp : implementation of the CEqlzrfigDoc class
//

#include "stdafx.h"
#include "eqlzrfig.h"

#include "eqlzrfigDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEqlzrfigDoc

IMPLEMENT_DYNCREATE(CEqlzrfigDoc, CDocument)

BEGIN_MESSAGE_MAP(CEqlzrfigDoc, CDocument)
	//{{AFX_MSG_MAP(CEqlzrfigDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEqlzrfigDoc construction/destruction

CEqlzrfigDoc::CEqlzrfigDoc()
{
	// TODO: add one-time construction code here

}

CEqlzrfigDoc::~CEqlzrfigDoc()
{
}

BOOL CEqlzrfigDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CEqlzrfigDoc serialization

void CEqlzrfigDoc::Serialize(CArchive& ar)
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
// CEqlzrfigDoc diagnostics

#ifdef _DEBUG
void CEqlzrfigDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEqlzrfigDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEqlzrfigDoc commands
