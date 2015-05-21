/*
 * Eclipse Java project for C - Lucky Lenny
 *
// Lenny's Lucky Lotto Lists
//
// Problem Description:
// Lenny likes to play the lotto in Lincoln county Louisiana. In the game,
// he picks a list of n numbers in the range from 1 to m. If his list
// matches the drawn list, he wins the big prize, a lifetime supply of
// large lemons.
//
// Lenny has a scheme that he thinks is likely to be lucky. He likes to
// choose his list so that each number in it is at least twice as large
// as the one before it. So, for example, if n = 4 and m = 10, then the
// possible lucky lists Lenny could like are:
//    1 2 4 8
//    1 2 4 9
//    1 2 4 10
//    1 2 5 10
// Thus Lenny has 4 lists to choose from.
//
// Your job, given n and m, is to count how many lucky lists Lenny
// has to choose from.
//
// Input Description:
// First line of input is a single non-negative integer, which is the
// number of data sets to follow. All data sets should be handled
// identically.
// The next lines, one per data set, contain two integers, n and m.
// You are guaranteed that 1 <= n <= 10 and 1 <= m <= 2000 and
// n <= m.
//
// Output Description:
// For each data set, print a line like the following:
//   Data set <i>: <n> <m> <number>
// where <i> is the data set number (beginning with 1), and <number>
// is the number of lucky lists corresponding to the provided values
// of n and m.
// This version uses dynamic programming. It took about
// 6 seconds on my machine (a 1.8ghz p4).
// NOTE: Let L(n,m) be the number of lists with the provided n and m.
// Notice that L has the following properties:
// (1) L(1,m) = m. That is, if you only have to choose one number from
//     1 to m, there are m ways to do it.
// (2) L(n, 2^{n-1}) = 1. That is, the only possible list is
//     1, 2, 4, ..., 2^{n-1}.
// (3) L(n, k) = 0 for k < 2^{n-1}. A corollary to the above property.
// (4) The recursive relationship for L is
//       L(i,j) = \sum_{k=1}^{j/2} ( L(i-1,k) - s_{k-1} ) * 
//     where j/2 is performed with integer division, and
//       s_k = \sum_{p=1}^k L(i-1,p) .
//     This leads to the solution, below. */
import java.io.*;
import java.math.BigInteger;

/**
 * @author Adam FLorence (Java version by: John Buck)
 *
 */
public class CLuckyLenny {
	public static final int MAX_N = 10;
	public static final int MAX_M = 2000;
	private BigInteger [][] m_data; 
	StreamTokenizer m_tok;
	
	CLuckyLenny()
	{
		m_data = new BigInteger[MAX_N+1][MAX_M+1];
		int i, j;
		/* Create all the big nums */
		for(i = 0; i <= MAX_N; i++){
			for(j = 0; j <= MAX_M; j++){
				m_data[i][j] = BigInteger.ZERO;
			}
		}
	}

	//	 Note: This is the fast solution. It uses dynamic programming. Populate
	//	 the global array data once, then perform lookups.
	void BuildTable()
	{
	   int k, i, j, jj;
	   BigInteger b;
	   
	   // Base case: There are m lists when n = 1.
	   for (j = 1; j <= MAX_M; ++j){
	      m_data[1][j] = BigInteger.valueOf((long)j);
	   }

	   // Inductive case.
	   for (i = 2; i <= MAX_N; ++i){
	      m_data[i-1][0] = BigInteger.ZERO;
	      for(j = 1; j <= MAX_M; ++j){
	         m_data[i][j] = BigInteger.ZERO;
	         jj = j / 2;
	         for(k = 1; k <= jj; ++k){
	         	b = m_data[i-1][k];
	         	b = b.subtract(m_data[i-1][k-1]);
	         	b = b.multiply(BigInteger.valueOf((long)(j - 2*k + 1)));
	         	m_data[i][j] = m_data[i][j].add(b);
	         }
	      }
	   }
	}
	
	void Lotto(int d)
	{
		int nTok, n, m;
		BigInteger b;
		
		try {
			/* Get N */
			while((nTok = m_tok.nextToken()) != StreamTokenizer.TT_NUMBER && nTok != StreamTokenizer.TT_EOF){
				;
			}
			if(nTok == StreamTokenizer.TT_EOF){
				return;
			}
			n = (int)m_tok.nval;
			/* Get M */
			while((nTok = m_tok.nextToken()) != StreamTokenizer.TT_NUMBER && nTok != StreamTokenizer.TT_EOF){
				;
			}
			if(nTok == StreamTokenizer.TT_EOF){
				return;
			}
			m = (int)m_tok.nval;
			b = m_data[n][m];
			System.out.println("Data set " + d + ": " + n + " " + m + " " + b.toString());
		} catch(Exception e){
			System.out.println("Oops2");
		}
	}
	
	void run()
	{
		int nTok, i, n;
		try {
			m_tok = new StreamTokenizer(new BufferedReader(new FileReader("c.in")));
			m_tok.parseNumbers();
			for(;;){
				while((nTok = m_tok.nextToken()) != StreamTokenizer.TT_NUMBER && nTok != StreamTokenizer.TT_EOF){
					;
				}
				/* End of input? */
				if(nTok == StreamTokenizer.TT_EOF){
					break;
				}
				/* Convert */
				n = (int)m_tok.nval;
				/* Let's do it */
				for(i = 1; i <= n; i++){
					Lotto(i);
				}
				/* All done */
				break;
			}
		} catch(Exception e){
			/* We dont care if we get here - something bad happend */
			System.out.println("Oops");
		}
	}
	public static void main(String[] args) {
		CLuckyLenny prog = new CLuckyLenny();
		prog.BuildTable();
		prog.run();
	}
}
