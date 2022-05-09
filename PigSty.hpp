#ifndef PIGSTY_H
#define PIGSTY_H

#include <QObject>
#include <QString>
#include <QReadWriteLock>
#include "GameTimer.hpp"
#include "Pig.hpp"
#include "FileManager.hpp"
#include "PigAmount.hpp"

class PigSty : public QObject
{

    Q_OBJECT
public:
    static const int PigAmountMax = 10; // For the `quarantine_sty`, the pig amount have no limit.
    static const int PigLiveTimeAfterInfected;
    static long money; // Store the total amount of money earned.
    static int InfectionTransRateInSty; // The probability of a infection spreading in a pigsty.
    static int InfectionTransRateAcrossSty; // The probability of a infection spreading across pigsty.
    static QString ArchiveName; // Current archive name.

    static PigAmount pig_sold_amount; // The total amount of sold pig.
private:
    static PigSty * temp_instance; // Used to emit the signal without the specific object.
protected:
    Pig * ptr_pig_head;
    PigAmount pig_amount;
    QString id;
public:
    explicit PigSty(QObject *parent = nullptr);
    explicit PigSty(const QString &sty_id_temp, QObject *parent = nullptr);
    ~PigSty();

    void AddTo10Pig();
    void AddPig(int number = 1);
    void AppendPig(Pig * const &ptr_pig_head_to_append);
    int SellPig();
    void RecordTrade(const FileManager::TradeType &type, Pig * const &ptr_pig_to_record) const;
    void DeletePig(Pig * const &ptr_pig_to_delete);
    void DeleteAllPigs();
    void LetAllPigGrow();
    int CountInfectedAmount();
    void InfectOnePig();
    void InfectionSpreadInSty();
    void InfectionSpreadFromOthers();
    bool InfectionExists();
    void InfectedPigDiedAfterTime();
    Pig * ExtractInfectedPigs();
    void ExtractPig(Pig * pig_middle);
    int GetPigTotalAmount() const;
    PigAmount GetPigAmount() const;
    void AddPigAmount(Pig::PigSpecies species);
    void ReducePigAmount(Pig::PigSpecies species);
    void GetStyData_ReturnWithSignal();
    QVector<Pig::PigInfo> GetStyData_ReturnWithValue();
    bool CheckStyIsInfected();
    QString GetID() const;
    enum StySpeciesSituation {NoPig, BlackPigExistence, BlackPigNonexistence};
    StySpeciesSituation CheckStySpeciesSituation() const;
    static void SetSoldAmount(const PigAmount &amount);
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
