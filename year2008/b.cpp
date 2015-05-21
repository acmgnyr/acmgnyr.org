/*
 * Cryptoquote solution
 * by John Buck
 * October 2008
 */

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
	int i, j, n;
	char szbuf[128], szkey[40];

	fgets(&(szbuf[0]), sizeof(szbuf), stdin);
	n = atoi(&(szbuf[0]));
	for(i = 1; i <= n; i++){
		if(fgets(&(szbuf[0]), sizeof(szbuf), stdin) == NULL ||
			fgets(&(szkey[0]), sizeof(szkey), stdin) == NULL){
			break;
		}
		fprintf(stdout, "%d ", i);
		for(j = 0; szbuf[j] != '\r' && szbuf[j] != '\n' && szbuf[j] != '\0'; j++){
			if(szbuf[j] == ' '){
				putc(' ', stdout);
			} else {
				putc(szkey[szbuf[j]-'A'], stdout);
			}
		}
		putc('\n', stdout);
	}
	return 0;
}
