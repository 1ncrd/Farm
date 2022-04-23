#include "Pig.hpp"
#include "GameTimer.hpp"
#include <cstdlib>
#include <ctime>
#include <QDebug>
#include "MyRandom.hpp"

std::map<Pig::PigSpecies, QString> Pig::EnumToSpeciesName =
{
    {Pig::BlackPig, QString("Black pig")},
    {Pig::SmallFlowerPig, QString("Small flower pig")},
    {Pig::BigWhitePig, QString("Big white pig")}
};

std::map<Pig::PigSpecies, int> Pig::SpeciesPrice =
{
    {Pig::BlackPig, 30},
    {Pig::SmallFlowerPig, 14},
    {Pig::BigWhitePig, 12}
};

Pig::Pig(const QString &sty_id_temp, const int &order/*The order of the pig born at the same time.*/, const int &sty_species_situation)
{
    // Initialize the pig.
    this -> age = 0;
    this -> in_sty_id = sty_id_temp;
    this -> birthday = QString::number(game_timer -> GetTime());
    this -> id = PigIDGenerator(order);
    this -> weight = Random() % 30 + 20;
    this -> is_infected = false;
    this -> infected_time = -1;

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
    qDebug() << "Pig released." << in_sty_id;
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


void Pig::Grow()
{
    // Weight gain is in range 0.0kg to 1.2kg at a time.
    this -> age++;
    this -> weight += ((Random() % 12) / 10.0);

    if (this -> is_infected)
    {
        infected_time++;
    }
}
