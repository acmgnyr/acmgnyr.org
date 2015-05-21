/*
 * Normal sort solution
 * by Fred Pickel
 * October 2008
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PROBS	1000
#define EPS	.001
#define ERR	.001

char inbuf[512];
char *pStr1, *pStr2;
int strLen1, strLen2;

int ReadDataSet(int probNum)
{
	int index, totlen;

	if(fgets(&(inbuf[0]), 511, stdin) == NULL)
	{
		fprintf(stderr, "read failed on problem number %d\n", probNum);
		return -1;
	}
	inbuf[511] = 0;
	totlen = strlen(&(inbuf[0]));
	pStr1 = &(inbuf[0]);
	strLen1 = 0;
	index = 0;
	while((inbuf[index] != 0) && (!isspace(inbuf[index])))
	{
		strLen1++;
		index++;
	}
	if(inbuf[index] != ' ')
	{
		fprintf(stderr, "missing space in problem number %d\n", probNum);
		return -2;
	}
	index++;
	strLen2 = 0;
	pStr2 = &(inbuf[index]);
	while((inbuf[index] != 0) && (!isspace(inbuf[index])))
	{
		strLen2++;
		index++;
	}
	return 0;
}

char getCurRep(char *str, int index, int len, char wasDigit)
{
	char ret;
	if(isdigit(str[index]))
	{
		ret = '0';
	}
	else if(((str[index] == '-') || (str[index] == '+'))
		&& (!wasDigit) && (index < (len - 1)) && isdigit(str[index+1]))
	{
		ret = '0';
	}
	else if((str[index] >= 'a') && (str[index] <= 'z'))
	{
		ret = toupper(str[index]);
	}
	else
	{
		ret = str[index];
	}
	return ret;
}

// compare numbers in str1, str2 starting at *pInd1, *pInd2 resp
// return -1, 0, 1 if 1 is numerically less, equal or greater than 2
// on return of 0, *pInd[12] point to first char after digits
int CompareNumbers(char *str1, int len1, int *pInd1, char *str2, int len2, int *pInd2)
{
	int sign1, sign2, ind1, ind2, dig1, dig2, i;
	sign1 = sign2 = 1;
	ind1 = *pInd1;
	ind2 = *pInd2;
	// get signs
	if(str1[ind1] == '+') ind1++;
	if(str1[ind1] == '-') 
	{
		ind1++;
		sign1 = -1;
	}
	if(str2[ind2] == '+') ind2++;
	if(str2[ind2] == '-') 
	{
		ind2++;
		sign2 = -1;
	}
	// skip leading zeroes
	while((ind1 < len1) && (str1[ind1] == '0')) ind1++;
	while((ind2 < len2) && (str2[ind2] == '0')) ind2++;
	if(((ind1 == len1) || (!isdigit(str1[ind1]))) &&
		((ind2 == len2) || (!isdigit(str2[ind2]))))
	{	// both all zero
		*pInd1 = ind1;
		*pInd2 = ind2;
		return 0;
	}
	if(sign1 != sign2)
	{	// opposit signs and one is non-zero
		if(sign1 < 0)
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}
	// count remaining digits
	dig1 = 0;
	while(((ind1 + dig1) < len1) && isdigit(str1[ind1 + dig1])) dig1++;
	dig2 = 0;
	while(((ind2 + dig2) < len2) && isdigit(str2[ind2 + dig2])) dig2++;
	if(dig1 != dig2)
	{	// one has more significant digits
		if(dig1 < dig2)
		{
			if(sign1 < 0)
			{	// num2 more negative
				return 1;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			if(sign1 < 0)
			{	// num1 more negative
				return -1;
			}
			else
			{
				return 1;
			}
		}
	}
	// both sam num sig digits, compare  digits
	for(i = 0; i < dig1 ; i++, ind1++, ind2++)
	{
		if(str1[ind1] < str2[ind2])
		{	// abs num1 < abs num2
			if(sign1 < 0)
			{	// num2 more negative
				return 1;
			}
			else
			{
				return -1;
			}
		}
		else if(str1[ind1] > str2[ind2])
		{	// abs num1 > abs num2
			if(sign1 < 0)
			{	// num1 more negative
				return -1;
			}
			else
			{
				return 1;
			}
		}
	}
	// if we get here, numswerethe same
	*pInd1 = ind1;
	*pInd2 = ind2;
	return 0;
}

int CompareStrings(char *str1, int len1, char *str2, int len2)
{
	int index1, index2, ret;
	char cur1, cur2;
	char prevDigit1, prevDigit2;
	prevDigit1 = prevDigit2 = 0;
	index1 = index2 = 0;
	while((index1 < len1) && (index2 < len2))
	{
		cur1 = getCurRep(str1, index1, len1, prevDigit1);
		cur2 = getCurRep(str2, index2, len2, prevDigit2);
		if(cur1 < cur2)
		{
			return -1;
		}
		if(cur1 > cur2)
		{
			return 1;
		}
		if(cur1 == '0')
		{ // numeric
			ret = CompareNumbers(str1, len1, &index1, str2, len2, &index2);
			if(ret != 0)
			{
				return ret;
			}
			prevDigit1 = prevDigit2 = 1;
		}
		else
		{
			index1++;
			index2++;
		}
	}
	if(index1 < len1)
	{
		return 1;
	}
	if(index2 < len2)
	{
		return -1;
	}
	return 0;
}

int main()
{
	int probCnt, curProb, ret;

	if(fgets(&(inbuf[0]), 511, stdin) == NULL)
	{
		fprintf(stderr, "read failed on problem count\n");
		return -1;
	}
	inbuf[511] = 0;
	probCnt = atoi(&(inbuf[0]));
	if((probCnt < 1) || (probCnt > MAX_PROBS))
	{
		fprintf(stderr, "Problem count %d not in range 1...%d\n", probCnt, MAX_PROBS);
		return -2;
	}
	for(curProb = 1; curProb <= probCnt ; curProb++)
	{
		if((ret = ReadDataSet(curProb)) != 0)
		{
			fprintf(stderr, "ReadDataSet returned %d on problem %d\n", ret, curProb);
			return -3;
		}
		ret = CompareStrings(pStr1, strLen1, pStr2, strLen2);
		printf("%d %d\n", curProb, ret);
	}
	return 0;
}

