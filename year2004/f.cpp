/* 
 * formula derivation:
 * choose corrd system with origin at center theodolite
 * x- axis along the lien of observers xy plane horizontal
 * z = distance above palneo theodolites so ht of rocket = z + h
 * so observer theodolites are at (-D,0,0) (0,0,0) and (D,0,0)
 * tan(alpha) = z/((x+D)^2 + y^2)
 * tan(beta) = z/(x^2 + y^2)
 * tan(gamma) = z/((x-D)^2 + y^2)
 * let a = 1/(tan(alpha))^2, b = 1/(tan(beta))^2, c = 1/(tan(gamma))^2
 * a*z^2 = (x+D)^2 + y^2 = x^2 + 2*D*x + D^2 + y^2
 * b*z^2 = x^2 + y^2 
 * c*z^2 = (x-D)^2 + y^2 = x^2 - 2*D*x + D^2 + y^2
 * a*z^2 - b*z^2 = 2*D*x + D^2
 * c*z^2 - b*z^2 = -2*D*x + D^2
 * a*z^2 +c*z^2 - 2*b*z^2 = 2*D^2
 * z^2 = (2*D^2)/(a + c -2*b)
 * z = D*sqrt(2/(a + c - 2*b))
 * altitude= z + h = D*sqrt(2/(a + c - 2*b)) + h
 */
#include <stdio.h>
#include <math.h>
#define EPS	(1.0E-10)
#define BIG (1,0E10)

double RadsPerDegree;

double FindAltitude(double d, double h, double alpha, double beta, double gamma)
{
	double a, b, c, z, temp;

	a = tan(alpha * RadsPerDegree);
	a = 1.0/(a*a);
	b = tan(beta * RadsPerDegree);
	b = 1.0/(b*b);
	c = tan(gamma * RadsPerDegree);
	c = 1.0/(c*c);
	temp = a + c - 2*b;
	if(temp < EPS)
	{
		return BIG;
	}
	z = sqrt(2.0/temp);
	z = z * d + h;
	return z;
}

char inbuf[256];

int main()
{
	double d, h, alpha, beta, gamma, ht;
	int nprob;
	FILE *fpi, *fpo;

	fpi = fopen("f.in", "rt");
	if(fpi == NULL){
		fprintf(stderr, "Can't open input f.in\n");
		return 1;
	}
	fpo = stdout;
	if(fpo == NULL){
		fprintf(stderr, "Can't open output f.out\n");
		return 2;
	}

	RadsPerDegree = atan2(1.0, 1.0)/45.0;
	if(fgets(&(inbuf[0]), 255, fpi) == NULL)
	{
		fprintf(stderr, "End of file reading theodolite separation and height\n");
		return -1;
	}
	if(sscanf(&(inbuf[0]), "%lf %lf", &d, &h) != 2)
	{
		fprintf(stderr, "Scan failed on theodolite separation and height\n");
		return -2;
	}
	nprob = 0;
	for(;;)
	{
		nprob++;
		if(fgets(&(inbuf[0]), 255, fpi) == NULL)
		{
			fprintf(stderr, "End of file reading problem %d\n", nprob);
			return -3;
		}
		if(sscanf(&(inbuf[0]), "%lf %lf %lf", &alpha, &beta, &gamma) != 3)
		{
			fprintf(stderr, "Scan failed on problem %d\n", nprob);
			return -4;
		}
		if((alpha == 0.0) || (beta == 0.0) || (gamma == 0.0))
		{
			return 0;
		}
		ht = FindAltitude(d, h, alpha, beta, gamma);
		fprintf(fpo, "%.0f\n", ht);
	}
	fclose(fpi);
//	fclose(fpo);
	return 0;
}
