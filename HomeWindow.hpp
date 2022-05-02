/* Initial window of the game. */

#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>

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
    void ConfigueUI();
    void Configue_pushButton_start_game();
    void Configue_pushButton_setting();
    void Configue_pushButton_quit();
    void Configue_pushButton_introduction();
    void Configue_pushButton_about();

signals:
    OnStartButtonClicked();
    OnSettingButtonClicked();
};
#endif // HOMEWINDOW_H
