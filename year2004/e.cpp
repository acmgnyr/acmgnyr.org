#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_CARDS	6

char *faceChars = "A234567891JQK";
char *suitChars = "CDHS";

int faceVals[MAX_CARDS], suitVals[MAX_CARDS], sortVals[MAX_CARDS];
int faceRes[MAX_CARDS], suitRes[MAX_CARDS], sortRes[MAX_CARDS];

FILE *fpi, *fpo;

int
ParseInput(char *pBuf, int nCards)
{
	int cfnd, fval, sval, len, i;
	char *pres;

	if(nCards > MAX_CARDS)
	{
		return -5;
	}
	len = strlen(pBuf);
	i = 0;
	for(cfnd = 0; cfnd < nCards ; cfnd++)
	{
		/* skip leading spaces */
		while((i < len) && (isspace(pBuf[i])))
		{
			i++;
		}
		if(i >= (len - 2))
		{
			return -1;
		}
		pres = strchr(faceChars, pBuf[i]);
		if(pres == NULL)
		{
			return -2;
		}
		fval = pres - faceChars;
		i++;
		if(fval == 9)
		{
			if(pBuf[i] == '0')
			{
				i++;
				if(i >= len)
				{
					return -1;
				}
			}
			else
			{
				return -2;
			}
		}
		pres = strchr(suitChars, pBuf[i]);
		if(pres == NULL)
		{
			return -3;
		}
		i++;
		sval = pres - suitChars;
		faceVals[cfnd] = fval;
		suitVals[cfnd] = sval;
		sortVals[cfnd] = 4*fval + sval;
	}
	return cfnd;
}

int
DoAdd(int *vals, int addval)
{
	int a, b, c, floc;
	/* sort vals */
	a = vals[0]; b = vals[1]; c = vals[2]; 
	if(a < b)
	{
		if(a < c)
		{
			if(b > c){ vals[1] = c; vals[2] = b; }
		}
		else { vals[0] = c; vals[1] = a; vals[2] = b; }
	}
	else
	{
		if(b < c)
		{
			if(a < c){ vals[0] = b; vals[1] = a; }
			else  { vals[0] = b; vals[1] = c; vals[2] = a; }
		}
		else  { vals[0] = c; vals[1] = b; vals[2] = a; }
	}
	a = vals[0];
	if(addval > 3){ floc = addval - 3; b = vals[2]; c = vals[1]; }
	else {floc = addval;  b = vals[1]; c = vals[2];  }
	if(floc == 1)
	{
		faceRes[2] = a/4; suitRes[2] = a%4;
		faceRes[3] = b/4; suitRes[3] = b%4;
		faceRes[4] = c/4; suitRes[4] = c%4;
	}
	else if(floc == 2)
	{
		faceRes[3] = a/4; suitRes[3] = a%4;
		faceRes[2] = b/4; suitRes[2] = b%4;
		faceRes[4] = c/4; suitRes[4] = c%4;
	}
	else
	{
		faceRes[4] = a/4; suitRes[4] = a%4;
		faceRes[2] = b/4; suitRes[2] = b%4;
		faceRes[3] = c/4; suitRes[3] = c%4;
	}

	return 0;
}

int
DoAssist()
{
	int suitLoc[4], i, ret1, ret2, face1, face2, suit;
	int addVals[3], j, add;
	/* find two cards with same suit */
	ret1 = ret2 = -1;
	for(i = 0; i < 4; i++) suitLoc[i] = -1;
	for(i = 0; i < 5; i++)
	{
		if(suitLoc[suitVals[i]] >= 0)
		{	/* found dup suit */
			ret1 = suitLoc[suitVals[i]];
			ret2 = i;
			suit = suitVals[i];
			face1 = faceVals[ret1];
			face2 = faceVals[ret2];
			break;
		}
		else suitLoc[suitVals[i]] = i;
	}
	/* save addvals */
	for(i = 0, j = 0; i < 5 ; i++)
	{
		if((i != ret1) && (i != ret2))
		{
			addVals[j++] = sortVals[i];
		}
	}
	suitRes[0] = suitRes[1] = suit;
	/* figure out which face is hidden */
	if(face1 < face2)
	{
		if((face2 - face1) <= 6)
		{
			faceRes[0] = face2;
			faceRes[1] = face1;
			add = face2 - face1;
		}
		else
		{
			faceRes[0] = face1;
			faceRes[1] = face2;
			add = face1 - face2 + 13;
		}
	}
	else
	{
		if((face1 - face2) <= 6)
		{
			faceRes[0] = face1;
			faceRes[1] = face2;
			add = face1 - face2;
		}
		else
		{
			faceRes[0] = face2;
			faceRes[1] = face1;
			add = face2 - face1 + 13;
		}
	}
	DoAdd(addVals, add);
	return 0;
}

void
PrintAssist()
{
	int i;
	for(i = 0; i < 5; i++)
	{
		fprintf(fpo, "%c", faceChars[faceRes[i]]);
		if(faceRes[i] == 9) fprintf(fpo, "0");
		if(i == 4) fprintf(fpo, "%c\n", suitChars[suitRes[i]]);
		else fprintf(fpo, "%c ", suitChars[suitRes[i]]);
	}
}

char inbuf[256];

int
main()
{
	int nprobs, probnum, ret;

	fpi = fopen("e.in", "rt");
	if(fpi == NULL){
		fprintf(stderr, "Can't open input e.in\n");
		return 1;
	}
	fpo = stdout;
	if(fpo == NULL){
		fprintf(stderr, "Can't open output e.out\n");
		return 2;
	}
	if(fgets(&(inbuf[0]), 256, fpi) == NULL)
	{
		fprintf(stderr, "read error getting count\n");
		return -10;
	}
	if(sscanf(&(inbuf[0]), "%d", &nprobs) != 1)
	{
		fprintf(stderr, "scan error getting count\n");
		return -11;
	}
	for(probnum = 0; probnum < nprobs ; probnum++)
	{
		if(fgets(&(inbuf[0]), 256, fpi) == NULL)
		{
			fprintf(stderr, "read error on problem %d\n", probnum + 1);
			return -12;
		}
		ret = ParseInput(&(inbuf[0]), 5);
		if(ret != 5)
		{
			return ret;
		}
		ret = DoAssist();
		if( ret != 0)
		{
			return ret;
		}
		PrintAssist();
	}
	fclose(fpi);
//	fclose(fpo);
	return 0;
}
