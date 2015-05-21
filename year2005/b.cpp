/*
 * 2005 ACM Greater New York Regional Collegiate Programming Contest
 * Problem B -- The Bank of Kalii
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define	TESTING
int jday[] = {
	0,
	31, 28, 31, 30,
	31, 30, 31, 31,
	30, 31, 30, 31
};

/* Return 1 if the given month(m) in the given year(n)
 * must add a leap year
 */
int IsLeapYear(int n, int m)
{
	return(m > 2 && (n%4)==0 && (n%100 || (n%400)==0));
}

int main(int argc, char* argv[])
{
	char szBuf[256];
	int i, n, nday, jd, jt;
	char *p, *s;
	int M, D, Y, m, d, y;

	/* Calculate julian days for each month */
	for(i = 1; i <= 12; i++){
		jday[i] += jday[i-1];
	}
#ifdef TESTING
	for(i = 1999; i <= 2200; i++){
		if(IsLeapYear(i, 3)){
			printf("%d\n", i);
		}
	}
#endif
	
	if(::fgets(&(szBuf[0]), sizeof(szBuf), stdin) == NULL){
		return 1;
	}

	n = ::atoi(&(szBuf[0]));
	for(i = 1; i <= n; i++){
		if(::fgets(&(szBuf[0]), sizeof(szBuf), stdin) == NULL){
			perror("out of input");
			break;
		}
		/* Form is: MM/DD/YYYY MM/DD */
		p = ::strchr(&(szBuf[0]), ' ');
		if(p == NULL){
			perror("no space");
			break;
		}
		*p++ = '\0';
		M = ::atoi(&(szBuf[0]));
		s = ::strchr(&(szBuf[0]), '/');
		if(s == NULL){
			perror("no first /");
			break;
		}
		s++;
		D = ::atoi(s);
		s = ::strchr(s, '/');
		if(s == NULL){
			perror("no 2nd /");
			break;
		}
		s++;
		Y = ::atoi(s);
		m = ::atoi(p);
		p = ::strchr(p, '/');
		if(p == NULL){
			perror("no compare first /");
			break;
		}
		p++;
		d = ::atoi(p);
#ifdef TESTING
		printf("%d/%d/%d  %d/%d\n", M, D, Y, m, d);
#endif
		if(M == m || M+1 == m || M-1 == m){
			y = Y;
			// Julian date of transaction
			jt = jday[M-1] + D + IsLeapYear(Y, M);
			// Julian for document year +-1 */
			jd = jday[m-1] + d + IsLeapYear(y, m);
			nday = jd - jt;
		} else if(M == 12 && m == 1){
			y = Y+1;
			/* Days in future */
			nday = d + (31-D);
		} else if(M == 1 && m == 12){
			y = Y-1;
			/* In past */
			nday = -(D + (31-d));
		} else {
			/* Not within a month */
			printf("%d OUT OF RANGE\n", i);
			continue;
		}
		if(nday > 7 || nday < -7){
			printf("%d OUT OF RANGE\n", i);
			continue;
		}
		if(nday == 0){
			printf("%d SAME DAY\n", i);
		} else {
			printf("%d %d/%d/%d ", i, m, d, y);
			if(nday < 0){
				printf("IS %d DAY%s PRIOR\n", -nday, nday == -1 ? "" : "S");
			} else {
				printf("IS %d DAY%s AFTER\n", nday, nday == 1 ? "" : "S");
			}
		}
	}

	return 0;
}
