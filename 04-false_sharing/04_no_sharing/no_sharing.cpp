#include <array>
#include <atomic>
#include <thread>
#include <vector>

struct AlignedAtomic
{
    alignas(64) std::atomic<int> counter = 0;
};

int main()
{
    const int num_iterations = 1 << 27;

    const int num_threads = 4;

    std::array<AlignedAtomic, 4> counters;
    std::atomic<int> final_sum = 0;

    const int elements_per_thread = num_iterations / num_threads;

    auto work = [&](int thread_id)
    {
        for (int i = 0; i < elements_per_thread; i++)
        {
            counters[thread_id].counter++;
        }

        final_sum += counters[thread_id].counter;
    };

    std::vector<std::jthread> threads;

    for (int i = 0; i < num_threads; i++)
    {
        threads.emplace_back(work, i);
    }

    return 0;
}
