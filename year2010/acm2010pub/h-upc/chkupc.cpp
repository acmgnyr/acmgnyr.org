#include <stdio.h>
#include <math.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;

FILE *probFP, *solnFP;

#define FIRST_SCAN_COUNT	50
#define SECOND_SCAN_COUNT	45
#define MAX_UPCS		128

char upcvals[MAX_UPCS][12];
BYTE fwdvals[12], fwdmasks[12], backvals[12], backmasks[12];
WORD fwddigits[12], backdigits[12];

BYTE lpattern[10] = 
{
	0x0d, 0x19, 0x13, 0x3d, 0x23, 0x31, 0x2f, 0x3b, 0x37, 0x0b
};
BYTE rpattern[10] = 
{
	0x72, 0x66, 0x6c, 0x42, 0x5c, 0x4e, 0x50, 0x44, 0x48, 0x74
};

char inbuf[256];

int ParseInData(int probnum)
{
	int i, j, k, ret;
	char c1, c2, c3, c4, c5;
	BYTE frmask, fval, brmask, bval, fmask, bmask;

	ret = 0;
	// get first row of data
	if(fgets(&(inbuf[0]), 255, probFP) == NULL) {
		fprintf(stderr, "Read failed on problem %d first row data\n", probnum);
		return -11;
	}
	// chack format
	for(i = 0; i < 50 ; i++) {
		c1 = inbuf[i];
		if((c1 != '0') && (c1 != '1') && (c1 != '?')) {
			fprintf(stderr, "bad %d char 0x%x on problem %d  first row data\n",
				i, c1, probnum);
			return -12;
		}
	}
	// check start
	c1 = inbuf[0]; c2 = inbuf[1]; c3 = inbuf[2];
	if(((c1 != '1') && (c1 != '?')) || 
		((c2 != '0') && (c2 != '?')) ||
		((c3 != '1') && (c3 != '?'))) {
		ret = 1;
	}
	// check middle
	c1 = inbuf[45]; c2 = inbuf[46]; c3 = inbuf[47]; c4 = inbuf[48]; c5 = inbuf[49];
	if(((c1 != '0') && (c1 != '?')) || 
		((c2 != '1') && (c2 != '?')) ||
		((c3 != '0') && (c3 != '?')) ||
		((c4 != '1') && (c4 != '?')) ||
		((c5 != '0') && (c5 != '?'))) {
		ret = 2;
	}
	// get first 6 digits of data
	for(i = 0, j = 3; i < 6 ; i++, j += 7) {
		frmask = fval = brmask = bval = 0;
		fmask = 0x40;
		bmask = 0x01;
		for(k = 0; k < 7 ; k++) {
			c1 = inbuf[j+k];
			if(c1 == '0') {	// valid and blank
				frmask |= fmask;
				brmask |= bmask;
			} else if(c1 == '1') { // valid and bar
				frmask |= fmask;
				brmask |= bmask;
				fval |= fmask;
				bval |= bmask;
			}	// else unknown char
			fmask >>= 1;
			bmask <<= 1;
		}
		fwdvals[i] = fval;
		fwdmasks[i] = frmask;
		backvals[11 - i] = bval;
		backmasks[11 - i] = brmask;
	}
	// get second row of data
	if(fgets(&(inbuf[0]), 255, probFP) == NULL) {
		fprintf(stderr, "Read failed on problem %d second row data\n", probnum);
		return -15;
	}
	// check format
	for(i = 0; i < 45 ; i++) {
		c1 = inbuf[i];
		if((c1 != '0') && (c1 != '1') && (c1 != '?')) {
			fprintf(stderr, "bad %d char 0x%x on problem %d  second row data\n",
				i, c1, probnum);
			return -16;
		}
	}
	// check end
	c1 = inbuf[42]; c2 = inbuf[43]; c3 = inbuf[44];
	if(((c1 != '1') && (c1 != '?')) || 
		((c2 != '0') && (c2 != '?')) ||
		((c3 != '1') && (c3 != '?'))) {
		fprintf(stderr, "bad end code %c%c%c on problem %d  data\n",
			c1, c2, c3, probnum);
		ret = 3;
	}
	// get second 6 digits of data
	for(i = 0, j = 0; i < 6 ; i++, j += 7) {
		frmask = fval = brmask = bval = 0;
		fmask = 0x40;
		bmask = 0x01;
		for(k = 0; k < 7 ; k++) {
			c1 = inbuf[j+k];
			if(c1 == '0') {	// valid and blank
				frmask |= fmask;
				brmask |= bmask;
			} else if(c1 == '1') { // valid and bar
				frmask |= fmask;
				brmask |= bmask;
				fval |= fmask;
				bval |= bmask;
			}	// else unknown
			fmask >>= 1;
			bmask <<= 1;
		}
		fwdvals[6+i] = fval;
		fwdmasks[6+i] = frmask;
		backvals[5 - i] = bval;
		backmasks[5 - i] = brmask;
	}
	return ret;
}

int ParseSolnData(int probnum)
{
	int index, numcodes, i, j;
	char c;

	if(fgets(&(inbuf[0]), 255, solnFP) == NULL)
	{
		fprintf(stderr, "Read failed on soln %d header\n", probnum);
		return -31;
	}
	if(sscanf(&(inbuf[0]), "%d %d", &index, &numcodes) != 2)
	{
		fprintf(stderr, "Scan failed on soln %d header\n", probnum);
		return -32;
	}
	if(index != probnum)
	{
		fprintf(stderr, "soln index %d not = expected problem %d\n",
			index, probnum);
		return -33;
	}
	if(numcodes == 0) {
		return 0;
	}
	if(numcodes > 9) {
		fprintf(stderr, "soln index %d num codes %d > 9\n",
			index, numcodes);
		return -34;
	}
	if(numcodes == 9) {
		numcodes = 8;
	}
	for(i = 0; i < numcodes ; i++) {
		if(fgets(&(inbuf[0]), 255, solnFP) == NULL)
		{
			fprintf(stderr, "Read failed on soln %d row %d data\n", probnum, i+1);
			return -35;
		}
		for(j = 0; j < 12 ; j++) {
			c = inbuf[j];
			if((c < '0') || (c > '9')) {
				fprintf(stderr, "Soln %d row %d char %d is not a digit\n",
					probnum, i+1, j+1);
				return -36;
			}
			upcvals[i][j] = c - '0';
		}
	}
	return numcodes;
}

int CheckUpcs(int probnum, int ncodes)
{
	int i, j, fwdok, backok;
	WORD mask;

	for(i = 0; i < ncodes ; i++) {
		fwdok = backok = 1;
		for(j = 0; j < 6 ; j++) {
			mask = lpattern[upcvals[i][j]];
			if((mask & fwdmasks[j]) != fwdvals[j]) fwdok = 0;
			if((mask & backmasks[j]) != backvals[j]) backok = 0;
		}
		for(j = 6; j < 12 ; j++) {
			mask = rpattern[upcvals[i][j]];
			if((mask & fwdmasks[j]) != fwdvals[j]) fwdok = 0;
			if((mask & backmasks[j]) != backvals[j]) backok = 0;
		}
		if((fwdok == 0) && (backok == 0)) {
			fprintf(stderr, "problem %d solution %d does not match\n",
				probnum, i);
			return -41;
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	int nprob, curprob, index, ret, ncodes;

	if(argc < 3) {
		fprintf(stderr, "USAGE: %s probelm_file solution_file\n", argv[0]);
		return -20;
		goto cleanup;
	}
	if((probFP = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Cannot open problem file \"%s\"\n", argv[1]);
		ret = -21;
		goto cleanup;
	}
	if((solnFP = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "Cannot open solution file \"%s\"\n", argv[2]);
		ret = -21;
		goto cleanup;
	}
	if(fgets(&(inbuf[0]), 255, probFP) == NULL)
	{
		fprintf(stderr, "Read failed on problem count\n");
		ret = -1;
		goto cleanup;
	}
	if(sscanf(&(inbuf[0]), "%d", &nprob) != 1)
	{
		fprintf(stderr, "Scan failed on problem count\n");
		ret = -2;
		goto cleanup;
	}
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, probFP) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d data\n", curprob);
			ret = -3;
			goto cleanup;
		}
		if(sscanf(&(inbuf[0]), "%d", &index) != 1)
		{
			fprintf(stderr, "Scan failed on problem %d data\n", curprob);
			ret = -4;
			goto cleanup;
		}
		if(index != curprob)
		{
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				index, curprob);
			ret = -5;
			goto cleanup;
		}
		ret = ParseInData(curprob);
		if(ret < 0) {
			goto cleanup;
		}
		ncodes = ParseSolnData(curprob);
		if(ncodes < 0) {
			ret = ncodes;
			goto cleanup;
		}
		if((ncodes > 0) && (ret > 0)) {
			fprintf(stderr, "problem %d ncodes %d > 0 with SME error\n",
				index, ncodes);
			ret = -6;
			goto cleanup;
		}
		if((ret = CheckUpcs(curprob, ncodes)) != 0) {
			goto cleanup;
		}
	}
cleanup:
	fclose(probFP);
	fclose(solnFP);
	return ret;
}
