#include "MyRandom.hpp"
#include <cstdlib>
#include <random>
#include <profileapi.h>
using namespace std;
int Random()
{
    LARGE_INTEGER seed;
    QueryPerformanceFrequency(&seed);   // 返回硬件支持的高精度计数器的频率
    QueryPerformanceCounter(&seed);     // 函数返回高精确度性能计数器的值, 以微妙为单位。
//    srand(seed.QuadPart);
//    return rand();
    default_random_engine r;
    r.seed(seed.QuadPart);
    return r();
}

bool Probability(const int &percent)
{
    return (Random() % 100 < percent);
}
