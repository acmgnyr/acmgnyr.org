/* *************************************************************************
 * Route Redundancy Ratio
 * Greater NY Regional ACM Contest 2011
 * Solution by Adam Florence
 *
 * This program just *loves* the STL and aggravates Microsoft's C compiler
 * a little with "overlength" variable names *sigh*.  If it annoys you, turn
 * off warning 4786.
 */
#include <iostream>
#include <vector>
#include <set>

#ifndef WIN32
#define	_MIN	min
#endif

using namespace std;

// Edmonds-Karp: get path with the greatest bottleneck (minimum) capacity. 
// Can be found with modified Dijkstra's shortest path algorithm.
unsigned get_path(const unsigned s,
                  const unsigned t,
                  const vector< vector<unsigned> >& G,
                  const vector< vector<unsigned> >& cf,
                  vector<unsigned>& reverse_path)
{
	const unsigned n = cf.size();
	
	vector<int> parent(n, -1);
	
	// capacity of path to node. Source has infinite
	vector<unsigned> g(n, 0);
	
	set<unsigned> unvisited;
	for (unsigned i = 0; i < n; ++i)
		unvisited.insert(i);
	
	unsigned u = s;
	while (u != t)
	{
		unvisited.erase(u);
		
		for (unsigned i = 0; i < G[u].size(); ++i)
		{
			const unsigned v = G[u][i];
			if (unvisited.find(v) != unvisited.end())
            {
				const unsigned h = u == s
					? cf[u][v] // g[u] is infinite
					: _MIN( g[u], cf[u][v] );
				if (h > g[v])
				{
					g[v] = h;
					parent[v] = u;
				}
            }
		}
		
		unsigned m = 0;
		for (set<unsigned>::const_iterator it = unvisited.begin();
		it != unvisited.end(); ++it)
			if (g[*it] > m)
            {
				m = g[*it];
				u = *it;
            }
			if (m == 0)
				// No path from source to sink
				return 0;
	}
	
	reverse_path.resize(0);
	reverse_path.push_back(u);
	while (u != s)
	{
		reverse_path.push_back( parent[u] );
		u = parent[u];
	}
	return g[t];
}

void go(int ds)
{
	// ---------------
	// Read network description
	// ---------------
	
	// Read number of nodes and edges, source index, sink index
	unsigned d, n, e, s, t, i;
	cin >> d >> n >> e >> s >> t;
	
	// G[i] is a list of nodes that i connects to
	vector< vector<unsigned> > G(n);
	
	// c[i][j] is capacity from node i to node j
	// f[i][f] is the flow from node i to node j
	// cf = c - f
	vector< vector<unsigned> > cf(n);
	for (i = 0; i < n; ++i)
	{
		cf[i].resize(n);
		fill( cf[i].begin(), cf[i].end(), 0 );
	}
	
	// Read each edge and its capacity
	for (i = 0; i < e; ++i)
	{
		unsigned u, v, capacity;
		cin >> u >> v >> capacity;
		cf[u][v] = capacity;
		G[u].push_back(v);
		G[v].push_back(u);
	}
	
	// ---------------
	// Ford-Fulkerson max-flow
	// ---------------
	
	unsigned flow = 0;
	
	bool first = true;
	unsigned first_flow = 0;
	
	// Find any unsaturated path from source to sink
	vector<unsigned> reverse_path;
	while ( unsigned m = get_path(s, t, G, cf, reverse_path) )
	{
		flow += m;
		
		if (first)
		{
			// We know that in the first iteration, the path chosen has
			// the maximum flow among all paths from source to sink
			first = false;
			first_flow = m;
		}
		
		// reduce capacity along the path by m
		for (unsigned i = 0; i < reverse_path.size()-1; ++i)
		{
			const unsigned v = reverse_path[i];
			const unsigned u = reverse_path[i+1];
			cf[u][v] -= m;
			cf[v][u] += m;
		}
	}
	
	if(first_flow == 0){
		printf("%d Invalid ratio\n", ds);
	} else {
		printf("%d %.3f\n", ds, (double)flow/(double)first_flow);
	}
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
