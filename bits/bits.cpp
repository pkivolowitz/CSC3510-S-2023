#include <cassert>
#include <iostream>
#include <cinttypes>
#include <iomanip>

using namespace std;

template<class T>
bool IsBitSet(T v, int bitnum) {
    assert(bitnum < sizeof(T) * 8);
    return v & (1 << bitnum);
}

template<class T>
T ClearBit(T v, int bitnum) {
    assert(bitnum < sizeof(T) * 8);
    return v & ~(1 << bitnum);
}

template<class T>
T SetBitTo(T v, int bitnum, T b) {
    assert(bitnum < sizeof(T) * 8);
	assert(b == 0 || b == 1);
    return v & ~(1 << bitnum) | (b << bitnum);
}

template<class T>
T FlipBit(T v, int bitnum) {
    assert(bitnum < sizeof(T) * 8);
    return v ^ (1 << bitnum);
}

bool IsLittleEndian() {
	uint16_t pattern = 1;
	return *((char *) & pattern) == 0x01;
}

template<class T>
bool IsPowerOfTwo(T v) {
	return ((v & (v - 1)) == 0);
}

bool IsSameSign(int64_t a, int64_t b) {
	return (a ^ b) >= 0;
}

template<class T>
bool IsNegative(T v) {
	T sb = T(1 << (sizeof(T) * 8 - 1));
	return bool(v & sb);
}

int main() {
    cout << boolalpha;
    cout << dec << setw(5) << left << __LINE__ << " true:  " << IsBitSet<uint64_t>(1, 0) << endl;
    cout << dec << setw(5) << left << __LINE__ << " false: " << IsBitSet<uint64_t>(1, 1) << endl;
	cout << dec << setw(5) << left << __LINE__ << " fe:    " << hex << int(ClearBit<uint8_t>(0xFF, 0)) << endl;
	cout << dec << setw(5) << left << __LINE__ << " 7f:    " << hex << int(ClearBit<uint8_t>(0xFF, 7)) << endl;
	cout << dec << setw(5) << left << __LINE__ << " ff:    " << hex << int(SetBitTo<uint8_t>(0x7F, 7, 1)) << endl;
	cout << dec << setw(5) << left << __LINE__ << " ff:    " << hex << int(SetBitTo<uint8_t>(0xFF, 7, 0)) << endl;
	cout << dec << setw(5) << left << __LINE__ << " fe:    " << hex << int(FlipBit<uint8_t>(0xFF, 0)) << endl;
	cout << dec << setw(5) << left << __LINE__ << " true:  " << IsPowerOfTwo<uint32_t>(64) << endl;
	cout << dec << setw(5) << left << __LINE__ << " false: " << IsPowerOfTwo<uint32_t>(65) << endl;
	cout << dec << setw(5) << left << __LINE__ << " true:  " << IsSameSign(20, 30) << endl;
	cout << dec << setw(5) << left << __LINE__ << " false: " << IsSameSign(20, -3) << endl;
	cout << dec << setw(5) << left << __LINE__ << " true:  " << IsNegative<int32_t>(-1) << endl;
	cout << dec << setw(5) << left << __LINE__ << " false: " << IsNegative<int32_t>(20) << endl;
	cout << dec << setw(5) << left << __LINE__ << " true:  " << IsNegative<int8_t>(-1) << endl;
	cout << dec << setw(5) << left << __LINE__ << " false: " << IsNegative<int16_t>(20) << endl;

	cout << "Little Endian: " << IsLittleEndian() << endl;
    return 0;
}
