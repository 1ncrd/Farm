#ifndef PIGSTY_H
#define PIGSTY_H

#include <QObject>
#include <QString>
#include <QReadWriteLock>
#include "GameTimer.hpp"
#include "Pig.hpp"
#include "FileManager.hpp"

class PigSty : public QObject
{

    Q_OBJECT
public:
    static const int PigAmountMax = 10; // For the `quarantine_sty`, the pig amount have no limit.
    static long money;
    static int InfectionTransRateInSty;
    static int InfectionTransRateAcrossSty;

    struct PigSoldAmount
    {
        int total = 0;
        int BlackPig = 0;
        int SmallFlowerPig = 0;
        int BigWhitePig = 0;
    };

    static PigSoldAmount pig_sold_amount;
protected:
    Pig * ptr_pig_head;
    int pig_amount;
    QString id;
public:
    explicit PigSty(const QString &sty_id_temp, QObject *parent = nullptr);
    ~PigSty();

    void AddRandomPig();
    void AddPig(int number = 1);
    // For the `quarantine_sty`.
    void AppendPig(Pig * const &ptr_pig_head_to_append);
    void SellPig();
    void CountSoldPig(Pig * const &ptr_pig_to_count) const;
    void RecordTrade(const FileManager::TradeType &type, Pig * const &ptr_pig_to_record) const;
    void DeletePig(Pig * const &ptr_pig_to_delete);
    void DeleteAllPigs();
    void LetAllPigGrow();
    void InfectOnePig();
    void InfectionSpreadInSty();
    void InfectionSpreadFromOthers();
    bool InfectionExists();
    Pig * ExtractInfectedPigs();
    void ExtractPig(Pig * pig_middle);
    int GetPigAmount() const;
    void GetStyData();
    bool CheckStyIsInfected();
    QString GetID() const;
    enum StySpeciesSituation {NoPig, BlackPigExistence, BlackPigNonexistence};
    StySpeciesSituation CheckStySpeciesSituation() const;

signals:
    ReturnStyData(QVector<Pig::PigInfo> data);
    ReturnIsInfected(bool is_infected);
    SellPigFinished();
};

#endif // PIGSTY_H
