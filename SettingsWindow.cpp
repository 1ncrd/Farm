#include "SettingsWindow.hpp"
#include "ui_settings_window.h"

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
