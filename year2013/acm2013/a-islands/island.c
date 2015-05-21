/*
 * A - Islands in the Data Stream
 * ICPC 2013 Greater NY Regional
 * Solution by Fred Pickel
 * Problem by Fred Pickel
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define N_TERMS 15

int main()
{
	int nprob, curprob, index, cur, prev, nisland, ind;

	if(scanf("%d", &nprob) != 1)
	{
		fprintf(stderr, "Scan failed on problem count\n");
		return -2;
	}
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		// get prob num and sequence index
		scanf("%d", &index);
		nisland = 0;
		scanf("%d", &cur);
		prev = cur;
		for(ind = 1; ind < N_TERMS ; ind++) {
			scanf("%d", &cur);
			if(cur > prev) {
				nisland++;
			}
			prev = cur;
		}
		printf("%d %d\n", index, nisland);

	}
	return 0;
}
