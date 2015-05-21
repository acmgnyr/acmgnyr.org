/*
 * ACM2006 Greater NY Region C++ Solution to H - Non-divisible 2-3 Power Sums
 *
 * Fred Pickel
 * October, 2006
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Exp2_3
{
	int exp_2;
	int exp_3;
};

Exp2_3 ExpData[32];

int pow3[20];

void
InitPow3()
{
	int i, n;

	for(i = 0, n = 1; i < 20 ; i++, n *= 3)
	{
		pow3[i] = n;
	}
}

// printf exp data in required format
void
PrintExpData(int nProb, int count)
{
	int i;

	printf("%d %d ", nProb, count);
	for(i = 0; i < (count - 1) ; i++)
	{
		printf("[%d,%d] ", ExpData[i].exp_2, ExpData[i].exp_3);
	}
	printf("[%d,%d]\n", 
		ExpData[count - 1].exp_2, ExpData[count - 1].exp_3);
}

// convert result for X to result for 2*x
void
TimesTwo(int start, int count)
{
	int i;

	for(i = 0; i < count ; i++)
	{
		ExpData[i+start].exp_2++;
	}
}

// recursive routine to find sum23 a sum 23 repn
// of n into exp data starting at start
// return number of terms
int
FindExp23Rep(int n, int start)
{
	int ret, k, pow3k;

	if(n == 0)
	{
		return 0;	// no terms added
	}
	else if(n == 1)
	{	// quotient is 1, just put in 0,0
		ExpData[start].exp_2 = 0;
		ExpData[start].exp_3 = 0;
		return 1;
	}
	else if((n % 2) == 0)
	{	// if even, do rep for n/2 and raise all 2 exps by 1
		ret = FindExp23Rep(n/2, start);
		TimesTwo(start, ret);
		return ret;
	}
	else
	{	// n is odd and > 1, find largestpower of 3 <= n
		k = (int)((log((double)n + .01))/(log(3.0)));
		if((k <= 0) || (k >= 20))
		{
			fprintf(stderr, "log %d too large or <= 0\n", k);
			return 0;
		}
		pow3k = pow3[k];
		ExpData[start].exp_2 = 0;
		ExpData[start].exp_3 = k;
		ret = FindExp23Rep(n - pow3k, start+1) + 1;
		return ret;
	}
}

char inbuf[256];
int main()
{
	int n, count, nProb, i;

	InitPow3();

	if(fgets(&(inbuf[0]), sizeof(inbuf), stdin) == NULL){
		return -1;
	}
	nProb = atoi(&(inbuf[0]));
	for(i = 1; i <= nProb; i++){
		if(fgets(&(inbuf[0]), sizeof(inbuf), stdin) == NULL){
			fprintf(stderr, "Read error on stdin\n");
			break;
		}
		n = atoi(&(inbuf[0]));
		count = FindExp23Rep(n, 0);
		PrintExpData(i, count);
	}
	return 0;
}