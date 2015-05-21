#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	char ibuf[128], *s;
	int i, n, p;

	if(fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
		return(1);
	}
	n = atoi(&(ibuf[0]));

	for(i = 1; i <= n; i++){
		if(fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
			break;
		}
		s = strchr(&(ibuf[0]), ' ');
		if(s == NULL){
			break;
		}
		*s++ = '\0';
		p = atoi(&(ibuf[0]));
		printf("%d ", i);
		for(; *s != '\0' && *s != '\n' && *s != '\r'; s++){
			p--;
			if(p == 0){
				continue;
			}
			putc(*s, stdout);
		}
		putc('\n', stdout);
	}

	return(0);
}