#include <atomic>
#include <thread>
#include <vector>

int main()
{

    const int num_iterations = 1 << 27;

    const int num_threads = 1;

    std::atomic<int> counter = 0;

    auto work = [&]()
    {
        for (int i = 0; i < num_iterations; i++)
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