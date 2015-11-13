/*
 * B - Running Steps solution
 * ICPC 2015 Greater NY Regional
 * Solution by Fred Pickel
 * Problem by Fred Pickel
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_STEPS 100

char inbuf[256];

/*
 * *sigh* Windows and Linux use different format specifiers for
 * 64 bit int's, not to mention different datatypes.
 */
#ifdef WIN32
typedef unsigned _int64 INT64;
#define	PRINT_FORMAT	"%d %I64u\n"
#else
#define	PRINT_FORMAT	"%d %llu\n"
typedef unsigned long long INT64;
#endif

// for 2*n steps each foot will take k single strides and (n-k)/2 double strides
// where k goes thru odd integers <= floor(n/3) for n odd
// and thru even integers <= floor(n/3) for n even
// each foot taking a total of (k + (n-k)/2) = (n+k)/2 strides
// for a given k, there are Comb(k + (n-k)/2, k) choices of which left foot strides are single strides
// and Comb(k + (n-k)/2, k) choices of which right foot strides are single strides
// for a total of (Comb(k + (n-k)/2, k))^2 choices with k single strides
// the total number of choices is
// SUM (k = 1, k <= floor(n/3), k += 2) ((Comb((n+k)/2, k))^2) for n odd and
// SUM (k = 0, k <= floor(n/3), k += 2) ((Comb((n+k)/2, k))^2) for n even
// Comb(N,K) = (N*...*(N-K+1))/(1*...*K)
// Comb(N+1,K+2) + ((N+1)*N*...*(N-K+1)*((N+1)-(K+2)+1))/(1*...*K*(K+1)*(K+2) =
// ((N+1)*N*...*(N-K+1)*(N-K))/(1*...*K*(K+1)*(K+2) = Comb(N,K)*(N+1)*(N-K)/((K+1)*(K+2))
INT64 CountStrideOrders(int steps)
{
	INT64 result, comb, t1, t2;
	int strides, lim, oneSteps;
	// init
	oneSteps = (steps & 1);
	if(oneSteps == 1) {
		strides = 1 + steps/2;
		comb = strides;
	} else {
		strides = steps/2;
		comb = 1;
	}
	result = 0;
	lim = steps/3;
	for( ; oneSteps <= lim ; strides++, oneSteps += 2) {
		result += comb*comb;
		t1 = (strides+1) * (strides-oneSteps);
		t2 = (oneSteps+1) * (oneSteps+2);
		comb = comb * t1 / t2;
	}
	return result;

}

int main()
{
	int nprob, curprob, index, steps;
	INT64 result;

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
		if(sscanf(&(inbuf[0]), "%d %d %d", &index, &steps) != 2)
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
		if((steps < 0) || (steps > MAX_STEPS)){
			fprintf(stderr, "problem index %d steps %d out of range\n",
				curprob, steps);
			return -8;
		}
		if((steps & 1) != 0) {
			fprintf(stderr, "problem index %d steps %d is odd\n",
				curprob, steps);
			return -8;
		}
		result = CountStrideOrders(steps/2);
		printf(PRINT_FORMAT, curprob, result);
	}
	return 0;
}
