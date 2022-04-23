#ifndef QUARANTINEPIGSTY_HPP
#define QUARANTINEPIGSTY_HPP

#include "PigSty.hpp"

class QuarantinePigSty : public PigSty
{

    Q_OBJECT
public:
    explicit QuarantinePigSty(const QString &sty_id_temp, QObject *parent = nullptr);
    void GetQuarantineStyData();
    void DisposeInfectedPig();
signals:
    ReturnQuarantineStyData(QVector<Pig::PigInfo> data);
};

#endif // QUARANTINEPIGSTY_HPP
