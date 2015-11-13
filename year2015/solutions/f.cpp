/*
 * F - Robots
 * ICPC GNYR 2015
 * Problem by Fabian Gundlach
 * Solution: Fabian Gundlach
 * Modified by: John Buck (any mistakes are mine).
 */
#include <cstdio>
#include <cassert>
#include <vector>
#include <queue>
using namespace std;

const int MAXN = 600;

int N;
int r[MAXN];
int g[MAXN];
vector<int> revr[MAXN]; // revr[k] = {a | r_a = k}
vector<int> revg[MAXN]; // revg[k] = {a | g_a = k}
bool found[MAXN][MAXN];

int main(int argc, char **argv)
{
	int P, K, i, a, b;
	bool allfound;
	vector<int>::iterator ia, ib;

	scanf("%d", &(P));
	for (int test = 0; test < P; test++) {
		scanf("%d %d", &(K), &(N));
		for (i = 0; i < N; i++) {
			scanf("%d", &(r[i]));
			assert(0 <= r[i] && r[i] < N && r[i] != i);
		}
		for (i = 0; i < N; i++) {
			scanf("%d", &(g[i]));
			assert(0 <= g[i] && g[i] < N && g[i] != i);
		}
		for (i = 0; i < N; i++) {
			revr[i].clear();
			revg[i].clear();
		}
		for (i = 0; i < N; i++) {
			revr[r[i]].push_back(i);
			revg[g[i]].push_back(i);
		}
		for (a = 0; a < N; a++){
			for (int b = 0; b < N; b++){
				found[a][b] = false;
			}
		}
		// Backwards BFS in the constructed graph, starting from vertices of the form (i,i).
		// A simple DFS should also work if the stack limit is not too low.
		queue<pair<int,int> > qu;
		for (i = 0; i < N; i++) {
			found[i][i] = true;
			qu.push(make_pair(i,i));
		}
		while(!qu.empty()) {
			int c = qu.front().first, d = qu.front().second;
			qu.pop();
			for (ia = revr[c].begin(); ia != revr[c].end(); ++ia){
				for (ib = revr[d].begin(); ib != revr[d].end(); ++ib){
					if (!found[*ia][*ib]) {
						found[*ia][*ib] = true;
						qu.push(make_pair(*ia, *ib));
					}
				}
			}
			for (ia = revg[c].begin(); ia != revg[c].end(); ++ia){
				for (ib = revg[d].begin(); ib != revg[d].end(); ++ib){
					if (!found[*ia][*ib]) {
						found[*ia][*ib] = true;
						qu.push(make_pair(*ia, *ib));
					}
				}
			}
		}
		allfound = true;
		for (a = 0; a < N; a++){
			for (b = 0; b < N; b++){
				if (!found[a][b]){
					allfound = false;
					break;
				}
			}
			if(!allfound){
				break;
			}
		}
		printf("%d %s\n", K, allfound ? "YES" : "NO");
	}
	return 0;
}
