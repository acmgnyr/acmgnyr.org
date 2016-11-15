/*
 * E - Permutation Descent Counts
 * Solution by Fred Pickel
 * ACM ICPC GNYR 2016
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define MAGIC_MODULUS	(1001113)

typedef unsigned long DWORD;

#define MAX_ORDER	100
#define NOT_YET_SET (0xffffffff)
char inbuf[256];

DWORD descnts[MAX_ORDER+1][MAX_ORDER];

void InitDescnt(void)
{
	int i, j;
	for(i = 0; i <= MAX_ORDER ; i++) {
		for(j = 0; j < MAX_ORDER ; j++) {
			descnts[i][j] = NOT_YET_SET;
		}
	}
	for(i = 1; i <= MAX_ORDER ; i++) {
		descnts[i][0] = 1;
		descnts[i][i-1] = 1;
	}
}

// to convert a permutation 0f 1...(n-1) to a permutation of 1...n
// we can insert n in any of n places in the original permutation
// (beginning, end or between any two adjacent elements in the permutation)
// if we insert n between two elements which form a descent, no new descent is created
// if we add n to the end, no new descent is created
// otherwise a new descent is created
// thus from a permutaion of order n-1 with d descents we get
// d+1 permutations of order n with d descents AND
// n-d-1 permeutations of order n with d+1 descents
//
// to get a permutation of order n with d descents we can
// take any permutation of order n-1 and d descents and insert n between descents or at the end OR
// take any permutation of order n-1 and d-1 descents and insert n at the begining or not between descent terms
// so
// PDC(n,d) = (d+1)*PDC(n-1,d) +	// d descents + at the end
//				(n-d)*PDC(n-1,d-1)	// there are n-1 slots between, d are descents so (n-1-d) are not
//							`		// 	so n-d slots including at the beginning						
DWORD descent(int order, int count)
{
	if((count == 0) || (count == (order -1))) {
		return 1;
	}
	if(descnts[order][count] != NOT_YET_SET) {
		return descnts[order][count];
	}
	descnts[order][count] = (((count + 1)) * descent(order-1, count) +
		((order - count)) * descent(order-1, count-1)) % MAGIC_MODULUS;
	return descnts[order][count];
}

int main()
{
	int nprob, curprob, index, order, count;
	DWORD dcount;

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
	InitDescnt();
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d header\n", curprob);
			return -3;
		}
		// get prob num and order of sequence
		if(sscanf(&(inbuf[0]), "%d %d %d", &index, &order, &count) != 3)
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
		if((order < 1) || (order > MAX_ORDER)) {
			fprintf(stderr, "problem index %d order %d not in range 1-%d\n",
				curprob, order, MAX_ORDER);
			return -8;
		}
		if((count < 0) || (count > (order - 1))) {
			fprintf(stderr, "problem index %d count %d not in range 0-%d\n",
				curprob, count, order-1);
			return -8;
		}
		dcount = descent(order, count);
#ifdef WIN32
		printf("%d %lu\n", index, dcount);
#else
		printf("%d %llu\n", index, dcount);
#endif
	}
	return 0;
}
