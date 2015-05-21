// Exocenter.cpp : Defines the entry point for the console application.
//

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class CDPPoint  
{
public:
	double dot(CDPPoint b);
	void add(CDPPoint b);
	void sub(CDPPoint b);
	double m_x;
	double m_y;
	CDPPoint();
	virtual ~CDPPoint();

};


#define EPS .0001

// get other corners of square on side from first to second opposide vertex other
// adj1 is corner adjacent to first, adj2 is corner adjacent to second
int
get_square(CDPPoint first, CDPPoint second, CDPPoint other, CDPPoint &adj1, CDPPoint &adj2)
{
	CDPPoint	vedge, vperp, vother;

	vedge = second;
	vedge.sub(first);	// vedge is vector from first to second
	vother = other;
	vother.sub(first);	// vother isvector from first to other
	vperp.m_x = vedge.m_y;
	vperp.m_y = -vedge.m_x;	// vperp is same lenght as vedge and perpendicular to it
	if(vperp.dot(vother) > 0.0)
	{	// if vperp is on the same side as other point, reverse it
		vperp.m_x = -vperp.m_x;
		vperp.m_y = -vperp.m_y;
	}
	adj1 = first;
	adj1.add(vperp);	// vperp is vector alongside of square, add to given vertices
	adj2 = second;
	adj2.add(vperp);	// to get the adjacent (non-triangle) vertices of the square

	return 0;
}

// solve 3 eqns in 2 unknowns, 
// return -1 if equations arenot consistent
int
reduce(double mat[3][3])
{
	double m0, m1, m2;
	int i, j, k;

	// find largest x coeff in abs value
	m0 = 0.0;
	j = -1;
	for(i = 0; i < 3; i++)
	{
		if(fabs(mat[i][0]) > m0)
		{
			m0 = fabs(mat[i][0]);
			j = i;
		}
	}
	if(m0 < EPS)
	{	// not a triangle
		return -1;
	}
	// swap largest coeef to top
	if(j != 0)
	{
		for(k = 0; k < 3 ; k++)
		{
			m0 = mat[0][k];
			mat[0][k] = mat[j][k];
			mat[j][k] = m0;
		}
	}
	// subtract multiples of row 0 form others to eliminate coeff 0
	m1 = mat[1][0]/mat[0][0];
	for(k = 0; k < 3; k++)
	{
		mat[1][k] = mat[1][k] - m1 * mat[0][k];
	}
	m2 = mat[2][0]/mat[0][0];
	for(k = 0; k < 3; k++)
	{
		mat[2][k] = mat[2][k] - m2 * mat[0][k];
	}
	// find larger of two coeffs
	m1 = fabs(mat[1][1]);
	if(fabs(mat[2][1]) > m1)
	{
		m1 = fabs(mat[2][1]);
		// swap rwo 1 and 2
		for(k = 0; k < 3 ; k++)
		{
			m0 = mat[1][k];
			mat[1][k] = mat[2][k];
			mat[2][k] = m0;
		}
	}
	if(m1 < EPS)
	{	// not a triangle
		return -1;
	}
	// eliminate var 2 from last row and first row
	m2 = mat[2][1]/mat[1][1];
	for(k = 0; k < 3; k++)
	{
		mat[2][k] = mat[2][k] - m2 * mat[1][k];
	}
	m0 = mat[0][1]/mat[1][1];
	for(k = 0; k < 3; k++)
	{
		mat[0][k] = mat[0][k] - m0 * mat[1][k];
	}
	
	return 0;
}
char inbuf[256];

int
main()
{
	CDPPoint	a, b, c;	// input triangle vertices
	CDPPoint	sab, sba, sbc, scb, sca, sac;	// adjacent square vertices sxy is vertex adj to x 
												// in square on side xy
	CDPPoint	ma, mb, mc;		// mid points of side opposite a, b, c resp. in exotriangle
	char *cp;
	double inx, iny;
	double m[3][3];
	int nDataSet;
    FILE *fp = fopen("i.in", "rt");
    if(fp == NULL){
        printf("Can't open input file i.in\n");
        return 1;
    }

	// get the  dataset count
	cp = fgets(&(inbuf[0]), sizeof(inbuf), fp);
	if(cp == NULL)
	{
		printf("missing input line 1\n");
		return -1;
	}
	nDataSet = ::atoi(cp);
	while(nDataSet-- > 0){
		// get the  three vertices
        cp = fgets(&(inbuf[0]), sizeof(inbuf), fp);
		if(cp == NULL)
		{
			printf("missing input line 1\n");
			return -1;
		}
		if(sscanf(&(inbuf[0]), "%lf %lf", &inx, &iny) != 2)
		{
			printf("bad format in line 1\n");
			return -1;
		}
		a.m_x = inx;
		a.m_y = iny;

		cp = fgets(&(inbuf[0]), sizeof(inbuf), fp);
		if(cp == NULL)
		{
			printf("missing input line 2\n");
			return -1;
		}
		if(sscanf(&(inbuf[0]), "%lf %lf", &inx, &iny) != 2)
		{
			printf("bad format in line 2\n");
			return -1;
		}
		b.m_x = inx;
		b.m_y = iny;

		cp = fgets(&(inbuf[0]), sizeof(inbuf), fp);
		if(cp == NULL)
		{
			printf("missing input line 3\n");
			return -1;
		}
		if(sscanf(&(inbuf[0]), "%lf %lf", &inx, &iny) != 2)
		{
			printf("bad format in line 3\n");
			return -1;
		}
		c.m_x = inx;
		c.m_y = iny;

		// get corresponding square vertices
		get_square(a, b, c, sab, sba);
		get_square(b, c, a, sbc, scb);
		get_square(c, a, b, sca, sac);

		// mid points are avg of square corners
		ma.m_x = 0.5 *(sab.m_x + sac.m_x);
		ma.m_y = 0.5 *(sab.m_y + sac.m_y);

		mb.m_x = 0.5 *(sba.m_x + sbc.m_x);
		mb.m_y = 0.5 *(sba.m_y + sbc.m_y);

		mc.m_x = 0.5 *(sca.m_x + scb.m_x);
		mc.m_y = 0.5 *(sca.m_y + scb.m_y);

		// now we get equations of exomedian lines in the form A*x + B*y = C
		// <A,B> is a vector perpendicular to the line
		// x coord is change in y, y coord is negative change in x
		m[0][0] = a.m_y - ma.m_y;
		m[0][1] = ma.m_x - a.m_x;

		m[1][0] = b.m_y - mb.m_y;
		m[1][1] = mb.m_x - b.m_x;

		m[2][0] = c.m_y - mc.m_y;
		m[2][1] = mc.m_x - c.m_x;

		// constant term obtained by plugging vertex into the equation
		m[0][2] = m[0][0] * a.m_x + m[0][1] * a.m_y;
		m[1][2] = m[1][0] * b.m_x + m[1][1] * b.m_y;
		m[2][2] = m[2][0] * c.m_x + m[2][1] * c.m_y;

		// now solve 3 equations in two unknowns for the common intersection point
		if(reduce(m) != 0)
		{
			printf("not a triangle\n");
			return -2;
		}
		if(fabs(m[2][2]) > EPS)
		{
			printf("inconsistent\n");
			return -3;
		}
		printf("%.4f %.4f\n", m[0][2]/m[0][0], m[1][2]/m[1][1]);
	}

	return 0;
}