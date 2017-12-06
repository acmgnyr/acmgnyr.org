#include <iostream>

using namespace std;

int main()
{
	int p; cin >> p;
	for (int i = 1; i <= p; i++) {
		int k, n; cin >> k >> n;
		cout << k << " ";
		cout << n + n * (n + 1) / 2 << endl;
	}
}
