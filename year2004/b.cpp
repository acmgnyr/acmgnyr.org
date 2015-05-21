#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pow26[] = {
	1, 26, 26*26, 26*26*26, 26*26*26*26, 26*26*26*26*26, 26*26*26*26*26*26
};

/*
 * Uses magic formula
 */
char *MapCol3(int nCol)
{
	int i, x;
	static char obuf[10];
	char *p = obuf;

	for(i = 5; i >= 0; i--){
		x = (1 - pow26[i+1])/(-25) - 1;
		if(nCol > x){
			*p++ = 'A' + ((nCol - x - 1)/pow26[i])%26;
		}
	}
	*p = '\0';
	return(obuf);
}

/*
 * Uses brute force
 */
char *MapCol2(int nCol)
{
	static char obuf[10];
	char *p = obuf;

	if(nCol > 12356630){
		*p++ = 'A' + ((nCol-12356631)/11881376) % 26;
	}
	if(nCol > 475254){
		*p++ = 'A' + ((nCol-475255)/456976) % 26;
	}
	if(nCol > 18278){
		*p++ = 'A' + ((nCol-18279)/17576) % 26;
	}
	if(nCol > 702){
		*p++ = 'A' + ((nCol-703)/676) % 26;
	}
	if(nCol > 26){
		*p++ = 'A' + ((nCol - 27)/26) % 26;
	}
	*p++ = 'A' + (nCol - 1) % 26;
	*p = '\0';
	return(obuf);

}


int main(int argc, char* argv[])
{
	char ibuf[128], *s;
	int nRow, nCol;
	int alg = 0;
	FILE *fpi, *fpo;

	/*
	 * Algorith choice.  If arg present, use brute force, else
	 * use magic formula
	 */
	if(argc > 1){
		alg = 1;
	}

	fpi = fopen("b.in", "rt");
	if(fpi == NULL){
		fprintf(stderr, "Can't open input b.in\n");
		return(1);
	}
	fpo = stdout;
	if(fpo == NULL){
		fprintf(stderr, "Can't open output b.out\n");
		return(2);
	}

	/* Datasets are GUARANTEED to be correct */
	while(fgets(&(ibuf[0]), sizeof(ibuf)-1, fpi) != NULL){
		if(ibuf[0] != 'R'){
			break;
		}
		nRow = ::atoi(&(ibuf[1]));
		s = ::strchr(&(ibuf[0]), 'C');
		if(s == NULL){
			break;
		}
		s++;
		nCol = ::atoi(s);
		// Exit case ?
		if(nRow == 0 && nCol == 0){
			break;
		}
		::fprintf(fpo, "%s%d\n", alg ? MapCol3(nCol) : MapCol2(nCol), nRow);
	}
	fclose(fpi);
//	fclose(fpo);
	return 0;
}

