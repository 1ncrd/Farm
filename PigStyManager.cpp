#include "PigStyManager.hpp"
#include <QDebug>
#include <PushButtonToSty.hpp>
#include "GameTimer.hpp"
#include "MyRandom.hpp"

PigStyManager * pig_sty_manager = new PigStyManager;

const int PigStyManager::PigStyPerRow = 10;
const int PigStyManager::PigStyPerColumn = 10;
const QString PigStyManager::QuarantineStyID("-1");
const int PigStyManager::QuarantineStyID_int = -1;
QString PigStyManager::ArchiveName;

PigStyManager::PigStyManager(QObject * parent)
    : QObject{parent}
{
    // It is safe to delete 0x0;
    for (int i = 0; i < PigStyAmount; i++)
    {
        pig_sty[i] = nullptr;
    }

    quarantine_sty = nullptr;

}

PigStyManager::~PigStyManager()
{
    for (int i = 0; i < PigStyAmount; i++)
    {
        delete pig_sty[i];
    }

    delete quarantine_sty;
}
void PigStyManager::StartTheFarm()
{
    // Create 100 `Pig_Sty` object.
    for (int i = 0; i < PigStyAmount; i++)
    {
        pig_sty[i] = new PigSty(QString::number(i));
        // When a new normal pig sty is created, add five pigs.
        pig_sty[i] -> AddPig(5);
    }

    // Create a `quarantine_sty` to manage the infected pig;
    quarantine_sty = new QuarantinePigSty(QuarantineStyID);
    CountPigAmount();
    this -> ConfigueFunc();
}

void PigStyManager::StartTheFarm(const FileManager::GameData &game_data)
{
    PigSty::SetSoldAmount(game_data.pigsty_info.pig_sold_amount);
    PigSty::SetMoney(game_data.pigsty_info.money);

    for (int i = 0; i < PigStyAmount; i++)
    {
        pig_sty[i] = new PigSty(QString::number(i));
    }

    // Create a `quarantine_sty` to manage the infected pig;
    quarantine_sty = new QuarantinePigSty(QuarantineStyID);

    for (QVector<Pig::PigInfo>::const_iterator iter = game_data.pig_data.begin(); iter != game_data.pig_data.end(); iter++)
    {
        Pig * pig = new Pig(*iter);

        if (iter -> is_quarantined)
        {
            quarantine_sty -> AppendPig(pig);
        }
        else
        {
            pig_sty[pig -> GetStyID().toInt()] -> AppendPig(pig);
        }
    }

    CountPigAmount();
    this -> ConfigueFunc();
}

QVector<Pig::PigInfo> PigStyManager::GetAllPigData()
{
    QVector<Pig::PigInfo> result;

    for (int i = 0; i < PigStyAmount; i++)
    {
        result.append(pig_sty[i] -> GetAllPigData());
    }

    result.append(quarantine_sty -> GetAllPigData());
    return result;
}

void PigStyManager::ConfigueFunc()
{

    // ********************************************************
    // The order of connection is critical.
    // It should determine the order of the function execution.
    // ********************************************************

    // Let pigs grow once a second.
    connect(game_timer, GameTimer::timeout, this, [ = ]()
    {
        for (int i = 0; i < PigStyAmount; i++)
        {
            pig_sty[i] -> LetAllPigGrow();
            pig_sty[i] -> InfectionSpreadInSty();
        }

        this -> InfectionSpreadAcrossSty();

        if (Probability(InfectionPosibility))
        {
            emit InfectionOccur();
        }

        CountPigAmount();
    });

    // Timeout_3Month
    connect(game_timer, GameTimer::Timeout_3Month, this, [ = ]()
    {
        // Sell pigs that meet the requirements once every three months.
        for (int i = 0; i < PigStyAmount; i++)
        {
            pig_sty[i] -> SellPig();
        }

        // After selling the pigs, randomly replenish(补充) pigs in the remaining space
        for (int i = 0; i < PigStyAmount; i++)
        {
            pig_sty[i] -> AddRandomPig();
        }
    });

    connect(this, PigStyManager::InfectionOccur, this, [ = ]()
    {
        pig_sty[Random() % 100] -> InfectOnePig();
    });

    connect(pig_sty[99], PigSty::SellPigFinished, this, [ = ]()
    {
        emit SellPigFinished();
    });

    for (int i = 0; i < PigStyAmount; i++)
    {
        // It looks like signals can be connected to signals in QT6.
        connect(pig_sty[i], PigSty::ReturnStyData, this, SendStyData);
    }

    connect(quarantine_sty, QuarantinePigSty::ReturnQuarantineStyData, this, PigStyManager::SendQuarantineStyData);

    connect(game_timer, GameTimer::timeout, this, SendInfectionInfo);

    for (int i = 0; i < PigStyAmount; i++)
    {
        connect(pig_sty[i], PigSty::ReturnIsInfected, this, [ = ](bool is_infected)
        {
            emit StyIsInfected(i, is_infected);
        });
    }

    connect(this, PigStyManager::QuarantineAllInfectedPig, this, [ = ]()
    {
        for (int i = 0; i < PigStyAmount; i++)
        {
            Pig * pig_to_quarantine_list = pig_sty[i] -> ExtractInfectedPigs();
            quarantine_sty -> AppendPig(pig_to_quarantine_list);
        }
    });

    connect(this, PigStyManager::SellPigFinished, PigSty::GetInstance(), PigSty::MoneyUpdate);
    connect(this, PigStyManager::SellPigFinished, PigSty::GetInstance(), PigSty::SoldAmountUpdate);
    SendInfectionInfo();
}

void PigStyManager::SendInfectionInfo()
{
    bool infection_exists = false;
    int infected_amount = 0;

    for (int i = 0; i < PigStyAmount; i++)
    {
        // Tell the button in `game_main_window` if the sty is infected.
        bool sty_infection_exist = pig_sty[i] -> CheckStyIsInfected();
        infected_amount += pig_sty[i] -> CountInfectedAmount();

        if (sty_infection_exist)
        {
            infection_exists = true;
        }
    }

    // Tell the `label_infection_status` in `game_main_window` if the infection exists.
    emit InfectionExists(infection_exists);
    emit SendInfectedAmount(infected_amount);
}

// Transit the signal from `sty_detail_window` to the specific sty.
QString PigStyManager::GetID(const int &sty_num)
{
    return pig_sty[sty_num] -> GetID();
}

// Transit the signal from `sty_detail_window` to the specific sty.
void PigStyManager::GetStyData(const int &sty_num)
{
    pig_sty[sty_num] -> GetStyData();
}

void PigStyManager::GetQuarantineStyData()
{
    quarantine_sty -> GetQuarantineStyData();
}

// Spread the infection across sty.
void PigStyManager::InfectionSpreadAcrossSty()
{
    QVector<int> sty_infected_num;

    for (int i = 0; i < PigStyAmount; i++)
    {
        if (pig_sty[i] -> InfectionExists())
        {
            sty_infected_num.append(i);
        }
    }

    for (int i = 0; i < sty_infected_num.length(); i++)
    {
        int sty_to_infect[4];   // 0, 1, 2, 3 stand for left, right, above, below.

        if (sty_infected_num[i] % PigStyPerRow == 0)
        {
            sty_to_infect[0] = -1;
        }
        else
        {
            sty_to_infect[0] = sty_infected_num[i] - 1;
        }

        if (sty_infected_num[i] % PigStyPerRow == PigStyPerRow - 1)
        {
            sty_to_infect[1] = -1;
        }
        else
        {
            sty_to_infect[1] = sty_infected_num[i] + 1;
        }

        if (sty_infected_num[i] < PigStyPerRow * 1)
        {
            sty_to_infect[2] = -1;
        }
        else
        {
            sty_to_infect[2] = sty_infected_num[i] - 10;
        }

        if (sty_infected_num[i] >= PigStyPerRow * (PigStyPerColumn - 1))
        {
            sty_to_infect[3] = -1;
        }
        else
        {
            sty_to_infect[3] = sty_infected_num[i] + 10;
        }

        for (int j = 0; j < 4; j++)
        {
            if (sty_to_infect[j] >= 0 and sty_to_infect[j] < PigStyPerRow * PigStyPerColumn)
            {
                pig_sty[sty_to_infect[j]] -> InfectionSpreadFromOthers(); // Bug
            }
        }
    }
}

void PigStyManager::SetInfectionPosibility(const float &posibility)
{
    this -> InfectionPosibility = posibility;
}

void PigStyManager::DisposeQuarantineSty()
{
    this -> quarantine_sty -> DisposeInfectedPig();
}

void PigStyManager::SetArchiveName(const QString &name)
{
    ArchiveName = name;
    PigSty::SetArchiveName(name);
}

void PigStyManager::CountPigAmount()
{
    int pig_amount = 0;

    for (int i = 0; i < PigStyAmount; i++)
    {
        pig_amount += pig_sty[i] -> GetPigAmount();
    }

    emit SendPigAmount(pig_amount);
}
