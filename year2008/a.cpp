// lcmgcf.cpp : Defines the entry point for the console application.
//
// John Buck
// Fall 2008  ACM GNYR Regional contest

#include <stdio.h>
#include <stdlib.h>

/* Euclid */
int gcd1(int a, int b)
{
	int t;

	while(b != 0){
		t = b;
		b = a % b;
		a = t;
	}
	return(a);
}

/* Recursive */
int gcd2(int a, int b)
{
	if(b != 0){
		return(gcd2(b, a % b));
	}
	return(a);
}

int main(int argc, char* argv[])
{
	char szbuf[128];
	int i, n, i1, i2, g;

	fgets(&(szbuf[0]), sizeof(szbuf), stdin);
	n = atoi(&(szbuf[0]));
	for(i = 1; i <= n; i++){
		if(fgets(&(szbuf[0]), sizeof(szbuf), stdin) == NULL){
			break;
		}
		if(sscanf(&(szbuf[0]), "%d %d", &(i1), &(i2)) != 2){
			break;
		}
		/* Safety */
		if(i1 == 0 || i2 == 0){
			break;
		}
#ifndef DO_RECURSIVE
		g = gcd1(i1, i2);
#else
		g = gcd2(i1, i2);
#endif
		/* print out prob #, lcm and gcf */
		printf("%d %d %d\n", i, (i1*i2)/g, g);
	}
	return 0;
}
