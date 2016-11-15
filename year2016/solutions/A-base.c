/*
 * A - Which base is it anyway? 
 * Solution by John Buck
 * ACM GNYR 2016
 */
#include <stdio.h>
#include <stdlib.h>

#define	BUF_SIZE	128

int main(int argc, char **argv)
{
	int p, k, i, j, m, ov, dv, hv;
	char szBuf[BUF_SIZE];

	if(fgets(&(szBuf[0]), BUF_SIZE-1, stdin) != NULL){
		p = atoi(&(szBuf[0]));
		for(i = 1; i <= p; i++){
			if(fgets(&(szBuf[0]), BUF_SIZE-1, stdin) == NULL){
				break;
			}
			/*
			 * Get problem number; first thing on line
			 */
			k = atoi(&(szBuf[0]));
			if(k != i){
				fprintf(stderr, "Wrong problem number %d for instance %d\n", k, i);
				break;
			}
			/*
			 * Skip to first space; basically, skipping over problem number
			 */
			for(j = 0; j < BUF_SIZE; j++){
				if(szBuf[j] == ' '){
					break;
				}
			}
			/*
			 * For this problem, the octal value is the only one that presents
			 * us with an issue.
			 * If the input value is "9", sscanf does not touch the octal value.
			 * If the input is 19, the octal value will be 1 and this is wrong.
			 * So, we have to validate the octal one by hand.
			 * We  go through character by character and build up the value until
			 * we hit the end of string ('\0') or we get a bad digit.
			 */
			ov = 0;
			for(m = j; m < BUF_SIZE && szBuf[m] != '\0' && szBuf[m] != '\r' && szBuf[m] != '\n'; m++){
				/*
				 * Leading spaces are skipped, not that there should be any...
				 */
				if(szBuf[m] == ' '){
					continue;
				}
				if(szBuf[m] >= '0' && szBuf[m] <= '7'){
					ov = (ov << 3) | (szBuf[m] - '0');
				} else {
					/*
					 * Any illegal digit is an error, so set the value to 0
					 */
					ov = 0;
					break;
				}
			}
			/*
			 * Since the input only contains decimal digits, these scanf's will always
			 * work right.
			 */
			sscanf(&(szBuf[j]), "%d", &(dv));
			sscanf(&(szBuf[j]), "%x", &(hv));
			printf("%d %d %d %d\n", k, ov, dv, hv);
		}
	}
	return(0);
}
