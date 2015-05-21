#include <stdio.h>

FILE *fpin, *fpout;

#define USE_FORMULA		// define this to use formula
//#define USE_MEMOIZATION // define this to use recursive with memoization
						// define neither to use simple recursion (VERY SLOW)

typedef unsigned long DWORD;
// this file gives three methods for computing the nmber of arrangements
// which is the number of standard Young tableau for the Ferrers diagram
// specified (basically the spots on whcih to place students)
// each method is given in a double precision version and a unsigned long version

// the first method uses the Hook Length Formula (see Amer. Math. Monthly
// v 111 number 8, p700)
// number of arrangements = N!/Product(h(i,j))
// where N = total number of students (= sum of row sizes)
// and product runs over all pairs  (col i, row j) which are valid places
// to put a student
// h(i,j) = the hook length of (i,j) = number of spots in front of (i,j)
// (below in the diagram) + number of spots right of (i,j) + 1

// the double precision cersion just computes the numerator and denominator
// in double precision, divides and outputs the result

// the unsigned integer version cannot compute and divide without overflow
// so we have a table of primes < 30 and the factors fo values <= 30 into those primes
// we compute the prime factors of n!
// the remove factors of each h(i,j)
// then multiply the remaining factors to get a result

// primes <= 30 are: 2 3 5 7 11 13 17 19 23 29

DWORD primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
int prime_facts[30][10] =
{ // 2  3  5  7 11 13 17 19 23 29
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	// 1
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},	// 2
	{0, 1, 0, 0, 0, 0, 0, 0, 0, 0},	// 3
	{2, 0, 0, 0, 0, 0, 0, 0, 0, 0},	// 4
	{0, 0, 1, 0, 0, 0, 0, 0, 0, 0},	// 5
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 0},	// 6
	{0, 0, 0, 1, 0, 0, 0, 0, 0, 0},	// 7
	{3, 0, 0, 0, 0, 0, 0, 0, 0, 0},	// 8
	{0, 2, 0, 0, 0, 0, 0, 0, 0, 0},	// 9
	{1, 0, 1, 0, 0, 0, 0, 0, 0, 0},	// 10
	{0, 0, 0, 0, 1, 0, 0, 0, 0, 0},	// 11
	{2, 1, 0, 0, 0, 0, 0, 0, 0, 0},	// 12
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0},	// 13
	{1, 0, 0, 1, 0, 0, 0, 0, 0, 0},	// 14
	{0, 1, 1, 0, 0, 0, 0, 0, 0, 0},	// 15
	{4, 0, 0, 0, 0, 0, 0, 0, 0, 0},	// 16
	{0, 0, 0, 0, 0, 0, 1, 0, 0, 0},	// 17
	{1, 2, 0, 0, 0, 0, 0, 0, 0, 0},	// 18
	{0, 0, 0, 0, 0, 0, 0, 1, 0, 0},	// 19
	{2, 0, 1, 0, 0, 0, 0, 0, 0, 0},	// 20
	{0, 1, 0, 1, 0, 0, 0, 0, 0, 0},	// 21
	{1, 0, 0, 0, 1, 0, 0, 0, 0, 0},	// 22
	{0, 0, 0, 0, 0, 0, 0, 0, 1, 0},	// 23
	{3, 1, 0, 0, 0, 0, 0, 0, 0, 0},	// 24
	{0, 0, 2, 0, 0, 0, 0, 0, 0, 0},	// 25
	{1, 0, 0, 0, 0, 1, 0, 0, 0, 0},	// 26
	{0, 3, 0, 0, 0, 0, 0, 0, 0, 0},	// 27
	{2, 0, 0, 1, 0, 0, 0, 0, 0, 0},	// 28
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1},	// 29
	{1, 1, 1, 0, 0, 0, 0, 0, 0, 0},	// 30
};

// check that the table of prime factors is correct
void check_prime_facts()
{
	DWORD res, test;
	int i, j, k;
	for(i = 0; i < 30 ; i++)
	{
		test = i+1;
		res = 1;
		for(j = 0; j < 10 ; j++)
		{
			for(k = 0; k < prime_facts[i][j] ; k++)
			{
				res *= primes[j];
			}
		}
		if(res != test)
		{
			printf("res %d != test %d\n", res, test);
		}
	}
}

// compute (double)n!
double dfact(int n)
{
	double result;
	int i;

	result = 1.0;
	for(i = 2; i <= n ; i++)
	{
		result = result *((double)i);
	}
	return result;
}

// compute Product(h(i,j))
double dhooks(int *rows, int rowcnt)
{
	double result;
	int i, j, hook, bottom;

	result = 1.0;
	bottom = rowcnt - 1;
	for(i = 0; i < rows[0] ; i++)
	{
		while(rows[bottom] <= i)
		{
			bottom--;
		}
		for(j = 0; j <= bottom ; j++)
		{
			hook = (bottom - j) + (rows[j] - i);
			result *= ((double)hook);
		}
	}
	return result;
}

// compute the number of arrangements as a double using the formula
double dyoung(int *rows, int rowcnt)
{
	int n, i;
	for(i = 0, n = 0; i < rowcnt ; i++)
	{
		n += rows[i];
	}
	return (dfact(n)/dhooks(rows, rowcnt));
}

// compute the prime factors of n!
int lfact(int n, int *factors)
{
	int i, j;
	for(i = 0; i < 10; i++)
	{
		factors[i] = 0;
	}
	for(i = 2; i <= n ; i++)
	{
		for(j = 0; j < 10 ; j++)
		{
			factors[j] += prime_facts[i-1][j];
		}
	}
	return 0;
}

// assumes factors of n!loaded with numerator
// remove factors of Product(h(i,j))
int lhooks(int *rows, int rowcnt, int *factors)
{
	int i, j, hook, bottom, k;

	bottom = rowcnt - 1;
	for(i = 0; i < rows[0] ; i++)
	{
		while(rows[bottom] <= i)
		{
			bottom--;
		}
		for(j = 0; j <= bottom ; j++)
		{
			hook = (bottom - j) + (rows[j] - i);
			if(hook > 1)
			{
				for(k = 0; k < 10 ; k++)
				{
					factors[k] -= prime_facts[hook-1][k];
				}
			}
		}
	}
	return 0;
}

// compute number of arrangements as an integer
// by finding prime factorization of n!
// removing prime factors of Product(h(i,j))
// and multiplying the remaining factors to get the result
DWORD lyoung(int *rows, int rowcnt)
{
	int n, i, k, j;
	int facts[10];
	DWORD result, oresult;

	for(i = 0, n = 0; i < rowcnt ; i++)
	{
		n += rows[i];
	}
	lfact(n, &(facts[0]));
	lhooks(rows, rowcnt, &(facts[0]));
	result = oresult = 1;
	for(i = 0; i < 10 ; i++)
	{
		if(facts[i] < 0)
		{
			fprintf(stderr, "facts[%d] = %d\n", i, facts[i]);
			return 0;
		}
		for(k = 0; k < facts[i] ; k++)
		{
			result *= primes[i];
			if(result < oresult)
			{
				fprintf(stderr, "overflow?? res 0x%x ores 0x%x\n facts: ", result, oresult);
				for(j = 0; j < 10 ; j++)
				{
					fprintf(stderr, "%d ", facts[j]);
				}
				fprintf(stderr, "\n");
				return 0;
			}
			oresult = result;
		}
	}
	return result;
}

// the second method is a straight forward recursion
// if the layout has only one row (rowcnt = 1) or one column (rows[0] = 1)
// there is only one arrangement
// otherwise, the shortest student must beat the end of some row
// with no student in front of him/her
// for each possible position of the shortest student, we count
// the number of ways of arranging the remaining students and add over
// possible shortest student positions
// number of arrangements = 
//		SUM over positions of shortest student (arrangements for other students)
// total = 0;
// for(i = 0; i < 5 ; i++)
//		if(rows[i] > rows[i+1])
//			total += arrangements with rows[i] = rows[i] - 1
//
// the problem with this version is similar to that of recursive fibonnaci
// smaller subproblems are recomputed multiple times leading to exponential
// time complexity

// double precision simple recursive version
double rdyoung(int *rows)
{
	int nrows[5], i;
	double result;

	if((rows[0] == 1) || (rows[1] == 0))
	{
		return 1.0;
	}
	result = 0.0;
	for(i = 0; i < 5 ; i++)
	{
		nrows[i] = rows[i];
	}
	if(nrows[4] > 0)
	{
		nrows[4]--;
		result += rdyoung(nrows);
		nrows[4]++;
	}
	for(i = 0; i < 4; i++)
	{
		if(nrows[i] > nrows[i+1])
		{
			nrows[i]--;
			result += rdyoung(nrows);
			nrows[i]++;
		}
	}
	return result;
}

// unsigned long simple recursive version
DWORD rlyoung(int *rows)
{
	int nrows[5], i;
	DWORD result, oresult;

	if((rows[0] == 1) || (rows[1] == 0))
	{
		return 1;
	}
	result = oresult = 0;
	for(i = 0; i < 5 ; i++)
	{
		nrows[i] = rows[i];
	}
	if(nrows[4] > 0)
	{
		nrows[4]--;
		result += rlyoung(nrows);
		nrows[4]++;
		if(result < oresult)
		{
			fprintf(stderr, "overflow?? result 0x%x oresult 0x%x\n", result, oresult);
			return 0;
		}
		oresult = result;
	}
	for(i = 0; i < 4; i++)
	{
		if(nrows[i] > nrows[i+1])
		{
			nrows[i]--;
			result += rlyoung(nrows);
			nrows[i]++;
			if(result < oresult)
			{
				fprintf(stderr, "overflow?? result 0x%x oresult 0x%x\n", result, oresult);
				return 0;
			}
			oresult = result;
		}
	}
	return result;
}

// the final method modifies the simple recursive version to use memoization??
// i.e. the intermediate results are remembered and returned immediately instead
// of being recomputed
// each intermediate result is stored in a hash table indexed by the 5 row sizes
// if the value has been previously computed, it is returned by table lookup
// if not, the value is computed and a node inserted in the table
// these are the same as the above except that
// after checking for the edge cases, we look to see if we already know the answer
// and if so return it
// and if we had to compute it, we insert the newly computed value for next time
//
// NOTE that the table is save between problem instances so values computed for
// previous problems may be reused
#define HFACT	37
#define HSIZE	1023

struct dhent
{
	dhent *pNext;
	int rows[5];
	double result;
};

struct lhent
{
	lhent *pNext;
	int rows[5];
	DWORD result;
};


dhent *dhtable[HSIZE];
lhent *lhtable[HSIZE];
int dhcnt, lhcnt;
int hinitted = 0;

// init hash tables, once only
void hinit()
{
	int i;
	if(hinitted != 0)
	{
		return;
	}
	for(i = 0; i < HSIZE ; i++)
	{
		dhtable[i] = NULL;
		lhtable[i] = NULL;
	}
	dhcnt = lhcnt = 0;
	hinitted = 1;
}

// hash fuction for 5 row sizes
int mhash(int *rows)
{
	int result, i;
	result = 0;
	for(i = 0; i < 5 ; i++)
	{
		result = result * HFACT + rows[i];
	}
	return (result % HSIZE);
}

// look for a table entry with the same row sizes
// return ptr ifthere, null if not (double result)
dhent * dlookup(int *rows)
{
	dhent *pCur;
	int hval;

	hval = mhash(rows);
	pCur = dhtable[hval];
	while(pCur != NULL)
	{
		if((pCur->rows[0] == rows[0]) && (pCur->rows[1] == rows[1]) &&
			(pCur->rows[2] == rows[2]) && (pCur->rows[3] == rows[3]) &&
			(pCur->rows[4] == rows[4]))
		{
			return pCur;
		}
		pCur = pCur->pNext;
	}
	return NULL;
}

// look for a table entry with the same row sizes
// return ptr ifthere, null if not  (unsigned int result)
lhent * llookup(int *rows)
{
	lhent *pCur;
	int hval;

	hval = mhash(rows);
	pCur = lhtable[hval];
	while(pCur != NULL)
	{
		if((pCur->rows[0] == rows[0]) && (pCur->rows[1] == rows[1]) &&
			(pCur->rows[2] == rows[2]) && (pCur->rows[3] == rows[3]) &&
			(pCur->rows[4] == rows[4]))
		{
			return pCur;
		}
		pCur = pCur->pNext;
	}
	return NULL;
}

// insert a new table entry corresponding to the specified row sizes
// including the result value (double result)
void dinsert(int *rows, double result)
{
	dhent *pNew;
	int i, hval;

	pNew = new dhent;
	if(pNew == NULL)
	{
		return;
	}
	hval = mhash(rows);
	pNew->result = result;
	for(i = 0; i < 5; i++)
	{
		pNew->rows[i] = rows[i];
	}
	pNew->pNext = dhtable[hval];
	dhtable[hval] = pNew;
	dhcnt++;
}

// insert a new table entry corresponding to the specified row sizes
// including the result value (unsigned int result)
void linsert(int *rows, DWORD result)
{
	lhent *pNew;
	int i, hval;

	pNew = new lhent;
	if(pNew == NULL)
	{
		return;
	}
	hval = mhash(rows);
	pNew->result = result;
	for(i = 0; i < 5; i++)
	{
		pNew->rows[i] = rows[i];
	}
	pNew->pNext = lhtable[hval];
	lhtable[hval] = pNew;
	lhcnt++;
}

double mdyoung(int *rows)
{
	int nrows[5], i;
	double result;
	dhent *pPrev;

	if((rows[0] == 1) || (rows[1] == 0))
	{
		return 1.0;
	}
	if((pPrev = dlookup(rows)) != NULL)
	{
		return pPrev->result;
	}
	result = 0.0;
	for(i = 0; i < 5 ; i++)
	{
		nrows[i] = rows[i];
	}
	if(nrows[4] > 0)
	{
		nrows[4]--;
		result += mdyoung(nrows);
		nrows[4]++;
	}
	for(i = 0; i < 4; i++)
	{
		if(nrows[i] > nrows[i+1])
		{
			nrows[i]--;
			result += mdyoung(nrows);
			nrows[i]++;
		}
	}
	dinsert(rows, result);
	return result;
}


// memoized recursive computation (unsigned int result)
DWORD mlyoung(int *rows)
{
	int nrows[5], i;
	DWORD result;
	lhent *pPrev;

	if((rows[0] == 1) || (rows[1] == 0))
	{
		return 1;
	}
	if((pPrev = llookup(rows)) != NULL)
	{
		return pPrev->result;
	}
	result = 0;
	for(i = 0; i < 5 ; i++)
	{
		nrows[i] = rows[i];
	}
	if(nrows[4] > 0)
	{
		nrows[4]--;
		result += mlyoung(nrows);
		nrows[4]++;
	}
	for(i = 0; i < 4; i++)
	{
		if(nrows[i] > nrows[i+1])
		{
			nrows[i]--;
			result += mlyoung(nrows);
			nrows[i]++;
		}
	}
	linsert(rows, result);
	return result;
}



char inbuf[256];

// return number of rows or negative on error
// read number of rows on first line
// and that many row sizes on the next line
// check for read errors
// bad number of rows
// not enough row sizes
// row sizes not non-increasing
// row size < 0
// row sizes add to more than 30
int read_instance(int *rows, int probnum)
{
	int cnt, rcnt, i, tot;

	if(fgets(&(inbuf[0]), 255, fpin) == 0)
	{
		fprintf(stderr, "unexpected EOF on count of problem %d\n", probnum);
		return -1;
	}
	if(sscanf(&(inbuf[0]), "%d", &cnt) != 1)
	{
		fprintf(stderr, "no count for problem %d\n", probnum);
		return -2;
	}
	if(cnt == 0)
	{
		return 0;
	}
	if(cnt > 5)
	{
		if(fgets(&(inbuf[0]), 255, fpin) == 0)
		{
			fprintf(stderr, "unexpected EOF on rows of problem %d (bad count %d)\n",
				probnum, cnt);
			return -3;
		}
		fprintf(stderr, "bad count %d for problem %d\n", cnt, probnum);
		return -4;
	}
	if(fgets(&(inbuf[0]), 255, fpin) == 0)
	{
		fprintf(stderr, "unexpected EOF on rows of problem %d (good count %d)\n",
			probnum, cnt);
		return -5;
	}
	switch(cnt)
	{
	case 1:
		if((rcnt = sscanf(&(inbuf[0]), "%d", &(rows[0]))) != 1)
		{
			fprintf(stderr, "bad row data cnt %d != %d for problem %d\n",
				rcnt, cnt, probnum);
			return -6;
		}
		rows[1] = rows[2] = rows[3] = rows[4] = 0;
		break;
	case 2:
		if((rcnt = sscanf(&(inbuf[0]), "%d %d", &(rows[0]), &(rows[1]))) != 2)
		{
			fprintf(stderr, "bad row data cnt %d != %d for problem %d\n",
				rcnt, cnt, probnum);
			return -6;
		}
		rows[2] = rows[3] = rows[4] = 0;
		break;
	case 3:
		if((rcnt = sscanf(&(inbuf[0]), "%d %d %d", &(rows[0]),
			&(rows[1]), &(rows[2]))) != 3)
		{
			fprintf(stderr, "bad row data cnt %d != %d for problem %d\n",
				rcnt, cnt, probnum);
			return -6;
		}
		rows[3] = rows[4] = 0;
		break;
	case 4:
		if((rcnt = sscanf(&(inbuf[0]), "%d %d %d %d", &(rows[0]),
			&(rows[1]), &(rows[2]), &(rows[3]))) != 4)
		{
			fprintf(stderr, "bad row data cnt %d != %d for problem %d\n",
				rcnt, cnt, probnum);
			return -6;
		}
		rows[4] = 0;
		break;
	case 5:
		if((rcnt = sscanf(&(inbuf[0]), "%d %d %d %d %d", &(rows[0]),
			&(rows[1]), &(rows[2]), &(rows[3]), &(rows[4]))) != 5)
		{
			fprintf(stderr, "bad row data cnt %d != %d for problem %d\n",
				rcnt, cnt, probnum);
			return -6;
		}
		break;
	default:
		fprintf(stderr, "bad cnt case %d for problem %d\n",
			cnt, probnum);
		return -7;
	}
	for(i = 0; i < 4 ; i++)
	{
		if(rows[i] < rows[i+1])
		{
			fprintf(stderr, "rows[%d] = %d < rows[%d] = %d\n", i, rows[i], i+1, rows[i+1]);
			return -8;
		}
	}
	for(i = 0, tot = 0; i < 5; i++)
	{
		if(rows[i] < 0)
		{
			fprintf(stderr, "rows[%d] = %d < 0\n", i, rows[i]);
			return -9;
		}
		tot += rows[i];
	}
	if(tot > 30)
	{
		fprintf(stderr, "total students %d > 30\n", tot);
		return -10;
	}
	return cnt;
}
int main(int argc, char **argv)
{
	extern int mkmain(int argc, char **argv);
	int n, rows[5], nprob;

	if(argc > 1){
		return(mkmain(argc, argv));
	}
	fpin = fopen("i.in", "rt");
	if(fpin == NULL){
		fprintf(stderr, "No input i.in\n");
		return(1);
	}
	fpout = stdout;
	if(fpout == NULL){
		fprintf(stderr, "No output file i.out\n");
		return(2);
	}

	nprob = 0;
	hinit();
	check_prime_facts();
	for(;;)
	{
		nprob++;
		n = read_instance(&(rows[0]), nprob);
		if(n <= 0)
		{
//			printf("dhcnt %d lhcnt %d\n", dhcnt, lhcnt);
			return n;
		}
#ifdef USE_FORMULA
		fprintf(fpout, "%lu\n", lyoung(&(rows[0]), n));
#else
#ifdef USE_MEMOIZATION
		fprintf(fpout, "%lu\n", mlyoung(&(rows[0])));
#else	// recursion, very slow
		fprintf(fpout, "%lu\n", rlyoung(&(rows[0])));
#endif
#endif
	}
	::fclose(fpin);
//	::fclose(fpout);
}
