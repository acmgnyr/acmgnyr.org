/*
 * E - Deranged exams
 * ICPC 2013 Greater NY Regional
 * Solution by Fred Pickel
 * Problem by Fred Pickel
 */
#include <stdio.h>

/*
 * *sigh* Windows and Linux use different format specifiers for
 * 64 bit int's, not to mention different datatypes.
 */
#ifdef WIN32
typedef _int64 INT64;
#define	PRINT_FORMAT	"%d %I64u\n"
#else
#define	PRINT_FORMAT	"%d %llu\n"
typedef long long INT64;
#endif

INT64 dvals[21][21];

void InitDvals()
{
	int i, j;
	INT64 fact;
	fact = 1;
	for(i = 0; i <= 17 ; i++) {
		dvals[i][0] = fact;
		fact *= (i+1);
		for(j = 1; j <= i ; j++) {
			dvals[i][j] = -1;	// mark as not yet computed
		}
	}
};

// S(N,0) = number of ways to answer questions with no restricitons on wrong answers = N! (see InitDvals above
// S(N, K) = number of ways to answer questions with first K wrong
// S(N, K-1) = number of ways to answer questions with first (K-1) wrong
// what can happen at the Kth question
// if the answer is wrong it counts toward S(N,K)
// if the answer is correct, consider dropping both the question and the answer
// and we have a way of answering N-1 questions with K-1 correct (and thsi is one-to-one
// thus S(N,K1) = S(N,K) + S(N-1,K-1) = answers with first K-1 wrong and Kth wrong or Kth correct
// OR S(N,K) + S(N, K-1) - (S(N-1,K-1) giving a double recursion
INT64 Deranged(int nquest, int firstWrong)
{
	if(dvals[nquest][firstWrong] > 0) {
		return dvals[nquest][firstWrong];
	} else {
		dvals[nquest][firstWrong] = Deranged(nquest, firstWrong - 1) - Deranged(nquest-1, firstWrong - 1);
		return dvals[nquest][firstWrong];
	}
}


char inbuf[256];

int main()
{
	int nprob, curprob, index, nquest, firstWrong;
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
	InitDvals();
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d size\n", curprob);
			return -3;
		}
		if(sscanf(&(inbuf[0]), "%d %d %d", &index, &nquest, &firstWrong) != 3)
		{
			fprintf(stderr, "Scan failed on problem %d data\n", curprob);
			return -4;
		}
		if(index != curprob)
		{
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				index, curprob);
			return -7;
		}
		if((nquest < 1) || (nquest > 17))
		{
			fprintf(stderr, "problem index %d number of questions %d not in range 1-20\n",
				curprob, nquest);
			return -9;
		}
		if((firstWrong < 0) || (firstWrong > nquest))
		{
			fprintf(stderr, "problem index %d initial worng answers %d not in range 0-%d\n",
				curprob, firstWrong, nquest);
			return -10;
		}
		result = Deranged(nquest, firstWrong);
		printf(PRINT_FORMAT, curprob, result);
	}
	return 0;
}