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
        result.append(pig_sty[i] -> GetStyData_ReturnWithValue());
    }

    result.append(quarantine_sty -> GetStyData_ReturnWithValue());
    return result;
}

void PigStyManager::ConfigueFunc()
{
    // **********************************************************
    // The order of connection is critical.
    // It should determine the order of the function execution.
    // For example, the `SellPig` should before the `AddTo10Pig`.
    // **********************************************************

    // Let pigs grow once a second.
    connect(game_timer, GameTimer::timeout, this, [ = ]()
    {
        this -> InfectionSpreadAcrossSty();

        for (int i = 0; i < PigStyAmount; i++)
        {
            pig_sty[i] -> LetAllPigGrow();
            pig_sty[i] -> InfectionSpreadInSty();
            pig_sty[i] -> InfectedPigDiedAfterTime();
        }

        SendInfectionInfo();
        emit SendTimeToDieOut(CalculateTimeToDieout());

        if (Probability(InfectionPosibility))
        {
            emit InfectionOccur();
        }

        CountPigAmount();
    });

    // Timeout_3Month
    connect(game_timer, GameTimer::Timeout_3Month, this, [ = ]()
    {
        int sale_money = 0;

        // Sell pigs that meet the requirements once every three months.
        for (int i = 0; i < PigStyAmount; i++)
        {
            sale_money += pig_sty[i] -> SellPig();
        }

        // Calculate the money earned from selling the pigs in this sale.
        emit SendSaleMoney(sale_money);

        // After selling the pigs, replenish(补充) pigs in the remaining space
        for (int i = 0; i < PigStyAmount; i++)
        {
            pig_sty[i] -> AddTo10Pig();
        }
    });

    connect(this, PigStyManager::InfectionOccur, this, [ = ]()
    {
        // Choose a random sty to outbreak.
        pig_sty[Random() % 100] -> InfectOnePig();
    });

    connect(pig_sty[99], PigSty::SellPigFinished, this, [ = ]()
    {
        // emit this signal to update the data in game main window.
        emit SellPigFinished();
    });

    for (int i = 0; i < PigStyAmount; i++)
    {
        // It looks like signals can be connected to signals in QT6.
        connect(pig_sty[i], PigSty::ReturnStyData, this, SendStyData);
    }

    connect(quarantine_sty, QuarantinePigSty::ReturnQuarantineStyData, this, PigStyManager::SendQuarantineStyData);

    for (int i = 0; i < PigStyAmount; i++)
    {
        // Accept the signal from pig sty and send it to game main window.
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
    // Send the time it takes for all the pigs to die.
//    emit SendTimeToDieOut();
}

// Transit the signal from `sty_detail_window` to the specific sty.
QString PigStyManager::GetID(const int &sty_num)
{
    return pig_sty[sty_num] -> GetID();
}

// Transit the signal from `sty_detail_window` to the specific sty.
void PigStyManager::GetStyData(const int &sty_num)
{
    pig_sty[sty_num] -> GetStyData_ReturnWithSignal();
}

void PigStyManager::GetQuarantineStyData()
{
    quarantine_sty -> GetQuarantineStyData();
}

// Spread the infection across sty.
void PigStyManager::InfectionSpreadAcrossSty()
{
    QVector<int> infected_sty_num;

    for (int i = 0; i < PigStyAmount; i++)
    {
        if (pig_sty[i] -> InfectionExists())
        {
            infected_sty_num.append(i);
        }
    }

    for (int i = 0; i < infected_sty_num.length(); i++)
    {
        int sty_to_infect[4];
        GetStyAround(infected_sty_num[i], sty_to_infect);

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
    PigAmount pig_amount;

    for (int i = 0; i < PigStyAmount; i++)
    {
        pig_amount += pig_sty[i] -> GetPigAmount();
    }

    emit SendPigAmount(pig_amount);
}

int PigStyManager::CalculateTimeToDieout()
{
    int day_result = 0;
    const int TimeSpreadAcross = 100 / PigSty::InfectionTransRateAcrossSty;
    int min = 100;
    bool infection_exists = false;

    for (int i = 0; i < PigStyPerColumn; i++)
    {
        for (int j = 0; j < PigStyPerColumn; j++)
        {
            if (pig_sty[i * PigStyPerColumn + j] -> InfectionExists())
            {
                infection_exists = true;
                int distance = 0;

                if (i > 5)
                {
                    distance += i;
                }
                else
                {
                    distance += 9 - i;
                }

                if (j > 5)
                {
                    distance += j;
                }
                else
                {
                    distance += 9 - j;
                }

                if (distance < min)
                {
                    min = distance;
                }
            }
        }
    }

    day_result = min * (TimeSpreadAcross - 1);
    day_result += 10;
    qDebug() << "[+] Day to dieout:" << day_result;

    if (infection_exists)
    {
        return day_result;
    }
    else
    {
        return -1;
    }
}

int * PigStyManager::GetStyAround(int i, int sty_to_infect[4])
{
    if (i % PigStyPerRow == 0)
    {
        sty_to_infect[0] = -1;
    }
    else
    {
        sty_to_infect[0] = i - 1;
    }

    if (i % PigStyPerRow == PigStyPerRow - 1)
    {
        sty_to_infect[1] = -1;
    }
    else
    {
        sty_to_infect[1] = i + 1;
    }

    if (i < PigStyPerRow * 1)
    {
        sty_to_infect[2] = -1;
    }
    else
    {
        sty_to_infect[2] = i - 10;
    }

    if (i >= PigStyPerRow * (PigStyPerColumn - 1))
    {
        sty_to_infect[3] = -1;
    }
    else
    {
        sty_to_infect[3] = i + 10;
    }

    return sty_to_infect;
}
