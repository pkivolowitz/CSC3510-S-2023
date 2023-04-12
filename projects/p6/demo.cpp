/*	Comparing 6 implementations of memcpy.
	Perry Kivolowitz
	Carthage College Computer Science
*/

#include <iostream>
#include <chrono>
#include <vector>
#include <utility>
#include <string>
#include <cstring>
#include <thread>
#include <cstdlib>
#include <iomanip>

using namespace std;
using namespace std::chrono;

/*	Another implementation might use simd intrinsics for multiple
	longs.

	Another implementation  might  be straight  assembly language
	using simd instructions.
*/

//	-----------

/*	naive implementation which suffers from unnecessary overhead.
	Each loop must deal with incrementing the loop counter, which
	does nothing but serve as a pointer offset.
*/

void memcpy_1(u_char * d, u_char * s, size_t size) {
	for (size_t i = 0; i < size; i++) {
		*(d++) = *(s++);
	}
}

/*	little improved implementation - here  the overhead of the
	loop counter has been eliminated.  The pointers themselves
	are used to terminate the loop - we can't get rid of them.
*/

void memcpy_2(u_char * d, u_char * s, size_t size) {
	u_char * e = s + size;
	while (s < e) {
		*(d++) = *(s++);
	}
}

/*	little more improvement - instead of moving one byte,
	why not move 8?
*/

void memcpy_3(u_char * d, u_char * s, size_t size) {
	int szl = sizeof(u_int64_t);
	u_int64_t * ls = (u_int64_t *) s;
	u_int64_t * ld = (u_int64_t *) d;
	u_int64_t * le = ls + (size / szl);

	while (ls < le) {
		*(ld++) = *(ls++);
	}
}

/*	a lot of improvement - unroll the loop 8 times -  note
	there are assumptions made here as this code will work
	only when size is a power of two / multiple of 64.
*/

void memcpy_4(u_char * d, u_char * s, size_t size) {
	int szll = sizeof(u_int64_t);
	u_int64_t * ls = (u_int64_t *) s;
	u_int64_t * ld = (u_int64_t *) d;
	u_int64_t * le = ls + (size / szll / 8);

	while (ls < le) {
		*(ld++) = *(ls++);
		*(ld++) = *(ls++);
		*(ld++) = *(ls++);
		*(ld++) = *(ls++);
		*(ld++) = *(ls++);
		*(ld++) = *(ls++);
		*(ld++) = *(ls++);
		*(ld++) = *(ls++);
	}
}

/*	The original memcpy.
*/

void memcpy_5(u_char * d, u_char * s, size_t size) {
	memcpy((void *) d, (void *) s, size);
}

/*	Two threads used to wrap memcpy_4.
*/

void memcpy_6(u_char * d, u_char * s, size_t size) {
	thread t1(memcpy_4, d, s, size / 2);
	thread t2(memcpy_4, d + size / 2, s + size / 2, size / 2);
	t1.join();
	t2.join();
}

typedef void (* MEMCPY_FUNC)(u_char *, u_char *, size_t);
typedef duration<double> DURATION;

struct MemcpyStruct {
	string label;
	MEMCPY_FUNC func;
	double total_time;

	MemcpyStruct(string l, MEMCPY_FUNC f, double d) : label(l), func(f), total_time(d) {}
};

const int ITER = 400;
const int SIZE = (1 << 24);

void Free(u_char * s, u_char * d) {
	if (s)
		free(s);
	if (d)
		free(d);
}

int main(int argc, char * argv []) {

	u_char * sbuffer = (u_char *) aligned_alloc(16, SIZE);
	u_char * dbuffer = (u_char *) aligned_alloc(16, SIZE);

	if (!sbuffer or !dbuffer) {
		cerr << "Failed to allocate one or more buffers" << endl;
		Free(sbuffer, dbuffer);
		return 1;
	}

	cout << "Timing 6 implementations of memcpy-like functions moving " << SIZE << " bytes " << ITER << " times\n";

	vector<MemcpyStruct> functions;
	functions.push_back(MemcpyStruct(string("V1"), memcpy_1, 0));
	functions.push_back(MemcpyStruct(string("V2"), memcpy_2, 0));
	functions.push_back(MemcpyStruct(string("V3"), memcpy_3, 0));
	functions.push_back(MemcpyStruct(string("V4"), memcpy_4, 0));
	functions.push_back(MemcpyStruct(string("V5"), memcpy_5, 0));
	functions.push_back(MemcpyStruct(string("V6"), memcpy_6, 0));

	/*	The reason I am interleaving the implementations is the hope
		that this will average out other activity on the CPU(s) even
		better than simply executing many iterations.
	*/

	for (int iteration = 0; iteration < ITER; iteration++) {
		// In the inner loop there is ONE statement that requires `f` to be a reference.
		// Which is it?
		for (auto & f : functions) {
			high_resolution_clock::time_point start_time, end_time;
			start_time = high_resolution_clock::now();
			(*f.func)(dbuffer, sbuffer, SIZE);
			end_time = high_resolution_clock::now();
			DURATION elapsed_time = duration_cast<DURATION>(end_time - start_time);
			f.total_time += elapsed_time.count();
		}
	}

	for (auto f : functions) {
		cout << f.label << ": " << fixed << setprecision(16) << setw(16) << f.total_time << " --- " << fixed << setprecision(16) << setw(16) << f.total_time / double(ITER) << endl;
	}

	return 0;
}
