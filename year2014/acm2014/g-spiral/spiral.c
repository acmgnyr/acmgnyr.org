/*
 * G - Spiral solution
 * ICPC 2014 Greater NY Regional
 * Solution by Fred Pickel
 * Problem by Fred Pickel
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

int steps[7];

// since each up segment is at least 2 longer than the previous down segment
// and similarly every right segment is at least two longer than the previous left segment
// each time we go around the origin, in the first quadrant, y increases by 2 and x increases by 2
// on the first pass (2 segments) we reach (x,y) with 1 <= x < y, so if x < y, we just use 2 segments, x followed by y
// on the second pass y >= 4
// to hit (x,y) with 4 <= y <= x (we got y >x on the first pass
// the shortest first 3 segments are 1, 2, 3 which ends at (-2, 2)
// to get to x, the second last segment must go from (-2,z) to (x,z) so must have length (x+2)
// this means that the segment to (x,y) must have length at least (x+3)
// this means z, the y value for the second last segment must be <= (y - (x+3)) = (y-x-3)
// this means the down segment to (-2,z) must have length >= 2 - (y-x-3) = 5+x-y
// since y >= 4, if we use z = (y-x-3), the third last segment has lenght <= x+1
// so is legal in an increasing spiral
// in any other case (y < 4) && (y <= x) there is no path
int FindSpiral(int index, int x, int y)
{
	if(y > x) {
		printf("%d 2 %d %d\n", index, x, y);
		steps[0] = 2;
		steps[1] = x;
		steps[2] = y;
	} else if (y >= 4) {
		printf("%d 6 1 2 3 %d %d %d\n", index, x + 5 - y, x+2, x+3);
		steps[0] = 6;
		steps[1] = 1;
		steps[2] = 2;
		steps[3] = 3;
		steps[4] = x + 5 - y;
		steps[5] = x+2;
		steps[6] = x+3;
	} else {
		printf("%d NO PATH\n", index);
		steps[0] = 0;
	}
	return steps[0];
}

int CheckSpiral(int x, int y)
{
	int xi, yi;
	xi = yi = 0;
	if(steps[0] == 0) {
		return 0;
	} else if(steps[0] == 2) {
		xi += steps[1];
		yi += steps[2];
	} else if(steps[0] == 6) {
		xi += steps[1];
		yi += steps[2];
		xi -= steps[3];
		yi -= steps[4];
		xi += steps[5];
		yi += steps[6];
	} else {
		printf("steps %d\n", steps[0]);
		return -1;
	}
	if((xi != x) || (yi != y)) {
		printf("(%d, %d) != (%d, %d)\n", xi, yi, x, y);
		return -2;
	}
	return 0;
}

char inbuf[256];

int main(int argc, char **argv)
{
	int nprob, curprob, index, x, y;

	if(fgets(&(inbuf[0]), 255, stdin) == NULL)
	{
		fprintf(stderr, "Read failed on problem and star count\n");
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
			fprintf(stderr, "Read failed on problem data problem index  %d \n", curprob);
			return -3;
		}
		if(sscanf(&(inbuf[0]), "%d %d %d", &index, &x, &y) != 3)
		{
			fprintf(stderr, "scan failed on problem data problem index %d\n",
				curprob);
			return -6;
		}
		if(curprob != index) {
			fprintf(stderr, "problem index %d not = expected %d\n",
				index, curprob);
			return -7;
		}
		FindSpiral(index, x, y);
		if(CheckSpiral(x, y) != 0) {
			printf("error\n");
		}
	}
	return 0;
}
