#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[])
{
	double d[12], r;
	int nScore[2], i, n;
	FILE *fpo;
	FILE *fpi = fopen("a.in", "rt");

	if(fpi == NULL){
		fprintf(stderr, "Can't open input a.in\n");
		return(1);
	}
	fpo = stdout;
	if(fpo == NULL){
		fprintf(stderr, "Can't open output a.out\n");
		fclose(fpi);
		return(2);
	}

	while(fscanf(fpi, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
		&(d[0]), &(d[1]), &(d[2]), &(d[3]), &(d[4]), &(d[5]),
		&(d[6]), &(d[7]), &(d[8]), &(d[9]), &(d[10]), &(d[11])) == 12 && d[0] != -100.){
		nScore[0] = nScore[1] = 0;
		for(i = 0; i < 12; i += 2){
			r = ::sqrt(d[i]*d[i] + d[i+1]*d[i+1]);
			if(r <= 3.){
				n = 100;
			} else if(r <= 6.){
				n = 80;
			} else if(r <= 9.){
				n = 60;
			} else if(r <= 12.){
				n = 40;
			} else if(r <= 15){
				n = 20;
			} else {
				n = 0;
			}
			nScore[i/6] += n;
		}
		fprintf(fpo, "SCORE: %d to %d, ", nScore[0], nScore[1]);
		if(nScore[0] == nScore[1]){
			fprintf(fpo, "TIE.\n");
		} else {
			fprintf(fpo, "PLAYER %d WINS.\n",
				nScore[0] < nScore[1] ? 2 : 1);
		}
	}
	fclose(fpi);
//	fclose(fpo);
	return 0;
}
