#include "PushButtonToSty.hpp"
#include <QDebug>

const int ButtonWidth = 60;
const int ButtonHeight = 25;
PushButtonToSty::PushButtonToSty(QWidget *parent)
    : QPushButton{parent}
{

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
