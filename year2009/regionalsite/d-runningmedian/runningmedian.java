/*
 * Running median
 * John Buck
 * October, 2009
 * ACM GNY Programming Contest
 */
 import java.util.Scanner;

 public class RunningMedian {
	private static int [] Values = new int [10000];

	public static void main(String[] args){
		int nProbs, i, j, k, l, nProb, nValues, nValue, nLine, nMed;

		Scanner stdin = new Scanner(System.in);
		nProbs = stdin.nextInt();

		for(i = 1; i <= nProbs; i++){
			// Get problem number
			nProb = stdin.nextInt();
			// Get number of values
			nValues = stdin.nextInt();
			// Calculate # of medians
			nMed = (nValues/2) + 1;
			System.out.println(i + " " + nMed);
			// nLine is how many values we put out on current output line
			nLine = 0;
			for(j = 0; j < nValues; j++){
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
				// Odd line? (really, since j starts at 0)
				if((j & 1) == 0){
					if(nLine == 0){
						// No leading blank for first value on output line
						System.out.print(Values[j/2]);
						nLine = 1;
					} else if(nLine == 9){
						// Last value on line causes newline
						System.out.println(" " + Values[j/2]);
						// Starting new output line
						nLine = 0;
					} else {
						// All other values just have leading space
						System.out.print(" " + Values[j/2]);
						// Bump to next value on output line
						nLine++;
					}
				}
			}
			// If we put any values out on this output line, force newline
			if(nLine > 0){
				System.out.println("");
			}
		}
	}
 };