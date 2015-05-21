/*
 * A - Penney Game
 * ACM International Collegiate Programming Contest
 * Greater New York Region
 * October 24, 2010
 */

#include <iostream>
#include <string>

using namespace std;

int array[8] = { 0 };

int main(void)
{
	// Read number of data sets to process
	int i, j, num, index, pn;

	cin >> num;
	
	// Process each data set identically
	for (i = 1; i <= num; ++i)
	{
		// read the problem number
		cin >> pn;

		// Read the sequence of forty coin flips
		string seq;
		cin >> seq;
	
		// Clear out array each time through
		for(j = 0; j < 8; j++){
			array[j] = 0;
		}
		// We will represent the eight three-coin sequences as
		// binary numbers, where T is zero and H is 1. So TTT has
		// index 0, TTH has index 1, etc. Note this corresponds
		// to the requested output order.
		
		for (j = 0; j < 38; ++j)
		{
			index = ( seq[j]	 == 'H' ? 4 : 0 )
				+ ( seq[j+1] == 'H' ? 2 : 0 )
				+ ( seq[j+2] == 'H' ? 1 : 0 );
			array[index]++;
		}
		
		// Print results
		cout << i;
		for (unsigned index = 0; index < 8; ++index){
			cout << " " << array[index];
		}
		cout << endl;
	}
	
	// We're done
	return 0;
}
