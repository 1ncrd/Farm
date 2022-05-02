#include "CreateWindow.hpp"
#include "ui_CreateWindow.h"
#include <QStyledItemDelegate>
#include <QListWidget>
#include <QStandardItemModel>
#include <QMessageBox>
#include "FileManager.hpp"

const int CreateWindow::WindowWidth = 512;
const int CreateWindow::WindowHeight = 576;

CreateWindow::CreateWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CreateWindow)
{
    ui->setupUi(this);

    this -> setFixedSize(WindowWidth, WindowHeight);
    this -> setWindowTitle("The Pig Farm");
    this -> setWindowIcon(QIcon(":/Resources/Picture/PigFace.png"));
    this -> setAttribute(Qt::WA_ShowModal, true);

    ui -> lineEdit_name -> setFont(QFont("Minecraft", 12));
    ui -> label_title -> setFont(QFont("Minecraft", 19));
    ui -> label_name -> setFont(QFont("Minecraft", 12));
    ui -> label_difficulty -> setFont(QFont("Minecraft", 12));
    ui -> pushButton_create -> setFont(QFont("Minecraft", 12));
    ui -> pushButton_abort -> setFont(QFont("Minecraft", 12));
    ConfigueComboBox();
    ConfigueButton();
}

CreateWindow::~CreateWindow()
{
    delete ui;
}

void CreateWindow::Start()
{
    SetDefaults();
    this -> show();
}

void CreateWindow::SetDefaults()
{
    // Set defaults.
    ui -> lineEdit_name -> setText(QString("My Farm"));
    ui -> comboBox_difficulty -> setCurrentIndex(0);
}

void CreateWindow::ConfigueComboBox()
{
    for (int i = 0; i < ui -> comboBox_difficulty -> count(); ++i)
    {
        // Set the content of the combobox to be horizontal center.
        static_cast<QStandardItemModel*>(ui -> comboBox_difficulty -> model())
        -> item(i) -> setTextAlignment(Qt::AlignCenter);
    }

    QString str = "QComboBox QAbstractItemView::item{\
                    font-size:12px;\
                    min-height:40px;\
                    min-width:20px;\
                    }";

    ui -> comboBox_difficulty -> setStyleSheet(str);
}

void CreateWindow::ConfigueButton()
{
    // Press the `pushButton_abort` to quit creating.
    connect(ui -> pushButton_abort, QPushButton::clicked, this, close);
    connect(ui -> pushButton_create, QPushButton::clicked, this, [ = ]()
    {
        QString archive_name(ui -> lineEdit_name -> text());

        if (current_existing_archive.contains(archive_name))
        {
            QMessageBox::warning(this, QString("Create fail"), QString("<font face = Minecraft size = 3>The archive name already exists!</font>"));
        }
        else
        {
            FileManager::CreateArchiveFile(archive_name);
            FileManager::CreateSaleRecordFile(archive_name);
            FileManager::GameData game_data;
            game_data.never_be_edited = true;
            game_data.total_time = 0;
            QDateTime current_date_time = QDateTime::currentDateTime();
            game_data.edit_time = current_date_time.toString("yyyy.MM.dd hh:mm:ss ddd");
            game_data.difficulty = ui -> comboBox_difficulty -> currentIndex();
            FileManager::WriteArchiveData(game_data, archive_name);
            emit file_manager -> NewArchiveCreated();
            this -> close();
        }
    });
}

void CreateWindow::SetCurrntExsistingArchiveList(const QVector<QString> &current_existing_archive)
{
    this -> current_existing_archive = current_existing_archive;
}
