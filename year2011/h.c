/*
 * X - Maximum in the Cycle of 1 - Solution
 * Greater NY Regional ACM Contest 2012
 * Solution by Fred Pickel
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

/*
 * Max number of permumations specified in problem
 */
#define MAX_PERM_SZ	20

/*
 * Array to remember factorials
 */
double Factorial[MAX_PERM_SZ + 1];

/*
 * Create factorials up to maximum number of permutations
 */
void MakeFactorial()
{
	int i;

	Factorial[0] = Factorial[1] = 1.0;
	for(i = 1 ; i < MAX_PERM_SZ ; i++) {
		Factorial[i+1] = ((double)(i+1)) * Factorial[i];
	}
}

/*
 * If max of the cycle is 1, then P(1) = 1 and the remaining
 * n-1 values are permuted arbitrarily so count = (n-1)!
 * Otherwise, there are j different values in the cycle of 1
 * 1, k and j-2 other values in the range 2 to k - 1. The remaining
 * n-j values are permuted arbitrarily so number of permutations
 * with k = max in the cycle of 1 and j elements in the cycle of 1
 * choose j-2 out of k-2 * permutations of j-1 * permutations of n-j =
 * ((k-2)!/(j-2)! * (k-j)!)) * (j-1)! * (n-j)!
 * to get the total count, we add these values for j = 2 to k
 */
double OneCycleMaxCount(int n, int k)
{
	double ret, cur;
	int j;

	if(k == 1){
		return Factorial[n-1];
	}
	ret = 0.0;
	for(j = 2; j <= k ; j++) {
		cur = (Factorial[k-2]/(Factorial[j-2]*Factorial[k-j]))*Factorial[j-1]*Factorial[n-j];
		ret += cur;
	}
	return ret;
}

/*
 * Alternate magic formula
 */
double OneCycleCheck(int n, int k)
{
	if(k == 1){
		return Factorial[n-1];
	}
	return Factorial[n]/(((double)(n-k+1))*((double)(n-k+2)));
}

int main()
{
	int nprob, curprob, index, permsz, cyclemax;
	double result;
	char inbuf[256];

	if(fgets(&(inbuf[0]), sizeof(inbuf)-1, stdin) == NULL)
	{
		fprintf(stderr, "Read failed on problem count\n");
		return -1;
	}
	if(sscanf(&(inbuf[0]), "%d", &nprob) != 1)
	{
		fprintf(stderr, "Scan failed on problem count\n");
		return -2;
	}
	MakeFactorial();
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d data\n", curprob);
			return -3;
		}
		// get prob num and subprob count
		if(sscanf(&(inbuf[0]), "%d %d %d", &index, &permsz, &cyclemax) != 3)
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
		if((permsz <= 0) || (permsz > MAX_PERM_SZ)){
			fprintf(stderr, "problem index %d permutation size %d out of range\n",
				curprob, permsz);
			return -8;
		}
		if((cyclemax < 1) || (cyclemax > permsz)){
			fprintf(stderr, "problem index %d cycle max %d not between 1 and %d\n",
				curprob, cyclemax, permsz);
			return -9;
		}
		result = OneCycleMaxCount(permsz, cyclemax);
		printf("%d %.0f\n", curprob, result);
	}
	return 0;
}
