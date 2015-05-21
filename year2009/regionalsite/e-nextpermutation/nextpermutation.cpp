#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

int digitCnts[10];
int nDigits;
char inbuf[255];

char *pNumStart = NULL;

int CheckInput(char *pBuf, int probnum)
{
	int index;
	char *cp;

	pNumStart = NULL;
	nDigits = 0;
	// skip leading space
	cp = pBuf;
	while((cp != 0) && isspace(*cp))
	{
		cp++;
	}
	index = atoi(cp);
	if(index != probnum)
	{
		fprintf(stderr, "bad index %d for problem %d\n", index, probnum);
		return -4;
	}
	// skip problem number
	while((cp != 0) && isdigit(*cp))
	{
		cp++;
	}
	// skip leading space
	while((cp != 0) && isspace(*cp))
	{
		cp++;
	}
	pNumStart = cp;
	// skip problem number
	while((cp != 0) && !isspace(*cp))
	{
		if(isdigit(*cp))
		{
			cp++;
			nDigits++;
		}
		else
		{
			fprintf(stderr, "no digit %c (%d) at %d for problem %d\n",
				*cp, *cp, cp - inbuf, probnum);
			return -5;
		}
	}
	return 0;
}

// as we scan from least significant digit to most significant,
// as long as the digit values are non-decreasing, no permutation
// of those digits can have a larger value.
// if we find a digit location whose digit value is smaller than the
// previous, swap in the next larger value seen so far,
// keep more siginificant digits the same and put the remaining digits
// back in increasing digits value as we go from most to least significant
// remember how many of each digit we haveseen so far to avoid the sort.
int PermuteInput(char *pBuf, int probnum)
{
	int i, j, prevDigit, curDigit, startIndex;
	// set counts to zero
	for(i = 0; i < 10 ; i++) digitCnts[i] = 0;
	prevDigit = pNumStart[nDigits - 1] - '0';
	digitCnts[prevDigit]++;
	startIndex = -1;
	for(i = nDigits - 2; i >= 0; i--)
	{
		curDigit = pNumStart[i] - '0';
		digitCnts[curDigit]++;
		if(curDigit < prevDigit)
		{
			startIndex = i;
			break;
		}
		else
		{
			prevDigit = curDigit;
		}
	}
	if(startIndex == -1)
	{	// no bigger permutation
		return -1;
	}
	// find the next larger digit we have seen and put it at startIndex
	for(i = curDigit+1; i < 10 ; i++)
	{
		if(digitCnts[i] > 0)
		{
			digitCnts[i]--;
			pNumStart[startIndex] = i + '0';
			break;
		}
	}
	startIndex++;
	// put out all digits in digitCnts in increasing order
	for(i = 0; i < 10 ; i++)
	{
		for(j = 0; j < digitCnts[i] ; j++)
		{
			pNumStart[startIndex++] = i + '0';
		}
	}
	return 0;
}

int main()
{
	int nprob, curprob, ret;

	if(fgets(&(inbuf[0]), 255, stdin) == NULL)
	{
		fprintf(stderr, "Read failed on problem count\n");
		return -1;
	}
	if(sscanf(&(inbuf[0]), "%d", &nprob) != 1)
	{
		fprintf(stderr, "Scan failed on problem count\n");
		return -2;
	}
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "input read failed problem %d\n", curprob);
			return -3;
		}
		// check input format
		if((ret = CheckInput(&(inbuf[0]), curprob)) != 0)
		{
			return ret;
		}
		if(PermuteInput(&(inbuf[0]), curprob) != 0)
		{
			printf("%d BIGGEST\n", curprob);
		}
		else
		{
			fputs(&(inbuf[0]), stdout);
		}
	}
	return 0;
}