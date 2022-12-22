#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <ctime>
#include <getopt.h>

using namespace std;

uint64_t counter = 0;
mutex m;

void worker(uint32_t thread_id, uint64_t max_value, uint64_t max_random_value)
{
    for (uint64_t i = 0; i < max_value; i++)
    {
        m.lock();
        counter++;
        cout << thread_id << "\t" << counter << endl;
        if (rand() % max_random_value)
        {
            m.unlock();
        }
    }
}

int main(int argc, char ** argv)
{
    uint64_t max_value = 100000;
    uint64_t max_random_value = 100000;
    int32_t c;

    uint32_t max_threads = thread::hardware_concurrency();
    vector<thread *> threads(max_threads);

    srand((unsigned int) time(nullptr));
    
    while ((c = getopt(argc, argv, "v:r:h")) != -1)
    {
        switch (c)
        {
            default:
            case 'h':
                cout << "usage: prog [-v max_value] [-r max_random_value]\n";
                return 0;

            case 'r':
                max_random_value = atol(optarg);
                break;

            case 'v':
                max_value = atol(optarg);
                break;
        }
    }
    
    // Launch all the threads.
    for (uint32_t i = 0; i < max_threads; i++)
        threads[i] = new thread(worker, i, max_value, max_random_value);

    // Embrace their death.
    for (uint32_t i = 0; i < max_threads; i++)
        threads[i]->join();

    // Emit results.
    cout << "All threads completed.\n";
    cout << "Expected: " << max_threads * max_value << ". Got: " << counter << endl;
    return 0;
}