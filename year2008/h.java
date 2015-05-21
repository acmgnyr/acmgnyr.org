/*
 * Algorithm from F. J. Gruenberger, "What Should We Compute?", RAND
 * Corporation, September 1963
 */
import java.math.BigInteger;
import java.util.Scanner;

public class Powers {

    private static final BigInteger TEN_TWENTY
        = new BigInteger("100000000000000000000");
    private static final BigInteger ONE = BigInteger.ONE;
    private static final BigInteger TWO = BigInteger.valueOf(2);
    private static final BigInteger FOUR = BigInteger.valueOf(4);
    private static final BigInteger FIVE = BigInteger.valueOf(5);
    private static final BigInteger TEN = BigInteger.TEN;
    
        // 33 is sufficient for 15 digit answers
    private static BigInteger[] powtab = new BigInteger[50];
    private static BigInteger[] ntab = new BigInteger[21];

    static BigInteger new_pow(BigInteger n) {
        int i;
        BigInteger result = ONE;
        for (i = 0; i <= n.bitLength(); i++) {
            if (n.testBit(i)) {
                result = result.multiply(powtab[i]).mod(TEN_TWENTY);
            }
        }
        return result;
    }

    public static void main(String[] args) {
        BigInteger pow, n, delta;
        init_powtab();

        ntab[1] = ONE;
        delta = BigInteger.valueOf(4);
        n = ntab[1].add(delta);
        pow = BigInteger.valueOf(32);  // 2^n
        for (int r = 2; r < 21; r++) {
            while (!digits_ok(pow, r)) {
                n = n.add(delta);
                pow = new_pow(n);
            }
            ntab[r] = n;
            delta = delta.multiply(FIVE);
        }
        
        Scanner in = new Scanner(System.in);
        int num_cases = in.nextInt();
        for (int i = 1; i <= num_cases; i++) {
            int r = in.nextInt();
            System.out.println(i + " " + r + " " + ntab[r]);
        }
    }

    static void init_powtab() {
        BigInteger pow = TWO;
        for (int i = 0; i < 50; i++) {
            powtab[i] = pow;
            pow = pow.multiply(pow).mod(TEN_TWENTY);
        }
    }

    static boolean digits_ok(BigInteger pow, int r)
    {
        for(; r > 0; r--) {
            int d = pow.mod(TEN).intValue();
            if (d < 1 || d > 2)
                return false;
            pow = pow.divide(TEN);
        }
        return true;
    }
}
