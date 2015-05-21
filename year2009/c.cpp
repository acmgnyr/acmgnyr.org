/*
 * C - Balls
 * ACM International Collegiate Programming Contest
 * Greater New York Region
 * October 18, 2009
 */

#include <iostream>
#include <algorithm> // for max, min

using namespace std;

// Maximum number of balls and storys in the building
const int NUM_BALLS = 50;
const int NUM_FLOORS = 1000;

// Memoization
int array[NUM_BALLS+1][NUM_FLOORS+1];

// Return minimum number of drops that are required, in the worst case,
// with b balls and f floors.
int compute(const int b, const int f)
   {
#ifdef NO_MEMOIZE
   // Do not take advantage of memoization.
   // Provide base case only.
   if (b == 1) return f;
   if (f == 0) return 0;
   if (f == 1) return 1;
#else
   // Do take advantage of memoization.
   // Check to see if we've already computed this one.
   if ( array[b][f] >= 0 )
      return array[b][f];
#endif

   // Note that with f floors we'll never need more than f drops.
   // So we can safely initialize minimum number of drops to any
   // number f or higher.
   int m = f;
   for (int n = 1; n <= f; ++n)
      {
      // Drop from floor n.
      // If it breaks, we have to test floors 1 to n-1 with b-1 balls.
      const int x = compute(b-1, n-1);
      // If it doesn't break, we have to test floors n+1 to f with b balls.
      const int y = compute(b, f-n);
      // We've used 1 drop already. What's worst case number of drops?
      const int z = 1 + (x > y ? x : y);
      // Keep track of the minimum over all n.
      m = (z < m ? z : m);
      }

   // Memoize this solution
   array[b][f] = m;

   // Return
   return m;
   }

int main(int argc, char* argv[])
   {
   int b, prob;
   // ----------------------------------
   // Initialize memoization array
   // ----------------------------------

   // With just 1 ball we have to check every floor
   for (int f = 1; f <= NUM_FLOORS; ++f)
      array[1][f] = f;

   // With just 1 floor we only need one drop.
   // With zero floors we don't need any drops.
   for (b = 2; b <= NUM_BALLS; ++b)
      {
      array[b][1] = 1;
      array[b][0] = 0;
      }

   // Initialize other elements to -1, which means we haven't computed
   // them yet.
   for (b = 2; b <= NUM_BALLS; ++b)
      for (int f = 2; f <= NUM_FLOORS; ++f)
         array[b][f] = -1;

   // ----------------------------------
   // Process input
   // ----------------------------------

   // Read number of data sets to process
   int num;
   cin >> num;

   // Process each data set identically
   for (int i = 1; i <= num; ++i)
      {
      int b, f;
      cin >> prob;
      cin >> b >> f;
      cout << prob << " " << compute(b,f) << endl;
      }

   // We're done
   return 0;
   }
