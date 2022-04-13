#include "SettingWindow.hpp"
#include "ui_settingwindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    setFixedSize(1024, 576);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}
