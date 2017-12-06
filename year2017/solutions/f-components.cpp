#include<iostream>
#include <vector>
#include <set>

using namespace std;
#ifdef WIN32
typedef __int64		ll;
#else
typedef long long ll;
#endif
typedef vector<int> vi;
typedef pair<int,int> pii;
typedef pair<double, double> pdd;
#define pb push_back
#define mp make_pair
#define fs first
#define sc second
#define rep(i, from, to) for (int i = from; i < (to); ++i)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
#define FOR(i, to) for (i = 0; i < (to); ++i)
typedef vector<vector<int> > vvi;
typedef vector<ll> vll;
typedef vector<vll> vvll;
typedef vector<pair<int, int> > vpi;
typedef pair<ll,ll> pll;
typedef vector<string> vs;
#ifndef max
#define	max(x, y)	((x) > (y) ? (x) : (y))
#endif

#define Nmax 2020
#define LM 1000

//input
string sx[Nmax+10];
char szxBuf[Nmax+10];
int N,M;

//0 components, 1 components and touching components
int lnum0[Nmax], lnum1[Nmax], lnumt[Nmax];
int rnum0[Nmax], rnum1[Nmax], rnumt[Nmax];
int par[Nmax*Nmax + 200],h[Nmax*Nmax + 200];

//Clear between tests
void clearx() {
	int i;
	for(i=0;i<max(N,M)+100;++i) {
		lnum0[i]=lnum1[i]=lnumt[i]=rnum0[i]=rnum1[i]=rnumt[i]=0;
		h[i]=par[i]=1;
		sx[i]="";
	}
}

//Code for union find

int findx(int x) {
	int R = x, y;
	while(par[R] != R) R = par[R];
	while(par[x] != x) {
		y = par[x];
		par[x] = R;
		x = y;
	}
	return R;
}
int unite(int x, int y) {
	x = findx(x); y = findx(y);
	if (x == y) return 0;
	if(h[x] > h[y]) {
		par[y] = x; h[x] += h[y];
	} else {
		par[x] = y; h[y] += h[x];
	}
	return 1;
}

int getNum(int x,int y) {
	return x * M + y;
}

void clear() {
	int i;
	FOR(i, N * M) {
		par[i] = i;
		h[i] = 1;
	}
}
int nr0,nr1;
void add(int x,int y, int sign) {
	if(sx[x][y] == '0') {
		nr0 += sign;
	} else nr1 += sign;
}

int make(int j) {
	int i;
	set<int> s;
	FOR(i,N) {
		if(sx[i][j] == '1')
			s.insert(findx(getNum(i,j)));
	}
	return sz(s);
}

//process from left
void makel() {
	int i, j;
	clear();
	nr0=nr1=0;
	FOR(j,M) {
		FOR(i,N) {
			add(i,j,1);
			if(i-1 >= 0 && sx[i][j] == sx[i-1][j]) {
				add(i,j,-unite(getNum(i,j),getNum(i-1,j)));
			}
			if(j-1 >= 0 && sx[i][j] == sx[i][j-1]) {
				add(i,j,-unite(getNum(i,j),getNum(i,j-1)));
			}
		}
		if(j+1 < M) {
			lnum0[j+1] = nr0;
			lnum1[j+1] = nr1;
			lnumt[j+1] = make(j);
		} 
		
	}
}

//process from right
void maker() {
	int i;
	clear();
	nr0=nr1=0;
	for(int j=M-1;j>=0;--j) {
		FOR(i,N) {
			add(i,j,1);
			if(i-1 >= 0 && sx[i][j] == sx[i-1][j]) {
				add(i,j,-unite(getNum(i,j),getNum(i-1,j)));
			}
			if(j+1 < M && sx[i][j] == sx[i][j+1]) {
				add(i,j,-unite(getNum(i,j),getNum(i,j+1)));  
			}
		}
		if(j-1 >= 0) {
			rnum0[j-1] = nr0;
			rnum1[j-1] = nr1;
			rnumt[j-1] = make(j);
		}
		
	}
}

//get the pair of numbers of components
pii calc(int j) {
	return mp(lnum0[j]+rnum0[j], 1 + lnum1[j] + rnum1[j] - lnumt[j] - rnumt[j]);
}

int main() {
	int T, id, i;
	cin >> T;
    while(T--) {
		cin >> id >> N >> M;
		clearx();
		FOR(i,N){
			cin >> szxBuf;
			sx[i] = &(szxBuf[0]);
		}
		makel();
		maker();
		int ret = 0;
		int nr = 0;
		pii retv;
		FOR(i,M) {
			pii a = calc(i);
			if(a.fs + a.sc > ret) {
				ret = a.fs + a.sc;
				retv = a;
			} else if(a.fs + a.sc == ret && retv < a) {
				retv = a;
			}
		}
		cout << id << " " << retv.fs << " " << retv.sc << endl;
	}
	return(0);
}

