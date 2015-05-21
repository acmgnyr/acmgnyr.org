/*
 * Adjacent Bit Counts
 * John Buck
 * October, 2009
 * ACM GNY Programming Contest
 */
 import java.util.Scanner;

/*
 * let N(n,k) = the number of bit strings of length n and
 * adjacent bit count k
 * we split that into
 * N0(n,k) = number of bit strings ending in 0 with AdjBC = k
 * N1(n,k) = number of bit strings ending in 1 with AdjBC = k
 * so N(n,k) = N0(n,k) + N1(n,k)
 * the only way to get k adjacent 1 bits in a string ending in 0
 * is if left n-1 bits has k adjacent 1 bits
 * so N0(n,k) = N(n-1,k) = N0(n-1,k) + N1(n-1,k)
 * if the last bit is 1, we can get AdjBC = k if
 * a) the bit before last is 0 and the first n-1 bits have AdjBC=k OR
 * b) the bit before last is 1 and the first n-1 bits have AdjBC=k-1 i.e
 * N1(n,k) = N0(n-1,k) + N1(n-1,k-1)
 *
 * NOTE: N(n,k) = N0(n,k) = N1(n,k) = 0 if k < 0 or k >= n
 * also
 * N1(1,0) = 1, N0(1,0) = 1, N(n,n-1) = 1 (all 1 bits)
 */

 public class AdjacentBits {
	private static final int MAX_N = 100;
	private static final int MAX_PRE = 10;

	private static int NLargest = 0;
	private static long [][] N0 = new long [MAX_N+1][MAX_N+1];
	private static long [][] N1 = new long [MAX_N+1][MAX_N+1];

	private static void AdjInit()
	{
		int i, j;
		for(i = 0; i <= MAX_N ; i++){
			for(j = 0; j <= MAX_N ; j++){
				N0[i][j] = N1[i][j] = 0;
			}
		}
		N0[1][0] = N1[1][0] = 1;
		for(i = 2; i <= MAX_PRE ; i++){
			N0[i][0] = N0[i-1][0] + N1[i-1][0];
			N1[i][0] = N0[i-1][0];
			for(j = 1; j < i; j++){
				N0[i][j] = N0[i-1][j] + N1[i-1][j];
				N1[i][j] = N0[i-1][j] + N1[i-1][j-1];
			}
		}
		NLargest = MAX_PRE;
	}

	private static long AdjBC(int n, int k)
	{
		int i, j;

		if((k < 0) || (k >= n)){
			return 0;
		}
		if(k == n-1){
			return 1;
		}
		if(n <= NLargest){
			if((N0[n][k] < 0) || (N1[n][k] < 0)){
				return -1;
			}
			return (N0[n][k] + N1[n][k]);
		}
		for(i = NLargest+1 ; i <= n ; i++){
			if(N0[i-1][0] < 0){
				N0[i][0] = N1[i][0] = -1;
			} else {
				N0[i][0] = N0[i-1][0] + N1[i-1][0];
				N1[i][0] = N0[i-1][0];
			}
			for(j = 1; j < i ; j++){
				if((N0[i-1][j] < 0) || (N1[i-1][j] < 0)){
					N0[i][j] = -1;
				} else {
					N0[i][j] = N0[i-1][j] + N1[i-1][j];
				}
				if((N0[i-1][j] < 0) || (N1[i-1][j-1] < 0)){
					N1[i][j] = -1;
				} else {
					N1[i][j] = N0[i-1][j] + N1[i-1][j-1];
				}
			}
		}
		if((N0[n][k] < 0) || (N1[n][k] < 0)){
			return -1;
		}
		return (N0[n][k] + N1[n][k]);
	}

	public static void main(String[] args){
		int nProbs, i, nProb, nBits, nBitCount;
		String sDigits;

		Scanner stdin = new Scanner(System.in);
		nProbs = stdin.nextInt();

		// Initialize table
		AdjInit();

		for(i = 1; i <= nProbs; i++){
			// Get problem number
			nProb = stdin.nextInt();
			// Get bits
			nBits = stdin.nextInt();
			// Get bit count
			nBitCount = stdin.nextInt();
			// Print answer
			System.out.println(nProb + " " + AdjBC(nBits, nBitCount));
		}
	}
}
