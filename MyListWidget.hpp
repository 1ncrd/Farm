#ifndef MYLISTWIDGET_HPP
#define MYLISTWIDGET_HPP

#include <QObject>
#include <QListWidget>
#include <QMouseEvent>

class MyListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MyListWidget(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event);
signals:
    ItemSelected(bool selected);
};

#endif // MYLISTWIDGET_HPP
