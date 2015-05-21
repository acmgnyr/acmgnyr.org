/*
 *  ACM2006 Greater NY Programming Contest - Problem B - Triangular Numbers
 *  
 *  John Buck
 *  October 2006
 */
public class CTriangularNumbers {
	/* Set to true to use quickie magic formula */
	public static final boolean MAGIC_FORMULA = false;
	
	/**
	 * Everything is done in main, why bother with classes?
	 */
	public static void main(String[] args) {
		int d, n, k, i, sum, t;
		String ibuf;

		/* Get number of datasets */
		ibuf = readString();
		d = Integer.parseInt(ibuf);
		/* For each dataset */
		for(i = 1; i <= d; i++){
			/* Get n */
			ibuf = readString();
			n = Integer.parseInt(ibuf);
			/* Calculate sum: recall: T(n) = T(n-1) + n */
			sum = 0;
			/* T(1) = 1 */
			t = 1;
			for(k = 1; k <= n; k++){
				/* Next T */
				t = t + (k+1);
				sum += k*t;
			}
			if(MAGIC_FORMULA){
				System.out.println(i + " " + sum + "  (Formula: " +
						(n*(n+1)*(n+2)*(n+3))/8 + ")");
			} else {
				System.out.println(i + " " + sum);
			}
		}
	}

	/*
	 * It is advisable to always read a string buffer
	 * and parse it as needed.  This insures there are
	 * no issues with line terminators, etc.
	 * The I/O is done character at-a-time. *sigh*
	 */
	public static String readString()
	{
		int ch;
		String r = "";
		boolean done = false;
		while (!done)
		{
			try {
				ch = System.in.read();
				if (ch < 0 || (char)ch == '\n'){
					done = true;
				} else if((char)ch != '\r'){
					r = r + (char) ch;
				}
			} catch(java.io.IOException e) {
				done = true;
			}		
		}
		return(r);
	}
}
