/*
 * E - Faulhaber's Triangle
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
typedef _int64 BIGINT;
#define	PRINT_FORMAT1	"%d %I64d\n"
#define	PRINT_FORMAT2	"%d %I64d/%I64d\n"
#else
#define	PRINT_FORMAT1	"%d %lld\n"
#define	PRINT_FORMAT2	"%d %lld/%lld\n"
typedef long long BIGINT;
#endif


#define MAX_ENTRY_SZ	400
char inbuf[256];
BIGINT FaulNum[MAX_ENTRY_SZ+1][MAX_ENTRY_SZ+2];
BIGINT FaulDenom[MAX_ENTRY_SZ+1][MAX_ENTRY_SZ+2];
int lastFullRow = -1;

// gcd of a and b
BIGINT BGCD(BIGINT a,BIGINT b)
{
	BIGINT r;
	if(a == 0) {
		if(b == 0) {
			return 1;
		} else {
			return b;
		}
	}
	if(b == 0) return a;
	if(a < 0) a = -a;
	if(b < 0) b = -b;
	while(a != 0) {
		r = b % a;
		b = a;
		a = r;
	}
	return b;
}

// find least common multiplier of denoms 2->row+1 in given row
BIGINT LCM(BIGINT row)
{
	BIGINT ret, r, i;

	ret = FaulDenom[row][row+1];
	for( i = row ; i > 1 ; i--) {
		r = BGCD(ret, FaulDenom[row][i]);
		ret = (ret * FaulDenom[row][i])/r;
		if((ret < 0)) {
			return -2;
		}
	}
	return ret;
}

// find F(row,1) assumes F(row,i) 1 < i <= row+1 filled in
int FillFirstValue(int row)
{
	BIGINT denom, num, i, val, next;

	denom = LCM(row);
	if(denom < 0) {
		return -2;
	}
	num = denom;	// start with 1
	for(i = 2; i <= row+1 ; i++) {
		if(FaulNum[row][i] != 0) {
			val = FaulNum[row][i] * (denom/FaulDenom[row][i]);
			if(((val > 0) && (FaulNum[row][i] < 0)) ||
				((val < 0) && (FaulNum[row][i] > 0))) {
				return -3;
			}
			next = num - val;
			if(((val > num) && (next > 0)) ||
				((val < num) && (next < 0))) {
				return -4;
			}
			num = next;
		}
	}
	val = BGCD(num, denom);
	FaulNum[row][1] = num/val;
	FaulDenom[row][1] = denom/val;
	return 0;
}

void FaulInit()
{
	int i, j;
	// set denoms to zero as flags not yet computed
	for(i = 0; i <= MAX_ENTRY_SZ ; i++) {
		for(j = 0; j <= MAX_ENTRY_SZ+1 ; j++) {
			FaulDenom[i][j] = 0;
		}
	}
	FaulDenom[0][1] = FaulNum[0][1] = 1;
	FaulDenom[1][1] = FaulDenom[1][2] = 2;
	FaulNum[1][1] = FaulNum[1][2] = 1;
	lastFullRow = 1;
}

int FillNextRow(int row)
{
	BIGINT num, denom, val, r, c;
	int i, ret;

	if(row != (lastFullRow + 1)){
		return -1;
	}
	for(i = 2; i <= row+1 ; i++) {
		if(FaulDenom[row][i] == 0) {	// not yet computed
			r = row;
			c = i;
			num = FaulNum[row-1][i-1] * row;
			if(((num > 0) && (FaulNum[row-1][i-1] < 0)) ||
				((num < 0) && (FaulNum[row-1][i-1] > 0))) {
				return -5;
			}
			denom = FaulDenom[row-1][i - 1] * i;
			if((denom < 0)) {
				return -6;
			}
			val = BGCD(num,denom);
			FaulNum[row][i] = num/val;
			FaulDenom[row][i] = denom/val;
		}
	}
	if((ret = FillFirstValue(row)) != 0) {
		return ret;
	}
	lastFullRow = row;
	return 0;
}

int CompFaul(int row, int index)
{
	BIGINT num, denom, val;
	int needRow, i, j, ret;
	if((row <= MAX_ENTRY_SZ) && (FaulDenom[row][index] > 0)) {
		return 0;	// already computed
	}
	if((needRow = (row - index + 1)) > lastFullRow) {
		for(i = lastFullRow+1; i <= needRow ; i++) {
			if((ret = FillNextRow(i)) != 0) {
				return ret;
			}
		}
	}
	for(i = needRow + 1, j = 2; i <= row ; i++, j++) {
		if(FaulDenom[i][j] == 0) {
			num = FaulNum[i-1][j-1]*(i);
			if(((num > 0) && (FaulNum[i-1][j-1] < 0)) ||
				((num < 0) && (FaulNum[i-1][j-1] > 0))) {
				return -7;
			}
			denom = FaulDenom[i-1][j - 1]*(j);
			if((denom < 0)) {
				return -8;
			}
			val = BGCD(num,denom);
			FaulNum[i][j] = num/val;
			FaulDenom[i][j] = denom/val;
		}
	}
	return 0;
}

int main()
{
	int nprob, curprob, index, m, k, ret;

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
	FaulInit();
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d data\n", curprob);
			return -3;
		}
		// get prob num and subprob count
		if(sscanf(&(inbuf[0]), "%d %d %d", &index, &m, &k) != 3)
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
		if((m < 0) || (m > MAX_ENTRY_SZ)){
			fprintf(stderr, "problem index %d power (m) %d out of range\n",
				curprob, m);
			return -8;
		}
		if((k < 1) || (k > (m+1))){
			fprintf(stderr, "problem index %d coeff idex (k) %d not between 1 and %d\n",
				curprob, k, m+1);
			return -9;
		}
		ret = CompFaul(m, k);
		if(ret != 0) {
			printf("overflow ret %d \n", ret);
		} else {
			if(FaulDenom[m][k] == 1) {
				printf(PRINT_FORMAT1, curprob, FaulNum[m][k]);
			} else {
				printf(PRINT_FORMAT2, curprob, FaulNum[m][k], FaulDenom[m][k]);
			}
		}
	}
	return 0;
}
