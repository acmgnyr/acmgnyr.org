#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define CHARS_PER_LINE	40
#define INFINITY 100000000
char inbuf[256];
int dbcnt = 0;

// which positions can move into blank
//		5 0
//	   4 6 1
//		3 2
// -1 means only two moves possible
int adjac[7][3] = {
	{1, 5, -1},
	{0, 2, 6},
	{1, 3, -1},
	{2, 4, -1},
	{3, 5, 6},
	{0, 4, -1},
	{1, 4, -1}
};

typedef struct _perm_
{
	struct _perm_ *pLNext;	// next level order scan
	struct _perm_ *pLPrev;	// previous level order scan
	int solveLen;
	char lastPos;			// where blank was we moved into
	char curPos;			// where blank is now
	char revSoln[64];		// moves to get here
	char permRep[8];		// this permutaion as a string
} PERM, *PPERM;

PERM permutations[5040];
PPERM permTail;

int factors[7] = {720, 120, 24, 6, 2, 1, 0};

// get index in array of a permuatation of A-F,' '
int GetPermIndex(char *pRep)
{
	char left[7];
	bool bFound;
	int i, j, ind, ret;
	for(i = 0; i < 7; i++) left[i] = i;
	ret = 0;
	for(i = 0; i < 7; i++)
	{
		if((pRep[i] != ' ') && ((pRep[i] < 'A') || (pRep[i] > 'F')))
		{
			fprintf(stderr, "bad char %c(%d) at %d for %s\n", 
				pRep[i], pRep[i], i, pRep);
			return -20;
		}
		if(pRep[i] == ' ')
			ind = 6;
		else
			ind = pRep[i] - 'A';
		bFound = false;
		for(j = 0; j < (7-i) ; j++)
		{
			if(bFound)
			{
				left[j-1] = left[j];
			}
			else
			{
				if(left[j] == ind)
				{
					bFound = true;
					ret += j*factors[i];
				}
			}
		}
		if(!bFound)
		{
			fprintf(stderr, "repeated char %c(%d) at %d for %s\n", 
				pRep[i], pRep[i], i, pRep);
			return -21;
		}
	}
	return ret;
}

int GetPermRep(int index, char *pRep)
{
	char left[6];
	int i, j, ind;

	if((index < 0) || (index >= 5040))
	{
		fprintf(stderr, "GetPermRep: bad index %d\n", index);
		return -22;
	}
	for(i = 0; i < 7; i++) left[i] = i;
	for(i = 0; i < 6; i++)
	{
		ind = index/factors[i];
		index -= ind*factors[i];
		if(left[ind] == 6)
			pRep[i] = ' ';
		else
			pRep[i] = 'A' + left[ind];
		for(j = ind + 1; j < 7 - i; j++)
		{
			left[j-1] = left[j];
		}
	}
	if(left[0] == 6)
		pRep[6] = ' ';
	else
		pRep[6] = 'A' + left[0];
	pRep[7] = 0;
	return 0;
}

void CopyPerm(char *pTo, char *pFrom)
{
	int i;
	for(i = 0; i < 7; i++)
	{
		pTo[i] = pFrom[i];
	}
	pTo[7] = 0;
}

// apply all non-reversing moves and if dist of new node
// is reduced, put it on the tail of the breadth first serch queue
void ProcessPermutation(PPERM pCur)
{
	PPERM pScan;
	char perm[8];
	int index, dist, i, nxtPos;
	int lastPos = pCur->lastPos;
	int curPos = pCur->curPos;
	
	// for each useful move, try it out
	for(i = 0; i < 3 ; i++)
	{
		nxtPos = adjac[curPos][i];
		if((nxtPos == lastPos) || (nxtPos == -1))
		{	// reversing move or no third move
			continue;
		}
		CopyPerm(&(perm[0]), &(pCur->permRep[0]));
		// apply move
		perm[curPos] = perm[nxtPos];
		perm[nxtPos] = ' ';
		dist = pCur->solveLen + 1;
		if((index = GetPermIndex(&(perm[0]))) >= 0)
		{	// look up new permutaion
			pScan = &permutations[index];
			if(dist < pScan->solveLen)
			{	// better than what we have, copy it in and put this on theend of the queue
				pScan->solveLen = dist;
				memcpy(&(pScan->revSoln[0]), &(pCur->revSoln[0]), pCur->solveLen);
				pScan->revSoln[dist-1] = perm[curPos];
				pScan->curPos = nxtPos;
				pScan->lastPos = curPos;
				if(pScan != permTail)
				{
					// unlink form where it was if necessary
					if(pScan->pLPrev != NULL)
					{
						pScan->pLPrev->pLNext = pScan->pLNext;
					}
					if(pScan->pLNext != NULL)
					{
						pScan->pLNext->pLPrev = pScan->pLPrev;
					}
					// now put this guy at the end
					pScan->pLNext = NULL;
					pScan->pLPrev = permTail;
					permTail->pLNext = pScan;
					permTail = pScan;
				}
			}
		}
		else
		{	// should not happen
			fprintf(stderr, "bad index %d for in %s out %s\n",
				index, &(pCur->permRep[0]), &(perm[0]));
		}
	}
}

// build table of permutaions and shortest movepath to each
// start with identity, apply moves to each elt and put newones on end of queue
int InitPermutaions(void)
{
	int i;
	PPERM pCur;
	// set defaults
	for(i = 0, pCur = &(permutations[0]); i < 5040 ; i++, pCur++)
	{
		pCur->pLNext = NULL;
		pCur->curPos = -1;
		pCur->lastPos = -1;
		pCur->revSoln[0] = 0;
		pCur->solveLen = INFINITY;
		GetPermRep(i, &(pCur->permRep[0]));
	}
	// do base item
	pCur = &permutations[0];
	pCur->solveLen = 0;
	pCur->curPos = 6;
	pCur->lastPos = -1;
	pCur->revSoln[0] = 0;
	permTail = pCur;
	while(pCur != NULL)
	{
		ProcessPermutation(pCur);
		pCur = pCur->pLNext;
	}
	return 0;
}


void PrintPermuatations(void)
{
	int i, maxlen, maxind, solncnt, ind;
	PPERM pCur;
	char buf[8];
	maxlen = maxind = solncnt = 0;
	ind = 13;
	for(i = 0, pCur = &permutations[0]; i < 5040 ; i++, pCur++)
	{
		if(pCur->permRep[6] == ' ')
		{
			::memcpy(&(buf[0]), &(pCur->permRep[0]), 6);
			buf[6] = 0;
		}
		if(pCur->solveLen < INFINITY)
		{
			printf("%d %s\n", ind, buf);
			if(pCur->solveLen > maxlen)
			{
				maxlen = pCur->solveLen;
				maxind = i;
			}
			solncnt++;
			ind++;
		}
		else
		{
			printf("%d %s\n", ind, buf);
			ind++;
		}
	}
	printf("%d solvable; max solve lend %d at index %d\n", solncnt, maxlen, maxind+1);
}

int ReadInput(char *pBuf, int probnum)
{
	int index, i;
	char *cp;
	char buf[8];

	index = atoi(pBuf);
	if(index != probnum)
	{
		fprintf(stderr, "bad index %d for problem %d\n", index, probnum);
		return -4;
	}
	cp = pBuf;
	while((*cp != 0) && (isdigit(*cp) || isspace(*cp)))
	{
		cp++;
	}
	for(i = 0; i < 6; i++)
	{
		buf[i]= cp[i];
	}
	buf[6] = ' ';
	buf[7] = 0;
	return GetPermIndex(buf);
}

void PrintSolution(int curprob, PPERM pPerm)
{
	char solnBuf[40];
	int curInd;
	int srcInd = pPerm->solveLen - 1;
	for(curInd = 0; curInd < pPerm->solveLen ; curInd++, srcInd--)
	{
		solnBuf[curInd] = pPerm->revSoln[srcInd];
	}
	solnBuf[curInd] = 0;
	printf("%d %d %s\n", curprob, pPerm->solveLen, &(solnBuf[0]));
}

int main()
{
	int nprob, curprob, index;
	PPERM pPerm;

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
	InitPermutaions();
//	PrintPermuatations();
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "input, problem %d\n", curprob);
			return -3;
		}
		index = ReadInput(&(inbuf[0]), curprob);
		if(index >= 0)
		{
			pPerm = &permutations[index];
			if(pPerm->solveLen == INFINITY)
			{
				printf("%d NO SOLUTION\n", curprob);
			}
			else
			{
				PrintSolution(curprob, pPerm);
			}
		}
	}
	return 0;
}
