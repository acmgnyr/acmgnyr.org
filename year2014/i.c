#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char inbuf[256];
#define MAX_RINGS	15
#define MAX_STONES_PER_RING	20
#define M_PI	((double)(3.141592653589793238462643383279))

/*
 * for the first ring of circles, the distance from the center of the patio (center of the central stone)
 * is 1 + radiius of first ring stones
 * distance between two ring stones = 2*radius = 2*sin(PI/numstones)*(1 + radius)
 * so first radius = sin(PI/numstones)/(1 - sin(PI/numstones))
 * for the remaining rings let R1 be the distance from the patio center to the centers of the previous ring stones
 * r1 = radius of the previous ring stones r2 = radius of the next ring stones
 * R2 the distance from the patio center to the centers of the next ring stones
 * choose coordinates centered at the center of the patio with y-axis thru the center of a previous ring stone
 * The center of that stone is at (0, R1)
 * the center of the adjacent stones in the next ring are at (+-R2*(PI/numstones), R2*cos(PI/numstones))
 * r2 = R2*sin(PI/numstones)
 * the distance from the center of the previous circle stone to the center of an adjacent next circle stone is
 * (lletting SI = sin(PI/numstones) and CS = cos(PI/numstones))
 * (r1 + r2) = sqrt((R2*SI)^2 + (R2*CS - R1)^2) SO
 * r1*r1 + 2*r1*r2 + r2*r2 = (R2*SI)^2 + R2*R2*CS*CS -2*R2*R1*CS + R1*R1 
 * using r2 = R2*SI
 * 0 = R2*R2*(CS*CS) - 2*R2*(R1*CS + r1*SI) +R1*R1 - r1*r1
 * using the quadratic formula (with + sign)
 * R2 = ((R1*CS + r1*SI) + sqrt((R1*R1*CS*CS + 2*R1*r1*CS*SI + r1*r1*SI*SI) - R1*R1*CS*CS + r1*r1*CS*CS))/CS*CS
 *	  = ((R1*CS + r1*SI) + sqrt(r1*r1 + 2*R1*r1*CS*SI))/CS*CS
 *
 * to find then length of the fence notice that the curved parts around the outer stones add up to one complete circle
 *	= 2*PI*r and each straight segment is 2*r so the total length is 2*PI*r + 2*r*numstones
 */
int doLayout(int seq, int stonePerRing, int nRings, double *pMaxRadius, double *pLen)
{
	int i;
	double si, cs, R1, r1, R2, r2;
	si = sin(M_PI/((double)stonePerRing));
	cs = cos(M_PI/((double)stonePerRing));
	r1 = si/(1.0 - si);
	R1 = 1.0 + r1;
	for(i = 1; i < nRings ; i++) {
		R2 = R1 * cs + r1 * si + sqrt(r1 * r1 + 2*r1*R1*cs*si);
		R2 = R2/(cs*cs);
		r2 = R2*si;
		R1 = R2;
		r1 = r2;
	}
	*pMaxRadius = r1;
	*pLen = 2.0* r1 * (M_PI + ((double)stonePerRing));
	return 0;
}

int main()
{
	int nprob, curprob, ret, index, stonePerRing, nRings;
	double maxRadius, fenceLen;

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
		// get prob num and stones per ring and number of rings
		if(sscanf(&(inbuf[0]), "%d %d %d", &index, &stonePerRing, &nRings) != 3)
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
		if((stonePerRing < 3) || (stonePerRing > MAX_STONES_PER_RING) || (nRings < 1) || (nRings > MAX_RINGS)) {
			fprintf(stderr, "problem index %d stones per ring %d not in range 3-%d or num rings %d not in range 1 - %d\n",
				curprob, stonePerRing, MAX_STONES_PER_RING, nRings, MAX_RINGS);
			return -7;
		}
		ret = doLayout(curprob, stonePerRing, nRings, &maxRadius, &fenceLen);
		if(ret != 0) {
			return ret;
		}
		printf("%d %.3f %.3f\n", curprob, maxRadius, fenceLen);
	}
	return 0;
}
