/*
 * Binary Clock 
 * Problem concept by Adrian Michel Gonzalez (adrianmichel@iteso.mx)
 *
 * C++ Solution
 * by John Buck
 * October 2008
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void encodeh(char *s, int n)
{
	int bit = 32;
	while(bit > 0){
		if(n & bit){
			*s++ = '1';
		} else {
			*s++ = '0';
		}
		bit >>= 1;
	}
}

void encodev(char *s, int n)
{
	int bit = 32;
	while(bit > 0){
		if(n & bit){
			*s = '1';
		} else {
			*s = '0';
		}
		s += 3;
		bit >>= 1;
	}
}

int main(int argc, char* argv[])
{
	char szbuf[80];
	int i, n, h, m, s;

	fgets(&(szbuf[0]), sizeof(szbuf), stdin);
	n = atoi(&(szbuf[0]));
	for(i = 0; i < n; i++){
		if(fgets(&(szbuf[0]), sizeof(szbuf), stdin) == NULL){
			break;
		}
		h = atoi(&(szbuf[0]));
		m = atoi(&(szbuf[3]));
		s = atoi(&(szbuf[6]));
		encodev(&(szbuf[0]), h);
		encodev(&(szbuf[1]), m);
		encodev(&(szbuf[2]), s);
		szbuf[18] = '\0';
		printf("%d", i+1);
		printf(" %s", &(szbuf[0]));
		encodeh(&(szbuf[0]), h);
		encodeh(&(szbuf[6]), m);
		encodeh(&(szbuf[12]), s);
		printf(" %s\n", &(szbuf[0]));
	}
	return 0;
}
