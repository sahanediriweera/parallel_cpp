#include <vector>
#include <random>
#include <span>

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
    std::vector<int> data;
    data.resize(1 << 20);

    for (int i = 0; i < 100; i++)
    {
        generate_data(data);
        process_data(data);
    }
}