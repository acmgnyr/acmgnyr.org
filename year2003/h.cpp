// DiverseSets.cpp : Defines the entry point for the console application.
//


#include <stdio.h>
#include <stdlib.h>

int cursz = 0;
int *counts = NULL;

int array_alloc(int n)
{
	cursz = 0;
	if(counts != NULL)
	{
		delete [] counts;
		counts = NULL;
	}
	counts = new int[n];
	if(counts == NULL)
	{
		return -1;
	}
	cursz = n;
	return 0;
}

void array_clean()
{
	if(counts != NULL)
	{
		delete [] counts;
		counts = NULL;
	}
	cursz = 0;
}

int output_diverse(int n)
{
	int nelts, i, j, k, cnt;
	bool bBaseSet = false;

	nelts = (n * (n + 1))/2;	// total number of elts
	if((nelts & 1) != 0){	// must be even for solution
		printf("0\n\n");	// output no solution code
		return 0;
	}
	if(nelts > cursz){		// make sure we have space
		if((i = array_alloc(2*nelts)) != 0)
		{
			return i;
		}
	}
	// init counts
	for(i = 0; i < n; i++) counts[i] = i+1;
	// allocate elements to sets
	// output set count
	printf("%d\n", n);
	// output sets
	// on each pass, we take the first elt that has not finished
	// if the number of elts would be odd, we skip the next elt
	// else output the rest
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			if(counts[j] > 0)
			{
				break;
			}
		}
		if(j < n)
		{
			printf("%d ", j+1);
			counts[j]--;
			cnt = n - j;
			if((cnt > 2) && (cnt & 1) == 0)
			{
				printf("%d ", j+2);
				counts[j+1]--;
			}
			for(k = j+2; k < (n-1) ; k++)
			{
				printf("%d ", k+1);
				counts[k]--;
			}
			printf("%d\n", n);
			counts[n-1]--;
		}
	}
	printf("\n");
	return 0;
}

int main()
{
	char buf[256];
	int n, err;
    FILE *fp = fopen("h.in", "rt");
    if(fp == NULL){
        return 1;
    }

	if(array_alloc(128) != 0)
	{
		fprintf(stderr, "init alloc of 128 failed\n");
		return -2;
	}
	while(fgets(&(buf[0]), 256, fp) != NULL)
	{
		n = ::atoi(&(buf[0]));
		if(n <= 0)
		{
            fclose(fp);
			return 0;
		}
		else
		{
			if((err = output_diverse(n)) != 0)
			{
				fprintf(stderr, "output_diverse error %d\n", err);
				return -2;
			}
		}
	}
    fclose(fp);
	// should not get here
	return -1;
}