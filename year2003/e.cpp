// Logical.cpp : Defines the entry point for the console application.
//


#include <stdio.h>

//#define	DEB
char    ops[2000];
char    *op = &(ops[0]), *edge;
int     nextt, val, used = 0;
extern  void    or(void);
int		vmap[26], vals[26];
FILE    *fp;
int
lex(void)
{
        register int c;

        while ((c = fgetc(fp)) != '\n') {
                switch (c) {
                case '(':
                case ')':
                case '~':
                case '&':
                case '^':
                case '|':
                        return(nextt = c);
                default:
                        if ((c >= 'a') && (c <= 'z')) {
                                return(nextt = c);
                        }
                }
        }
        return(EOF);
}

void
unary(void)
{
        switch (nextt) {
        case '(':
                lex(); or(); lex(); break;
        case '~':
                lex(); unary(); *(op++) = '~'; break;
        default:
                used |= (1 << (nextt - 'a'));
                *(op++) = nextt; lex();
        }
}

void
and(void)
{
        unary();
        while (nextt == '&') {
                lex();
                unary();
                *(op++) = '&';
        }
}

void
xor(void)
{
        and();
        while (nextt == '^') {
                lex();
                and();
                *(op++) = '^';
        }
}

void
or(void)
{
        xor();
        while (nextt == '|') {
                lex();
                xor();
                *(op++) = '|';
        }
}

int
eval(register char *s,
register char *e)
{
        char stack[1000];
        register char *sp = &(stack[0]);

        sp[0] = 0;
        while (s < e) {
                switch (*s) {
                case '~':       sp[0] = !sp[0]; break;
                case '&':       --sp; sp[0] = sp[0] & sp[1]; break;
                case '^':       --sp; sp[0] = sp[0] ^ sp[1]; break;
                case '|':       --sp; sp[0] = sp[0] | sp[1]; break;
                default:        ++sp; sp[0] = (vals[vmap[(*s - 'a')]]);
                }
                ++s;
        }
        return(sp[0]);
}

int
main(int argc, char **argv)
{
    int i, j, nv, nused, n, ival, bit;
    fp = fopen("e.in", "rt");
    if(fp == NULL){
        printf("Can't open input e.in\n");
        return 1;
    }

    if(fscanf(fp, "%d", &n) != 1){
        printf("Bad number of cases\n");
    } else {
        // Scanf doesnt read nl
        while(fgetc(fp) != '\n');
        for(i = 1; i <= n; i++){
            used = 0;
            op = &(ops[0]);
            lex();
            or();
            edge = op;
            or();

            printf("Data set %d: ", i);
			for(j = 0, nv = 0; j < 26; j++){
				if(used & (1<<j)){
					vmap[j] = nv++;
				} else {
					vmap[j] = 0;
				}
#ifdef DEB
				printf("%d: %d\n", j, vmap[j]);
#endif
			}
			nused = nv;
			nv = (1 << nv);
			for(ival = 0; ival < nv; ival++){
				// convert to vars
				for(bit = 0; bit < nused; bit++){
					if(ival & (1<<bit)){
						vals[bit] = 1;
					} else {
						vals[bit] = 0;
					}
				}
#ifdef DEB
				printf("Try: 0x%x\n", ival);
#endif
                register int a = eval(&(ops[0]), edge);
                register int b = eval(edge, op);
                if (a != b) {
                        printf("Different\n");
                        goto done;
                }
            }
            printf("Equivalent\n");
done:
            ;
        }
    }
    return(0);
}
