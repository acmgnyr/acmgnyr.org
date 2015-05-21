/*
 * G - Mad Veterinarian
 * ICPC 2012 Greater NY Regional
 * Solution by Fred Pickel
 * Problem by Fred Pickel
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

typedef unsigned long DWORD;

#define MAX_SOLN_STEPS	30
#define MAX_SUB_COUNT	10

int machOut[3][3];
int subIn[3], subOut[3];
int maxVal[3];
char inbuf[256];
char outbuf[256];

#define STATE_CNT_MASK	0xff
#define PREV_MACH_MASK	0xf
#define PREV_MACH_SHFT	8
#define PREV_IND_MASK	0xffff
#define PREV_IND_SHFT	12
#define FOUND_STATE_SIZE	32
#define FOUND_STATE_AR_SZ	(FOUND_STATE_SIZE*FOUND_STATE_SIZE*FOUND_STATE_SIZE)

DWORD foundState[FOUND_STATE_AR_SZ];

char *machCodes = "ABCabc";
char *revCodes = "abcABC";

typedef struct _vet_step_
{
	struct _vet_step_ *pNext;
	int state[3];
	int stepcnt;
	int usedMach[6];
} VET_STEP, *PVET_STEP;

PVET_STEP pVQHead, pVQTail;

int CheckApplyMach(char c, int *pState)
{
	int i, step[3], need[3];
	switch(c)
	{
	case 'A':
		for(i = 0; i < 3; i++) {
			step[i] = machOut[0][i];
			need[i] = 0;
		}
		need[0] = 1;
		break;
	case 'B':
		for(i = 0; i < 3; i++) {
			step[i] = machOut[1][i];
			need[i] = 0;
		}
		need[1] = 1;
		break;
	case 'C':
		for(i = 0; i < 3; i++) {
			step[i] = machOut[2][i];
			need[i] = 0;
		}
		need[2] = 1;
		break;
	case 'a':
		for(i = 0; i < 3; i++) {
			need[i] = machOut[0][i];
			step[i] = 0;
		}
		step[0] = 1;
		break;
	case 'b':
		for(i = 0; i < 3; i++) {
			need[i] = machOut[1][i];
			step[i] = 0;
		}
		step[1] = 1;
		break;
	case 'c':
		for(i = 0; i < 3; i++) {
			need[i] = machOut[2][i];
			step[i] = 0;
		}
		step[2] = 1;
		break;
	default:	// bad char
		return -1;
	}
	// check we can appply it
	for(i = 0; i < 3; i++) {
		if(need[i] > pState[i]) {
			return -2;
		}
	}
	// apply it
	for(i = 0; i < 3; i++) {
		pState[i] = pState[i] - need[i] + step[i];
	}
	return 0;
}
	
void AddQ(PVET_STEP pStep)
{
	if(pVQHead == NULL) {
		pVQHead = pVQTail = pStep;
	}
	else {
		pVQTail->pNext = pStep;
		pVQTail = pStep;
		pStep->pNext = NULL;
	}
}

void CleanQ()
{
	PVET_STEP pCur, pNext;
	pCur = pVQHead;
	while(pCur != NULL) {
		pNext = pCur->pNext;
		free(pCur);
		pCur = pNext;
	}
	pVQHead = pVQTail = NULL;
}

int ProduceSoln(int *pState, int codeind)
{
	DWORD code;
	int cnt, ret, step, prevInd;
	char c;
	code = foundState[codeind];
	ret = cnt = code & STATE_CNT_MASK;
	outbuf[cnt] = 0;
	while(cnt > 0) {
		cnt--;
		step = (code >> PREV_MACH_SHFT) & PREV_MACH_MASK;
		prevInd = (code >> PREV_IND_SHFT) & PREV_IND_MASK;
		c = machCodes[step];
		outbuf[cnt] = c;
		code = foundState[prevInd];
	}
	return ret;
}

int ProcessHead(int *pOut)
{
	PVET_STEP pCur, pNext;
	int state[3], i, j, codeind, prevInd;
	DWORD code;
	pCur = pVQHead;
	pVQHead = pCur->pNext;
	if(pVQHead == NULL) pVQTail = NULL;
	prevInd = pCur->state[0]*FOUND_STATE_SIZE*FOUND_STATE_SIZE +
		pCur->state[1]*FOUND_STATE_SIZE + pCur->state[2];
	for(i = 0; i < 6; i++) {
		if(((i < 3) && (pCur->usedMach[i+3] == 0)) ||
			((i >= 3) && (pCur->usedMach[i-3] == 0))){	// have not gone other way
			for(j = 0; j < 3; j++) {
				state[j] = pCur->state[j];
			}
			if(CheckApplyMach(machCodes[i], &(state[0])) == 0) {
				if((state[0] >= maxVal[0]) || 
					(state[1] >= maxVal[1]) ||
					(state[2] >= maxVal[2])) {
					// out of range
					continue;
				}
				// can do this step
				codeind = state[0]*FOUND_STATE_SIZE*FOUND_STATE_SIZE +
					state[1]*FOUND_STATE_SIZE + state[2];
				code = foundState[codeind];
				if(code != 0xffffffff) {
					if((code & STATE_CNT_MASK) <= (unsigned)(pCur->stepcnt)) {
						// already been here a no larger number of steps
						continue;
					}
				}
				code = (pCur->stepcnt + 1) | (i << PREV_MACH_SHFT) | (prevInd << PREV_IND_SHFT);
				foundState[codeind] = code;
				if((state[0] == pOut[0]) && (state[1] == pOut[1]) && (state[2] == pOut[2])) {
					return ProduceSoln(state, codeind);
				}
				if((pCur->stepcnt + 1) > MAX_SOLN_STEPS) {	// too long
					continue;
				}
				pNext = (PVET_STEP) malloc(sizeof(VET_STEP));
				if(pNext == NULL) {
					return -101;
				}
				memcpy(pNext, pCur, sizeof(VET_STEP));
				for(j = 0; j < 3; j++) {
					pNext->state[j] = state[j];
				}
				pNext->stepcnt++;
				pNext->usedMach[i]++;
				AddQ(pNext);
			}
		}
	}
	free(pCur);
	return 0;
}

int MadVetSolve(int *pIn, int *pOut, int subprob, int mainprob)
{
	int i, ret;
#ifndef AGGRESSIVE
	int j;
#endif
	// check in and out are the same
	if((pIn[0] == pOut[0]) && (pIn[1] == pOut[1]) && (pIn[2] == pOut[2])) {
		return 0;
	}
#ifdef AGGRESSIVE
	for(i = 0; i < 3 ; i++) {
		maxVal[i] = machOut[0][i];
		if(maxVal[i] < machOut[1][i]) maxVal[i] = machOut[1][i];
		if(maxVal[i] < machOut[2][i]) maxVal[i] = machOut[21][i];
		if(pIn[i] > pOut[i]) maxVal[i] += (pIn[i] + 1);
		else maxVal[i] += (pOut[i] + 1);
		if(maxVal[i] > FOUND_STATE_SIZE) maxVal[i] = FOUND_STATE_SIZE;
	}
#else
	for(i = 0; i < 3 ; i++) {
		maxVal[i] = (pIn[i] + 1);
		for(j = 0; j < 3 ; j++) {
			maxVal[i] += machOut[j][i];
		}
		maxVal[i] += pOut[i];
		if(maxVal[i] > FOUND_STATE_SIZE) maxVal[i] = FOUND_STATE_SIZE;
	}
#endif
	memset(&(foundState[0]), 0xff, FOUND_STATE_AR_SZ * sizeof(DWORD));
	foundState[pIn[0]*FOUND_STATE_SIZE*FOUND_STATE_SIZE + pIn[1]*FOUND_STATE_SIZE + pIn[2]] = 0;
	pVQHead = pVQTail = (PVET_STEP)malloc(sizeof(VET_STEP));
	if(pVQHead == NULL) {
		return -100;
	}
	memset(pVQHead, 0, sizeof(VET_STEP));
	for(i = 0; i < 3; i++) {
		pVQHead->state[i] = pIn[i];
	}
	while(pVQHead != NULL) {
		ret = ProcessHead(pOut);
		if(ret < 0) {
			CleanQ();
			return ret;
		} else if(ret > 0) {
			CleanQ();
			return ret;
		}
	}
	return -1;
}

int main()
{
	int nprob, curprob, index, result, subcount, subInd, i;

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
			fprintf(stderr, "Read failed on problem %d data\n", curprob);
			return -3;
		}
		// get prob num and subprob count
		if(sscanf(&(inbuf[0]), "%d %d", &index, &subcount) != 2)
		{
			fprintf(stderr, "scan failed on problem header problem index %d\n",
				curprob);
			return -6;
		}
		if(index != curprob)
		{
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				index, curprob);
			return -7;
		}
		if((subcount <= 0) || (subcount > MAX_SUB_COUNT)){
			fprintf(stderr, "problem index %d sub count %d out of range\n",
				index, curprob);
			return -8;
		}
		// read machine data 
		for(i = 0; i < 3; i++) {
			if(fgets(&(inbuf[0]), 255, stdin) == NULL)
			{
				fprintf(stderr, "Read failed machine %d problem %d data\n", i+1, curprob);
				return -9;
			}
			if(sscanf(&(inbuf[0]), "%d %d %d",
				&(machOut[i][0]), &(machOut[i][1]), &(machOut[i][2])) != 3)
			{
				fprintf(stderr, "scan failed on machine %d problem %d data\n",
					i+1, curprob);
				return -10;
			}
		}
		// print solution header
		printf("%d %d\n", index, subcount);
		// read and process subproblems
		for(subInd = 1; subInd <= subcount ; subInd++)
		{
			if(fgets(&(inbuf[0]), 255, stdin) == NULL)
			{
				fprintf(stderr, "Read failed sub problem %d problem %d data\n",
					subInd, curprob);
				return -11;
			}
			if(sscanf(&(inbuf[0]), "%d %d %d %d %d %d %d", &i,
				&(subIn[0]), &(subIn[1]), &(subIn[2]),
				&(subOut[0]), &(subOut[1]), &(subOut[2])) != 7)
			{
				fprintf(stderr, "scan failed on  sub problem %d  problem %d data\n",
					subInd, curprob);
				return -12;
			}
			result = MadVetSolve(subIn, subOut, subInd, curprob);
			if(result < -1) {
				fprintf(stderr, "MadVetSolve ret %d on problem %d\n",
					result, curprob);
				return -13;
			} else if(result == -1) {
				printf("%d NO SOLUTION\n", subInd);
			} else if(result == 0) {
				printf("%d 0\n", subInd);
			} else {
				printf("%d %d %s\n", subInd, result, &(outbuf[0]));
			}
		}
	}
	return 0;
}
