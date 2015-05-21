// 2009 ACM ICPC Greater NY Region
// Problem A
//
// Description:
//
// Read an unsorted array of distinct positive integers and print the
// third-largest element. The array will have exactly ten elements.
// All elements will be between 1 and 1000, inclusive.
//
// Input:
//
// The first line of input contains a single positive integer N which
// is the number of data sets that follow. Each data set should be
// processed identically and independently.
//
// The remaining N lines of input will each contain ten integers,
// space separated.
//
// Output:
//
// For each data set, print the data set number as a decimal integer
// (start counting at one), a space, and the third largest of the
// corresponding ten integers.
//
// Sample Input:
//
// 4
// 1 2 3 4 5 6 7 8 9 1000
// 338 304 619 95 343 496 489 116 98 127
// 931 240 986 894 826 640 965 833 136 138
// 940 955 364 188 133 254 501 122 768 408
//
// Sample Output:
//
// 1 8
// 2 489
// 3 931
// 4 768
//
//

#include <iostream>
#include <vector>

using namespace std;

int main(void)
   {
   // Read number of data sets to process
   int num, nprob;
   cin >> num;

   // Process each data set identically
   for (int i = 1; i <= num; ++i)
      {
	   cin >> nprob;
      // Array containing the three largest elements.
      // We want a[0] > a[1] > a[2]. Then after processing all
      // ten elements a[2] is the answer. We initialize the array
      // to all -1's because we know the inputs are all positive.
      vector<int> a(3, -1);
      for (int j = 0; j < 10; ++j)
         {
         int x;
         cin >> x;
         // Put x in its appropriate location in the array a.
         // Bubble it down to its right location.
         for (int k = 0; k < 3; ++k)
            {
            if (x > a[k])
               {
               const int temp = a[k];
               a[k] = x;
               x = temp;
               }
            }
         // Notice that x falls off the array a and is lost.
         }
      cout << nprob << " " << a[2] << endl;
      }

   // We're done
   return 0;
   }
