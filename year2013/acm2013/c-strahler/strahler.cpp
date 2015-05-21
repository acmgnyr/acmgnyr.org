/*
 * C - Strahler
 * ICPC 2013 Greater NY Regional
 * Solution by Adam Florence
 * Problem by Adam Florence
 */

//
// Description:
//
// In geology, a river system can be represented as a directed graph.
// Each river segment is an edge, with the edge pointing the same way
// the water flows. Nodes are either the source of a river segment (for
// example, a lake or spring), where river segments merge or diverge,
// or the mouth of the river.
//
// The Strahler order of a river system is computed as follows.
//
// * The order of each source node is 1.
// * For every other node, let i be the highest order of all its upstream
//   nodes. If just one upstream node has order i, then this node also
//   has order i. If two or more upstream nodes have order i, then this
//   node has order i+1.
//
// The order of the entire river system is the order of the mouth node.
// In this problem, the river system will have just one mouth.
//
// You must write a program to determine the order of a given river system.
//
// The actual river with the highest order is the Amazon, with order 12.
// The highest in the U.S. is the Mississippi, with order 10.
//
// Input:
//
// The first line of input contains a single positive integer N which
// is the number of data sets that follow. Each data set should be
// processed identically and independently.
//
// Each data set consists of:
// * a line containing two positive integers, M and P. M is the number
//   of nodes in the graph, P is the number of edges.
// * P lines, each describing an edge in the graph. The line will
//   contain two positive integers, A and B, indicating that water
//   flows from node A to node B. Note 1 <= A, B <= M.
//
// Node M is the mouth of the river. It has no outgoing edges.
//
// Output:
//
// For each data set, print the data set number, a space, and the order
// of the river system.
//
//
//
// Note: compiled with 'g++ -o strahler.exe strahler.cc' using g++ ver 4.4.0.
//

#include <iostream>
#include <vector>

using namespace std;

// Read a river system and return its order
int go(void)
   {
   // Read number of nodes, M, and number of edges, P
   int i, j, k, m, p;
   cin >> k >> m >> p;

   // Represent the river as an array of arrays. r[i] is an array
   // of the nodes that flow into node i. We won't use r[0].
   vector< vector<int> > r(m+1);

   // Read edges
   for (j = 1; j <= p; ++j)
      {
      int a, b;
      cin >> a >> b;
      // Water flows from a to b
      r[b].push_back(a);
      }

   // Order of each node. Initialize all to 0 indicating we don't know yet.
   vector<int> order(m+1, 0);

   // If r[i] is empty, then it's a source node and its order is 1.
   for (i = 1; i <= m; ++i)
      {
	   if ( r[i].size() == 0 ){
         order[i] = 1;
	   }
      }

   // If r[i] is not empty, and we know the order of all the immediately
   // upstream nodes, we can compute the order of i. Stop when we know
   // the order of node m.
   while ( order[m] == 0 )
      {
      for (i = 1; i <= m; ++i)
         {
         if ( order[i] == 0 && r[i].size() > 0 )
            {
            bool seen_zero = false;
            int max_order = 0; // maximum order of upstream nodes
            int num_max = 0;   // number of nodes with that order
            for (j = 0; j < r[i].size(); ++j)
               {
               int node = r[i][j];
               if ( order[node] == 0 )
                  seen_zero = true;
               if ( order[node] == max_order )
                  ++num_max;
               if ( order[node] > max_order )
                  {
                  max_order = order[node];
                  num_max = 1;
                  }
               }
            if (!seen_zero)
               {
               if (num_max >= 2)
                  order[i] = max_order+1;
               else
                  order[i] = max_order;
               }
            }
         // Done with node i
         }
      }

   return order[m];
   }

int main(void)
   {
   // Read number of data sets to process
   int num;
   cin >> num;

   // Process each data set identically
   for (int i = 1; i <= num; ++i)
      {
      int order = go();
      cout << i << " " << order << endl;
      }

   // We're done
   return 0;
   }
