/*
 * ACM2006 Greater NY Region C++ Solution to I - Margaritas on the River Walk
 *
 * Fred Pickel
 * October, 2006
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

// sort vals in non-decreasing order
// array cannot be too big, just bubble sort
int sortVals(int *pVals, int *sortOrder, int nVals)
{
	int i, j, val;

	if(nVals <= 0)
	{
		return -1;
	}
	for(i = nVals-1; i > 0 ; i--)
	{	// for each elt from largest to smallest
		// push it down to the bottom
		val = pVals[0];
		for(j = 0; j < i ; j++)
		{
			if(val <= pVals[j+1])
			{
				pVals[j] = val;
				val = pVals[j+1];
			}
			else
			{
				pVals[j] = pVals[j+1];
			}
		}
		pVals[i] = val;
	}
	return 0;
}

// ASSUMES pVals is sorted in non-decreasing order
// Outer loop
// at the end of each step  i, ret is the number of valid ways of getting a sum
// between target - pVals[i] + 1 and target (inclusive)
// (valid meaning that if pVals[j] <= (target - sum). pVals[j] isincluded in the sum
// at step i if pVals[i] == last_value, we already have all possiblities
// otherwise, we can include all values before i in the sum (cumSum)
// and count how many ways there are to get sums between
// target - pVals[i] + 1 and target - last_value 
// to do this we find out how many ways there are to get sum the values from i to valCnt -1
// to get a sum between 
// target - pVals[i] + 1 - cumSum and target - last_value - cumSum
// to do this we have an array of counts, counts[i] = number of ways to get a sum of i
// using each element at most once
// initially counts[0] = 1 and all others are 0
// as we consider each element value, not adding it gives counts from the
// previous step, if it is added to a set with sum j, the result is j + value
// thus counts[j + value] = counts[j + value] (all the sets of that sum with value not added)
//						+ counts[j] (all the sets of sum j with value added
// in doing this, we start with the largest index and work down so the counts[j]
// in the above sum does not include sums with the new value included
int MSetCountMU(int *pVals, int *pCounts, int valCnt, int target)
{
	int ret, curVal, cntInd, curTarg, curBot, lastVal, curSum;
	int curInd, curMinInd, curMinVal;

	ret = 0;	// init return value
	// init
	curSum = 0;	// initially no items forced to be included
	lastVal = 0;	// no previous value
	for(curMinInd = 0; (curMinInd < valCnt) && (curSum < target) ; curMinInd++)
	{	// we get to stop if the sum of the forced elements is >= target
		curMinVal = pVals[curMinInd];
		if(curMinVal > lastVal)	// new larger value, so more sums possible
		{	// compute how many ways to use all items up to curMinInd - 1
			// and get a total between target - curMinVal + 1 and target - lastVal
			curTarg = target - curSum - lastVal;
			curBot = target - curSum - curMinVal + 1;
			if(curBot <=  0)
			{
				curBot = 0;	// clip
				if(curSum == 0)
				{
					curBot = 1; // and avoid empty set
				}
			}
			::memset(pCounts, 0, (target + 1)*sizeof(int));
			// there is 1 way to get a total of 0
			pCounts[0] = 1;
			// for each value, if it is not used, counts are as for previously
			// processed values, if it is used, for each count in the table,
			// it increases the count at table + current value
			for(curInd = curMinInd ; curInd < valCnt ; curInd++)
			{
				curVal = pVals[curInd];
				// count down to use the value at most once
				for(cntInd = curTarg; cntInd >= curVal ; cntInd--)
				{
					pCounts[cntInd] += pCounts[cntInd - curVal];
					if(pCounts[cntInd] < 0)
					{
						fprintf(stderr, " overflow min %d cur %d cnt %d\n",
							curMinInd, curInd, cntInd);
						return 0;
					}
				}
			}
			// add in the counts for sums in the new range
			for(cntInd = curTarg; cntInd >= curBot ; cntInd--)
			{
				ret += pCounts[cntInd];
				if(ret < 0)
				{
					fprintf(stderr, " ret overflow min %d cur %d \n",
						curMinInd, curInd);
					return 0;
				}
			}
		}
		curSum += curMinVal;	// add current value into those that must be included
		lastVal = curMinVal;	// remember it to compare to the next value
	}

	return ret;
}

int inVals[32];
int sortInds[32];
int counts[1024];
char inbuf[256];
char outstr[32];

double readData(int *pData, int maxCnt, int &nVals, int &target)
{
	double sum = 0.0;
	int nv, targ, expd, res, *pd, line, i;

	// get first problem header
	if(fgets(&(inbuf[0]), 255, stdin) == NULL)
	{
		fprintf(stderr, "bad read on problem header\n");
		return -1.0;
	}
	if((res = sscanf(&(inbuf[0]), "%d %d", &nv, &targ)) != 2)
	{
		fprintf(stderr, "bad data count %d != 2 in problem header\n", res);
		return -2.0;
	}
	nVals = nv;
	target = targ;
	line = 0;
	pd = pData;
	// read the vendor prices
	while(nv > 0)
	{
		line++;
		expd = nv;
		if(expd > 10) expd = 10;
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "bad read on problem data line %d nv=%d\n", line, nv);
			return -3.0;
		}
		res = sscanf(&(inbuf[0]), "%d %d %d %d %d %d %d %d %d %d",
			pd, pd+1, pd+2, pd+3, pd+4, pd+5, pd+6, pd+7, pd+8, pd+9);
		if(res < 1)
		{
			fprintf(stderr, "bad data count %d problem data line %d\n",
				res, line);
			return -4.0;
		}
		for(i = 0; i < res ; i++)
		{
			sum += (double)pd[i];
		}
		nv -= res;
		pd += res;
	}
	return sum;
}

int main()
{
	int nVals, target, res, i, n;
	double sum;
	char ibuf[256];

	if(fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
		return 1;
	}
	n = atoi(&(ibuf[0]));

	for(i = 1; i <= n; i++)
	{
		sum = readData(&(inVals[0]), 32, nVals, target);
		if(sum < 0.0)
		{
			return -1;
		}
		sortVals(&(inVals[0]), &(sortInds[0]), nVals);
		res = MSetCountMU(&(inVals[0]), &(counts[0]), nVals, target);
		printf("%d %d\n", i, res);
	}
	return 0;
}