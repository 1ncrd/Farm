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
private:
    QString in_sty_id;          // 0 ~ 99
    QString birthday;           // birthday = current game time

    QString id;                 // sty_id + birthday / 30. I didn't use the 0~9 because it looks a little bit strange.
    int age;                    // day
    volatile float weight;      // kg
    PigSpecies species;
    bool is_infected;

    Pig * next_pig;
    Pig * previous_pig;
public:
    friend class PigSty;

    struct PigInfo
    {
        QString id;
        int age;
        float weight;
        PigSpecies species;
        bool is_infected;
    };

    explicit Pig(const QString &sty_id_temp, const int &order, const int &blackpig_allowed);
    ~Pig();

    QString PigIDGenerator(const int &order);
    static std::map<PigSpecies, QString> EnumToSpeciesName;               // Literal meaning
    static std::map<Pig::PigSpecies, int> SpeciesPrice;            // selling price

    static const short PriceBlackPig, PriceSmallFlowerPig, PriceBigWhitePig;
    void Grow();
    int Random();
};

#endif // PIG_H
