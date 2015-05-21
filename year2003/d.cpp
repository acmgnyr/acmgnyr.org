// Taxation.cpp : Defines the entry point for the console application.
//


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

char *monname[] = { "", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };

int main(int argc, char* argv[])
{
    char ibuf[256], *s, *psave, startdate[64], startsave[64], enddate[64], *t;
    double rate;
    int num, den, n, mon, day, yr, emon, eyr, idx;
	int bfound;
    FILE *fp = fopen("d.in", "rt");
    if(fp == NULL){
        printf("Can't open d.in\n");
        return 1;
    }

    idx = 0;
    while(fgets(&(ibuf[0]), sizeof(ibuf)-1, fp) != NULL){
		bfound = 0;
        idx++;
        printf("<Kalii Index=%d>", idx);
        s = &(ibuf[0]);
        while(*s){
            s++;
        }
        // trim nl
        while(--s >= &(ibuf[0]) && (*s == '\n' || *s == '\r' || *s == ' ')){
            *s = '\0';
        }
        // Isolate rate
        s = strchr(ibuf, '%');
        if(s == NULL){
            // must be fraction
            s = strchr(ibuf, '/');
            if(s == NULL || s == ibuf){
                goto bad;
            } else {
                num = s[-1] - '0';
                den = s[1] - '0';
                if(den != 2 && den != 4 && den != 8){
                    goto bad;
                }
                if(num >= den){
                    goto bad;
                }
                s[-1] = '\0';
				for(t = ibuf; *t; t++){
					if(!isdigit(*t)){
						goto bad;
					}
				}
                rate = atof(ibuf) + (float)(num)/(float)(den);
                if(rate < 0 || rate > 99.875){
                    goto bad;
                }
                // point to date
                s += 2;
            }
        } else {
            *s++ = '\0';
            for(t = ibuf; *t; t++){
                if(!isdigit(*t)){
                    goto bad;
                }
            }
            rate = atof(ibuf);
            if(rate < 0 || rate > 99.875){
                goto bad;
            }
        }
        while(*s == ' '){
            s++;
        }
        n = (int)strlen(s);
        // gotta be at least 6
        if(n < 6){
            goto bad;
        }
        // Now we work backwards
        t = s + n;
        t--;
        if(!isdigit(*t) || !isdigit(t[-1])){
            goto bad;
        }
        t--;
        day = atoi(t);
        if(day < 1 || day > 31){
            goto bad;
        }
        *t-- = '\0';
        if(*t == '-'){
            *t-- = '\0';
        }
        if(!isdigit(*t)){
            goto bad;
        }
        mon = atoi(t);
        t--;
        // see if next char back is blank, or bigger than 1
        if(*t == '0' || (*t == '1' && mon < 3)){
            // Could be part of this date
            psave = t;
        } else {
            psave = NULL;
        }
tryagain:
        // lets assume no 2 digit for now
        yr = 03;
        if(mon < 11 || (mon == 11 && day < 9)){
            yr++;
        }
        sprintf(startdate, "%02d%s%02d", day, monname[mon], yr);
        while(*t == ' '){
            t--;
        }
        if(t - s < 2 || !isdigit(*t) || !isdigit(t[-1])){
TestBad:
            if(psave){
                mon = atoi(psave);
                if(mon < 1 || mon > 12){
                    goto bad;
                }
                t = --psave;
                psave = NULL;
                goto tryagain;
            }
            goto bad;
        }
        eyr = (*t - '0') + ((t[-1] - '0')*10);
        t -= 2;
        if(*t == '-'){
            t--;
        }
        if(t < s || !isdigit(*t)){
            goto TestBad;
        }
        emon =  *t - '0';
        t--;
        if(t > s){
            goto TestBad;
        }
        if(t == s){
            emon += (*t - '0')*10;
        }
        if(emon < 1 || emon > 12){
            goto TestBad;
        }
		if(bfound){
			bfound = 0;
			goto bad;
		}
        sprintf(enddate, "15%s%02d", monname[emon], eyr);
		if(psave){
			strcpy(startsave, startdate);
			bfound = 1;
            mon = atoi(psave);
            if(mon < 1 || mon > 12){
                goto bad;
            }
            t = --psave;
            psave = NULL;
            goto tryagain;
		}
good:
        printf("\n    <startdate>%s</startdate>\n", startdate);
        printf("    <rate>%.3f</rate>\n", rate);
        printf("    <enddate>%s</enddate>\n", enddate);
        goto done;
bad:
		if(bfound){
			strcpy(startdate, startsave);
			goto good;
		}
        printf("BAD DATA");
done:
        printf("</Kalii>\n");
    }
    fclose(fp);
	return 0;
}

