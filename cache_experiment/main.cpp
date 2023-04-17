#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory.h>
#include <sys/time.h>
#include <string>

using std::cout;
using std::string;

const int ARRAY_SIZE = 1 << 24;
const int LOOPS = 1 << 5;

/* Test1() timing a sequential march through a large array.
*/
timeval Test1() {
	timeval start, end, elapsed;
    volatile unsigned long * buffer;
    buffer = (unsigned long *) malloc(ARRAY_SIZE * sizeof(long));
    assert(buffer);
	gettimeofday(&start, NULL);
	for (int i = 0; i < LOOPS; i++) {
        volatile unsigned long accumulator;
        volatile unsigned long * end = buffer + ARRAY_SIZE;
        volatile unsigned long * ptr = buffer;
        while (ptr < end) {
			volatile int waster = rand() % ARRAY_SIZE;
			accumulator += *(ptr++);
		}
	}
	gettimeofday(&end, NULL);
	timersub(&end, &start, &elapsed);
    free((void *) buffer);
    return elapsed;
}

/* Test2() timing a random march through a large array.
 */
timeval Test2() {
	timeval start, end, elapsed;
	volatile unsigned long *buffer;
	buffer = (unsigned long *)malloc(ARRAY_SIZE * sizeof(long));
	assert(buffer);
	gettimeofday(&start, NULL);
	for (int i = 0; i < LOOPS; i++) {
		volatile unsigned long accumulator;
		volatile unsigned long *end = buffer + ARRAY_SIZE;
		volatile unsigned long *ptr = buffer;
		while (ptr < end) {
            volatile int index = rand() % ARRAY_SIZE;
			accumulator += *(buffer + index);
            ptr++;
		}
	}
	gettimeofday(&end, NULL);
	timersub(&end, &start, &elapsed);
	free((void *)buffer);
	return elapsed;
}

void PrintElapsedTime(string s, timeval &et) {
	cout << s << " ";
    cout << et.tv_sec + (double) et.tv_usec / (double) 1000000;
    cout << " seconds.\n";
}

int main() {
    timeval et;
    srand((unsigned int) time(nullptr));

    et = Test1();
    PrintElapsedTime("Sequential access:", et);
	et = Test2();
	PrintElapsedTime("Random access:", et);
	return 0;
}