// D_Pancakes.cpp
//
// Two solutions presented here.  Define "FIRST_WAY" to use the direct approach
// Undefine FIRST_WAY and get the obscure, but better solution. less flips.
// Its main purpose is to generate solutions from
// which it is harder to guess the solution method.

//#define FIRST_WAY

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN		30

int data[MAX_LEN+1];
int check[MAX_LEN+1];
int flips[2*MAX_LEN];

char inbuf[512];

FILE *fpi, *fpo;

#ifdef FIRST_WAY
int
ParseInbuf(char *pBuf, int *pVals)
{
	int nvals, curval, len, pos;
	int inval;

	len = strlen(pBuf);
	pos = 0;
	/* skip leading spaces */
	while(isspace(pBuf[pos]))
	{
		pos++;
		if(pos >= len)
		{
			return -4;
		}
	}
	for(curval = 0; curval <= MAX_LEN ; curval++)
	{
		check[curval] = -1;
	}
	/* now point at non-whitespace */
	/* get count */
	if(sscanf(&(pBuf[pos]), "%d", &nvals) != 1)
	{
		return -5;
	}
	if(nvals > MAX_LEN)
	{
		return -9;
	}
	for(curval = 0; curval < nvals ; curval++)
	{
		/* skip over last value */
		while(!isspace(pBuf[pos]))
		{
			pos++;
			if(pos >= len)
			{
				return -6;
			}
		}
		/* skip over spaces */
		while(isspace(pBuf[pos]))
		{
			pos++;
			if(pos >= len)
			{
				return -7;
			}
		}
		/* read integer value */
		if(sscanf(&(pBuf[pos]), "%d", &inval) != 1)
		{
			return -8;
		}
		/* verify */
		if((inval < 1) || (inval > nvals))
		{
			return -11;
		}
		if(check[inval] >= 0)
		{
			return -12;
		}
		check[inval] = curval;
		pVals[curval] =inval;
	}
	return nvals;
}

/* flip first k values */
void 
flip(int k, int *pVals)
{
	int i, j, tmp;
	for(i = 0, j = k-1; i < k/2 ; i++, j--)
	{
		tmp = pVals[i];
		pVals[i] = pVals[j];
		pVals[j] = tmp;
	}
}

/*
 * figure out the flips
 * for k = nVals .. 3 find pos (k)
 * if pos(k) == k, done
 * else if pos(k) == 1, flip(k)
 * else flip(pos(k) to get to 1 then flip(k)
 * then for 1,2 if not in order flip 2
 */
int 
findFlips(int nVals, int *pVals, int *pFlips)
{
	int nflips, i, k, pos;

	nflips = 0;
	for(k = nVals; k > 2 ; k--)
	{
		pos = -1;
		for(i = 0; i < k ; i++)
		{
			if(pVals[i] == k)
			{
				pos = i+1;
				break;
			}
		}
		if(pos <= 0)
		{	/* cannot find it?? */
			return -30;
		}
		if(pos == k)
		{	/* already in the correct place */
			continue;
		}
		else if(pos == 1)
		{	/* in first place, flip to k */
			pFlips[nflips++] = k;
			flip(k, pVals);
		}
		else
		{	/* first flip from cur pos to 1 then to k */
			pFlips[nflips++] = pos;
			flip(pos, pVals);
			pFlips[nflips++] = k;
			flip(k, pVals);
		}
	}
	if(pVals[0] != 1)
	{
		pFlips[nflips++] = 2;
		flip(2, pVals);
	}
	/* check in order */
	for(i = 0; i < nVals; i++)
	{
		if(pVals[i] != i+1)
		{
			return -31;
		}
	}
	return nflips;
}

void
PrintFlips(int nflips, int *pFlips)
{
	int i;

	if(nflips == 0)
	{
		fprintf(fpo, "0\n");
		return;
	}
	fprintf(fpo, "%d ", nflips);
	for(i = 0; i < nflips - 1; i++)
	{
		fprintf(fpo, "%d ", pFlips[i]);
	}
	fprintf(fpo, "%d\n", pFlips[nflips - 1]);
}

int
main()
{
	int dataset, ncnt, nflips, dbval;
	fpi = fopen("d.in", "rt");
	if(fpi == NULL){
		fprintf(stderr, "Can not open input d.in\n");
		return(1);
	}
	fpo = fopen("d.out", "wt");
	if(fpo == NULL){
		fprintf(stderr, "Can not open output d.out\n");
		return(2);
	}

	dataset = 0;
	if(fgets(&(inbuf[0]), 512, fpi) == NULL)
	{
		fprintf(stderr, "read failed on first dataset\n");
		return -20;
	}
	dbval = 0;
	while(1)
	{
		if(dataset == 0x31)
		{
			dbval++;
		}
		ncnt = ParseInbuf(&(inbuf[0]), &(data[0]));
		if(ncnt <= 0)
		{	/* 0 means end of data <0 means error */
			return ncnt;
		}
		nflips = findFlips(ncnt, &(data[0]), &(flips[0]));
		if(nflips < 0)
		{
			return nflips;
		}
		PrintFlips(nflips, &(flips[0]));
		dataset++;
		if(fgets(&(inbuf[0]), 512, fpi) == NULL)
		{
			fprintf(stderr, "read failed on dataset %d\n", dataset + 1);
			return -20;
		}
	}
	fclose(fpi);
	fclose(fpo);
	return 0;
}

#else
int revFindFlips(int low, int high, int *pVals, int *pFlips, int *pNFlips);

int
ParseInbuf(char *pBuf, int *pVals)
{
	int nvals, curval, len, pos;
	int inval;

	len = strlen(pBuf);
	pos = 0;
	/* skip leading spaces */
	while(isspace(pBuf[pos]))
	{
		pos++;
		if(pos >= len)
		{
			return -4;
		}
	}
	for(curval = 0; curval <= MAX_LEN ; curval++)
	{
		check[curval] = -1;
	}
	/* now point at non-whitespace */
	/* get count */
	if(sscanf(&(pBuf[pos]), "%d", &nvals) != 1)
	{
		return -5;
	}
	if(nvals > MAX_LEN)
	{
		return -9;
	}
	for(curval = 0; curval < nvals ; curval++)
	{
		/* skip over last value */
		while(!isspace(pBuf[pos]))
		{
			pos++;
			if(pos >= len)
			{
				return -6;
			}
		}
		/* skip over spaces */
		while(isspace(pBuf[pos]))
		{
			pos++;
			if(pos >= len)
			{
				return -7;
			}
		}
		/* read integer value */
		if(sscanf(&(pBuf[pos]), "%d", &inval) != 1)
		{
			return -8;
		}
		/* verify */
		if((inval < 1) || (inval > nvals))
		{
			return -11;
		}
		if(check[inval] >= 0)
		{
			return -12;
		}
		check[inval] = curval;
		pVals[curval] =inval;
	}
	return nvals;
}

/* flip first k values */
void 
flip(int k, int *pVals)
{
	int i, j, tmp;
	for(i = 0, j = k-1; i < k/2 ; i++, j--)
	{
		tmp = pVals[i];
		pVals[i] = pVals[j];
		pVals[j] = tmp;
	}
}

/*
 * recursively figure out the flips
 * if high val is on the bottom of current stack
 *	leave it there and recursively call findFlips with high-1
 * else if high val is on top, flip it to the bottom
 *	and recursively call findFlips with high-1
 * else if low val is on the bottom
 *	recursively call revFindFlips with low+1 to get in rev order
 *	then flip the lot
 * else
 *	find location of high value
 *	flip it to the top
 *	flip the whole stack to get it to the bottom
 *	and recursively call findFlips with high-1
*/
int 
findFlips(int low, int high, int *pVals, int *pFlips, int *pNFlips)
{
	int nleft, i, pos, ret;

	nleft = high - low + 1;
	if(nleft == 1)
	{
		if(pVals[0] != high)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}
	else if(nleft == 2)
	{
		if((pVals[0] == low) && (pVals[1] == high))
		{
			return 0;
		}
		else if((pVals[1] == low) && (pVals[0] == high))
		{
			flip(2, pVals);
			pFlips[*pNFlips] = 2;
			(*pNFlips)++;
			return 0;
		}
		else
		{
			return -2;
		}
	}
	if(pVals[nleft-1] == high)
	{	/* already in the right place on to the next */
		return(findFlips(low, high-1, pVals, pFlips, pNFlips));
	}
	else if(pVals[0] == high)
	{	/* in first place flip to correct pos */
		flip(nleft, pVals);
		pFlips[*pNFlips] = nleft;
		(*pNFlips)++;
		return(findFlips(low, high-1, pVals, pFlips, pNFlips));
	}
	else if(pVals[nleft-1] == low)
	{
		ret = revFindFlips(low+1, high, pVals, pFlips, pNFlips);
		if(ret != 0)
		{
			return ret;
		}
		/* now in reverse order, flip the whole thing */
		flip(nleft, pVals);
		pFlips[*pNFlips] = nleft;
		(*pNFlips)++;
		return 0;
	}
	else
	{
		pos = -1;
		for(i = 0; i < nleft ; i++)
		{
			if(pVals[i] == high)
			{
				pos = i+1;
				break;
			}
		}
		if(pos <= 0)
		{	/* cannot find it?? */
			return -30;
		}
		if(pos == nleft)
		{	/* already in the correct place */
			return(findFlips(low, high-1, pVals, pFlips, pNFlips));
		}
		else if(pos == 1)
		{	/* in first place, flip to k */
			flip(nleft, pVals);
			pFlips[*pNFlips] = nleft;
			(*pNFlips)++;
			return(findFlips(low, high-1, pVals, pFlips, pNFlips));
		}
		else
		{	/* first flip from cur pos to 1 then to k */
			flip(pos, pVals);
			pFlips[*pNFlips] = pos;
			(*pNFlips)++;
			flip(nleft, pVals);
			pFlips[*pNFlips] = nleft;
			(*pNFlips)++;
			return(findFlips(low, high-1, pVals, pFlips, pNFlips));
		}
	}
}

/*
 * recursively figure out the flips to get stack in reverse order
 * if low val is on the bottom of current stack
 *	leave it there and recursively call revFindFlips with low+1
 * else if low val is on top, flip it  to the bottom
 *	and recursively call revFindFlips with low+1
 * else if high val is on the bottom
 *	recursively call findFlips with high-1 to get in fwd order
 *	then flip the lot
 * else
 *	find location of low value
 *	flip it to the top
 *	flip the whole stack to get it to the bottom
 *	and recursively call revFindFlips with low+1
*/
int 
revFindFlips(int low, int high, int *pVals, int *pFlips, int *pNFlips)
{
	int nleft, i, pos, ret;

	nleft = high - low + 1;
	if(nleft == 1)
	{
		if(pVals[0] != high)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}
	else if(nleft == 2)
	{
		if((pVals[1] == low) && (pVals[0] == high))
		{
			return 0;
		}
		else if((pVals[0] == low) && (pVals[1] == high))
		{
			flip(2, pVals);
			pFlips[*pNFlips] = 2;
			(*pNFlips)++;
			return 0;
		}
		else
		{
			return -2;
		}
	}
	if(pVals[nleft-1] == low)
	{	/* already in the right place on to the next */
		return(revFindFlips(low+1, high, pVals, pFlips, pNFlips));
	}
	else if(pVals[0] == low)
	{	/* at top, flip to bottom */
		flip(nleft, pVals);
		pFlips[*pNFlips] = nleft;
		(*pNFlips)++;
		return(revFindFlips(low+1, high, pVals, pFlips, pNFlips));
	}
	else if(pVals[nleft-1] == high)
	{	/* right place for fwd */
		ret = findFlips(low, high-1, pVals, pFlips, pNFlips);
		if(ret != 0)
		{
			return ret;
		}
		/* now in fwd order, flip the whole thing to get reverse */
		flip(nleft, pVals);
		pFlips[*pNFlips] = nleft;
		(*pNFlips)++;
		return 0;
	}
	else
	{
		pos = -1;
		for(i = 0; i < nleft ; i++)
		{
			if(pVals[i] == low)
			{
				pos = i+1;
				break;
			}
		}
		if(pos <= 0)
		{	/* cannot find it?? */
			return -30;
		}
		if(pos == nleft)
		{	/* already in the correct place */
			return(revFindFlips(low+1, high, pVals, pFlips, pNFlips));
		}
		else if(pos == 1)
		{	/* in first place, flip to k */
			flip(nleft, pVals);
			pFlips[*pNFlips] = nleft;
			(*pNFlips)++;
			return(revFindFlips(low+1, high, pVals, pFlips, pNFlips));
		}
		else
		{	/* first flip from cur pos to 1 then to k */
			flip(pos, pVals);
			pFlips[*pNFlips] = pos;
			(*pNFlips)++;
			flip(nleft, pVals);
			pFlips[*pNFlips] = nleft;
			(*pNFlips)++;
			return(revFindFlips(low+1, high, pVals, pFlips, pNFlips));
		}
	}
}

int CheckResult(int nvals, int *pVals)
{
	int i;
	for(i = 0; i < nvals ; i++)
	{
		if(pVals[i] != i+1)
		{
			fprintf(stderr, "CheckResult: val[%d] = %d\n",
				i+1, pVals[i]);
			return -1;
		}
	}
	return 0;
}

void
PrintFlips(int nflips, int *pFlips)
{
	int i;

	if(nflips == 0)
	{
		printf("0\n");
		return;
	}
	fprintf(fpo, "%d ", nflips);
	for(i = 0; i < nflips - 1; i++)
	{
		fprintf(fpo, "%d ", pFlips[i]);
	}
	fprintf(fpo, "%d\n", pFlips[nflips - 1]);
}

int
main()
{
	int dataset, ncnt, nflips, ret;
	fpi = fopen("d.in", "rt");
	if(fpi == NULL){
		fprintf(stderr, "Can not open input d.in\n");
		return(1);
	}
	fpo = stdout;
	if(fpo == NULL){
		fprintf(stderr, "Can not open output d.out\n");
		return(2);
	}

	dataset = 0;
	if(fgets(&(inbuf[0]), 512, fpi) == NULL)
	{
		fprintf(stderr, "read failed on first dataset\n");
		return -20;
	}
	while(1)
	{
		ncnt = ParseInbuf(&(inbuf[0]), &(data[0]));
		if(ncnt <= 0)
		{	/* 0 means end of data <0 means error */
			return ncnt;
		}
		nflips = 0;
		ret = findFlips(1, ncnt, &(data[0]), &(flips[0]), &nflips);
		if(ret != 0)
		{
			return ret;
		}
		if(CheckResult(ncnt, &(data[0])) != 0)
		{
			return -21;
		}
		PrintFlips(nflips, &(flips[0]));
		dataset++;
		if(fgets(&(inbuf[0]), 512, fpi) == NULL)
		{
			fprintf(stderr, "read failed on dataset %d\n", dataset + 1);
			return -20;
		}
	}
	fclose(fpi);
//	fclose(fpo);
	return 0;
}
#endif
