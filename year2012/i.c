/*
 * I - PushTo Telescope
 * ICPC 2012 Greater NY Regional
 * Solution by Fred Pickel
 * Problem by Fred Pickel
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#undef TEST

#define EPS .01
#define MAX_STARS	100
#define MAX_SUBPROBS 10
#define PI	(3.141592653589793238462643383279)

double rotRate = (2.0 * PI * 1.0027379093)/86400.0;

double mat[3][6];
double matA[3][3];
typedef struct _star_data_
{
	double azimuth;
	double elevation;
} STAR_DATA;

STAR_DATA stars[MAX_STARS+1];
double v0[3], v1[3], v2[3];
double star1base[3], star1meas[3], star2base[3], star2meas[3], star3base[3], star3meas[3];
double star2baseA[3], star2measA[3], star3baseA[3], star3measA[3];

// we use a coordinate system that rotates with the earth and aligns with
// geocentric equatorial coordinates when the telescpoe is turned on (at time = 0)
// in this coordinate system the declination of a start is the same but the right ascension is 
// ra - t*rotRate	where ra is right ascension in geocentric equatorial coordinates
// and rot rate is the earths rotaion rate in radians per second
//
// in SetStar1 below we get a vector in this coordinate system to the first star
// the corresponding vector in telescope coordinates is determined by the measured azimuth and elevation
// in SetStar2 below we get a vector in this coordinate system to the second star
// and a vector in telescope coordinates
// then get a third independnet vector in each coordinate system by taking the cross product
// of the two vectors we already have
// what we want is a matrix that transforms the rotating start coordinates to telsescope coordinates
// if the columns of A are the vectors in star coordinates and the vectors in B are the corresponding
// telescope coordinates, we want B*A^(-1) the solution to X*A = B or Atrans * Xtrans = Btrans (trans = transpose
// solvestars uses gausian elimination to solve for Xtrans (int mat[0-2][3-5]
// this is then used to find telescope coordinates for subsequent stars
int solvestars()
{
	int i, j, k, pivind;
	double pivot, temp;
	for(i = 0; i < 3; i++) {
		mat[0][i] = star1base[i];
		mat[1][i] = star2base[i];
		mat[2][i] = star3base[i];
		mat[0][3+i] = star1meas[i];
		mat[1][3+i] = star2meas[i];
		mat[2][3+i] = star3meas[i];
	}
	// gauss elim
	for( i = 0; i < 3 ; i++) {
		// find largest abs size elt in col i row >= i
		pivot = mat[i][i];
		pivind = i;
		for(j = i+1 ; j < 3; j++) {
			if(fabs(mat[j][i]) > fabs(pivot)) {
				pivot = mat[j][i];
				pivind = j;
			}
		}
		if(pivind != i) {
			// swap rows
			for(j = i; j < 6; j++) {
				temp = mat[i][j];
				mat[i][j] = mat[pivind][j];
				mat[pivind][j] = temp;
			}
		}
		if(fabs(pivot) < EPS) {
			return -1;
		}
		pivot = 1.0/pivot;
		// normalize pivot row
		for(j = i; j < 6 ; j++) {
			mat[i][j] *= pivot;
		}
		// eliminate from remaining rows
		for(j = 0; j < 3; j++) {
			if(j == i) {
				continue;
			}
			pivot = mat[j][i];
			for(k = i ;k < 6; k++) {
				mat[j][k] -= pivot * mat[i][k];
			}
		}
	}
#ifdef TEST
	for(i = 0; i < 3; i++) {
		v0[i] = v1[i] = v2[i] = 0;
	}
	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			v0[i] += mat[j][3+i]*star1base[j];
			v1[i] += mat[j][3+i]*star2base[j];
			v2[i] += mat[j][3+i]*star3base[j];
		}
	}
	fprintf(stderr, "%.4f %.4f %.4f %.4f %.4f %.4f\n",
		v0[0], v0[1], v0[2], star1meas[0], star1meas[1], star1meas[2]);
	fprintf(stderr, "%.4f %.4f %.4f %.4f %.4f %.4f\n",
		v1[0], v1[1], v1[2], star2meas[0], star2meas[1], star2meas[2]);
	fprintf(stderr, "%.4f %.4f %.4f %.4f %.4f %.4f\n",
		v2[0], v2[1], v2[2], star3meas[0], star3meas[1], star3meas[2]);
#endif
	return 0;
}

int SetStar1(int secs, int ind, double measaz, double measelev)
{
	STAR_DATA *pBase = &(stars[ind]);
	star1base[2] = sin(pBase->elevation);
	star1base[0] = cos(pBase->elevation) * cos(pBase->azimuth - ((double)secs)*rotRate);
	star1base[1] = cos(pBase->elevation) * sin(pBase->azimuth - ((double)secs)*rotRate);
	star1meas[2] = sin(measelev);
	star1meas[0] = cos(measelev) * cos(measaz);
	star1meas[1] = cos(measelev) * sin(measaz);
	return 0;
}

int SetStar2(int secs, int ind, double measaz, double measelev)
{
	STAR_DATA *pBase = &(stars[ind]);
	star2base[2] = sin(pBase->elevation);
	star2base[0] = cos(pBase->elevation) * cos(pBase->azimuth - ((double)secs)*rotRate);
	star2base[1] = cos(pBase->elevation) * sin(pBase->azimuth - ((double)secs)*rotRate);
	star2meas[2] = sin(measelev);
	star2meas[0] = cos(measelev) * cos(measaz);
	star2meas[1] = cos(measelev) * sin(measaz);
	// star3  dir is cross prod
	star3base[0] = star1base[1] * star2base[2] - star1base[2] * star2base[1];
	star3base[1] = star1base[2] * star2base[0] - star1base[0] * star2base[2];
	star3base[2] = star1base[0] * star2base[1] - star1base[1] * star2base[0];
	star3meas[0] = star1meas[1] * star2meas[2] - star1meas[2] * star2meas[1];
	star3meas[1] = star1meas[2] * star2meas[0] - star1meas[0] * star2meas[2];
	star3meas[2] = star1meas[0] * star2meas[1] - star1meas[1] * star2meas[0];
	return solvestars();
}

// alternate version
// we make star2baseA and star2measA orthogonal to star1base and star1meas resp
// then make them unit vectors
// then compute star3... as befor by cross product
// the resulting matrix of columns A = [star1base star2baseA star3baseA] is orthognal so A^-1 = Atrans
// so the matrix we need is B*Atrans which we compute directly

// NOTE: these two methods do not ingeneral compute exactly the same star directions
// the first method computes a matrix which takes the base star directions to the measured star directions
// the second method computes and orthogonal (rotation) matrix which takes the first star base direction
// to the first star measured direction and the plane of the two star base diections to the plane of the
// two measured star directions
// In the second case assunes that the angle between the base star directions and the angle between
// the measured star directions is exactly the same (rotations preserve angles)
// If this is not true, due for instance to errors in pointing at the base stars, the two methods
// will not give the same answer
// It is beyond the scope of this problem to correct for those errors
// for instance by adjusting the two measured star directions by the smallest amount that gives
// the same angle for the adjusted measured directions as the base directions
int SetStar2A(int secs, int ind, double measaz, double measelev)
{
	STAR_DATA *pBase = &(stars[ind]);
	double dbase, dmeas, magbase, magmeas;
	int i, j;
	star2baseA[2] = sin(pBase->elevation);
	star2baseA[0] = cos(pBase->elevation) * cos(pBase->azimuth - ((double)secs)*rotRate);
	star2baseA[1] = cos(pBase->elevation) * sin(pBase->azimuth - ((double)secs)*rotRate);
	star2measA[2] = sin(measelev);
	star2measA[0] = cos(measelev) * cos(measaz);
	star2measA[1] = cos(measelev) * sin(measaz);
	// find dot product
	dbase = dmeas = 0.0;
	magbase = magmeas = 0.0;
	for(i= 0; i < 3; i++) {
		dbase += star1base[i]*star2baseA[i];
		dmeas += star1meas[i]*star2measA[i];
		magbase += star1base[i] * star1base[i];
		magmeas += star1meas[i] * star1meas[i];
	}
	// make star2 perp to star1 and compute magnitudes
	magbase = magmeas = 0.0;
	for(i= 0; i < 3; i++) {
		star2baseA[i] -= dbase * star1base[i];
		star2measA[i] -= dmeas * star1meas[i];
		magbase += star2baseA[i] * star2baseA[i];
		magmeas += star2measA[i] * star2measA[i];
	}
	if((magbase < EPS) || (magmeas < EPS)) {
		return -5;
	}
	magbase = 1.0/sqrt(magbase);
	magmeas = 1.0/sqrt(magmeas);
	// normalize
	dbase = dmeas = 0.0;
	for(i= 0; i < 3; i++) {
		star2baseA[i] *= magbase;
		star2measA[i] *= magmeas;
		dbase += star1base[i]*star2baseA[i];
		dmeas += star1meas[i]*star2measA[i];
	}
	// star3  dir is cross prod
	star3baseA[0] = star1base[1] * star2baseA[2] - star1base[2] * star2baseA[1];
	star3baseA[1] = star1base[2] * star2baseA[0] - star1base[0] * star2baseA[2];
	star3baseA[2] = star1base[0] * star2baseA[1] - star1base[1] * star2baseA[0];
	star3measA[0] = star1meas[1] * star2measA[2] - star1meas[2] * star2measA[1];
	star3measA[1] = star1meas[2] * star2measA[0] - star1meas[0] * star2measA[2];
	star3measA[2] = star1meas[0] * star2measA[1] - star1meas[1] * star2measA[0];
	dbase = dmeas = 0.0;
	magbase = magmeas = 0.0;
	for(i= 0; i < 3; i++) {
		dbase += star1base[i]*star3baseA[i];
		dmeas += star1meas[i]*star3measA[i];
		magbase += star2baseA[i] * star3baseA[i];
		magmeas += star2measA[i] * star3measA[i];
	}

	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			matA[i][j] = star1base[j] * star1meas[i] + star2baseA[j] * star2measA[i] + star3baseA[j] * star3measA[i];
		}
	}
#ifdef TEST
	for(i = 0; i < 3; i++) {
		v0[i] = v1[i] = v2[i] = 0;
	}
	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			v0[i] += matA[i][j]*star1base[j];
			v1[i] += matA[i][j]*star2base[j];
			v2[i] += matA[i][j]*star3base[j];
		}
	}
	fprintf(stderr, "%.4f %.4f %.4f %.4f %.4f %.4f\n",
		v0[0], v0[1], v0[2], star1meas[0], star1meas[1], star1meas[2]);
	fprintf(stderr, "%.4f %.4f %.4f %.4f %.4f %.4f\n",
		v1[0], v1[1], v1[2], star2meas[0], star2meas[1], star2meas[2]);
	fprintf(stderr, "%.4f %.4f %.4f %.4f %.4f %.4f\n",
		v2[0], v2[1], v2[2], star3meas[0], star3meas[1], star3meas[2]);
#endif
	return 0;
}

int FindStarA(int secs, int ind, double *paz, double *pelev)
{
	double x, y, z, r, base[3], az;
	STAR_DATA *pBase = &(stars[ind]);
	base[2] = sin(pBase->elevation);
	base[0] = cos(pBase->elevation) * cos(pBase->azimuth - ((double)secs)*rotRate);
	base[1] = cos(pBase->elevation) * sin(pBase->azimuth - ((double)secs)*rotRate);

	x = base[0] * matA[0][0] + base[1] * matA[0][1] + base[2] * matA[0][2];
	y = base[0] * matA[1][0] + base[1] * matA[1][1] + base[2] * matA[1][2];
	z = base[0] * matA[2][0] + base[1] * matA[2][1] + base[2] * matA[2][2];
	r = sqrt(x*x + y*y);
	*pelev = atan2(z, r) * 180.0/PI;
	az = atan2(y, x) * 180.0/PI;
	if(az < 0.0) az += 180.0;
	*paz = az;

	return 0;
}

int FindStar(int secs, int ind, double *paz, double *pelev)
{
	double x, y, z, r, base[3], az;
	STAR_DATA *pBase = &(stars[ind]);
	base[2] = sin(pBase->elevation);
	base[0] = cos(pBase->elevation) * cos(pBase->azimuth - ((double)secs)*rotRate);
	base[1] = cos(pBase->elevation) * sin(pBase->azimuth - ((double)secs)*rotRate);

	x = base[0] * mat[0][3] + base[1] * mat[1][3] + base[2] * mat[2][3];
	y = base[0] * mat[0][4] + base[1] * mat[1][4] + base[2] * mat[2][4];
	z = base[0] * mat[0][5] + base[1] * mat[1][5] + base[2] * mat[2][5];
	r = sqrt(x*x + y*y);
	*pelev = atan2(z, r) * 180.0/PI;
	az = atan2(y, x) * 180.0/PI;
	if(az < 0.0) az += 180.0;
	*paz = az;

	return 0;
}

char inbuf[256];

int main()
{
	int nprob, nstars, curprob, index, subprobs, starInd, secs, subprob;
	double elev, az;

	if(fgets(&(inbuf[0]), 255, stdin) == NULL)
	{
		fprintf(stderr, "Read failed on problem and star count\n");
		return -1;
	}
	if(sscanf(&(inbuf[0]), "%d %d", &nstars, &nprob) != 2)
	{
		fprintf(stderr, "Scan failed on problem and star count\n");
		return -2;
	}
	if((nstars < 5) || (nstars > MAX_STARS))
	{
		fprintf(stderr, "star count must be in range 5 to %d\n", MAX_STARS);
		return -21;
	}
	for(starInd = 1; starInd <= nstars ; starInd++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on star %d data\n", starInd);
			return -23;
		}
		if(sscanf(&(inbuf[0]), "%d %lf %lf", &index, &az, &elev) != 3)
		{
			fprintf(stderr, "scan failed on star %d data\n", starInd);
			return -26;
		}
		if(starInd != index) {
			fprintf(stderr, "star index %d not = expected star %d\n",
				index, starInd);
			return -27;
		}
		stars[index].azimuth = (az * PI)/180.0;
		stars[index].elevation = (elev * PI)/180.0;
	}
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem header problem index  %d \n", curprob);
			return -3;
		}
		// get prob num and subprob count
		if(sscanf(&(inbuf[0]), "%d %d", &index, &subprobs) != 2)
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
		if((subprobs < 1) || (subprobs > MAX_SUBPROBS)) {
			fprintf(stderr, "number of subproblems %d not in range 1 to %d problem %d\n",
				subprobs, MAX_SUBPROBS, curprob);
			return -8;
		}

		// get first alignment star
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on first alignment star problem %d \n", curprob);
			return -10;
		}
		if(sscanf(&(inbuf[0]), "%d %d %lf %lf", &secs, &starInd, &az, &elev) != 4)
		{
			fprintf(stderr, "scan failed on on first alignment star problem %d\n",
				curprob);
			return -11;
		}
		if((starInd < 1) || (starInd > MAX_STARS)) {
			fprintf(stderr, "first alignment star index %d not in range 1 to %d problem %d\n",
				starInd, MAX_STARS, curprob);
			return -51;
		}
		SetStar1(secs, starInd, (PI*az)/180.0, (PI*elev)/180.0);
		// get second alignment star
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on second alignment star problem %d \n", curprob);
			return -12;
		}
		if(sscanf(&(inbuf[0]), "%d %d %lf %lf", &secs, &starInd, &az, &elev) != 4)
		{
			fprintf(stderr, "scan failed on on second alignment star problem %d\n",
				curprob);
			return -13;
		}
		if((starInd < 1) || (starInd > MAX_STARS)) {
			fprintf(stderr, "second alignment star index %d not in range 1 to %d problem %d\n",
				starInd, MAX_STARS, curprob);
			return -52;
		}
#ifdef USE_ORIG
		if(SetStar2(secs, starInd, (PI*az)/180.0, (PI*elev)/180.0) != 0) {
			fprintf(stderr, "bad alignment stars problem %d\n",
				curprob);
			return -14;
		}
#else
		if(SetStar2A(secs, starInd, (PI*az)/180.0, (PI*elev)/180.0) != 0) {
			fprintf(stderr, "bad alignmentA stars problem %d\n",
				curprob);
			return -24;
		}
#endif
		printf("%d %d\n", curprob, subprobs);
		for(subprob = 1 ; subprob <= subprobs ; subprob++) {
			if(fgets(&(inbuf[0]), 255, stdin) == NULL)
			{
				fprintf(stderr, "Read failed on sub problem %d problem %d \n", subprob, curprob);
				return -15;
			}
			if(sscanf(&(inbuf[0]), "%d %d %d", &index, &secs, &starInd) != 3)
			{
				fprintf(stderr, "scan failed on on sub problem %d problem %d\n",
					subprob, curprob);
				return -16;
			}
			if(index != subprob) {
				fprintf(stderr, "subproblem index %d not = expected %d problem %d\n",
					index, subprob, curprob);
				return -17;
			}
			if((starInd < 1) || (starInd > MAX_STARS)) {
				fprintf(stderr, "sub problem %d star index %d not in range 1 to %d problem %d\n",
					subprob, starInd, MAX_STARS, curprob);
				return -53;
			}
#ifdef USE_ORIG
			FindStar(secs, starInd, &az, &elev);
#else
			FindStarA(secs, starInd, &az, &elev);
#endif
			if(elev < 0.0) {
				printf("%d NOT VISIBLE\n", subprob);
			} else {
				printf("%d %.1lf %.1lf\n", subprob, az, elev);
			}
		}

	}
	return 0;
}
