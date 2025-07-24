#include <atomic>
#include <cassert>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>

int main(){

    const int iterations = 1 << 25;
    const int num_threads = 8;
    const int items_per_thread = iterations / num_threads;

    std::atomic<int> sink = 0;

    auto work = [&](){
        for(int i =0;i<items_per_thread;i++) sink++;
    };

    std::vector<std::jthread> threads;

    for(int i =0;i<num_threads;i++){
        threads.emplace_back(work);
    }

    return 0;
}