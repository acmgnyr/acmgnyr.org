/*
 * D - Happy Happy Prime Prime
 * ICPC 2014 Greater NY Regional
 * Solution by Adam Florence
 * Problem by Adam Florence
 */

#include <iostream>
#include <vector>
#include <set>
#include <cmath> // sqrt

using namespace std;

const unsigned MAX = 10000;

int main(void)
{
	// Initialize is_happy array to "don't know".
	// 0 = don't know, 1 = happy, 2 = not happy
	vector<char> is_happy(MAX+1, 0);

	is_happy[1] = 1;
	
	// Initialize is_prime using sieve
	vector<bool> is_prime(MAX+1, true);
	is_prime[1] = false;
	for (unsigned i = 2; i <= sqrt(MAX+1); ++i){
		if (is_prime[i])
		{
			for (unsigned j = i+i; j <= MAX; j += i)
				is_prime[j] = false;
		}
	}
		
//		cerr << "--> " << 7 << " " << is_prime[7] << endl;
		
	// Read number of data sets to process
	int num;
	cin >> num;
	
	// Process each data set identically
	for (int ds = 1; ds <= num; ++ds)
	{
		unsigned m, dsn;
		cin >> dsn >> m;
		
		// If it's not prime, or known to be not happy, stop
		if (!is_prime[m] || is_happy[m] == 2)
		{
			cout << ds << " " << m << " " << "NO" << endl;
			continue;
		}
		
		// Otherwise it's prime. If it's happy, stop
		if (is_happy[m] == 1)
		{
			cout << ds << " " << m << " " << "YES" << endl;
			continue;
		}
		
		// Otherwise, check whether it's happy
		set<unsigned> seen;
		unsigned n = m;

		do {
			seen.insert(n);
//			cerr << "n = " << n << endl;
			
			unsigned sum = 0;
			while (n > 0)
			{
				const unsigned digit = n % 10;
				n = n / 10;
				sum += digit * digit;
			}
			n = sum;
//			cerr << "sum = " << sum << ", is_happy " << is_happy[n] << endl;
		} while ( seen.find(n) == seen.end() && is_happy[n] == 0 );
		
		// Everything in seen set has same happiness as n
		for (set<unsigned>::iterator it = seen.begin();
		it != seen.end(); ++it){
			is_happy[*it] = is_happy[n];
		}
		
		cout << ds << " " << m << " " << (is_happy[m] == 1 ? "YES" : "NO")
			<< endl;
	}
	
	// We're done
	return 0;
}
