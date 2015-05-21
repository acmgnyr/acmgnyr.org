/*
 * QR Data Decoding
 * ACM GNY Regional Contest 2011
 *
 * Program by John Buck, C-Scape Consulting Corp.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define	MODE_NUMERIC	0x0001
#define	MODE_ALPHA		0x0002
#define	MODE_BYTE		0x0004
#define	MODE_KANJI		0x0008
#define	MODE_TERM		0x0000

/* There are 38 nibbles (19 bytes = 38 nibbles) */
unsigned char ucBytes[38];
/* Index into ucBytes during decode */
int idx;
/* Current bit position in current nibble */
unsigned char bit;

/* Quickie reference for counts for each mode */
int countbits[16] = {
	0, 10, 9, 0, 8, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0
};
/* Alphanumeric character set as per problem specification */
char *szAlphaNum = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

void ResetBytePointer()
{
	idx = 0;
	bit = 0x08;
}

/*
 * Get the next 'nbits' size value from the nibble stream
 */
int fetch_value(int nbits)
{
	int nVal = 0;
	int obit = (1 << (nbits-1));

	while(nbits-- > 0){
		if(idx < 38){
			if(ucBytes[idx] & bit){
				nVal |= obit;
			}
		}
		obit >>= 1;
		bit >>= 1;
		if(bit == 0x00){
			bit = 0x08;
			idx++;
		}
	}
	return(nVal);
}

int main(int argc, char **argv)
{
	int n, np, ds, j, m, bDone, cnt, v, nchar;
	char szBuf[256], *s;
	
	if(::fgets(&(szBuf[0]), sizeof(szBuf)-1, stdin) != NULL){
		np = ::atoi(&(szBuf[0]));
		for(n = 1; n <= np; n++){
			if(::fgets(&(szBuf[0]), sizeof(szBuf)-1, stdin) == NULL){
				break;
			}
			ds = ::atoi(&(szBuf[0]));
			if(ds != n){
				::fprintf(stderr, "Data set mismatch: %d != %d\n", n, ds);
				break;
			}
			s = ::strchr(&(szBuf[0]), ' ');
			if(s == NULL){
				break;
			}
			s++;
			/* Decode nibbles */
			for(j = 0; j < 38; j++, s++){
				if(isupper(*s)){
					ucBytes[j] = (*s - 'A') + 10;
				} else if(islower(*s)){
					ucBytes[j] = (*s - 'a') + 10;
				} else {
					ucBytes[j] = *s - '0';
				}
			}
			s = &(szBuf[0]);
			ResetBytePointer();
			/* Keep track of number of qr characters, NOT the length of the string
			 * we output!
			 */
			nchar = 0;
			for(bDone = 0; !bDone;){
				/* Get mode */
				m = fetch_value(4);
				/* Get count of chars */
				cnt = fetch_value(countbits[m]);
				nchar += cnt;
				switch(m){
				case MODE_TERM:
					bDone = 1;
					break;
				case MODE_NUMERIC:
					/* Decode numerics 3 at-a-time */
					while(cnt >= 3){
						v = fetch_value(10);
						s += ::sprintf(s, "%03d", v);
						cnt -= 3;
					}
					/* If any left, do them individually */
					if(cnt == 2){
						v = fetch_value(7);
						s += ::sprintf(s, "%02d", v);
					} else if(cnt == 1){
						v = fetch_value(4);
						s += ::sprintf(s, "%d", v);
					}
					break;
				case MODE_ALPHA:
					/* Decode alphanumerics 2 at-a-time */
					while(cnt >= 2){
						v = fetch_value(11);
						s += ::sprintf(s, "%c%c", szAlphaNum[v/45], szAlphaNum[v%45]);
						cnt -= 2;
					}
					/* Deal with left-over */
					if(cnt == 1){
						v = fetch_value(6);
						*s++ = szAlphaNum[v];
					}
					break;
				case MODE_BYTE:
					/* Bytes are relatively easy */
					while(cnt > 0){
						v = fetch_value(8);
						if(isprint(v)){
							/* Handle escaping of special chars */
							if(v == '\\' || v == '#'){
								*s++ = '\\';
							}
							*s++ = (char)v;
						} else {
							/* Non-printables */
							s += ::sprintf(s, "\\%02X", v);
						}
						cnt--;
					}
					break;
				case MODE_KANJI:
					/* Kanji, is perhaps, the easiest of all */
					while(cnt > 0){
						v = fetch_value(13);
						s += ::sprintf(s, "#%04X", v);
						cnt--;
					}
					break;
				}
			}
			*s = '\0';
			::fprintf(stdout, "%d %d %s\n", n, nchar, &(szBuf[0]));
		}
	}
	return(0);
}
