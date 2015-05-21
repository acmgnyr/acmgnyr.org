/*
 * G - Triangulate
 * ICPC 2013 Greater NY Regional
 * Solution by Fred Pickel
 * Problem by Fred Pickel
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define MAX_VERTS	20

int vertCnts[MAX_VERTS+1];
int triVerts[3*MAX_VERTS];
int *tvsort[MAX_VERTS];

char inbuf[256];

char *Intscan(char *pbuf, int *pres)
{
	int val, digcnt, cur;
	val = digcnt = 0;
	while((*pbuf != 0) && isspace(*pbuf)) pbuf++;	/* skip leading spaces */
	while(isdigit(cur = *pbuf)) {
		digcnt++;
		val = 10*val + (cur - '0');
		pbuf++;
	}
	if(digcnt > 0) {
		*pres = val;
		return pbuf;
	} else {
		*pres = -1;
		return NULL;
	}
}

int ParseVerts(int curprob, int vcnt, char *pbuf)
{
	char *pscan = pbuf;
	int val, cnt, sum, onecnt;
	cnt = sum = onecnt = 0;
	if((pscan = Intscan(pscan, &val)) == NULL) {
		return -11;
	}
	if(curprob != val){
		return -12;
	}
	if((pscan = Intscan(pscan, &val)) == NULL) {
		return -11;
	}
	if(vcnt != val){
		return -13;
	}
	for(cnt = 0; cnt < vcnt ; cnt++) {
		if((pscan = Intscan(pscan, &val)) == NULL) {
			return -11;
		}
		if(val < 1) {
			return -14;
		}
		vertCnts[cnt] = val;
		sum += val;
		if(val == 1) onecnt++;
	}
	if(sum != 3*(vcnt-2)) {
		return 1;
	}
	if(onecnt < 2) {
		return 2;
	}
	return 0;
}

/*
 * succesively find triangles of the tringulation by:
 * (there must always be an "outside" triangle with one vertex which is a vertex of only hte outside triangle)
 *	1. Find a vertex with a triangle count of 1	
 *	2. scan left (with wrap) to find the next vertex with a non-zero triangle count
 *	3. scan right (with wrap) to find the next vertex with a non-zero triangle count
 * these are thhe vertices of a triangle of the triangulation
 * write thes vertices out (in numerical order) to the triangle list and decrement
 * each of the triangle counts by 1 to indicate removing the triangle
 * if at any point this fails, the vertex counts did not belong to a triangulation
 * otherwise we get the vcnt-2 triangles of the triangulation
 */
int FindTriangles(int curprob, int vcnt)
{
	int tricnt, scanone, scanr, scanl, i, j;
	for(tricnt = 0; tricnt < vcnt-2 ; tricnt++) {
		scanone = -1;
		for(i = 0; i < vcnt; i++){
			if(vertCnts[i] == 1) {
				scanone = i;
				break;
			}
		}
		if(scanone < 0){
			return -1;
		}
		scanr = -1;
		i = (scanone + 1) % vcnt;
		for(j = 0; j < vcnt-1 ; j++) {
			if(vertCnts[i] > 0) {
				scanr = i;
				break;
			}
			i = (i+1)%vcnt;
		}
		if(scanr < 0){
			return -2;
		}
		scanl = -1;
		i = (scanone - 1 + vcnt) % vcnt;
		for(j = 0; j < vcnt-1 ; j++) {
			if(vertCnts[i] > 0) {
				scanl = i;
				break;
			}
			i = (i-1 + vcnt)%vcnt;
		}
		if((scanl < 0) || (scanl == scanr)){
			return -3;
		}
		vertCnts[scanone]--;
		vertCnts[scanr]--;
		vertCnts[scanl]--;
		if(scanone < scanr){
			if(scanone < scanl) {
				if(scanl < scanr){
					triVerts[3*tricnt] = scanone;
					triVerts[3*tricnt+1] = scanl;
					triVerts[3*tricnt+2] = scanr;
				} else {
					triVerts[3*tricnt] = scanone;
					triVerts[3*tricnt+1] = scanr;
					triVerts[3*tricnt+2] = scanl;
				}
			} else {
				triVerts[3*tricnt] = scanl;
				triVerts[3*tricnt+1] = scanone;
				triVerts[3*tricnt+2] = scanr;
			}
		} else {
			if(scanr < scanl) {
				if(scanl < scanone){
					triVerts[3*tricnt] = scanr;
					triVerts[3*tricnt+1] = scanl;
					triVerts[3*tricnt+2] = scanone;
				} else {
					triVerts[3*tricnt] = scanr;
					triVerts[3*tricnt+1] = scanone;
					triVerts[3*tricnt+2] = scanl;
				}
			} else {
				triVerts[3*tricnt] = scanl;
				triVerts[3*tricnt+1] = scanr;
				triVerts[3*tricnt+2] = scanone;
			}
		}
	}
	return 0;
}

int compareTris(const void *a, const void *b)
{
	int *pa = *((int **)a);
	int *pb = *((int **)b);
	if(pa[0] < pb[0]) {
		return -1;
	} else if(pa[0] > pb[0]) {
		return 1;
	} else {	// pa[0] == pb[0]
		if(pa[1] < pb[1]) {
			return -1;
		} else if(pa[1] > pb[1]) {
			return 1;
		} else {	// pa[0] == pb[0] AND  pa[1] == pb[1]
			if(pa[2] < pb[2]) {
				return -1;
			} else if(pa[2] > pb[2]) {
				return 1;
			} else {
				return 0;	// should not happen
			}
		}
	}
}

void SortTris(int curprob, int vcnt)
{
	int i;
	for(i = 0; i < vcnt-2 ; i++) {
		tvsort[i] = &(triVerts[3*i]);
	}
	qsort((void *)(&(tvsort[0])), (vcnt -2), sizeof(int *), compareTris);

}

int main()
{
	int nprob, curprob, index, vcnt, ret, i, *pv;

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
			fprintf(stderr, "Read failed on problem %d data\n", curprob);
			return -3;
		}
		// get prob num and sequence index
		if(sscanf(&(inbuf[0]), "%d %d", &index, &vcnt) != 2)
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
		if((vcnt < 4) || (vcnt > MAX_VERTS)) {
			fprintf(stderr, "problem index %d vertex count %d not in range 4 ... %d\n",
				curprob, vcnt, MAX_VERTS);
			return -8;
		}
		ret = ParseVerts(curprob, vcnt, &(inbuf[0]));
		if(ret < 0){
			fprintf(stderr, "problem index %d scan of vertices returned %d\n",
				curprob, ret);
			return ret;
		}
		if(ret > 0) {
			printf("%d N\n", curprob);
		} else {
			ret = FindTriangles(curprob, vcnt);
			if(ret < 0) {
				printf("%d N\n", curprob);
			} else {
				SortTris(curprob, vcnt);
				printf("%d Y\n", curprob);
				for(i = 0; i < vcnt-2; i++) {
					pv = tvsort[i];
					printf("%d %d %d\n", pv[0]+1, pv[1]+1, pv[2]+1);
				}
			}

		}
	}
	return 0;
}
