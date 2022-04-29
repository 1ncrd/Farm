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

    // Set the statusbar
    QStatusBar * MyStatusBar = new QStatusBar(this);
    this -> setStatusBar(MyStatusBar);

    // Set the Version label
    QLabel * Version_label = new QLabel("Vertion " + VertionNumber, this);
    Version_label -> setFont(QFont("Minecraft", 10));
    MyStatusBar -> addWidget(Version_label);
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

// TODO Stroe in a file and use in the game.
const QString HomeWindow::message_guidance("<p><strong><font face=Minecraft size=4>Guidance<\font></strong></p>"
        "<p>You are the owner of this pig farm.</p>"
        "<p>A second in the game is a day.</p>"
        "<p>There are 100 sty in all.</p>"
        "<p>There are three breeds of pigs in the game.</p>"
        "<p>Healthy pigs grow every day.</p>"
        "<p>Every three months farms automatically sell pigs weighing more than 75kg or older than a year.</p>"
        "<hr/>"
        "<table><tr><td><strong>Species<\strong></td><td><strong>Price<\strong>/kg</td></tr><tr><td>BlackPig</td><td>7.5</td></tr><tr><td>SmallFlowerPig</td><td>3.5</td></tr><tr><td>BigWhitePig</td><td>3</td></tr></table>");
const QString HomeWindow::message_about("<p><strong><font face=Minecraft size=4>The Pig Farm 3.0<\font></strong></p>"
                                        "<p>Based on Qt 6.2.2</p>"
                                        "<p><\p>"
                                        "<p>Built on April 2022<\p>"
                                        "<p><\p>"
                                        "<p>Author: Incrd<\p>"
                                        "<p><\p>"
                                        //"<p>Open source URL: <a href=https://github.com/1ncrd/Farm>https://github.com/1ncrd/Farm</a></p>"
                                        "<p></p>");
void HomeWindow::Configue_pushButton_introduction()
{
    connect(ui -> pushButton_introduction, QPushButton::clicked, this, [ = ]()
    {
        QMessageBox::information(this, QString("Guidance"), message_guidance);
    });
}
void HomeWindow::Configue_pushButton_about()
{
    connect(ui -> pushButton_about, QPushButton::clicked, this, [ = ]()
    {
        QMessageBox::about(this, QString("About"), message_about);
    });
}
