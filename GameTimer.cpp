#include "GameTimer.hpp"
#include <QDebug>

GameTimer *game_timer = new GameTimer;

const int GameTimer::DayInterval = 100;
GameTimer::GameTimer(QObject *parent)
    : QTimer{parent}
{
    total_time = 0;
    InfectionPosibility = 0;
    this -> setInterval(DayInterval);
    connect(this, QTimer::timeout, this, [&]()
    {
        if ((total_time + 1) % 90 == 0)
        {
            emit Timeout_3Month();
        }

        if (/*total_time > 50 and */(Random() % RAND_MAX) < RAND_MAX * InfectionPosibility)
        {
            emit InfectionOccur();
        }

        total_time++;
    });
}

long GameTimer::GetTime()
{
    return this -> total_time;
}

GameTimer::Date GameTimer::ConvertToDate(int day)
{
    int day_temp = day % 30 + 1;
    int month_temp = (day / 30) % 12 + 1;
    int year_temp = day / 360 + 1;
    Date date_temp{day_temp, month_temp, year_temp};
    return date_temp;
}

void GameTimer::SetInfectionPosibility(const float &posibility)
{
    this -> InfectionPosibility = posibility;
}

int GameTimer::Random()
{
    static unsigned int offset = 0;
    srand(time(0));
    offset = offset + rand() * rand();
    srand(offset);

    return rand();
}
