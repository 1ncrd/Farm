#include "GameTimer.hpp"
#include <QTimer>
#include <QDebug>

GameTimer *game_timer = new GameTimer;

const int GameTimer::DayInterval = 100;
GameTimer::GameTimer(QObject *parent)
    : QTimer{parent}
{
    total_time = 0;
    this -> setInterval(DayInterval);
    connect(this, QTimer::timeout, this, [&]()
    {
        if (total_time % 90 == 0 and total_time != 0)
        {
            emit Timeout_3Month();
        }

        total_time++;
    });
}

long GameTimer::GetTime()
{
    return total_time;
}


