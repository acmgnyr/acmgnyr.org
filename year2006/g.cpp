/*
 * ACM2006 Greater NY Region C++ Solution to G - Triangular N-Queens
 *
 * Fred Pickel
 * October, 2006
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define MAX_SIZE	1000
#define MAX_SOLN	750
int soln_row[MAX_SOLN], soln_col[MAX_SOLN];

char inbuf[256];

// current board stored as n^2 array only col <= row used,
// 0 avail, 1 attacked, 2 occupied

// put queen at row, col (1 based)
// return -2, col > row
// return -1 if attacked or ocupied
// else return 0
int AddQ(int row, int col, int size, char *pBoard)
{
	int index;

	if(col > row)
	{
		return -2;
	}
	index = (row - 1)*size + (col -1);
	if(pBoard[index] != 0)
	{
		return -1;
	}
	pBoard[index] = 2;
	// go thru attacked locations, set new ones attacke
	// row
	for(index = (row-1)*size; index < (row-1)*size + row ; index++)
	{
		if(pBoard[index] == 0)
		{
			pBoard[index] = 1;
		}
	}
	// file parallel to left edge
	for(index = (col - 1) * (size + 1); index < size * size ; index += size)
	{
		if(pBoard[index] == 0)
		{
			pBoard[index] = 1;
		}
	}
	// file parallel to right edge
	for(index = (row - col) * size; index < size * size ; index += (size + 1))
	{
		if(pBoard[index] == 0)
		{
			pBoard[index] = 1;
		}
	}
	return 0;
}

// only try locations after lastrow, lastcol
// return number of additonal queens added
int FindSolnRec(char *pBoard, int lastrow, int lastcol, int level, int size, int expsz)
{
	char *pBdTest;
	int trow[MAX_SOLN], tcol[MAX_SOLN];
	int row, col, startcol, bestret, index, ret;

	if(expsz == 0)
	{
		return 0;
	}
	pBdTest = new char[size*size];
	bestret = 0;
	for(row = lastrow; row <= size; row++)
	{
		if(row == lastrow)
		{
			startcol = lastcol + 1;
		}
		else
		{
			startcol = 1;
		}
		for(col = startcol; col <= row ; col++)
		{
			index = (row-1)*size + (col-1);
			if(pBoard[index] == 0)
			{	// possible candidate
				::memcpy(pBdTest, pBoard, size*size*sizeof(char));
				if(AddQ(row, col, size, pBdTest) == 0)
				{
					soln_row[level] = row;
					soln_col[level] = col;
					ret = FindSolnRec(pBdTest, row, col, level+1, size, expsz - 1);
					if(ret == (expsz - 1))
					{
						delete [] pBdTest;
						return expsz;
					}
					else if((ret+1) > bestret)
					{
						bestret = (ret+1);
						::memcpy(&(trow[0]), &(soln_row[0]), MAX_SOLN*sizeof(int));
						::memcpy(&(tcol[0]), &(soln_col[0]), MAX_SOLN*sizeof(int));
					}
				}
			}
		}
	}
	delete [] pBdTest;
	if(bestret > 0)
	{
		::memcpy(&(soln_row[0]), &(trow[0]), MAX_SOLN*sizeof(int));
		::memcpy(&(soln_col[0]), &(tcol[0]), MAX_SOLN*sizeof(int));
	}
	return bestret;
}

int FindSoln(int size)
{
	int expsz = (2*size + 1)/3;
	int row, col, ret, bestret;
	int trow[MAX_SOLN], tcol[MAX_SOLN];
	char *pBoard;

	if((size <= 0) || (size > MAX_SIZE))
	{
		return 0;
	}
	else if(size <= 2)
	{
		soln_row[0] = soln_col[0] = 1;
		return 1;
	}
	else if(size == 3)
	{
		soln_row[0] = soln_col[0] = 1;
		soln_row[1] = 3;
		soln_col[1] = 2;
		return 2;
	}
	pBoard = new char[size*size];
	bestret = 0;
	for(row = 1; row <= (size/2) + 1 ; row++)	// symmetry
	{
		for(col = 1; col <= row ; col++)
		{
			::memset(pBoard, 0, size*size*sizeof(char));
			soln_row[0] = row;
			soln_col[0] = col;
			if(AddQ(row, col, size, pBoard) == 0)
			{
				ret = FindSolnRec(pBoard, row, col, 1, size, expsz-1);
				if(ret == (expsz - 1))
				{	// done
					delete [] pBoard;
					return expsz;
				}
				else if((ret+1) > bestret)
				{	// remember best
						bestret = (ret+1);
					::memcpy(&(trow[0]), &(soln_row[0]), MAX_SOLN*sizeof(int));
					::memcpy(&(tcol[0]), &(soln_col[0]), MAX_SOLN*sizeof(int));
				}
			}
		}
	}
	delete [] pBoard;
	// if we get here, soimething is screwed return what we have
	if(bestret > 0)
	{
		::memcpy(&(soln_row[0]), &(trow[0]), MAX_SOLN*sizeof(int));
		::memcpy(&(soln_col[0]), &(tcol[0]), MAX_SOLN*sizeof(int));
	}
	return bestret;
}

int FindSolnKnight(int size)
{
	int expsz = (2*size + 1)/3;
	int row, crow, col, index, ind, bestret, level;
	int trow[MAX_SOLN], tcol[MAX_SOLN];
	char *pBoard;

	bestret = 0;
	if((size <= 0) || (size > MAX_SIZE))
	{
		return 0;
	}
	else if(size <= 2)
	{
		soln_row[0] = soln_col[0] = 1;
		return 1;
	}
	else if(size == 3)
	{
		soln_row[0] = soln_col[0] = 1;
		soln_row[1] = 3;
		soln_col[1] = 2;
		return 2;
	}
	pBoard = new char[size*size];

	for(row = (size - expsz + 1); row >= 1 ; row--)	// last row to start and get expsz
	{
		level = 0;
		::memset(pBoard, 0, size*size*sizeof(char));
		crow = row;
		col = 1;
		while(crow <= size)
		{
			soln_row[level] = crow;
			soln_col[level] = col;
			if(AddQ(crow, col, size, pBoard) == 0)
			{
				level++;
			}
			crow++;
			col += 2;
			if(col > crow)
			{
				col++;
				index = (crow - 1)*size;
				for(ind = 1; ind <= crow ; ind++, index++)
				{
					if(pBoard[index] == 0)
					{
						col = ind;
						break;
					}
				}
			}
			if(level == expsz)
			{
				delete [] pBoard;
				return expsz;
			}
		}
		if(level > bestret)
		{	// remember best
			bestret = level;
			::memcpy(&(trow[0]), &(soln_row[0]), MAX_SOLN*sizeof(int));
			::memcpy(&(tcol[0]), &(soln_col[0]), MAX_SOLN*sizeof(int));
		}
	}
	delete [] pBoard;
	// if we get here, soimething is screwed return what we have
	if(bestret > 0)
	{
		::memcpy(&(soln_row[0]), &(trow[0]), MAX_SOLN*sizeof(int));
		::memcpy(&(soln_col[0]), &(tcol[0]), MAX_SOLN*sizeof(int));
	}
	return bestret;
}

void PrintSoln(int probnum, int size, int solnSz)
{
	int i, lastnl;
	printf("%d %d %d\n", probnum, size, solnSz);
	lastnl = 1;
	for(i = 0; i < solnSz; i++)
	{
		printf("[%d,%d]", soln_row[i], soln_col[i]);
		if((i % 8) == 7)
		{
			printf("\n");
			lastnl = 1;
		}
		else
		{
			printf(" ");
			lastnl = 0;
		}
	}
	if(lastnl)
	{
		printf("\n");
	}
	else
	{
		printf("\n\n");
	}
}

int main()
{
	int size, i, n, solnsz;
	char ibuf[256];

	if(fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
		return 1;
	}
	n = atoi(&(ibuf[0]));

	for(i = 1; i <= n; i++){
		if(fgets(&(inbuf[0]), 255, stdin) == NULL){
			fprintf(stderr, "unexpected end of file after problem number %d\n", i);
			break;
		}
		if(sscanf(&(inbuf[0]), "%d", &size) != 1)
		{
			fprintf(stderr, "scanfailed on problem number %d\n", i);
			return -1;
		}
		if(size == 0)
		{
			return 0;
		}
		if((size < 0) || (size > MAX_SIZE))
		{
			fprintf(stderr, "bad size %d on problem number %d\n", size, i);
			return -2;
		}
		solnsz = FindSolnKnight(size);
		PrintSoln(i, size, solnsz);
	}
	return 0;
}
