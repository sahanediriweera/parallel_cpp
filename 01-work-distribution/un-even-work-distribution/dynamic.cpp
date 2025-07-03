#include <tbb/parallel_for.h>

#include <algorithm>
#include <chrono>
#include <iterator>
#include <random>
#include <vector>

int main(){

    std::random_device rd;
    std::mt19937 mt(rd());

    std::uniform_int_distribution bin_1(1,25);
    std::uniform_int_distribution bin_2(26,50);
    std::uniform_int_distribution bin_3(51,75);
    std::uniform_int_distribution bin_4(76,100);

    int num_work_items = 1 << 10;
    int n_bins = 4;
    int elements_per_bin = num_work_items / n_bins;

    std::vector<int> work_items;

    std::generate_n(
        std::back_inserter(work_items),
        elements_per_bin,
        [&] {return bin_1(mt);}
    );
    std::generate_n(
        std::back_inserter(work_items),
        elements_per_bin,
        [&] {return bin_2(mt);}
    );
    std::generate_n(
        std::back_inserter(work_items),
        elements_per_bin,
        [&] {return bin_3(mt);}
    );
    std::generate_n(
        std::back_inserter(work_items),
        elements_per_bin,
        [&] {return bin_4(mt);}
    );

    return 0;
}