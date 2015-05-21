/* *************************************************************************
Glass Balls

Description:

The classic "two glass balls" brain-teaser is often posed as:

"Given two identical glass spheres, you would like to determine the lowest
floor in a 100-story building from which they will break when dropped.
Assume the spheres are undamaged when dropped below this point.
What is the strategy that will minimize the worst-case scenario for number
of drops?"

Suppose for a moment that we had only one ball. We'd have to drop from
each floor from 1 to 100 in sequence, requiring 100 drops in the worst
case.

Now consider the case where we have two balls. We know the ball does not
break when dropped from floor i = 0. Suppose we drop the first
ball from floor n. If it breaks we're in the case where we have one ball
remaining and we need to drop from floors 1 to n-1 in sequence, yielding
n drops in the worst case (the first ball is dropped once, the second at
most n-1 times). However, if it does not break when dropped from floor
n, we have reduced the problem to dropping from floors n+1 to 100. In
either case we must keep in mind that we've already used one drop. So
the minimum number of drops, in the worst case, is the minimum over all
n.

It turns out there's a closed-form solution for the best n:
   ceil( ( sqrt(1+8f) - 1 ) / 2 )
where f is the number of floors. However, knowing this isn't necessary
to solve the problem.


Your task is to determine the minimum number of drops required, in the
worst case, given B balls and an M-story building.

Input:

The first line of input contains a single positive integer N which
is the number of data sets that follow. Each data set should be
processed identically and independently.

The remaining N lines of input will each contain two integers, B and M,
space separated. You are guaranteed that 1 <= B <= 50 and
1 <= M <= 1000.

Output:

For each data set, print the data set number as a decimal integer
(start counting at one), a space, and the solution for the corresponding
values of B and M.

Sample Input:

9
2 10
2 100
2 200
2 300
3 100
4 100
5 100
10 100
25 900

Sample Output:

1 4
2 14
3 20
4 24
5 9
6 8
7 7
8 7
9 10

************************************************************************* */

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
