#include <stdio.h>

#undef MAKE_DATA

#define MAX_ROWS	(47)
#define MAX_COLS	(63)
#define MAX_REGIONS	(96)
#define MAX_COMPS	(MAX_ROWS * MAX_COLS)
#define MAX_PTS		(2*MAX_ROWS * MAX_COLS)
#define CODE_MSK	(0x07)
#define OFF_PIXEL	('.')
#define ON_PIXEL	('X')
#define DIRS_PER_LINE	(40)
#define EXTRA	(64)

FILE *fpi, *fpo;

struct SRegion
{
	int StartX;
	int StartY;
	int xl;
	int xr;
	int yt;
	int yb;
	int ptcnt;
	int rbitmap[MAX_ROWS+2][MAX_COLS+2];
	char fillChar;
};

int bitmap[MAX_ROWS+2][MAX_COLS+2];
char fillChars[MAX_REGIONS + 1];
char *dirCodes = "ABCDEFGH";	// letter codes for directions
// first possible next code if lastcode was i
int startCode[8] = {7, 7, 1, 1, 3, 3, 5, 5};
// last possible next code if lastcode was i (+ 8 )
int  endCode[8] = {12, 12, 6, 6, 8, 8, 10, 10};
// step to next x, y for code
int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
int *directions = NULL;
int ndirections = 0;

SRegion region;

void DumpBitmp(int nrows, int ncols, char *label)
{
	int i, j;
	fprintf(fpo, "\n%s\n", label);
	for(i = 1; i <= nrows ; i++)
	{
		for(j = 1; j <= ncols ; j++)
		{
			fprintf(fpo, "%2d", bitmap[i][j]);
		}
		fprintf(fpo, "\n");
	}
	fprintf(fpo, "\n");
}


void ZeroBitmap()
{
	int i, j;

	for(i = 0; i <= (MAX_ROWS+1) ; i++)
	{
		for(j = 0; j <= (MAX_COLS+1) ; j++)
		{
			bitmap[i][j] = 0;
		}
	}
}

int SetupRegion(int x, int y, int ptcnt, int nprob, int nregion, char fillChar)
{
	int i, j;

	region.xl = region.xr = region.StartX = x;
	region.yt = region.yb = region.StartY = y;
	region.ptcnt = ptcnt;
	region.fillChar = fillChar;
	for(i = 0; i <= (MAX_ROWS+1) ; i++)
	{
		for(j = 0; j <= (MAX_COLS+1) ; j++)
		{
			region.rbitmap[i][j] = 0;
		}
	}
	if(ptcnt > ndirections)
	{
		if(directions != NULL)
		{
			delete [] directions;
		}
		directions = new int[ptcnt + EXTRA];
		if(directions == NULL)
		{
			fprintf(stderr, "Alloc of %d directions failed problem %d region %d\n",
				ptcnt + EXTRA, nprob, nregion);
			return -1;
		}
		ndirections = ptcnt + EXTRA;
	}
	return 0;
}

int ScanDirections(int nrow, int ncol, int nprob, int nregion)
{
	int x, y, i;
	x = region.StartX;
	y = region.StartY;
	for(i = 0; i < region.ptcnt ; i++)
	{
		if((x < 1) || (x > ncol) || (y < 1) || (y > nrow))
		{
			fprintf(fpo, "REGION %c GOES OUTSIDE THE ARRAY\n", region.fillChar);
			return -1;
		}
		if(bitmap[y][x] != 0)
		{
			fprintf(fpo, "REGION %c BOUNDARY INTERSECTS REGION %c\n", region.fillChar, fillChars[bitmap[y][x]]);
			return -2;
		}
		x += dx[directions[i]];
		y += dy[directions[i]];
		if(x > region.xr) region.xr = x;
		if(x < region.xl) region.xl = x;
		if(y > region.yb) region.yb = y;
		if(y < region.yt) region.yt = y;
	}
	if((x != region.StartX) || (y != region.StartY))
	{
		fprintf(fpo, "REGION %c BOUNDARY IS NOT CLOSED\n", region.fillChar);
		return -3;
	}
	return 0;
}

// this basically uses the even odd rule to fill
// by xor-ing the line to the right of the boundary pt
// the tricky part is to not do it if you are moving
// horizontally unless the next step reverses the previous vertical move
int FillRegion(int nprob, int nregion)
{
	int x, y, i, xr, j, lastDy, nextDy, nextDx, prevRealDy;
	x = region.StartX;
	y = region.StartY;
	xr = region.xr;
	lastDy = dy[directions[region.ptcnt - 1]];
	// find lastreal dy
	prevRealDy = 0;
	for(i = region.ptcnt - 1; i >= 0 ; i--)
	{
		if(dy[directions[i]] != 0)
		{
			prevRealDy = dy[directions[i]];
			break;
		}
	}
	if(prevRealDy != 0)
	{	// if no vert moves, its a horz line and the outline bit will cover it
		for(i = 0; i < region.ptcnt ; i++)
		{
			nextDx = dx[directions[i]];
			nextDy = dy[directions[i]];
			// only do the xor lastdy != 0 and not reverse vert dir
			// or nextdy != 0 and and last dy == 0 && it is reverse dir
			if(((lastDy != 0) && ((lastDy * nextDy) >= 0)) ||	// last vert and nott reverse
				((lastDy == 0) && (nextDy != 0) && ((prevRealDy * nextDy) < 0)))	// not reverse vert direction
			{
				for(j = x ; j <= xr; j++)
				{
					region.rbitmap[y][j] ^= 1;
				}
			}
			x += nextDx;
			y += nextDy;
			lastDy = nextDy;
			if(lastDy != 0)
			{
				prevRealDy = lastDy;
			}
		}
	}
	// that leaves out the right bdy  and horz excursions
	// so go back and make sure the boundary is set
	for(i = 0; i < region.ptcnt ; i++)
	{
		region.rbitmap[y][x] = 1;
		x += dx[directions[i]];
		y += dy[directions[i]];
	}
	// copy this rgion into the main bitmap
	for(i = region.yt ; i <= region.yb ; i++)
	{
		for(j = region.xl; j <= region.xr ; j++)
		{
			if(region.rbitmap[i][j] == 1)
			{
				bitmap[i][j] = nregion;
			}
		}
	}
	return 0;
}

char inbuf[256];
char outbuf[MAX_COLS + 4];

int ReadDirections(int ptcnt, int nprob, int nregion)
{
	int curdir, linecnt, i, curline;

	if(ptcnt > ndirections)
	{
		fprintf(stderr, "ReadDirections: ptcnt %d > ndirections %d problem %d region %d\n",
			ptcnt, ndirections, nprob, nregion);
		return -1;
	}
	curdir = curline = 0;
	while(ptcnt > 0)
	{
		curline++;
		linecnt = ptcnt;
		if(linecnt > DIRS_PER_LINE) linecnt = DIRS_PER_LINE;
		if(fgets(&(inbuf[0]), 255, fpi) == NULL)
		{
			fprintf(stderr, "Unexpected end of file on direction codes line %d problem %d region %d\n",
				curline, nprob, nregion);
			return -2;
		}
		for(i = 0; i < linecnt; i++)
		{
			if((inbuf[i] < 'A') || (inbuf[i] > 'H'))
			{
				fprintf(stderr, "bad dir code char 0x%x char %d line %d problem %d region %d\n",
					inbuf[i], i, curline, nprob, nregion);
				return -3;
			}
			directions[curdir++] = inbuf[i] - 'A';
		}
		ptcnt -= linecnt;
	}
	return 0;
}

void PrintBitmap(int nrows, int ncols)
{
	int i, j;
#ifdef MAKE_DATA
	printf("%d %d 2\n", nrows, ncols);
#endif
	for(i = 1; i <= nrows ; i++)
	{
		for(j = 1; j <= ncols ; j++)
		{
			outbuf[j] = fillChars[bitmap[i][j]];
		}
		outbuf[ncols+1] = 0;
		fprintf(fpo, "%s\n", &(outbuf[1]));
	}
#ifndef MAKE_DATA
	fprintf(fpo, "\n");
#endif
}

int main()
{
	int nrows, ncols, nprob, i, nregions, startX, startY, ptCnt;

	fpi = fopen("g.in", "rt");
	if(fpi == NULL){
		fprintf(stderr, "Can't open input g.in\n");
		return 1;
	}
	fpo = stdout;
	if(fpo == NULL){
		fprintf(stderr, "Can't open output g.out\n");
		return 2;
	}

	nprob = 0;
	directions = new int[MAX_PTS];
	if(directions == NULL)
	{
		fprintf(stderr, "alloc of %d directions failed\n", MAX_PTS);
		return -1;
	}
	ndirections = MAX_PTS;
	for(;;)
	{
		nprob++;
		if(fgets(&(inbuf[0]), 255, fpi) == NULL)
		{
			fprintf(stderr, "Unexpected end of file on region size, problem %d\n", nprob);
			return 1;
		}
		if(sscanf(&(inbuf[0]), "%d %d %d", &nrows, &ncols, &nregions) != 3)
		{
			fprintf(stderr, "scan of dimensions failed problem %d\n", nprob);
			return 2;
		}
		if(nrows > MAX_ROWS)
		{
			fprintf(stderr, "problem %d: row count %d > max %d\n",
				nprob, nrows, MAX_ROWS);
			return 3;
		}
		if(ncols > MAX_COLS)
		{
			fprintf(stderr, "problem %d: col count %d > max %d\n",
				nprob, ncols, MAX_COLS);
			return 4;
		}
		if(nregions > MAX_REGIONS)
		{
			fprintf(stderr, "problem %d: region count %d > max %d\n",
				nprob, nregions, MAX_REGIONS);
			return 5;
		}
		if((nrows == 0) || (ncols == 0))
		{	// end of data
#ifdef MAKE_DATA
		printf("0 0 0\n");
#endif
			return 0;
		}
		ZeroBitmap();
		fillChars[0] = OFF_PIXEL;
		// read and process each region to be filled
		for(i = 1 ; i <= nregions ; i++)
		{
			if(fgets(&(inbuf[0]), 255, fpi) == NULL)
			{
				fprintf(stderr, "Unexpected end of file on row %d, problem %d\n", i, nprob);
				return 6;
			}
#ifdef MAKE_DATA
			fillChars[i] = 'X';
#else
			fillChars[i] = inbuf[0];
#endif
			if(sscanf(&(inbuf[1]), "%d %d %d", &startY, &startX, &ptCnt) != 3)
			{
				fprintf(stderr, "scan of start pt and cnt failed problem %d region %d\n",
					nprob, i);
				return 7;
			}
			if(SetupRegion(startX, startY, ptCnt, nprob, i, inbuf[0]))
			{
				return 8;
			}
			if(ReadDirections(ptCnt, nprob, i))
			{
				return 9;
			}
			if(ScanDirections(nrows, ncols, nprob, i) == 0)
			{	// scans ok fill into bitmap
				FillRegion(nprob, i);
			}
		}
		PrintBitmap(nrows, ncols);
	}
	::fclose(fpi);
	// ::fclose(fpo);

	return 0;
}
