#include <stdio.h>
#include <math.h>

/*
 * take the x-ayis along the line between A and B with origin halfway
 * between but z = 0 is the height of the launch platform.
 * (xa, ya, za) = (-dist/2, 0, ha) is the location of theodolite A
 * (xb, yb, zb) = (+dist/2, 0, hb) is the location of theodolite B
 * the sight line from A is
 * x = -(dist/2) + cos(alpha)*cos(gamma)*ta = xa + dxa*ta
 * y = cos(alpha)*sin(gamma)*ta = ya + dya*ta
 * z = ha + sin(alpha)*ta = za + dza*ta
 * the sight line form b is
 * x = (dist/2) + cos(beta)*cos(delta)*tb = xb + dxb*tb
 * y = cos(beta)*sin(delta)*tb = yb + dyb*tb
 * z = hb + sin(beta)*tb = zb + dzb*tb
 *
 * the vertical plane containing sightline A is
 * x = xa + dxa*ta, y = ya + dya*ta, z = anything OR (x - xa)/dxa = (y - ya)/dya
 * or x*dya - y*dxa = xa*dya - ya*dxa = R
 * similarly the vertical plane containing sightline B is
 * x*dyb - y*dxb = xa*dyb - ya*dxb = S
 * solving (cramer's rule):
 * x = (S*dxa - R*dxb)/(dyb*dxa - dya*dxb)
 * y = (S*dya - R*dyb)/(dyb*dxa - dya*dxb)
 * z = anything is the vertical line
 * plug this into the sight line equations
 * if (fabs(dxa) > fabs(dya)) ta = (x - xa)/dxa else ta = (y - ya)/dya
 * if (fabs(dxb) > fabs(dyb)) tb = (x - xb)/dxb else tb = (y - yb)/dyb
 * rza = za + ta*dza
 * rzb = zb + tb*dzb
 * if(fabs(rza - rzb) > errdist) error
 * else z_res = (rza + rzb)/2.0
 */
#define EPS	.01

char inbuf[512];

int main()
{
	int nProbs, probNum, z_out;
	double alpha, beta, gamma, delta, deg2rad;
	double xa, xb, ya, yb, za, zb, dxa, dxb, dya, dyb, dza, dzb;
	double R, S, x, y, rza, rzb, ta, tb, z_res, dist, denom, offset, errdist;

	if(fgets(&(inbuf[0]), 511, stdin) == NULL)
	{
		fprintf(stderr, "Read failed on problem count\n");
		return -1;
	}
	if(sscanf(&(inbuf[0]), "%d", &nProbs) != 1)
	{
		fprintf(stderr, "Scan failed on problem count\n");
		return -2;
	}
	if((nProbs < 1) || (nProbs > 1000))
	{
		fprintf(stderr, "problem count %d not in range 1 to 1000\n", nProbs);
		return -3;
	}
	// get global parameters
	if(fgets(&(inbuf[0]), 255, stdin) == NULL)
	{
		fprintf(stderr, "read failed on global parameters\n");
		return -4;
	}
	if(sscanf(&(inbuf[0]), "%lf %lf %lf %lf %lf", &dist, &offset, &za, &zb, &errdist) != 5)
	{
		fprintf(stderr, "scan failed on global parameters\n");
		return -5;
	}
	deg2rad = atan2(1.0,1.0)/45.0;
	xb = dist * 0.5;
	xa = -xb;
	yb = ya = 0.0;

	for(probNum = 1; probNum <= nProbs ; probNum++)
	{
		if(fgets(&(inbuf[0]), 511, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem dims problem %d\n", probNum);
			return -6;
		}
		if(sscanf(&(inbuf[0]), "%lf %lf %lf %lf", &alpha, &beta, &gamma, &delta) != 4)
		{
			fprintf(stderr, "scan failed on problem %d\n", probNum);
			return -7;
		}
		if((alpha <= 0.0) || (alpha >= 90.0) ||
			(beta <= 0.0) || (beta >= 90.0) ||
			(gamma <= 0.0) || (gamma >= 90.0) ||
			(delta <= 90.0) || (delta >= 180.0))
		{
			printf("%d DISQUALIFIED\n", probNum);
			continue;
		}
		dxa = cos(deg2rad*alpha)*cos(deg2rad*gamma);
		dya	= cos(deg2rad*alpha)*sin(deg2rad*gamma);
		dza = sin(deg2rad*alpha);
		dxb = cos(deg2rad*beta)*cos(deg2rad*delta);
		dyb	= cos(deg2rad*beta)*sin(deg2rad*delta);
		dzb = sin(deg2rad*beta);

		R = xa*dya - ya*dxa;
		S = xb*dyb - yb*dxb;
		denom = dyb*dxa - dya*dxb;
		if(fabs(denom) < EPS)
		{
			printf("%d DIV BY ZERO\n", probNum);
			continue;
		}
		x = (S*dxa - R*dxb)/denom;
		y = (S*dya - R*dyb)/denom;
		if(fabs(dxa) >= fabs(dya))
		{
			ta = (x - xa)/dxa;
		}
		else
		{
			ta = (y - ya)/dya;
		}
		if(fabs(dxb) >= fabs(dyb))
		{
			tb = (x - xb)/dxb;
		}
		else
		{
			tb = (y - yb)/dyb;
		}
		rza = za + ta*dza;
		rzb = zb + tb*dzb;
		if(fabs(rza - rzb) > errdist)
		{
			printf("%d ERROR\n", probNum);
		}
		else
		{
			z_res = (rza + rzb)/2.0;
			z_out = (int)(z_res + 0.5);
			printf("%d %d\n", probNum, z_out);
		}
	}
	return 0;
}
