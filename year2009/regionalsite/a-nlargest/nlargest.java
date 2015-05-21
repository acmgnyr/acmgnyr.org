/*
 * MaxNthValue
 * John Buck
 * October, 2009
 * ACM GNY Programming Contest
 */
 import java.util.Scanner;

 public class NLargest {
	private static final int MAX_VALUES = 10;
	private static final int NTH = MAX_VALUES-3;
	private static int [] Values = new int [MAX_VALUES];

	public static void main(String[] args){
		int nProbs, i, j, k, l, nProb, nValue;

		Scanner stdin = new Scanner(System.in);
		nProbs = stdin.nextInt();

		for(i = 1; i <= nProbs; i++){
			// Get problem number
			nProb = stdin.nextInt();
			// Get values
			for(j = 0; j < MAX_VALUES; j++){
				// Next value
				nValue = stdin.nextInt();
				// Insertion sort
				for(k = 0; k < j; k++){
					if(nValue < Values[k]){
						break;
					}
				}
				// Shift everything right after where the new element goes
				for(l = j; l > k; l--){
					Values[l] = Values[l-1];
				}
				Values[k] = nValue;
			}
			System.out.println(nProb + " " + Values[NTH]);
		}
	}
 };