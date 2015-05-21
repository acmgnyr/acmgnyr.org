// eqlzrfigView.cpp : implementation of the CEqlzrfigView class
//

#include "stdafx.h"
#include "eqlzrfig.h"

#include "eqlzrfigDoc.h"
#include "eqlzrfigView.h"
#include "TriangleDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPS (.00001)

/////////////////////////////////////////////////////////////////////////////
// CEqlzrfigView

IMPLEMENT_DYNCREATE(CEqlzrfigView, CView)

BEGIN_MESSAGE_MAP(CEqlzrfigView, CView)
	//{{AFX_MSG_MAP(CEqlzrfigView)
	ON_COMMAND(ID_EDIT_SETTRANGLE, OnEditSetTriangle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEqlzrfigView construction/destruction

CEqlzrfigView::CEqlzrfigView()
{
	m_nScale = 100;
	m_szVertexStr = "0 0 0 3 4 0";
	m_nEq = 0;
	m_szLabel = "A";
}

CEqlzrfigView::~CEqlzrfigView()
{
}

BOOL CEqlzrfigView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CEqlzrfigView drawing

void CEqlzrfigView::OnDraw(CDC* pDC)
{
	CEqlzrfigDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	RECT rect;
	int i;
	CPen triPen(PS_SOLID, 3, RGB(0,0,0)), eqPen(PS_SOLID, 1, RGB(0,0,0)), *pOldPen;
	GetClientRect(&rect);
	if(m_nEq != 0) {
		GetClientRect(&rect);
		pDC->FillSolidRect(&rect, RGB(255,255,255));
		MakePoints(&rect);
		pDC->TextOut(m_labelPt.x, m_labelPt.y, m_szLabel);
		pOldPen = pDC->SelectObject(&triPen);
		pDC->MoveTo(m_vertPts[0]);
		pDC->LineTo(m_vertPts[1]);
		pDC->LineTo(m_vertPts[2]);
		pDC->LineTo(m_vertPts[0]);
		pDC->SelectObject(&eqPen);
		for(i = 0; i < m_nEq ; i++) {
			pDC->MoveTo(m_eqEndPts[2*i]);
			pDC->LineTo(m_eqEndPts[2*i+1]);
		}
		pDC->SelectObject(pOldPen);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEqlzrfigView diagnostics

#ifdef _DEBUG
void CEqlzrfigView::AssertValid() const
{
	CView::AssertValid();
}

void CEqlzrfigView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEqlzrfigDoc* CEqlzrfigView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEqlzrfigDoc)));
	return (CEqlzrfigDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEqlzrfigView message handlers

void CEqlzrfigView::OnEditSetTriangle() 
{
	CTriangleDlg dlg;
	double verts[6];
	int i;
	dlg.m_nScale = m_nScale;
	dlg.m_szVertexStr = m_szVertexStr;
	dlg.m_szLabel = m_szLabel;
	if(dlg.DoModal() == IDOK) {
		if(sscanf(dlg.m_szVertexStr, "%lf %lf %lf %lf %lf %lf",
			&(verts[0]), &(verts[1]), &(verts[2]), 
			&(verts[3]), &(verts[4]), &(verts[5])) != 6) {
			::AfxMessageBox("Parse error in vertex string");
			return;
		}
		// valid verts, process
		if(dlg.m_nScale <= 0) {
			m_nScale = 1;
		} else {
			m_nScale = dlg.m_nScale;
		}
		m_szLabel = dlg.m_szLabel;
		m_szVertexStr = dlg.m_szVertexStr;
		for(i = 0; i < 6; i++) {
			m_verts[i] = verts[i];
		}
		FindEqualizers();
		if(m_nEq != 0) {
			FindMaxMin();

			Invalidate();
			UpdateWindow();
		}
	}
	
}

int CEqlzrfigView::FindEqualizers()
{
	double vAB[2], vAC[2], vBC[2], soln[4];
	double a, b, c, x0, y0, x1, y1;
	int ret;
	m_nEq = 0;
	vAB[0] = m_verts[2] - m_verts[0];
	vAB[1] = m_verts[3] - m_verts[1];
	vAC[0] = m_verts[4] - m_verts[0];
	vAC[1] = m_verts[5] - m_verts[1];
	vBC[0] = m_verts[4] - m_verts[2];
	vBC[1] = m_verts[5] - m_verts[3];
	a = sqrt(vBC[0]*vBC[0] + vBC[1]*vBC[1]);
	b = sqrt(vAC[0]*vAC[0] + vAC[1]*vAC[1]);
	c = sqrt(vAB[0]*vAB[0] + vAB[1]*vAB[1]);
	if((ret = CheckEqSolns(a, b, c, &(soln[0]))) > 0) { // soln crossin AB and AC
		x0 = m_verts[0] + vAB[0]*soln[0];
		y0 = m_verts[1] + vAB[1]*soln[0];
		x1 = m_verts[0] + vAC[0]*soln[1];
		y1 = m_verts[1] + vAC[1]*soln[1];
		m_equalizers[m_nEq][0] = 2*x0 - x1;
		m_equalizers[m_nEq][1] = 2*y0 - y1;
		m_equalizers[m_nEq][2] = 2*x1 - x0;
		m_equalizers[m_nEq][3] = 2*y1 - y0;
		m_nEq++;
		if(ret == 2) {
			x0 = m_verts[0] + vAB[0]*soln[2];
			y0 = m_verts[1] + vAB[1]*soln[2];
			x1 = m_verts[0] + vAC[0]*soln[3];
			y1 = m_verts[1] + vAC[1]*soln[3];
			m_equalizers[m_nEq][0] = 2*x0 - x1;
			m_equalizers[m_nEq][1] = 2*y0 - y1;
			m_equalizers[m_nEq][2] = 2*x1 - x0;
			m_equalizers[m_nEq][3] = 2*y1 - y0;
			m_nEq++;
		}
	}
	if((ret = CheckEqSolns(b, c, a, &(soln[0]))) > 0) { // soln crossing BC and BA
		x0 = m_verts[2] + vBC[0]*soln[0];
		y0 = m_verts[3] + vBC[1]*soln[0];
		x1 = m_verts[2] - vAB[0]*soln[1];
		y1 = m_verts[3] - vAB[1]*soln[1];
		m_equalizers[m_nEq][0] = 2*x0 - x1;
		m_equalizers[m_nEq][1] = 2*y0 - y1;
		m_equalizers[m_nEq][2] = 2*x1 - x0;
		m_equalizers[m_nEq][3] = 2*y1 - y0;
		m_nEq++;
		if(ret == 2) {
			x0 = m_verts[2] + vBC[0]*soln[2];
			y0 = m_verts[3] + vBC[1]*soln[2];
			x1 = m_verts[2] - vAB[0]*soln[3];
			y1 = m_verts[3] - vAB[1]*soln[3];
			m_equalizers[m_nEq][0] = 2*x0 - x1;
			m_equalizers[m_nEq][1] = 2*y0 - y1;
			m_equalizers[m_nEq][2] = 2*x1 - x0;
			m_equalizers[m_nEq][3] = 2*y1 - y0;
			m_nEq++;
		}
	}
	if((ret = CheckEqSolns(c, a, b, &(soln[0]))) > 0) { // soln crossing CA and CB
		x0 = m_verts[4] - vAC[0]*soln[0];
		y0 = m_verts[5] - vAC[1]*soln[0];
		x1 = m_verts[4] - vBC[0]*soln[1];
		y1 = m_verts[5] - vBC[1]*soln[1];
		m_equalizers[m_nEq][0] = 2*x0 - x1;
		m_equalizers[m_nEq][1] = 2*y0 - y1;
		m_equalizers[m_nEq][2] = 2*x1 - x0;
		m_equalizers[m_nEq][3] = 2*y1 - y0;
		m_nEq++;
		if(ret == 2) {
			x0 = m_verts[4] - vAC[0]*soln[2];
			y0 = m_verts[5] - vAC[1]*soln[2];
			x1 = m_verts[4] - vBC[0]*soln[3];
			y1 = m_verts[5] - vBC[1]*soln[3];
			m_equalizers[m_nEq][0] = 2*x0 - x1;
			m_equalizers[m_nEq][1] = 2*y0 - y1;
			m_equalizers[m_nEq][2] = 2*x1 - x0;
			m_equalizers[m_nEq][3] = 2*y1 - y0;
			m_nEq++;
		}
	}
	return 0;
}

int CEqlzrfigView::CheckEqSolns(double a, double b, double c, double *pSoln)
{
	double perim, discr, x, y;
	int ret = 0;
	perim = a+b+c;
	discr = perim*perim - 8*b*c;
	if(discr < 0) {
		return 0;
	}
	discr = sqrt(discr);
	x = (perim + discr)/(4.0*c);
	y = (perim - discr)/(4.0*b);
	if((x > 0.0) && (x <= (1.0 + EPS)) && (y > 0.0) && (y <= (1.0 + EPS))) {
		ret++;
		pSoln[0] = x;
		pSoln[1] = y;
	}
	x = (perim - discr)/(4.0*c);
	y = (perim + discr)/(4.0*b);
	if((x > 0.0) && (x <= (1.0 + EPS)) && (y > 0.0) && (y <= (1.0 + EPS))) {
		pSoln[2*ret] = x;
		pSoln[2*ret+1] = y;
		ret++;
	}
	return ret;
}

void CEqlzrfigView::FindMaxMin()
{
	int i;
	m_MaxX = m_MinX = m_verts[0];
	m_MaxY = m_MinY = m_verts[1];
	if(m_MaxX < m_verts[2]) m_MaxX = m_verts[2];
	else if(m_MinX > m_verts[2]) m_MinX = m_verts[2];
	if(m_MaxY < m_verts[3]) m_MaxY = m_verts[3];
	else if(m_MinY > m_verts[3]) m_MinY = m_verts[3];
	if(m_MaxX < m_verts[4]) m_MaxX = m_verts[4];
	else if(m_MinX > m_verts[4]) m_MinX = m_verts[4];
	if(m_MaxY < m_verts[5]) m_MaxY = m_verts[5];
	else if(m_MinY > m_verts[5]) m_MinY = m_verts[5];
	for(i = 0; i < m_nEq; i++) {
		if(m_MaxX < m_equalizers[i][0]) m_MaxX = m_equalizers[i][0];
		else if(m_MinX > m_equalizers[i][0]) m_MinX = m_equalizers[i][0];
		if(m_MaxY < m_equalizers[i][1]) m_MaxY = m_equalizers[i][1];
		else if(m_MinY > m_equalizers[i][1]) m_MinY = m_equalizers[i][1];
		if(m_MaxX < m_equalizers[i][2]) m_MaxX = m_equalizers[i][2];
		else if(m_MinX > m_equalizers[i][2]) m_MinX = m_equalizers[i][2];
		if(m_MaxY < m_equalizers[i][3]) m_MaxY = m_equalizers[i][3];
		else if(m_MinY > m_equalizers[i][3]) m_MinY = m_equalizers[i][3];
	}
}

void CEqlzrfigView::MakePoints(RECT *pRect)
{
	double cenX, cenY, wid, scale;
	int icenX, icenY, i;
	cenX = 0.5 *(m_MaxX + m_MinX);
	cenY = 0.5 *(m_MaxY + m_MinY);
	icenX = (pRect->left + pRect->right)/2;
	icenY = (pRect->top + pRect->bottom)/2;
	wid = m_MaxX - m_MinX;
	scale = m_MaxY - m_MinY;
	if(scale > wid) wid = scale;
	if(wid < 1.0) wid = 1.0;
	scale = ((double)m_nScale)/wid;
	m_labelPt.x = icenX - m_nScale/3;;
	m_labelPt.y = icenY - m_nScale/3;
	for(i = 0; i < 3; i++) {
		m_vertPts[i].x = (int)((m_verts[2*i] - cenX)*scale) + icenX;
		m_vertPts[i].y = -(int)((m_verts[2*i+1] - cenY)*scale) + icenY;
	}
	for(i = 0; i < m_nEq ; i++) {
		m_eqEndPts[2*i].x = (int)((m_equalizers[i][0] - cenX)*scale) + icenX;
		m_eqEndPts[2*i].y = -(int)((m_equalizers[i][1] - cenY)*scale) + icenY;
		m_eqEndPts[2*i+1].x = (int)((m_equalizers[i][2] - cenX)*scale) + icenX;
		m_eqEndPts[2*i+1].y = -(int)((m_equalizers[i][3] - cenY)*scale) + icenY;
	}
}
