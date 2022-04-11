#include "HomeWindow.hpp"
#include "ui_homewindow.h"
#include <QLabel>
#include <QDebug>
#include <QFont>
#include <QStatusBar>

const int HomeWindow::WindowWidth = 1024;
const int HomeWindow::WindowHeight = 576;
const QString HomeWindow::VertionNumber = "1.0";

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
    ui -> PigFace_label -> setScaledContents(true);
    ui -> PigFace_label -> setPixmap(Pigimage);
    ui -> PigFace_label -> resize(100, 100);
//    ui -> Title_Welcome_label -> setFont(QFont("Minecraft", 36));
//    ui -> StartGame_pushButton -> setStyleSheet("background: gray");

    // Set the statusbar
    QStatusBar * MyStatusBar = new QStatusBar(this);
    this -> setStatusBar(MyStatusBar);

    // Set the Version label
    QLabel * Version_label = new QLabel("Vertion " + VertionNumber, this);
    Version_label -> setFont(QFont("Minecraft", 8));
    MyStatusBar -> addWidget(Version_label);

    // Connect the function of the button.
    connect(ui -> StartGame_pushButton, QPushButton::clicked, this, [ = ]()
    {
        this -> hide();
        emit Start_button_clicked();
    });
    connect(ui -> Quit_pushButton, QPushButton::clicked, this, close);

}

HomeWindow::~HomeWindow()
{
    delete ui;
}

