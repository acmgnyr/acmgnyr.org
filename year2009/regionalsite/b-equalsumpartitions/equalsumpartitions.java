/*
 * Equal Sum Partitions
 * John Buck
 * October, 2009
 * ACM GNY Programming Contest
 */
 import java.util.Scanner;

 public class EqualSumPartitions {
	private static int [] Values = new int [10000];
	private static int nValues;
	
	/*
	 * Starting at idxStart, sum up elements until the sum is
	 * equal to the desired value (sum), or bigger than it.  If bigger
	 * it means it's no good (IE sum wont work)
	 */
	private static int TrySum(int sum, int idxStart)
	{
		int trysum = 0;
	
		/* Loop through remaining values in array */	
		for(; idxStart < nValues; idxStart++){
			/* Accumulate sum */
			trysum += Values[idxStart];
			/* Same sum is good, so return index of next element */
			if(trysum == sum){
				return(idxStart+1);
			}
			/* Sum is too big, so it won't work */
			if(trysum > sum){
				return(-1);
			}
		}
		/* Used up remaining values, and sums are the same - thats good */
		if(trysum == sum){
			return(idxStart+1);
		}
		/* Sums are different meaning this sum can't work */
		return(-1);
	}
	
	private static int GetSmallestSum()
	{
		int i, j, k, sum;
		
		/* Main sum loop.  Start at left and try adding each value
		 * until we find one that works
		 */
		for(i = 0, sum = 0; i < nValues; i++){
			/* Accumulate the sum */
			sum += Values[i];
			/* Try each remaining subset */
			for(j = i+1;;){
				/* Sum up the right most elements until we get the right sum or not */
				k = TrySum(sum, j);
				/* Used up all values - this is good, return the sum */
				if(k >= nValues){
					return(sum);
				}
				/* Nope, won't work, get out and try adding in the next value */
				if(k == -1){
					break;
				}
				/* Now sum up the next group to the right */
				j = k;
			}
		}
		/* None worked, so just return the sum of the whole thing */
		return(sum);
	}
	
	public static void main(String[] args){
		int nProbs, i, j, nProb;

		Scanner stdin = new Scanner(System.in);
		nProbs = stdin.nextInt();

		for(i = 1; i <= nProbs; i++){
			// Get problem number
			nProb = stdin.nextInt();
			// Get number of values
			nValues = stdin.nextInt();
			for(j = 0; j < nValues; j++){
				// Next value
				Values[j] = stdin.nextInt();
			}
			/* Print answer */
			System.out.println(nProb + " " + GetSmallestSum());
		}
	}
};
