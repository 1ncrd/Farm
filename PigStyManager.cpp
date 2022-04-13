#include "PigStyManager.hpp"
#include <QDebug>

PigStyManager * pig_sty_manager = new PigStyManager;

const int PigStyManager::PigStyPerRow = 10;
const int PigStyManager::PigStyPerColumn = 10;

PigStyManager::PigStyManager(QObject * parent)
    : QObject{parent}
{

}

void PigStyManager::Create100Sty()
{
    // Create 100 `Pig_Sty` object.
    for (int i = 0; i < PigStyAmount; i++)
    {
        pig_sty[i] = new PigSty(QString::number(i));
    }

    // Let pigs grow once a seconds.
    connect(game_timer, GameTimer::timeout, this, [ = ]()
    {
        for (int i = 0; i < PigStyAmount; i++)
        {
            pig_sty[i] -> LetAllPigGrow();
        }
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

    connect(game_timer, GameTimer::InfectionOccur, this, [ = ]()
    {
        srand(time(0));
        pig_sty[rand() % 100] -> InfectOnePig();
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

    connect(this, SendStyData, this, []()
    {
        qDebug() << "SendStyData";
    });

}
QString PigStyManager::GetID(const int &sty_num)
{
    return pig_sty[sty_num] -> GetID();
}

void PigStyManager::GetStyData(const int &sty_num)
{
    pig_sty[sty_num] -> GetStyData();
}

int PigStyManager::Random()
{
    static unsigned int offset = 0;
    srand(time(0));
    offset = offset + rand() * rand();
    srand(offset);

    return rand();
}
