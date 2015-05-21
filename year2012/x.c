/*
 * X - eXperimental Problem
 * ICPC 2012 Greater NY Regional
 * Solution by John buck
 * Problem by John Buck
 */

/*
 * This is a simple decoding algorithm.  The string that is read in
 * is the dictionary. The values are added to the current index (zero based)
 * and the character in the dictionary at the position is the decoded character.
 * Wrap-arounds are handled.
 */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	char szBuf[80], szMap[128];
	int p, d, n, x, nd, i, sgn, c, pos, nstate, bdone;

	/* Get number of data sets */
	if(fgets(&(szBuf[0]), sizeof(szBuf)-1, stdin) != NULL){
		p = atoi(&(szBuf[0]));
		/* Process each data set individually */
		for(d = 1; d <= p; d++){
			/* Get data set number */
			if(fgets(&(szBuf[0]), sizeof(szBuf)-1, stdin) == NULL){
				break;
			}
			nd = atoi(&(szBuf[0]));
			/* make sure it's right */
			if(nd != d){
				printf("problem number mismatch: %d != %d\n", nd, d);
				break;
			}
			/* Read the dictionary map */
			if(fgets(&(szMap[0]), sizeof(szMap), stdin) == NULL){
				break;
			}
			/* Determine length of dictionary by looking for end of line */
			for(i = 0; szMap[i] != '\0' && szMap[i] != '\r' && szMap[i] != '\n'; i++){
				;
			}
			/* read in the number of indicies */
			if(fgets(&(szBuf[0]), sizeof(szBuf)-1, stdin) == NULL){
				break;
			}
			n = atoi(&(szBuf[0]));
			/* Display data set number */
			printf("%d ", d);
			pos = 0;
			nstate = 0;
			/* Mickey Mouse state machine to read the indicies one at-a-time.
			 * since the line can be quite long, we dont read this as a single
			 * buffer.
			 */
			for(bdone = 0; !bdone;){
				c = getc(stdin);
				switch(nstate){
				case 0:			/* First digit or - */
					x = 0;
					if(c == ' '){
						break;
					}
					if(c == '-'){
						nstate = 1;
						sgn = -1;
						break;
					} else 	if(isdigit(c)){
						sgn = 1;
						nstate = 1;
					} else if(c == EOF || c == '\n' || c == '\r'){
						bdone = 1;
						break;
					}
				case 1:			/* Digits or end of number */
					if(isdigit(c)){
						x = x * 10 + c - '0';
					} else if(c == ' ' || c == '\n'){
						pos += (x*sgn);
						if(pos >= i){
							pos -= i;
						}
						if(pos < 0){
							pos += i;
						}
						printf("%c", szMap[pos]);
						nstate = 0;
						n--;
						/* If no more to do, then we're done */
						if(n == 0){
							bdone = 1;
						}
					}
					break;
				}
			}
			/* If there was extra fluff on the line, suck it up so we stay in sync */
			if(!bdone){
				while((c = getc(stdin)) != '\n' && c != EOF){
					;
				}
			}
			printf("\n");
		}
	}
	return(0);
}

