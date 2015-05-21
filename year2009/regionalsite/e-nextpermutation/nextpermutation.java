/*
 * Next Larger Permutation
 * John Buck
 * October, 2009
 * ACM GNY Programming Contest
 */
 import java.util.Scanner;

 public class NextPermutation {
 
	private static String NextPerm(String s)
	{
		int i, nLen, j, k, nleft, idxUsed;
		char lastc, c;
		nLen = s.length();
		String snew;
		
		// right most digit to prime the loop
		lastc = s.charAt(nLen-1);
		for(i = nLen-2; i >= 0; i--){
			// compare with next char to left
			c = s.charAt(i);
			// if smaller, then this one has to be substituted
			if(c < lastc){
				// left over stuff to left stays the same
				snew = s.substring(0, i);
				// find next biggest digit on the right starting from end (IE the smallest) - there has to be one
				for(j = nLen-1; j > i; j--){
					// is this digit bigger? If so, then it's the one
					if(s.charAt(j) > c){
						break;
					}
				}
				// Add the char right after the fixed stuff
				snew += s.charAt(j);
				// Remember index of the char we took
				idxUsed = j;
				
				/* Now, add remaining chars, in ascending order, which is reverse order,
				 * and insert 'c' in the right spot
				 */
				for(j = nLen-1; j > i; j--){
					// Was this the char we used to replace 'c' ?
					if(j == idxUsed){
						// yes, just stick 'c' in its place
						snew += c;
					} else {
						// otherwise, just copy the next digit
						snew += s.charAt(j);
					}
				}
				return(snew);
			}
			// Remember last char
			lastc = c;
		}
		// digits are in descending order, so it's the biggest possible
		return("BIGGEST");
	}
		
	public static void main(String[] args){
		int nProbs, i, nProb;
		String sDigits;

		Scanner stdin = new Scanner(System.in);
		nProbs = stdin.nextInt();

		for(i = 1; i <= nProbs; i++){
			// Get problem number
			nProb = stdin.nextInt();
			// Get string of digits and trim spaces
			sDigits = stdin.nextLine().trim();
			// Print answer
			System.out.println(nProb + " " + NextPerm(sDigits));
		}
	}
};
