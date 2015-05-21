/*
 * H - Power's of Pascal
 * ICPC 2013 Greater NY Regional
 * Solution by Fred Pickel
 * Problem by Fred Pickel
 */
#include <stdio.h>
#include <math.h>

/*
 * *sigh* Windows and Linux use different format specifiers for
 * 64 bit int's, not to mention different datatypes.
 */
#ifdef WIN32
typedef unsigned _int64 INT64;
#define	PRINT_FORMAT	"%d %I64u\n"
#else
#define	PRINT_FORMAT	"%d %llu\n"
typedef unsigned long long INT64;
#endif

/*
 * looking at the first few powers of (the upper left corner of
 * the pascal matrix
 *
 *   1    0    0    0    0    0    0    0
 *   1    1    0    0    0    0    0    0
 *   1    2    1    0    0    0    0    0 
 *   1    3    3    1    0    0    0    0 
 *   1    4    6    4    1    0    0    0 
 *   1    5   10   10    5    1    0    0 
 *   1    6   15   20   15    6    1    0 
 *   1    7   21   35   35   21    7    1 
 *
 *    1    0    0    0    0    0    0    0 
 *    2    1    0    0    0    0    0    0 
 *    4    4    1    0    0    0    0    0
 *    8   12    6    1    0    0    0    0 
 *   16   32   24    8    1    0    0    0 
 *   32   80   80   40   10    1    0    0 
 *   64  192  240  160   60   12    1    0 
 *  128  448  672  560  280   84   14    1 
 * 
 *    1    0    0    0    0    0    0    0
 *    3    1    0    0    0    0    0    0 
 *    9    6    1    0    0    0    0    0 
 *   27   27    9    1    0    0    0    0 
 *   81  108   54   12    1    0    0    0 
 *  243  405  270   90   15    1    0    0 
 *  729 1458 1215  540  135   18    1    0
 * 2187 5103 5103 2835  945  189   21    1 
 * 
 * it is fairly obvious that the first column
 * is (power)^row and the subdiagonal is power * row
 * which can be verrified using the binomial theorem
 * and direct computation
 *
 * guess and check leads to (Pascal ^ n)[i, j] =
 * (n ^ (i - j)) * C(i, j)
 * C(i, j) = Pascal[i, j] = combinations of i things j at a time
 *
 * another way to look at it is
 * Pascal * vect(x^n) = vect((x+1)^n) 
 * vect(x^n) = [1 x x^2 x^3 ,,,]  vect((x+1)^n) = {1 (x+1) (x+1)^2 (x+1)^3 ...]
 * Pascal * (Pascal * vect(x^n)) = Pascal * vect((x+1)^n) = 
 *  vect(((x+1)+1)^n) = vect((x+2)^n)
 * (Pascal ^ K) * vect(x^n) = vect((x+K)^n)
 * comparing coefficients of x^j in entry i
 * (Pascal ^ K)[i, j] = C(i, j) * (K^(i-j))
 */

/*
 * compute combinations on n things k at a time
 * with minimal multiplies
 */
INT64 Comb(int n, int k)
{
	INT64 res, d_n, d_k;
	int i;

	/* easy cases */
	if((k < 0) || (n  < 0) || (k > n))
	{
		return 0;
	}
	if((k == 0) || (k == n))
	{
		return 1;
	}
	/* now the rest if k > n/2 use C(n, k) = C(n, n-k) 
	 * to reduce computaion and avoid overflow */
	d_n = n;
	d_k = 1;
	if(k > (n/2)) k = n - k;
	res = 1;
	for(i = 1; i <= k ; i++)
	{
		res = (res * d_n)/d_k;
		d_n -= 1;
		d_k += 1;
	}
	return res;
}

/* power assume exponent >= 0 */
INT64 Power(INT64 base, int exponent)
{
	INT64 res, sqr;
	int exp;

	res = 1;
	sqr = base;
	exp = exponent;
	while(exp > 0)
	{
		if((exp & 1) != 0)
		{
			res *= sqr;
		}
		exp >>= 1;
		sqr = sqr * sqr;
	}
	return res;
}

INT64 PascPower(int pow, int row, int col)
{
	INT64 res, d_p;

	if(col > row)
		return 0;
	if(col == row)
	{
		return 1;
	}
	res = Comb(row, col);
	d_p = pow;
	res = res * Power(d_p, (row - col));
	return res;
}

char inbuf[256];

int main()
{
	int nprob, curprob, index, pow, row, col;
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
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d size\n", curprob);
			return -3;
		}
		if(sscanf(&(inbuf[0]), "%d %d %d %d",
			&index, &pow, &row, &col) != 4)
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
		if((pow < 1) || (pow > 100000))
		{
			fprintf(stderr, "problem index %d power %d not in range 1-100000\n",
				curprob, pow);
			return -8;
		}
		if((row < 0) || (row > 100000))
		{
			fprintf(stderr, "problem index %d row %d not in range 0-100000\n",
				curprob, row);
			return -9;
		}
		if((col < 0) || (col > row))
		{
			fprintf(stderr, "problem index %d col %d not in range 0-%d\n",
				curprob, col, row);
			return -10;
		}
		result = PascPower(pow, row, col);
		printf(PRINT_FORMAT, curprob, result);
	}
	return 0;
}