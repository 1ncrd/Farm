#include "HomeWindow.hpp"
#include "ui_homewindow.h"
#include <QLabel>
#include <QDebug>
#include <QFont>
#include <QStatusBar>
#include <QMessageBox>
#include "BgmPlayer.hpp"
#include "GameMainWindow.hpp"
#include "SettingWindow.hpp"

const int HomeWindow::WindowWidth = 1024;
const int HomeWindow::WindowHeight = 576;
const QString HomeWindow::VertionNumber = "2.0";

HomeWindow::HomeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HomeWindow)
{
    ui->setupUi(this);

    FileManager::ClearRecordFile(QString("Record_1.dat"));
    // Set window's title and icon.
    this -> setFixedSize(WindowWidth, WindowHeight);
    this -> setWindowIcon(QIcon(":/Resources/Picture/PigFace.png"));
    this -> setWindowTitle("The Pig Farm");

    QPixmap Pigimage;
    // Get the pig picture and set up the logo.
    Pigimage.load(":/Resources/Picture/PigFace.png");
    ui -> label_pig_logo -> setScaledContents(true);
    ui -> label_pig_logo -> setPixmap(Pigimage);
    ui -> label_pig_logo -> resize(100, 100);
    ui -> label_title_welcome -> setFont(QFont("Minecraft", 36));
    ui -> pushButton_start_game -> setFont(QFont("Minecraft", 15));
    ui -> pushButton_setting -> setFont(QFont("Minecraft", 15));
    ui -> pushButton_quit -> setFont(QFont("Minecraft", 15));
    ui -> pushButton_introduction -> setFont(QFont("Minecraft", 10));
    ui -> pushButton_about -> setFont(QFont("Minecraft", 10));
//    ui -> StartGame_pushButton -> setStyleSheet("background: gray");

    // Set the statusbar
    QStatusBar * MyStatusBar = new QStatusBar(this);
    this -> setStatusBar(MyStatusBar);

    // Set the Version label
    QLabel * Version_label = new QLabel("Vertion " + VertionNumber, this);
    Version_label -> setFont(QFont("Minecraft", 8));
    MyStatusBar -> addWidget(Version_label);

    // Connect the function of the button.
    connect(ui -> pushButton_start_game, QPushButton::clicked, this, [ = ]()
    {
        this -> hide();
        emit OnStartButtonClicked();
    });
    connect(ui -> pushButton_setting, QPushButton::clicked, this, [ = ]()
    {
        emit OnSettingButtonClicked();
    });
    connect(ui -> pushButton_quit, QPushButton::clicked, this, close);
    connect(ui -> pushButton_about, QPushButton::clicked, this, [ = ]()
    {
        QString message("<p><strong>The Pig Farm</strong></p><p>Based on Qt 6.2.2. </p>"
                        "<p>Open source URL:</p>"
                        "<p><a href=https://github.com/1ncrd/Farm>https://github.com/1ncrd/Farm</a></p>");
        QMessageBox::about(this, QString("About"), message);
    });

}

HomeWindow::~HomeWindow()
{
    delete ui;
}

