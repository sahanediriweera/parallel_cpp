#include <pthread.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

int main()
{

    const int num_iterations = 1 << 22;

    const int num_threads = 8;

    std::array<int, 8> max_wait_time;

    pthread_spinlock_t spinlock;
    pthread_spin_init(&spinlock, 0);

    auto work = [&](int tid)
    {
        int max = 0;

        for (int i = 0; i < num_iterations; i++)
        {
            auto start = std::chrono::system_clock::now();
            pthread_spin_lock(&spinlock);
            auto stop = std::chrono::system_clock::now();

            pthread_spin_unlock(&spinlock);

            int diff = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

            max = std::max(max, diff);
        }

        max_wait_time[tid] = max;
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; i++)
    {
        threads.emplace_back(work, i);
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    for (auto max : max_wait_time)
    {
        std::cout << max << " ";
    }

    std::cout << "\n";

    return 0;
}