#if !defined(AFX_GENUPCDLG_H__4BB5A2FE_113E_4450_A71C_76E5DE376BC8__INCLUDED_)
#define AFX_GENUPCDLG_H__4BB5A2FE_113E_4450_A71C_76E5DE376BC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenUpcDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGenUpcDlg dialog

class CGenUpcDlg : public CDialog
{
// Construction
public:
	void MakeUPCBits(char *upcVals);
	char CompCheckDigit(char *upcDigits);
	CGenUpcDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGenUpcDlg)
	enum { IDD = IDD_GEN_UPC_DLG };
	BOOL	m_bInvert;
	CString	m_szUPCBits;
	int		m_nUpcCnt;
	CString	m_szUpcCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenUpcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGenUpcDlg)
	afx_msg void OnBitsButn();
	afx_msg void OnSaveButn();
	afx_msg void OnTestButn();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int ParseInData();
	void GetDigits();
	void CheckValidCodes(int index, char *upc, char fwdValid, char backValid);
	char m_upcCode[12];
	char m_upcBits[95];
	int m_curProb;
	FILE *m_FP;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENUPCDLG_H__4BB5A2FE_113E_4450_A71C_76E5DE376BC8__INCLUDED_)
