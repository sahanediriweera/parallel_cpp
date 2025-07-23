#include <atomic>
#include <cassert>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>

int main()
{

    const int iterations = 1 << 15;
    const int num_threads = 8;

    volatile int sink = 0;

    std::mutex m;

    auto work = [&]()
    {
        while (true)
        {
            std::lock_guard<std::mutex> lg(m);
            if (sink == iterations)
                break;
            sink = sink + 1;
        }
    };

    auto slow_work = [&]()
    {
        while (true)
        {
            std::lock_guard<std::mutex> lg(m);
            if (sink == iterations)
                break;
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            sink = sink + 1;
        }
    };

    std::vector<std::jthread> threads;

    for (int i = 0; i < num_threads; i++)
    {
        threads.emplace_back(work);
    }

    threads.emplace_back(slow_work);

    for (auto &thread : threads)
        thread.join();

    std::cout << "Completed " << sink << " iteration\n";
    return 0;
}