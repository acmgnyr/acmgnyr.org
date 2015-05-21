// upcpicView.cpp : implementation of the CUpcpicView class
//

#include "stdafx.h"
#include "upcpic.h"

#include "upcpicDoc.h"
#include "upcpicView.h"
#include "UpcPropDlg.h"
#include "GenUpcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BYTE lpattern[10] = 
{
	0x0d, 0x19, 0x13, 0x3d, 0x23, 0x31, 0x2f, 0x3b, 0x37, 0x0b
};
BYTE rpattern[10] = 
{
	0x72, 0x66, 0x6c, 0x42, 0x5c, 0x4e, 0x50, 0x44, 0x48, 0x74
};

/////////////////////////////////////////////////////////////////////////////
// CUpcpicView

IMPLEMENT_DYNCREATE(CUpcpicView, CView)

BEGIN_MESSAGE_MAP(CUpcpicView, CView)
	//{{AFX_MSG_MAP(CUpcpicView)
	ON_COMMAND(ID_EDIT_SETUPCPROPERTIES, OnEditSetUpcProperties)
	ON_COMMAND(ID_EDIT_GENUPCTESTS, OnEditGenUpcTests)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpcpicView construction/destruction

CUpcpicView::CUpcpicView()
{
	srand( (unsigned)time( NULL ) );
	m_BarXDim = 2;
	m_ExpXDim = 6;
	m_BarHt = 100;
	m_ExpHt = 60;
	m_upcCode[0] = 0;
	m_upcCode[1] = 4;
	m_upcCode[2] = 9;
	m_upcCode[3] = 7;
	m_upcCode[4] = 0;
	m_upcCode[5] = 5;
	m_upcCode[6] = 6;
	m_upcCode[7] = 8;
	m_upcCode[8] = 2;
	m_upcCode[9] = 3;
	m_upcCode[10] = 0;
	m_upcCode[11] = 2;
	m_bHaveUPC = FALSE;
	m_bFontCreated = FALSE;
	m_FontHt = 16;
	m_FontWt = 600;
	m_szFontName = "Lucida Console";
	m_bExpand = FALSE;
	m_bInvert = FALSE;
}

CUpcpicView::~CUpcpicView()
{
	if(m_bFontCreated) {
		m_font.DeleteObject();
	}
}

BOOL CUpcpicView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CUpcpicView drawing

void CUpcpicView::OnDraw(CDC* pDC)
{
	CUpcpicDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect, RGB(255,255,255));
	if(m_bHaveUPC) {
		DrawUPC(pDC, &rect);
	}
	pDC->TextOut(20, 500, "Hello");
}

/////////////////////////////////////////////////////////////////////////////
// CUpcpicView diagnostics

#ifdef _DEBUG
void CUpcpicView::AssertValid() const
{
	CView::AssertValid();
}

void CUpcpicView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUpcpicDoc* CUpcpicView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUpcpicDoc)));
	return (CUpcpicDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUpcpicView message handlers

char CUpcpicView::CompCheckDigit(char *upcDigits)
{
	int sum;
	sum = upcDigits[0] + upcDigits[2] + upcDigits[4] +
			upcDigits[6] + upcDigits[8] + upcDigits[10];
	sum = sum * 3 + upcDigits[1] + upcDigits[3] +
			upcDigits[5] + upcDigits[7] + upcDigits[9];
	sum = sum % 10;
	if(sum == 0) return 0;
	else return (char)(10 - sum);
}

void CUpcpicView::MakeRandUPC(char *upcVals)
{
	int i, val;
	for(i = 0; i < 11 ; i++)
	{
		val = rand();
		val >>= 5;
		upcVals[i] = (char)(val % 10);
	}
	upcVals[11] = CompCheckDigit(upcVals);
}

void CUpcpicView::MakeUPCBits(char *upcVals)
{
	int i, j, k;
	BYTE srcMsk, tstMsk;
	::memset(&(m_upcBits[0]), 0, 95);
	// start
	m_upcBits[0] = m_upcBits[2] = 1;
	// middle
	m_upcBits[46] = m_upcBits[48] = 1;
	// end
	m_upcBits[92] = m_upcBits[94] = 1;
	for(i = 0; i < 6; i++)
	{
		srcMsk = lpattern[upcVals[i]];
		tstMsk = 0x40;
		k = 3 + 7 * i;
		for(j = 0; j < 7 ; j++, k++) {
			if((tstMsk & srcMsk) != 0) {
				m_upcBits[k] = 1;
			}
			tstMsk >>= 1;
		}
	}
	for(i = 6; i < 12; i++)
	{
		srcMsk = rpattern[upcVals[i]];
		tstMsk = 0x40;
		k = 8 + 7 * i;
		for(j = 0; j < 7 ; j++, k++) {
			if((tstMsk & srcMsk) != 0) {
				m_upcBits[k] = 1;
			}
			tstMsk >>= 1;
		}
	}
}

void CUpcpicView::OnEditSetUpcProperties() 
{
	CUpcPropDlg dlg;
	CString code, err;
	char tempcode[12];
	int i;
	code.Empty();
	for(i = 0; i < 12; i++) code += ('0' + m_upcCode[i]);
	dlg.m_nBarXDim = m_BarXDim;
	dlg.m_nExpXDim = m_ExpXDim;
	dlg.m_nBarHeight = m_BarHt;
	dlg.m_nExpHt = m_ExpHt;
	dlg.m_szUpcCode = code;
	dlg.m_nFontHt = m_FontHt;
	dlg.m_nFontWt = m_FontWt;
	dlg.m_bExpand = m_bExpand;
	dlg.m_bInvert = m_bInvert;
	dlg.m_szFontName = m_szFontName;
	m_bHaveUPC = FALSE;
	if(m_bFontCreated) {
		m_font.DeleteObject();
		m_bFontCreated = FALSE;
	}

	if(dlg.DoModal() == IDOK) {
		code = dlg.m_szUpcCode;
		if(code.GetLength() < 11) {
			::AfxMessageBox("UPC Code must have at least 11 digits");
			return;
		}
		for(i = 0; i < 11; i++) {
			if((code[i] < '0') || (code[i] > '9')) {
				err.Format("code char %d (%c (0x%x) is not a digit",
					i+1, code[i], code[i]);
				::AfxMessageBox(err);
				return;
			}
			tempcode[i] = code[i] - '0';
		}
		m_FontWt = dlg.m_nFontWt;
		m_bExpand = dlg.m_bExpand;
		m_bInvert = dlg.m_bInvert;
		m_FontHt = dlg.m_nFontHt;
		m_szFontName = dlg.m_szFontName;
		if(m_font.CreateFont(m_FontHt, 0, 0, 0, m_FontWt,
			0, 0, 0, ANSI_CHARSET,
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DRAFT_QUALITY,
			DEFAULT_PITCH|FF_DONTCARE, m_szFontName) == 0)
		{
			::AfxMessageBox("Create Font Failure");
			return;
		}
		m_bFontCreated = TRUE;
		m_BarXDim = dlg.m_nBarXDim;
		m_ExpXDim = dlg.m_nExpXDim;
		m_BarHt = dlg.m_nBarHeight;
		m_ExpHt = dlg.m_nExpHt;

		for(i = 0; i < 11; i++) {
			m_upcCode[i] = tempcode[i];
		}
		m_upcCode[11] = CompCheckDigit(&(m_upcCode[0]));
		MakeUPCBits(&(m_upcCode[0]));
		m_bHaveUPC = TRUE;
		Invalidate();
		UpdateWindow();
	}
	
}

void CUpcpicView::DrawUPC(CDC *pDC, CRect *pRect)
{
	int upcTop, x, bw, cw, i, cx, cextra, ht, htextra, ind, step;
	int y1, y2;
	char label[2];
	CString lab;
	CFont *pOldFont;
	if(m_bExpand) {
		bw = m_ExpXDim;
		ht = m_ExpHt;
		htextra = 0;
	} else {
		bw = m_BarXDim;
		ht = m_BarHt;
		htextra = 5*bw;
	}
	if((bw*95) > pRect->Width()) bw = (pRect->Width())/95;
	if(bw < 1) return;
	upcTop = (pRect->Height() - ht)/2;
	if(upcTop < 0) upcTop = 0;
	y1 = upcTop - 10;
	y2 = upcTop - 2;
	x = (pRect->Width() - (95*bw))/2;
	if(m_bExpand) {
		cw = bw;
	}else {
		cw = (42*bw)/5;
	}
	cextra = (42*bw)%5;
	pOldFont = pDC->SelectObject(&m_font);
	label[1] = 0;
	upcTop += ht + 1;
	pDC->SetTextAlign(TA_CENTER | TA_TOP);
	if(m_bExpand) {
		cx = x + cw/2;
		if(m_bInvert) {
			ind = 94;
			step = -1;
		} else {
			ind = 0;
			step = 1;
		}
		for(i = 0; i < 95; i++, ind += step) {
			label[0] = (m_upcBits[ind] + '0');
			lab = label;
			pDC->TextOut(cx, upcTop, lab);
			cx += cw;
		}
	} else {
		label[0] = (m_upcCode[0] + '0');
		lab = label;
		cx = (pRect->Width() - (95*bw))/2 - cw/2;
		pDC->TextOut(cx, upcTop, lab);
		cx = (pRect->Width() - (89*bw))/2 + (cw+cextra)/2;
		for(i = 1; i < 6; i++){
			label[0] = (m_upcCode[i] + '0');
			lab = label;
			pDC->TextOut(cx, upcTop, lab);
			cx += cw;
		}
		cx = (pRect->Width() + (5*bw))/2 + (cw+cextra)/2;
		for(i = 6; i < 11; i++){
			label[0] = (m_upcCode[i] + '0');
			lab = label;
			pDC->TextOut(cx, upcTop, lab);
			cx += cw;
		}
		label[0] = (m_upcCode[11] + '0');
		lab = label;
		cx = (pRect->Width() + (95*bw))/2 + cw/2;
		pDC->TextOut(cx, upcTop, lab);
	}
	pDC->SelectObject(pOldFont);

	upcTop = (pRect->Height() - ht)/2;
	if(upcTop < 0) upcTop = 0;
	if(m_bExpand) {
		if(m_bInvert) {
			ind = 94;
			step = -1;
		} else {
			ind = 0;
			step = 1;
		}
	} else {
		ind = 0;
		step = 1;
	}
	for(i = 0; i < 3; i++, ind += step) {
		if(m_upcBits[ind] != 0) {
			pDC->FillSolidRect(x, upcTop, bw, ht + htextra, RGB(0,0,0));
		}
		x += bw;
	}
	for(i = 3; i < 45; i++, ind += step) {
		if(m_upcBits[ind] != 0) {
			pDC->FillSolidRect(x, upcTop, bw, ht, RGB(0,0,0));
		}
		x += bw;
	}
	for(i = 45; i < 50; i++, ind += step) {
		if(m_upcBits[ind] != 0) {
			pDC->FillSolidRect(x, upcTop, bw, ht + htextra, RGB(0,0,0));
		}
		x += bw;
	}
	for(i = 50; i < 92; i++, ind += step) {
		if(m_upcBits[ind] != 0) {
			pDC->FillSolidRect(x, upcTop, bw, ht, RGB(0,0,0));
		}
		x += bw;
	}
	for(i = 92; i < 95; i++, ind += step) {
		if(m_upcBits[ind] != 0) {
			pDC->FillSolidRect(x, upcTop, bw, ht + htextra, RGB(0,0,0));
		}
		x += bw;
	}
	x = (pRect->Width() - (95*bw))/2;
	pDC->MoveTo(x, y1);
	pDC->LineTo(x, y2);
	x += 3*bw;
	for(i = 0; i < 6; i++) {
		pDC->MoveTo(x, y1);
		pDC->LineTo(x, y2);
		x += 7*bw;
	}
	pDC->MoveTo(x, y1);
	pDC->LineTo(x, y2);
	x += 5*bw;
	for(i = 0; i < 6; i++) {
		pDC->MoveTo(x, y1);
		pDC->LineTo(x, y2);
		x += 7*bw;
	}
	pDC->MoveTo(x, y1);
	pDC->LineTo(x, y2);
	x += 3*bw;
	pDC->MoveTo(x, y1);
	pDC->LineTo(x, y2);
	x += 3*bw;
}

void CUpcpicView::OnEditGenUpcTests() 
{
	CGenUpcDlg dlg;

	dlg.DoModal();
	
}
