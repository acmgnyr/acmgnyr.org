/*
 * B - Floating Point Conversion
 * ICPC 2013 Greater NY Regional
 * Solution by Fred Pickel
 * Problem by Fred Pickel
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

typedef unsigned long DWORD;

/*
 Conversion process	(FOR normalized numbers)		(with example 0x22222222)	
 extract sign = GouldFP & 0x80000000					sign = 0
 Gould offset exponent = (GouldFP & 0x7f000000) >> 24	off exp = 0x22 = 34
	actual exponent = offset exponent - 64				(actual exponent = 34 - 64 = -30
 mantissa = GouldFP & 0xffffff;							mantissa = 0x222222
													value = ((16)^-30)*(0x222222/2^24)
 convert exponent to base 2							value = ((2)^-120)*(0x222222/2^24)
 IF mantissa == 0, return 0
 multiply mantissa by 2 and decrement exponent		value = ((2)^-121)*(0x444444/2^24)
	until MSB of matissa is 1						value = ((2)^-122)*(0x888888/2^24)
 change mantissae divisor to 2^23
	and decrement exponent							value = ((2)^-123)*(0x888888/2^23)
 pull out MSB of mantissa (now = 1)
	mantissa &= 0x7fffff							value = ((2)^-123)*(1 + (0x088888/2^23))
 read off ieee FP terms
 sign = Gould sign									sign = 0 exp = -123 + 127 = 4 mantissa = 0x088888
 exp = base 2 exp + 127
 mantissa &= 0x7fffff
 IEEE FP = Gould sing | (exp << 23) |mantissa		0x02088888
 */
DWORD FormatConvert(DWORD GouldFP)
{
	DWORD sign, expon, mantissa, res;
	int expv;
	// extract Gould terms
	sign = GouldFP & 0x80000000;
	expon = (GouldFP & 0x7f000000) >> 24;
	expv = expon - 64;		// actual exponent
	mantissa = GouldFP & 0xffffff;
	if(mantissa == 0) {
		return 0;
	}
	expv *= 4;		// convert exponent to base 2
	while((mantissa & 0x800000) == 0){	// multiply mantissa by 2 and decrement exponent
		mantissa <<= 1;					// until MSB of matissa is 1
		expv--;
	}
	expv--;		//change mantissae divisor to 2^23 and decrement exponent
	if(expv > 127) { // +- infinity
		res = sign | 0x7f800000;
	} else if(expv < -149) {
		res = sign;	// +- 0
	} else if(expv >= -126) {
		expon = expv + 127;	// normal number
		expon <<= 23;
		res = sign | expon | (mantissa & 0x7fffff);
	} else { // denormal number
		while(expv < -126) {	// divide mantissa by 2 and increment exponent
			expv++;				// until exponent is -126
			mantissa >>= 1;
		}
		expon = 0;		// signal denormalized
		res = sign | expon | (mantissa & 0x7fffff);
	}
	return res;
}

int main()
{
	int nprob, curprob, index;
	DWORD inval, res;

	if(scanf("%d", &nprob) != 1)
	{
		fprintf(stderr, "Scan failed on problem count\n");
		return -2;
	}
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		// get prob num and sequence index
		if(scanf("%d %lx", &index, &inval) != 2)
		{
			fprintf(stderr, "scan failed on problem data problem index %d\n",
				curprob);
			return -6;
		}
		res = FormatConvert(inval);
		printf("%d %08lX\n", curprob, res);
	}
	return 0;
}
