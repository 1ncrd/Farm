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

    ui -> label_title -> setFont(QFont("Minecraft", 19));
    ui -> label_volume -> setFont(QFont("Minecraft", 12));
    ui -> label_volume_value -> setFont(QFont("Minecraft", 12));
}

SettingWindow::~SettingWindow()
{
    delete ui;
}

void SettingWindow::ConfigueVolumeControl()
{
    ui -> horizontalSlider_volume -> setMaximum(100);
    ui -> horizontalSlider_volume -> setMinimum(0);
    ui -> horizontalSlider_volume -> setTracking(true);
    connect(ui -> horizontalSlider_volume, QAbstractSlider::sliderMoved, this, [ = ]()
    {
        ui -> label_volume_value -> setText(QString::number(ui -> horizontalSlider_volume -> value()));
    });

    connect(ui -> horizontalSlider_volume, QAbstractSlider::valueChanged, this, [ = ]()
    {
        ui -> label_volume_value -> setText(QString::number(ui -> horizontalSlider_volume -> value()));
    });

    connect(ui -> horizontalSlider_volume, QAbstractSlider::sliderReleased, this, [ = ]()
    {
        BGM_player -> SetVolume(ui -> horizontalSlider_volume -> value());
    });

    connect(ui -> horizontalSlider_volume, QAbstractSlider::valueChanged, this, [ = ]()
    {
        BGM_player -> SetVolume(ui -> horizontalSlider_volume -> value());
    });
}

void SettingWindow::SetVolume(const int &volume)
{
    ui -> horizontalSlider_volume -> setSliderPosition(volume);
}

void SettingWindow::Start()
{
    SetVolume(BGM_player -> GetVolume());
    this -> show();
}

// TODO can set in game.
// TODO Beautifier.
// TODO Introduction in game.
// TODO Maybe a minecraft book icon is good.
