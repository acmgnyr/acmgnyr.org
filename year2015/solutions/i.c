/*
 * I - RATS
 * ICPC GNYR 2015
 * Problem and Solution: Fred Pickel
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define MAX_COUNT 100
char inbuf[256];
typedef unsigned char BYTE;

int index, count;
/*
 * since the values will always have digits sorted in increasing order
 * we only need to no how many times each digit 1-9 appears in the value
 */
typedef struct _fats_value_
{
	struct _fats_value_ *next;	// hash link
	int hash;	// large hash value	(table index is this % 129)
	int index;	// which iteration this is
	BYTE totDigitCount;	// total digits
	BYTE digitCounts[10];	// how many of each digit
} RATS_Value;

RATS_Value input;				// input value = first value
RATS_Value terms[MAX_COUNT];	// the values n the sequence, the inital value is terms[0]
RATS_Value *hashTable[129];		// hash table of terms so far to search for matches

/*
 * clear out terms and hashTable for next probelm instance
 */
void ClearAll(void)
{
	int i;
	memset(&(terms[0]), 0, MAX_COUNT*sizeof(RATS_Value));
	for(i = 0; i < 129 ; i++) hashTable[i] = NULL;
}

/*
 * clear a single RATS struct
 */
void ClearRATS(RATS_Value *pRV)
{
	int i;
	pRV->next = NULL;
	pRV->hash = 0;
	pRV->totDigitCount = 0;
	for(i = 0; i < 10 ; i++) {
		pRV->digitCounts[i] = 0;
	}
}

/*
 * parse input string (buf) to get problem index (in glabal index)
 * count (in global count) and inital decimal (in input)
 */
int ParseInput(char *buf)
{
	char *pb = buf;
	int hash, i;
	ClearRATS(&input);
	// find index
	while((*pb != 0) && isspace(*pb)) pb++;
	if(!isdigit(*pb)) return -1;
	index = 0;
	while(isdigit(*pb)) {
		index = index *10 + (*pb - '0');
		pb++;
	}
	// find count
	while((*pb != 0) && isspace(*pb)) pb++;
	if(!isdigit(*pb)) return -2;
	count = 0;
	while(isdigit(*pb)) {
		count = count *10 + (*pb - '0');
		pb++;
	}
	// get digit counts of input
	while((*pb != 0) && isspace(*pb)) pb++;
	if(!isdigit(*pb)) return -3;
	while(isdigit(*pb)) {
		input.digitCounts[(*pb - '0')]++;
		input.totDigitCount++;
		pb++;
	}
	// compute hash
	for(i = 0, hash = 0; i < 10 ; i++) {
		hash = hash*5 + input.digitCounts[i];
	}
	input.hash = hash;
	return 0;
}

/*
 * check a RATS_Value to see if it has entered the creeper by checking for
 * 1233+4444 or 5566+7777
 * return 1 for in creeper
 */
int CheckCreeper(RATS_Value *pRV)
{
	if((pRV->digitCounts[1] == 1) && (pRV->digitCounts[2] == 1) && (pRV->digitCounts[3] >= 2) &&
		(pRV->digitCounts[4] == 4) && (pRV->digitCounts[5] == 0) && (pRV->digitCounts[6] == 0) &&
		(pRV->digitCounts[7] == 0) && (pRV->digitCounts[8] == 0) && (pRV->digitCounts[9] == 0)) {
		return 1;
	}
	if((pRV->digitCounts[1] == 0) && (pRV->digitCounts[2] == 0) && (pRV->digitCounts[3] == 0) &&
		(pRV->digitCounts[4] == 0) && (pRV->digitCounts[5] == 2) && (pRV->digitCounts[6] >= 2) &&
		(pRV->digitCounts[7] == 4) && (pRV->digitCounts[8] == 0) && (pRV->digitCounts[9] == 0)) {
		return 1;
	}
	return 0;
}

char outbuf[256];

/*
 * generate the decimal string (in outbuf) corresponding to *pRV
 */
char *OutRATSVal(RATS_Value *pRV)
{
	char *pb = &(outbuf[0]);
	int i, j;
	for(i = 1; i < 10 ; i++) {
		for(j = 0; j < pRV->digitCounts[i] ; j++) {
			*pb++ = i + '0';
		}
	}
	*pb = 0;
	return &(outbuf[0]);
}

/*
 * compute the RATS vakue of terms[index-1] into terms[index]
 */
int CompRATS(int index)
{
	RATS_Value *pSrc, *pDest;
	int i1, i2, j1, j2, digcnt, carry, sum, hash;
	pSrc = &(terms[index-1]);
	pDest = &(terms[index]);
	ClearRATS(pDest);
	// i1 scans the value from left ot right 1's to 9's
	// j1 is how many of the current left digit count we have used
	// i2 scans the value from right to left 9's to 1's
	// j1 is how many of the current left digit count we have used
	i1 = 1; i2 = 9; digcnt = pSrc->totDigitCount;
	j1 = 0; j2 = 0; carry = 0;
	while(digcnt > 0) {	// for each digit in the source
		digcnt--;
		// scan left to right until we find an used digit
		while((i1 < 10) && (j1 >= pSrc->digitCounts[i1])){
			i1++;
			j1 = 0;
		}
		// scan right to left until we find an used digit
		while((i2 > 0) && (j2 >= pSrc->digitCounts[i2])){
			i2--;
			j2 = 0;
		}
		// add the left and right digits + any carry
		sum = i1 + i2 + carry;
		// increment the corresponding digit count and set carry if there is one
		pDest->digitCounts[sum%10]++;
		pDest->totDigitCount++;
		carry = sum/10;
		j1++, j2++;	// increment count of number of current digit used
	}
	// if carry is non-zero at the end, we have another 1 on the left
	if(carry > 0){
		pDest->digitCounts[1]++;
		pDest->totDigitCount++;
	}
	// remove leading 0 digits
	pDest->totDigitCount -= pDest->digitCounts[0];
	pDest->digitCounts[0] = 0;
	// compute hash
	for(i1 = 0, hash = 0; i1 < 10 ; i1++) {
		hash = hash*5 + pDest->digitCounts[i1];
	}
	pDest->hash = hash;
	pDest->index = index;
	return 0;
}

/*
 * cehck to see if a previous term is the same as the current term
 */
int CheckDup(int index)
{
	int hind, i, ret;
	RATS_Value *pRV, *prev;
	hind = terms[index].hash % 129;
	// if the hashTable entry is NULL, no match
	if(hashTable[hind] == NULL) {
		hashTable[hind] = &terms[index];
		return 0;
	}
	// if not, check each term in the chain from the hashTable entry
	pRV = hashTable[hind];
	while(pRV != NULL) {
		// if the large hash values do not match, no chance of exact match
		if(pRV->hash == terms[index].hash) {
			ret = index;
			// large hash the same, check each digit count
			for(i = 0; i < 10 ; i++) {
				if(pRV->digitCounts[i] != terms[index].digitCounts[i]) {
					ret = 0;	// if one does not match, give up
					break;
				}
			}
			if(ret != 0) {
				return ret;	// found a match
			}
		}
		// remember the current term and on to the next
		prev = pRV;
		pRV = pRV->next;
	}
	// if we get here, no match, put this term at the end of the hash chain
	prev->next = &(terms[index]);
	return 0;

}

int main()
{
	int nprob, curprob, ret, i;

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
		if((ret = ParseInput(&(inbuf[0]))) != 0) {
			fprintf(stderr, "scan failed on problem data problem index %d\n",
				curprob);
			return -6;
		}
		if(index != curprob)
		{
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				index, curprob);
			return -7;
		}
		if(count > MAX_COUNT)
		{
			fprintf(stderr, "problem index %d: count %d > max count %d\n",
				index, count, MAX_COUNT);
			return -8;
		}
		// start decimal may already be in the creeper
		if(CheckCreeper(&input)) {
			printf("%d C 1\n", index);
		} else {
			ret = 0;
			ClearAll();
			terms[0] = input;
			terms[0].index = 0;
			hashTable[input.hash % 129] = &(terms[0]);
			for(i = 1; (i < count) && (ret == 0) ; i++) {
				CompRATS(i);
				if((ret = CheckCreeper(&terms[i]))) {
					printf("%d C %d\n", index, i+1);
				} else if((ret = CheckDup(i)) != 0) {
					printf("%d R %d\n", index, i+1);
				}
			}
			if(ret == 0) {
				printf("%d %s\n", index, OutRATSVal(&(terms[count-1])));
			}
		}
	}
	return 0;
}
