/*
 * X - Maximum in the Cycle of 1 - Solution
 * Greater NY Regional ACM Contest 2011
 * Solution by Fred Pickel
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define EPS .001

double	m_dA[2];
double	m_dB[2];
double	m_dC[2];
double	m_dD[2];
double	m_dAp[2];
double	m_dBp[2];
double	m_dCp[2];
double	m_dDp[2];
double	m_dApp[2];
double	m_dBpp[2];
double	m_dCpp[2];
double	m_dDpp[2];

double m_AArea, m_BArea, m_CArea, m_DArea, m_EArea;

/*
 * Calculates 1/2 cross product of A->B with A->C
 */
double TriArea(double *pA, double *pB, double *pC)
{
	double dx1, dx2, dy1, dy2, ret;
	dx1 = pB[0] - pA[0];
	dy1 = pB[1] - pA[1];
	dx2 = pC[0] - pA[0];
	dy2 = pC[1] - pA[1];
	ret = 0.5 *fabs(dx1 * dy2 - dx2 * dy1);
	return ret;
}

/* find intersection of line from pA to pB with line from pC to pD
 * return 0 if intersection is interioe to both line segments, else return -1
 * res_X = A_X*s + B_X * (1-s) = C_X * t + D_X * (1 - t) 0 <= s,t <= 1
 * res_Y = A_Y*s + B_Y * (1-s) = C_Y * t + D_Y * (1 - t) 0 <= s,t <= 1
 * (A_X - B_X) * s + (D_X - C_X) * t = D_X - B_X
 * (A_Y - B_Y) * s + (D_Y - C_Y) * t = D_Y - B_Y
 */
int FindInter(double *pA, double *pB, double *pC, double *pD, double *pRes)
{
	double a, b, c, d, s, t, rx, ry, den;

	a = pA[0] - pB[0];
	b = pD[0] - pC[0];
	rx = pD[0] - pB[0];
	c = pA[1] - pB[1];
	d = pD[1] - pC[1];
	ry = pD[1] - pB[1];
	den = a*d - b*c;

	/* Sanity check */
	if(fabs(den) < EPS) return -1;

	s = (d * rx - b * ry)/den;
	t = (a * ry - c * rx)/den;
	/* More sanity checking */
	if((s < 0.0) || (s > 1.0) || (t < 0.0) || (t > 1.0)) {
		return -1;
	}
	pRes[0] = pA[0] * s + pB[0] * (1.0 - s);
	pRes[1] = pA[1] * s + pB[1] * (1.0 - s);
	return 0;
}

/*
 * Find midpoints of opposite sides (d?p) and where lines intersect (d?pp)
 */
int FindQuadInters()
{
	int ret;

	m_dAp[0] = m_dB[0] * 0.5 + m_dC[0] * 0.5;
	m_dAp[1] = m_dB[1] * 0.5 + m_dC[1] * 0.5;
	m_dBp[0] = m_dC[0] * 0.5 + m_dD[0] * 0.5;
	m_dBp[1] = m_dC[1] * 0.5 + m_dD[1] * 0.5;
	m_dCp[0] = m_dD[0] * 0.5 + m_dA[0] * 0.5;
	m_dCp[1] = m_dD[1] * 0.5 + m_dA[1] * 0.5;
	m_dDp[0] = m_dA[0] * 0.5 + m_dB[0] * 0.5;
	m_dDp[1] = m_dA[1] * 0.5 + m_dB[1] * 0.5;
	if((ret = FindInter(m_dA, m_dAp, m_dB, m_dBp, m_dApp)) != 0) return ret;
	if((ret = FindInter(m_dB, m_dBp, m_dC, m_dCp, m_dBpp)) != 0) return ret;
	if((ret = FindInter(m_dC, m_dCp, m_dD, m_dDp, m_dCpp)) != 0) return ret;
	if((ret = FindInter(m_dD, m_dDp, m_dA, m_dAp, m_dDpp)) != 0) return ret;
	return 0;
}

int FindAreas(double bx, double cx, double cy, double  dx, double dy, double *pAreas)
{
	double len;
	int ret = 0;

	m_dA[0] = m_dA[1] = m_dB[1] = 0.0;
	m_dB[0] = bx;
	m_dC[0] = cx;
	m_dC[1] = cy;
	m_dD[0] = dx;
	m_dD[1] = dy;
	if(FindQuadInters() != 0) {
		return -1;
	}
	pAreas[0] = TriArea(m_dA, m_dB, m_dApp)/160.0;
	pAreas[1] = TriArea(m_dB, m_dC, m_dBpp)/160.0;
	pAreas[2] = TriArea(m_dC, m_dD, m_dCpp)/160.0;
	pAreas[3] = TriArea(m_dD, m_dA, m_dDpp)/160.0;
	pAreas[4] = (TriArea(m_dApp, m_dBpp, m_dCpp) + TriArea(m_dApp, m_dCpp, m_dDpp))/160.0;
	len = sqrt((m_dApp[0] - m_dBpp[0])*(m_dApp[0] - m_dBpp[0]) + (m_dApp[1] - m_dBpp[1])*(m_dApp[1] - m_dBpp[1]));
	len += sqrt((m_dBpp[0] - m_dCpp[0])*(m_dBpp[0] - m_dCpp[0]) + (m_dBpp[1] - m_dCpp[1])*(m_dBpp[1] - m_dCpp[1]));
	len += sqrt((m_dCpp[0] - m_dDpp[0])*(m_dCpp[0] - m_dDpp[0]) + (m_dCpp[1] - m_dDpp[1])*(m_dCpp[1] - m_dDpp[1]));
	len += sqrt((m_dApp[0] - m_dDpp[0])*(m_dApp[0] - m_dDpp[0]) + (m_dApp[1] - m_dDpp[1])*(m_dApp[1] - m_dDpp[1]));
	len *= 16.5;
	/* round up  */
	len = ceil(len);
	ret = (int)len;
	return ret;
}

int main()
{
	int nprob, curprob, index, fencelen;
	double bx, cx, cy, dx, dy;
	double areas[5];
	char inbuf[256];

	if(fgets(&(inbuf[0]), sizeof (inbuf)-1, stdin) == NULL)
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
		if(fgets(&(inbuf[0]), sizeof (inbuf)-1, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d data\n", curprob);
			return -3;
		}
		// get prob num and non-zero coordinates
		if(sscanf(&(inbuf[0]), "%d %lf %lf %lf %lf %lf", &index, &bx, &cx, &cy, &dx, &dy) != 6)
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
		fencelen = FindAreas(bx, cx, cy, dx, dy, &(areas[0]));
		printf("%d %.3f %.3f %.3f %.3f %.3f %d\n", curprob,
			areas[0], areas[1], areas[2], areas[3], areas[4], fencelen);
	}
	return 0;
}
