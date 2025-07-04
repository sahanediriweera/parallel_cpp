#include <algorithm>
#include <chrono>
#include <iterator>
#include <random>
#include <span>
#include <thread>
#include <vector>

int main(){

    std::random_device rd;
    std::mt19937 mt(rd());

    std::uniform_int_distribution bin_1(1,25);
    std::uniform_int_distribution bin_2(26,50);
    std::uniform_int_distribution bin_3(51,75);
    std::uniform_int_distribution bin_4(76,100);

    int num_work_items = 1 << 18;
    int num_of_threads = 8;

    std::vector<int> work_items;

    work_items.reserve(num_work_items);

    for(int i =0;i<num_work_items;i++){
        work_items.push_back(bin_1(mt));
        work_items.push_back(bin_1(mt));

        work_items.push_back(bin_2(mt));
        work_items.push_back(bin_2(mt));
        
        work_items.push_back(bin_3(mt));
        work_items.push_back(bin_3(mt));
        
        work_items.push_back(bin_4(mt));
        work_items.push_back(bin_4(mt));
    }


    auto work = [&] (int thread_id){
        for(int i = thread_id; i< num_of_threads;i++){
            std::this_thread::sleep_for(std::chrono::microseconds(work_items[i]));
        }
    };

    std::vector<std::jthread> threads;

    for(int i =0;i<num_of_threads;i++){
        threads.emplace_back(work,i);
    }

    return 0;
}