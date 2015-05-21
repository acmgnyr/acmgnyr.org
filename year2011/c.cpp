/*
 * E - Programming the EDSAC
 * Greater NY Regional ACM Contest 2011
 * Solution by Lee Wittenberg
 */
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
using namespace std;

#define ADJUST  10000000000000000LL
#define COLLATING	"PQWERTYUIOJ#SZK*?F@D!HNM&LXGABCV"
string e_convert(string);
string e_instruction(long);


long long pad(long long, int);

string e_convert(string d)
{
	long bit_pattern = 0;
	bool is_negative = (d[0] == '-');;
	const char *s = d.c_str();
	if (is_negative) { s++; }	// skip the sign char
	int prefix = atoi(s);
	long long suffix = 0;
#ifndef WIN32
	sscanf(strchr(s, '.')+1, "%Ld", &(suffix));
#else
	sscanf(strchr(s, '.')+1, "%I64d", &(suffix));
#endif
	suffix = pad(suffix, 18 - strlen(s));;
	if (is_negative && prefix == 1 && suffix == 0) {
		return e_instruction(0x10000);
	} else if (prefix > 0) {
		return "INVALID VALUE";
	};
	for (int i = 0; i < 16; i++) {
		suffix *= 2;
		bit_pattern <<= 1;
		if (suffix >= ADJUST) {
			suffix -= ADJUST;
			bit_pattern += 1;
		}
	};
	if (is_negative) { bit_pattern = -bit_pattern; }
	return e_instruction(bit_pattern & 0x1ffff);
}
string e_instruction(long e)
{
	stringstream result;
	char prefix = COLLATING[(e >> 12) & 0x1f];
	char suffix = ((e & 1) == 0) ? 'F' : 'D';
	result << prefix << " " << ((e>>1) & 0x7ff)
		<< " " << suffix;
	return result.str();
}
long long pad(long long n, int d)
{
	for (int i = 0; i < d; i++) {
		n *= 10;
	}
	return n;
}

int main()
{
	int p;
	cin >> p;
	for (int i = 0; i < p; i++) {
		int n; string d;
		cin >> n;
		cin >> d;
		cout << n << " " << e_convert(d).c_str() << endl;;
	}
	return 0;
}
