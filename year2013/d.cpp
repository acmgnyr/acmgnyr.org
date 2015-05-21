/*
 * D - Pisano
 * ICPC 2013 Greater NY Regional
 * Solution by Adam Florence
 * Problem by Adam Florence
 */
#include <stdio.h>

/*
 * OS agnostic getline
 */
char *getline(char  *pBuf, int nMax)
{
	int c;
	char *pOrig = pBuf;

	/* Leave room for null byte */
	nMax--;
	while((c = fgetc(stdin)) != EOF){
		if(c == '\r' || c == '\n'){
			break;
		}
		if(nMax > 0){
			*pBuf++ = c;
			nMax--;
		}
	}
	*pBuf = '\0';
	return(pOrig);
}

int k(int m)
{
	/*
	 * int's are fine since max remainder is (x%1000000) <= 999999
	 */
	int nSeq, f1, f2;
	
	/* Special case is easy */
	if(m == 2){
		return(3);
	}
	nSeq = 2;
	/* Prime Fibonacci pump */
	f1 = 1;
	f2 = 1;
	for(;; nSeq += 2){
		/*
		 * we do 2 values at a time, since if m > 2, k(m) is even
		 */
		/* Next value in sequence remainder */
		f1 = (f1 + f2) % m;
		/* Value after that remainder */
		f2 = (f2 + f1) % m;
		/* Sequence repeats when next 2 values match first 2 which
		 * are always 1
		 */
		if(f1 == 1 && f2 == 1){
			break;
		}
	}
	return(nSeq);
}

int main(int argc, char **argv)
{
	int p, i, n, m;
	char szBuf[256];

	if(getline(&(szBuf[0]), sizeof(szBuf)) != NULL){
		if(sscanf(&(szBuf[0]), "%d", &(p)) == 1){
			for(i = 1; i <= p; i++){
				if(getline(&(szBuf[0]), sizeof(szBuf)) == NULL){
					break;
				}
				if(sscanf(&(szBuf[0]), "%d %d", &(n), &(m)) != 2){
					break;
				}
				if(n != i){
					fprintf(stderr, "Problem number mismatch: %d should be %d\n",
						n, i);
					break;
				}
				printf("%d %d\n", i, k(m));
			}
		}
	}
	return(0);
}
