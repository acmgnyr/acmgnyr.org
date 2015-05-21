#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define	NUM_STUDENTS	20

int main(int argc, char **argv)
{
	int nprob, curprob, thisprob, nstud, thisstud, i, j, nmove;
	int arht[NUM_STUDENTS];

	if(scanf("%d", &(nprob)) != 1)
	{
		fprintf(stderr, "Scan failed on problem count\n");
		return -2;
	}
	/*
	 * Process each problem independently
	 */
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		nmove = 0;

		// Fetch problem number
		scanf("%d", &(thisprob));

		// Process each student's height
		for(nstud = 0; nstud < NUM_STUDENTS; nstud++) {
			 // Get height
			scanf("%d", &(thisstud));
			// Find first student taller
			for(i = 0; i < nstud; i++){
				if(arht[i] > thisstud){
					 // Have everyone take a step back (starting from end of line)
					for(j = nstud; j > i; j--){
						arht[j] = arht[j-1];
						// Count step back
						nmove++;
					}
					// Found where we go, so break out
					break;
				}
			}
			// Stand in line
			arht[i] = thisstud;
		}
		// Debug
		//printf("Prob %d:", thisprob);
		//for(j = 0; j < 20; j++){
		//	printf(" %d", j);
		//}
		//printf("\n");

		printf("%d %d\n", thisprob, nmove);
	}

	return(0);
}
