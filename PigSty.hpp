#ifndef PIGSTY_H
#define PIGSTY_H

#include <QObject>
#include <QString>
#include <QReadWriteLock>
#include "Pig.hpp"
#include "FileManager.hpp"
class PigSty : public QObject
{

    Q_OBJECT
private:
    QReadWriteLock rwlock;
    Pig * ptr_pig_head;
    int pig_amount;
    QString id;
public:
    struct PigSoldAmount
    {
        int total = 0;
        int BlackPig = 0;
        int SmallFlowerPig = 0;
        int BigWhitePig = 0;
    };
    static const int PigAmountMax = 10;
    struct PigStyData
    {
        // C++11 standard allows setting defaults.
        // The struct of the return data when query the pig sty information.
        QString pig_id[PigAmountMax] = {QString("")};
        Pig::PigSpecies pig_species[PigAmountMax] = {Pig::PigSpecies(0)};
        float pig_weight[PigAmountMax] = {0};
        int pig_age[PigAmountMax] = {0};
        int pig_amount = 0;
    };
    static long money;
    static PigSoldAmount pig_sold_amount;

    explicit PigSty(const QString &sty_id_temp, QObject *parent = nullptr);
    ~PigSty();

    void AddRandomPig();
    void AddPig(int number = 1);
    void SellPig();
    void CountSoldPig(Pig * const &ptr_pig_to_count) const;
    void RecordTrade(const FileManager::TradeType &type, Pig * const &ptr_pig_to_record) const;
    void DeletePig(Pig * const &ptr_pig_to_delete);
    void DeleteAllPigs();
    void LetAllPigGrow();
    int Random() const;
    int GetPigAmount() const;

    void GetStyData();
    QString GetID() const;
    enum StySpeciesSituation {NoPig, BlackPigExistence, BlackPigNonexistence};
    StySpeciesSituation CheckStySpeciesSituation() const;


signals:
    SendStyData(PigStyData &pig_data);
    SellPigFinished();
};

#endif // PIGSTY_H
