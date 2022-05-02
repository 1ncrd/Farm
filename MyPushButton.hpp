#ifndef MYPUSHBUTTON_HPP
#define MYPUSHBUTTON_HPP

#include <QPushButton>
#include <QSoundEffect>


class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton(QWidget *parent = nullptr);
private:
    QSoundEffect effect;
signals:

};

#endif // MYPUSHBUTTON_HPP
