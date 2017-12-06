/*
 * Solution by Fabian Gundlach
 * On Windows, build with VS2015
 */
#include <cstdio>
#include <vector>
using namespace std;

#define REP(i,a,n) for (int i = (a); i < (n); i++)

const long long MOD = 1000000007;

vector<pair<int,int> > factorization(int n) {
	vector<pair<int,int> > res;
	for (int p = 2; p*p <= n; p++)
		if (n%p == 0) {
			int a = 0;
			while(n%p == 0) {
				a++;
				n /= p;
			}
			res.push_back(make_pair(p, a));
		}
	if (n > 1)
		res.push_back(make_pair(n, 1));
	return res;
}

vector<pair<int,int> > divisors_with_phi(int n) { // returns all pairs (d,phi(d)) for d | n
	vector<pair<int,int> > fact = factorization(n), res = { make_pair(1,1)};
	for (const pair<int,int> &pa : fact) {
		int p = pa.first, a = pa.second;
		vector<pair<int,int> > nres = res;
		for (pair<int,int> re : res) {
			re.second *= p-1;
			REP(i,0,a) {
				re.first *= p;
				nres.push_back(re);
				re.second *= p;
			}
		}
		swap(res, nres);
	}
	return res;
}

int powmod(long long a, long long n) {
	long long r = 1;
	for (int i = 0; (1ll<<i) <= n; i++, a = a*a%MOD)
		if (n&(1ll<<i))
			r = r*a%MOD;
	return r;
}

int gcd(int a, int b) {
	while(b) {
		a %= b;
		swap(a,b);
	}
	return a;
}

int main() {
	int P, kprob;
	scanf("%d", &P);
	for (int test = 0; test < P; test++) {
		int n, m;
		scanf("%d%d%d", &kprob, &n, &m);
//		fprintf(stderr, "gcd(n,m) = %d\n", gcd(n,m));
		vector<pair<int,int> > dipn = divisors_with_phi(n), dipm = divisors_with_phi(m);
		long long res = 0;
		for (const pair<int,int> &pn : dipn)
		for (const pair<int,int> &pm : dipm)
			res = (res+(long long)pn.second*pm.second%MOD*powmod(2, (long long)(n/pn.first)*(m/pm.first)*gcd(pn.first, pm.first))%MOD)%MOD;
		res = res*powmod(n, MOD-2)%MOD*powmod(m, MOD-2)%MOD; // MOD is prime, so x^(MOD-2) = x^(-1) mod MOD
		printf("%d %lld\n", kprob, res);
	}
	return 0;
}
