/*
 * C - Immortal Porpoises
 * ICPC 2015 Greater NY Regional
 * Solution by Lee Wittenburg
 * Problem by Lee Wittenburg
 */
#include <stdio.h>
#include <stdlib.h>

/*
 * *sigh*
 */
#ifdef WIN32
typedef	__int64	LONGLONG;
#define	LL_FORMAT	"%I64d"
#else
typedef long long LONGLONG;
#define	LL_FORMAT	"%lld"
#endif

#define MAX_PORPS   1000000000

#define odd(n)  ((n) & 0x1)

// assuming 32-bit ints; not standard, but usual
// [logarithmic fib algorithm, max. input value = 2^48 (log = 48),
// with at most 100 input values => no more than 4800 values will
// need to be cached]
struct {
    LONGLONG index;
    int value;
} cache[4800] = { {1, 0}, {2, 1}, {3, 1}, {4, 2}, {5, 3}};
int nfib = 6;   // # of cached values

// linear search is fast enough (with O(log n) fib algorithm)
int lookup(LONGLONG n)
{
    int i;
    for (i = 0; i < nfib; i++) {
        if (cache[i].index == n) { return cache[i].value; };
    }
    return -1;
}

// remember that fib(n) = f
void insert(LONGLONG n, int f)
{
    cache[nfib].index = n;
    cache[nfib++].value = f;
}

// f(1) = 0, f(2) = 1, f(3) = 1, f(4) = 2, ... , f(n) = f(n-1) + f(n-2),
// return f(n) mod MAX_PORPS
// O(log n) algorithm
int fib_mod(LONGLONG n)
{
    int result = lookup(n);
    if (result >= 0) { return result; }
    if (odd(n)) {
        LONGLONG fib_half_1 = fib_mod(n / 2 + 1);
        result = (2 * fib_mod(n / 2) * fib_half_1
                + fib_half_1 * fib_half_1) % MAX_PORPS;
        insert(n, result);
    } else {
        LONGLONG fib_half   = fib_mod(n / 2);
        LONGLONG fib_half_1 = fib_mod(n / 2 + 1);
        result = (fib_half * fib_half + fib_half_1 * fib_half_1) % MAX_PORPS;
        insert(n, result);
    }
    return result;
}

int main(int argc, char *argv[])
{
    int p, i;
    scanf("%d", &p);
    for (i = 0; i < p; i++) {
        int n; LONGLONG y;
        scanf("%d", &(n));
		scanf(LL_FORMAT, &y);
        printf("%d ", n);
		printf(LL_FORMAT, fib_mod(y+1));
		printf("\n");
    }
    return EXIT_SUCCESS;
}
