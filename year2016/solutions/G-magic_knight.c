/*
 * G - Magic Knights Tour
 * Solution by Fred Pickel
 * ACM ICPC GNYR 2016
 */
#include <stdio.h>
#include <ctype.h>
#include <math.h>

#define MAGIC_SUM 260		//((64*(64+1))/2)/8
typedef unsigned short WORD;
typedef unsigned char BYTE;

char fixedx[66], fixedy[66];
char movex[8] = {1, 2, 2, 1, -1, -2, -2, -1};
char movey[8] = {-2, -1, 1, 2, 2, 1, -1, -2};
WORD remsum[9];
 
int maxlevel;

typedef struct _board_t_
{
	WORD rowsum[8];
	WORD colsum[8];
	BYTE rowfree[8];
	BYTE colfree[8];
	char curx;
	char cury;
	BYTE curmove;
	char pad[5];
	char board[8][8];
} BOARD;

BOARD boards[64];

int CheckSums(int level)
{
	int i;
	for(i = 0; i < 8 ; i++) {
		if((MAGIC_SUM - boards[level].rowsum[i]) > remsum[boards[level].rowfree[i]]) {
			// this row can not possibly get up to MAGIC_SUM
			return -1;
		}
		if((MAGIC_SUM - boards[level].colsum[i]) > remsum[boards[level].colfree[i]]) {
			// this col can not possibly get up to MAGIC_SUM
			return -2;
		}
		if((boards[level].rowsum[i] + boards[level].rowfree[i]*boards[level].curmove) > MAGIC_SUM) {
			// this row sum is guaranteed to be larger than MAGIC_SUM
			return -3;
		}
		if((boards[level].colsum[i] + boards[level].colfree[i]*boards[level].curmove) > MAGIC_SUM) {
			// this row sum is guaranteed to be larger than MAGIC_SUM
			return -4;
		}
	}
	return 0;
}

int test = 0;

int SolveBoard(int level)
{
	char newx, newy, move, nextmove;
	int nextlevel, found, ret = -1;
	int curmove = boards[level].curmove;
	if(CheckSums(level) != 0) {
		return -1;
	}
	if(level > maxlevel) maxlevel = level;
	if(curmove == 64) {
		return level;
	} else if(curmove == 63) {
		test++;
	}
	for(move = 0; move < 8 ; move++) {
		newx = boards[level].curx + movex[move];
		newy = boards[level].cury + movey[move];
		if((newx < 0) || (newx >= 8) || (newy < 0) || (newy >= 8)) {
			continue;
		}
		if(boards[level].board[newy][newx] >= 0) {
			continue;
		}
		if((fixedx[curmove+2]) >= 0) { // move after this is fixed so we better be able to get there
			found = 0;
			for(nextmove = 0; nextmove < 8 ; nextmove++) {
				if(((newx+movex[nextmove]) == fixedx[curmove+2]) && 
					((newy+movey[nextmove]) == fixedy[curmove+2])) {
					found = 1;
					break;
				}
			}
			if(found == 0) {
				continue;
			}
		}
		nextlevel = level+1;
		nextmove = curmove + 1;
		boards[nextlevel] = boards[level];
		boards[nextlevel].board[newy][newx] = curmove+1;
		boards[nextlevel].rowfree[newy]--;
		boards[nextlevel].colfree[newx]--;
		boards[nextlevel].colsum[newx] += curmove+1;
		boards[nextlevel].rowsum[newy] += curmove+1;
		boards[nextlevel].curx = newx;
		boards[nextlevel].cury = newy;
		boards[nextlevel].curmove = nextmove;
		while(fixedx[nextmove+1] >= 0) { // skip to the next board
			nextmove++;
			boards[nextlevel+1] = boards[nextlevel];
			boards[nextlevel+1].curx = fixedx[nextmove];
			boards[nextlevel+1].cury = fixedy[nextmove];
			boards[nextlevel+1].curmove = nextmove;
			nextlevel++;
		}
		if((ret = SolveBoard(nextlevel)) >= 0) {
			return ret;
		}
		
	}
	return -1;
}

char inbuf[256];

int main()
{
	int nprob, curprob, index;
	int i, j, ret, vals[8], sum;
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
		if(sscanf(&(inbuf[0]), "%d", &index) != 1) {
			fprintf(stderr, "scan failed on problem %d header\n", curprob);
			return -4;
		}
		if(index != curprob) {
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				index, curprob);
			return -5;
		}
		for(i = 0; i < 8 ; i++) {
			boards[0].rowsum[i] = boards[0].colsum[i] = 0;
			boards[0].rowfree[i] = boards[0].colfree[i] = 8;
		}
		for(i = 1; i <= 65 ; i++) {
			fixedx[i] = fixedy[i] = -1;
		}
		// read region data
		for(i = 0; i < 8 ; i++) {

			if(fgets(&(inbuf[0]), 255, stdin) == NULL)
			{
				fprintf(stderr, "Read failed on row %d of problem %d\n", i, curprob);
				return -6;
			}
			if(sscanf(&(inbuf[0]), "%d %d %d %d %d %d %d %d", 
				&(vals[0]), &(vals[1]), &(vals[2]), &(vals[3]),
				&(vals[4]), &(vals[5]), &(vals[6]), &(vals[7])) != 8) {
				fprintf(stderr, "scan failed on row %d of problem %d\n", i, curprob);
				return -7;
			}
			for(j = 0; j < 8 ; j++) {
				if((vals[j] != -1) && ((vals[j] < 1) || (vals[j] > 64))) {
					fprintf(stderr, "val at %d, %d = %d is out of range problem %d\n", i, j, vals[j], curprob);
					return -8;
				}
				boards[0].board[i][j] = (char)vals[j];
				if(vals[j] > 0) {
					boards[0].rowsum[i] += (WORD)vals[j];
					boards[0].colsum[j] += (WORD)vals[j];
					boards[0].rowfree[i]--;
					boards[0].colfree[j]--;
					fixedx[vals[j]] = j;
					fixedy[vals[j]] = i;
				}
			}
		}
		remsum[0] = 0;
		for(i = 64, j= 1, sum = 0; (i >= 1) && (j <= 8) ; i--) {
			if(fixedx[i] < 0) {
				sum += i;
				remsum[j] = sum;
				j++;
			}
		}
		if((fixedx[1] >= 0) && (fixedy[1] >= 0)) {
			i = 1;
			while((fixedx[i] >= 0) && (fixedy[i] >= 0)) {
				boards[0].curx = fixedx[i];
				boards[0].cury = fixedy[i];
				boards[0].curmove = i;
				i++;
			}
		} else {
			fprintf(stderr, "problem %d move 1 is not given\n", curprob);
			return -9;
		}
		maxlevel = 0;
		if((ret = SolveBoard(0)) >= 0) {
			printf("%d\n", curprob);
			for(i = 0; i < 8 ; i++) {
				for(j = 0; j < 8 ; j++) {
					printf("%2d ", boards[ret].board[i][j]);
				}
				printf("\n");
			}
		} else {
			printf("No solution for problem %d maxlevel %d\n", curprob, maxlevel);
		}
	}
	return 0;
}
