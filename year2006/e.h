// e.h: interface for the CDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAY_H__F7B44AFD_B9E1_4E53_ABB6_41328A80194F__INCLUDED_)
#define AFX_DISPLAY_H__F7B44AFD_B9E1_4E53_ABB6_41328A80194F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BAD_TAG			1
#define TAG_CONFLICT	2
#define TOO_BIG			3


class CDisplay  
{
public:
	void Print(int probNum);
	int ParseMulti(int nrows, int ncols, int nchars, char *pStr);
	void Init(int nrows, int ncols, int nchars, char *pStr);
	CDisplay();
	virtual ~CDisplay();

	char m_display[25][82];
	int m_curChar;
	int m_lineJust;
	int m_curRow;
	int m_leftCount;
	int m_centerCount;
	int m_rightCount;
	int m_charSpace;
	int m_charCount;
	int m_rowCount;
	int m_colCount;
	int m_curLineCount;
	char m_leftBuf[80];
	char m_centerBuf[80];
	char m_rightBuf[80];
	char *m_pBuf;

private:
	int ProcNewLine();
	int ProcJust(char just);
	int ProcessTag();
	int AddChar(char c);
};

#endif // !defined(AFX_DISPLAY_H__F7B44AFD_B9E1_4E53_ABB6_41328A80194F__INCLUDED_)
