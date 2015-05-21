/*
 * B - Nim-B Sum
 * ACM International Collegiate Programming Contest
 * Greater New York Region
 * October 24, 2010
 */

#include <stdio.h>
#include <math.h>

char inbuf[256];

int NimNSum(int base, int val1, int val2)
{
	int result, factor, dig1, dig2;
	result = 0;
	factor = 1;
	while((val1 != 0) || (val2 != 0)) {
		dig1 = val1 % base;
		dig2 = val2 % base;
		val1 /= base;
		val2 /= base;
		dig1 = (dig1 + dig2) % base;
		result += factor * dig1;
		factor *= base;
	}
	return result;
}

int main()
{
	int nprob, curprob, index, ret, base, val1, val2;

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
			fprintf(stderr, "Read failed on problem %d data\n", curprob);
			return -3;
		}
		if(sscanf(&(inbuf[0]), "%d %d %d %d", &index, &base, &val1, &val2) != 4)
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
		if((base < 2) || (val1 < 0) || (val2 < 0)) {
			fprintf(stderr, "problem index %d base %d < 2 or val %d %d < 0\n",
				curprob, base, val1, val2);
			return -8;
		}

		ret = NimNSum(base, val1, val2);
		printf("%d %d\n", curprob, ret);
	}
	return 0;
}


