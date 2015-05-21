/*
 * ACM2006 Greater NY Region C++ Solution to B - Triangular Numbers
 *
 * John Buck
 * October, 2006
 */
#include <stdio.h>
#include <stdlib.h>

/*
 * Define MAGIC_FORMULA if you want to see the real easy way
 */
//#define	MAGIC_FORMULA

int main()
{
	int d, n, k, i, sum, t;
	char ibuf[256];

	/* Get number of datasets */
	if(::fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
		return(1);
	}
	d = ::atoi(&(ibuf[0]));
	/* For each dataset */
	for(i = 1; i <= d; i++){
		/* Get n */
		if(::fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
			break;
		}
		n = ::atoi(&(ibuf[0]));
		/* Calculate sum: recall: T(n) = T(n-1) + n */
		sum = 0;
		/* T(1) = 1 */
		t = 1;
		for(k = 1; k <= n; k++){
			/* Next T */
			t = t + (k+1);
			sum += k*t;
		}
#ifdef MAGIC_FORMULA
		::printf("%d %d %d   (Formula: %d)\n", i, n, sum,
			(n*(n+1)*(n+2)*(n+3))/8);
#else
		::printf("%d %d %d\n", i, n, sum);
#endif
	}
	return(0);
}