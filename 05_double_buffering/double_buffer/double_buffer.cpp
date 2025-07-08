#include <random>
#include <semaphore>
#include <span>
#include <thread>
#include <vector>

void generate_data(std::span<int> data)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution dist(1, 100);

    for (auto &value : data)
    {
        value = dist(mt);
    }
}

void process_data(std::span<int> data)
{
    for (int i = 0; i < 5; i++)
    {
        for (auto &value : data)
        {
            value %= value + 1;
        }
    }
}

int main()
{

    std::vector<int> data_1;
    std::vector<int> data_2;

    std::binary_semaphore signal_to_process{0};
    std::binary_semaphore signal_to_generate{1};

    const int num_iterations = 0;

    auto data_generator = [&]()
    {
        for (int i = 0; i < num_iterations; i++)
        {
            generate_data(data_1);

            signal_to_generate.acquire();

            data_1.swap(data_2);

            signal_to_process.release();
        }
    };

    auto data_processor = [&]()
    {
        for (int i = 0; i < num_iterations; i++)
        {
            signal_to_process.acquire();

            process_data(data_2);

            signal_to_generate.release();
        }
    };

    std::jthread generator(data_generator);
    std::jthread processor(data_processor);

    return 0;
}