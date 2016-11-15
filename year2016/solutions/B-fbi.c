/*
 * B - FBI Control Numbers
 * Solution by Fred Pickel
 * ACM ICPC GNYR 2016
 */
#include <stdio.h>
#include <ctype.h>

int alphaLookup[26] =
{
	10,	//A
	8,	//B = 8
	11,	//C
	12,	//D
	13,	//E
	14, //F
	11, //G = C
	15, // H
	1,  //I = 1
	16, //J
	17, //K
	18, //L
	19, //M
	20, //N
	0,  //O = 0
	21, //P
	0,  //Q = 0
	22, //R
	5,  //S = 5
	23, //T
	24, //U = V
	24, //V
	25, //W
	26, //X
	24, //Y = V
	2	//Z = 2
};

int factors[8] = {2, 4, 5, 7, 8, 10, 11, 13};

int UCNdigits[9];

int parseInput(char *pb, int inprob) 
{
	int i, probnum = 0;
	char c;
	// skip leading spaces
	while(*pb != 0) {
		if(!isspace(*pb))
			break;
		pb++;
	}
	if(*pb == 0) {
		printf("parseInput: unexpect end of string  on prob num %d\n", inprob);
		return -1;
	}
	// get prob num
	while(*pb != 0) {
		if(!isdigit(*pb))
			break;
		probnum = (probnum*10) + (*pb - '0');
		pb++;
	}
	if(*pb == 0) {
		printf("parseInput: unexpect end of string  on prob num %d\n", inprob);
		return -2;
	}
	// skip spaces between probnum and digit string
	while(*pb != 0) {
		if(!isspace(*pb))
			break;
		pb++;
	}
	if(*pb == 0) {
		printf("parseInput: unexpect end of string  on prob num %d\n", inprob);
		return -3;
	}
	// parse UCN digits
	for(i = 0; i < 9 ; i++) {
		c = *pb++;
		if((c >= '0') && (c <= '9')) {
			UCNdigits[i] = c - '0';
		} else if((c >= 'A') && (c <= 'Z')) {
			UCNdigits[i] = alphaLookup[c - 'A'];
		} else {
			printf("parseInput: bad digit char %c[0x%02x] prob num %d\n", c, c, inprob);
			return -4;
		}
	}
	return probnum;
}

int checkDigit()
{
	int i, ret = 0;
	for(i = 0; i < 8 ; i++) {
		ret += UCNdigits[i]*factors[i];
	}
	ret = (ret % 27);
	return ret;
}

char inbuf[256];
int main()
{
	int i, nprob, curprob, index, ret;
	unsigned long low, high;

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
		if((index = parseInput(&(inbuf[0]), curprob)) < 0) {
			return index;
		}
		if(index != curprob)
		{
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				index, curprob);
			return -7;
		}
		ret = checkDigit();
		if(ret != UCNdigits[8]) {
			printf("%d Invalid\n", curprob);
		} else {
			high = low = 0;
			for(i = 0; i < 8 ; i++) {
				low = (low *27) + UCNdigits[i];
				high = (high *27) + low/100000000;
				low = low%100000000;
			}
			printf("%d %d%08d\n", curprob, high, low);
		}
	}
	return 0;
}
