#include "PigAmount.hpp"

PigAmount::PigAmount()
{

}

PigAmount::PigAmount(int total, int BlackPig, int SmallFlowerPig, int BigWhitePig)
{
    this -> total = total;
    this -> BlackPig = BlackPig;
    this -> SmallFlowerPig = SmallFlowerPig;
    this -> BigWhitePig = BigWhitePig;
}

PigAmount PigAmount::operator + (const PigAmount& amount)
{
    return PigAmount(this -> total + amount.total, this -> BlackPig + amount.BlackPig,
                     this -> SmallFlowerPig + amount.SmallFlowerPig, this -> BigWhitePig + amount.BigWhitePig);
}

PigAmount & PigAmount::operator += (const PigAmount &amount)
{
    this -> total += amount.total;
    this -> BlackPig += amount.BlackPig;
    this -> SmallFlowerPig += amount.SmallFlowerPig;
    this -> BigWhitePig += amount.BigWhitePig;
    return *this;
}
