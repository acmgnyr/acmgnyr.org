#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int m[22][22];
int di[4][2] = {
	{ -1, 0 },
	{ 0, 1},
	{ 1, 0 },
	{ 0, -1 }
};

int
main(int argc, char **argv)
{	char ibuf[512], *s, obuf[400];
	int i, n, ir, ic, r, c, ch, cnt, dir, loop, idx;

	if(fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
		return(0);
	}
	n = atoi(&(ibuf[0]));
	for(i = 1; i <= n; i++){
		if(fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
			printf("premature eof %d\n", i);
			break;
		}
		r = atoi(&(ibuf[0]));
		s = strchr(&(ibuf[0]), ' ');
		if(s == NULL){
			printf("missing separator 1 (%d)\n", i);
			break;
		}
		s++;
		c = atoi(s);
		s = strchr(s, ' ');
		if(s == NULL){
			printf("missing separator 2 (%d)\n", i);
			break;
		}
		s++;
		/* remainder of line is text */
		/* Fill in matrix with 0's and a border of -1*/
		for(ir = 1; ir <= r; ir++){
			m[ir][0] = -1;
			for(ic = 1; ic <= c; ic++){
				m[ir][ic] = 0;
			}
			/* Set up border */
			m[ir][ic] = -1;
		}
		/* Set up border */
		for(idx = 0; idx <= ic; idx++){
			m[ir][idx] = -1;
			m[0][idx] = -1;
		}
		ir = 1;
		ic = 1;
		ch = 0;
		dir = 1;
		/* Populate matrix */
		for(cnt = 0;;){
			if(cnt == 0){
				/* Get next char */
				if(*s >= 'A' && *s <= 'Z'){
					ch = *s - 'A' + 1;
				} else if(*s == ' '){
					ch = 0;
				} else {
					break;
				}
				s++;
				cnt = 5;
			}
			cnt--;
			if(ch & (1 << cnt)){
				m[ir][ic] = 2;
			} else {
				m[ir][ic] = 1;
			}
			loop = 0;
			/* Next move taken? */
			while(m[ir+di[dir][0]][ic+di[dir][1]] != 0){
				dir++;
				if(dir >= 4){
					dir = 0;
					if(loop++){
						break;
					}
				}
			}
			if(loop > 1){
				break;
			}
			ir += di[dir][0];
			ic += di[dir][1];
		}
		/* Matrix filled in now unwind */
		s = &(obuf[0]);
		for(ir = 1; ir <= r; ir++){
			for(ic = 1; ic <= c; ic++){
				if(m[ir][ic] == 2){
					*s++ = '1';
				} else {
					*s++ = '0';
				}
			}
		}
		*s = '\0';
#ifndef MKINPUT
		printf("%d %s\n", i, &(obuf[0]));
#else
		printf("%d %d %s\n", r, c, &(obuf[0]));
#endif
	}
	return 0;
}
