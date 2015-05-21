/*
 * F - Monkey Vines - GNY ACM Contest 2007
 * Solution by: Yury Kholondyrev
 *
 * Modified by John Buck (October 2007).  Any bugs,
 * were undoubtedly added by me -- JB
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main () {
	char buf[256];
	int i, n, pn, len;
	int count, max;

	/* Get problem count */
	if(fgets(&(buf[0]), sizeof(buf), stdin) == NULL){
		return(0);
	}

	n = atoi(&(buf[0]));

	for(pn = 1; pn <= n; pn++){
		if(fgets(&(buf[0]), sizeof(buf), stdin) == NULL || strlen(&(buf[0])) > 150){
			printf("%d Line too long [%s]\n", pn, &(buf[0]));
			continue;
		}
		count = 0;
		max = 0;
		len = strlen(&(buf[0]));
		for (i = 0; i < len; i++) {
			if(buf[i] == '['){
				count++;
				if(count > max){
					max = count;
				}
			} else if(buf[i] == ']'){
				count--;
			} else {
				break;
			}
			if (count > 25){
				printf("%d Too much nesting [%s]\n", pn, &(buf[0]));
				continue;
			}
			if (count < 0){
				printf("%d Unmatched bracket [%s]\n", pn, &(buf[0]));
				continue;
			}
		}
		if (count != 0){
			printf("%d Unmatched bracket [%s]\n", pn, &(buf[0]));
		}
		printf("%d %d\n", pn, 1 << max);
	}
	return(0);
}

