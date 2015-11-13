/*
 * G - Compositions
 * ICPC GNYR 2015
 * Problem and solution by Fred Pickel
 */
 #include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

/* Define this to do the problem 2 different ways for sanity checking */
#undef DOUBLE_CHECK

typedef unsigned long DWORD;
#define MAX_ORDER	30

#define METHOD1
char inbuf[256];

DWORD powers2[30];
DWORD CC[31];	// work array

void MakePowers()
{
	int i;
	powers2[0] = 1;
	for(i = 1 ; i < 30 ; i++) {
		powers2[i] = 2*powers2[i-1];
	}
}

/*
 * To see there are 2^(n-1) unrestricted compositions of n
 * consider any composition of n
 * if the first and only term is n, we get 1 composition
 * if the first term is k, the remaining terms are a composition of n-k
 * so CC(n) = 1 + CC(n-1) + CC(n-2) + ... + CC(1)
 * by induction: CC(n) = 1 + sum (i=1 to n-1)(2^(i-1)) = 1 + (1 +2 + 4 + ...+ 2^(n-2) = 2^(n-1)
 * if we define CC(0) = 1 to represent the empty composition, we get the 
 * composititon of n alone by adding n to the empty composition so we get
 *	CC(n) = CC(n-1) + CC(n-2) + ... + CC(1) + CC(0)
 *
 * Now consider CCV with exclusions
 * consider any valid composition of n.
 * It must begin with a non-excluded integer j and the rest of the composition is a valid
 *	composition of (n-j)
 * 
 * Now we get CCV(n) = sum (j = 0 to n, j not excluded)(CC(n-j))
 *
 * for n < m (or n < k if m = 0) CCV(n) = CC(n) = 2^(n-1)
 * for n = m (or n = k if m = 0) There is only one invalid composition,
 * the set consisting only of n So CCV(n) = CC(n) - 1 = 2^(n-1) - 1
 */

DWORD CountCompositions(int n, int m, int k)
{
	int i, j;
	DWORD sum;
	if(m == 0) { // powers of k
		if(n < k) {
			return powers2[n-1];
		} else if(n == k) {
			return (powers2[n-1] - 1);
		} else {
			CC[0] = 1;
			CC[1] = 1;
			for(i = 2; i < k ; i++) {
				CC[i] = powers2[i-1];
			}
			CC[k] = powers2[k-1] - 1;
			for(i = k+1; i <= n; i++) {
				sum = 0;
				for(j = 1; j <= i; j++) {
					if((j % k) != 0) {
						sum += CC[i-j];
					}
				}
				CC[i] = sum;
			}
			return CC[n];
		}
	} else {
		if(n < m) {
			return powers2[n-1];
		} else if(n == m) {
			return (powers2[n-1] - 1);
		} else {
			CC[0] = 1;
			for(i = 1; i < m ; i++) {
				CC[i] = powers2[i-1];
			}
			CC[m] = powers2[m-1] - 1;
			for(i = m+1; i <= n; i++) {
				sum = 0;
				for(j = 1; j <= i; j++) {
					if((j % k) != m) {
						sum += CC[i-j];
					}
				}
				CC[i] = sum;
			}
			return CC[n];
		}
	}
	return 0;
}

/* ALTERNATE APPROACH
 * To see there are 2^(n-1) unrestricted compositions of n
 * consider any composition of n
 * if the first term is 1, we remove that term and get a composition of (n-1)
 * and all compositions of (n-1) occur this way
 * if the first term is > 1, we subtract 1 from the first term and again get a
 * composition of (n-1) and all compositions of (n-1) occur this way
 * so CC(n) = 2*CC(n-1)
 *
 * Now consider CCV with exclusions
 * first the case where m = 0
 * for n < k all posible terms are valid so CCV(n) = CC(n) = 2^(n-1)
 * for n = k There is only one invalid composition,
 * the set consisting only of n So CCV(n) = CC(n) - 1 = 2^(n-1) - 1
 * consider any valid composition of n. for n > k
 * It must begin with a non-excluded integer j and the rest of the composition is a valid
 *	composition of (n-j)
 * Counting compostions with first term j < k, we get CCV(n-1) + ... + CCV(n-k+1)
 * The first term cannot be k so the rest of the compositiosn have first term > k
 * if the term is valid, we can subtract k and get another valid term and we get a valid composition of n-k
 * and every valid composition of n-k can be obtained this way
 * So, if m = 0 and n > k
 * CCV(n) = CCV(n-1) + ... + CCV(n-k)
 * 
 * Now consider 0 < m < k
 * for n < m all compositions are valid CCV(n) = CC(n) = 2^(n-1)
 * for n = m,  CCV(n) = 2^(n-1) - 1
 * for m < n <= k
 * consider the first term j, which can be any value 1<=j <=n EXCEPT j=m
 * and the remaining terms are a composition of (n-j) and the case of n alone if n != m
 * So CCV(n) = CCV(n-1) + ...+CCV(n-m+1) + CCV(n-m-1) + ... CCV(1) + 1 (last tterm for n alone
 * for n > k
 * again consider the first term j in a compostion
 * if it is <= k, it is not = m. If we remove it we get a composition of (n-j)
 * if it is > k, we subtract k from it and it is still valid and we get a composition of n-k
 * So
 * CCV(n) = CCV(n-1) + ...CCV(n-m+1) + CCV(n-m-1) + ...+ CCV(n-k) + CCV(n-k) 
 * (last term for compositons starting with a value > k)
 */

DWORD CountCompositions2(int n, int m, int k)
{
	int i, j;
	DWORD sum;
	if(m == 0) { // powers of k
		if(n < k) {
			return powers2[n-1];
		} else if(n == k) {
			return (powers2[n-1] - 1);
		} else {
			CC[0] = 1;
			CC[1] = 1;
			for(i = 2; i < k ; i++) {
				CC[i] = powers2[i-1];
			}
			CC[k] = powers2[k-1] - 1;
			for(i = k+1; i <= n; i++) {
				sum = 0;
				for(j = 1; j <= k; j++) {
					sum += CC[i-j];
				}
				CC[i] = sum;
			}
			return CC[n];
		}
	} else {
		if(n < m) {
			return powers2[n-1];
		} else if(n == m) {
			return (powers2[n-1] - 1);
		} else {
			CC[0] = 1;
			for(i = 1; i < m ; i++) {
				CC[i] = powers2[i-1];
			}
			CC[m] = powers2[m-1] - 1;
			if(n <= k) {
				for(i = m+1; i <= n ; i++) {
					sum = 0;
					for(j = 1; j <= n; j++) {
						if(j != m) {
							sum += CC[i-j];
						}
					}
					CC[i] = sum;
				}
				return CC[n];
			}
			else {
				for(i = m+1; i <= k ; i++) {
					sum = 0;
					for(j = 1; j <= n; j++) {
						if(j != m) {
							sum += CC[i-j];
						}
					}
					CC[i] = sum;
				}
				for(i = k+1; i <= n; i++) {
					sum = 0;
					for(j = 1; j < k; j++) {
						if(j != m) {
							sum += CC[i-j];
						}
					}
					sum += 2*CC[i-k];
					CC[i] = sum;
				}
				return CC[n];
			}
		}
	}
	return 0;
}

int main()
{
	int nprob, curprob, idx, n, m, k;
	DWORD ret;
#ifdef DOUBLE_CHECK
	DWORD ret2;
#endif

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
	MakePowers();
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d header\n", curprob);
			return -3;
		}
		// get prob num and n and sequnce parameters m and k
		if(sscanf(&(inbuf[0]), "%d %d %d %d",
			&idx, &n, &m, &k) != 4)
		{
			fprintf(stderr, "scan failed on problem header problem index %d\n",
				curprob);
			return -6;
		}
		if(idx != curprob)
		{
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				idx, curprob);
			return -7;
		}
		if((n < 1) || (n > MAX_ORDER) || (m < 0) || (m > MAX_ORDER) || (k < 1) || (k > MAX_ORDER))
		{
			fprintf(stderr, "problem index %d invalid problem data n %d m %d k %d\n",
				idx, n, m , k);
			return -7;
		}
		if((ret = CountCompositions(n, m, k)) == 0) {
			printf("ERROR in Count Compositions\n");
		} else {
			printf("%d %ld\n", idx, ret);
		}
#ifdef DOUBLE_CHECK
		// NOT REQUIRED SANITY CHECK
		if((ret2 = CountCompositions2(n, m, k)) == 0) {
			printf("ERROR in Count Compositions2\n");
		} else if (ret2 != ret) {
			printf("%d %ld != %d\n", idx, ret, ret2);
		}
#endif
	}
	return 0;
}
