#include <tbb/concurrent_queue.h>

#include <random>
#include <thread>
#include <vector>

int main()
{

    const int num_elements = 1 << 25;

    const int num_threads = 8;
    const int elements_per_thread = num_elements / num_threads;

    tbb::concurrent_queue<int> queue;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution dist(1, 100);

    auto work = [&]()
    {
        for (int i = 0; i < elements_per_thread; i++)
        {
            queue.push(dist(mt));
        }
    };

    std::vector<std::jthread> threads;
    for (int i = 0; i < num_threads; i++)
    {
        threads.emplace_back(work);
    }

    return 0;
}