#include "Pig.hpp"
#include "GameTimer.hpp"
#include <cstdlib>
#include <ctime>
#include <QDebug>

extern GameTimer * game_timer;
const short Pig::PriceBlackPig = 30, Pig::PriceSmallFlowerPig = 14, Pig::PriceBigWhitePig = 12;
int Pig::Random()
{
    // Generate different number in one second.
    static unsigned int offset = 0;
    srand(time(0) + id.toInt() + offset);
    offset += rand();
    return rand();
}
Pig::Pig(const QString &sty_id_temp, const int &order/*The order of the pig born at the same time.*/, const int &sty_species_situation)
{
    // Initialize the pig.
    this -> age = 0;
    this -> in_sty_id = sty_id_temp;
    this -> birthday = QString::number(game_timer -> GetTime());
    this -> id = PigIDGenerator(order);
    this -> weight = Random() % 30 + 20;
    this -> is_infected = false;

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


std::map<Pig::PigSpecies, QString> Pig::EnumToSpeciesName =
{
    {Pig::BlackPig, QString("Black pig")}, {Pig::SmallFlowerPig, QString("Small flower pig")},
    {Pig::BigWhitePig, QString("Big white pig")}
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
