/*
 * F - King's Ups and Downs
 * ICPC 2012 Greater NY Regional
 * Solution by Fred Pickel
 * Problem by Fred Pickel
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

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

#define MAX_GUARDS	20

INT64 comb[MAX_GUARDS][MAX_GUARDS];
INT64 Tall[MAX_GUARDS+1];

/*
 *	If we let Tall(n) be the number of up-down sequences of 1..n with first element > second
 *	and Short(n) be the number of up-down sequences of 1..n with first element < second 
 *	then Short(n) = Tall(n) [ for each item in a short sequnce replace k with n+1-k to get the corresponding tall sequnce
 *	The result is 1 if n = 1
 *  else it is Tall(n) + Short(n) = 2*Tall(n)
 *  Setting Tall[0] = 1, we also have Tall[1] = Tall[2] = 1
 *	For larger n, to find Tall(n), item n (the tallest) must be in a "taller" position
 *	1, 3, 5,...
 *  Once we have chosen a position, k, for the tallest item, we must choose k-1 items to put left of the tallest
 *	and arrange those (k-1) items in up down order Tall first
 *	and we must arrange the remianing items in up-down order shorter first
 *	The total number of ways of doing this (for a fixed k) is
 *	Comb(n-1,k-1) * Tall(k-1) * Short(n-k) = Comb(n-1,k-1) * Tall(k-1) * Tall(n-k)
 *  So Tall(n) =
 *	SUM (k = 1; K <= n ; k += 2) [Comb(n-1,k-1) * Tall(k-1) * Tall(n-k)]
 */
int makeComb()
{
	int i, n;
	comb[0][0] = 1;
	for(i = 1; i < MAX_GUARDS ; i++) {
		comb[0][i] = 0;
	}
	for(n = 1; n < MAX_GUARDS ; n++) {
		comb[n][0] = 1;
		for(i = 1; i < MAX_GUARDS ; i++) {
			comb[n][i] = comb[n-1][i-1] + comb[n-1][i];
		}
	}
	return 0;
}

int initTall()
{
	int i;

	Tall[0] = Tall[1] = Tall[2] = 1;
	for(i = 3; i <= MAX_GUARDS ; i++)
	{
		Tall[i] = -1;
	}
	return 0;
}

INT64 GetTall(int n)
{
	INT64 ret = 0;
	int loc;
	if(n > MAX_GUARDS) return -1;
	if(Tall[n] > 0) return Tall[n];
	for(loc = 1; loc <= n ; loc += 2) {
		ret += GetTall(loc - 1) * comb[n-1][loc - 1] * GetTall(n-loc);
	}
	return ret;
}

char inbuf[256];

int main()
{
	int nprob, curprob, index, guardCnt;
	INT64 res;

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
	makeComb();
	initTall(0);
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d data header\n", curprob);
			return -3;
		}
		// get prob num and count of guards
		if(sscanf(&(inbuf[0]), "%d %d", &index, &guardCnt) != 2)
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
		if((guardCnt < 1) || (guardCnt > MAX_GUARDS)) {
			fprintf(stderr, "problem index %d guard count %d not in range 1-%d\n",
				curprob, guardCnt, MAX_GUARDS);
			return -7;
		}
		if(guardCnt == 1) {
			res = 1;
		} else {
			res = 2*GetTall(guardCnt);
		}
		printf(PRINT_FORMAT, curprob, res);
	}
	return 0;
}
