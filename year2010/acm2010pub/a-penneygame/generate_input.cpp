/* *************************************************************************
Input generator for Penney's Sequences problem.


Compiled with GCC 4.4.0.
************************************************************************* */

#include <iostream>
#include <cstdlib>  // rand
#include <ctime>

using namespace std;

int main(int argc, char* argv[])
   {
   // Command line processing
   const int num_args = argc - 1;
   if (num_args < 1 || num_args > 2)
      {
      cerr << endl << "Usage: generate_input <num> [seed]" << endl << endl;
      return 1;
      }

   const unsigned num = atoi( argv[1] );

   // Seed random number generator
   if (num_args >= 2)
      {
      int seed = atoi( argv[2] );
      srand(seed);
      }
   else
      srand( time(NULL) );

   cout << num << endl;
   for (unsigned i = 0; i < num; ++i)
      {
      string seq;
      for (unsigned j = 0; j < 40; ++j)
         seq.push_back( rand() % 2 ? 'H' : 'T' );
      cout << seq << endl;
      }

   // We're done
   return 0;
   }
