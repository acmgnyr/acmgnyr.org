#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define EPS (.00001)
#define EPS2 (.0001)
#define EPS3 (.001)
double equalizers[6][4];
char inbuf[256];
FILE *infp;

double frand()
{
	double ret;
	int x = rand()  & 0x7fff;
	ret = (double)x;
	ret /= 32767.0;
	return ret;
}

int CheckEqSolns(double a, double b, double c, double *pSoln)
{
	double perim, discr, x, y;
	int ret = 0;
	perim = a+b+c;
	discr = perim*perim - 8*b*c;
	if(discr < 0) {
		return 0;
	}
	discr = sqrt(discr);
	x = (perim + discr)/(4.0*c);
	y = (perim - discr)/(4.0*b);
	if((x > 0.0) && (x <= (1.0 + EPS)) && (y > 0.0) && (y <= (1.0 + EPS))) {
		ret++;
		pSoln[0] = x;
		pSoln[1] = y;
	}
	x = (perim - discr)/(4.0*c);
	y = (perim + discr)/(4.0*b);
	if((x > 0.0) && (x <= (1.0 + EPS)) && (y > 0.0) && (y <= (1.0 + EPS))) {
		pSoln[2*ret] = x;
		pSoln[2*ret+1] = y;
		ret++;
	}
	return ret;
}

int FindEqualizers(double *pVerts)
{
	double vAB[2], vAC[2], vBC[2], soln[4];
	double a, b, c, x0, y0, x1, y1, rA, rB, denom;
	int ret, nEq;
	char bisA, bisB, bisC, skip;
	bisA = bisB = bisC = 0;
	nEq = 0;
	vAB[0] = pVerts[2] - pVerts[0];
	vAB[1] = pVerts[3] - pVerts[1];
	vAC[0] = pVerts[4] - pVerts[0];
	vAC[1] = pVerts[5] - pVerts[1];
	vBC[0] = pVerts[4] - pVerts[2];
	vBC[1] = pVerts[5] - pVerts[3];
	a = sqrt(vBC[0]*vBC[0] + vBC[1]*vBC[1]);
	b = sqrt(vAC[0]*vAC[0] + vAC[1]*vAC[1]);
	c = sqrt(vAB[0]*vAB[0] + vAB[1]*vAB[1]);
	if((ret = CheckEqSolns(a, b, c, &(soln[0]))) > 0) { // soln crossin AB and AC
		x0 = pVerts[0] + vAB[0]*soln[0];
		y0 = pVerts[1] + vAB[1]*soln[0];
		x1 = pVerts[0] + vAC[0]*soln[1];
		y1 = pVerts[1] + vAC[1]*soln[1];
		if(fabs(soln[0] - 0.5) < EPS2) {
			bisC = 1;
		}
		if(fabs(soln[1] - 0.5) < EPS2) {
			bisB = 1;
		}
		rA = y1 - y0;
		rB = -(x1 - x0);
		if(rA < 0.0) {
			rA = -rA;
			rB = -rB;
		}
		denom = sqrt(rA*rA + rB*rB);
		equalizers[nEq][0] = rA/denom;
		equalizers[nEq][1] = rB/denom;
		equalizers[nEq][2] = (rA*x0 + rB*y0)/denom;
		nEq++;
		if(ret == 2) {
			skip = 0;
			if(fabs(soln[2] - 0.5) < EPS2) {
				if(bisC == 1) skip++;
				bisC = 1;
			}
			if(fabs(soln[3] - 0.5) < EPS2) {
				if(bisB == 1) skip++;
				bisB = 1;
			}
			if(skip == 0) {
				x0 = pVerts[0] + vAB[0]*soln[2];
				y0 = pVerts[1] + vAB[1]*soln[2];
				x1 = pVerts[0] + vAC[0]*soln[3];
				y1 = pVerts[1] + vAC[1]*soln[3];
				rA = y1 - y0;
				rB = -(x1 - x0);
				if(rA < 0.0) {
					rA = -rA;
					rB = -rB;
				}
				denom = sqrt(rA*rA + rB*rB);
				equalizers[nEq][0] = rA/denom;
				equalizers[nEq][1] = rB/denom;
				equalizers[nEq][2] = (rA*x0 + rB*y0)/denom;
				nEq++;
			}
		}
	}
	if((ret = CheckEqSolns(b, c, a, &(soln[0]))) > 0) { // soln crossing BC and BA
		skip = 0;
		if(fabs(soln[0] - 0.5) < EPS2) {
			if(bisA == 1) skip++;
			bisA = 1;
		}
		if(fabs(soln[1] - 0.5) < EPS2) {
			if(bisC == 1) skip++;
			bisC = 1;
		}
		if(skip == 0) {
			x0 = pVerts[2] + vBC[0]*soln[0];
			y0 = pVerts[3] + vBC[1]*soln[0];
			x1 = pVerts[2] - vAB[0]*soln[1];
			y1 = pVerts[3] - vAB[1]*soln[1];
			rA = y1 - y0;
			rB = -(x1 - x0);
			if(rA < 0.0) {
				rA = -rA;
				rB = -rB;
			}
			denom = sqrt(rA*rA + rB*rB);
			equalizers[nEq][0] = rA/denom;
			equalizers[nEq][1] = rB/denom;
			equalizers[nEq][2] = (rA*x0 + rB*y0)/denom;
			nEq++;
		}
		if(ret == 2) {
			skip = 0;
			if(fabs(soln[2] - 0.5) < EPS2) {
				if(bisA == 1) skip++;
				bisA = 1;
			}
			if(fabs(soln[3] - 0.5) < EPS2) {
				if(bisC == 1) skip++;
				bisC = 1;
			}
			if(skip == 0) {
				x0 = pVerts[2] + vBC[0]*soln[2];
				y0 = pVerts[3] + vBC[1]*soln[2];
				x1 = pVerts[2] - vAB[0]*soln[3];
				y1 = pVerts[3] - vAB[1]*soln[3];
				rA = y1 - y0;
				rB = -(x1 - x0);
				if(rA < 0.0) {
					rA = -rA;
					rB = -rB;
				}
				denom = sqrt(rA*rA + rB*rB);
				equalizers[nEq][0] = rA/denom;
				equalizers[nEq][1] = rB/denom;
				equalizers[nEq][2] = (rA*x0 + rB*y0)/denom;
				nEq++;
			}
		}
	}
	if((ret = CheckEqSolns(c, a, b, &(soln[0]))) > 0) { // soln crossing CA and CB
		skip = 0;
		if(fabs(soln[0] - 0.5) < EPS2) {
			if(bisB == 1) skip++;
			bisB = 1;
		}
		if(fabs(soln[1] - 0.5) < EPS2) {
			if(bisA == 1) skip++;
			bisA = 1;
		}
		if(skip == 0) {
			x0 = pVerts[4] - vAC[0]*soln[0];
			y0 = pVerts[5] - vAC[1]*soln[0];
			x1 = pVerts[4] - vBC[0]*soln[1];
			y1 = pVerts[5] - vBC[1]*soln[1];
			rA = y1 - y0;
			rB = -(x1 - x0);
			if(rA < 0.0) {
				rA = -rA;
				rB = -rB;
			}
			denom = sqrt(rA*rA + rB*rB);
			equalizers[nEq][0] = rA/denom;
			equalizers[nEq][1] = rB/denom;
			equalizers[nEq][2] = (rA*x0 + rB*y0)/denom;
			nEq++;
		}
		if(ret == 2) {
			skip = 0;
			if(fabs(soln[2] - 0.5) < EPS2) {
				if(bisB == 1) skip++;
				bisB = 1;
			}
			if(fabs(soln[3] - 0.5) < EPS2) {
				if(bisA == 1) skip++;
				bisA = 1;
			}
			if(skip == 0) {
				x0 = pVerts[4] - vAC[0]*soln[2];
				y0 = pVerts[5] - vAC[1]*soln[2];
				x1 = pVerts[4] - vBC[0]*soln[3];
				y1 = pVerts[5] - vBC[1]*soln[3];
				rA = y1 - y0;
				rB = -(x1 - x0);
				if(rA < 0.0) {
					rA = -rA;
					rB = -rB;
				}
				denom = sqrt(rA*rA + rB*rB);
				equalizers[nEq][0] = rA/denom;
				equalizers[nEq][1] = rB/denom;
				equalizers[nEq][2] = (rA*x0 + rB*y0)/denom;
				equalizers[nEq][3] = 2*y1 - y0;
				nEq++;
			}
		}
	}
	return nEq;
}

int MakeTri(double scale, double minsize, double *pVerts)
{
	int i, j;
	double vAB[2], vAC[2], vBC[2], a, b, c, area;
	for(i = 0; i < 1000 ; i++)
	{
		for(j = 0; j < 6; j++)
			pVerts[j] = scale*frand();
		vAB[0] = pVerts[2] - pVerts[0];
		vAB[1] = pVerts[3] - pVerts[1];
		vAC[0] = pVerts[4] - pVerts[0];
		vAC[1] = pVerts[5] - pVerts[1];
		vBC[0] = pVerts[4] - pVerts[2];
		vBC[1] = pVerts[5] - pVerts[3];
		area = fabs(vAB[0] * vAC[1] - vAB[1] * vAC[0]);
		c = sqrt(vAB[0]*vAB[0] + vAB[1]*vAB[1]);
		b = sqrt(vAC[0]*vAC[0] + vAC[1]*vAC[1]);
		a = sqrt(vBC[0]*vBC[0] + vBC[1]*vBC[1]);
		if((a >= minsize) && (b >= minsize) && 
			(c >= minsize) && (area >= minsize)) {
			return 0;
		}
	}
	return -1;
}

int main(int argc, char **argv)
{
	int probnum, count, inprob, nprob, index;
	double scale, minsize, triverts[6];
	if(argc < 5) {
		fprintf(stderr, "USAGE: %s <infile> <scale> <minsize> <num tris required>\n", argv[0]);
		return -1;
	}
	if((infp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "cannot open input file \"%s\" for read\n", argv[1]);
		return -2;
	}
	scale = atof(argv[2]);
	minsize = atof(argv[3]);
	count = atoi(argv[4]);
	probnum = 0;
	if(count < 1) {
		fprintf(stderr, "count %d must be >= 1\n", count);
		return -3;
	}
	if(scale < 10.0 * minsize) {
		fprintf(stderr, "scale %f must be >= 10 * minsize %f\n", scale, minsize);
		return -4;
	}
	srand( (unsigned)time( NULL ) );
	if(fgets(&(inbuf[0]), 255, infp) == NULL)
	{
		fprintf(stderr, "Read failed on problem count\n");
		return -5;
	}
	if(sscanf(&(inbuf[0]), "%d", &nprob) != 1)
	{
		fprintf(stderr, "Scan failed on problem count\n");
		return -6;
	}
	printf("%d\n", count);
	for(inprob = 1; inprob <= nprob ; inprob++)
	{
		if(fgets(&(inbuf[0]), 255, infp) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d data\n", inprob);
			return -3;
		}
		if(sscanf(&(inbuf[0]), "%d %lf %lf %lf %lf %lf %lf", &index,
			&(triverts[0]), &(triverts[1]), &(triverts[2]),
			&(triverts[3]), &(triverts[4]), &(triverts[5])) != 7)
		{
			fprintf(stderr, "Scan failed on problem %d data\n", inprob);
			return -4;
		}
		if(index != inprob)
		{
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				index, inprob);
			return -7;
		}
		if((FindEqualizers(&(triverts[0])) == 1) && (equalizers[0][0] > EPS3)) {
			probnum++;
			printf("%d %.5f %.5f %.5f %.5f %.5f %.5f\n", probnum,
				triverts[0], triverts[1], triverts[2], 
				triverts[3], triverts[4], triverts[5]);
		}
	}
	while(probnum < count) {
		if(MakeTri(scale, minsize, &(triverts[0])) != 0) {
			return -5;
		}
		if((FindEqualizers(&(triverts[0])) == 1) && (equalizers[0][0] > EPS3)) {
			probnum++;
			printf("%d %.5f %.5f %.5f %.5f %.5f %.5f\n", probnum,
				triverts[0], triverts[1], triverts[2], 
				triverts[3], triverts[4], triverts[5]);
		}
	}
	return 0;
}
