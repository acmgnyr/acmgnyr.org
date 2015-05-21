// PushButtonLock.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int nbuttons = 0;
unsigned int max_move = 0;
int savemove[16384];

unsigned long count(unsigned int used)
{
    unsigned int move;
    unsigned int nMove = 0;

    if(savemove[used] > 0){
        return(savemove[used]);
    }
    for (move = 1; move < max_move; move++)
    {
        if (move & used)
        {
            continue;
        }
        nMove++;
        nMove += count(used | move);
    }
    savemove[used] = nMove;
    return(nMove);
}

int main()
{
    int n, i, ncomb;
    char ibuf[256];

    if(::fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
        return(-1);
    }
    n = ::atoi(&(ibuf[0]));

    for(i = 1; i <= n; i++){
        if(::fgets(&(ibuf[0]), sizeof(ibuf), stdin) == NULL){
            break;
        }
        nbuttons = ::atoi(&(ibuf[0]));
        max_move = 1<<nbuttons;

        ::memset(&(savemove[0]), '\0', sizeof(savemove));
        ncomb = count(0);
        printf("%d %d %lu\n", i, nbuttons, ncomb);
    }
    return (0);
}
