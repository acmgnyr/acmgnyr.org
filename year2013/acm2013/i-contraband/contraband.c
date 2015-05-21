/*
 * I - Contraband
 * ICPC 2013 Greater NY Regional
 * Solution by Fred Pickel
 * Problem by Fred Pickel
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define EPS .01
#define MAX_STARS	100
#define MAX_SUBPROBS 10
#define PI	(3.141592653589793238462643383279)

#define BASE_X1 (3.715)
#define BASE_Y1 (1.761)
#define BASE_X2 (2.894)
#define BASE_Y2 (-2.115)

double cx[3], cy[3], s[3], c[3];
double e[3], a[3];

void InitLocs()
{
	cx[0] = cy[0] = 0.0;
	cx[1] = BASE_X1;
	cy[1] = BASE_Y1;
	cx[2] = BASE_X2;
	cy[2] = BASE_Y2;
}

// parametric eqn of line i is x = cx[i] + t*s[i]; y = cy[i] + t*c[i];	// s[i] = sin(ang[i]) c[i] = cos(ang[i])
// viewpoint of observer i os at (cx[i], cy[i])
// regular eqn of line i is -c[i] * (x - cx[i]) + s[i] * (y - cy[i]) = signed distance from (x,y) to line i = 0;
// to find intersection of line i and line j sub parametric eqn for 1 into regular eqn for the other
// -c[i] * (cx[j] + t*s[j] - cx[i]) + s[i] * (cy[j] + t * c[j] - cy[i]) = 0
// t *(-si*cj + ci*sj) = si *(cxj - cx[i]) - ci * (cyj - cy[i])
// The function we are trying to minimize is
// SUM(i = 0 to 2) {(0.2 + err[i]) * (dist to line i)^2 =
// SUM(i = 0 to 2) {(0.2 + err[i]) * (-c[i] * (x - cx[i]) + s[i] * (y - cy[i]))^2
// the minimum on the triangle occurs
// at an interior point where the grdient is zero
// Gradient =
// SUM(i = 0 to 2) {(0.2 + err_i) * 2.0 * (-c[i] * (x - cx[i]) + s[i] * (y - cy[i])) * [-c[i], s[i]]}
// so gradient = 0 means
// (SUM(i= 0 to 2) {0.2+err_i) * 2.0 * c[i]*c[i]} ) *x - SUM(i= 0 to 2) {0.2+err_i) * 2.0 * c[i]*s[i]} ) * y = 
//		SUM(i= 0 to 2) {0.2+err_i) * 2.0 * (c[i]*c[i]*cx[i] - c[i]*s[i]*cy[i])} )	// x coord of gradient = 0
// Ax + By = E
// -(SUM(i= 0 to 2) {0.2+err_i) * 2.0 * c[i]*s[i]} ) *x + SUM(i= 0 to 2) {0.2+err_i) * 2.0 * s[i]*s[i]} ) * y = 
//		SUM(i= 0 to 2) {0.2+err_i) * 2.0 * (-c[i]*s[i]*cx[i] + s[i]*s[i]*cy[i])} )	// x coord of gradient = 0
// Cx + Dy = F
// x = (E*D - F*B)/A*D - B*C)	 y = (A*F - C*E)/(A*D - B*C)

int FindCenter(double *px, double *py)
{
	double A, B, C, D, E, F, denom;
	int i;
	A = B = C = D = E = F = 0.0;
	for( i= 0; i < 3 ; i++) {
		c[i] = cos((a[i] * PI)/180.0);
		s[i] = sin((a[i] * PI)/180.0);
		A += (0.2 + e[i])*2.0*c[i]*c[i];
		B -= (0.2 + e[i])*2.0*s[i]*c[i];
		C -= (0.2 + e[i])*2.0*c[i]*s[i];
		D += (0.2 + e[i])*2.0*s[i]*s[i];
		E += (0.2 + e[i])*2.0*(c[i]*c[i]*cx[i] - c[i]*s[i]*cy[i]);
		F += (0.2 + e[i])*2.0*(s[i]*s[i]*cy[i] - c[i]*s[i]*cx[i]);
	}
	denom = A*D - B*C;
	if(fabs(denom) < EPS) {
		fprintf(stderr, "denom %g A %g B %g C %g D %g\n", denom, A, B, C, D);
		return -1;
	}
	*px = (E*D - F*B)/denom;
	*py = (A*F - C*E)/denom;
	return 0;
}

char inbuf[256];

int main()
{
	int nprob, curprob, index, ret;
	double x, y;

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
	InitLocs();
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem data problem index  %d \n", curprob);
			return -3;
		}
		// get problem data
		if(sscanf(&(inbuf[0]), "%d %lf %lf %lf %lf %lf %lf", &index, &a[0], &e[0], &a[1], &e[1], &a[2], &e[2]) != 7)
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
		if((ret = FindCenter(&x, &y)) != 0) {
			return ret;
		}
		printf("%d %0.3lf %0.3lf\n", curprob, x, y);
	}
	return 0;
}
