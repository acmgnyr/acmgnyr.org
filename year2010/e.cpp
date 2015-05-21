/* 
 * E - Non-Decreasing Digits
 * ACM International Collegiate Programming Contest
 * Greater New York Region
 * October 24, 2010
 */

/* Define NO_MEMOIZE to not remember any previous computations.
 * This will cause it to blow time.
 */
//#define	 NO_MEMOIZE

#include <stdio.h>
#include <stdlib.h>

/* *sigh* */
#ifdef WIN32
typedef	_int64	bigint;
#define	BIGFMT	"%I64d"
#else
typedef long long bigint;
#define	BIGFMT	"%lld"
#endif

// Maximum base and number of digits
const int MAX_BASE = 10;
const int MAX_DIGITS = 64;

// Memoization
bigint array[MAX_BASE+1][MAX_DIGITS+1];

// Return number of numbers, written in base b with n digits, that have
// non-decreasing digits
bigint compute(const int b, const int n)
{
#ifndef NO_MEMOIZE
	// Take advantage of memoization.
	// Check to see if we've already computed this one.
	if ( array[b][n] >= 0 ){
		return array[b][n];
	}
#endif
	
	bigint sum = 0;
	
	// --------------
	// Base cases:
	// --------------
	// If we have 1 digit, can make b numbers: 0, 1, ..., (b-2), (b-1)
	if (n == 1){
		sum = b;
	} else if (b == 1){
		// In base 1 the only number we can make is all 0's
		sum = 1;
	/*
	 * Two more possible base cases are:
	 * // In base 2 there are n+1 numbers: n zeros, (n-1) zeros, ...
	 * //2 zeros, 1 zero, 0 zeros
	 * else if (b == 2)
	 *		sum = n+1;
	 * // With two digits, if first digit is i then there are (n-i) choices
	 * // for second digit: this is sum of numbers from 1 to n
	 * else if (n == 2)
	 *		sum = b * (b+1) / 2;
	 */
	// --------------
	// Degenerate base cases:
	// --------------
	} else if (n == 0){
		// If we have no digits we can't make any numbers
		sum = 0;
	} else if (b == 0){
		// Base 0 doesn't make any sense
		sum = 0;
	} else {
		// --------------
		// Recursive case
		// --------------
		for (int i = 0; i < b; ++i)
		{
			// Set the first digit to i. Then we have n-1 digits left and can
			// use the numbers i to b-1.
			sum += compute(b-i, n-1);
		}
		
	}
	// Memoize this solution
	array[b][n] = sum;
	
	// Return
	return sum;
}

int main(int argc)
{
	// Initialize memoization array to all -1
	for (int b = 0; b <= MAX_BASE; ++b){
		for (int n = 0; n <= MAX_DIGITS; ++n){
			array[b][n] = -1;
		}
	}

#ifdef _DEBUG		
	// Diagnostic output if called with an argument
	if (argc > 1)
	{
		for (int b = 0; b <= MAX_BASE; ++b){
			for (int n = 0; n <= MAX_DIGITS; ++n){
				printf("%d %d\t", b, n);
				printf(BIGFMT, compute(b, n));
				printf("\n");
			}
		}
		return 0;
	}
#endif

	// Read number of data sets to process
	int num, n, np;
	char ibuf[128];

	fgets(&(ibuf[0]), sizeof(ibuf), stdin);
	num = atoi(&(ibuf[0]));
	
	// Process each data set identically
	for (int i = 1; i <= num; ++i)
	{
		if(fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
			break;
		}
		if(sscanf(&(ibuf[0]), "%d %d", &(np), &(n)) != 2){
			break;
		}
		printf("%d ", np);
		printf(BIGFMT, compute(10, n));
		printf("\n");
	}
	
	// We're done
	return 0;
}
