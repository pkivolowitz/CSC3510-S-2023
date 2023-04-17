#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory.h>
#include <sys/time.h>
#include <string>
#include <arm_neon.h>

using std::cout;
using std::string;

const int ARRAY_SIZE = 1 << 24;
const int LOOPS = 1 << 5;

/* Test1() timing a sequential march through a large array.
*/
timeval Test1() {

	timeval start, end, elapsed;
     unsigned short * buffer;
    buffer = (unsigned short *) malloc(ARRAY_SIZE * sizeof(short));
    assert(buffer);
	gettimeofday(&start, NULL);
	for (int i = 0; i < LOOPS; i++) {
         unsigned short accumulator;
         unsigned short * end = buffer + ARRAY_SIZE;
         unsigned short * ptr = buffer;
        while (ptr < end) {
			 int waster = rand() % ARRAY_SIZE;
			accumulator += *(ptr++);
            __builtin_prefetch((const void *) (ptr + 16), 0, 1);
            // prfm
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
	volatile unsigned short *buffer;
	buffer = (unsigned short *)malloc(ARRAY_SIZE * sizeof(short));
	assert(buffer);
	gettimeofday(&start, NULL);
	for (int i = 0; i < LOOPS; i++) {
		volatile unsigned short accumulator;
		volatile unsigned short *end = buffer + ARRAY_SIZE;
		volatile unsigned short *ptr = buffer;
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


/*
    volatile char * keyboard = an_address;
    for (i = 0; i < 1000; i++) {
        c = *keyboard;
    }
*/