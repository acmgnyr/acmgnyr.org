#include <stdio.h>
#include <stdlib.h>

char *szhex = "0123456789ABCDEF";

int main(int argc, char **argv)
{
	FILE *fp = NULL;
	int n, p, nc;
	int bval = -1;
	if(argc != 4){
		fprintf(stderr, "Usage: %s prob file numbytes\n", argv[0]);
		return(1);
	}
	if(argv[2][0] == '0'){
		sscanf(&(argv[2][2]), "%x", &(bval));
	} else if(argv[2][0] == '!'){
		bval = -2;
	} else {
		fp = ::fopen(argv[2], "rb");
		if(fp == NULL){
			perror(argv[2]);
			return(2);
		}
	}
	p = ::atoi(argv[1]);
	n = ::atoi(argv[3]);
	printf("%d %d\n", p, n);
	nc = 0;
	while(n-- > 0){
		if(fp == NULL){
			if(bval < -1){
				p = n & 0xff;
			} else {
				p = bval;
			}
		} else {
			p = fgetc(fp) & 0xff;
		}
		putc(szhex[p>>4], stdout);
		putc(szhex[p & 0xf], stdout);
		nc++;
		if(nc == 40){
			putc('\n', stdout);
			nc = 0;
		}
	}
	if(nc > 0){
		putc('\n', stdout);
	}
	if(fp != NULL){
		::fclose(fp);
	}
	return(0);
}