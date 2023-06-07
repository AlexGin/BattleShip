#pragma once
#include <random> // std::mt19937 support
// Psevdo Random Number Generator:
class PRNG
{
public:
    PRNG()
    {
        std::random_device device;
        random_generator_.seed(device());
    }
    int getRandom() 
    {
        std::uniform_int_distribution<int> range(0, 99);
        return range(random_generator_);
    }
private:
    std::mt19937 random_generator_;
};