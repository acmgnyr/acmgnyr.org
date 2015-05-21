/*
 * B - Von Neumann's Fly
 * ICPC 2013 Greater NY Regional
 * Solution by Lee Wittenburg
 * Problem by Lee Wittenburg
 */
/*
 * Note that it is not really necessary to bother with the infinite
 * sum.  All we need is the formula
 *      Distance = Rate * Time.
 * Given d (the initial distance), a (cyclist A's speed) and 
 * b (cyclist B's speed), we can easily compute the time at which
 * A and B meet, t:
 *      at = the distance traveled by A when they meet
 *      bt = the distance traveled by B when they meet
 *      at + bt = d
 *      (a + b)t = d
 *      t = d/(a + b)
 * We need merely to multiply t by f (the fly's speed) to determine
 * the distance traveled by the fly in that time.
 *
 * The parameters of the problem ensure that float's are sufficient;
 * double precision is unnecessary.
 */
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int p, i;
    scanf("%d", &p);

    for (i = 0; i < p; i++) {
        int n;
        float d, a, b, f;
        float t;        /* time traveled at meeting */
        float result;
        scanf("%d %f %f %f %f", &n, &d, &a, &b, &f);
        t = d / (a + b);
        result = f * t;
        printf("%d %1.2f\n", n, result);
    }

    return EXIT_SUCCESS;
}
