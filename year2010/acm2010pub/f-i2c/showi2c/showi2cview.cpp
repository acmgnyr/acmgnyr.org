// ShowI2CView.cpp : implementation of the CShowI2CView class
//

#include "stdafx.h"
#include "ShowI2C.h"

#include "ShowI2CDoc.h"
#include "ShowI2CView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowI2CView

IMPLEMENT_DYNCREATE(CShowI2CView, CFormView)

BEGIN_MESSAGE_MAP(CShowI2CView, CFormView)
	//{{AFX_MSG_MAP(CShowI2CView)
	ON_CBN_SELCHANGE(IDC_COMBO_WAVE, OnSelchangeComboWave)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_ZOOM, OnReleasedcaptureSliderZoom)
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDIT_SKIP, OnChangeEditSkip)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowI2CView construction/destruction

CShowI2CView::CShowI2CView()
	: CFormView(CShowI2CView::IDD)
{
	//{{AFX_DATA_INIT(CShowI2CView)
	m_nWaveForm = -1;
	m_nZoom = 10;
	m_nSkip = 0;
	//}}AFX_DATA_INIT
	m_bInit = FALSE;
}

CShowI2CView::~CShowI2CView()
{
}

void CShowI2CView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShowI2CView)
	DDX_Control(pDX, IDC_SPIN_SKIP, m_spinSkip);
	DDX_Control(pDX, IDC_SLIDER_ZOOM, m_slZoom);
	DDX_Control(pDX, IDC_COMBO_WAVE, m_cbForms);
	DDX_CBIndex(pDX, IDC_COMBO_WAVE, m_nWaveForm);
	DDX_Slider(pDX, IDC_SLIDER_ZOOM, m_nZoom);
	DDX_Text(pDX, IDC_EDIT_SKIP, m_nSkip);
	//}}AFX_DATA_MAP
}

BOOL CShowI2CView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CShowI2CView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_slZoom.SetRange(2, 50, TRUE);
	m_spinSkip.SetRange(0, 1100);
	UpdateData(FALSE);
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	m_bInit = TRUE;

}

/////////////////////////////////////////////////////////////////////////////
// CShowI2CView printing

BOOL CShowI2CView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CShowI2CView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CShowI2CView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CShowI2CView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CShowI2CView diagnostics

#ifdef _DEBUG
void CShowI2CView::AssertValid() const
{
	CFormView::AssertValid();
}

void CShowI2CView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CShowI2CDoc* CShowI2CView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CShowI2CDoc)));
	return (CShowI2CDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShowI2CView message handlers

void CShowI2CView::OpenFile(LPCTSTR szFile)
{
	CString szMsg;
	FILE *fp;
	int n = 0;
	int np, nb, c, nump;
	char ibuf[200];

	szMsg.Format("Open %s\n", szFile);
	::OutputDebugString(szMsg);

	fp = ::fopen(szFile, "rt");
	if(fp != NULL){
		m_arForms.RemoveAll();
		m_cbForms.ResetContent();
		::fgets(&(ibuf[0]), sizeof(ibuf)-1, fp);
		nump = ::atoi(&(ibuf[0]));
		for(n = 0; n < nump && ::fgets(&(ibuf[0]), sizeof(ibuf)-1, fp) != NULL; ){
			if(!isdigit(ibuf[0])){
				continue;
			}
			if(::sscanf(&(ibuf[0]), "%d %d", &(np), &(nb)) != 2){
				break;
			}
			szMsg.Empty();
			nb *= 2;
			while(nb > 0){
				c = getc(fp);
				if(c == EOF){
					break;
				}
				if(c == '0' || c == '1'){
					szMsg += (char)c;
					nb--;
				}
			}
			if(c == EOF){
				break;
			}
			m_arForms.Add(szMsg);
			n++;
			szMsg.Format("%d", n);
			m_cbForms.AddString(szMsg);
		}
		szMsg.Format("%d waveforms\n", n);
		::OutputDebugString(szMsg);
		::fclose(fp);
		m_nWaveForm = 0;
		UpdateData(FALSE);
		Invalidate(TRUE);
		UpdateWindow();
	}
}

void CShowI2CView::OnDraw(CDC* pDC) 
{
	LPCTSTR s;
	RECT rect;
	int c, d, i;
	int h, w, nclk, yc, yd, lyd, lyc;
	if(m_nWaveForm == -1 || m_nWaveForm >= m_arForms.GetSize()){
		return;
	}
	s = (LPCTSTR)m_arForms.GetAt(m_nWaveForm);
	nclk = ::strlen(s)/2;
	nclk -= m_nSkip;
	if(nclk < 0){
		return;
	}
	s += m_nSkip*2;
	pDC->GetClipBox(&(rect));
	h = rect.bottom - rect.top;
	w = rect.right - rect.left;
	yc = h/4;
	yd = yc + h/2;
	yc -= h/6;
	yd -= h/6;
	yd -= h/8;
	h = (h/3);
	lyc = yc;
	lyd = yd;

	CPen p2(PS_DOT, 1, RGB(255, 0, 0)), *pold;
	pold = pDC->SelectObject(&(p2));
	for(i = 1; i < nclk; i++){
		pDC->MoveTo(i*m_nZoom, yc);
		pDC->LineTo(i*m_nZoom, yd + h);
	}
	CPen p(PS_SOLID, 1, RGB(0, 0, 255));
	pDC->SelectObject(&(p));
	for(i = 1; i <= nclk && *s != '\0'; s += 2, i++){
		c = yc + (1 - (*s - '0'))*h;
		d = yd + (1 - (s[1] - '0'))*h;
		pDC->MoveTo((i-1)*m_nZoom, lyc);
		if(lyc != c){
			pDC->LineTo((i-1)*m_nZoom, c);
			lyc = c;
		}
		pDC->LineTo(i*m_nZoom, c);
		pDC->MoveTo((i-1)*m_nZoom, lyd);
		if(lyd != d){
			pDC->LineTo((i-1)*m_nZoom, d);
			lyd = d;
		}
		pDC->LineTo(i*m_nZoom, d);

	}
	pDC->SelectObject(pold);
}

void CShowI2CView::OnSelchangeComboWave() 
{
	UpdateData(TRUE);
	Invalidate(TRUE);
	UpdateWindow();
}

void CShowI2CView::OnReleasedcaptureSliderZoom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString szMsg;

	UpdateData(TRUE);
	szMsg.Format("Zoom now %d\n", m_nZoom);
	::OutputDebugString(szMsg);
	Invalidate(TRUE);
	UpdateWindow();
	
	*pResult = 0;
}

void CShowI2CView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	if(m_bInit){
		Invalidate(TRUE);
		UpdateWindow();
	}
}

void CShowI2CView::OnChangeEditSkip() 
{
	if(m_bInit){
		UpdateData(TRUE);
		Invalidate(TRUE);
		UpdateWindow();
	}
}
