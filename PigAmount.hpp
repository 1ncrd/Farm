#ifndef PIGAMOUNT_H
#define PIGAMOUNT_H

struct PigAmount
{
    int total = 0;
    int BlackPig = 0;
    int SmallFlowerPig = 0;
    int BigWhitePig = 0;

    PigAmount();
    PigAmount(int total, int BlackPig, int SmallFlowerPig, int BigWhitePig);
    PigAmount operator + (const PigAmount& amount);
    PigAmount &operator += (const PigAmount &amount);
};

#endif // PIGAMOUNT_H
