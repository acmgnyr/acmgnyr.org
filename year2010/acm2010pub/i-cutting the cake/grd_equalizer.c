#include <stdio.h>
#include <math.h>

#define EPS (.001)

int CheckEqualizer(int probnum, double *pVerts, double *pResult)
{
	double vAB[2], vAC[2], vBC[2];
	double a, b, c, r, s, t, denom, perimov2;
	double rA, rB, rC;
	rA = pResult[0];
	rB = pResult[1];
	rC = pResult[2];

	if(fabs((rA*rA + rB*rB) - 1.0) > EPS) {
		fprintf(stderr, "%d: rA %f and  rB %f norm %f != 1.0\n",
			probnum, rA, rB, rA*rA + rB*rB);
		return -20;
	}
	vAB[0] = pVerts[2] - pVerts[0];
	vAB[1] = pVerts[3] - pVerts[1];
	vAC[0] = pVerts[4] - pVerts[0];
	vAC[1] = pVerts[5] - pVerts[1];
	vBC[0] = pVerts[4] - pVerts[2];
	vBC[1] = pVerts[5] - pVerts[3];
	a = sqrt(vBC[0]*vBC[0] + vBC[1]*vBC[1]);
	b = sqrt(vAC[0]*vAC[0] + vAC[1]*vAC[1]);
	c = sqrt(vAB[0]*vAB[0] + vAB[1]*vAB[1]);
	perimov2 = 0.5*(a+b+c);
	// find intersection with each side
	// side eqn = pt + t*vect want this to satisfy AX + By = C
	// A(ptx + t * vectx) + B(pty + t * vecty) = C
	// t * (A*vectx + B*vecty) = C - A*ptx -B*pty
	r = s = t = 1.0e30;
	denom = rA*vAB[0] + rB*vAB[1];
	if(fabs(denom) > EPS) {
		r = (rC - rA*pVerts[0] - rB*pVerts[1])/denom;
	}
	denom = rA*vAC[0] + rB*vAC[1];
	if(fabs(denom) > EPS) {
		s = (rC - rA*pVerts[0] - rB*pVerts[1])/denom;
	}
	denom = rA*vBC[0] + rB*vBC[1];
	if(fabs(denom) > EPS) {
		t = (rC - rA*pVerts[2] - rB*pVerts[3])/denom;
	}
	if((r < -EPS) || (r > (1.0 + EPS))) {
		if((s  < -EPS) || (s > (1.0 + EPS)) || (t < -EPS) || (t > (1.0 + EPS)))
		{
			fprintf(stderr, "%d: r %f s %f t %f outside triangle\n",
				probnum, r, s, t);
			return -1;
		}
		// intersect AC and BC cut off C need other end of both
		s = 1.0 - s;
		t = 1.0 - t;
		if(fabs(s*t - 0.5) > EPS) {
			fprintf(stderr, "%d: s %f t % f s*t %f != 0.5\n",
				probnum, s, t, s*t);
			return -2;
		}
		if(fabs(s*b+t*a - perimov2) > 0.5) {
			fprintf(stderr, "%d: s %f t % f s*b + t*a %f != %f\n",
				probnum, s, t, s*b+t*a, perimov2);
			return -3;
		}
	} else if(r < EPS) {
		// goes thru A
		if(fabs(t - 0.5) > EPS) {
			fprintf(stderr, "%d: r %f s %f t %f != 0.5\n",
				probnum, r, s, t);
			return -10;
		}
		if(fabs(b - c) > EPS) {
			fprintf(stderr, "%d: r %f s %f t %f b %f != c %f \n",
				probnum, r, s, t, b, c);
			return -11;
		}
	} else if(r > 1.0 - EPS) {
		// goes thru B
		if(fabs(s - 0.5) > EPS) {
			fprintf(stderr, "%d: r %f s %f  != 0.5 t %f\n",
				probnum, r, s, t);
			return -12;
		}
		if(fabs(a - c) > EPS) {
			fprintf(stderr, "%d: r %f s %f t %f a %f != c %f \n",
				probnum, r, s, t, a, c);
			return -11;
		}
	} else if((t < -EPS) || (t > (1.0 + EPS))) {
		if((s  < -EPS) || (s > (1.0 + EPS)))
		{
			fprintf(stderr, "%d: r %f s %f t %f outside triangle\n",
				probnum, r, s, t);
			return -4;
		}
		// intersect AB and AC cut off A s and t correct values
		if(fabs(r*s - 0.5) > EPS) {
			fprintf(stderr, "%d: r %f r % f r*s %f != 0.5\n",
				probnum, r, s, r*s);
			return -5;
		}
		if(fabs(s*b+r*c - perimov2) > 0.5) {
			fprintf(stderr, "%d: s %f r % f s*b + r*c %f != %f\n",
				probnum, s, r, s*b+r*c, perimov2);
			return -6;
		}
	} else {
		// intersect AB and BC cut off B t OK rev r
		r = 1.0 - r;
		if(fabs(r*t - 0.5) > EPS) {
			fprintf(stderr, "%d: r %f t % f r*t %f != 0.5\n",
				probnum, r, t, r*t);
			return -7;
		}
		if(fabs(r*c+t*a - perimov2) > 0.5) {
			fprintf(stderr, "%d: r %f t % f r*c + t*a %f != %f\n",
				probnum, r, t, r*c+t*a, perimov2);
			return -8;
		}
	}
	return 0;
}

FILE *probFP, *solnFP;
char inbuf[256];
int main(int argc, char **argv)
{
	int nprob, curprob, index, ret;
	double invals[6], result[3];

	if(argc < 3) {
		fprintf(stderr, "USAGE: %s <problem_file> <solution_file>\n", argv[0]);
		return -30;
	}
	if((probFP = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "cannot open <problem_file> \"%s\" for read\n", argv[1]);
		return -31;
	}
	if((solnFP = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "cannot open <solution_file> \"%s\" for read\n", argv[2]);
		fclose(probFP);
		return -32;
	}
	ret = 0;
	if(fgets(&(inbuf[0]), 255, probFP) == NULL)
	{
		fprintf(stderr, "Read failed on problem count\n");
		ret = -1;
		goto cleanup;
	}
	if(sscanf(&(inbuf[0]), "%d", &nprob) != 1)
	{
		fprintf(stderr, "Scan failed on problem count\n");
		ret = -2;
		goto cleanup;
	}
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, probFP) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d data\n", curprob);
			ret = -3;
			goto cleanup;
		}
		if(sscanf(&(inbuf[0]), "%d %lf %lf %lf %lf %lf %lf", &index,
			&(invals[0]), &(invals[1]), &(invals[2]),
			&(invals[3]), &(invals[4]), &(invals[5])) != 7)
		{
			fprintf(stderr, "Scan failed on problem %d data\n", curprob);
			ret = -4;
			goto cleanup;
		}
		if(index != curprob)
		{
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				index, curprob);
			ret = -5;
			goto cleanup;
		}
		if(fgets(&(inbuf[0]), 255, solnFP) == NULL)
		{
			fprintf(stderr, "Read failed on solution %d data\n", curprob);
			ret = -6;
			goto cleanup;
		}
		if(sscanf(&(inbuf[0]), "%d %lf %lf %lf", &index,
			&(result[0]), &(result[1]), &(result[2])) != 4)
		{
			fprintf(stderr, "Scan failed on solution %d data\n", curprob);
			ret = -7;
			goto cleanup;
		}
		if(index != curprob)
		{
			fprintf(stderr, "solution index %d not = expected problem %d\n",
				index, curprob);
			ret = -8;
			goto cleanup;
		}
		if((ret = CheckEqualizer(curprob, &(invals[0]), &(result[0]))) != 0) {
			goto cleanup;
		}
	}
cleanup:
	fclose(probFP);
	fclose(solnFP);
	return 0;
}
