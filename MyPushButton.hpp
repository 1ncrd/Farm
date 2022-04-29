#ifndef MYPUSHBUTTON_HPP
#define MYPUSHBUTTON_HPP

#include <QObject>

class MyPushButton : public QObject
{
    Q_OBJECT
public:
    explicit MyPushButton(QObject *parent = nullptr);

signals:

};

#endif // MYPUSHBUTTON_HPP
