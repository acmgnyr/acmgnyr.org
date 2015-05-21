/*
 * H - Windmill Animation
 * ICPC 2012 Greater NY Regional
 * Solution by Fred Pickel
 * Problem by Fred Pickel
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define PI	(3.141592653589793238462643383279)
#define EPS (.001)
#define EPS2 (.0005)

#define MAX_PTS	20
#define MAX_STEPS 20

double x[MAX_PTS+1], y[MAX_PTS+1];
double toAng[MAX_PTS+1][MAX_PTS+1];
int sortInd[MAX_PTS+1][MAX_PTS+1];
int stepIndex[MAX_STEPS+1];

// For each point, we find the angle of the line from the point to each other point
// counterclockwise from horizontal
// then sort the angles in order of increasing size
// when done toAng[i][j] is the angle from point i to point sortInd[i][j]
// and is the j-th increasing size
int FindAngs(int nPts)
{
	int i, j, k, ind;
	double srcx, srcy, ang;

	for(i = 1; i <= nPts; i++) {
		srcx = x[i];
		srcy = y[i];
		for(j = 1; j <= nPts; j++) {
			sortInd[i][j] = j;
			if(i == j) {
				toAng[i][j] = -1.0;
			} else {
				if((fabs(x[j]- srcx) + fabs(y[j] - srcy)) < EPS) {
					fprintf(stderr, "Points %d and %d too close together\n", i, j);
					return -21;
				}
				ang = atan2(y[j] - srcy, x[j]- srcx);
				if(ang >= PI) ang -= PI;
				else if(ang < 0.0) ang += PI;
				toAng[i][j] = ang;
			}
		}
		// bubble sort I am lazy
		for(j = nPts; j > 1; j--) {
			ind = sortInd[i][1];
			ang = toAng[i][1];
			for(k = 2; k <= j; k++) {
				if(ang >= toAng[i][k]) { // next is smaller, move it up
					toAng[i][k-1] = toAng[i][k];
					sortInd[i][k-1] = sortInd[i][k];
				} else {	// next is larger, cur ang has found it place
					toAng[i][k-1] = ang;
					sortInd[i][k-1] =ind;
					ind = sortInd[i][k];
					ang = toAng[i][k];
				}
			}
			toAng[i][j] = ang;
			sortInd[i][j] = ind;
		}
		// check for dup angs
		for(j = 1; j < nPts; j++) {
			if(fabs(toAng[i][j] - toAng[i][j+1]) < EPS){
				fprintf(stderr, "points %d %d %d are all (almost) on the same line\n", i, sortInd[i][j], sortInd[i][j+1]);
				return -22;
			}
		}
	}
	return 0;
}

// to find the point sequence, we search toAng[curPt][.] to find the next angle, toAng[curPt][j],
// just larger than the current angle
// if the current angle is larger than any of these angles we start back at the beginning and set the angle to -EPS
// so we will select the first point (other than curPt) in the array
// the next point in the sequence is the point this angle belongs to, sortInd[curPt][j]
int FindPtSequence(int nPts, int initPt, double initAng, int steps)
{
	int curStep, curPt, k;
	double curAng;
	curPt = initPt;
	curAng = initAng;
	for(curStep = 0; curStep <= steps; curStep++) {
		stepIndex[curStep] = curPt;
		// find next angle for cur Pt
		if(curAng >= toAng[curPt][nPts]) {	// past the last point co back to 0
			curAng = -EPS;
		}
		for(k = 1; k <= nPts; k++) {
			if(toAng[curPt][k] > curAng) {
				curAng = toAng[curPt][k] + EPS2;
				curPt = sortInd[curPt][k];
				break;
			}
		}
	}
	return 0;
}

char inbuf[256];


int main()
{
	int nprob, curprob, index, ret, nPts, initPt, curPt, steps, i;
	double initAng;

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
			fprintf(stderr, "Read failed on problem header problem index  %d \n", curprob);
			return -3;
		}
		// get problem header
		if(sscanf(&(inbuf[0]), "%d %d %d %d %lf", &index, &nPts, &steps, &initPt, &initAng) != 5)
		{
			fprintf(stderr, "scan failed on problem header problem index %d\n",
				curprob);
			return -6;
		}
		if(curprob != index) {
			fprintf(stderr, "problem index %d not = expected %d\n",
				index, curprob);
			return -7;
		}
		if((nPts < 3) || (nPts > MAX_PTS)) {
			fprintf(stderr, "num points %d not in range 3 to %d problem %d\n",
				nPts, MAX_PTS, curprob);
			return -8;
		}
		if((steps < 3) || (steps > MAX_STEPS)) {
			fprintf(stderr, "step count %d not in range 3 to %d problem %d\n",
				steps, MAX_STEPS, curprob);
			return -8;
		}
		if((initPt < 1) || (initPt > nPts)) {
			fprintf(stderr, "initial point index %d not in range 1 to %d problem %d\n",
				initPt, nPts, curprob);
			return -9;
		}
		if((initAng < 0.0) || (initAng >= 180.0)) {
			fprintf(stderr, "initial angle %lf not in range 0.0 to 180.0 problem %d\n",
				initAng, curprob);
			return -10;
		}
		for(curPt = 1; curPt <= nPts ; curPt++) {
			if(fgets(&(inbuf[0]), 255, stdin) == NULL)
			{
				fprintf(stderr, "Read failed on point %d problem index  %d \n", curPt, curprob);
				return -11;
			}
			// get point data
			if(sscanf(&(inbuf[0]), "%d %lf %lf", &index, &(x[curPt]), &(y[curPt])) != 3)
			{
				fprintf(stderr, "scan failed on point data %d problem index %d\n",
					curPt, curprob);
				return -12;
			}
		}
		if((ret = FindAngs(nPts)) != 0) {
			return ret;
		}
		initAng *= (PI/180.0);
		if((ret = FindPtSequence(nPts, initPt, initAng, steps)) != 0) {
			return ret;
		}
		printf("%d", curprob);
		for(i = 1; i <= steps ; i++) {
			printf(" %d", stepIndex[i]);
		}
		printf("\n");
	}
	return 0;
}
