#include <stdio.h>
#include <ctype.h>
#include <math.h>

#define MAX_NODES	20
#define MAX_QUERIES	10

/*
 * we want to find the diffence in voltages at query node1 and query node 2 when
 * the current into query node 1 is 1 amp and the current out of query node 2 is 1 (-1 in) and
 * the current into all other nodes is zero
 * current into node i = SUM (current out over each edge connected to node i)
 * = SUM(eij is an edge from i to j: current from i to j)
 * = SUM(eij is an edge from i to j: voltage at  i - votage at j) since all resistances are 1
 * = 0 for i != qurey node 1 and i != query node 2
 * = 1 for query node 1
 * = -1 for query node 2
 * this give nnodes equations in nnodes unknowns HOWEVER
 * the euqations are independent. If you add ath same value to each voltage in a solution you 
 * get another solution (since all the euqations are summs of (Vi - Vj)
 * we add ather row requiring the summ of all the cvoltages to be 0
 * we solve this system of equations by Gaussian elimination
 * NOTE that you can solve for all queries at once since the coeeficient matrix is the same
 */
double Adjacent[MAX_NODES + 1][MAX_NODES + MAX_QUERIES];
int query1[MAX_QUERIES], query2[MAX_QUERIES];

// zero out the main matrix and set the last coefficient row to all 1's
int AdjacentInit(int nnodes, int nqueries)
{
	int i, j;
	for(i = 0; i <= nnodes ; i++) {
		for(j = 0; j < nnodes + nqueries ; j++) {
			Adjacent[i][j] = 0.0;
		}
	}
	for(i = 0; i < nnodes ; i++) {
		Adjacent[nnodes][i] = 1.0;
	}
	return 0;
}

// scan string for <node>sp<count>sp<count other end nodes>
// and add/subtrat values from the corresponding matrix entries
int ScanEdgeData(int nnodes, char *pb)
{
	int val, node, count, i;
	node = count = 0;
	while((*pb != 0) && (isspace(*pb))) pb++;
	if((*pb == 0) || (!isdigit(*pb))){
		return -1;
	}
	while(isdigit(*pb)) {
		node = (node * 10) + (*pb - '0');
		pb++;
	}
	if((*pb == 0) || (!isspace(*pb))) return -2;
	while((*pb != 0) && (isspace(*pb))) pb++;
	if((*pb == 0) || (!isdigit(*pb))) return -3;
	while(isdigit(*pb)) {
		count = (count * 10) + (*pb - '0');
		pb++;
	}
	Adjacent[node-1][node-1] += (double)count;
	for(i = 0; i < count ; i++) {
		if((*pb == 0) || (!isspace(*pb))) return -4;
		while((*pb != 0) && (isspace(*pb))) pb++;
		if((*pb == 0) || (!isdigit(*pb))) return -5;
		val = 0;
		while(isdigit(*pb)) {
			val = (val * 10) + (*pb - '0');
			pb++;
		}
		if((val == 0) || (val > nnodes)) {
			fprintf(stderr, "node %d val %d not in range 1 .. %d \n", i, val, nnodes);
			return -6;
		}
		Adjacent[node-1][val-1] = -1.0;
		Adjacent[val-1][node-1] = -1.0;
		Adjacent[val-1][val-1] += 1.0;
	}
	return count;
}

// find the row below or equal currow with the max abs value in column currow
int FindMaxRow(int nnodes, int nqueries, int currow)
{
	int i, maxrow;
	double max, tmp;
	max = fabs(Adjacent[currow][currow]);
	maxrow = currow;
	for(i = currow + 1; i <= nnodes ; i++) {
		tmp = fabs(Adjacent[i][currow]);
		if(tmp > max) {
			max = tmp;
			maxrow = i;
		}
	}
	return maxrow;
}

// interchange maxrow and currow
void SwapRows(int maxrow, int currow, int nnodes, int nqueries)
{
	int i, ncols;
	double tmp;
	ncols = nnodes + nqueries;
	for(i = 0; i < ncols ; i++) {
		tmp = Adjacent[currow][i];
		Adjacent[currow][i] = Adjacent[maxrow][i];
		Adjacent[maxrow][i] = tmp;
	}
}

// Adjacent[currow][currow] = 1.0
// for each other row, subtract muliples of currow to make Adjacent[i][currow] = 0.0
int Eliminate(int currow, int nrows, int ncols)
{
	int i, j;
	double factor;
	for(i = 0; i < nrows; i++) {
		if(i == currow) {
			continue;
		}
		factor = Adjacent[i][currow];
		for(j = currow; j < ncols ; j++) {
			Adjacent[i][j] -= factor*Adjacent[currow][j];
		}
	}
	return 0;
}

// dump current Adjacent matrix for debugging
void DumpMatrix(int nrows, int ncols)
{
	int i, j;
	for(i = 0; i < nrows ; i++){
		for(j = 0; j < ncols ; j++) {
			printf("%.2lf ", Adjacent[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

// use row operations to convert Adjacent to [identity| stuff]
// so you can read off solutions from stuff
int SolveMatrix(int nnodes, int nqueries)
{
	int nrows, ncols, currow, maxrow, i;
	double pivot;
	ncols = nnodes + nqueries;
	nrows = nnodes + 1;
	for(currow = 0; currow < nnodes; currow++) {
		// find max in col currow in row not yet used
		maxrow = FindMaxRow(nnodes, nqueries, currow);
		// if not currow, swap
		if(maxrow != currow) {
			SwapRows(maxrow, currow, nnodes, nqueries);
		}
		// div currow by pivot
		pivot = Adjacent[currow][currow];
		if(fabs(pivot) < .001) {
			return -1;
		}
		pivot = 1.0/pivot;
		for(i = currow; i < ncols ; i++) {
			Adjacent[currow][i] *= pivot;
		}
		// now make Adjacent[i][currow] = 0 in all other rows
		Eliminate(currow, nrows, ncols);
//		DumpMatrix(nrows, ncols);
	}
	return 0;
}

char inbuf[256];
int main()
{
	int nprob, curprob, index, nnodes, nqueries, nedges;
	int i, edgecnt, edgelines, queryno;
	double dist;
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
		if(sscanf(&(inbuf[0]), "%d %d %d %d", &index, &nnodes, &nqueries, &nedges) != 4) {
			fprintf(stderr, "scan failed on problem %d\n", curprob);
			return -4;
		}
		if(index != curprob) {
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				index, curprob);
			return -5;
		}
		if((nnodes > MAX_NODES) || (nqueries > MAX_QUERIES)) {
			fprintf(stderr, "prob %d: nnodes %d > max %d or nqueries %d > max %d\n",
			curprob, nnodes, MAX_NODES, nqueries, MAX_QUERIES);
			return -6;
		}
		AdjacentInit(nnodes, nqueries);
		// read edge data
		edgecnt = edgelines = 0;
		while(edgecnt < nedges) {
			if(fgets(&(inbuf[0]), 255, stdin) == NULL)
			{
				fprintf(stderr, "Read failed on problem %d edgeline %d\n", curprob, edgelines);
				return -7;
			}
			if((i = ScanEdgeData(nnodes, &(inbuf[0]))) < 0) {
				fprintf(stderr, "scan failed on problem %d edgeline %d\n", curprob, edgelines);
				return -8;
			}
			edgelines++;
			edgecnt += i;
		}
		for(i = 0; i < nqueries ; i++) {
			if(fgets(&(inbuf[0]), 255, stdin) == NULL)
			{
				fprintf(stderr, "Read failed on problem %d query %d\n", curprob, i+1);
				return -9;
			}
			if(sscanf(&(inbuf[0]), "%d %d %d", &queryno, &(query1[i]), &(query2[i])) != 3) {
				fprintf(stderr, "scan failed on problem %d query %d\n", curprob, i+1);
				return -10;
			}
			if(i+1 != queryno) {
				fprintf(stderr, "read query num %d != expected %d problem %d\n", queryno, i+1, curprob);
				return -11;
			}
			if((query1[i] < 1) || (query1[i] > nnodes) || (query2[i] < 1) || (query2[i] > nnodes) || (query1[i] == query2[i])) {
				fprintf(stderr, "bad queryid1 %d or queryid2 %d problem %d query %d\n",
					query1, query2, curprob, i+1);
				return -12;
			}
			Adjacent[query1[i]-1][nnodes + i] = 1.0;
			Adjacent[query2[i]-1][nnodes + i] = -1.0;
		}
//		DumpMatrix(nnodes+1, nnodes + nqueries);
		if((i = SolveMatrix(nnodes, nqueries)) != 0) {
			fprintf(stderr, "error retrun %d from SolveMatrix problem %d\n", i, curprob);
			return -13;
		} else {
			printf("%d", curprob);
			for(i = 0; i < nqueries ; i++) {
				dist = fabs(Adjacent[query1[i]-1][nnodes + i] - Adjacent[query2[i]-1][nnodes + i]);
				printf(" %.3lf", dist);
			}
			printf("\n");
		}
	}
	return 0;
}
