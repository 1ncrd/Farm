#ifndef PUSHBUTTONTOSTY_H
#define PUSHBUTTONTOSTY_H

#include <QPushButton>
#include <QString>

class PushButtonToSty : public QPushButton
{
    Q_OBJECT
private:
    QString btn_entersty_id;

public:
    explicit PushButtonToSty(QWidget *parent = nullptr);

    static const int ButtonWidth;
    static const int ButtonHeight;
    PushButtonToSty();
    ~PushButtonToSty();

    void SetID(const QString &ID_temp);
    QString GetID();
    void ChangeInfectedColor(bool is_infected);
    void InfectionClear();
signals:
    CheckStyIsInfected(const int &sty_num);
};

#endif // PUSHBUTTONTOSTY_H
