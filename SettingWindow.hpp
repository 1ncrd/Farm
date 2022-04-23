#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class SettingWindow;
}

class SettingWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const int WindowWidth;
    static const int WindowHeight;

    explicit SettingWindow(QWidget *parent = nullptr);
    ~SettingWindow();
    void ConfigueVolumeControl();

private:
    Ui::SettingWindow *ui;

signals:
    VolumeChanged(int volume_temp);
};

#endif // SETTINGWINDOW_H
// TODO set volume;
// TODO auther information;
// TODO set update_enable;
