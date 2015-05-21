#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define N_TERMS 12

int main()
{
	int nprob, curprob, index, seq[N_TERMS], lev[N_TERMS+1], ind, level, prev, cur, nisland;

	if(scanf("%d", &nprob) != 1)
	{
		fprintf(stderr, "Scan failed on problem count\n");
		return -2;
	}
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		// get prob num and sequence index
		scanf("%d", &index);
		level = prev = nisland = 0;
		scanf("%d", &cur);
		lev[0] = prev = cur;
		for(ind = 1; ind < N_TERMS ; ind++) {
			scanf("%d", &cur);
			seq[ind] = cur;
			if(cur > prev) {
				level++;
				nisland++;
				lev[level] = cur;
			} else if(cur < prev) {
				while((level > 0) && (cur < lev[level])) {
					level--;
				}
				if(cur > lev[level]) {
					level++;
					nisland++;
					lev[level] = cur;
				}
			}
			prev = cur;
		}
		printf("%d %d\n", curprob, nisland);

	}
	return 0;
}
