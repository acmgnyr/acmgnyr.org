// RookCircuits2.cpp : Defines the entry point for the console application.
//


#include <stdio.h>
#include <string.h>


typedef unsigned char BYTE;

// so reverse = 3 - value
#define NORTH	0
#define SOUTH	3
#define EAST	1
#define WEST	2
#define NORTH_MASK	0x1
#define SOUTH_MASK	0x8
#define EAST_MASK	0x2
#define WEST_MASK	0x4
#define ALL_MASK	0xf

#define MARK_MASK	0x10

struct CSquare  
{
	static int sm_nNSOffset;
	int m_nMarkSeq;
	BYTE m_nAdjNbrs[4];
	BYTE m_nAvailNbrs;
	BYTE m_ucNbrMask;

};

//#define DO_DIAG

#define OUT_SZ		40

int sm_nNSOffset;

CSquare *pBoard = NULL;
FILE *fp = NULL;

int scan_fwd();
int scan_rev();

char *pResult = NULL;
int nSquares = 0;

char inbuf[128];
char outbuf[48];

int nrow, ncol, nblock, startx, starty, res_sz;
int scan_cnt_lo, scan_cnt_hi, fwd_depth, rev_depth;
int fwd_x, fwd_y, rev_x, rev_y;
int blocksz = 0;
int *blocks = NULL;
int mark_seq = 0;

BYTE adj_masks[4] = {NORTH_MASK, EAST_MASK, WEST_MASK, SOUTH_MASK};
BYTE rev_adj_masks[4] = {SOUTH_MASK, WEST_MASK, EAST_MASK, NORTH_MASK};
int adj_offset[4];
char adj_tags[4] = {'N', 'E', 'W', 'S'};
int adj_xs[4] = {0, 1, -1, 0};
int adj_ys[4] = {-1, 0, 0, 1};

int read_input()
{
	int nread, i, x, y, evencnt;

	if(fgets(&(inbuf[0]), 128, fp) == NULL)
	{
		return -1;
	}
	if((nread = sscanf(&(inbuf[0]), "%d %d %d %d %d",
		&nrow, &ncol, &nblock, &startx, &starty)) != 5)
	{
		return -2;
	}
	if(nrow == 0)
	{
		return 0;
	}
	evencnt = 0;
	if(((nrow * ncol) & 1) != 0)
	{
		evencnt = 1;
	}
	if(blocksz < nblock)
	{
		if(blocks != NULL)
		{
			delete [] blocks;
			blocks = NULL;
			blocksz = 0;
		}
		blocks = new int[4*nblock];
		if(blocks == NULL)
		{
			return -3;
		}
		blocksz = 2*nblock;
	}
	for(i = 0; i < nblock ; i++)
	{
		if(fgets(&(inbuf[0]), 128, fp) == NULL)
		{
			return -4;
		}
		if((nread = sscanf(&(inbuf[0]), "%d %d",
			&x, &y)) != 2)
		{
			return -5;
		}
		blocks[2*i] = x;
		blocks[2*i + 1] = y;
		if(((x + y) & 1) == 0)
		{
			evencnt--;
		}
		else
		{
			evencnt++;
		}
	}
	// read separating blank line
	if(fgets(&(inbuf[0]), 128, fp) == NULL)
	{
		return -6;
	}
	if(evencnt == 0) return 1;	// soln possible
	else return 2;	// no soln possible
}

int setup_grid()
{
	int newsize, i, j, k;
	CSquare *pSq, *pSqNbr;

	if(nSquares < (nrow * ncol))
	{
		if(pBoard != NULL)
		{
			delete [] pBoard;
			pBoard = NULL;
			nSquares = 0;
		}
		if(pResult != NULL)
		{
			delete [] pResult;
			pResult = NULL;
		}
		newsize = 2 * nrow * ncol;
		pBoard = new CSquare[newsize];
		if(pBoard == NULL)
		{
			return -7;
		}
		pResult = new char[newsize];
		if(pResult == NULL)
		{
			delete [] pBoard;
			pBoard = NULL;
			return -7;
		}
		nSquares = newsize;
	}
	sm_nNSOffset = nrow;
	newsize = nrow * ncol;
	// base setup
	pSq = pBoard;
	for(j = 0; j < newsize ; j++, pSq++)
	{
		pSq->m_nAvailNbrs = 4;
		pSq->m_nAdjNbrs[NORTH] = 4;
		pSq->m_nAdjNbrs[WEST] = 4;
		pSq->m_nAdjNbrs[EAST] = 4;
		pSq->m_nAdjNbrs[SOUTH] = 4;
		pSq->m_ucNbrMask = ALL_MASK;
		pSq->m_nMarkSeq = -1;
	}
	// adjust top row
	pSq = pBoard;
	for(j = 0; j < ncol; j++, pSq++)
	{
		pSq->m_nAvailNbrs--;
		pSq->m_ucNbrMask &= ~NORTH_MASK;
		pSq->m_nAdjNbrs[NORTH] = 0;
	}
	// adjust top row
	pSq = pBoard + (nrow - 1) * ncol;
	for(j = 0; j < ncol; j++, pSq++)
	{
		pSq->m_nAvailNbrs--;
		pSq->m_ucNbrMask &= ~SOUTH_MASK;
		pSq->m_nAdjNbrs[SOUTH] = 0;
	}
	// adjust left col
	pSq = pBoard;
	for(j = 0; j < nrow; j++, pSq += ncol)
	{
		pSq->m_nAvailNbrs--;
		pSq->m_ucNbrMask &= ~WEST_MASK;
		pSq->m_nAdjNbrs[WEST] = 0;
	}
	// adjust right col
	pSq = pBoard + (ncol - 1);
	for(j = 0; j < nrow; j++, pSq += ncol)
	{
		pSq->m_nAvailNbrs--;
		pSq->m_ucNbrMask &= ~EAST_MASK;
		pSq->m_nAdjNbrs[EAST] = 0;
	}
	// now mark the blocks
	for(k = 0; k < nblock; k++)
	{
		i = blocks[2*k];
		j = blocks[2*k + 1];
		pSq = pBoard + j * ncol + i;
		pSq->m_nAvailNbrs = 0;	// mark not avail
		pSq->m_nMarkSeq = -2;
		// tell nbrs
		if(pSq->m_ucNbrMask & NORTH_MASK)
		{	// north nbr
			pSqNbr = pSq - ncol;
			pSqNbr->m_nAvailNbrs--;
			pSqNbr->m_ucNbrMask &= ~SOUTH_MASK;
			pSqNbr->m_nAdjNbrs[SOUTH] = 0;
		}
		if(pSq->m_ucNbrMask & SOUTH_MASK)
		{	// south nbr
			pSqNbr = pSq + ncol;
			pSqNbr->m_nAvailNbrs--;
			pSqNbr->m_ucNbrMask &= ~NORTH_MASK;
			pSqNbr->m_nAdjNbrs[NORTH] = 0;
		}
		if(pSq->m_ucNbrMask & WEST_MASK)
		{	// west nbr
			pSqNbr = pSq - 1;
			pSqNbr->m_nAvailNbrs--;
			pSqNbr->m_ucNbrMask &= ~EAST_MASK;
			pSqNbr->m_nAdjNbrs[EAST] = 0;
		}
		if(pSq->m_ucNbrMask & EAST_MASK)
		{	// west nbr
			pSqNbr = pSq + 1;
			pSqNbr->m_nAvailNbrs--;
			pSqNbr->m_ucNbrMask &= ~WEST_MASK;
			pSqNbr->m_nAdjNbrs[WEST] = 0;
		}
	}
	/* look for non-block with less than two nbrs */
	pSq = pBoard;
	for(j = 0; j < newsize ; j++, pSq++)
	{
		if((pSq->m_nAvailNbrs < 2) && (pSq->m_nMarkSeq != -2))
		{
			return 2;
		}
	}
	res_sz = (nrow * ncol) - nblock;
	adj_offset[NORTH] = -ncol;
	adj_offset[SOUTH] = ncol;
	adj_offset[EAST] = 1;
	adj_offset[WEST] = -1;
	mark_seq = 0;
	return 0;
}

int scan_rev()
{
	CSquare *pSq, *pSqNbr;
	int curAdjCnt, oneCntInd, i, retval, adjcnt, onlyadj, depth, x, y;

	scan_cnt_lo++;
	if(scan_cnt_lo >= 1000000000)
	{
		scan_cnt_lo -= 1000000000;
		scan_cnt_hi++;
		printf("scan cnt %d%09d\n", scan_cnt_hi, scan_cnt_lo);
	}
	depth = rev_depth;
	x = rev_x;
	y = rev_y;
	if(rev_depth == fwd_depth + 1)
	{
		if((rev_x == fwd_x - 1) && (rev_y == fwd_y))
		{
			pResult[rev_depth - 1] = 'W';
			return 0;
		}
		else if((rev_x == fwd_x + 1) && (rev_y == fwd_y))
		{
			pResult[rev_depth - 1] = 'E';
			return 0;
		}
		else if((rev_x == fwd_x) && (rev_y == fwd_y - 1))
		{
			pResult[rev_depth - 1] = 'N';
			return 0;
		}
		else if((rev_x == fwd_x) && (rev_y == fwd_y + 1))
		{
			pResult[rev_depth - 1] = 'S';
			return 0;
		}
		else
		{	// end of the line and not adj to other end
			return -1;
		}
	}
	// mark this node and tell adj nodes
	retval = adjcnt = 0;
	pSq = pBoard + rev_y * ncol + rev_x;
	curAdjCnt = pSq->m_nAvailNbrs;
	oneCntInd = -1;
	pSq->m_nAvailNbrs = 0;	// mark as used
	pSq->m_nMarkSeq = mark_seq++;
	// tell avail nbrs if not start node
	if((rev_x != startx) || (rev_y != starty))
	{
		for(i = 0; i < 4; i++)
		{
			if((pSq->m_ucNbrMask & adj_masks[i]) != 0)
			{
				pSqNbr = pSq + adj_offset[i];
				if(pSqNbr->m_nMarkSeq >= 0)
				{
					printf("nbr %d of %d,%d markSeq %d cur seq %d\n",
						i, rev_x, rev_y, pSqNbr->m_nMarkSeq, mark_seq);
				}
				pSqNbr->m_nAvailNbrs--;
				pSqNbr->m_ucNbrMask &= ~rev_adj_masks[i];
				pSqNbr->m_nAdjNbrs[3-i] = 0;
				pSq->m_nAdjNbrs[i] = pSqNbr->m_nAvailNbrs;
				if(((rev_x + adj_xs[i]) != fwd_x) || ((rev_y + adj_ys[i]) != fwd_y))
				{	// not the other end, valid next choice
					if(pSqNbr->m_nAvailNbrs == 1)
					{	// forced to go this way
						if(oneCntInd >= 0)
						{
							oneCntInd = 5;	// have to go two ways
							retval = -2;	// backtrack
						}
						else oneCntInd = i;
					}
					adjcnt++;
					onlyadj = i;
				}
			}
		}
	}
	else
	{
		for(i = 0; i < 4; i++)
		{
			if((pSq->m_ucNbrMask & adj_masks[i]) != 0)
			{
				pSqNbr = pSq + adj_offset[i];
				if(((rev_x + adj_xs[i]) != fwd_x) || ((rev_y + adj_ys[i]) != fwd_y))
				{	// not the other end, valid next choice
					if(pSqNbr->m_nAvailNbrs == 1)
					{	// forced to go this way
						if(oneCntInd >= 0)
						{
							oneCntInd = 5;	// have to go two ways
							retval = -2;	// backtrack
						}
						else oneCntInd = i;
					}
					adjcnt++;
					onlyadj = i;
				}
			}
		}
	}
	if(adjcnt == 0)
	{
		retval = -3;
	}
	if(retval == 0)
	{
		if(oneCntInd >= 0)
		{	// force direction
			rev_depth = depth - 1;
			pResult[rev_depth] = adj_tags[3 - oneCntInd];
			rev_x = x + adj_xs[oneCntInd];
			rev_y = y + adj_ys[oneCntInd];
			retval = scan_rev();
		}
		else if(adjcnt == 1)
		{	// only one way to go, go there
			rev_depth = depth - 1;
			pResult[rev_depth] = adj_tags[3 - onlyadj];
			rev_x = x + adj_xs[onlyadj];
			rev_y = y + adj_ys[onlyadj];
			retval = scan_rev();
		}
		else
		{	// for each choice, move there but try other end first
			for(i = 0; i < 4; i++)
			{
				if((pSq->m_ucNbrMask & adj_masks[i]) != 0)
				{
					if(((x + adj_xs[i]) != fwd_x) || ((y + adj_ys[i]) != fwd_y))
					{	// not the other end, valid next choice
						rev_depth = depth - 1;
						pResult[rev_depth] = adj_tags[3-i];
						rev_x = x + adj_xs[i];
						rev_y = y + adj_ys[i];
						retval = scan_fwd();
						if(retval == 0)
						{
							break;
						}
					}
				}
			}
		}
	}
	rev_depth = depth;
	rev_x = x;
	rev_y = y;
	// undo setup
	pSq->m_nMarkSeq = -1;
	mark_seq--;
	pSq->m_nAvailNbrs = curAdjCnt;	// put back old value
	// tell avail nbrs
	if((rev_x != startx) || (rev_y != starty))
	{
		for(i = 0; i < 4; i++)
		{
			if((pSq->m_ucNbrMask & adj_masks[i]) != 0)
			{	// north nbr
				pSqNbr = pSq + adj_offset[i];
				pSqNbr->m_nAvailNbrs++;
				pSqNbr->m_ucNbrMask |= rev_adj_masks[i];
				pSqNbr->m_nAdjNbrs[3-i] = curAdjCnt;
				pSq->m_nAdjNbrs[i] = pSqNbr->m_nAvailNbrs;
			}
		}
	}
	return retval;
}

int scan_fwd()
{
	CSquare *pSq, *pSqNbr;
	int curAdjCnt, oneCntInd, i, retval, adjcnt, onlyadj, depth, x, y;

	scan_cnt_lo++;
	if(scan_cnt_lo >= 1000000000)
	{
		scan_cnt_lo -= 1000000000;
		scan_cnt_hi++;
		printf("scan cnt %d%09d\n", scan_cnt_hi, scan_cnt_lo);
	}
	depth = fwd_depth;
	x = fwd_x;
	y = fwd_y;
	if(fwd_depth == rev_depth - 1)
	{
		if((fwd_x == rev_x - 1) && (fwd_y == rev_y))
		{
			pResult[fwd_depth] = 'E';
			return 0;
		}
		else if((fwd_x == rev_x + 1) && (fwd_y == rev_y))
		{
			pResult[fwd_depth] = 'W';
			return 0;
		}
		else if((fwd_x == rev_x) && (fwd_y == rev_y - 1))
		{
			pResult[fwd_depth] = 'S';
			return 0;
		}
		else if((fwd_x == rev_x) && (fwd_y == rev_y + 1))
		{
			pResult[fwd_depth] = 'N';
			return 0;
		}
		else
		{	// end of the line and not adj to start
			return -1;
		}
	}
	// mark this node andtell adj nodes
	retval = adjcnt = 0;
	pSq = pBoard + fwd_y * ncol + fwd_x;
	curAdjCnt = pSq->m_nAvailNbrs;
	oneCntInd = -1;
	pSq->m_nAvailNbrs = 0;	// mark as used
	pSq->m_nMarkSeq = mark_seq++;
	// tell avail nbrs unless doing start node
	for(i = 0; i < 4; i++)
	{
		if((pSq->m_ucNbrMask & adj_masks[i]) != 0)
		{
			pSqNbr = pSq + adj_offset[i];
			if(pSqNbr->m_nMarkSeq >= 0)
			{
				printf("nbr %d of %d,%d markSeq %d cur seq %d\n",
					i, fwd_x, fwd_y, pSqNbr->m_nMarkSeq, mark_seq);
			}
			pSqNbr->m_nAvailNbrs--;
			pSqNbr->m_ucNbrMask &= ~rev_adj_masks[i];
			pSqNbr->m_nAdjNbrs[3-i] = 0;
			pSq->m_nAdjNbrs[i] = pSqNbr->m_nAvailNbrs;
			if(((fwd_x + adj_xs[i]) != rev_x) || ((fwd_y + adj_ys[i]) != rev_y))
			{	// not the other end, valid next choice
				if(pSqNbr->m_nAvailNbrs == 1)
				{	// forced to go this way
					if(oneCntInd >= 0)
					{
						oneCntInd = 5;	// have to go two ways
						retval = -2;	// backtrack
					}
					else oneCntInd = i;
				}
				adjcnt++;
				onlyadj = i;
			}
		}
	}
	if(adjcnt == 0)
	{
		retval = -3;
	}
	if(retval == 0)
	{
		if(oneCntInd >= 0)
		{	// force direction
			pResult[depth] = adj_tags[oneCntInd];
			fwd_x = x + adj_xs[oneCntInd];
			fwd_y = y + adj_ys[oneCntInd];
			fwd_depth = depth + 1;
			retval = scan_fwd();
		}
		else if(adjcnt == 1)
		{	// only one way to go, go there
			pResult[depth] = adj_tags[onlyadj];
			fwd_x = x + adj_xs[onlyadj];
			fwd_y = y + adj_ys[onlyadj];
			fwd_depth = depth + 1;
			retval = scan_fwd();
		}
		else
		{
			for(i = 0; i < 4; i++)
			{
				if((pSq->m_ucNbrMask & adj_masks[i]) != 0)
				{
					if(((x + adj_xs[i]) != rev_x) || ((y + adj_ys[i]) != rev_y))
					{	// not the other end, valid next choice
						pResult[depth] = adj_tags[i];
						fwd_x = x + adj_xs[i];
						fwd_y = y + adj_ys[i];
						fwd_depth = depth + 1;
						retval = scan_rev();
						if(retval == 0)
						{
							break;
						}
					}
				}
			}
		}
	}
	fwd_depth = depth;
	fwd_x = x;
	fwd_y = y;
	// undo setup
	pSq->m_nAvailNbrs = curAdjCnt;	// put back old value
	pSq->m_nMarkSeq = -1;
	mark_seq--;
	// tell avail nbrs
	for(i = 0; i < 4; i++)
	{
		if((pSq->m_ucNbrMask & adj_masks[i]) != 0)
		{	// north nbr
			pSqNbr = pSq + adj_offset[i];
			pSqNbr->m_nAvailNbrs++;
			pSqNbr->m_ucNbrMask |= rev_adj_masks[i];
			pSqNbr->m_nAdjNbrs[3-i] = curAdjCnt;
			pSq->m_nAdjNbrs[i] = pSqNbr->m_nAvailNbrs;
		}
	}
	return retval;
}

int do_scan()
{
	CSquare *pSq, *pSqNbr;
	int curAdjCnt, oneCntInd, i, j, retval, adjcnt;

	scan_cnt_lo = 0;
	scan_cnt_hi = 0;
	rev_depth = res_sz;
	fwd_depth = 0;
	fwd_x = rev_x = startx;
	fwd_y = rev_y = starty;

	retval = -1;
	adjcnt = 0;
	pSq = pBoard + fwd_y * ncol + fwd_x;
	curAdjCnt = pSq->m_nAvailNbrs;
	oneCntInd = -1;
	pSq->m_nAvailNbrs = 0;	// mark as used
	pSq->m_nMarkSeq = mark_seq++;
	// tell avail nbrs
	for(i = 0; i < 4; i++)
	{
		if((pSq->m_ucNbrMask & adj_masks[i]) != 0)
		{
			pSqNbr = pSq + adj_offset[i];
			pSqNbr->m_nAvailNbrs--;
			pSqNbr->m_ucNbrMask &= ~rev_adj_masks[i];
			pSqNbr->m_nAdjNbrs[3-i] = 0;
			pSq->m_nAdjNbrs[i] = pSqNbr->m_nAvailNbrs;
			adjcnt++;
		}
	}
	if(adjcnt < 2)
	{
		return -1;
	}
	for(i = 0; i < 4; i++)
	{
		if((pSq->m_ucNbrMask & adj_masks[i]) != 0)
		{
			fwd_x = startx + adj_xs[i];
			fwd_y = starty + adj_ys[i];
			pResult[0] = adj_tags[i];
			fwd_depth = 1;
			for(j = i+1; j < 4; j++)
			{
				if((pSq->m_ucNbrMask & adj_masks[j]) != 0)
				{
					rev_x = startx + adj_xs[j];
					rev_y = starty + adj_ys[j];
					pResult[res_sz - 1] = adj_tags[3-j];
					rev_depth = res_sz - 1;
					retval = scan_fwd();
					if(retval == 0)
					{
						break;
					}
				}
			}
			if(retval == 0)
			{
				break;
			}
		}
	}
	return retval;
}

void print_soln()
{
	int sz, outsz;
	char *cp;

	sz = res_sz;
	cp = pResult;

	while(sz > 0)
	{
		if(sz > OUT_SZ) outsz = OUT_SZ;
		else outsz = sz;
		::memcpy(&(outbuf[0]), cp, outsz);
		outbuf[outsz] = 0;

		printf("%s\n", outbuf);
		sz -= outsz;
		cp += outsz;
	}
	printf("\n");
}

void show_scan_cnt()
{
#ifdef DO_DIAG
	if(scan_cnt_hi == 0)
	{
		printf("scan count %d\n", scan_cnt_lo);
	}
	else
	{
		printf("scan count %d%09d\n", scan_cnt_hi, scan_cnt_lo);
	}
#endif
}

int main()
{
	int ret;

    fp = fopen("f.in", "rt");
    if(fp == NULL){
        printf("Can't open input f.in\n");
        return 1;
    }

	while((ret = read_input()) > 0)
	{
		if(ret == 2)
		{
#ifdef DO_DIAG
			printf("even-odd failure: ");
#endif
			printf("NO SOLUTION\n\n");
		}
		else
		{
			ret = setup_grid();
			if(ret < 0)
			{
				fprintf(stderr, "Alloc error in setup_grid()\n");
				return ret;
			}
			else if(ret == 2)
			{
#ifdef DO_DIAG
				printf("one nbr failure: ");
#endif
				printf("NO SOLUTION\n\n");
			}
			else
			{	// try to solve
				rev_depth = res_sz;
				fwd_depth = 0;
				ret = do_scan();
				if(ret == 0)
				{
					show_scan_cnt();
					print_soln();
				}
				else
				{
					printf("NO SOLUTION\n\n");
				}
			}
		}
	}
    fclose(fp);
	if(ret < 0)
	{
		fprintf(stderr, "Invalid input format\n");
		return ret;
	}
	return 0;
}
