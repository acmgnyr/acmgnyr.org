/*
 * A - Repeating Characters
 * Greater NY Regional, Oct 30, 2011
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	MAX_REPEAT	8
#define	MAX_CHARS	20

int main(int argc, char **argv)
{
	char szBuf[64], szStr[32], szNew[MAX_REPEAT * MAX_CHARS + 1];
	int n, np, ds, r, i, io, idx, nLen;

	if(::fgets(&(szBuf[0]), sizeof(szBuf)-1, stdin) != NULL){
		np = ::atoi(&(szBuf[0]));
		for(n = 1; n <= np; n++){
			if(::fgets(&(szBuf[0]), sizeof(szBuf)-1, stdin) == NULL){
				break;
			}
			if(::sscanf(&(szBuf[0]), "%d %d %s", &(ds), &(r), &(szStr[0])) != 3){
				break;
			}
			if(ds != n){
				::fprintf(stderr, "Problem number mismatch: %d != %d\n", n, ds);
				break;
			}
			nLen = ::strlen(&(szStr[0]));
			for(io = 0, idx = 0; idx < nLen; idx++){
				for(i = 0; i < r; i++){
					szNew[io++] = szStr[idx];
				}
			}
			szNew[io] = '\0';
			::fprintf(stdout, "%d %s\n", n, &(szNew[0]));
		}
	}
	return(0);
}
