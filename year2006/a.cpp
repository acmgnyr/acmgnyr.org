#include <stdio.h>
#include <stdlib.h>


int change(int &c, int coin)
{
	int cnt = 0;

	while(c >= coin){
		cnt++;
		c -= coin;
	}
	return(cnt);
}

int main()
{
	char ibuf[256];
	int i, n, c, q, d, ni, p;

	if(::fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
		return(-1);
	}
	n = ::atoi(&(ibuf[0]));
	for(i = 1; i <= n; i++){
		if(::fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
			return(-1);
		}
		c = ::atoi(&(ibuf[0]));
		if(c < 1 || c > 500){
			::fprintf(stderr, "%d is out of range for input\n", c);
			continue;
		}
		q = change(c, 25);
		d = change(c, 10);
		ni = change(c, 5);
		p = change(c, 1);
		::fprintf(stdout, "%d %d QUARTER(S), %d DIME(S), %d NICKEL(S), %d PENNY(S)\n",
			i, q, d, ni, p);
	}
	return 0;
}