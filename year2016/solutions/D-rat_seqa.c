/*
 * D - Rational Sequence
 * Solution by Fred Pickel
 * ACM ICPC GNYR 2016
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define MAX_INPUT 2147483647
char inbuf[256];

/*
 * this is pretty standard binary tree level order sequence stuff
 * find the most significant bit of the sequence number: this puts you at the root
 * for each following bit 0 = left child, 1 = right child
 * use the definitions of the parent child relationship
 * when you run out of bits, you have the label for the node
 */
int GetSeqElt(int seq, int *pNum, int *pDenom)
{
	unsigned long val, mask;
	int num, denom;

	val = (unsigned long)seq;
	mask = 0x40000000;
	while((mask > 0) && ((val & mask) == 0)) {
		mask >>= 1;
	}
	num = denom = 1;
	mask >>= 1;
	while(mask > 0) {
		if((val & mask) == 0) {
			denom = num + denom;
		} else {
			num = denom + num;
		}
		mask >>= 1;
	}
	*pNum = num;
	*pDenom = denom;
	return 0;
}

int main()
{
	int nprob, curprob, index, seq, num, denom, ret;

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
			fprintf(stderr, "Read failed on problem %d data\n", curprob);
			return -3;
		}
		// get prob num and sequence index
		if(sscanf(&(inbuf[0]), "%d %d", &index, &seq) != 2)
		{
			fprintf(stderr, "scan failed on problem header problem index %d\n",
				curprob);
			return -6;
		}
		if(index != curprob)
		{
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				index, curprob);
			return -7;
		}
		if((seq < 1) || (seq > MAX_INPUT)) {
			fprintf(stderr, "problem index %d seq %d not in range 1-%d\n",
				curprob, seq, MAX_INPUT);
			return -7;
		}
		ret = GetSeqElt(seq, &num, &denom);
		if(ret != 0) {
			return ret;
		}
		printf("%d %d/%d\n", curprob, num, denom);
	}
	return 0;
}
