/*
 * F - Adjacent Bit Counts
 * ACM International Collegiate Programming Contest
 * Greater New York Region
 * October 18, 2009
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define MAX_N   100
#define   MAX_PRE   10

char inbuf[256];

/*
 * let N(n,k) = the number of bit strings of length n and
 * adjacent bit count k
 * we split that into
 * N0(n,k) = number of bit strings ending in 0 with AdjBC = k
 * N1(n,k) = number of bit strings ending in 1 with AdjBC = k
 * so N(n,k) = N0(n,k) + N1(n,k)
 * the only way to get k adjacent 1 bits in a string ending in 0
 * is if left n-1 bits has k adjacent 1 bits
 * so N0(n,k) = N(n-1,k) = N0(n-1,k) + N1(n-1,k)
 * if the last bit is 1, we can get AdjBC = k if
 * a) the bit before last is 0 and the first n-1 bits have AdjBC=k OR
 * b) the bit before last is 1 and the first n-1 bits have AdjBC=k-1 i.e
 * N1(n,k) = N0(n-1,k) + N1(n-1,k-1)
 *
 * NOTE: N(n,k) = N0(n,k) = N1(n,k) = 0 if k < 0 or k >= n
 * also
 * N1(1,0) = 1, N0(1,0) = 1, N(n,n-1) = 1 (all 1 bits)
 */

int N0[MAX_N+1][MAX_N+1];
int N1[MAX_N+1][MAX_N+1];

int largestN = 0;

void AdjInit(void)
{
   int i, j;
   for(i = 0; i <= MAX_N ; i++)
   {
      for(j = 0; j <= MAX_N ; j++)
      {
         N0[i][j] = N1[i][j] = 0;
      }
   }
   N0[1][0] = N1[1][0] = 1;
   for(i = 2; i <= MAX_PRE ; i++)
   {
      N0[i][0] = N0[i-1][0] + N1[i-1][0];
      N1[i][0] = N0[i-1][0];
      for(j = 1; j < i; j++)
      {
         N0[i][j] = N0[i-1][j] + N1[i-1][j];
         N1[i][j] = N0[i-1][j] + N1[i-1][j-1];
      }
   }
   largestN = MAX_PRE;
}

int AdjBC(int n, int k)
{
   int i, j;

   if((k < 0) || (k >= n))
   {
      return 0;
   }
   if(k == n-1)
   {
      return 1;
   }
   if(n <= largestN)
   {
      if((N0[n][k] < 0) || (N1[n][k] < 0))
         return -1;
      else
         return (N0[n][k] + N1[n][k]);
   }
   for(i = largestN+1 ; i <= n ; i++)
   {
      if(N0[i-1][0] < 0)
      {
         N0[i][0] = N1[i][0] = -1;
      }
      else
      {
         N0[i][0] = N0[i-1][0] + N1[i-1][0];
         N1[i][0] = N0[i-1][0];
      }
      for(j = 1; j < i ; j++)
      {
         if((N0[i-1][j] < 0) || (N1[i-1][j] < 0))
         {
            N0[i][j] = -1;
         }
         else
         {
            N0[i][j] = N0[i-1][j] + N1[i-1][j];
         }
         if((N0[i-1][j] < 0) || (N1[i-1][j-1] < 0))
         {
            N1[i][j] = -1;
         }
         else
         {
            N1[i][j] = N0[i-1][j] + N1[i-1][j-1];
         }
      }
   }
   if((N0[n][k] < 0) || (N1[n][k] < 0))
      return -1;
   else
      return (N0[n][k] + N1[n][k]);
}

int main()
{
   int nprob, curprob, nbits, adjval, index;
   int ret;

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
   AdjInit();
   for(curprob = 1; curprob <= nprob ; curprob++)
   {
      if(fgets(&(inbuf[0]), 255, stdin) == NULL)
      {
         fprintf(stderr, "Read failed on operation count\n");
         return -3;
      }
      if(sscanf(&(inbuf[0]), "%d %d %d", &index, &nbits, &adjval) != 3)
      {
         fprintf(stderr, "Scan failed on problem count\n");
         return -4;
      }
      if(index != curprob)
      {
         fprintf(stderr, "problem index %d not = expected problem %d\n", index, curprob);
//         return -5;
      }
      ret = AdjBC(nbits, adjval);
      if(ret < 0)
      {
         printf("%d OVERFLOW\n", curprob);
      }
      else
      {
         printf("%d %d\n", curprob, ret);
      }
   }
   return 0;
}
