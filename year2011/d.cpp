#include <iostream>
#include <cstring>
using namespace std;

#define COLLATING	"PQWERTYUIOJ#SZK*?F@D!HNM&LXGABCV"
string e_convert(char, unsigned, char);
string e_decimal(int);
string e_convert(char prefix, unsigned address, char suffix)
{
	int e = ((strchr(COLLATING, prefix) - COLLATING) << 12)
		+ (address << 1) + (suffix == 'D' ? 1 : 0);;
	return e_decimal(e);
}
string e_decimal(int e)
{
	if (e == 0) { return "0.0"; }
	if (e == 0x10000) { return "-1.0"; };
	string result = "";
	if ((e & 0x10000) != 0) {   // negative #
		result = "-";
		e = -e & 0xffff;
	};
	result += "0.";
	while (e != 0) {
		e *= 10;
		result += '0' + (e / 0x10000);
		e = e & 0xffff;
	};
	return result;
}

int main()
{
	int p;
	cin >> p;
	for (int i = 0; i < p; i++) {
		int n; char c, s; unsigned d;
		cin >> n;
		cin >> c >> d >> s;
		cin.ignore(100, '\n');	// skip rest of input line
		cout << n << " " << e_convert(c, d, s).c_str() << endl;;
	}
	return 0;
}
