#include "PushButtonToSty.hpp"
#include <QDebug>

const int ButtonWidth = 60;
const int ButtonHeight = 25;
PushButtonToSty::PushButtonToSty(QWidget *parent)
    : QPushButton{parent}
{
    this -> setStyleSheet("background-color: rgba(173, 255, 47, 255)");

    effect.setSource(QUrl::fromLocalFile("./Resources/Sound/button_click.wav"));
    effect.setLoopCount(1);
    effect.setVolume(0.5);
    connect(this, QPushButton::clicked, &effect, QSoundEffect::play);
}

PushButtonToSty::~PushButtonToSty()
{
//    qDebug() << "PushButton_EnterSty released." << btn_entersty_id;
}

void PushButtonToSty::SetID(const QString &ID_temp)
{
    if (ID_temp.length() < 2)
    {
        btn_entersty_id = QString("0") + ID_temp;
    }
    else
    {
        btn_entersty_id = ID_temp;
    }
}

QString PushButtonToSty::GetID()
{
    return btn_entersty_id;
}

void PushButtonToSty::ChangeInfectedColor(bool is_infected)
{
    if (is_infected)
    {
        // Turn to red.
        this -> setStyleSheet("background-color: rgba(255, 182, 193, 255)");
    }
    else
    {
        // Turn to green.
        this -> setStyleSheet("background-color: rgba(173, 255, 47, 255)");
    }
}

void PushButtonToSty::InfectionClear()
{
    this -> setStyleSheet("background-color: rgba(173, 255, 47, 255)");
}
