#ifndef PIGSTYMANAGER_HPP
#define PIGSTYMANAGER_HPP

#include <QObject>
#include "GameTimer.hpp"
#include "PigSty.hpp"

class PigStyManager : public QObject
{

    Q_OBJECT
public:
    static const int PigStyAmount = 100;
    static const int PigStyPerRow;
    static const int PigStyPerColumn;
private:
    PigSty * pig_sty[PigStyAmount];
public:
    PigStyManager(QObject * parent = nullptr);
    QString GetID(const int &sty_num);
    void GetStyData(const int &sty_num);
    void Create100Sty();
    int Random();
signals:
    SellPigFinished();
    SendStyData(const QVector<Pig::PigInfo> &sty_data);
};
extern PigStyManager * pig_sty_manager;
#endif // PIGSTYMANAGER_HPP
