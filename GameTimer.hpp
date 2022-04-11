/* A encapsulated class to record the time and emit signal once a second. */

#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <QTimer>

class GameTimer : public QTimer
{
    Q_OBJECT
private:
    QTimer * timer;
    long total_time;    // Record the total time of the game.
public:
    explicit GameTimer(QObject *parent = nullptr);
    static const int DayInterval;
    long GetTime();     // Return the total time now.
signals:
    Timeout_3Month();
};
extern GameTimer *game_timer;

#endif // GAMETIMER_H
