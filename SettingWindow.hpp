#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class SettingsWindow;
}

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

private:
    Ui::SettingsWindow *ui;
};

#endif // SETTINGWINDOW_H
// TODO set volume;
// TODO auther information;
// TODO set update_enable;
