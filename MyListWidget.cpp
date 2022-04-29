#include "MyListWidget.hpp"

MyListWidget::MyListWidget(QWidget *parent)
    : QListWidget{parent}
{

}

void MyListWidget::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event);
    // Click on the blank to cancel all selected items.
    auto index = this -> indexAt(event -> pos());

    if (!index.isValid())
    {
        this -> clearSelection();
    }

    emit ItemSelected((this -> selectedItems()).length() > 0);
}
