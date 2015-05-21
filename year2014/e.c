/*
 * E - Mancala
 * ICPC 2014 Greater NY Regional
 * Solution by Fred Pickel
 * Problem by Fred Pickel
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

/*
 * we compute all winning boards inductively then return whichever solution is requested
 * If we start woith a winning board and play the smallest index harvetable bin,
 * the reulst is a harvestable board with one less token
 * we build winning boards in order of increasing total count by "un-palying" a winning
 * board to get the next larger winning board
 * the reuslt of harvesting bin k is that bin k has no tokens and each bin j j < k has one more token
 * to un-play, we find the smallest empty bin k in the current winning board
 * set its count to its index and subtract 1 form the count for each bin j < k
 * bins j > k have the same count as the previous winnning board
 * mancalaCnt[count] is the index of the last non-empty bin in the winnng board of count tokens
 * mancala[count][i] is the number of tokens in bin i in the winnng board of count tokens (i = 1 .. mancalaCnt[count])
 */
#define MAX_INPUT	2000
#define MAX_BINS	80
int mancala[MAX_INPUT+1][MAX_BINS+1];
int mancalaCnt[MAX_INPUT+1];

char inbuf[256];

int MakeMancala()
{
	int i, j, slot, maxslot;
	mancalaCnt[1] = maxslot = 1;
	mancala[1][1] = 1;
	for(i = 2; i <= MAX_INPUT ; i++) {
		slot = mancalaCnt[i] = mancalaCnt[i-1] + 1;
		for(j = 1; j <= mancalaCnt[i-1]; j++) {
			if(mancala[i-1][j] == 0){
				slot = j;
				mancalaCnt[i] = mancalaCnt[i-1];
				break;
			}
		}
		if(slot > MAX_BINS) {
			printf("bin overflow at count %d\n", i);
			return -1;
		}
		if(slot > maxslot) {
			maxslot = slot;
		}
		for(j = 1; j < slot ; j++) {
			mancala[i][j] = mancala[i-1][j] - 1;
		}
		mancala[i][slot] = slot;
		for(j = slot+1; j <= mancalaCnt[i] ; j++) {
			mancala[i][j] = mancala[i-1][j];
		}
	}
//	printf("max slot %d\n", maxslot);
	return 0;
}

int main()
{
	int nprob, curprob, index, count, i;

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
	if(MakeMancala() < 0) {
		return -20;
	}
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d data\n", curprob);
			return -3;
		}
		// get prob num and sequence index
		if(sscanf(&(inbuf[0]), "%d %d", &index, &count) != 2)
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
		if((count < 1) || (count > MAX_INPUT)) {
			fprintf(stderr, "problem index %d count %d not in range 1-%d\n",
				curprob, count, MAX_INPUT);
			return -7;
		}
		printf("%d %d\n", curprob, mancalaCnt[count]);
		for(i = 1; i <= mancalaCnt[count] ; i++) {
			printf("%d", mancala[count][i]);
			if(i < mancalaCnt[count]) {
				if((i % 10) == 0){
					printf("\n");
				} else {
					printf(" ");
				}
			}
		}
		printf("\n");
	}
	return 0;
}
