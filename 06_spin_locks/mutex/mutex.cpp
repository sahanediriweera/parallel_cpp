#include <array>
#include <atomic>
#include <thread>
#include <vector>

int main()
{
    const int num_iterations = 1 << 27;

    const int num_threads = 4;

    std::array<std::atomic<int>, 4> counters = {0, 0, 0, 0};
    std::atomic<int> final_sum = 0;

    const int elements_per_thread = num_iterations / num_threads;

    auto work = [&](int thread_id)
    {
        for (int i = 0; i < elements_per_thread; i++)
        {
            counters[thread_id]++;
        }

        final_sum += counters[thread_id];
    };

    std::vector<std::jthread> threads;

    for (int i = 0; i < num_threads; i++)
    {
        threads.emplace_back(work, i);
    }

    return 0;
}