#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define	MAX_SAMP	1161
#define	MAX_LINE	(MAX_SAMP*2)

char buf[128];
char obuf[MAX_LINE+100];
char *p;

void LowClock(int n, int ndat, int randdat)
{
	int c;
	while(n > 0){
		*p++ = '0';
		if(randdat){
			c = ((rand() & 0x4000) != 0);
		} else {
			c = ndat;
		}
		if(c != 0){
			c = 1;
		}
		*p++ = c+'0';
		n--;
	}
}

void HighClock(int ndat)
{
	if(ndat != 0){
		ndat = 1;
	}
	*p++ = '1';
	*p++ = ndat+'0';
}

void CodeByte(int nb)
{
	int i;

	for(i = 0x80; i != 0; i >>= 1){
		LowClock(1, nb & i, 0);
		HighClock(nb & i);
	}
}

int main(int argc, char **argv)
{
	int n, nProb = 0, addr, nb, i, c, j, nIdle, nOnes, nZeroes, nRand;
	int bNoStart, bNoStop, bNoAck, nNoAckOff, nNoAckClocks, bDelay, nDelayOff, nDelayClocks;
	int nDelayOffs[100];
	FILE *fp;
	char *s, *t, *pc, *ps;

	if(argc != 2){
		fprintf(stderr, "Usage: %s inputscript\n", argv[0]);
		return(1);
	}
	fp = ::fopen(argv[1], "rt");
	if(fp == NULL){
		perror(argv[1]);
		return(2);
	}
	srand(0);
	while(::fgets(&(buf[0]), sizeof(buf), fp) != NULL){
		if(buf[0] == 'r' || buf[0] == 'w'){
			nProb++;
		}
	}
	::fseek(fp, 0L, 0);
	printf("%d\n", nProb);
	nProb = 0;
	while(::fgets(&(buf[0]), sizeof(buf), fp) != NULL){
		if(buf[0] != 'r' && buf[0] != 'w'){
			continue;
		}
		bNoAck = 0;
		bNoStop = 0;
		bNoStart = 0;
		bDelay = 0;
		nIdle = 0;
		nOnes = 0;
		nZeroes = 0;
		nRand  = 0;
		nb = 1;
		ps = &(buf[1]);
		ps = ::strchr(ps, '/');
		for(;ps != NULL ;){
			s = ps+1;
			/* Set up for next one */
			ps = ::strchr(s, '/');
			if(ps != NULL){
				*ps = '\0';
			}
			for(t = s + ::strlen(s); --t >= s && isspace(*t); ){
				;
			}
			*++t = '\0';
			t = ::strchr(s, ' ');
			if(t != NULL){
				*t++ = '\0';
			}
			if(::strcmp(s, "nostop") == 0){
				bNoStop = 1;
			} else if(::strcmp(s, "nostart") == 0){
				bNoStart = 1;
			} else if(::strcmp(s, "noack") == 0){
				bNoAck = 1;
				if(t != NULL){
					nNoAckOff = ::atoi(t);
					t = ::strchr(t, ' ');
					if(t != NULL){
						*t++ = '\0';
						nNoAckClocks = ::atoi(t);
					} else {
						nNoAckClocks = 1;
					}
				} else {
					nNoAckOff = 0;
				}
			} else if(::strcmp(s, "delay") == 0){
				if(t != NULL){
					bDelay = 1;
					for(nDelayOff = 0;;){
						nDelayOffs[nDelayOff++] = ::atoi(t);
						pc = ::strchr(t, ',');
						if(pc == NULL){
							break;
						}
						t = pc+1;
					}
					t = ::strchr(t, ' ');
					if(t != NULL){
						*t++ = '\0';
						nDelayClocks = ::atoi(t);
					} else {
						nDelayClocks = 1;
					}
				} else {
					/* Delay address ack */
					bDelay = 1;
					nDelayOff = 1;
					nDelayOffs[0] = 0;
					nDelayClocks = 1;
				}
			} else if(::strcmp(s, "idle") == 0){
				if(t != NULL){
					nIdle = ::atoi(t);
				} else {
					nIdle = 1;
				}
			} else if(::strcmp(s, "ones") == 0){
				if(t != NULL){
					nOnes = ::atoi(t);
				} else {
					nOnes = 1;
				}
			} else if(::strcmp(s, "zeroes") == 0){
				if(t != NULL){
					nZeroes = ::atoi(t);
				} else {
					nZeroes = 1;
				}
			} else if(::strcmp(s, "rand") == 0){
				if(t != NULL){
					nRand = ::atoi(t);
				} else {
					nRand = 1;
				}
			}
		}
		sscanf(&(buf[1]), "%x %d", &(addr), &(nb));
		p = &(obuf[0]);
		nProb++;
		if(nOnes > 0){
			while(nOnes > 0){
				nOnes--;
				HighClock(1);
			}
			goto done;
		}
		if(nZeroes > 0){
			LowClock(nZeroes, 0, 0);
			goto done;
		}
		if(nRand > 0){
			while(nRand > 0){
				*p++ = ((rand() & 0x4000) ? '0' : '1');
				*p++ = ((rand() & 0x4000) ? '0' : '1');
				nRand--;
			}
			goto done;
		}
		nIdle++;
		/* Send out idle */
		while(nIdle-- > 0){
			HighClock(1);
		}		
		LowClock(1, 1, 0);
		/* Start */
		HighClock(1);
		if(!bNoStart){
			HighClock(0);
		}
		CodeByte((addr << 1) | (buf[0] == 'r'));
		/* Ack */
		if(bNoAck && nNoAckOff == 0){
			LowClock(1, 0, 0);
			/* Don't ack the address */
			for(n = 0; n < nNoAckClocks; n++){
				HighClock(1);
			}
		} else {
			nNoAckOff--;
			for(i = 0; i < nb; i++){
				if(p > &(obuf[MAX_LINE])){
					fprintf(stderr, "Too many clocks on prob %d at byte %d\n", nProb, i);
					return(2);
				}
				if(bDelay){
					for(j = 0; j < nDelayOff; j++){
						if(i == nDelayOffs[j]){
							LowClock(nDelayClocks, 1, 0);
							break;
						}
					}
				}
				/* Ack */
				LowClock(1, 0, 0);	// pull sda low
				HighClock(0);		// release clock, with sda low is an ack.
				/* Send next byte */
				c = (i % 96) + ' ';
				CodeByte(c);
				/* No ack, we bail now */
				if(bNoAck && i == nNoAckOff){
					for(n = 0; n < nNoAckClocks; n++){
						HighClock(1);
					}
					break;
				}
			}
			if(!bNoAck){
				/* Final ack */
				LowClock(1, 0, 0);
				HighClock(0);
				LowClock(1, 0, 0);
				/* Stop */
				if(!bNoStop){
					HighClock(0);
				}
				HighClock(1);
				HighClock(1);
			}
		}
done:
		*p = '\0';
		printf("%d %d\n", nProb, (p - &(obuf[0]))/2);
		n = 0;
		p = &(obuf[0]);
		while(*p){
			putc(*p, stdout);
			p++;
			n++;
			if(n == 80){
				n = 0;
				putc('\n', stdout);
			}
		}
		if(n > 0){
			putc('\n', stdout);
		}
	}

	::fclose(fp);
	return(0);
}
