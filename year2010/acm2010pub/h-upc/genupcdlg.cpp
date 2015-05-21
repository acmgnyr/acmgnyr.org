// GenUpcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "upcpic.h"
#include "GenUpcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern BYTE lpattern[10];
extern BYTE rpattern[10];

#define MAX_UPCS		128

char upcvals[MAX_UPCS][12];
BYTE fwdvals[12], fwdmasks[12], backvals[12], backmasks[12];
WORD fwddigits[12], backdigits[12];


/////////////////////////////////////////////////////////////////////////////
// CGenUpcDlg dialog


CGenUpcDlg::CGenUpcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGenUpcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenUpcDlg)
	m_bInvert = FALSE;
	m_szUPCBits = _T("");
	m_nUpcCnt = 0;
	m_szUpcCode = _T("");
	//}}AFX_DATA_INIT
	m_curProb = 0;
	m_FP = ::fopen("test.txt", "w");
}


void CGenUpcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenUpcDlg)
	DDX_Check(pDX, IDC_INVERT, m_bInvert);
	DDX_Text(pDX, IDC_UPC_BITS, m_szUPCBits);
	DDX_Text(pDX, IDC_UPC_CNT, m_nUpcCnt);
	DDX_Text(pDX, IDC_UPC_CODE, m_szUpcCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenUpcDlg, CDialog)
	//{{AFX_MSG_MAP(CGenUpcDlg)
	ON_BN_CLICKED(IDC_BITS_BUTN, OnBitsButn)
	ON_BN_CLICKED(IDC_SAVE_BUTN, OnSaveButn)
	ON_BN_CLICKED(IDC_TEST_BUTN, OnTestButn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenUpcDlg message handlers

void CGenUpcDlg::OnBitsButn() 
{
	int i;
	char c;
	UpdateData(TRUE);
	if(m_szUpcCode.GetLength() < 11) {
		::AfxMessageBox("Code must have at least 11 characters");
		return;
	}
	for(i = 0; i < 11; i++) {
		c = m_szUpcCode[i];
		if((c < '0') || (c > '9')) {
			::AfxMessageBox("Code characters must be decimal digits");
			return;
		}
		m_upcCode[i] = c - '0';
	}
	m_upcCode[11] = CompCheckDigit(&(m_upcCode[0]));
	m_szUpcCode.Empty();
	for(i = 0; i < 12 ; i++) {
		m_szUpcCode += (char)(m_upcCode[i] + '0');
	}
	MakeUPCBits(&(m_upcCode[0]));
	UpdateData(FALSE);
}

void CGenUpcDlg::OnSaveButn() 
{
	int i;
	char c;
	CString left, right, temp;
	UpdateData(TRUE);
	if(m_FP != NULL) {
		if(m_szUPCBits.GetLength() < 108) {
			::AfxMessageBox("Must have 108 upc bits and space");
			return;
		}
		temp = m_szUPCBits;
		left = temp.Left(3);
		temp = temp.Mid(4);
		for(i = 0; i < 6; i++) {
			left += temp.Left(7);
			temp = temp.Mid(8);
		}
		left += temp.Left(5);
		temp = temp.Mid(6);
		for(i = 0; i < 6; i++) {
			left += temp.Left(7);
			temp = temp.Mid(8);
		}
		left += temp.Left(3);
		temp = left;

		for(i = 0; i < 95; i++) {
			c = temp[i];
			if((c != '0') && (c != '1') && (c != '?')) {
				::AfxMessageBox("Upc bits must be 0 1 or ?");
				return;
			}
		}
		left = temp.Left(50);
		right = temp.Mid(50, 45);
		::fprintf(m_FP, "%d\n", m_curProb);
		::fprintf(m_FP, "%s\n", left);
		::fprintf(m_FP, "%s\n", right);
		m_curProb++;
	}
}

void CGenUpcDlg::OnTestButn() 
{
	char curUpc[12];
	UpdateData(TRUE);
	m_nUpcCnt = 0;
	if(ParseInData() == 0) {
		GetDigits();
		CheckValidCodes(0, &(curUpc[0]), 1, 1);
	}
	UpdateData(FALSE);
}

char CGenUpcDlg::CompCheckDigit(char *upcDigits)
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

void CGenUpcDlg::MakeUPCBits(char *upcVals)
{
	int i, j, k, m;
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
	if(m_bInvert) {
		j = 94;
		k = -1;
	} else {
		j = 0;
		k = 1;
	}
	m_szUPCBits.Empty();

	for(i = 0; i < 3 ; i++, j += k) {
		m_szUPCBits += (char)(m_upcBits[j] + '0');
	}
	m_szUPCBits += " ";
	for(m = 0; m < 6; m++) {
		for(i = 0; i < 7 ; i++, j += k) {
			m_szUPCBits += (char)(m_upcBits[j] + '0');
		}
		m_szUPCBits += " ";
	}
	for(i = 0; i < 5 ; i++, j += k) {
		m_szUPCBits += (char)(m_upcBits[j] + '0');
	}
	m_szUPCBits += " ";
	for(m = 0; m < 6; m++) {
		for(i = 0; i < 7 ; i++, j += k) {
			m_szUPCBits += (char)(m_upcBits[j] + '0');
		}
		m_szUPCBits += " ";
	}
	for(i = 0; i < 3 ; i++, j += k) {
		m_szUPCBits += (char)(m_upcBits[j] + '0');
	}
}

void CGenUpcDlg::OnOK() 
{
	if(m_FP != NULL) {
		::fclose(m_FP);
		m_FP = NULL;
	}
	CDialog::OnOK();
}

int CGenUpcDlg::ParseInData()
{
	int i, j, k, ret;
	char c1, c2, c3, c4, c5;
	BYTE frmask, fval, brmask, bval, fmask, bmask;

	ret = 0;
	// check start
	c1 = m_szUPCBits[0]; c2 = m_szUPCBits[1]; c3 = m_szUPCBits[2];
	if(((c1 != '1') && (c1 != '?')) || 
		((c2 != '0') && (c2 != '?')) ||
		((c3 != '1') && (c3 != '?'))) {
		ret = 1;
	}
	// check middle
	c1 = m_szUPCBits[52]; c2 = m_szUPCBits[53]; c3 = m_szUPCBits[54];
	c4 = m_szUPCBits[55]; c5 = m_szUPCBits[56];
	if(((c1 != '0') && (c1 != '?')) || 
		((c2 != '1') && (c2 != '?')) ||
		((c3 != '0') && (c3 != '?')) ||
		((c4 != '1') && (c4 != '?')) ||
		((c5 != '0') && (c5 != '?'))) {
		ret = 2;
	}
	// get first 6 digits of data
	for(i = 0, j = 4; i < 6 ; i++, j += 8) {
		frmask = fval = brmask = bval = 0;
		fmask = 0x40;
		bmask = 0x01;
		for(k = 0; k < 7 ; k++) {
			c1 = m_szUPCBits[j+k];
			if(c1 == '0') {	// valid and blank
				frmask |= fmask;
				brmask |= bmask;
			} else if(c1 == '1') { // valid and bar
				frmask |= fmask;
				brmask |= bmask;
				fval |= fmask;
				bval |= bmask;
			}	// else unknown char
			fmask >>= 1;
			bmask <<= 1;
		}
		fwdvals[i] = fval;
		fwdmasks[i] = frmask;
		backvals[11 - i] = bval;
		backmasks[11 - i] = brmask;
	}
	// check end
	c1 = m_szUPCBits[106]; c2 = m_szUPCBits[107]; c3 = m_szUPCBits[108];
	if(((c1 != '1') && (c1 != '?')) || 
		((c2 != '0') && (c2 != '?')) ||
		((c3 != '1') && (c3 != '?'))) {
		ret = 3;
	}
	// get second 6 digits of data
	for(i = 0, j = 58; i < 6 ; i++, j += 8) {
		frmask = fval = brmask = bval = 0;
		fmask = 0x40;
		bmask = 0x01;
		for(k = 0; k < 7 ; k++) {
			c1 = m_szUPCBits[j+k];
			if(c1 == '0') {	// valid and blank
				frmask |= fmask;
				brmask |= bmask;
			} else if(c1 == '1') { // valid and bar
				frmask |= fmask;
				brmask |= bmask;
				fval |= fmask;
				bval |= bmask;
			}	// else unknown
			fmask >>= 1;
			bmask <<= 1;
		}
		fwdvals[6+i] = fval;
		fwdmasks[6+i] = frmask;
		backvals[5 - i] = bval;
		backmasks[5 - i] = brmask;
	}
	return ret;
}

void CGenUpcDlg::GetDigits()
{
	int i, j;
	WORD outmask;
	for(i = 0; i < 12; i++) {
		fwddigits[i] = backdigits[i] = 0;
	}
	// left digits
	for(i = 0; i < 6; i++) {
		outmask = 1;
		for(j = 0; j < 10 ; j++) {
			if((lpattern[j] & fwdmasks[i]) == fwdvals[i]) {
				fwddigits[i] |= outmask;
			}
			if((lpattern[j] & backmasks[i]) == backvals[i]) {
				backdigits[i] |= outmask;
			}
			outmask <<= 1;
		}
	}
	// right digits
	for(i = 6; i < 12; i++) {
		outmask = 1;
		for(j = 0; j < 10 ; j++) {
			if((rpattern[j] & fwdmasks[i]) == fwdvals[i]) {
				fwddigits[i] |= outmask;
			}
			if((rpattern[j] & backmasks[i]) == backvals[i]) {
				backdigits[i] |= outmask;
			}
			outmask <<= 1;
		}
	}
}

void CGenUpcDlg::CheckValidCodes(int index, char *upc, char fwdValid, char backValid)
{
	char digit, newfwd, newback;
	int i;
	WORD mask;
	// check too many codes
	if(m_nUpcCnt >= MAX_UPCS) {
		return;
	}
	// end case
	if(index == 11) {	// check digit
		digit = CompCheckDigit(upc);
		mask = (1 << digit);
		if((fwdValid && ((mask & fwddigits[11]) != 0)) ||
			(backValid && ((mask & backdigits[11]) != 0))) {
			upc[11] = digit;
			for(i = 0; i < 12 ; i++) {
				upcvals[m_nUpcCnt][i] = '0' + upc[i];
			}
			m_nUpcCnt++;
		}
	}
	else { // scan through all digit possibilities
		for(digit = 0, mask = 1; digit < 10 ; digit ++, mask <<= 1) {
			newfwd = newback = 0;
			if(fwdValid && ((mask & fwddigits[index]) != 0)) newfwd = 1;
			if(backValid && ((mask & backdigits[index]) != 0)) newback = 1;
			if(newfwd || newback) {
				upc[index] = digit;
				CheckValidCodes(index+1, upc, newfwd, newback);
			}
		}
		// check too many codes
		if(m_nUpcCnt >= MAX_UPCS) {
			return;
		}
	}
}

