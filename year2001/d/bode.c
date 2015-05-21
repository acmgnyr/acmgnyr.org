#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define FILE_IN "bode.in"
#define FILE_OUT "bode.out"

void main(void)
{
  FILE* fpin;
  FILE* fpout;
  float VS, R, C;
  int n;
  int i;

  /* Open the input file. */
  fpin = fopen(FILE_IN, "r");
  if (fpin == NULL)
    {
      printf("Error: could not open '%s' for reading\n", FILE_IN);
      exit(-1);
    }

  /* Open the output file. */
  fpout = fopen(FILE_OUT, "w");
  if (fpout == NULL)
    {
      printf("Error: could not open '%s' for writing\n", FILE_OUT);
      exit(-1);
    }

  /* Read the four values. */
  fscanf(fpin, "%f %f %f %d", &VS, &R, &C, &n);

  /* Read the frequencies, and output VR. */
  for (i = 0; i < n; i++)
    {
      float omega;
      float x;
      float VR;

      fscanf(fpin, "%f", &omega);
      x = omega * R * C;
      VR = VS * x / sqrt(1 + x*x);
      fprintf(fpout, "%0.3f\n", VR);
    }

  /* Close the files. */
  fclose(fpin);
  fclose(fpout);
}

      

