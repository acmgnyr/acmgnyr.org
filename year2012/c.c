/*
 * C -Pen Counts
 * ICPC 2012 Greater NY Regional
 * Solution by Fred Pickel
 * Problem idea by Fred Pickel
 * Problem statement by John Buck
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define MAX_PERIMETER 10000

// we want sides of the pen a, b, c with a <= b <= c with a+b > c and a+b+c = perimeter
// if a < b < c, we get two different pens by flipping the triangle over 
// c must be <= floor((perimeter-1)/2)	[c >= periemter/2 means a+b not > c)
// c must be >= ceiling(perimeter/3)	[c >= b >=a and a+b+c = perimeter smallest c when all are (close to equal)
// for a given value of c, b must be <= c and <= perimeter - c - 1 [b = perimeter - c - a
// NOTE that c <= floor((perimeter-1)/2) means c <= perimeter - c - 1 so it is enough to say b <= c
// also b >= ceiling((perimeter - c)/2)	[ a+b = perimeter -c and b >= a
// when b = c, we get only one "different" triangle
// if a = b, we  get only one "different" triangle, this can only happen if (perimeter - c) is even
// if perimeter is divisible by 3, we can have a = b = c. do not count twice as a = b and b = c
int GetPerimeterCount(int perimeter)
{
	int count, c;
	count = 0;
	for(c = (perimeter - 1)/2; c >= (perimeter + 2)/3 ; c--) {
		if(c == (perimeter/3)) {	// only one triangle a=b=c
			count += 1;
		}
		else if(((perimeter - c) & 1) == 0) {	// even, we get 1 for b = c and 1 for b = a and 2 each for the rest
			count += 1 + 2*(c - (perimeter - c + 1)/2 - 1) + 1;
		}
		else {	// perimeter - c is odd, b cannot = a, count b=c once and the rest twice
			count += 1 + 2*(c - (perimeter - c + 1)/2);
		}
	}
	return count;
}

char inbuf[256];

int main()
{
	int nprob, curprob, index, perimeter, count;

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
			fprintf(stderr, "Read failed on problem %d data header\n", curprob);
			return -3;
		}
		// get prob num and count of guards
		if(sscanf(&(inbuf[0]), "%d %d", &index, &perimeter) != 2)
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
		if((perimeter < 3) || (perimeter > MAX_PERIMETER)) {
			fprintf(stderr, "problem index %d guard count %d not in range 1-%d\n",
				curprob, perimeter, MAX_PERIMETER);
			return -7;
		}
		count = GetPerimeterCount(perimeter);
		printf("%d %d\n", curprob, count);
	}
	return 0;
}
