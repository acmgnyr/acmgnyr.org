/*
 * F - Chomp game
 * ICPC 2013 Greater NY Regional
 * Solution by Fred Pickel
 * Problem by Fred Pickel
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define MAX_INPUT	100
char inbuf[256];
int LoserP[MAX_INPUT+1][MAX_INPUT+1];
int test[2*MAX_INPUT+1];	/* LoserP(q,r) <= LoserP(q,q) <= 2*q */
/*
 * for each value of q >= r there is at most 1 value of p > r so that p,(min(p,q),r is a losing position
 *	(if there were a larger p1 > p, we could play (p+1,1) to get to p,(min(p,q),r so it would be a winning position
 * MakeLosers finds this p value for each 0<=r<=q<=100 by induction
 */
int MakeLosers()
{
	int q, r, t, max;
	LoserP[0][0] = 1;	/* the poisoned piece alone */
	LoserP[1][0] = 2;	/* the 2 block equal leg L-shape */
	LoserP[1][1] = 3;	/* the 3 block equal leg L-shape */
	/*
	 * (q+1,q,0) is always a losing position
	 * if you play (r+1,1) (r <= q) to get to (r,r,0), opponent plays (r,2) to get to (r,r-1,0) lose by induction
	 * if you play (r,2) (r<= q) ro get to (q+1,r-1,0) opponent plays (r+1,1) to get to (r,r-1,0) lose by induction
	 */
	for(q = 2; q <= MAX_INPUT ; q++) {
		LoserP[q][0] = q+1;
	}
	/*
	 * for r > 0, and q >= r, we mark all winning positions (p,q,r) in test
	 * and then take the smallest unmarked position for LoerP[q][r]
	 * max keeps track of the largest marked p
	 */
	memset(&(test[0]), 0, (MAX_INPUT+1)*sizeof(int));
	for(r = 1; r <= MAX_INPUT ; r++) {
		for(q = r; q <= MAX_INPUT ; q++) {
			max = 0;
			for(t = 0; t < r; t++) {
				test[LoserP[q][t]] = 1;		/* if (p,q,t) (t<r) is  a loser, the play (t+1,3) takes
												us from (p,q,r) to (p,q,t) so (p,q,r) is a winner */
				if(max < LoserP[q][t]) max = LoserP[q][t];
				test[LoserP[t][t]] = 1;		/* if (p,t,t) (t<r) is  a loser, the play (t+1,2) takes
												us from (p,q,r) to (p,t,t) so (p,q,r) is a winner */
				if(max < LoserP[t][t]) max = LoserP[t][t];
			}
			for(t = r; t < q; t++) {
				test[LoserP[t][r]] = 1;		/* if (p,t,r) (t<q) is  a loser, the play (t+1,2) takes
												us from (p,q,r) to (p,t,r) so (p,q,r) ias a winner */
				if(max < LoserP[t][r]) max = LoserP[t][r];
			}
			/* marked all the winners, scanfro the first unmarked as the loser */
			for(t = 1; t <= max+1 ; t++) {
				if(test[t] == 0) {
					LoserP[q][r] = t;
					break;
				}
			}
			memset(&(test[0]), 0, (max+1)*sizeof(int));	/* reset test */
			/* if (q,q,r) is a loser, then for any p>=t>q we can move from (p,t,r) to (q,q,r) by (q+1,1) */
			if(LoserP[q][r] == q) {
				for(t = q+1; t <= MAX_INPUT; t++) {
					LoserP[t][r] = q;
				}
				break;
			}
		}
	}
	return 0;
}

int main()
{
	int nprob, curprob, index, p, q, r, i, found;

	if(fgets(&(inbuf[0]), 255, stdin) == NULL)
	{
		fprintf(stderr, "Read failed on problem count\n");
		return -1;
	}
	if(sscanf(&(inbuf[0]), "%d", &nprob) != 1)
	{
		fprintf(stderr, "Scan failed on problem count\n");
		return -2;
	}
	/* make the loser list */
	if(MakeLosers() < 0) {
		return -20;
	}
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d data\n", curprob);
			return -3;
		}
		// get prob num and sequence index
		if(sscanf(&(inbuf[0]), "%d %d %d %d", &index, &p, &q, &r) != 4)
		{
			fprintf(stderr, "scan failed on problem header problem index %d\n",
				curprob);
			return -6;
		}
		if(index != curprob)
		{
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				index, curprob);
			return -7;
		}
		if((q > p) || (r > q)) {
			fprintf(stderr, "problem index %d p %d > MAX_INPUT %d OR p < q %d OR q < r %d\n",
				curprob, p, MAX_INPUT, q, r);
			return -8;
		}
		if(LoserP[q][r] == p) {
			printf("%d L\n", curprob);
		} else {
			/* it is not a loser so it better be a winner, look for a move to a loser */
			if(LoserP[q][r] < p) {	/* if (p1,q,r) (p1<p) is a loser, play (p+1,1) to get there */
				printf("%d W %d 1\n", curprob, LoserP[q][r]+1);
			} else {
				found = 0;
				for(i = 0; i < r ; i++) {
					if(LoserP[q][i] == p) {		/* if (p,q,i) (i<r) is a loser, play (i+1,3) to get there */
						printf("%d W %d 3\n", curprob, i+1);
						found = 1;
						break;
					} else if(LoserP[i][i] == p) {	/* if(p,i,i) (i < r) is a loser, play (i+1,2) to get there */
						printf("%d W %d 2\n", curprob, i+1);
						found = 1;
						break;
					} 
				}
				if(found == 0) {
					for(i = r; i < q; i++) {
						if((LoserP[i][r] == p)) {	/* if(p,i,r) (r < i) is a loser, play (i+1,2) to get there */
							printf("%d W %d 2\n", curprob, i+1);
							found = 1;
							break;
						}
					}
				}
				if(found == 0) {	/* oops */
					printf("prob %d p %d q %d r %d not loser, no move found\n",
						curprob, p, q, r);
				}
			}
		}
	}
	return 0;
}
