/*
 * ACM2006 Greater NY Region C++ Solution to C - Chip Shuffle
 *
 * John Buck
 * October, 2006
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DBG

int trim(char *s)
{
    char *p = s;
    p += ::strlen(p);
    while(--p >= s){
        if(*p == '\r' || *p == '\n' || *p == ' '){
            *p = '\0';
        } else {
            break;
        }
    }
    p++;
    return(p-s);
}

int validate(const char *s)
{
    while(*s != '\0'){
        if(*s < 'A' || *s > 'H'){
            return(0);
        }
        s++;
    }
    return(1);
}

void shuffle(char *s1, char *s2, char *s12, int c)
{
#ifdef DBG
    ::fprintf(stdout, "*****\ns1=%s\ns2=%s\n", s1, s2);
#endif
    char *os1 = s1;
    char *os2 = s2;
    char *os12 = s12;
    while(*s2 != '\0'){
        *s12++ = *s2++;
        *s12++ = *s1++;
    }
    *s12 = '\0';
    ::strncpy(os1, os12, c);
    os1[c] = '\0';
    ::strncpy(os2, os12+c, c);
    os2[c] = '\0';
#ifdef DBG
    ::fprintf(stdout, "s12=%s\n", os12);
#endif
}

int main(int argc, char* argv[])
{
    char ibuf[256], s1[128], s2[128], sres[256], s12[256];
    int ds, n, c, s1l, s2l, sresl, i, gen, r, fail, c2;
    char *szin, *szout;
    FILE *fpi, *fpo;

    if(::fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
        return 1;
    }
    n = ::atoi(&(ibuf[0]));
    if(argc == 3){
        gen = 1;
        szin = argv[1];
        szout = argv[2];
        fpi = ::fopen(szin, "wt");
        fpo = ::fopen(szout, "wt");
        if(fpi == NULL || fpo == NULL){
            ::fprintf(stderr, "file open error\n");
            return 2;
        }
        ::fprintf(fpi, "%d\n", n);
    } else {
        gen = 0;
    }
    for(ds = 1; ds <= n; ds++){
        if(::fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
            break;
        }
        c = ::atoi(&(ibuf[0]));
        c2 = c<<1;
        if(::fgets(&(s1[0]), sizeof(s1), stdin) == NULL ||
            ::fgets(&(s2[0]), sizeof(s2), stdin) == NULL ||
            ::fgets(&(sres[0]), sizeof(sres), stdin) == NULL){
            break;
        }
        s1l = ::trim(&(s1[0]));
        s2l = ::trim(&(s2[0]));
        sresl = ::trim(&(sres[0]));
        if(s1l != s2l || s1l != c || (sresl != c2 && !gen)){
            ::fprintf(stderr, "DS%d: Bad values: s1l=%d s2l=%d c=%d sresl=%d\n",
                ds, s1l, s2l, c, sresl);
            continue;
        }
        if(::validate(&(s1[0])) == 0 ||
            ::validate(&(s2[0])) == 0 ||
            (!gen && ::validate(&(sres[0])) == 0)){
            ::fprintf(stderr, "DS%d: Bad colors\n", ds);
            continue;
        }
        ::memset(&(s12[0]), '\0', sizeof(s12));
        if(gen){
            /* Count max interations, first make result */
            ::strcpy(&(sres[0]), &(s1[0]));
            ::strcpy(&(sres[c]), &(s2[0]));
            for(i = 0; ; i++){
                if(::strcmp(&(s12[0]), &(sres[0])) == 0){
                    break;
                }
                ::shuffle(&(s1[0]), &(s2[0]), &(s12[0]), c);
            }
            /* Pick iteration */
            r = rand()%i + 1;
            /* Make 5% be failures */
            fail = (rand()%20 == 1);
            ::memset(&(s12[0]), '\0', sizeof(s12));
            ::fprintf(fpi, "%d\n%s\n%s\n", c, &(s1[0]), &(s2[0]));
            /* Perform the requisite # of shuffles */
            for(i = 0; i < r; i++){
                ::shuffle(&(s1[0]), &(s2[0]), &(s12[0]), c);
            }
//            if(!fail){
                ::fprintf(fpi, "%s\n", &(s12[0]));
                ::fprintf(fpo, "%d %d\n", ds, r);
//            } else {

//            }
        } else {
            for(i = 1; i <= c2; i++){
                ::shuffle(&(s1[0]), &(s2[0]), &(s12[0]), c);
#ifdef DBG
                ::fprintf(stdout, "%d-%d: %s  %s  %s\n", ds, i, s1, s2, s12);
#endif
                if(::strcmp(&(s12[0]), &(sres[0])) == 0){
                    break;
                }
            }
            if(i > c2){
                ::fprintf(stdout, "%d -1\n", ds);
            } else {
                ::fprintf(stdout, "%d %d\n", ds, i);
            }
        }
    }
    if(gen){
        ::fclose(fpi);
        ::fclose(fpo);
    }
	return 0;
}
