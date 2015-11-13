/*
 * ICPC GNYR 2015
 * Problem by Fabian Gundlach
 * Alternate Solution
 * Solution: Yury Kholondyrev
 */
#include <iostream>
#include "string.h"

using namespace std;

int n, f, b, q[1024*1024];
int red[512][512], green[512][512], reds[512], greens[512];
bool good[512][512];

void process(int e[512][512], int s[512], int x, int y){
	for(int i = 0; i < s[x]; i++){
		int xx = e[x][i];
		for(int j = 0; j < s[y]; j++){
			int yy = e[y][j];
			if(!good[xx][yy]){
				good[xx][yy] = good[yy][xx] = true;
				q[b++] = xx; q[b++] = yy;
			}
		}
	}
}

int main(){

	int p; cin >> p;
	int dest;
	int i, j, K;
	bool allGood;
	int x, y;

	while(p-- && cin >> K >> n){
		memset(reds, 0, sizeof(reds));
		memset(greens, 0, sizeof(greens));

		for(i = 0; i < n && cin >> dest; i++){
			red[dest][reds[dest]++] = i;
		}
		for(i = 0; i < n && cin >> dest; i++){
			green[dest][greens[dest]++] = i;
		}

		memset(good, 0, sizeof(good));
		f = b = 0;
		
		for(i = 0; i < n; i++){
 			good[i][i] = true;
			q[b++] = i; q[b++] = i;
		}

		while(b > f){
			x = q[f++];
			y = q[f++];
			process(red, reds, x, y);
			process(green, greens, x, y);
		}

		allGood = true;
		for(i = 0; i < n; i++){
			for(j = 0; j < n; j++){
				allGood &= good[i][j];
			}
		}

		cout << K << " " << (allGood ? "YES" : "NO") << endl;
	}

	return 0;
}