#include <stdio.h>
#include <math.h>


char inbuf[256];
int main()
{
	int nprob, curprob, index, row, col, result;

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
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d data\n", curprob);
			return -3;
		}
		if(sscanf(&(inbuf[0]), "%d %d %d", &index, &row, &col) != 3)
		{
			fprintf(stderr, "Scan failed on problem %d data\n", curprob);
			return -4;
		}
		if(index != curprob)
		{
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				index, curprob);
			return -7;
		}
		if((row < 0) || (col < 0) || (col > row)) {
			fprintf(stderr, "problem index %d bad row %d or col %d\n",
				index, row, col);
			return -8;
		}
		result = col * (row - col) + 1;
		printf("%d %d\n", curprob, result);
	}
	return 0;
}
