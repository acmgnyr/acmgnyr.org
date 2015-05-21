/*
 * I - Golden Ceiling
 * Greater NY Regional ACM Contest 2011
 * Solution by Fred Pickel
 */
#include <stdio.h>
#include <math.h>

#define EPS (0.0001)
#define BIG	(1.0e30)

double getNorm(double *v)
{
	double norm;
	norm = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	return sqrt(norm);
}

double vert[8][3];

double vertVals[8];

double applyEqn(double *eqn, double *vert)
{
	return (eqn[0] * vert[0] + eqn[1] * vert[1] + eqn[2] * vert[2] - eqn[3]);
}


/*
 *		     1 ---- 3
 *			/|     /|  z
 *		   5 ---- 7 |  |
 *		   | 0 ---| 2  .-- y
 *		   |/     |/  /
 *		   4 ---- 6  x
 */
int topFaceVerts[5] = {1, 3, 7, 5, 1};
int bottomFaceVerts[5] = {0, 2, 6, 4, 0};

void SetVerts(double a, double b, double c)
{
	int i;
	for(i = 0; i < 8 ; i++)
	{
		if((i & 1) != 0)
		{
			vert[i][2] = c;
		}
		else
		{
			vert[i][2] = 0.0;
		}
		if((i & 2) != 0)
		{
			vert[i][1] = b;
		}
		else
		{
			vert[i][1] = 0.0;
		}
		if((i & 4) != 0)
		{
			vert[i][0] = a;
		}
		else
		{
			vert[i][0] = 0.0;
		}
	}
}

/*
 * find point between vert[posInd] and vert[negInd] where linear
 * function posVal at posInd and negVal ar negInd would be 0
 */
int FindEdgeInter(double pos, double neg, int posInd, int negInd, double *res)
{
	double posFact, negFact, denom;
	denom = pos - neg;
	posFact = -neg/denom;
	negFact = pos/denom;
	res[0] = posFact * vert[posInd][0] + negFact * vert[negInd][0];
	res[1] = posFact * vert[posInd][1] + negFact * vert[negInd][1];
	res[2] = posFact * vert[posInd][2] + negFact * vert[negInd][2];
	return 0;
}

/*
 *	return area of polygon with all z-values the same
 * just do z component of cross product
 */
double FindFlatPolyArea(int nPts, double pts[][3])
{
	double v1[2], v2[2], tmp, sum;
	int i, j;
	sum = 0.0;
	for(j = 1; j < nPts - 1 ; j++) {
		for(i = 0; i < 2; i++) {
			v1[i] = pts[j][i] - pts[0][i];
			v2[i] = pts[j+1][i] - pts[0][i];
		}
		tmp = v1[0] * v2[1] - v2[0] * v1[1];
		sum += tmp;
	}
	return (0.5 * fabs(sum));
}

double FindFaceArea(int *vertInds)
{
	int curInd, nxtInd, nPts, i;
	double dverts[10][3], curVal, nxtVal;
	curInd = vertInds[0];
	nPts = 0;
	curVal = vertVals[curInd];
	for(i = 0; i < 4; i++) {
		if(curVal <= EPS) {
			dverts[nPts][0] = vert[curInd][0];
			dverts[nPts][1] = vert[curInd][1];
			dverts[nPts][2] = vert[curInd][2];
			nPts++;
		}
		nxtInd = vertInds[i+1];
		nxtVal = vertVals[nxtInd];
		if((curVal*nxtVal < EPS) &&	// change sign
			(fabs(curVal) > EPS) && (fabs(nxtVal) > EPS)) {	// don't duplicate zero pts
			if(nxtVal > 0.0) {
				FindEdgeInter(nxtVal, curVal, nxtInd, curInd, &(dverts[nPts][0]));
			} else {
				FindEdgeInter(curVal, nxtVal, curInd, nxtInd, &(dverts[nPts][0]));
			}
			nPts++;
		}
		curInd = nxtInd;
		curVal = nxtVal;
	}
	curVal = FindFlatPolyArea(nPts, dverts);
	return curVal;
}

/*
 * total area is top area + slant area
 * slant area = area of projection (of slant area on bottom) / cos of angle of plane with horizontal
 * cos of angle of plane with horizontal = C/sqrt(A*A + B*B + C*C)
 * area of projection on bottom = bottom area - top area
 * bottom area = area of base which is below the plane
 * top area = area of top which is below the plane
 * total area = top_area + (bottom_area - top_area)/cos of angle
 */

double CompArea1(double *eqn, double *corner)
{
	double val, area, minVal, maxVal;
	int i, minIndex, maxIndex;
	double cos_phi, top_area, bottom_area;

	if((corner[0] < EPS) || (corner[1] < EPS) || (corner[2] < EPS))
	{
		return -1.0;
	}
	if(eqn[2] < 1.0)
	{	// vertical plane
		return -2.0;
	}
	// cos of angle between plane perpendicular and z axis
	cos_phi = eqn[2]/getNorm(&(eqn[0]));
	maxVal = 0.0;
	minVal = 0.0;
	minIndex = maxIndex = -1;
	area = 0.0;
	SetVerts(corner[0], corner[1], corner[2]);
	for(i = 0; i < 8; i++)
	{
		val = applyEqn(eqn, &(vert[i][0]));
		vertVals[i] = val;
		if(val > maxVal)
		{
			maxVal = val;
			maxIndex = i;
		}
		if(val < minVal)
		{
			minVal = val;
			minIndex = i;
		}
	}
	/* sanity check */
	if((minIndex == -1) || (maxIndex == -1))
	{	// all veritces below plane or all above
		return -3.0;
	}
	area = 0.0;
	// see if plane hits top of box
	if((vertVals[1] >= 0.0) && (vertVals[3] >= 0.0) && 
		(vertVals[5] >= 0.0) && (vertVals[7] >= 0.0)) {
		/* plane does not hit top of box */
		top_area = 0.0;
	} else {
		top_area = FindFaceArea(topFaceVerts);
	}

	if((vertVals[0] <= 0.0) && (vertVals[2] <= 0.0) && 
		(vertVals[4] <= 0.0) && (vertVals[6] <= 0.0)) {
		/* does not hit bottom, arrea is whole base */
		bottom_area = corner[0] * corner[1];
	} else {
		bottom_area = FindFaceArea(bottomFaceVerts);
	}
	area = top_area + (bottom_area - top_area)/cos_phi;
	return area;
}

int main()
{
	int nprob, curprob, index;
	double eqn[4], corn[3], result; //, result1;
	char inbuf[256];

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
		if(sscanf(&(inbuf[0]), "%d %lf %lf %lf %lf %lf %lf %lf",
			&index, &(corn[0]), &(corn[1]), &(corn[2]),
			&(eqn[0]), &(eqn[1]), &(eqn[2]), &(eqn[3])) != 8)
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
		if((corn[0] < EPS) || (corn[1] < EPS) || (corn[2] < EPS))
		{
			fprintf(stderr, "problem index %d corner val too small %lf %lf %lf\n",
				curprob, corn[0], corn[1], corn[2]);
			return -8;
		}
		if(eqn[2] < 1.0)
		{
			fprintf(stderr, "problem index %d z coeff %lf < 1.0\n",
				curprob, eqn[2]);
			return -9;
		}
		result = CompArea1(eqn, corn);
		printf("%d %d\n", curprob, (int)(ceil(result)));
	}
	return 0;
}
