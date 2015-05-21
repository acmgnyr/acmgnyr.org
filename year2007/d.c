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
{
	char ibuf[512], *s, obuf[400/5+2];
	int i, n, ir, ic, r, c, ch, cnt, dir, loop, io, or, oc;

	if(fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
		return(0);
	}
	n = atoi(&(ibuf[0]));
#ifdef MKINPUT
	printf("%d\n", n);
#endif
	for(i = 1; i <= n; i++){
		if(fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
			printf("premature eof\n");
			break;
		}
		r = atoi(&(ibuf[0]));
		or = r;
		s = strchr(&(ibuf[0]), ' ');
		if(s == NULL){
			printf("no separator space 1(%d)\n", i);
			break;
		}
		s++;
		c = atoi(s);
		oc = c;
		s = strchr(s, ' ');
		if(s == NULL){
			printf("no separator space 2(%d)\n", i);
			break;
		}
		s++;
		/* Fill in matrix */
		for(ir = 1; ir <= r; ir++){
			m[ir][0] = -1;
			for(ic = 1; ic <= c; ic++, s++){
				m[ir][ic] = *s - '0';
			}
			/* Set up border */
			m[ir][ic] = -1;
		}
		/* Set up border */
		for(c = 0; c <= ic; c++){
			m[ir][c] = -1;
			m[0][c] = -1;
		}
		r = 1;
		c = 1;
		ch = 0;
		dir = 1;
		for(io = 0, cnt = 0;;){
			ch = (ch<<1) | m[r][c];
			m[r][c] = -1;
			cnt++;
			if(cnt == 5){
				if(ch == 0){
					obuf[io++] = ' ';
				} else {
					obuf[io++] = 'A' + ch - 1;
				}
				cnt = 0;
				ch = 0;
			}
			loop = 0;
			/* Next move taken? */
			while(m[r+di[dir][0]][c+di[dir][1]] == -1){
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
			r += di[dir][0];
			c += di[dir][1];
		}
		/* Trim trailing */
		while(--io >= 0 && obuf[io] == ' '){
			;
		}
		obuf[++io] = '\0';
#ifndef MKINPUT
		printf("%d %s\n", i, &(obuf[0]));
#else
		printf("%d %d %s\n", or, oc, &(obuf[0]));
#endif
					
	}
	return(0);
}

