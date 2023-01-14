// Can be compiled on the Mac, Windows and Linux.
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

void worker(uint32_t thread_id, uint64_t max_value, uint64_t * counter)
{
    for (uint64_t i = 0; i < max_value; i++)
    {
        (*counter)++;
    }
}

int main(int argc, char ** argv)
{
    uint64_t max_value = 100000;
    uint32_t max_threads = thread::hardware_concurrency();
    vector<thread *> threads(max_threads);
    vector<uint64_t> counters(max_threads);

    // If a command line argument is present, use it to override max_value.
    if (argc > 1)
        max_value = (uint64_t) atol(argv[1]);

    // Launch all the threads.
    for (uint32_t i = 0; i < max_threads; i++)
        threads[i] = new thread(worker, i, max_value, &counters[i]);

    // Embrace their death and perform reduction manually.
    uint64_t total = 0;
    for (uint32_t i = 0; i < max_threads; i++)
    {
        threads[i]->join();
        total += counters[i];
    }
    // Emit results.
    cout << "All threads completed.\n";
    cout << "Expected: " << max_threads * max_value << ". Got: " << total << endl;
    return 0;
}