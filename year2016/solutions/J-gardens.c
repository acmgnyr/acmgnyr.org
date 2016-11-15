/*
 * J - Smoothed Gardens
 * Solution by Fred Pickel
 * ACM ICPC GNYR 2016
 */
#include <stdio.h>
#include <ctype.h>
#include <math.h>

double math_pi = 3.141592653589793238462643383279;
double radii[1025];
double switchx[6], switchy[6], switchdist[6], switchang[6];

// find length of line from pt1 to pt3 to pt2
double Get2PtLen(double x1, double y1, double x2, double y2, double x3, double y3)
{
	double l1, l2;
	l1 = sqrt((x3-x1)*(x3-x1) + (y3-y1)*(y3-y1));
	l2 = sqrt((x3-x2)*(x3-x2) + (y3-y2)*(y3-y2));
	return (l1 + l2);
}

// find radius r so if x = r*cos(ang) y = r*sin(ang) Get2PtLen(x1, y1, x2, y2, x, y) = desired_lem
// init_guess is supposed to be close to correct value
// since len will be an increasing function of r 
// find r1 <= soln <= r2 then smart bisect to get soln
// return < 0.0 indicates error
double FindLenDist(double x1, double y1, double x2, double y2, double ang,
				   double desired_len, double init_guess)
{
	double c, s, r1, r2, r, dr, len1, len2, len, rcur, rprev, lencur, lenprev;
	int retry_cnt;
	c = cos(ang);
	s = sin(ang); 
	// first get r1 <= soln <= r2 
	r1 = init_guess;
	len1 = Get2PtLen(x1, y1, x2, y2, c*r1, s*r1);
	retry_cnt = 0;
	if(fabs(len1 - desired_len) < .00001) {
		return r1;
	}
	if(len1 < desired_len) {
		// r1 is lower bound search for upper bound
		// if you luck out, return
		dr = 0.1;
		r2 = r1+dr;
		len2 = Get2PtLen(x1, y1, x2, y2, c*r2, s*r2);
		if(fabs(len2 - desired_len) < .00001) {
			return r2;
		}
		while((retry_cnt < 20) && (len2 < desired_len)) {
			len1 = len2;
			dr *= 2.0;
			r1 = r2;
			r2 = r1+dr;
			len2 = Get2PtLen(x1, y1, x2, y2, c*r2, s*r2);
			if(fabs(len2 - desired_len) < .00001) {
				return r2;
			}
		}
		if(retry_cnt >= 20) {
			return -1.0;
		}
	} else {
		// r1 is upper bound, set r2 = r1 and serch for lower bound
		// bound cannot be < 0
		// if you luck out, return
		len2 = len1;
		r2 = r1;
		dr = -0.1;
		r1 = r2 + dr;
		if(r1 < 0.0) r1 = 0.0;
		len1 = Get2PtLen(x1, y1, x2, y2, c*r1, s*r1);
		if(fabs(len1 - desired_len) < .00001) {
			return r1;
		}
		while((retry_cnt < 20) && (len1 > desired_len)) {
			retry_cnt++;
			len2 = len1;
			dr *= 2.0;	// double step size each time through
			r2 = r1;
			r1 = r2+dr;
			if(r1 < 0.0) r1 = 0.0;
			len1 = Get2PtLen(x1, y1, x2, y2, c*r1, s*r1);
			if(fabs(len1 - desired_len) < .00001) {
				return r1;
			}
		}
		if(retry_cnt >= 20) {
			return -1.0;
		}
	}
	// shrink the interval to where pseudo newqton should work
	// if you luck out, return
	while((r2 - r1) > .05)
	{
		r = 0.5*(r1 + r2);
		len = Get2PtLen(x1, y1, x2, y2, c*r, s*r);
		if(fabs(len - desired_len) < .00001) {
			return r;
		}
		if(len < desired_len) {
			r1 = r;
			len1 = len;
		} else {
			r2 = r;
			len2 = len;
		}
	}
	// bounded secant method limits are r1 <= soln < = r2 len1 <= desired_len <= len2
	// rprev and lenprev are for prev quess, rcur and lencur for current guess
	// d(len)/dr ~ (lenprev - lencur)/(rprev - rcur)
	// nextr = rcur + (desiredlen - lencur)/(d(len)/dr) -
	retry_cnt = 0;
	rcur = r1; lencur = len1; rprev = r2; lenprev = len2;
	while((retry_cnt < 20) && ((r2 - r1) > .00001)){
		retry_cnt++;
		r = rcur + (desired_len - lencur)*(rprev-rcur)/(lenprev - lencur);
		// if guess is outside the interval, use midpoint
		if((r <= r1) || (r >= r2)) r = 0.5*(r1+r2);
		len = Get2PtLen(x1, y1, x2, y2, c*r, s*r);
		if(fabs(len - desired_len) < .00001) {
			return r;
		}
		if(len < desired_len) {
			if(len > len1) {
				r1 = r;
				len1 = len;
				rprev = rcur;
				lenprev = lencur;
				rcur = r;
				lencur = len;
			} else {	// this should not happen since r > r1
				rprev = rcur;
				lenprev = lencur;
				rcur = r1;
				lencur = len1;
			}
		} else {
			if(len < len2) {
				r2 = r;
				len2 = len;
				rprev = rcur;
				lenprev = lencur;
				rcur = r;
				lencur = len;
			} else {	// this should not happen since r < r2
				rprev = rcur;
				lenprev = lencur;
				rcur = r2;
				lencur = len2;
			}
		}
	}
	if(retry_cnt >= 20) {
		return -2.0;
	}
	return 0.5*(r1+r2);
}

// find area of a sector (center at 0,0) of an ellipse with
// foci at (x1,y1) and (x2,y2) and string length len (*pGuess is r at ang1 on return r at ang2)
// area = INT(ang1 to ang2: 0.5*r*r*dang)
// approx with simpsons rule 
// area = SUM(r[ang-dang]*r[ang-dang] + 4.0*r[ang]*r[ang] + r[ang+dang]*r[ang*dang]) *(dang/3.0)
// we compute with 32 points and 64 points if the areas are close enough, return the 64 pt value
// else comput 128 and compare 64 and 128 etc up to 1024 pts (32 is usually enough
double findSectorArea(double x1, double y1, double x2, double y2,
					  double sectlen, double ang1, double ang2, double *pGuess)
{
	double ang, dang, area1, area2, r, rsq, err;
	int i, step;
	step = 16;
	r = *pGuess;
	dang = (ang2 - ang1)/64.0;
	ang = ang1;
	// compute values for 64 pts (and 32) into 1025 pt array
	for(i = 0; i < 65; i++, ang += dang) {
		radii[step*i] = FindLenDist(x1, y1, x2, y2, ang, sectlen, r);
		r = radii[step*i];
		if(r < 0.0) return -1.0;
	}
	// compute weighted sums of r*r (weights 1,4,2, 4,2, ...,2,4,1)
	*pGuess = r;
	area1 = area2 = radii[0]*radii[0] + radii[1024]*radii[1024];
	area2 += 4.0*radii[step]*radii[step];
	for(i = 1; i < 32 ; i++) {
		r = radii[2*i*step];
		rsq = r*r;
		if(i & 1) {
			area1 += 4.0*rsq;
		} else {
			area1 += 2.0*rsq;
		}
		area2 += 2.0*rsq;
		r = radii[2*i*step+step];
		area2 += 4.0*r*r;
	}
	area1 *= dang/3.0;
	area2 *= dang/6.0;	// dang is half as large
	if((err = fabs(area2 - area1)) < .0005) {	// if close enough return
		return area2;
	}
	// else compute area with double the number of points untill you get close (or 1024)
	while((step >= 2) && (fabs(area1 - area1) < .0005)) {
		ang = ang1 + 0.5*dang;
		area1 = area2;
		area2 = radii[0]*radii[0] + radii[1024]*radii[1024];
		for(i = 0; i < 1024 ; i += step, ang += dang) {
			radii[i+step/2] = FindLenDist(x1, y1, x2, y2, ang, sectlen, radii[i]);
			if(radii[i+step/2] < 0.0) return -1.0;
		}
		step = step/2;
		dang = 0.5*dang;
		area2 += 4.0*radii[step]*radii[step];
		for(i = 2*step; i < 1024 ; i += 2*step) {
			r = radii[i];
			area2 += 2.0*r*r;
			r = radii[i+step];
			area2 += 4.0*r*r;
		}
		area2 *= dang/6.0;
		r = fabs(area2 - area1);
		// areas close or the eror is worse give up
		if((r < .0005) || (r > err)) {
			return area2;
		}
		err = r;
	}
	return area2;
}

// find area of smooth bit as sum of 6 sector areas (with center at A = (0,0)
// from angle 0 to angle of AC foci B, C sectlen = len - ablen - aclen
// from angAC to ang2 (where ellips hits extension of BC foci A, B sectlen = len - ablen
// from ang2 to pi foci A,C sectlen = len - ablen - bclen
// from pi to angAC + pi foci B,C sectlen = len - bclen
// from angAC+pi to ang5 (where ellipse hits other extensiion fo BC foci A, B sectlen = len - bclen - aclen
// from agn5 to 2*pi foci A,C sectlen = len - aclen
double findArea(double bx, double cx, double cy, double len)
{
	double ablen, bclen, aclen, area, darea;
	double acang, bcang, ang2, ang5, r, x, y, extra, startr, curr;
	ablen = bx;
	bclen = sqrt((cx-bx)*(cx-bx) + cy*cy);
	aclen = sqrt(cx*cx + cy*cy);
	if((extra = (len - (ablen + bclen + aclen))) <= 0.0) {
		return -10.0;
	}
	acang = atan2(cy, cx);
	bcang = atan2(cy, cx-bx);
	// to find ang2 shift origin to B find point on extn of BC that hits curve
	r = FindLenDist(0.0, 0.0, -bx, 0.0, bcang, len - ablen, 0.7*extra + bclen);
	if(r < 0.0) return -1.0;
	x = bx + r*cos(bcang); y = r*sin(bcang);
	ang2 = atan2(y, x);
	curr = startr = sqrt(x*x + y*y);
	// to find ang5 shift origin to B find point on extn of BC that hits curve other dir
	r = FindLenDist(0.0, 0.0, -bx, 0.0, bcang + math_pi, len - aclen - bclen, 0.7*extra);
	if(r < 0.0) return -1.0;
	x = bx - r*cos(bcang); y = -r*sin(bcang);
	ang5 = atan2(y, x);
	if(ang5 < 0.0) {
		ang5 += 2.0*math_pi;
	} else if(ang5 < math_pi) {
		ang5 += math_pi;
	}

	// start with ang2 since we "know the correct r
	area = 0.0;
	darea = findSectorArea(0.0, 0.0, cx, cy, len - ablen - bclen, ang2, math_pi, &curr);
	if(darea < 0.0) return -1.0;
	area += darea;
	darea = findSectorArea(bx, 0.0, cx, cy, len - bclen, math_pi, math_pi + acang, &curr);
	if(darea < 0.0) return -1.0;
	area += darea;
	darea = findSectorArea(0.0, 0.0, bx, 0.0, len - aclen - bclen, math_pi + acang, ang5, &curr);
	if(darea < 0.0) return -1.0;
	area += darea;
	darea = findSectorArea(0.0, 0.0, cx, cy, len - aclen, ang5, 2*math_pi, &curr);
	if(darea < 0.0) return -1.0;
	area += darea;
	darea = findSectorArea(bx, 0.0, cx, cy, len - ablen - aclen, 0.0, acang, &curr);
	if(darea < 0.0) return -1.0;
	area += darea;
	darea = findSectorArea(0.0, 0.0, bx, 0.0, len - ablen, acang, ang2, &curr);
	if(darea < 0.0) return -1.0;
	area += darea;
	return area;
}

// find area of smooth bit as sum of 6 sector areas (with center at median of triangle
// (since this gives most nearly equal steps along the curve for equal angles
// the 6 sector areas are cut off by where the extensions of the sides othe tringle hit the curve
// 0 where pos x axis hits curve right of B
// 1 where AC above C hits the curve
// 2 where BC above C hits the curve
// 3 where neg x axis hits curve left of A
// 4 where AC below A hits the curve
// 5 where BC below B hits the curve
// from angle 0 to angle 1 foci B, C sectlen = len - ablen - aclen (rope tight against AB and AC away from BC)
// from ang 1 to ang2  foci A, B sectlen = len - ablen (rope tight against AB, away from BC and AC
// from ang2 to ang 3 foci A,C sectlen = len - ablen - bclen (rope tight against AB and BC away from AC)
// from ang3 to ang4 foci B,C sectlen = len - bclen (rope tight against BC, away from AB and AC
// from ang4 to ang5 foci A, B sectlen = len - bclen - aclen (rope tight against AC and BC away from AB)
// from agn5 to ang0 foci A,C sectlen = len - aclen (rope tight against AC, away from AB and BC
double findArea2(double bx, double cx, double cy, double len)
{
	double ablen, bclen, aclen, area, darea;
	double cenx, ceny;
	double acang, bcang, r, x, y, extra, curr;
	cenx = (bx + cx)/3.0;
	ceny = cy/3.0;

	ablen = bx;
	bclen = sqrt((cx-bx)*(cx-bx) + cy*cy);
	aclen = sqrt(cx*cx + cy*cy);
	if((extra = (len - (ablen + bclen + aclen))) <= 0.0) {
		return -10.0;
	}
	acang = atan2(cy, cx);
	bcang = atan2(cy, cx-bx);
	//find pts where extensions of each side hit the outer curve
	// in each case we get the point (x,y) where the curve hits the extended side
	// then find the angle from the centroid (cenx, ceny) and the distance from the centroid
	// ang0 is along pos x-axis based use A as origin B,C as foci 
	r = FindLenDist(bx, 0.0, cx, cy, 0.0, len - ablen -aclen, 0.7*extra + ablen);
	if(r < 0.0) return -1.0;
	switchx[0] = x = r; switchy[0] = y = 0.0;
	switchang[0] = atan2(y-ceny, x-cenx);
	if(switchang[0] < 0.0) {
		switchang[0] += 2.0*math_pi;
	}
	switchdist[0] = sqrt((x-cenx)*(x-cenx) + (y-ceny)*(y-ceny));
	// ang1 is along ACtowward C based at A
	r = FindLenDist(bx, 0.0, cx, cy, acang, len - ablen -aclen, 0.7*extra + aclen);
	if(r < 0.0) return -1.0;
	switchx[1] = x = r*cos(acang); switchy[1] = y = r*sin(acang);
	switchang[1] = atan2(y-ceny, x-cenx);
	if(switchang[1] < 0.0) {
		switchang[1] += 2.0*math_pi;
	}
	switchdist[1] = sqrt((x-cenx)*(x-cenx) + (y-ceny)*(y-ceny));
	// to find ang2 shift origin to B find point on extn of BC that hits curve
	r = FindLenDist(0.0, 0.0, -bx, 0.0, bcang, len - ablen, 0.7*extra + bclen);
	if(r < 0.0) return -1.0;
	switchx[2] = x = bx + r*cos(bcang); switchy[2] = y = r*sin(bcang);
	switchang[2] = atan2(y-ceny, x-cenx);
	if(switchang[2] < 0.0) {
		switchang[2] += 2.0*math_pi;
	}
	switchdist[2] = sqrt((x-cenx)*(x-cenx) + (y-ceny)*(y-ceny));
	// ang4 is along pos x-axis based at A negaticve dir
	r = FindLenDist(0.0, 0.0, cx, cy, math_pi, len - ablen -bclen, 0.7*extra);
	if(r < 0.0) return -1.0;
	switchx[3] = x = -r; switchy[3] = y = 0.0;
	switchang[3] = atan2(y-ceny, x-cenx);
	if(switchang[3] < 0.0) {
		switchang[3] += 2.0*math_pi;
	}
	switchdist[3] = sqrt((x-cenx)*(x-cenx) + (y-ceny)*(y-ceny));
	// ang4 is along AC away from C based at A
	r = FindLenDist(0.0, 0.0, bx, 0.0, acang + math_pi, len - bclen -aclen, 0.7*extra);
	if(r < 0.0) return -1.0;
	switchx[4] = x = -r*cos(acang); switchy[4] = y = -r*sin(acang);
	switchang[4] = atan2(y-ceny, x-cenx);
	if(switchang[4] < 0.0) {
		switchang[4] += 2.0*math_pi;
	}
	switchdist[4] = sqrt((x-cenx)*(x-cenx) + (y-ceny)*(y-ceny));
	// to find ang5 shift origin to B find point on extn of BC that hits curve other dir
	r = FindLenDist(0.0, 0.0, -bx, 0.0, bcang + math_pi, len - aclen - bclen, 0.7*extra);
	if(r < 0.0) return -1.0;
	switchx[5] = x = bx - r*cos(bcang); switchy[5] = y = -r*sin(bcang);
	switchang[5] = atan2(y-ceny, x-cenx);
	if(switchang[5] < 0.0) {
		switchang[5] += 2.0*math_pi;
	}
	switchdist[5] = sqrt((x-cenx)*(x-cenx) + (y-ceny)*(y-ceny));
	// start with ang2 for historical reasons
	// in each case we set the coordinate origin at the centroid and find the area fo each sector
	curr = switchdist[2];
	area = 0.0;
	darea = findSectorArea(-cenx, -ceny, cx-cenx, cy-ceny, len - ablen - bclen, switchang[2], switchang[3], &curr);
	if(darea < 0.0) return -1.0;
	area += darea;
	darea = findSectorArea(bx-cenx, -ceny, cx-cenx, cy-ceny, len - bclen, switchang[3], switchang[4], &curr);
	if(darea < 0.0) return -1.0;
	area += darea;
	darea = findSectorArea(-cenx, -ceny, bx-cenx, -ceny, len - aclen - bclen, switchang[4], switchang[5], &curr);
	if(darea < 0.0) return -1.0;
	area += darea;
	darea = findSectorArea(-cenx, -ceny, cx-cenx, cy-ceny, len - aclen, switchang[5], switchang[0], &curr);
	if(darea < 0.0) return -1.0;
	area += darea;
	darea = findSectorArea(bx-cenx, -ceny, cx-cenx, cy-ceny, len - ablen - aclen, switchang[0], switchang[1]+2.0*math_pi, &curr);
	if(darea < 0.0) return -1.0;
	area += darea;
	darea = findSectorArea(-cenx, -ceny, bx-cenx, -ceny, len - ablen, switchang[1], switchang[2], &curr);
	if(darea < 0.0) return -1.0;
	area += darea;
	return area;
}

char inbuf[256];
int main()
{
	int nprob, curprob, index;
	double bx, cx, cy, len, area;
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
			fprintf(stderr, "Read failed on problem %d header\n", curprob);
			return -3;
		}
		if(sscanf(&(inbuf[0]), "%d %lf %lf %lf %lf", &index, &bx, &cx, &cy, &len) != 5) {
			fprintf(stderr, "scan failed on problem %d\n", curprob);
			return -4;
		}
		if(index != curprob)
		{
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				index, curprob);
			return -7;
		}
		if((bx < 0.0) || (cy < 0.0)) {
			fprintf(stderr, "invalid input on problem %d\n", curprob);
			return -9;
		}
		area = findArea2(bx, cx, cy, len);
		if(area < 0.0) {
			fprintf(stderr, "invalid input on problem %d\n", curprob);
			return -9;
		} else {
			printf("%d %.2lf\n", curprob, area);
		}
	}
	return 0;
}
