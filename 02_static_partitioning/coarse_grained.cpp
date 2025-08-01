#include <algorithm>
#include <chrono>
#include <iterator>
#include <random>
#include <span>
#include <thread>
#include <vector>

int main()
{

    std::random_device rd;
    std::mt19937 mt(rd());

    std::uniform_int_distribution bin_1(1, 25);
    std::uniform_int_distribution bin_2(36, 50);
    std::uniform_int_distribution bin_3(51, 75);
    std::uniform_int_distribution bin_4(76, 100);

    int num_work_items = 1 << 10;
    int n_bins = 4;
    int elements_per_bin = num_work_items / n_bins;

    std::vector<int> work_items;

    std::generate_n(
        std::back_inserter(work_items),
        elements_per_bin,
        [&]
        { return bin_1(mt); });
    std::generate_n(
        std::back_inserter(work_items),
        elements_per_bin,
        [&]
        { return bin_2(mt); });
    std::generate_n(
        std::back_inserter(work_items),
        elements_per_bin,
        [&]
        { return bin_3(mt); });
    std::generate_n(
        std::back_inserter(work_items),
        elements_per_bin,
        [&]
        { return bin_4(mt); });

    auto work = [](std::span<int> items)
    {
        for (const auto item : items)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(item));
        }
    };

    int num_threads = 8;
    int items_per_thread = num_work_items / num_threads;

    std::vector<std::jthread> threads;

    for (int i = 0; i < num_threads; i++)
    {
        int start = i * items_per_thread;
        threads.emplace_back(work, std::span(work_items.begin() + start, items_per_thread));
    }

    return 0;
}