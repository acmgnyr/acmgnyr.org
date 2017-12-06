/*
 * D - Paver Tiling
 * GNYR 2017 Contest
 * by Fred Pickel
 *
 * DP solution
 */
#define	DO_LONG
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/*
 * F[n] = # of different tilings of 2xn rectangle by 1x1 tile, 2x1 time and right tromino
 * F1[n] = # of 1x1 tiles in all tilings counted in F[n]
 * F2[n] = # of 2x1 tiles in all tilings counted in F[n]
 * F3[n] = # of tromino tiles in all tilings counted in F[n]
 * G[n] = # tilings of 2xn retangle with lower rt corner removed and upper rt corner in not 1x1
 * G1[n] = # of 1x1 tiles in all tilings counted in G[n]
 * G2[n] = # of 2x1 tiles in all tilings counted in G[n]
 * G3[n] = # of tromino tiles in all tilings counted in G[n]
 * H[n] = # tilings of 2xn retangle with upper rt corner removed and lower rt corner in not 1x1
 * H1[n] = # of 1x1 tiles in all tilings counted in H[n]
 * H2[n] = # of 2x1 tiles in all tilings counted in H[n]
 * H3[n] = # of tromino tiles in all tilings counted in H[n]
 * lets now count tilings in F[n+1] by looking at possibilities on the right end
 * a. tiling can be split vertically at right end into a 2xn tiling and a 2x1 tiling
 *		take any 2xn tiling and
 *		add vertical 2x1 or					two stacked 1x1		2*F[n] contributes to F[n+1]
 *		for 1,2, 3 counts we have 2*counts for n plus
 *		1 new 2x1 for each F[n]				2 new 1x1 for each F[n]
 *		F1[n], F2[n]+F[n], F3[n]				F1[n]+2*F[n], F2[n], F3[n]
 * b. tiling can be split vertically at right end into a 2x(n-1) tiling and a 2x2 tiling
 *		and cannot be spilt as in a. Tke any tiling of 2x(n-1) and add
 *		two stacked horz 2x1				horz 2x1 above 2 1x1
 *		F1[n-1], F2[n-1]+2*F[n-1],F3[n-1]	F1[n-1]+2*F[n-1],F2[n-1]+F[n-1],F3[n-1]
 *		horz 2x1 below 2 1x1					tromino + 1x1 in lower rt	
 *		F1[n-1]+2*F[n-1],F2[n-1]+F[n-1],F3[n-1]	F1[n-1]+F[n-1],F2[n-1],F3[n-1]+F[n-1]	
 *		tromino + 1x1 in upper rt				tromino + 1x1 in lower left
 *		F1[n-1]+F[n-1],F2[n-1],F3[n-1]+F[n-1]	F1[n-1]+F[n-1],F2[n-1],F3[n-1]+F[n-1]
 *		tromino + 1x1 in upper left
 *		F1[n-1]+F[n-1],F2[n-1],F3[n-1]+F[n-1]		7*F[n-1] contributes to F[n+1]
 * If neither a not b hold then there is a tile crossing the vertical at n (2x1 or tromino)
 * and a tile crossing the vertical at n-1. to count these we use G and H
 * c. take a G[n] and add
 *		a tromino					a horz 2x1 below a 1x1 in upper right
 *		G1[n],G2[n],G3[n]+G[n]		G1[n]+G[n],G2[n]+G[n],G3[n]		total 2*G[n] contributes to F[n+1]
 * d. take a H[n] and add
 *		a tromino				a horz 2x1 above a 1x1 in lower right
 *		H1[n],H2[n],H3[n]+H[n]		H1[n]+H[n],H2[n]+H[n],H3[n]		total 2*H[n] contributes to F[n+1]
 * Now how can we make tilings for G[n+1]
 * a. take a tiling of 2x(n-1) and add
 *		a tromino with lower right missing		a horz 2x1 above a 1x1 in lower left
 *		F1[n-1],F2[n-1],F3[n-1]+F[n-1]			F1[n-1]+F[n-1],F2[n-1]+F[n-1],F3[n-1]		total 2*F[n-1]
 * b. take an H tiling of length n and add a 2x1 in upper right
 *		H1[n],H2[n]+H[n],H3[n]
 * Now how can we make tilings for H[n+1]
 * a. take a tiling of 2x(n-1) and add
 *		a tromino with upper right missing		a horz 2x1 below a 1x1 in lower left
 *		F1[n-1],F2[n-1],F3[n-1]+F[n-1]			F1[n-1]+F[n-1],F2[n-1]+F[n-1],F3[n-1]		total 2*F[n-1]
 * b. take a G tiling of length n and add a 2x1 in lower right
 *		G1[n],G2[n]+G[n],G3[n]
 * SO
 * NOTE: G[n] = H[n], G1[n] = H1[n], G2[n] = H2[n], G3[n] = H3[n];
 * F[n+1] = 2*F[n] + 7*F[n-1] + 4*G[n]
 * F1[n+1] = 2*F1[n] + 2*F[n] + 7*F1[n-1] + 8*F[n-1] + 4*G1[n]+2*G[n]
 * F2[n+1] = 2*F2[n] + F[n] + 7*F2[n-1] + 4*F[n-1] + 4*G2[n]+2*G[n]
 * F3[n+1] = 2*F3[n] + 7*F3[n-1] + 4*F[n-1] + 4*G3[n]+2*G[n]
 * F[0] = 1, F[1] = 2, F[3] = 11; F1[0] = 0, F1[1] = 2, F1[2] = 16
 * F2[0] = 0, F2[1] = 1, F2[2] = 8; F3[0] = 0, F3[1] = 0, F3[2] = 4
 *
 * G[n+1] = 2*F[n-1] + G[n]; G1[n+1] = 2*F1[n-1] + F[n-1] + G1[n]
 * G2[n+1] = 2*F2[n-1] + F[n-1] + G2[n] + G[n]; G3[n+1] = 2*F3[n-1] + F[n-1] + G3[n]
 * G[0] = 0, G[1] = 0, G[2] = 2; G1[0] = G1[1] = 0, G1[2] = 1
 * G2[0] = G2[1] = 0, G2[2] = 1; G3[0] = G3[1] = 0, G3[2] = 1
 */

#ifdef DO_LONG
typedef unsigned long CNT_TYPE;
#define MAX_SIZE	13
#else
#ifdef DO_QUAD
#ifdef LINUX
typedef unsigned long long CNT_TYPE;
#else
typedef unsigned __int64 CNT_TYPE;
#endif
#define MAX_SIZE	25
#else
typedef double CNT_TYPE;
#define MAX_SIZE	400
#endif
#endif
CNT_TYPE F[MAX_SIZE+1], F1[MAX_SIZE+1], F2[MAX_SIZE+1], F3[MAX_SIZE+1];
CNT_TYPE G[MAX_SIZE+1], G1[MAX_SIZE+1], G2[MAX_SIZE+1], G3[MAX_SIZE+1];

int maxindex = 0;

int comp_tiles()
{
	int n;
	CNT_TYPE test, test1;
	F[0] = 1; F[1] = 2; F[2] = 11;
	F1[0] = 0; F1[1] = 2; F1[2] = 16;
	F2[0] = 0; F2[1] = 1; F2[2] = 8;
	F3[0] = 0; F3[1] = 0; F3[2] = 4;
	G[0] = 0; G[1] = 0; G[2] = 2;
	G1[0] = G1[1] = 0; G1[2] = 1;
	G2[0] = G2[1] = 0; G2[2] = 1;
	G3[0] = G3[1] = 0; G3[2] = 1;
	for(n = 2; n <= MAX_SIZE-1 ; n++) {
		F[n+1] = 2*F[n] + 7*F[n-1] + 4*G[n];
		F1[n+1] = 2*F1[n] + 2*F[n] + 7*F1[n-1] + 8*F[n-1] + 4*G1[n]+2*G[n];
		F2[n+1] = 2*F2[n] + F[n] + 7*F2[n-1] + 4*F[n-1] + 4*G2[n]+2*G[n];
		F3[n+1] = 2*F3[n] + 7*F3[n-1] + 4*F[n-1] + 4*G3[n]+2*G[n];
		test = 2.0*((double)(n+1))*F[n+1];
		test1 = F1[n+1] + 2.0*F2[n+1] + 3.0*F3[n+1];
		if(fabs(test - test1) > .0000001*test) {
			fprintf(stderr, "mismatch %d: %g != %g\n", n+1, test, test1);
		}
		G[n+1] = 2*F[n-1] + G[n];
		G1[n+1] = 2*F1[n-1] + F[n-1] + G1[n];
		G2[n+1] = 2*F2[n-1] + F[n-1] + G2[n] + G[n];
		G3[n+1] = 2*F3[n-1] + F[n-1] + G3[n];
	}
	return 0;
}

char inbuf[256];
int main()
{
	int nprob, curprob, index, n;
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
	comp_tiles();
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d header\n", curprob);
			return -3;
		}
		if(sscanf(&(inbuf[0]), "%d %d", &index, &n) != 2) {
			fprintf(stderr, "scan failed on problem %d\n", curprob);
			return -4;
		}
		if(index != curprob)
		{
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				index, curprob);
			return -5;
		}
		if(n == 1){
			printf("%d 2 2 1 0\n", curprob);
		} else if((n < 2) || (n > MAX_SIZE)) {
			fprintf(stderr, "array width %d not in range 2 .. %d problem %d\n", n, MAX_SIZE, curprob);
			return -6;
		} else {
#ifdef DO_LONG
			printf("%d %lu %lu %lu %lu\n",
				curprob, F[n], F1[n], F2[n], F3[n]);
#else
#ifdef DO_QUAD
			printf("%d %llu %llu %llu %llu\n",
				curprob, F[n], F1[n], F2[n], F3[n]);
#else
			printf("%d %g %g %g %g %.6lf\n",
				curprob, F[n], F1[n], F2[n], F3[n], F1[n]/(F1[n]+F2[n]+F3[n]));
#endif
#endif
		}
	}
	return 0;
}
