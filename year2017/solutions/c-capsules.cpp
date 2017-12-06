#ifdef _MSC_VER
#include <ciso646>
#endif

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <queue>

using namespace std;

struct Square {
	char val;
	int row, col;
	set<char> *avail;
	Square() : val('#') {}
	bool operator < (const Square& other) const
	{ return avail->size() > other.avail->size(); }
};

ostream& operator << (ostream& os, const Square& s)
{
	os << s.val;
	return os;
}

struct Solution {
	vector< vector<Square> > grid;
	priority_queue<Square *> to_do;
};

ostream& operator << (ostream& os, const Solution& s)
{
	for (int i = 1; i < s.grid.size() - 1; i++) {
		for (int j = 1; j < s.grid[i].size() - 1; j++) {
			if (j != 1) { os << " "; }
			os << s.grid[i][j];
		}
		os << endl;
	}
	return os;
}

bool adjacent_okay(const Solution& s, char val, int r, int c)
{
	for (int dr = -1; dr <= 1; dr++) {
		for (int dc = -1; dc <= 1; dc++) {
			if (s.grid[r+dr][c+dc].val == val) {
				return false;
			}
		}
	}
	return true;
}

bool attempt(Solution& s)
{
	if (s.to_do.empty()) { return true; }
	Square *curr = s.to_do.top(); s.to_do.pop();
	int row = curr->row, col = curr->col;
	set<char> avail = *curr->avail;
	
	for (auto p = avail.begin(); p != avail.end(); p++) {
		if (adjacent_okay(s, *p, row, col)) {
			curr->avail->erase(*p);
			s.grid[row][col].val
				= *p;
			if (attempt(s)) { return true; }
			s.grid[row][col].val = '-';
			curr->avail->insert(*p);
		}
	}
	s.to_do.push(curr);
	return false;
}

void init_solution(Solution& s)
{
	int r, c; cin >> r >> c;
	s.grid = vector< vector< Square> >(r+2, vector<Square>(c+2));
	string v;
	for (int i = 1; i <= r; i++ ) {
		for (int j = 1; j <= c; j++) {
			cin >> v;
			s.grid[i][j].val = v[0];
			s.grid[i][j].row = i;
			s.grid[i][j].col = j;
		}
	}
	int nblocks; cin >> nblocks;
	for (int i = 0; i < nblocks; i++) {
		set<char> *blk = new set<char>();
		int nsquares; cin >> nsquares;
		for (int j = 1; j <= nsquares; j++) { blk->insert('0' + j); }
		for (int j = 1; j <= nsquares; j++) {
			cin >> v;
			istringstream vs(v);
			int r, c; char dummy;
			vs >> dummy >> r >> dummy >> c;
			blk->erase(s.grid[r][c].val);
			s.grid[r][c].avail = blk;
			if (s.grid[r][c].val == '-') { s.to_do.push(&s.grid[r][c]); }
		}
	}
}

int main()
{
	int p; cin >> p;
	for (int i = 1; i <= p; i++) {
		int k; cin >> k;
		cout << k << endl;
		Solution soln;
		init_solution(soln);
		if (attempt(soln)) { cout << soln; }
		else { cout << "No solution." << endl; }
	}
}
