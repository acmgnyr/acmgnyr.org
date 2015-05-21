/*
 * A - Hailstone HOTPO solution
 * ICPC 2012 Greater NY Regional
 * John Buck
 * Problem idea by Adam Florence
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * There isn't much to this problem.  Simply compute the sequence, keeping
 * track of the largest value (imax).  When the sequence hits 1, we're done.
 * Print the result.
 */
int
main(int argc, char **argv)
{
	int i, k, p, n, imax;

	char szBuf[128];
	/*
 	 * See how many problems there are
	 */
	if(fgets(&(szBuf[0]), sizeof(szBuf)-1, stdin) != NULL){
		k = atoi(&(szBuf[0]));
		/*
		 * For each problem, process independently and identically
		 */
		for(i = 1; i <= k; i++){
			/*
			 * Read data set.  We read the whole line and
			 * deal with it using sscanf below.  This way we
			 * don't have to worry about newlines, etc.
			 */
			if(fgets(&(szBuf[0]), sizeof(szBuf)-1, stdin) == NULL){
				break;
			}
			/*
			 * Convert to values we need: Dataset number, starting point
			 */
			if(sscanf(&(szBuf[0]), "%d %d", &(p), &(n)) != 2){
				/*
				 * Bail on errors
				 */
				break;
			}
			if(p != i){
				printf("invalid dataset number %d, expecting %d\n", p, k);
				break;
			}
			/*
			 * Start max off as the starting point
			 */
			imax = n;
			/*
			 * As long as there's terms in the sequence, get the next term
			 */
			while(n > 1){
				/*
				 * Odd value?
				 */
				if(n & 1){
					n = 3*n + 1;
				} else {
					/*
					 * Even value
					 */
					n /= 2;
				}
				/*
				 * Do we have a new max?
				 */
				if(n > imax){
					 imax = n;
				}
			}
			/*
			 * Print result
			 */
			printf("%d %d\n", p, imax);
		}
	}
	
	return(0);
}

