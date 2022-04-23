#include <cstdlib>
#include <ctime>

int Random()
{
    static unsigned int offset = 0;
    srand(time(0) + offset);
    offset += rand();
    return rand();
}

bool Probability(const int &percent)
{
    return (Random() % 100 < percent);
}
