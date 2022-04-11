#include "GameTimer.hpp"
#include "Pig.hpp"
#include <cstdlib>
#include <ctime>
#include <QDebug>

const short Pig::PriceBlackPig = 30, Pig::PriceSmallFlowerPig = 14, Pig::PriceBigWhitePig = 12;
int Pig::Random()
{
    /*
     * I encapsulated a function to generate random numbers
     * to solve the problem of too short random number generation intervals(much less then 1s).
     * Because each round of random number generation takes less than a second,
     * the function time(0) return same number in a second.
     * So I add the sty_id and the pig_id into the generation process.
     * the constant was selected from the first 30 digits of the golden section(0.6180339887 4989484820 4586834365...)
     * This algorithm involves sty_id and pig_id in the seed generation process for the better randomness.
     */
    srand(time(0) ^ (in_sty_id.toInt() * 6180339887) ^ (4989484820 ^ (id.toInt() * 4586834365)));
    return rand();
}
Pig::Pig(const QString &sty_id_temp, const int &order/*The order of the pig born at the same time.*/, const int &sty_species_situation)
{
    // Initialize the sty_id, weight and PigSpecies.
    this -> age = 0;
    this -> in_sty_id = sty_id_temp;
    this -> birthday = QString::number(game_timer -> GetTime());
    this -> id = PigIDGenerator(order);
    this -> weight = Random() % 30 + 20;

    // The generation of pig species depends on the sty's situation.
    switch (StySpeciesSituation(sty_species_situation))
    {
        case NoPig:
            this -> species = PigSpecies(Random() % 3);
            break;

        case BlackPigExistence:
            this -> species = BlackPig;
            break;

        case BlackPigNonexistence:
            this -> species = PigSpecies(Random() % 2 + 1);
    }

    this -> next_pig = nullptr;
}

Pig::~Pig()
{
//    qDebug() << "Pig released.";
}

QString Pig::PigIDGenerator(const int &order)    // The order aim to distinguish between piglets(small pig) born at the same time.
{
    int ID_length = 8;

    if (birthday.length() > 8)
    {
        ID_length = 10;
    }

    return in_sty_id +
           QString("").fill('0', int(ID_length - in_sty_id.length() - QString::number(birthday.toInt() / 30).length() - 1))
           + QString::number(birthday.toInt() / 30) + QString::number(order);
}


std::map<int, QString> Pig::EnumToSpeciesName =
{
    {0, QString("Black pig")}, {1, QString("Small flower pig")},
    {2, QString("Big white pig")}
};

std::map<Pig::PigSpecies, int> Pig::SpeciesPrice =
{
    {BlackPig, Pig::PriceBlackPig}, {SmallFlowerPig, Pig::PriceSmallFlowerPig},
    {BigWhitePig, Pig::PriceBigWhitePig}
};
void Pig::Grow()
{
    // Weight gain is in range 0.0kg to 1.2kg at a time.
    this -> age++;
    this -> weight += ((Random() % 12) / 10.0);
}
