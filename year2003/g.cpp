// RhymeSchemes.cpp : Defines the entry point for the console application.
//


#include <stdio.h>
#include <stdlib.h>
#include <math.h>


char inbuf[128];

// recursive computation of R(n,k)
// = number of rhyming patterns for a stanza of n lines
// with k rhyming ends
// R(n,1) = R(n,n) = 1
// R(n+1,k) = k*R(n,k) + R(n,k-1)
// to get a stanza of n+1 lines with k rhyming ends
// the last rhyming end is either has previously occurred
// so can be obtained by tacking one of k ends onto
// a pattern of n lines with k ends (k*R(n,k))
// OR the last rhyme end is differnet from the previous
// so you have any stanze of n lines and k-1 ends folowed by a new end
// then total is sum k = 1 to n(R(n,k))
double *stirp = NULL;
int stirpsz = 0;
double stirling(int n)
{
	int sz, i, j;
	double *dp1, *dp2, result;

	if(n <= 0)
	{
		return -1.0;
	}
	if(stirpsz < (n+1)*(n+1))
	{
		if(stirp != NULL)
		{
			delete [] stirp;
			stirp = NULL;
			stirpsz = 0;
		}
		sz = 4*n*n;
		stirp = new double[sz];
		if(stirp == NULL)
		{
			return -1.0;
		}
		stirpsz = sz;
	}
	// do s(m,1) = s(m,m) = 1
	dp1 = dp2 = stirp;
	for(i = 0; i < n ; i++)
	{
		*dp1 = *dp2 = 1.0;
		dp1 += n;
		dp2 += (n+1);
	}
	// start with m = 3 and compute R(m,k) = k * R(m-1,k) + R(m-1,k-1)
	for(j = 3; j <= n; j++)
	{
		dp1 = stirp + (j-2)*n;	// R(j-1,1)
		dp2 = stirp + (j-1)*n + 1;	//R(j, 2)
		for(i = 2; i < j; i++)
		{
			*dp2 = *dp1 + ((double)i)*dp1[1];
			dp1++;
			dp2++;
		}
	}
	// result = sum(k = 1 to n)(R(n,k))
	result = 0;
	dp1 = stirp + (n-1)*n;
	for(j = 1; j <= n; j++)
	{
		result += *dp1;
		dp1++;
	}
	return result;
}

// compute total = Bell(n) iteratively using the formula
// Bell(n+1) = Sum(k = 0 to n) [B(n.k) * Bell(k)];
// where B(n,k) is the binomial coefficient
double *bellp = NULL;
int bellsz = 0;
int lastbell = -1;

double bell(int n)
{
	double *dp, *dp1, *dp2, result, bnk;
	int sz, i;

	if(n <= 0)
	{
		return -1;
	}
	if(n <= lastbell)
	{
		return bellp[n];
	}
	if(n >= bellsz)
	{
		sz = 2*n;
		if(sz < 10) sz = 10;
		dp = new double[sz];
		if(dp == NULL)
		{
			return -1.0;
		}
		if(bellp != NULL)
		{	// copy in previously computed values
			dp1 = bellp; dp2 = dp;
			for(i = 0; i <= lastbell ; i++)
			{
				*dp2++ = *dp1++;
			}
			delete [] bellp;
			bellp = dp;
		}
		else
		{	// init the first few
			bellp = dp;
			bellp[0] = bellp[1] = 1.0;
			bellp[2] = 2.0;
			bellp[3] = 5.0;
			bellp[4] = 15.0;
			lastbell = 4;
		}
		bellsz = sz;
	}
	// array is big enough keep computing new bells until we get to n
	for(; lastbell < n; lastbell++)
	{
		bnk = 1.0;
		dp = bellp;
		result = 0.0;
		for(i = 0; i <= lastbell ; i++)
		{
			result += bnk * (*dp);
			dp++;
			bnk *= ((double)(lastbell - i));
			bnk /= ((double)(i+1));
		}
		*dp = result;
	}

	return bellp[n];
}

int main()
{
	int n;
	double b1, b2;
    FILE *fp = fopen("g.in", "rt");
    if(fp == NULL){
        printf("Can't open input g.in\n");
        return 1;
    }

	while(fgets(&(inbuf[0]), 128, fp) != NULL)
	{
		n = ::atoi(&(inbuf[0]));
		if(n == 0)
		{
			return 0;
		}
		b1 = stirling(n);
		b2 = bell(n);
		printf("%d %.0f\n", n, b2);
	}
    fclose(fp);
	return -1;
}
