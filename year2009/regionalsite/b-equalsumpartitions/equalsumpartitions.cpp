#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define NUMS_PER_LINE	10

char inbuf[256];

int indata[10000];

int readcnt = 0;
char *curPtr = &(inbuf[0]);
int incnt = 0;
int linecnt = 0;
int outcnt = 0;
int remcnt = 0;
int sum = 0;

int ReadInput(int size, int curprob)
{
	int val;
	readcnt = NUMS_PER_LINE;
	incnt = 0;
	linecnt = 0;
	outcnt = 0;
	remcnt = size;
	curPtr = &(inbuf[0]);
	sum = 0;
	while(remcnt > 0)
	{
		if(readcnt >= NUMS_PER_LINE)
		{
			linecnt++;
			readcnt = 0;
			curPtr = &(inbuf[0]);
			if(fgets(&(inbuf[0]), 255, stdin) == NULL)
			{
				fprintf(stderr, "Data read failed on problem %d value %d line %d\n",
					curprob, incnt, linecnt);
				return -10;
			}
		}
		val = atoi(curPtr);
		readcnt++;
		indata[incnt++] = val;
		sum += val;
		remcnt--;
		if(readcnt < NUMS_PER_LINE)
		{
			while((*curPtr != 0) && !isspace(*curPtr)) curPtr++;
			if((remcnt > 0) && (*curPtr != ' '))
			{
				fprintf(stderr, "missing space on problem %d value %d line %d\n",
					curprob, incnt, linecnt);
				return -11;
			}
			curPtr++;
			if((remcnt > 0) && ((*curPtr == 0) || isspace(*curPtr)))
			{
				fprintf(stderr, "bad format on problem %d value %d line %d\n",
					curprob, incnt, linecnt);
				return -12;
			}
		}
	}
	return 0;
}

int CheckLeadSum(int leadSum, int startInd, int size)
{
	int i, tSum;
	tSum = 0;
	for(i = startInd; i < size ; i++)
	{
		tSum += indata[i];
		if(tSum > leadSum)
		{
			return 0;
		}
		else if(tSum == leadSum)
		{
			tSum = 0;
		}
	}
	if(tSum != 0)
	{
		return 0;
	}
	return 1;
}

// the first equal partition must consist of the first several elements of the array
// and its sum must divide, the sum all values
// so we try leading sums until one fits
int MinEquiSum(int probsz)
{
	int leadSum, i;
	for(i = 0, leadSum = 0; (i < probsz) && (leadSum <= sum/2) ; i++)
	{
		leadSum += indata[i];
		if((sum % leadSum) == 0)
		{	// leadSum exactly divides sum, it is a candidate
			if(CheckLeadSum(leadSum, i+1, probsz))
			{
				return leadSum;
			}
		}
	}
	// if we get here, no factor worked, return the whole thing
	return sum;
}

int main()
{
	int nprob, curprob, probsz, ret, index, minsum;

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
			fprintf(stderr, "Read failed on problem %d size\n", curprob);
			return -3;
		}
		if(sscanf(&(inbuf[0]), "%d %d", &index, &probsz) != 2)
		{
			fprintf(stderr, "Scan failed on problem %d size\n", curprob);
			return -4;
		}
		if(probsz < 1)
		{
			fprintf(stderr, "problem size %d is < 1 in problem %d\n", probsz, curprob);
			return -5;
		}
		if(index != curprob)
		{
			fprintf(stderr, "problem index %d not = expected problem %d\n", index, curprob);
			return -7;
		}
		if((ret = ReadInput(probsz, curprob)) != 0)
		{
			return ret;
		}
		minsum = MinEquiSum(probsz);
		printf("%d %d\n", curprob, minsum);
	}
	return 0;
}