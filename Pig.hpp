
#ifndef PIG_H
#define PIG_H

#include <map>
#include <QString>
#include <cstdlib>

class Pig
{
public:
    enum PigSpecies {BlackPig = 0, SmallFlowerPig, BigWhitePig};
    enum StySpeciesSituation {NoPig = 0, BlackPigExistence, BlackPigNonexistence};
    static std::map<PigSpecies, QString> EnumToSpeciesName; // Literal meaning
    static std::map<PigSpecies, int> SpeciesPrice;          // selling price

    struct PigInfo
    {
        QString id;
        int age;
        float weight;
        PigSpecies species;
        bool is_infected;
    };
private:
    QString birthday;           // birthday = current game time
    QString in_sty_id;          // 0 ~ 99
    QString id;                 // sty_id + birthday / 30. I didn't use the 0~9 because it looks a little bit strange.
    int age;                    // day
    volatile float weight;      // kg
    PigSpecies species;
    bool is_infected;
    int infected_time;

    Pig * next_pig;
    Pig * previous_pig;
public:
    friend class PigSty;
    friend class QuarantinePigSty;

    explicit Pig(const QString &sty_id_temp, const int &order, const int &blackpig_allowed);
    ~Pig();

    QString PigIDGenerator(const int &order);

    void Grow();
};

#endif // PIG_H
