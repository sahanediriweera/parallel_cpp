#include <atomic>
#include <vector>
#include <cassert>
#include <chrono>
#include <thread>
#include <iostream>

int main()
{

    const int iterations = 1 << 25;

    const int num_of_threads = 8;

    const int items_per_thread = iterations / num_of_threads;

    std::atomic<int> sink = 0;

    auto work = [&]()
    {
        for (int i = 0; i < items_per_thread; i++)
        {

            int desired;

            int expected = sink.load();

            do
            {
                desired = expected + 1;
            } while (!sink.compare_exchange_strong(expected, desired));
        }
    };

    std::vector<std::jthread> threads;

    for (int i = 0; i < num_of_threads; i++)
    {
        threads.emplace_back(work);
    }

    return 0;
}