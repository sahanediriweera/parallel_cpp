#include <atomic>
#include <vector>
#include <thread>

int main()
{
    const int num_iterations = 1 << 27;

    const int num_threads = 4;

    const int elements_per_thread = num_iterations / num_threads;

    std::atomic<int> counter = 0;

    auto work = [&]()
    {
        for (int i = 0; i < elements_per_thread; i++)
        {
            counter++;
        }
    };

    std::vector<std::jthread> threads;

    for (int i = 0; i < num_threads; i++)
    {
        threads.emplace_back(work);
    }

    return 0;
}