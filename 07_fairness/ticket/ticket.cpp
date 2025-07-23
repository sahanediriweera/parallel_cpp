#include <emmintrin.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

class Spinlock
{

private:
    std::atomic<std::uint16_t> line{0};
    volatile std::uint16_t serving{0};

public:
    void lock()
    {

        auto place = line.fetch_add(1);

        while (serving != place)
            _mm_pause();
    }

    void unlock()
    {
        asm volatile("" : : : "memory");
        serving = serving + 1;
    }
};

int main()
{

    const int num_iterations = 1 << 22;

    const int num_threads = 8;

    std::array<int, 8> max_wait_time;

    Spinlock spinlock;
    auto work = [&](int tid)
    {
        int max = 0;

        for (int i = 0; i < num_iterations; i++)
        {
            auto start = std::chrono::system_clock::now();

            spinlock.lock();

            auto stop = std::chrono::system_clock::now();
            spinlock.unlock();

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
        std::cout << max << ' ';
    }

    std::cout << '\n';

    return 0;
}