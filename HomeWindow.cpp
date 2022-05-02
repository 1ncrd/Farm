#include "HomeWindow.hpp"
#include "ui_homewindow.h"
#include <QLabel>
#include <QDebug>
#include <QFont>
#include <QStatusBar>
#include <QMessageBox>
#include <QThread>
#include "BgmPlayer.hpp"
#include "GameMainWindow.hpp"
#include "SettingWindow.hpp"
#include "GameInfo.hpp"

const int HomeWindow::WindowWidth = 1024;
const int HomeWindow::WindowHeight = 576;
const QString HomeWindow::VertionNumber = "3.0";

HomeWindow::HomeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HomeWindow)
{
    ui->setupUi(this);

    // Set window's title and icon.
    this -> setFixedSize(WindowWidth, WindowHeight);
    this -> setWindowIcon(QIcon(":/Resources/Picture/PigFace.png"));
    this -> setWindowTitle("The Pig Farm");

    ConfigueUI();
    // Connect the function of the button.
    Configue_pushButton_start_game();
    Configue_pushButton_setting();
    Configue_pushButton_quit();
    Configue_pushButton_introduction();
    Configue_pushButton_about();

    // *********************************************************************
    // The task about data processing would be put into `thread_to_process`.
    // The main thread should only process the task about UI display
    // to avoid interface lag.
    // *********************************************************************

    // Start `thread_to_process`.
    // 使用多线程处理数据计算和文件操作。
    QThread * thread_to_process_pig = new QThread;
    QThread * thread_to_process_file = new QThread;
    thread_to_process_pig -> start();
    thread_to_process_file -> start();
    thread_to_process_pig -> setPriority(QThread::TimeCriticalPriority);
    thread_to_process_file -> setPriority(QThread::TimeCriticalPriority);

    pig_sty_manager -> moveToThread(thread_to_process_pig);
    file_manager -> moveToThread(thread_to_process_file);

    connect(this, &HomeWindow::destroyed, this, [ = ]()
    {
        // Release the `thread_to_process` safely.
        thread_to_process_pig -> quit();
        thread_to_process_file -> quit();
        thread_to_process_pig -> wait();
        thread_to_process_file -> wait();
        thread_to_process_pig -> deleteLater();
        thread_to_process_file -> deleteLater();
    });
}

HomeWindow::~HomeWindow()
{
    delete ui;
}

void HomeWindow::ConfigueUI()
{
    QPixmap Pigimage;
    // Get the pig picture and set up the logo.
    Pigimage.load(":/Resources/Picture/PigFace.png");
    ui -> label_pig_logo -> setScaledContents(true);
    ui -> label_pig_logo -> setPixmap(Pigimage);
    ui -> label_pig_logo -> resize(100, 100);
    ui -> label_title_welcome -> setFont(QFont("Minecraft", 36));
    ui -> pushButton_start_game -> setFont(QFont("Minecraft", 16));
    ui -> pushButton_setting -> setFont(QFont("Minecraft", 16));
    ui -> pushButton_quit -> setFont(QFont("Minecraft", 16));
    ui -> pushButton_introduction -> setFont(QFont("Minecraft", 11));
    ui -> pushButton_about -> setFont(QFont("Minecraft", 11));
//    ui -> StartGame_pushButton -> setStyleSheet("background: gray");

    // Set the Version label
    QLabel * Version_label = new QLabel("Vertion " + VertionNumber, this);
    Version_label -> setFont(QFont("Minecraft", 10));
    Version_label -> adjustSize();
    Version_label -> move(0, WindowHeight - 20);
    Version_label -> setStyleSheet("color: rgba(255,255,255,200);");
}

void HomeWindow::Configue_pushButton_start_game()
{
    connect(ui -> pushButton_start_game, QPushButton::clicked, this, [ = ]()
    {
        this -> hide();
        emit OnStartButtonClicked();
    });
}

void HomeWindow::Configue_pushButton_setting()
{
    connect(ui -> pushButton_setting, QPushButton::clicked, this, [ = ]()
    {
        emit OnSettingButtonClicked();
    });
}

void HomeWindow::Configue_pushButton_quit()
{
    connect(ui -> pushButton_quit, QPushButton::clicked, this, close);
}

void HomeWindow::Configue_pushButton_introduction()
{
    connect(ui -> pushButton_introduction, QPushButton::clicked, this, [ = ]()
    {
        QMessageBox::about(this, QString("Guidance"), message_guidance);
    });
}
void HomeWindow::Configue_pushButton_about()
{
    connect(ui -> pushButton_about, QPushButton::clicked, this, [ = ]()
    {
        QMessageBox::about(this, QString("About"), message_about);
    });
}
