/*
 * C - M-ary Partitions
 * Solution by Fred Pickel
 * ACM ICPC GNYR 2016
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_PROB	1000
#define MAX_N	10000
#define MAX_M	100
typedef unsigned long DWORD;
typedef unsigned short WORD;

char inbuf[256];

/*
 *  assume partitions terms are written in descending order of value e.g. 9+3+3=+1+1
 *	if n = m*n1 + r (0 <= r < m, we get the m-ary partitions of n from those of n1 by:
 *	1. for each partion of n1, multiply each term in the partition by m and tack on r 1'sat the end
 *	2. for each partition of n1 which ends in k 1's, we multiply each term by m and add r 1's 
 *		as in 1. to get a partition ending in k m's followed by r 1's, we then replace i of the
 *		trailing m's by m 1's (i = 1 .. k) to obtain k new partitions of n
 *	Thus the total number of m-ary partitions of n is the number of m-ary partitions of n1 PLUS
 *	the number of trailing 1's in all the partitions of n1.
 *  NOTES:
 *	1. NumPartitions(n, m) = NumPartitions((n/m)*m)	i.e the remainder does not affect the count
 *	2. if n = a[0] + a[1]*m + ... + a[k]*m^k then
 *		NumPartitions(n*m, m) = Product[i = 0..k, (1+a[i])] mod m		Note the n*m
 *	3. The code with check1 and check2 is just using 2 above to verify results and is not required
 *		for the solution (since you probably did not know 2)
 */

DWORD oneCount[2][MAX_N];
DWORD FindMPartions(int n, int m, int level, DWORD *chk1, DWORD *chk2)
{
	DWORD total, i, j, n1, rem, rem1, subcnt, check1, check2;
	DWORD *pNew, *pPrev;
	if(n < m) {
		memset(&(oneCount[level & 1][0]), 0, MAX_N*sizeof(DWORD));
		oneCount[level & 1][n] = 1;
		*chk1 = *chk2 = 1;
		return 1;
	}
	n1 = n/m;
	rem = n % m;
	rem1 = n1 % m;
	subcnt = FindMPartions(n1, m, level+1, &check1, &check2);
	check2 = (check2 * (1+rem1)) % m;
	total = subcnt;
	memset(&(oneCount[level & 1][0]), 0, MAX_N*sizeof(WORD));
	pNew = &(oneCount[level & 1][0]);
	pPrev = &(oneCount[(level+1) & 1][0]);
	pNew[rem] = subcnt;		// all the partitions that are just m*partition of n1 get rem 1's at the end
	for(i = 1; i <= n1 ; i++) {	// for all the partions of n1 with at least one 1 at the end, expand
		total = (total + i*pPrev[i]);	// for each partion of n1 ending i 1's, get i partions of n
		check1 = (check1 + i*pPrev[i]) % m;
		if(pPrev[i] > 0) {
			for(j = 1; j <= i ; j++) {	// for each j <= i, we replace j m'a with j*m 1's and tack on rem 1's
				pNew[j*m+rem] += pPrev[i];
			}
		}
	}
	*chk1 = check1;
	*chk2 = check2;
	if(check1 != check2) {
		printf("Chk1 %u chk2 %u n %d m %d\n", check1, check2, n, m);
	}
	return total;
}

int main()
{
	int nprob, curprob, index;
	int m, n;
	DWORD result, check1, check2;
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
	if((nprob < 5) || (nprob > MAX_PROB)) {
		fprintf(stderr, "problem count %d not in range 5 .. %d\n", nprob, MAX_PROB);
		return -20;
	}
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d header\n", curprob);
			return -3;
		}
		if(sscanf(&(inbuf[0]), "%d %d %d", &index, &m, &n) != 3) {
			fprintf(stderr, "scan failed on problem %d dat\n", curprob);
			return -4;
		}
		if(index != curprob) {
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				index, curprob);
			return -5;
		}
		if((m < 3) || (m > MAX_M) || (n < 2) || (n > MAX_N)) {
			fprintf(stderr, "problem %d: m %d out of rande [2, %d] or n %d out of range [2, %d]\n",
				index, m, MAX_M, n, MAX_N);
			return -6;
		}
		result = FindMPartions(n, m, 0, &check1, &check2);
		printf("%d %u\n", curprob, result);
	}
	return 0;
}
