// candygame.cpp : Defines the entry point for the console application.
//

#include <stdio.h>


char inbuf[256];

int *CurVals = NULL;
int *TmpVals = NULL;
int maxsize;

void cleanup()
{
	if(CurVals != NULL)
	{
		delete [] CurVals;
		CurVals = NULL;
	}
	if(TmpVals != NULL)
	{
		delete [] TmpVals;
		TmpVals = NULL;
	}
}

// actually play the game, return number of cycles
int PlayGame(int gmsize)
{
	int max, min, cycles, i;

	cycles = 0;
	max = min = CurVals[0];
	for(i = 1; i < gmsize ; i++)
	{
		if(CurVals[i] > max) max = CurVals[i];
		else if(CurVals[i] < min) min = CurVals[i];
	}
	while(min < max)
	{
		cycles++;
		i = min;
		min = max;
		max = i;
		// pass to right (= receive from left)
		TmpVals[0] = CurVals[gmsize - 1]/2 + CurVals[0]/2;
		for(i = 1; i < gmsize; i++)
		{
			TmpVals[i] = CurVals[i - 1]/2 + CurVals[i]/2;
		}
		// add 1 for odd and move back to curvals
		for(i = 0; i < gmsize ; i++)
		{
			if((TmpVals[i] & 1) != 0) CurVals[i] = TmpVals[i] + 1;
			else CurVals[i] = TmpVals[i];
			if(CurVals[i] > max) max = CurVals[i];
			if(CurVals[i] < min) min = CurVals[i];
		}
	}
	return cycles;
}

int main()
{
	char *pc;
	int curcnt, gmcnt, i, curval, gmcycles;
    FILE *fp = fopen("b.in", "rt");
    if(fp == NULL){
        printf("Can't open input b.in\n");
        return 1;
    }

	CurVals = new int[256];
	TmpVals = new int[256];
	maxsize = 256;
	if((CurVals == NULL) || (TmpVals == NULL))
	{
		fprintf(stderr, "Init workspace alloc failed\n");
		cleanup();
		return -1;
	}
	// get first count
	pc = fgets(&(inbuf[0]), 255, fp);
	if(pc == NULL)
	{
		fprintf(stderr, "Bad read on first count\n");
		cleanup();
		return -2;
	}
	if(sscanf(&(inbuf[0]), "%d", &curcnt) < 1)
	{
		fprintf(stderr, "Bad value convert on first count\n");
		cleanup();
		return -3;
	}
	gmcnt = 1;
	while(curcnt > 0)
	{
		// if not enough space allocate new space
		if(curcnt > maxsize)
		{
			maxsize = 2*curcnt;
			cleanup();
			CurVals = new int[maxsize];
			TmpVals = new int[maxsize];
			if((CurVals == NULL) || (TmpVals == NULL))
			{
				fprintf(stderr, "workspace realloc size %d failed\n", maxsize);
				cleanup();
				return -4;
			}
		}
		// read in game data
		for(i = 0; i < curcnt ; i++)
		{
			pc = fgets(&(inbuf[0]), 255, fp);
			if(pc == NULL)
			{
				fprintf(stderr, "Bad read on data %d game %d\n", i+1, gmcnt);
				cleanup();
				return -7;
			}
			if(sscanf(&(inbuf[0]), "%d", &curval) < 1)
			{
				fprintf(stderr, "Bad value convert on data %d game %d\n", i+1, gmcnt);
				cleanup();
				return -8;
			}
			// check that the value is even
			if((curval & 1) != 0)
			{
				fprintf(stderr, "Odd value %d on data %d game %d\n", curval, i+1, gmcnt);
				cleanup();
				return -9;
			}
			CurVals[i] = curval;
		}
		// play game
		gmcycles = PlayGame(curcnt);
		// output result
		printf("%d %d\n", gmcycles, CurVals[0]);
		// get next count
		gmcnt++;
		pc = fgets(&(inbuf[0]), 255, fp);
		if(pc == NULL)
		{
			fprintf(stderr, "Bad read on count %d\n", gmcnt);
			cleanup();
			return -5;
		}
		if(sscanf(&(inbuf[0]), "%d", &curcnt) < 1)
		{
			fprintf(stderr, "Bad value convert on count %d\n", gmcnt);
			cleanup();
			return -6;
		}
	}
	cleanup();
	return 0;
}