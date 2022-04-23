#include "SettingWindow.hpp"
#include "ui_settingwindow.h"
#include "BgmPlayer.hpp"

const int SettingWindow::WindowWidth = 512;
const int SettingWindow::WindowHeight = 576;

SettingWindow::SettingWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingWindow)
{
    ui->setupUi(this);
    this -> setAttribute(Qt::WA_ShowModal, true);
    setFixedSize(WindowWidth, WindowHeight);
    this -> ConfigueVolumeControl();
}

SettingWindow::~SettingWindow()
{
    delete ui;
}

void SettingWindow::ConfigueVolumeControl()
{
    ui -> horizontalSlider_volume -> setMaximum(101);
    ui -> horizontalSlider_volume -> setMinimum(-1);
    ui -> horizontalSlider_volume -> setTracking(true);
    connect(ui -> horizontalSlider_volume, QAbstractSlider::sliderMoved, this, [ = ]()
    {
        ui -> label_volume_value -> setText(QString::number(ui -> horizontalSlider_volume -> value()));
    });
    connect(ui -> horizontalSlider_volume, QAbstractSlider::releaseMouse, this, [ = ]()
    {
        BGM_player -> SetVolume(ui -> horizontalSlider_volume -> value());
    });
}
