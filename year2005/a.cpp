/*
 * 2005 ACM Greater New York Regional Collegiate Programming Contest
 * Problem A -- 01000001
 */

#define	MAXLINE	256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	char szLine[MAXLINE], szResult[MAXLINE];
	char *s, *t, *r;
	int i, n, res, l1, l2, c;

	n = ::atoi(::fgets(&(szLine[0]), sizeof(szLine)-1, stdin));
	for(i = 1; i <= n; i++){
		::fgets(&(szLine[0]), sizeof(szLine)-1, stdin);
		/* Look for ' ' */
		s = ::strchr(&(szLine[0]), ' ');
		if(s == NULL){
			::fprintf(stdout, "INSUFFICIENT INPUT\n");
			continue;
		}
		/* Remember end of first */
		t = s;
		/* Length of first one */
		l1 = s - &(szLine[0]);
		/* Separate strings */
		*s++ = '\0';
		/* Length of second one */
		for(l2 = 0; *s == '1' || *s == '0'; s++){
			l2++;
		}
		c = 0;
		r = &(szResult[sizeof(szResult)]);
		*--r = '\0';
		while(l2 > 0 && l1 > 0){
			res = *--s + *--t + c - '0';
			c = (res >= '2');
			*--r = '0' + (res & 1);
			l1--;
			l2--;
		}
		/* Copy rest of remaining value */
		while(l2 > 0){
			res = c + *--s;
			if(res == '2'){
				c = 1;
				*--r = '0';
			} else {
				c = 0;
				*--r = res;
			}
			l2--;
		}
		while(l1 > 0){
			res = c + *--t;
			if(res == '2'){
				c = 1;
				*--r = '0';
			} else {
				c = 0;
				*--r = res;
			}
			l1--;
		}
		if(c != 0){
			*--r = '1';
		}
		/* Suck up leading zeroes */
		while(*r == '0'){
			r++;
		}
		/* Make sure there's at least one digit */
		if(*r == '\0'){
			r--;
		}
		::fprintf(stdout, "%d: %s\n", i, r);
	}
	return 0;
}
