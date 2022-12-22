/*	IEEE-754 Single Precision Explorer
	Perry Kivolowitz
	Carthage College
*/

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cmath>

using namespace std;

const int FRAC_SIZE = 23;
const int EXPO_SIZE = 8;
const int SIGN_SIZE = 1;

/*	This is an example of a C / C++ bitfield. While each member of this struct
	are typed unsigned int, the colon followed by a number says that the field
	is that many bits wide.
*/

struct SP {
	unsigned int frac : FRAC_SIZE;
	unsigned int expo : EXPO_SIZE;
	unsigned int sign : SIGN_SIZE;
};

/*	This is an example of a C / C++ union - each of the members of the union
	occupy the same memory.  Thus, you can look at the  underlying memory in
	different ways.
*/

union U {
	float f;
	SP sp;
};

/*	Given the fractional part of a single precision float, this
	function accrues the decimal version of the binary fraction
	by interrogating the fraction bit by bit (literally).
*/

float Debinary(unsigned int frac) {
	float f = 0.0f;

	for (int i = 0; i < FRAC_SIZE; i++) {
		if (frac & (1 << (FRAC_SIZE - 1 - i))) {
			f += 1.0f / float(1 << (i + 1));
		}
	}
	return f;
}

int main(int argc, char * argv[]) {
	U u;
	float f;
	float sign = 1.0f;
	int debiased_exp;
	const int BIAS = 127;
	const int PREC = 12;
	
	for (int i = 1; i < argc; i++) {
		u.f = atof((const char *) argv[i]);
		sign *= (u.sp.sign ? -1.0f : 1.0f);
		f = Debinary(u.sp.frac);
		debiased_exp = u.sp.expo - BIAS;

		cout << "float:  " << setprecision(PREC) << u.f << endl;
		cout << "sign:   " << u.sp.sign << endl;
		cout << "exp:    0x" << hex << left << setw(6) << u.sp.expo;
		cout << " debiased: " << dec << debiased_exp << endl;
		cout << "frac:   0x" << hex << setfill('0') << setw(6) << u.sp.frac;
		cout << " decimal:  " << dec << f << endl;
		cout << setfill(' ') << endl;
		cout << (u.sp.sign ? "-" : "") << 1.0 + f << " x 2^" << debiased_exp << " == ";
		cout << setprecision(PREC) << sign * (1.0 + f) * pow(2.0, double(debiased_exp)) << endl;
		cout << endl;
	}
	return 0;
}