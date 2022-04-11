/* Initial window of the game. */

#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>

#include "GameMainWindow.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class HomeWindow;
}
QT_END_NAMESPACE

class HomeWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::HomeWindow *ui;

public:
    static const int WindowWidth;
    static const int WindowHeight;
    static const QString VertionNumber;

    HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();

signals:
    Start_button_clicked();

};
#endif // HOMEWINDOW_H
