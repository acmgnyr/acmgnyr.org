/*
 * 2005 ACM Greater New York Regional Collegiate Programming Contest
 * Problem I -- Sequence Sum Possibilities
 */

#include <stdio.h>
#include <ctype.h>

typedef unsigned long DWORD;

#define MAX_PRIMES	64

/*
 * if n is the sum of k consecutive integers starting with m > 0
 * then n = (m+k)*(m+k-1)/2 - m*(m-1)/2 = (2*m*k + k*k -k)/2
 * so 2*n = k*(2*m + k - 1) so 2*n must be divisible by k
 * and 2*m = 2*n/k - k + 1 must be even (and > 0)
 * if k is odd, 2*n/k - k + 1 is always even
 * if k is even, 2*n/k - k + 1 is even only if 2*n/k is odd
 * so if n = (2^j)*odd then k must be (2^(j+1))*(odd divisor of n possibly 1)
 * these two types of candidate k's occur in pairs: if k is an odd divisor of n
 * then 2*n/k is (2^(j+1))*(odd divisor of n possibly 1)
 * exactly one of these will have 2*n/k -k + 1 > 0
 * thus the number of sequence lengths k is the number of odd divisors
 * of 2*n other than 1
 * if the prime power representation of n is (2^j)*PROD(p[i]^n[i])
 * the number of odd divisors of n = PROD(n[i] + 1)
 * in an odd divisor, each prime p[i] may occur to a power 0...n[i]
 * if all powers are zero, we get 1 which is not an allowed length so
 * the count of number of ways to write n as a sequence of consecutive
 * positive integers is PROD(n[i] + 1) - 1
 */
int primes[MAX_PRIMES];
int prime_powers[MAX_PRIMES];
char inbuf[256];

// code to find primes < 2^16 which is max prime factor of an int
#define MAX_ODD_PRIMES 8192	// determined experimentally

int oddPrimes[MAX_ODD_PRIMES];
int nPrimes;

DWORD sieve[1024];	// bit n off if (2*n+1) is prime

#define SET_BIT(n)	(sieve[n>>5] |= (1 << (n & 31)))
#define TEST_BIT(n)	((sieve[n>>5] & (1 << (n & 31))) == 0)

// mark all odd values as possible primes
void
InitSieve()
{
	int i;
	for(i = 0; i < 1024 ; i++)
	{
		sieve[i] = 0;
	}
}

// prime = 2*n + 1, set bits starting at n in steps of 2*n+1
// = odds divisible by 2*n+1
void
SetPrime(int n)
{
	int step, i;
	i = n;
	step = 2*n + 1;
	i += step;
	while(i < 32768)
	{
		SET_BIT(i);
		i += step;
	}
}

// find primes < 2^16 with the usual seive
int
GetPrimes()
{
	int n;
	nPrimes = 0;
	InitSieve();
	for(n = 1; n < 32768 ; n++)
	{
		if(TEST_BIT(n))
		{
			oddPrimes[nPrimes++] = 2*n+1;
			SetPrime(n);
		}
		if(nPrimes >= MAX_ODD_PRIMES)
		{	// should not happen now that MAX_ODD_PRIMES is big enough
			fprintf(stderr, "nPrimes %d >= max %d at n %d\n",
				nPrimes, MAX_ODD_PRIMES, n);
			return -1;
		}
	}
	return 0;
}

// get primepower factorization of n, return number of prime power factors
// for each prime < n if prime divides n, keep trying until it does not
// record prime and power and continue with the quotient
int
GetPrimePowers(int n)
{
	int curPrime, curPow, nfactors, pInd;
	nfactors = 0;
	// first deal with 2
	curPrime = 2;
	curPow = 0;
	while((n & 1) == 0)
	{
		n >>= 1;
		curPow++;
	}
	if(curPow > 0)
	{
		primes[nfactors] = 2;
		prime_powers[nfactors] = curPow;
		nfactors++;
	}
	// now look for odd prime factors
	for(pInd = 0; (pInd < nPrimes) && (n > 1) ; pInd++)
	{
		curPrime = oddPrimes[pInd];
		curPow = 0;
		while((n % curPrime) == 0)
		{
			n /= curPrime;
			curPow++;
		}
		if(curPow > 0)
		{
			primes[nfactors] = curPrime;
			prime_powers[nfactors] = curPow;
			nfactors++;
		}
		if(nfactors > MAX_PRIMES)
		{
			fprintf(stderr, "nfactors %d >= max %d at prime %d (ind %d)\n",
				nfactors, MAX_PRIMES, curPrime, pInd);
			return -1;
		}
	}
	if(n > 1)
	{	// not div by any prime < sqrt(n), must be prime itself
		primes[nfactors] = n;
		prime_powers[nfactors] = 1;
		nfactors++;
	}
	return nfactors;
}

int
main(int argc, char **argv)
{
	DWORD res;
	int i, j, nprimes, nprobs, probnum;
	int	probVal;

	if(GetPrimes() != 0)
	{
		return -19;
	}

	if(fgets(&(inbuf[0]),256,stdin) == NULL)
	{
		fprintf(stderr, "Cannot read problem count\n");
		return -1;
	}
	if(sscanf(&(inbuf[0]), "%d", &nprobs) != 1)
	{
		fprintf(stderr, "Cannot parse problem count\n");
		return -2;
	}
	for(i = 0; i < nprobs ; i++)
	{
		// read and parse problem instance
		if(fgets(&(inbuf[0]),256,stdin) == NULL)
		{
			fprintf(stderr, "Cannot read problem %d\n", i+1);
			return -3;
		}
		if(sscanf(&(inbuf[0]), "%d %d", &probnum, &probVal) != 2)
		{
			fprintf(stderr, "Cannot parse problem number and val, problem %d\n", i+1);
			continue;
		}
		// extraneous validity check
		if(probnum != (i+1))
		{
			fprintf(stderr, "Problem number %d != sequence number %d\n",
				probnum, i+1);
			continue;
		}
		// get prime power factorization
		nprimes = GetPrimePowers(probVal);
		if(nprimes <= 0)
		{
			fprintf(stderr, "Problem number %d GetPrimePowers returned %d\n",
				probnum, nprimes);
			continue;
		}
		// compute (PROD[oddprime factors](power[i] + 1))
		res = 1;
		for(j = 0; j < nprimes ; j++)
		{
			if(primes[j] == 2)
			{
				continue;
			}
			else
			{
				res *= (prime_powers[j] + 1);
			}
		}
		// result is product - 1
		printf("%d %d\n",probnum, res - 1);
	}
	return 0;
}
