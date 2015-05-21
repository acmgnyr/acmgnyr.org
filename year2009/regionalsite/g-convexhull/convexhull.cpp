#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define PTS_PER_LINE	5
#define HASH_SIZE 23
#define MAX_POINTS 50
#define BIG_VALUE	1000000000

typedef struct _scan_line_
{
	struct _scan_line_ *pNext; // next line down
	int y;
	int xl;
	int xr;
} SCAN_LINE, *PSCAN_LINE;

typedef struct _vertex_
{
	struct _vertex_ *pNext;	// next in clockwise order
	struct _vertex_ *pPrev;	// next in counterclockwise order
	int x;
	int y;
} VERTEX, *PVERTEX;

typedef struct _edge_	// non-horizontal edge
{
	struct _edge_ *pNext;
	struct _edge_ *pPrev;
	int tx;
	int ty;
	int bx;
	int by;
	int dx;	// change in x and y along edge
	int dy;
} EDGE, *PEDGE;

PSCAN_LINE pFreeScans = NULL;
PVERTEX pFreeVerts = NULL;
PEDGE pFreeEdges = NULL;

PSCAN_LINE scanHash[HASH_SIZE];
PSCAN_LINE pPolyScans = NULL;	// interior of polygon
VERTEX inPoints[MAX_POINTS];	// input points to find hull of
PVERTEX pPeelVerts = NULL;		// vertices of convex hull of interior points
PEDGE pPolyLeft = NULL;			// left side input edges
PEDGE pPolyRight = NULL;		// right side input edges
PEDGE pPeelLeft = NULL;			// left side convex hull edges
PEDGE pPeelRight = NULL;		// right side convex hull edges

int topY;
int bottomY = 0;
int nVerts = 0;

// return greatest int <= num/denom
int TruncLeft(int num, int denom)
{
	int q, r;

	q = num/denom;
	r = num - q*denom;
	if(r > 0) q--;
	return q;
}

// return least int >= num/denom
int TruncRight(int num, int denom)
{
	int q, r;

	q = num/denom;
	r = num - q*denom;
	if(r < 0) q++;
	return q;
}

PSCAN_LINE GetNewScan(void)
{
	PSCAN_LINE pRet;
	if(pFreeScans != NULL)
	{
		pRet = pFreeScans;
		pFreeScans = pFreeScans->pNext;
	}
	else
	{
		pRet = new SCAN_LINE;
	}
	return pRet;
}

PVERTEX GetNewVert(void)
{
	PVERTEX pRet;
	if(pFreeVerts != NULL)
	{
		pRet = pFreeVerts;
		pFreeVerts = pFreeVerts->pNext;
	}
	else
	{
		pRet = new VERTEX;
	}
	return pRet;
}

PEDGE GetNewEdge(void)
{
	PEDGE pRet;
	if(pFreeEdges != NULL)
	{
		pRet = pFreeEdges;
		pFreeEdges = pFreeEdges->pNext;
	}
	else
	{
		pRet = new EDGE;
	}
	return pRet;
}

void ReleaseEdge(PEDGE pEdge)
{
	pEdge->pNext = pFreeEdges;
	pFreeEdges = pEdge;
}

void CleanLocal(void)
{
	PEDGE pCurE, pNxtE;
	PVERTEX pCurV, pNxtV;
	PSCAN_LINE pCurS, pNxtS;

	pCurS = pPolyScans;
	pPolyScans = NULL;
	while(pCurS != NULL)
	{
		pNxtS = pCurS->pNext;
		pCurS->pNext = pFreeScans;
		pFreeScans = pCurS;
		pCurS = pNxtS;
	}

	pCurV = pPeelVerts;
	pPeelVerts = NULL;
	while(pCurV != NULL)
	{
		pNxtV = pCurV->pNext;
		pCurV->pNext = pFreeVerts;
		pFreeVerts = pCurV;
		pCurV = pNxtV;
	}

	pCurE = pPolyLeft;
	pPolyLeft = NULL;
	while(pCurE != NULL)
	{
		pNxtE = pCurE->pNext;
		pCurE->pNext = pFreeEdges;
		pFreeEdges = pCurE;
		pCurE = pNxtE;
	}

	pCurE = pPolyRight;
	pPolyRight = NULL;
	while(pCurE != NULL)
	{
		pNxtE = pCurE->pNext;
		pCurE->pNext = pFreeEdges;
		pFreeEdges = pCurE;
		pCurE = pNxtE;
	}

	pCurE = pPeelLeft;
	pPeelLeft = NULL;
	while(pCurE != NULL)
	{
		pNxtE = pCurE->pNext;
		pCurE->pNext = pFreeEdges;
		pFreeEdges = pCurE;
		pCurE = pNxtE;
	}

	pCurE = pPeelRight;
	pPeelRight = NULL;
	while(pCurE != NULL)
	{
		pNxtE = pCurE->pNext;
		pCurE->pNext = pFreeEdges;
		pFreeEdges = pCurE;
		pCurE = pNxtE;
	}
}

void CleanGlobal(void)
{
	PEDGE pCurE, pNxtE;
	PVERTEX pCurV, pNxtV;
	PSCAN_LINE pCurS, pNxtS;

	CleanLocal();

	pCurS = pFreeScans;
	pFreeScans = NULL;
	while(pCurS != NULL)
	{
		pNxtS = pCurS->pNext;
		delete pCurS;
		pCurS = pNxtS;
	}

	pCurV = pFreeVerts;
	pFreeVerts = NULL;
	while(pCurV != NULL)
	{
		pNxtV = pCurV->pNext;
		delete pCurV;
		pCurV = pNxtV;
	}

	pCurE = pFreeEdges;
	pFreeEdges = NULL;
	while(pCurE != NULL)
	{
		pNxtE = pCurE->pNext;
		delete pCurE;
		pCurE = pNxtE;
	}

}

char inbuf[256];

int AddScanPoint(int x,int y, int index, int curprob)
{
	PSCAN_LINE pScan, pNew;
	int hindex;
	hindex = y % HASH_SIZE;
	pScan = scanHash[hindex];
	while(pScan != NULL)
	{
		if(pScan->y == y)
		{
			if(x < pScan->xl)
			{
				pScan->xl = x;
			}
			else if(x > pScan->xr)
			{
				pScan->xr = x;
			}
			return 0;
		}
		pScan = pScan->pNext;
	}
	// if we get here, need another scan
	if((pNew = GetNewScan()) == NULL)
	{
		fprintf(stderr, "Scan x %d y %d alloc failed on point %d curprob %d\n",
			x, y, index, curprob);
		return -60;
	}
	pNew->y = y;
	pNew->xl = pNew->xr = x;
	pNew->pNext = NULL;
	if((pScan = scanHash[hindex]) == NULL)
	{
		scanHash[hindex] = pNew;
	}
	else if(pScan->y < y)
	{
		pNew->pNext = pScan;
		scanHash[hindex] = pNew;
	}
	else
	{
		while((pScan->pNext != NULL) && (pScan->pNext->y > y))
		{
			pScan = pScan->pNext;
		}
		pNew->pNext = pScan->pNext;
		pScan->pNext = pNew;
	}
	return 0;
}

int ReadInput(int nPts, int curprob)
{
	int i, x, y, incnt, ret, isneg;
	char *pCur;

	incnt = PTS_PER_LINE;
	topY = -BIG_VALUE;
	bottomY = BIG_VALUE;
	for(i = 0; i < nPts ; i++)
	{
		if(incnt >= PTS_PER_LINE)
		{
			if(fgets(&(inbuf[0]), 255, stdin) == NULL)
			{
				fprintf(stderr, "Data read failed on problem %d point %d \n",
					curprob, i);
				return -10;
			}
			pCur = &(inbuf[0]);
			incnt = 0;
		}
		isneg = 0;
		// skip leading space
		while((*pCur != 0) && (isspace(*pCur)))
		{
			pCur++;
		}
		if(*pCur == '-')
		{
			isneg = 1;
			pCur++;
		}
		if(!isdigit(*pCur))
		{
			fprintf(stderr, "bad char x on problem %d point %d \n",
				curprob, i);
			return -11;
		}
		// read x
		x = 0;
		while((*pCur != 0) && (isdigit(*pCur)))
		{
			x = 10*x + *pCur - '0';
			pCur++;
		}
		if(isneg)
		{
			x = -x;
		}
		isneg = 0;
		// skip leading space
		while((*pCur != 0) && (isspace(*pCur)))
		{
			pCur++;
		}
		if(*pCur == '-')
		{
			isneg = 1;
			pCur++;
		}
		if(!isdigit(*pCur))
		{
			fprintf(stderr, "bad char x on problem %d point %d \n",
				curprob, i);
			return -11;
		}
		// read y
		y = 0;
		while((*pCur != 0) && (isdigit(*pCur)))
		{
			y = 10*y + *pCur - '0';
			pCur++;
		}
		if(isneg)
		{
			y = -y;
		}
		inPoints[i].x = x;
		inPoints[i].y = y;
		if(y > topY) topY = y;
		if(y < bottomY) bottomY = y;
		if((ret = AddScanPoint(x, y, i, curprob)) != 0)
		{
			return ret;
		}
		incnt++;
	}
	return 0;
}

int BuildScans(int nPts, int curprob)
{
	PSCAN_LINE pNew, pTail;
	int y, hindex;

	for(y = topY ; y >= bottomY ; y--)
	{
		hindex = y % HASH_SIZE;
		pNew= scanHash[hindex];
		if((pNew != NULL) && (pNew->y == y))
		{	// non-empty scna here
			scanHash[hindex] = pNew->pNext;	// remove from hash table
			pNew->pNext = NULL;
			if(pPolyScans == NULL)
			{
				pPolyScans = pTail = pNew;
			}
			else
			{
				pTail->pNext = pNew;
				pTail = pNew;
			}
		}
	}
	return 0;
}

int BuildHullEdges(int curprob)
{
	PSCAN_LINE pScan;
	PEDGE pLNew, pLTail, pRNew, pRTail;
	PVERTEX pLVert, pRVert;
	int nDX, nDY, lX, rX, lY, rY, dir;

	pScan = pPolyScans;
	pLTail = pRTail = NULL;
	nVerts = 0;
	if(pScan == NULL)
	{	// no points in interior
		return 0;
	}
	lY = rY = pScan->y;
	lX = pScan->xl;
	rX = pScan->xr;
	// scan the rest of lines
	pScan = pScan->pNext;
	while(pScan != NULL)
	{
		if(pScan->xl <= pScan->xr)
		{	// valid points
			if(pLTail != NULL)
			{	// if there is a previous edge
				nDX = pScan->xl - lX;
				nDY = pScan->y - lY;
				dir = -nDX *(pLTail->dy) + (pLTail->dx)*nDY;
				if(dir > 0)
				{	// bends right, add new edge
					pLNew = GetNewEdge();
					if(pLNew == NULL)
					{
						fprintf(stderr, "LEdge y %d alloc failed on problem %d\n",
							pScan->y, curprob);
						return -40;
					}
					pLNew->pNext = NULL;
					pLNew->tx = lX;
					pLNew->ty = lY;
					pLNew->bx = pScan->xl;
					pLNew->by = pScan->y;
					pLNew->dy = pLNew->by - pLNew->ty;
					pLNew->dx = pLNew->bx - pLNew->tx;
					pLNew->pPrev = pLTail;
					pLNew->pNext = NULL;
					pLTail->pNext = pLNew;
					pLTail = pLNew;
					lX = pLNew->bx;
					lY = pLNew->by;
				}
				else
				{	// left or straight, forget pTail bottom vert
					lX = pLTail->tx;
					lY = pLTail->ty;
					nDX = pScan->xl - lX;
					nDY = pScan->y - lY;
					pLTail->bx = pScan->xl;
					pLTail->by = pScan->y;
					pLTail->dx = nDX;
					pLTail->dy = nDY;
					// we may be able to go up further
					while(pLTail->pPrev != NULL)
					{
						dir = -nDX *(pLTail->pPrev->dy) + (pLTail->pPrev->dx)*nDY;
						if(dir <= 0)
						{
							pLNew = pLTail;
							pLTail = pLTail->pPrev;
							lX = pLTail->tx;
							lY = pLTail->ty;
							nDX = pScan->xl - lX;
							nDY = pScan->y - lY;
							pLTail->bx = pScan->xl;
							pLTail->by = pScan->y;
							pLTail->dx = nDX;
							pLTail->dy = nDY;
							pLTail->pNext = NULL;
							ReleaseEdge(pLNew);
						}
						else
						{
							break;
						}
					}
					lX = pLTail->bx;
					lY = pLTail->by;
				}

			}
			else
			{
				pLNew = GetNewEdge();
				if(pLNew == NULL)
				{
					fprintf(stderr, "LEdge y %d alloc failed on problem %d\n",
						pScan->y, curprob);
					return -41;
				}
				pLNew->pNext = NULL;
				pLNew->tx = lX;
				pLNew->ty = lY;
				pLNew->bx = pScan->xl;
				pLNew->by = pScan->y;
				pLNew->dy = pLNew->by - pLNew->ty;
				pLNew->dx = pLNew->bx - pLNew->tx;
				pPeelLeft = pLTail = pLNew;
				pLNew->pPrev = pLNew->pNext = NULL;
				lX = pLNew->bx;
				lY = pLNew->by;
			}

			if(pRTail != NULL)
			{	// if there is a previous edge
				nDX = pScan->xr - rX;
				nDY = pScan->y - rY;
				dir = -nDX *(pRTail->dy) + (pRTail->dx)*nDY;
				if(dir < 0)
				{	// bends left, add new edge
					pRNew = GetNewEdge();
					if(pRNew == NULL)
					{
						fprintf(stderr, "REdge y %d alloc failed on problem %d\n",
							pScan->y, curprob);
						return -42;
					}
					pRNew->pNext = NULL;
					pRNew->tx = rX;
					pRNew->ty = rY;
					pRNew->bx = pScan->xr;
					pRNew->by = pScan->y;
					pRNew->dy = pRNew->by - pRNew->ty;
					pRNew->dx = pRNew->bx - pRNew->tx;
					pRNew->pPrev = pRTail;
					pRNew->pNext = NULL;
					pRTail->pNext = pRNew;
					pRTail = pRNew;
					rX = pRNew->bx;
					rY = pRNew->by;
				}
				else
				{	// left or straight, forget pTail bottom vert
					rX = pRTail->tx;
					rY = pRTail->ty;
					nDX = pScan->xr - rX;
					nDY = pScan->y - rY;
					pRTail->bx = pScan->xr;
					pRTail->by = pScan->y;
					pRTail->dx = nDX;
					pRTail->dy = nDY;
					// we may be able to go up further
					while(pRTail->pPrev != NULL)
					{
						dir = -nDX *(pRTail->pPrev->dy) + (pRTail->pPrev->dx)*nDY;
						if(dir >= 0)
						{
							pRNew = pRTail;
							pRTail = pRTail->pPrev;
							rX = pRTail->tx;
							rY = pRTail->ty;
							nDX = pScan->xr - rX;
							nDY = pScan->y - rY;
							pRTail->bx = pScan->xr;
							pRTail->by = pScan->y;
							pRTail->dx = nDX;
							pRTail->dy = nDY;
							pRTail->pNext = NULL;
							ReleaseEdge(pRNew);
						}
						else
						{
							break;
						}
					}
					rX = pRTail->bx;
					rY = pRTail->by;
				}

			}
			else
			{
				pRNew = GetNewEdge();
				if(pRNew == NULL)
				{
					fprintf(stderr, "LEdge y %d alloc failed on problem %d\n",
						pScan->y, curprob);
					return -43;
				}
				pRNew->pNext = NULL;
				pRNew->tx = rX;
				pRNew->ty = rY;
				pRNew->bx = pScan->xr;
				pRNew->by = pScan->y;
				pRNew->dy = pRNew->by - pRNew->ty;
				pRNew->dx = pRNew->bx - pRNew->tx;
				pPeelRight = pRTail = pRNew;
				pRNew->pPrev = pRNew->pNext = NULL;
				rX = pRNew->bx;
				rY = pRNew->by;
			}

		}
		pScan = pScan->pNext;
	}
	if(pPeelLeft == NULL)
	{
		if(lX == rX)
		{
			pLVert = GetNewVert();
			if(pLVert == NULL)
			{
				fprintf(stderr, "LVert y %d alloc failed on problem %d\n",
					lY, curprob);
				return -44;
			}
			pPeelVerts = pLVert;
			pLVert->pNext = NULL;
			pLVert->pPrev = NULL;
			pLVert->x = lX;
			pLVert->y = lY;
			nVerts = 1;
		}
		else
		{
			pLVert = GetNewVert();
			if(pLVert == NULL)
			{
				fprintf(stderr, "LVert y %d alloc failed on problem %d\n",
					lY, curprob);
				return -45;
			}
			pPeelVerts = pLVert;
			pLVert->pNext = NULL;
			pLVert->pPrev = NULL;
			pLVert->x = lX;
			pLVert->y = lY;
			pRVert = GetNewVert();
			if(pRVert == NULL)
			{
				fprintf(stderr, "LVert y %d alloc failed on problem %d\n",
					lY, curprob);
				return -46;
			}
			pLVert->pNext = pRVert;
			pRVert->pNext = NULL;
			pRVert->x = rX;
			pRVert->y = rY;
			nVerts = 2;
		}
	}
	return 0;
}

int BuildHullVerts(int curprob)
{
	PVERTEX pNew, pTail;
	PEDGE pREdge, pLEdge;

	if(nVerts > 0)
	{	// already done 1 or 2 horz verts
		return 0;
	}
	if(pPeelLeft == NULL)
	{	// no points at all
		return 0;
	}
	pNew = GetNewVert();
	if(pNew == NULL)
	{
		fprintf(stderr, "Vert x %d y %d alloc failed on problem %d\n",
			pPeelLeft->tx, pPeelLeft->ty, curprob);
		return -50;
	}
	pPeelVerts = pTail = pNew;
	pNew->pPrev = pNew->pNext = NULL;
	pNew->x = pPeelLeft->tx;
	pNew->y = pPeelLeft->ty;
	nVerts = 1;
	if(pPeelRight->tx != pPeelLeft->tx)
	{	// first edge is horz
		pNew = GetNewVert();
		if(pNew == NULL)
		{
			fprintf(stderr, "Vert x %d y %d alloc failed on problem %d\n",
				pPeelRight->tx, pPeelRight->ty, curprob);
			return -51;
		}
		pNew->pNext = NULL;
		pNew->pPrev = pTail;
		pTail->pNext = pNew;
		pTail = pNew;
		pNew->x = pPeelRight->tx;
		pNew->y = pPeelRight->ty;
		nVerts = 2;
	}
	else if((pPeelLeft->pNext == NULL) && (pPeelRight->pNext == NULL) &&
		(pPeelLeft->bx == pPeelRight->bx) && (pPeelLeft->by == pPeelRight->by))
	{	// check for single nonhorz line
		pNew = GetNewVert();
		if(pNew == NULL)
		{
			fprintf(stderr, "Vert x %d y %d alloc failed on problem %d\n",
				pPeelRight->tx, pPeelRight->ty, curprob);
			return -52;
		}
		pNew->pNext = NULL;
		pNew->pPrev = pTail;
		pTail->pNext = pNew;
		pTail = pNew;
		pNew->pNext = NULL;
		pNew->x = pPeelRight->bx;
		pNew->y = pPeelRight->by;
		nVerts = 2;
		return 0;
	}
	// get to left bottom
	pLEdge = pPeelLeft;
	while(pLEdge->pNext != NULL)
	{
		pLEdge = pLEdge->pNext;
	}
	// we have to scan
	pREdge = pPeelRight;
	while(pREdge->pNext != NULL)
	{
		pNew = GetNewVert();
		if(pNew == NULL)
		{
			fprintf(stderr, "Vert x %d y %d alloc failed on problem %d\n",
				pREdge->bx, pREdge->by, curprob);
			return -52;
		}
		pNew->pNext = NULL;
		pNew->pPrev = pTail;
		pTail->pNext = pNew;
		pTail = pNew;
		pNew->x = pREdge->bx;
		pNew->y = pREdge->by;
		nVerts++;
		pREdge = pREdge->pNext;
	}
	// do bottom rt vertex
	pNew = GetNewVert();
	if(pNew == NULL)
	{
		fprintf(stderr, "Vert x %d y %d alloc failed on problem %d\n",
			pREdge->bx, pREdge->by, curprob);
		return -52;
	}
	pNew->pNext = NULL;
	pNew->pPrev = pTail;
	pTail->pNext = pNew;
	pTail = pNew;
	pNew->x = pREdge->bx;
	pNew->y = pREdge->by;
	nVerts++;
	// horz bottom??
	if(pLEdge->bx != pREdge->bx)
	{	// horz bottom edge
		pNew = GetNewVert();
		if(pNew == NULL)
		{
			fprintf(stderr, "Vert x %d y %d alloc failed on problem %d\n",
				pPeelRight->tx, pPeelRight->ty, curprob);
			return -52;
		}
		pNew->pNext = NULL;
		pNew->pPrev = pTail;
		pTail->pNext = pNew;
		pTail = pNew;
		pNew->x = pLEdge->bx;
		pNew->y = pLEdge->by;
		nVerts++;
	}
	while(pLEdge != pPeelLeft)
	{
		pNew = GetNewVert();
		if(pNew == NULL)
		{
			fprintf(stderr, "Vert x %d y %d alloc failed on problem %d\n",
				pPeelRight->tx, pPeelRight->ty, curprob);
			return -52;
		}
		pNew->pNext = NULL;
		pNew->pPrev = pTail;
		pTail->pNext = pNew;
		pTail = pNew;
		pNew->x = pLEdge->tx;
		pNew->y = pLEdge->ty;
		nVerts++;
		pLEdge = pLEdge->pPrev;
	}
	return 0;
}		

void PrintSolution(int curprob)
{
	PVERTEX pVert;
	printf("%d %d\n", curprob, nVerts);
	if(nVerts > 0)
	{
		pVert = pPeelVerts;
		while(pVert != NULL)
		{
			printf("%d %d\n", pVert->x, pVert->y);
			pVert = pVert->pNext;
		}
	}
}

int main()
{
	int nprob, curprob, probsz, ret, index, i;

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
	for(i = 0 ; i < HASH_SIZE ; i++)
	{
		scanHash[i] = NULL;
	}
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d size\n", curprob);
			CleanGlobal();
			return -3;
		}
		if(sscanf(&(inbuf[0]), "%d %d", &index, &probsz) != 2)
		{
			fprintf(stderr, "Scan failed on problem %d size\n", curprob);
			CleanGlobal();
			return -4;
		}
		if((probsz < 3) || (probsz > MAX_POINTS))
		{
			fprintf(stderr, "problem size %d is < 3 or > %d in problem %d\n",
				probsz, MAX_POINTS, curprob);
			CleanGlobal();
			return -5;
		}
		if(index != curprob)
		{
			fprintf(stderr, "problem index %d not = expected problem %d\n", index, curprob);
			CleanGlobal();
			return -7;
		}
		if((ret = ReadInput(probsz, curprob)) != 0)
		{
			CleanGlobal();
			return ret;
		}
		if((ret = BuildScans(probsz, curprob)) != 0)
		{
			CleanGlobal();
			return ret;
		}
		if((ret = BuildHullEdges(curprob)) != 0)
		{
			CleanGlobal();
			return ret;
		}
		if((ret = BuildHullVerts(curprob)) != 0)
		{
			CleanGlobal();
			return ret;
		}
		PrintSolution(curprob);
		CleanLocal();
	}
	CleanGlobal();
	return 0;
}