#include <tbb/parallel_for.h>

#include <algorithm>
#include <chrono>
#include <iterator>
#include <random>
#include <vector>

int main()
{
    std::random_device rd;
    std::mt19937 mt(rd());

    std::uniform_int_distribution bin(20, 30);

    int num_work_items = 1 << 10;

    std::vector<int> work_items;

    std::generate_n(std::back_inserter(work_items), num_work_items, [&]
                    { return bin(mt); });

    tbb::parallel_for(
        tbb::blocked_range<int>(0, num_work_items),
        [&](tbb::blocked_range<int> r)
        {
            for (int i = r.begin(); i < r.end(); i++)
            {
                std::this_thread::sleep_for(std::chrono::microseconds(work_items[i]));
            }
        },
        tbb::static_partitioner());

    return 0;
}