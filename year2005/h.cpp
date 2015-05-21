/*
 * 2005 ACM Greater New York Regional Collegiate Programming Contest
 * Problem H -- Model Rocket Height
 */

#include <stdio.h>
#include <math.h>

/*
 * take the y-ayis along the line between A and B with origin halfway
 * between but z = 0 is the height of the launch platform.
 * the ray from A is:
 * x = -(dist/2) + cos(alpha)*cos(gamma)*ta = xa + dxa*ta
 * y = cos(alpha)*sin(gamma)*ta = ya + dya*ta
 * z = ha + sin(alpha)*ta = za + dza*ta
 * the ray from B is:
 * x = (dist/2) + cos(beta)*cos(delta)*tb = xb + dxb*tb
 * y = cos(beta)*sin(delta)*tb = yb + dyb*tb
 * z = hb + sin(beta)*tb = zb + dzb*tb
 *
 * the vector between the two closest points on ray A and ray B
 * must be perpendicular to each of ray A and ray B
 * (Consider the pair of parallel planes containing rayA and ray B
 * [note that ray A and ray B cannot be parallel by limits on gamma and delta]
 *	respectively. The normal vector to each of these planes is perpendicular
 *	to each line. Consider ptA om ray A and ptB on ray B. If the vector
 *	between ptA and ptB is not perpendicular to the planes, by sliding
 *	ptA along rayA or ptB along rayB, the distance between the points may
 *	be reduced.)
 *
 *	the vector from ptA to ptB is
 *	dx = (xb - xa) + (dxb*tb - dxa*ta)
 *	dy = (yb - ya) + (dyb*tb - dya*ta)
 *	dz = (zb - za) + (dzb*tb - dza*ta)
 *
 * to be perpendicular to ray A dx*dxa + dy*dya + dz*dza = 0
 * (xb - xa)*dxa + (yb - ya)*dya + (zb - za)*dza =
 *	dist*dxa + 0 * dya + (hb - ha)*dza = 
 *		ta*(dxa*dxa + dya*dya + dza*dza) - tb*(dxb*dxa + dyb*dya + dzb*dza)
 * to be perpendicular to ray B dx*dxb + dy*dyb + dz*dzb = 0
 * (xb - xa)*dxb + (yb - ya)*dyb + (zb - za)*dzb =
 *	dist*dxb + 0 * dyb + (hb - ha)*dzb = 
 *		ta*(dxa*dxb + dya*dyb + dza*dzb) - tb*(dxb*dxb + dyb*dyb + dzb*dzb)
 *
 *	R*ta - S*tb = U
 *	S*ta - T*tb = V
 *	ta = (-U*T + V*S)/(-R*T +S*S);
 *	tb = (V*R - U*S)/(-R*T +S*S);
 *
 *	z_result = (a + dza*ta + zb + dzb*tb)/2
 */

char inbuf[256];
#define EPS	.01
int
main(int argc, char **argv)
{
	int i, n, probnum;
	double alpha, beta, gamma, delta, deg2rad;
	double xa, xb, ya, yb, za, zb, dxa, dxb, dya, dyb, dza, dzb;
	double R, S, T, U, V, ta, tb, z_res, dist, denom;

	// get global parameters
	if(fgets(&(inbuf[0]), 255, stdin) == NULL)
	{
		fprintf(stderr, "read failed on global parameters\n");
		return -1;
	}
	// HA and HB
	if(sscanf(&(inbuf[0]), "%d %lf %lf", &n, &za, &zb) != 3)
	{
		fprintf(stderr, "scan failed on global parameters\n");
		return -2;
	}
	deg2rad = atan2(1.0,1.0)/45.0;
	dist = 100.0;
	xb = dist * 0.5;
	xa = -xb;
	yb = ya = 0.0;
	probnum = 0;
	for(i = 1; i <= n; i++)
	{
		probnum++;
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "read failed on problem %d\n", probnum);
			return -3;
		}
		if(sscanf(&(inbuf[0]), "%lf %lf %lf %lf", &alpha, &beta, &gamma, &delta) != 4)
		{
			fprintf(stderr, "scan failed on problem %d\n", probnum);
			return -4;
		}
		// alternate way out from original problem spec.
		if((alpha <= 0.0) || (beta <= 0.0) || (gamma <= 0.0) || (delta <= 0.0))
		{
			return 0;
		}
		dxa = cos(deg2rad*alpha)*cos(deg2rad*gamma);
		dya	= cos(deg2rad*alpha)*sin(deg2rad*gamma);
		dza = sin(deg2rad*alpha);
		dxb = cos(deg2rad*beta)*cos(deg2rad*delta);
		dyb	= cos(deg2rad*beta)*sin(deg2rad*delta);
		dzb = sin(deg2rad*beta);
		U = dist*dxa + (zb - za)*dza;
		V = dist*dxb + (zb - za)*dzb;
		R = dxa*dxa + dya*dya + dza*dza; 
		S = dxa*dxb + dya*dyb + dza*dzb; 
		T = dxb*dxb + dyb*dyb + dzb*dzb;
		denom = -R*T + S*S;
		if(fabs(denom) < EPS)
		{
			fprintf(stderr, "denom %.4f too small on problem %d\n", denom, probnum);
			return -5;
		}
		ta = (-U*T + V*S)/denom;
		tb = (V*R - U*S)/denom;
		z_res = (za + dza*ta + zb + dzb*tb)/2.0;
		printf("%d: %.0f\n", i, z_res);
	}
	return 0;
}
