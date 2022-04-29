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
    this -> is_quarantined = false;

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

    this -> previous_pig = nullptr;
    this -> next_pig = nullptr;
}

Pig::Pig(const QString &id, const int &age, const float &weight, const PigSpecies &species,
         const bool &is_infected, const int &infected_time, const bool &is_quarantined)
{
    this -> id = id;
    this -> in_sty_id = id.left(2);
    this -> age = age;

    if (id.length() == 8)
    {
        this -> birthday = QString::number((id.mid(2, 5)).toInt());
    }
    else if (id.length() == 10)
    {
        this -> birthday = QString::number((id.mid(2, 7)).toInt());
    }

    this -> weight = weight;
    this -> species = species;
    this -> is_infected = is_infected;
    this -> infected_time = infected_time;
    this -> is_quarantined = is_quarantined;
    this -> previous_pig = nullptr;
    this -> next_pig = nullptr;
}

Pig::Pig(const PigInfo &pig_info)
    : Pig::Pig(pig_info.id, pig_info.age, pig_info.weight, pig_info.species,
               pig_info.is_infected, pig_info.infected_time, pig_info.is_quarantined)
{

}
Pig::~Pig()
{
    // qDebug() << "[+] Pig ID : " + id + " released.";
}

QString Pig::PigIDGenerator(const int &order)    // The order aim to distinguish between piglets(small pig) born at the same time.
{
    // sty_id + birthday / 30 + order
    //    2   +    5 or 7     +  1
    int ID_length = 8;

    if (birthday.length() > 5)
    {
        ID_length = 10;
    }

    return in_sty_id +
           QString("").fill('0', int(ID_length - in_sty_id.length() - QString::number(birthday.toInt() / 30).length() - 1))
           + QString::number(birthday.toInt() / 30) + QString::number(order);
}

QString Pig::GetStyID()
{
    return in_sty_id;
}

Pig::PigInfo Pig::GetPigInfo()
{
    PigInfo pig_info;
    pig_info.id = id;
    pig_info.age = age;
    pig_info.species = species;
    pig_info.weight = weight;
    pig_info.is_infected = is_infected;
    pig_info.is_quarantined = is_quarantined;
    pig_info.infected_time = infected_time;

    return pig_info;
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
