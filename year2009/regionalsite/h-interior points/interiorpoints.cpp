#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define NUMS_PER_LINE	10

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

PSCAN_LINE pPolyScans = NULL;	// interior of polygon
PVERTEX pPolyVerts = NULL;		// vertices of input polygon
PEDGE pPolyLeft = NULL;			// left side input edges
PEDGE pPolyRight = NULL;		// right side input edges

int bottomY = 0;
int nVerts = 0;
int nNonEmptyScans;

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

	pCurV = pPolyVerts;
	if((pPolyVerts != NULL) && (pPolyVerts->pPrev != NULL))
	{
		pPolyVerts->pPrev->pNext = NULL;	// get rid of circularity
	}
	pPolyVerts = NULL;
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

int ReadInput(int nPts, int curprob)
{
	int i, x, y;
	PVERTEX pNew, pTail;

	for(i = 0; i < nPts ; i++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Data read failed on problem %d point %d \n",
				curprob, i);
			return -10;
		}
		if(sscanf(&(inbuf[0]), "%d %d", &x, &y) != 2)
		{
			fprintf(stderr, "Scan failed on problem %d point %d \n",
				curprob, i);
			return -11;
		}
		pNew = GetNewVert();
		if(pNew == NULL)
		{
			fprintf(stderr, "Vertex alloc failed on problem %d point %d \n",
				curprob, i);
			return -12;
		}
		pNew->x = x;
		pNew->y = y;
		pNew->pNext = NULL;
		if(pPolyVerts == NULL)
		{
			pNew->pPrev = NULL;
			pPolyVerts = pTail = pNew;
		}
		else
		{
			pNew->pPrev = pTail;
			pTail->pNext = pNew;
			pTail = pNew;
		}
	}
	pPolyVerts->pPrev = pTail;
	pTail->pNext = pPolyVerts;
	return 0;
}

int BuildEdges(int curprob)
{
	PVERTEX pVert;
	PEDGE pNew, pTail;
	// assume pPolyVerts is top left do right side first
	pVert = pPolyVerts;
	if(pVert->y == pVert->pNext->y)
	{	// horz top, move right
		pVert = pVert->pNext;
	}
	while(pVert->y > pVert->pNext->y)
	{
		pNew = GetNewEdge();
		if(pNew == NULL)
		{
			fprintf(stderr, "Rt Poly Edge y's %d %d alloc failed on problem %d\n",
				pVert->y, pVert->pNext->y, curprob);
			return -20;
		}
		pNew->tx = pVert->x;
		pNew->ty = pVert->y;
		pNew->bx = pVert->pNext->x;
		pNew->by = pVert->pNext->y;
		pNew->dy = pNew->by - pNew->ty;
		pNew->dx = pNew->bx - pNew->tx;
		pNew->pNext = NULL;
		if(pPolyRight == NULL)
		{
			pPolyRight = pTail = pNew;
		}
		else
		{
			pTail->pNext = pNew;
			pTail = pNew;
		}
		pVert = pVert->pNext;
	}
	// now do left side
	pVert = pPolyVerts;
	while(pVert->y > pVert->pPrev->y)
	{
		pNew = GetNewEdge();
		if(pNew == NULL)
		{
			fprintf(stderr, "Left Poly Edge y's %d %d alloc failed on problem %d\n",
				pVert->y, pVert->pPrev->y, curprob);
			return -21;
		}
		pNew->tx = pVert->x;
		pNew->ty = pVert->y;
		pNew->bx = pVert->pPrev->x;
		pNew->by = pVert->pPrev->y;
		pNew->dy = pNew->by - pNew->ty;
		pNew->dx = pNew->bx - pNew->tx;
		pNew->pNext = NULL;
		if(pPolyLeft == NULL)
		{
			pPolyLeft = pTail = pNew;
		}
		else
		{
			pTail->pNext = pNew;
			pTail = pNew;
		}
		pVert = pVert->pPrev;
	}
	bottomY = pVert->y;
	return 0;
}

int BuildScans(int curprob)
{
	int y, lftDY, rtDY, lftX, rtX;
	int curLftX, curRtX;
	PSCAN_LINE pNew, pTail;
	PEDGE pLftE, pRtE;

	nNonEmptyScans = 0;
	y = pPolyVerts->y - 1;	// top line of poly not interior
	pLftE = pPolyLeft;
	pRtE = pPolyRight;
	lftDY = rtDY = -1;
	lftX = pLftE->tx;
	rtX = pRtE->tx;
	while(y > bottomY)
	{
		pNew = GetNewScan();
		if(pNew == NULL)
		{
			fprintf(stderr, "Scan y %d alloc failed on problem %d\n",
				y, curprob);
			return -30;
		}
		curLftX = lftX + TruncLeft(lftDY * pLftE->dx, pLftE->dy) + 1;
		curRtX = rtX + TruncRight(rtDY * pRtE->dx, pRtE->dy) - 1;	
		pNew->xl = curLftX;
		pNew->xr = curRtX;
		pNew->y = y;
		if(curLftX <= curRtX)
		{
			nNonEmptyScans++;
		}
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
		if(y == pLftE->by)
		{
			lftX = pLftE->bx;	// do this first, pLftE->pNext may be NULL
			pLftE = pLftE->pNext;
			lftDY = 0;
		}
		if(y == pRtE->by)
		{
			rtX = pRtE->bx;	// do this first, pRtE->pNext may be NULL
			pRtE = pRtE->pNext;
			rtDY = 0;
		}
		lftDY--;
		rtDY--;
		y--;
	}
	return 0;
}

void PrintSolution(int curprob)
{
	PSCAN_LINE pScan;
	printf("%d %d\n", curprob, nNonEmptyScans);
	if(nNonEmptyScans > 0)
	{
		pScan = pPolyScans;
		while(pScan != NULL)
		{
			if(pScan->xl <= pScan->xr)
			{
				printf("%d %d %d\n", pScan->y, pScan->xl, pScan->xr);
			}
			pScan = pScan->pNext;
		}
	}
}

int main()
{
	int nprob, curprob, probsz, ret, index;

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
		if(probsz < 3)
		{
			fprintf(stderr, "problem size %d is < 3 in problem %d\n", probsz, curprob);
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
		if((ret = BuildEdges(curprob)) != 0)
		{
			CleanGlobal();
			return ret;
		}
		if((ret = BuildScans(curprob)) != 0)
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