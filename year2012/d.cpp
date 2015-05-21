/*
 * D - Maximum Random Walk solution
 * ICPC 2012 Greater NY Regional
 * Solution by Adam Florence
 * Problem by Adam Florence
 */

#include <cstdio> // for printf
#include <cstdlib> // for exit
#include <algorithm> // for max
#include <iostream>
#include <vector>

using namespace std;

class State
{
public:
   State(int n) : _n(n), _p(2*n+1)
      {
      if (n < 1)
         {
         cout << "Ctor error, n = " << n << endl;
         exit(1);
         }
      for (int i = -n; i <= n; ++i)
         _p.at(i+_n) = vector<double>(n+1, 0.0);
      }

   void zero(const int n)
      {
      for (int i = -n; i < n; ++i)
         for (int m = 0; m <= n; ++m)
            _p[i+_n][m] = 0;
      }

   double operator()(int i, int m) const
      {
#ifdef DEBUG
      if ((i < -_n) || (i > _n))
         {
         cout << "Out of range error, i = " << i << ", n = " << _n << endl;
         exit(1);
         }
      if ((m < 0) || (m > _n))
         {
         cout << "Out of range error, m = " << m << ", n = " << _n << endl;
         exit(1);
         }
#endif
      return _p[i+_n][m];
      }

   double& operator()(int i, int m)
      {
#ifdef DEBUG
      if ((i < -_n) || (i > _n))
         {
         cout << "Out of range error, i = " << i << ", n = " << _n << endl;
         exit(1);
         }
      if ((m < 0) || (m > _n))
         {
         cout << "Out of range error, m = " << m << ", n = " << _n << endl;
         exit(1);
         }
#endif
      return _p[i+_n][m];
      }

   static int min(int x, int y)
   {
	   return(x < y ? x : y);
   }
   static int max(int x, int y)
   {
	   return(x > y ? x : y);
   }

private:
   int _n;

   // First index is the current position, from -n to n.
   // Second index is the maximum position so far, from 0 to n.
   // Value is probability.
   vector< vector<double> > _p;
};

void go(int ds)
   {
   // Read n, l, r
   int n, nds;
   double l, r;
   cin >> nds >> n >> l >> r;
   const double c = 1 - l - r;

   if(nds != ds){
	   cout << "Dataset number " << nds << " does not match " << ds << endl;
	   return;
   }

   // Initialize state, probability 1 at (0,0)
   State s(n);
   s(0,0) = 1;

   State t(n);

   State* p1 = &s;
   State* p2 = &t;

   for (int k = 1; k <= n; ++k)
      {
      // Compute probabilities at step k

      p2->zero(k);

      // At step k, the farthest from the origin you can be is k
      for (int i = -k; i <= k; ++i)
         {
		  const int mm = State::min( State::max(0, i+k), k);
         for (int m = 0; m <= mm; ++m)
            {
            // At step k-1, p = probability of (i,m)
            const double p = p1->operator()(i,m);
            if (p > 0)
               {
               // Step left
               p2->operator()(i-1, m) += p*l;
               // Step right
               p2->operator()(i+1, State::max(i+1,m)) += p*r;
               // Stay put
               p2->operator()(i, m) += p*c;
               }
            }
         }
      swap(p1, p2);
      }

   // Compute expected maximum position
   double p = 0;
   for (int i = -n; i <= n; ++i)
      for (int m = 0; m <= n; ++m)
         p += m * p1->operator()(i,m);

   printf("%d %0.4f\n", ds, p);
   }

int main(int argc, char* argv[])
   {
   // Read number of data sets to process
   int num;
   cin >> num;

   // Process each data set identically
   for (int i = 1; i <= num; ++i)
      go(i);

   // We're done
   return 0;
   }
