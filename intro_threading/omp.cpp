#include <iostream>
#include <thread>
#include <vector>
#include <omp.h>

using namespace std;

uint64_t counter = 0;

int main(int argc, char ** argv)
{
    uint64_t max_value = 100000;
    uint64_t counter = 0;
    int32_t max_threads = omp_get_max_threads();

    if (argc > 1)
        max_value = (uint64_t)atol(argv[1]);

    #pragma omp parallel for reduction(+ : counter)
    for (int32_t i = 0; i < max_threads; i++)
    {
        for (uint64_t j = 0; j < max_value; j++)
        {
            counter++;
        }
    }

    cout << "All threads completed.\n";
    cout << "Expected: " << max_threads * max_value << ". Got: " << counter << endl;
    return 0;
}