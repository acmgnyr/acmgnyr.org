// e.cpp: implementation of the CDisplay class.
//
//////////////////////////////////////////////////////////////////////

#include "e.h"
#include <ctype.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDisplay::CDisplay()
{

}

CDisplay::~CDisplay()
{

}

void CDisplay::Init(int nrows, int ncols, int nchars, char *pStr)
{
	int i, j;

	for(i = 0; i < 25; i++)
	{
		for(j = 0; j < 82 ; j++)
		{
			m_display[i][j] = ' ';
		}
	}
	m_lineJust = 2;
	m_leftCount = 0;
	m_centerCount = 0;
	m_rightCount = 0;
	m_curChar = 0;
	m_charSpace = 0;
	m_curRow = 0;
	m_curLineCount = 0;
	m_charCount = nchars;
	m_rowCount = nrows;
	m_colCount = ncols;
	m_pBuf = pStr;
}

int CDisplay::AddChar(char c)
{
	int i;

	if(m_curRow >= m_rowCount)
	{
		return TOO_BIG;
	}
	if(m_lineJust == 2)
	{
		if(m_leftCount == 0)
		{
			m_leftBuf[m_leftCount] = c;
			m_leftCount++;
			m_curLineCount++;
		}
		else if((m_leftCount + m_charSpace + 1) > m_colCount)
		{
			return TOO_BIG;
		}
		else
		{
			for(i = 0; i < m_charSpace ; i++)
			{
				m_leftBuf[m_leftCount] = ' ';
				m_leftCount++;
				m_curLineCount++;
			}
			m_leftBuf[m_leftCount] = c;
			m_leftCount++;
			m_curLineCount++;
		}
	}
	else if(m_lineJust == 4)
	{
		if(m_rightCount == 0)
		{
			m_rightBuf[m_rightCount] = c;
			m_rightCount++;
			m_curLineCount++;
		}
		else if((m_rightCount + m_charSpace + 1) > m_colCount)
		{
			return TOO_BIG;
		}
		else
		{
			for(i = 0; i < m_charSpace ; i++)
			{
				m_rightBuf[m_rightCount] = ' ';
				m_rightCount++;
				m_curLineCount++;
			}
			m_rightBuf[m_rightCount] = c;
			m_rightCount++;
			m_curLineCount++;
		}
	}
	else if(m_lineJust == 3)
	{
		if(m_centerCount == 0)
		{
			m_centerBuf[m_centerCount] = c;
			m_centerCount++;
			m_curLineCount++;
		}
		else if((m_centerCount + m_charSpace + 1) > m_colCount)
		{
			return TOO_BIG;
		}
		else
		{
			for(i = 0; i < m_charSpace ; i++)
			{
				m_centerBuf[m_centerCount] = ' ';
				m_centerCount++;
				m_curLineCount++;
			}
			m_centerBuf[m_centerCount] = c;
			m_centerCount++;
			m_curLineCount++;
		}
	}
	else
	{	// full just does not use spacing
		if(m_centerCount >= m_colCount)
		{
			return TOO_BIG;
		}
		m_centerBuf[m_centerCount] = c;
		m_centerCount++;
		m_curLineCount++;
	}
	return 0;
}

// '[' at cur char, process tag and advance cur char to after tag
int CDisplay::ProcessTag()
{
	char c;

	if(m_pBuf[m_curChar] != '[')
	{
		return BAD_TAG;
	}
	m_curChar++;
	if(m_curChar >= m_charCount)
	{
		return BAD_TAG;
	}
	c = ::toupper(m_pBuf[m_curChar]);
	m_curChar++;
	if(m_curChar >= m_charCount)
	{
		return BAD_TAG;
	}
	if(c == 'N')
	{
		if((m_curChar+1) >= m_charCount)
		{	// space for ']'
			return BAD_TAG;
		}
		if((::toupper(m_pBuf[m_curChar]) == 'L') && (m_pBuf[m_curChar+1] == ']'))
		{	// new line
			m_curChar += 2;
			return ProcNewLine();
		}
		else
		{	// badly formed new line tag
			return BAD_TAG;
		}
	}
	else if(c == 'J')
	{
		if((m_curChar+2) >= m_charCount)
		{	// space for number + ']'
			return BAD_TAG;
		}
		if((::toupper(m_pBuf[m_curChar]) == 'L') && (m_pBuf[m_curChar+2] == ']') &&
			(m_pBuf[m_curChar+1] >= '2') && (m_pBuf[m_curChar+1] <= '5'))
		{	// justification
			c = m_pBuf[m_curChar+1];
			m_curChar += 3;
			return ProcJust(c);
		}
		else
		{	// badly formed line justification tag
			return BAD_TAG;
		}
	}
	else if(c == 'S')
	{
		if((m_curChar+2) >= m_charCount)
		{	// space for digit + ']'
			return BAD_TAG;
		}
		if((::toupper(m_pBuf[m_curChar]) == 'C') && (m_pBuf[m_curChar+2] == ']') &&
			(::isdigit(m_pBuf[m_curChar+1])))
		{	// char space
			c = m_pBuf[m_curChar+1];
			m_curChar += 3;
			m_charSpace = c - '0';
			return 0;
		}
		else
		{	// badly formed char spacing tag
			return BAD_TAG;
		}
	}
	else if(c == '/')
	{
		if((m_curChar+2) >= m_charCount)
		{	// space for C + ']'
			return BAD_TAG;
		}
		if((::toupper(m_pBuf[m_curChar]) == 'S') && (m_pBuf[m_curChar+2] == ']') &&
			(::toupper(m_pBuf[m_curChar+1]) == 'C'))
		{	// new line
			m_curChar += 3;
			m_charSpace = 0;
			return 0;
		}
		else
		{	// badly formed end char spacing tag
			return BAD_TAG;
		}
	}
	else
	{	// not a valid tag
		return BAD_TAG;
	}
	return 0;
}

int CDisplay::ParseMulti(int nrows, int ncols, int nchars, char *pStr)
{
	int ret;
	char c;

	Init(nrows, ncols, nchars, pStr);
	while(m_curChar < nchars)
	{
		c = pStr[m_curChar];
		if(c == '[')
		{
			if((m_curChar+1 < nchars) && (pStr[m_curChar+1] == '['))
			{
				if((ret = AddChar('[')) != 0)
				{
					return ret;
				}
				m_curChar += 2;
			}
			else
			{
				if((ret = ProcessTag()) != 0)
				{
					return ret;
				}
			}
		}
		else if(c == ']')
		{
			if((m_curChar+1 < nchars) && (pStr[m_curChar+1] == ']'))
			{
				if((ret = AddChar(']')) != 0)
				{
					return ret;
				}
				m_curChar += 2;
			}
			else
			{
				return BAD_TAG;
			}
		}
		else
		{
			if((ret = AddChar(c)) != 0)
			{
				return ret;
			}
			m_curChar++;
		}
	}
	while(m_curRow < m_rowCount)
	{
		if((ret = ProcNewLine()) != 0)
		{
			return ret;
		}
	}
	return 0;
}

int CDisplay::ProcJust(char just)
{
	int newJust = just - '0';
	if(newJust == m_lineJust)
	{	// no change, return
		return 0;
	}
	if(((newJust == 2) || (newJust == 5) || (m_lineJust == 5))
		&& (m_curLineCount > 0))
	{	// cannot change to or from full just or to left just once line started
		return TAG_CONFLICT;
	}
	if((newJust == 3) && (m_lineJust == 4) && (m_rightCount > 0))
	{	// cannot change from right to center just once right text started
		return TAG_CONFLICT;
	}
	// OK set new value
	m_lineJust = newJust;
	return 0;
}

int CDisplay::ProcNewLine()
{
	int cFirst, cLast, rFirst, lLast, i, j, rem, skip;

	if(m_curLineCount == 0)
	{	// nothing to do
		m_display[m_curRow][m_colCount] = '\n';		// terminate the line string
		m_display[m_curRow][m_colCount+1] = 0;
		m_curRow++;	// inc row counter
		return 0;
	}

	if(m_lineJust == 5)
	{	// full justification
		rem = m_colCount - m_centerCount;
		if(rem < 0)
		{
			return TOO_BIG;
		}
		if(m_centerCount > 1)
		{
			skip = rem/(m_centerCount - 1);
		}
		else
		{
			skip = 0;
		}
		cFirst = (rem - (skip * (m_centerCount - 1)))/2;
		for(i = 0, j = cFirst ; i < m_centerCount ; i++, j += (skip + 1))
		{
			m_display[m_curRow][j] = m_centerBuf[i];
		}
	}
	else
	{
		if(m_rightCount > 0)
		{
			rFirst = m_colCount - m_rightCount;
		}
		else
		{	// no space needed
			rFirst = m_colCount + 1;
		}
		if(m_leftCount > 0)
		{
			lLast = m_leftCount - 1;
		}
		else
		{	// no space needed
			lLast = -2;
		}
		if((lLast + 1) >= rFirst)
		{
			return TOO_BIG;
		}
		if(m_centerCount > 0)
		{
			cFirst = (m_colCount - m_centerCount)/2;
			cLast = cFirst + m_centerCount - 1;
			if((lLast + 1) >= cFirst)
			{
				return TOO_BIG;
			}
			if((cLast + 1) >= rFirst)
			{
				return TOO_BIG;
			}
		}
		// it fits, copy in
		if(m_leftCount > 0)
		{
			for(i = j = 0 ; i < m_leftCount ; i++, j++)
			{
				m_display[m_curRow][j] = m_leftBuf[i];
			}
		}
		if(m_centerCount > 0)
		{
			for(i = 0, j = cFirst ; i < m_centerCount ; i++, j++)
			{
				m_display[m_curRow][j] = m_centerBuf[i];
			}
		}
		if(m_rightCount > 0)
		{
			for(i = 0, j = rFirst ; i < m_rightCount ; i++, j++)
			{
				m_display[m_curRow][j] = m_rightBuf[i];
			}
		}
	}
	m_display[m_curRow][m_colCount] = '\n';		// terminate the line string
	m_display[m_curRow][m_colCount+1] = 0;
	// next row and init per-row data
	m_curRow++;
	m_leftCount = 0;
	m_centerCount = 0;
	m_rightCount = 0;
	m_curLineCount = 0;

	return 0;
}

void CDisplay::Print(int probNum)
{
	int i;
	printf("%d\n", probNum);
	for(i = 0; i < m_rowCount ; i++)
	{
		printf("%s", &(m_display[i][0]));
	}
	printf("\n");
}
