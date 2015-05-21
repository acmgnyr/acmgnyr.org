#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_PROBS	1000

int pWid;
char inbuf[256];

int maxval = 16384 * 8192;

int F[101], G[101], H[101];

int compVals()
{
	int i;
	F[0] = 1; F[1] = 1; G[0] = 0; G[1] = 1; H[0] = 0; H[1] = 1;
	for(i = 2; i < 101 ; i++)
	{
		F[i] = F[i-1] + F[i-2] + G[i-1] + 2*H[i-1];
		G[i] = F[i-1] + G[i-2];
		H[i] = F[i-1] + H[i-1];
		if(F[i] >= maxval)
		{
			return i;
		}
	}
	return 100;
}

/*
 * F[n] = number of ways to tile a 4-by-n grid
 * G[n] = number of ways to tile a 4-by-n grid with top-right and bottom-right
 *			squares uncovered
 * H[n] = number of ways to tile a 4-by-n grid with bottom-right 2
 *			squares uncovered
 *      = number of ways to tile a 4-by-n grid with top-right 2
 *			squares uncovered
 * if n >= 2, the right end of any tiling can be
 *	two vertical dominoes (F[n-1] ways)
 *	horz, horz vert (H[n-1] ways)
 *	horz, vert, horz (G[n-1] ways)
 *	vert, horz, horz (H[n-1] ways)
 *	4 horizontal dominoes (F[n-2] ways)
 * F[n] = F[n-1] + G[n-1] + 2*H[n-1] + F[n-2];
 * For G: the right end can be a vertical domino (F[n-1] ways)
 *		or two horizontal dominoes => top & bottom are horz = G[n-2]
 *		G[n] = F[n-1] + G[n-2];
 * For H: the right end can be a vertical domino (F[n-1] ways)
 *		or two horizontal dominoes (H[n-1] ways)
 *		H[n] = F[n-1] + H[n-1];
 * F[0] = 1, F[1] = 1, G[0] = 0, G[1] = 1, H[0] = 0, H[1] = 1
 */
int main()
{
	int probCnt, curProb, ret;

	ret = compVals();
/*	printf("compVals ret %d F = %d\n", ret, F[ret]); */
	if(fgets(&(inbuf[0]), 255, stdin) == NULL)
	{
		fprintf(stderr, "read failed on problem count\n");
		return -1;
	}
	inbuf[255] = 0;
	probCnt = atoi(&(inbuf[0]));
	if((probCnt < 1) || (probCnt > MAX_PROBS))
	{
		fprintf(stderr, "Problem count %d not in range 1...%d\n", probCnt, MAX_PROBS);
		return -2;
	}
	for(curProb = 1; curProb <= probCnt ; curProb++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "read failed on problem %d\n", curProb);
			return -3;
		}
		if(sscanf(&(inbuf[0]), "%d", &pWid) != 1)
		{
			fprintf(stderr, "scan failed on problem %d\n", curProb);
			return -4;
		}
		if(pWid > ret)
		{
			fprintf(stderr, "width %d too large problem %d\n", pWid, curProb);
			return -5;
		}
		printf("%d %d\n", curProb, F[pWid]);
	}
	return 0;
}
