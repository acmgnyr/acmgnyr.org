/*
 * C - Just The Simple Fax
 * ACM International Collegiate Programming Contest
 * Greater New York Region
 * October 24, 2010
 */

//#define	DECODE

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>

#define	BIT_RAW	0
#define	BIT_RLE	0x80

int nRaw = 0;
unsigned char *pRaw = NULL;
unsigned char *pDst = NULL;

void FlushRaw()
{
	while(nRaw > 128){
		*pDst++ = BIT_RAW|127;
		memcpy(pDst, pRaw, 128);
		pRaw += 128;
		pDst += 128;
		nRaw -= 128;
	}
	if(nRaw > 0){
		*pDst++ = BIT_RAW|(nRaw-1);
		memcpy(pDst, pRaw, nRaw);
		pRaw += nRaw;
		pDst += nRaw;
		nRaw = 0;
	}
}

int FlushRLE(int n, unsigned char c)
{
	int nRes = 0;

	if(n >= 3){
		FlushRaw();
		/* Handle big blocks */
		while(n > 130){
			*pDst++ = BIT_RLE|127;
			*pDst++ = c;
			n -= 130;
			nRes += 130;
		}
		/* Worth encoding */
		if(n >= 3){
			*pDst++ = BIT_RLE|(n-3);
			*pDst++ = c;
			nRes += n;
		}
	}
	return(nRes);
}


/*
 * Decode 
 * Input:
 *	pSrc	Ptr to source encoded RLE data
 *  nLen	# of bytes of data to use in pSrc, -1 means keep going until nOut reached
 *  pOut	Ptr to destination of decoded data
 *	nOut	# of bytes of output to generate.  -1 means use up all input.
 *
 * Output:
 *	# of bytes decoded, or -1 on error
 *
 * Notes: nLen and nOut must not both be -1
 */
int Decode(unsigned char *pSrc, int nLen, unsigned char *pOut, int nOut)
{
	int n;
	unsigned char *pOutOrig = pOut;
	int bSrcAll = (nLen == -1);
	int bDstAll = (nOut == -1);

	/* Not allowed */
	if(bDstAll && bSrcAll){
		return(-1);
	}
	if(nOut == 0 || nLen == 0 || nLen == 1){
		return(0);
	}
	for(;;){
		n = (*pSrc & 0x7f);
		if(nLen == 1){
			break;
		} else if(nLen > 0){
			nLen--;
		}
		/* Decode a run */
		if(*pSrc++ & BIT_RLE){
			n += 3;
			while(n-- > 0 && (bDstAll || nOut > 0)){
				*pOut++ = *pSrc;
				if(!bDstAll){
					nOut--;
				}
			}
			pSrc++;
			if(!bSrcAll){
				nLen--;
			}
			if(nLen == 0 || nLen == 1 || nOut == 0){
				break;
			}
		} else {
			n++;
			/* Decode raw data */
			while(n-- > 0 && (bSrcAll || nLen > 0) && (bDstAll || nOut > 0)){
				*pOut++ = *pSrc++;
				if(!bSrcAll){
					nLen--;
				}
				if(!bDstAll){
					nOut--;
				}
			}
			if(nLen == 0 || nLen == 1 || nOut == 0){
				break;
			}
		}
	}
	return(pOut - pOutOrig);
}

int Encode(unsigned char *pSrc, int nLen, unsigned char *pOut)
{
	int nRLE, i, n;
	unsigned char *p;

	i = 0;
	nRaw = 0;
	pRaw = pSrc;
	pDst = pOut;

	while(i < nLen){
		/* Count same in a row */
		for(p = pSrc, nRLE = 0; i < nLen; i++, nRLE++, p++){
			if(*p != *pSrc){
				break;
			}
		}
		n = FlushRLE(nRLE, *pSrc);
		pSrc += n;
		if(nRaw == 0){
			pRaw = pSrc;
		}
		nRLE -= n;
		if(nRLE > 0){
			/* Apply this to raw bytes */
			nRaw += nRLE;
			pSrc += nRLE;
			nRLE = 0;
		}
	}
	FlushRaw();
	return(pDst - pOut);
}

char buf[128];
int nLeft = 0;
int idx;

unsigned char GetByte()
{
	char *s;
	unsigned char bret;

	if(nLeft == 0){
		/* They promise there's enough data, so this shouldnt happen */
		if(fgets(&(buf[0]), sizeof(buf), stdin) == NULL){
			return(0);
		}
		for(s = &(buf[0]); isxdigit(*s); s++){
		}
		nLeft = s - &(buf[0]);
		idx = 0;
	}
	if(nLeft < 2){
		return(0);
	}
	if(isdigit(buf[idx])){
		bret = (buf[idx] - '0');
	} else if(islower(buf[idx])){
		bret = (buf[idx] - 'a' + 10);
	} else {
		bret = (buf[idx] - 'A' + 10);
	}
	bret <<= 4;
	nLeft--;
	idx++;
	if(isdigit(buf[idx])){
		bret |= (buf[idx] - '0');
	} else if(islower(buf[idx])){
		bret |= (buf[idx] - 'a' + 10);
	} else {
		bret |= (buf[idx] - 'A' + 10);
	}
	nLeft--;
	idx++;
	return(bret);
}

void DumpBytes(unsigned char *p, int n)
{
	int i;

	for(i = 0; i < n; i++, p++){
		printf("%02X", *p);
		if((i%40) == 39){
			putc('\n', stdout);
		}
	}
	if(i%40){
		putc('\n', stdout);
	}
}

#define	MAX_IN	5000
#define	MAX_OUT	10000

unsigned char ibuf[MAX_IN];
unsigned char obuf[MAX_OUT];

int main(int argc, char **argv)
{
	int nProb;
	int n, np, nb, i, nres;

	while(fgets(&(buf[0]), sizeof(buf), stdin) != NULL){
		nProb = atoi(&(buf[0]));
		for(n = 1; n <= nProb; n++){
			if(fgets(&(buf[0]), sizeof(buf), stdin) == NULL){
				break;
			}
			if(sscanf(&(buf[0]), "%d %d", &(np), &(nb)) != 2){
				break;
			}
			nLeft = 0;
			for(i = 0; i < nb; i++){
				ibuf[i] = GetByte();
			}
#ifdef DECODE
			nres = Decode(&(ibuf[0]), nb, &(obuf[0]), -1);
#else
			nres = Encode(&(ibuf[0]), nb, &(obuf[0]));
#endif
			printf("%d %d\n", n, nres);
			DumpBytes(&(obuf[0]), nres);
		}
	}
	return(0);
}
