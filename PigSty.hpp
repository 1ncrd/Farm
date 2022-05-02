#ifndef PIGSTY_H
#define PIGSTY_H

#include <QObject>
#include <QString>
#include <QReadWriteLock>
#include "GameTimer.hpp"
#include "Pig.hpp"
#include "FileManager.hpp"
#include "PigSoldAmount.hpp"

class PigSty : public QObject
{

    Q_OBJECT
public:
    static const int PigAmountMax = 10; // For the `quarantine_sty`, the pig amount have no limit.
    static long money;
    static int InfectionTransRateInSty;
    static int InfectionTransRateAcrossSty;
    static QString ArchiveName;

    static PigSoldAmount pig_sold_amount;
private:
    static PigSty * temp_instance;
protected:
    Pig * ptr_pig_head;
    int pig_amount;
    QString id;
public:
    explicit PigSty(QObject *parent = nullptr);
    explicit PigSty(const QString &sty_id_temp, QObject *parent = nullptr);
    ~PigSty();

    void AddRandomPig();
    void AddPig(int number = 1);
    void AppendPig(Pig * const &ptr_pig_head_to_append);
    void SellPig();
    void RecordTrade(const FileManager::TradeType &type, Pig * const &ptr_pig_to_record) const;
    void DeletePig(Pig * const &ptr_pig_to_delete);
    void DeleteAllPigs();
    void LetAllPigGrow();
    int CountInfectedAmount();
    void InfectOnePig();
    void InfectionSpreadInSty();
    void InfectionSpreadFromOthers();
    bool InfectionExists();
    Pig * ExtractInfectedPigs();
    void ExtractPig(Pig * pig_middle);
    int GetPigAmount() const;
    void GetStyData();
    QVector<Pig::PigInfo> GetAllPigData();
    bool CheckStyIsInfected();
    QString GetID() const;
    enum StySpeciesSituation {NoPig, BlackPigExistence, BlackPigNonexistence};
    StySpeciesSituation CheckStySpeciesSituation() const;
    static void SetSoldAmount(const PigSoldAmount &amount);
    static void AddSoldAmountPig(const Pig::PigSpecies &species, const int &amount = 1);
    static void SetMoney(const int &amount);
    static void AddMoney(const int &amount);
    static PigSty * GetInstance();
    static void DeleteInstance();
    static void SetArchiveName(const QString &name);
signals:
    ReturnStyData(QVector<Pig::PigInfo> data);
    ReturnIsInfected(bool is_infected);
    SellPigFinished();
    SoldAmountUpdate();
    MoneyUpdate();
};

#endif // PIGSTY_H
