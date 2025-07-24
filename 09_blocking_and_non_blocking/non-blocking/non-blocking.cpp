#include <atomic>
#include <cassert>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>

int main(){

    const int iteration = 1 << 15;
    const int num_threads = 8;

    std::atomic<int> sink = 0;
    auto work = [&](){
        while(true){
            int desired;

            int expected =  sink.load();
            do {
                if(expected == iteration) return;

                desired = expected + 1;
            } while (!sink.compare_exchange_strong(expected,desired));
            
        }
    };

    auto slow_work = [&](){
        while(true){
            int desired;

            int expected = sink.load();

            do {
                if(expected == iteration) return;

                desired = expected + 1;

            } while(!sink.compare_exchange_strong(expected,desired));
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    };

    std::vector<std::jthread> threads;
    for(int i =0;i<num_threads;i++){
        threads.emplace_back(work);
    }

    threads.emplace_back(slow_work);

    for(auto &thread: threads) thread.join();

    std::cout << "Completed " << sink << "iterations \n";

    return 0;
}
