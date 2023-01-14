// This program can compile on Mac, Windows and Linux.

#include <iostream>
#include <thread>
#include <vector>

using namespace std;

uint64_t counter = 0;

void worker(uint32_t thread_id, uint64_t max_value)
{
    //cout << "Thread: " << thread_id << " entering." << endl;
    for (uint64_t i = 0; i < max_value; i++)
        counter++;
    //cout << "Thread: " << thread_id << " exiting." << endl;
}

int main(int argc, char ** argv)
{
    uint64_t max_value = 100000;
    uint32_t max_threads = thread::hardware_concurrency();
    vector<thread *> threads(max_threads);

    // If a command line argument is present, use it to override max_value.
    if (argc > 1)
        max_value = (uint64_t) atol(argv[1]);

    // Launch all the threads.
    for (uint32_t i = 0; i < max_threads; i++)
        threads[i] = new thread(worker, i, max_value);

    // Embrace their death.
    for (uint32_t i = 0; i < max_threads; i++)
        threads[i]->join();
        
    // Emit results.
    cout << "All threads completed.\n";
    cout << "Expected: " << max_threads * max_value << ". Got: " << counter << endl;
    return 0;
}