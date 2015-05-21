#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define	GAL_TO_LITERS	(3.7854)
#define	LITERS_TO_GAL	(0.2642)
#define	LB_TO_KG		(0.4536)
#define	KG_TO_LB		(2.2046)

int main()
{
	char ibuf[256], *s, *t;
	int i, n;
	double v, cf;

	/* Get # problems */
	if(fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
		return(0);
	}
	n = atoi(&(ibuf[0]));

	/* Process each dataset */
	for(i = 1; i <= n; i++){
		/* Get next value */
		if(fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
			break;
		}
		v = atof(&(ibuf[0]));
		s = strchr(&(ibuf[0]), ' ');
		if(s == NULL){
			continue;
		}
		s++;
		cf = 1;
		t = "?";
		switch(*s){
		case 'g':
			/* Gallons to litres */
			cf = GAL_TO_LITERS;
			t = "l";
			break;

		case 'l':
			/* pounds, litres */
			if(s[1] == 'b'){
				/* Pounds to kg */
				cf = LB_TO_KG;
				t = "kg";
			} else {
				/* liters to gallons */
				cf = LITERS_TO_GAL;
				t = "g";
			}
			break;

		case 'k':
			/* kg to pounds */
			cf = KG_TO_LB;
			t = "lb";
			break;
		}
		/* Convert */
		v = v*cf;
		printf("%d %.4f %s\n", i, v, t);
	}
	return(0);
}
