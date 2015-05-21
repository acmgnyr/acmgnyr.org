/*
 * C - Balls
 * ACM International Collegiate Programming Contest
 * Greater New York Region
 * October 18, 2009
 */

import java.util.Scanner;

public class Balls {
	private static final int NUM_BALLS = 50;
	private static final int NUM_FLOORS = 1000;
	
	private static int [][] array = new int [NUM_BALLS+1][NUM_FLOORS+1];
	
	private static int Drop(int nb, int ns)
	{
		int m, n, x, y, z;
		
		/*
		 * Terminal cases
		 */
		if(array[nb][ns] >= 0){
			return(array[nb][ns]);
		}
		// With ns stories, we'll never need more than ns drops, so set min to this for starters
		for(m = ns, n = 1; n <= ns; n++){
			// Drop from floor n, if it breaks we have to test floors 1 to n-1 with b-1 balls
			x = Drop(nb-1, n-1);
			// If it doesnt break, we have to test floors n+1 to ns with nb balls
			y = Drop(nb, ns-n);
			// We've used 1 drop already.  What's the worst case number of drops
			z = 1 + (x > y ? x : y);
			// Keep track of minimum over all n
			if(z < m){
				m = z;
			}
		}
		array[nb][ns] = m;
		return(m);
	}
	public static void main(String[] args){
		int nProbs, nBalls, nStories, i, j, k, l, nProb, nValue;
		Scanner stdin = new Scanner(System.in);
		
		// Set up matrix
		
		// With one ball, need to check every floor
		for(nStories = 1; nStories <= NUM_FLOORS; nStories++){
			array[1][nStories] = nStories;
		}
		// With 1 story we only need one drop.
		// With 0 stories, we don't need any
		for(nBalls = 2; nBalls <= NUM_BALLS; nBalls++){
			array[nBalls][1] = 1;
			array[nBalls][0] = 0;
		}
		// Initialize other elements to -1, meaning we have to fill them in
		for(nBalls = 2; nBalls <= NUM_BALLS; nBalls++){
			for(nStories = 2; nStories <= NUM_FLOORS; nStories++){
				array[nBalls][nStories] = -1;
			}
		}
		// Get # of problems
		nProbs = stdin.nextInt();

		for(i = 1; i <= nProbs; i++){
			// Get problem number
			nProb = stdin.nextInt();
			// Get balls
			nBalls = stdin.nextInt();
			// Get number of stories
			nStories = stdin.nextInt();
			
			System.out.println(nProb + " " + Drop(nBalls, nStories));
		}
	}
 };
