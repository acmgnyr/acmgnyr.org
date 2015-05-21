/*
 * 2005 ACM Greater New York Regional Collegiate Programming Contest
 * Problem E -- brainf*ck
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define	BFDEBUG

#define	MAXDATA		30000
#define	MAXCODE		(128*1024)
char pdata[MAXDATA];
char *ptr;
unsigned char pcode[MAXCODE];
int			jmploc[MAXCODE];
int			jmpstk[MAXCODE];
unsigned char *pc;

int main(int argc, char* argv[])
{
	int i, n, err, idx, idx2;
	char buf[128], *s;
	int	nstk;
	unsigned long ninst;

	/* Get number of programs */
	fgets(&(buf[0]), sizeof(buf)-1, stdin);
	n = atoi(&(buf[0]));
	for(i = 1; i <= n; i++){
		fprintf(stdout, "PROGRAM #%d:\n", i);
		pc = &(pcode[0]);
		err = 0;
		nstk = -1;
		/* Clear jump table */
		for(idx = 0; idx < MAXCODE; idx++){
			jmploc[idx] = -1;
		}
		while(fgets(&(buf[0]), sizeof(buf)-1, stdin) != NULL){
			if(strncmp(&(buf[0]), "end", 3) == 0){
				break;
			}
			/* Strip out garbage */
			for(s = &(buf[0]); *s != '\0' && *s != '%'; s++){
				switch(*s){
				case '>': case '<': case '+': case '-': case '.': case ',':
					*pc++ = (unsigned char)*s;
					break;
				case '[':
					jmpstk[++nstk] = pc - &(pcode[0]);
					*pc++ = (unsigned char)*s;
					break;
				case ']':
					idx = pc - &(pcode[0]);
					if(nstk == -1){
						err = 1;
#ifdef BFDEBUG
						fprintf(stderr, "MISMATCHED ] AT %d\n", idx);
#endif
						break;
					}
					jmploc[idx] = jmpstk[nstk];
					jmploc[jmpstk[nstk--]] = idx;
					*pc++ = (unsigned char)*s;
					break;
				}
			}
			if(err){
				break;
			}
			if(pc >= &(pcode[MAXCODE])){
				fprintf(stderr, "CODE OVERFLOW\n");
				break;
			}
		}
		if(nstk != -1){
#ifdef BFDEBUG
			fprintf(stderr, "MISMATCHED '[' AT %d (%d TOTAL)\n", jmpstk[nstk], nstk);
#endif
			err = 1;
		}
		if(err){
			fprintf(stdout, "COMPILE ERROR\n");
			continue;
		}
		*pc = '\0';
		/* zero memory */
		memset(&(pdata[0]), '\0', sizeof(pdata));
		/* Set ptr */
		ptr = &(pdata[0]);
		/* Set PC */
		pc = &(pcode[0]);
		/* Run */
		ninst = 0;
		while(*pc != '\0'){
			switch(*pc++){
			case '>':
				ptr++;
				break;
			case '<':
				ptr--;
				break;
			case '+':
				(*ptr)++;
				break;
			case '-':
				(*ptr)--;
				break;
			case '.':
				fputc((int)*ptr, stdout);
#ifdef BFDEBUG
				fflush(stdout);
#endif
				break;
			case ',':
//				*p = fgetc(stdin);
				fprintf(stderr, "WANT INPUT\n");
				break;
			case '[':
				if(*ptr == '\0'){
					idx2 = (pc-1) - &(pcode[0]);
					idx = jmploc[idx2];
					if(idx == -1){
						fprintf(stderr, "INVALID JUMP at %d\n", idx2);
						break;
					} else {
						pc = &(pcode[idx]);
					}
				}
				break;
			case ']':
				if(*ptr != '\0'){
					idx2 = (pc-1) - &(pcode[0]);
					idx = jmploc[idx2];
					if(idx == -1){
						fprintf(stderr, "INVALID JUMP at %d\n", idx2);
						break;
					} else {
						pc = &(pcode[idx]);
					}
				}
				break;
			default:
				fprintf(stderr, "INVALID OP CODE 0x%x\n", *pc);
				break;
			}
			ninst++;
		}
#ifdef BFDEBUG
		fprintf(stdout,"\n%lu INSTRUCTIONS\n", ninst);
#else
		fprintf(stdout, "\n");
#endif
		fflush(stdout);
	}
	return 0;
}
