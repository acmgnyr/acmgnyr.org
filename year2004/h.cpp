#include <stdio.h>

#undef MAKE_DATA

#define MAX_ROWS	(47)
#define MAX_COLS	(63)
#define MAX_COMPS	(MAX_ROWS * MAX_COLS)
#define MAX_PTS		(2*MAX_ROWS * MAX_COLS)
#define CODE_MSK	(0x07)
#define OFF_PIXEL	('.')
#define ON_PIXEL	('X')
#define DIRS_PER_LINE	40

struct SComponent
{
	int StartX;
	int StartY;
	SComponent *pParent;
	int compID;
	int pixelCnt;
};

FILE *fpi, *fpo;

int region[MAX_ROWS+2][MAX_COLS+2];
SComponent comps[MAX_COMPS];	// cannot be more comp structs than this
int nNxtComponent, nNxtOutComp;
int edgePts[MAX_PTS];	// values for direction codes
char *dirCodes = "ABCDEFGH";	// letter codes for directions
// first possible next code if lastcode was i
int startCode[8] = {7, 7, 1, 1, 3, 3, 5, 5};
// last possible next code if lastcode was i (+ 8 )
int  endCode[8] = {12, 12, 6, 6, 8, 8, 10, 10};
// step to next x, y for code
int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};

void DumpRegion(int nrows, int ncols, char *label)
{
	int i, j;
	fprintf(fpo, "\n%s\n", label);
	for(i = 1; i <= nrows ; i++)
	{
		for(j = 1; j <= ncols ; j++)
		{
			fprintf(fpo, "%2d", region[i][j]);
		}
		fprintf(fpo, "\n");
	}
	fprintf(fpo, "\n");
}

void GetCurID(SComponent *pComp)
{
	int id;
	SComponent *pTop, *pCur;

	if(pComp->pParent != NULL)
	{
		pTop = pComp;
		while(pTop->pParent != NULL)
		{
			pTop = pTop->pParent;
		}
		id = pTop->compID;
		// set everyone in the chain to point to top and have this id
		pCur = pComp;
		while(pCur != pTop)
		{
			pCur->pParent = pTop;
			pCur->compID = id;
			pCur = pCur->pParent;
		}
	}
}

void ZeroRegion()
{
	int i, j;

	nNxtComponent = 1;
	for(i = 0; i <= (MAX_ROWS+1) ; i++)
	{
		for(j = 0; j <= (MAX_COLS+1) ; j++)
		{
			region[i][j] = 0;
		}
	}
}

int ProcessInRowData(char *data, int rowNum, int nCols)
{
	int j, col;
	SComponent *pComp, *pUp, *pLeft;

	for(j = 0; j < nCols ; j++)
	{
		if(data[j] == OFF_PIXEL)
		{
			continue;
		}
		else if(data[j] == ON_PIXEL)
		{
			col = j+1;
			if(region[rowNum-1][col] != 0)
			{	// adjacent to pixel above
				pUp = &(comps[region[rowNum-1][col]]);
				GetCurID(pUp);
				if(region[rowNum][col-1] != 0)
				{
					pLeft = &(comps[region[rowNum][col-1]]);
					GetCurID(pLeft);
					if(pLeft->compID != pUp->compID)
					{	// combine components
						if(pLeft->compID < pUp->compID)
						{	// left is earlier
							pUp->pParent = pLeft;
							GetCurID(pUp);
						}
						else
						{
							pLeft->pParent = pUp;
							GetCurID(pUp);
						}
					}
				}
				region[rowNum][col] = pUp->compID;
				pUp->pixelCnt++;
			}
			else if(region[rowNum][col-1] != 0)
			{	// adjacent to pixel on theleft
				pLeft = &(comps[region[rowNum][col-1]]);
				GetCurID(pLeft);
				region[rowNum][col] = pLeft->compID;
				pLeft->pixelCnt++;
			}
			else
			{	// not adjacent to any previous pixel, new component
				if(nNxtComponent >= MAX_COMPS)
				{
					fprintf(stderr, "Out of componenets row %d col %d\n",
						rowNum, col);
					return -2;
				}
				pComp = &comps[nNxtComponent];
				pComp->compID = nNxtComponent;
				nNxtComponent++;
				pComp->pixelCnt = 1;
				pComp->pParent = NULL;
				pComp->StartX = col;
				pComp->StartY = rowNum;
				region[rowNum][col] = pComp->compID;
			}
		}
		else
		{
			fprintf(stderr, "bad char value 0x%x col %d row %d\n", data[j], j+1, rowNum);
			return -1;
		}
	}
	return 0;
}

int CheckPointCode(int x, int y, int comp, int newCode)
{
	int nx, ny, tx, ty, tcode;

	nx = x + dx[newCode];
	ny = y + dy[newCode];
	if((newCode & 1) != 0)
	{	// diagonal, check straight first
		tcode = (newCode + 1) & CODE_MSK;
		tx = x + dx[tcode];
		ty = y + dy[tcode];
		if(region[ty][tx] != comp)
		{	// cannot get there
			return 0;
		}
	}
	if(region[ny][nx] == comp)
	{
		return 1;
	}
	return 0;
}

int ProcessComponent(SComponent *pComp, int nrows, int ncols)
{
	int x, y, sx, sy, comp, lastCode, npts, i, newCode;
	bool bDone, bLastCR;

	x = sx = pComp->StartX;
	y = sy = pComp->StartY;
	npts = 0;
	comp = pComp->compID;
	// can only go rt, down or down
	if(region[y][x+1] == comp)
	{	// move right
		edgePts[npts] = 2;
		lastCode = 2;
		npts++;
		x = x+1;
	}
	else if(region[y+1][x] == comp)
	{
		if(region[y+1][x+1] == comp)
		{	// down rt
			edgePts[npts++] = 3;
			lastCode = 3;
			x = x+1;
			y = y+1;
		}
		else
		{	// down
			edgePts[npts++] = 4;
			lastCode = 4;
			y = y+1;
		}
	}
	else
	{
		fprintf(stderr, "bad start pt (%d, %d)\n", sx, sy);
		return -1;
	}
	while((npts < MAX_PTS) && ((x != sx) || (y != sy)))
	{
		bDone = false;
		for(i = startCode[lastCode] ; i <= endCode[lastCode] ; i++)
		{
			newCode = i & CODE_MSK;
			if(CheckPointCode(x, y, comp, newCode))
			{
				bDone = true;
				edgePts[npts++] = newCode;
				lastCode = newCode;
				x += dx[newCode];
				y += dy[newCode];
				break;
			}
		}
		if(!bDone)
		{
			fprintf(stderr, "No next pt at (%d, %d) (start (%d, %d)\n", x, y, sx, sy);
			return -2;
		}
	}
	if((x != sx) || (y != sy))
	{
		fprintf(stderr, "No end of boundary starting at (%d, %d)\n", sx, sy);
		return -3;
	}
	// output start pt and count
#ifdef MAKE_DATA
	printf("%c %d %d %d\n", 'A' + nNxtOutComp, sy, sx, npts);
	nNxtOutComp++;
#else
	fprintf(fpo, "%d %d %d\n", sy, sx, npts);
#endif
	for(i = 0; i < npts; i++)
	{
		bLastCR = false;
		fputc(dirCodes[edgePts[i]], fpo);
		if((i % DIRS_PER_LINE) == (DIRS_PER_LINE - 1))
		{
			fprintf(fpo, "\n");
			bLastCR = true;
		}
	}
	// add spacing line
	if(!bLastCR)
	{
		fprintf(fpo, "\n");
	}
	return 0;
}
int ProcessComponents(int nrows, int ncols, int minPixels)
{
	int i, ncomp, j, k;
	SComponent *pComp;

	ncomp = 0;
	for(i = 1, pComp = &(comps[1]); i < nNxtComponent ; i++, pComp++)
	{
		GetCurID(pComp);
		// add in pixels with this id if not parent
		if(pComp->pParent != NULL)
		{
			pComp->pParent->pixelCnt += pComp->pixelCnt;
		}
	}
	// count valid components
	for(i = 1, pComp = &(comps[1]); i < nNxtComponent ; i++, pComp++)
	{
		if((pComp->pParent == NULL) && (pComp->pixelCnt >= minPixels))
		{
			ncomp++;
			if(pComp->compID != i)
			{
				fprintf(stderr, "comp %d != compid %d\n", i, pComp->compID);
				return -1;
			}
		}
	}
//	DumpRegion(nrows, ncols, "before change");
	// change pixels to correct component id
	for(i = 1; i <= nrows ; i++)
	{
		for(j = 1; j <= ncols ; j++)
		{
			if((k = region[i][j]) != 0)
			{
				region[i][j] = comps[k].compID;
			}
		}
	}
//	DumpRegion(nrows, ncols, "after change");
	// output number of components
#ifdef MAKE_DATA
	printf("%d %d %d\n", nrows, ncols, ncomp);
#else
	fprintf(fpo, "%d\n", ncomp);
#endif
	// output each component
	for(i = 1, pComp = &(comps[1]); i < nNxtComponent ; i++, pComp++)
	{
		if((pComp->pParent == NULL) && (pComp->pixelCnt >= minPixels))
		{
			if(ProcessComponent(pComp, nrows, ncols))
			{
				return -2;
			}
		}
	}

	return 0;
}
char inbuf[256];

int main()
{
	int nrows, ncols, nprob, i, minpix;

	fpi = fopen("h.in", "rt");
	if(fpi == NULL){
		fprintf(stderr, "Can't open input h.in\n");
		return 1;
	}
	fpo = stdout;
	if(fpo == NULL){
		fprintf(stderr, "Can't open output h.out\n");
		return 2;
	}

	nprob = 0;
	for(;;)
	{
		nprob++;
		if(fgets(&(inbuf[0]), 255, fpi) == NULL)
		{
			fprintf(stderr, "Unexpected end of file on region size, problem %d\n", nprob);
			return 1;
		}
		if(sscanf(&(inbuf[0]), "%d %d %d", &nrows, &ncols, &minpix) != 3)
		{
			fprintf(stderr, "scan of dimensions failed problem %d\n", nprob);
			return 2;
		}
#ifdef MAKE_DATA
		minpix = 2;	// force all out
		nNxtOutComp = 0;
#endif
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
		if((nrows == 0) || (ncols == 0))
		{	// end of data
#ifdef MAKE_DATA
	printf("0 0 0\n");	// termination code
#endif
			return 0;
		}
		ZeroRegion();
		// read and process each row of pixels
		for(i = 1 ; i <= nrows ; i++)
		{
			if(fgets(&(inbuf[0]), 255, fpi) == NULL)
			{
				fprintf(stderr, "Unexpected end of file on row %d, problem %d\n", i, nprob);
				return 5;
			}
			if(ProcessInRowData(&(inbuf[0]), i, ncols))
			{
				return 6;
			}
		}
		if(ProcessComponents(nrows, ncols, minpix))
		{
			return 7;
		}
	}
	::fclose(fpi);
//	::fclose(fpo);

	return 0;
}
