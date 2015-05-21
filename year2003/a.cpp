// Biker.cpp : Defines the entry point for the console application.
//


#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#define PI  (float)3.1415927
#define FPM (float)5280.
#define IPF (float)12.
#define SPH (float)3600.

int main(int argc, char* argv[])
{
    float dist, diam, dur, m;
    int nrev, n;
    FILE *fp = fopen("a.in", "rt");
    if(fp == NULL){
        printf("Can't open input a.in\n");
        return 1;
    }

    n = 0;
    while(fscanf(fp, "%f %d %f", &(diam), &(nrev), &(dur)) == 3){
        if(nrev == 0){
            break;
        }
        n++;
        dist = diam*PI*(float)(nrev);
        m = dist/(FPM*IPF);
        printf("Trip #%d: %.2f %.2f\n", n, m, m/(dur/SPH));

    }
    fclose(fp);
    return(0);
}
