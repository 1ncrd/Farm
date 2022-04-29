#include "GameMainWindow.hpp"
#include "ui_gamemainwindow.h"
#include <QDebug>
#include <QScrollBar>
#include <QLabel>
#include <QMessageBox>
#include "BgmPlayer.hpp"

const int GameMainWindow::WindowWidth = 1024;
const int GameMainWindow::WindowHeight = 576;

std::map<int, int> GameMainWindow::DifficultyToInfectionPercent =
{
    {0, 1}, {1, 3}, {2, 10}
};

GameMainWindow::GameMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameMainWindow)
{
    ui -> setupUi(this);
    // Set window's size, title and icon.
    this -> setFixedSize(WindowWidth, WindowHeight);
    this -> setWindowTitle("The Pig Farm");
    this -> setWindowIcon(QIcon(":/Resources/Picture/PigFace.png"));


    // ***************************************************************************
    // Create first and then connect them to enssure the object to connect exists.
    // ***************************************************************************
    // Create widget.
    this -> Create_label_date();
    this -> Create_label_money();
    this -> Create_label_infection_status();
    this -> Create_label_pig_sold_amount();
    this -> Create_area_choose_sty_with_btn();
    this -> Create_sty_detail_window();
    this -> Create_quarantine_sty_window();
    this -> Create_trade_record_window();
    this -> Create_button_show_trade_record_window();
    this -> Create_button_quarantine_pig();
    this -> Create_button_show_quarantine_sty();

    // Read the pig data after all the process about pig being finished.
    this -> Connect_label_date();
    this -> Connect_label_money();
    this -> Connect_label_pig_sold_amount();
    this -> Connect_label_infection_status();
    this -> Connect_button_to_Sty_Detail();
    this -> Connect_button_quarantine_pig();
    this -> Connect_button_show_trade_record_window();
    this -> Connect_button_show_quarantine_sty();

    // Without the "Sty_Detail_Window_PreLoad()",
    // there will be a bit of lag to show the `sty_window` when the user clicks the pushbutton_entersty first time for some unknown reason.
    this -> ConfigueArchiveStore();
}

GameMainWindow::~GameMainWindow()
{
    // Stand for the game finished.
    PigSty::DeleteInstance();
    delete pig_sty_manager;
    delete file_manager;
    delete game_timer;
    delete BGM_player;
    delete ui;
}

void GameMainWindow::StartGame(QString file_name)
{
    // Start the game.
    FileManager::GameData game_data = file_manager -> ReadArchiveData(file_name);

    difficulty = game_data.difficulty;
    archive_name = game_data.name;
    pig_sty_manager -> SetArchiveName(game_data.name);
    file_manager -> SetCurrentArchiveName(game_data.name);

    if (game_data.never_be_edited)
    {
        pig_sty_manager -> StartTheFarm();
        this -> Sty_Detail_Window_PreLoad();
        pig_sty_manager -> SetInfectionPosibility(DifficultyToInfectionPercent[game_data.difficulty]);
    }
    else
    {
        game_timer -> SetTime(game_data.total_time);
        pig_sty_manager -> StartTheFarm(game_data);
        this -> Sty_Detail_Window_PreLoad();
        pig_sty_manager -> SetInfectionPosibility(DifficultyToInfectionPercent[game_data.difficulty]);
    }

    game_timer -> start();
    this -> show();
}

void GameMainWindow::Create_label_date()
{
    // Create a `QLabel` to display the *date*.
    this -> label_date = new QLabel(this);
    label_date -> move(15, 15);
    label_date -> setFont(QFont("Minecraft", 15));
    label_date -> setText(QString("Day:\t0"));
    label_date -> adjustSize();
    label_date -> show();
}

void GameMainWindow::Create_label_money()
{
    // Create a `QLabel` object to display the *money*.
    this -> label_money = new QLabel(this);
    label_money -> move(15, 40);
    label_money -> setFont(QFont("Minecraft", 15));
    label_money -> setText(QString("Money:\t0"));
    label_money -> adjustSize();
    label_money -> show();
}

void GameMainWindow::Create_label_pig_sold_amount()
{
    // Create a `QLabel` object to display the *amount of pig sold*.
    this -> label_pig_sold_amount = new QLabel(this);
    label_pig_sold_amount -> move(15, 75);
    label_pig_sold_amount -> setFont(QFont("Minecraft", 11));
    label_pig_sold_amount -> setText(QString("Amount of pig sold:"
                                     "\n\nBlackPig:\t\t0"
                                     "\n\nSmallFlowerPig:\t0"
                                     "\n\nBigWhitePig:\t0"
                                     "\n\nSum:\t\t0"));
    label_pig_sold_amount -> adjustSize();
    label_pig_sold_amount -> show();
}

void GameMainWindow::Create_label_infection_status()
{
    this -> label_infection_status = new QLabel(this);
    label_infection_status -> move(WindowWidth - 230, 25);
    label_infection_status -> setFont(QFont("Minecraft", 15));
    label_infection_status -> setStyleSheet("QLabel{color:rgb(0,205,0);}");
    label_infection_status -> setText(QString("No Infections."));
    label_infection_status -> adjustSize();
    label_infection_status -> show();
}
void GameMainWindow::Create_area_choose_sty_with_btn()
{
    // Create a `QScrollArea` object to display the *100-sty choice*.
    this -> scroll_area_choose_sty = new QScrollArea(this);
    scroll_area_choose_sty -> setGeometry(WindowWidth / 4, 0, WindowWidth / 2, WindowHeight - 171);
    scroll_area_choose_sty -> setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll_area_choose_sty -> setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll_area_choose_sty -> verticalScrollBar() -> setDisabled(true);
    scroll_area_choose_sty -> setWidgetResizable(false);

    // Set a `QWidget` object in the `scroll_area_choose_sty` to display the 100 buttons.
    area_choose_sty = new QWidget(scroll_area_choose_sty);
    const int area_choose_sty_width = WindowWidth - 20;
    const int area_choose_sty_height = WindowHeight - 170;

    area_choose_sty -> setGeometry(0, 0, WindowWidth - 20, WindowHeight - 170);
    scroll_area_choose_sty -> setWidget(area_choose_sty);

    // Create 100 `PushButton_EnterSty` object and place them into the `scroll_area_choose_sty`.

    for (int i = 0; i < PigStyManager::PigStyPerColumn; i++)
    {
        for (int j = 0; j < PigStyManager::PigStyPerRow; j++)
        {
            const int W = 60;
            const int H = 25;
            const int margin_x = 20;
            const int margin_y = 10;
            // Create 100 `PushButton_EnterSty` object.
            button_show_normal_sty[i * 10 + j] = new PushButtonToSty(area_choose_sty);

            // Set the text content.
            button_show_normal_sty[i * 10 + j] -> SetID(QString::number(i * 10 + j));
            button_show_normal_sty[i * 10 + j] -> setText(button_show_normal_sty[i * 10 + j] -> GetID());
            button_show_normal_sty[i * 10 + j] -> setFont(QFont("Minecraft", 12));

            // Set the position and the sizes.
            button_show_normal_sty[i * 10 + j] ->
            setGeometry(margin_x + j * (area_choose_sty_width - margin_x * 2 - W) / 9,
                        margin_y + i * (area_choose_sty_height - margin_y * 2 - H - 15) / 9,
                        W, H);
        }
    }
}
void GameMainWindow::Create_sty_detail_window()
{
    // Create a `Sty_Detail_Window` object.
    sty_detail_window = new Sty_Detail_Window(this);
}

void GameMainWindow::Create_trade_record_window()
{
    // Create a `TradeRecord_Window` object.
    trade_record_window = new TradeRecordWindow(this);
}

void GameMainWindow::Create_button_show_trade_record_window()
{
    // Create a `QPushButton` object.
    button_show_trade_record_window = new QPushButton(this);
    button_show_trade_record_window -> move(15, WindowHeight - 55);
    button_show_trade_record_window -> resize(150, 40);
    button_show_trade_record_window -> setText(QString("Trade Record"));
    button_show_trade_record_window -> setFont(QFont("Minecraft", 12));
}

void GameMainWindow::Create_button_quarantine_pig()
{
    button_quarantine_pig = new QPushButton(this);
    button_quarantine_pig -> move(WindowWidth - 235, 60);
    button_quarantine_pig -> resize(150, 35);
    button_quarantine_pig -> setText(QString("Quarantine"));
    button_quarantine_pig -> setFont(QFont("Minecraft", 12));
}

void GameMainWindow::Create_button_show_quarantine_sty()
{
    button_show_quarantine_sty = new QPushButton(this);
    button_show_quarantine_sty -> move(WindowWidth - 235, 110);
    button_show_quarantine_sty -> resize(190, 35);
    button_show_quarantine_sty -> setText(QString("Quarantine Sty"));
    button_show_quarantine_sty -> setFont(QFont("Minecraft", 12));
}

void GameMainWindow::Create_quarantine_sty_window()
{
    quarantine_sty_window = new QuarantineStyWindow(this);
}

void GameMainWindow::Connect_label_date()
{
    auto UpdateDisplay = [ = ]()
    {
        label_date -> setText(QString("Day:\t") + QString::number(game_timer -> GetTime()));
        label_date -> adjustSize();
    };
    connect(game_timer, GameTimer::timeout, this, UpdateDisplay);
    connect(game_timer, GameTimer::TimeUpdate, this, UpdateDisplay);
}

void GameMainWindow::Connect_label_money()
{
    // Update the `label_money` after the last `pig_sty` finished the sale.
    connect(PigSty::GetInstance(), PigSty::MoneyUpdate, this, [ = ]()
    {
        label_money -> setText(QString("Money:\t") + QString::number(PigSty::money));
        label_money -> adjustSize();
    });
}

void GameMainWindow::Connect_label_pig_sold_amount()
{
    // Update the `label_pig_sold_amount` after the last `pig_sty` finished the sale.
    connect(PigSty::GetInstance(), PigSty::SoldAmountUpdate, this, [ = ]()
    {
        label_pig_sold_amount -> setText(QString("Amount of pig sold:") +
                                         QString("\n\nBlackPig:\t\t") + QString::number(PigSty::pig_sold_amount.BlackPig) +
                                         QString("\n\nSmallFlowerPig:\t") + QString::number(PigSty::pig_sold_amount.SmallFlowerPig) +
                                         QString("\n\nBigWhitePig:\t") + QString::number(PigSty::pig_sold_amount.BigWhitePig) +
                                         QString("\n\nSum:\t\t") + QString::number(PigSty::pig_sold_amount.total));
        label_pig_sold_amount -> adjustSize();
    });
}

void GameMainWindow::Connect_label_infection_status()
{
    connect(pig_sty_manager, PigStyManager::InfectionExists, this, [ = ](bool infection_exists)
    {

        if (infection_exists)
        {
            // Turn to red.
            this -> label_infection_status -> setStyleSheet("QLabel{color:rgb(255,0,0);}");
            this -> label_infection_status -> setText(QString("Outbreak!"));
            this -> label_infection_status -> adjustSize();
        }
        else
        {
            // Turn to green.
            this -> label_infection_status -> setStyleSheet("QLabel{color:rgb(0,205,0);}");
            this -> label_infection_status -> setText(QString("No Infections."));
            this -> label_infection_status -> adjustSize();
        }
    });
}

void GameMainWindow::Connect_button_to_Sty_Detail()
{
    // Press the ptr_btn_entersty to start the `sty_window`.
    for (int i = 0; i < PigStyManager::PigStyAmount; i++)
    {
        // Let the `sty_window` accept the data from `pig_sty`.

        connect(button_show_normal_sty[i], PushButtonToSty::clicked, this, [ = ]()
        {
            sty_detail_window -> Start(i);
        });
    }

    connect(pig_sty_manager, PigStyManager::StyIsInfected, this, [ = ](int sty_num, bool is_infected)
    {
        button_show_normal_sty[sty_num] -> ChangeInfectedColor(is_infected);
    });
}

void GameMainWindow::Connect_button_show_trade_record_window()
{
    connect(button_show_trade_record_window, QPushButton::clicked, trade_record_window, TradeRecordWindow::Start);
}

void GameMainWindow::Connect_button_quarantine_pig()
{
    connect(button_quarantine_pig, QPushButton::clicked, pig_sty_manager, PigStyManager::QuarantineAllInfectedPig);

    for (int i = 0; i < PigStyManager::PigStyAmount; i++)
    {
        connect(button_quarantine_pig, QPushButton::clicked, button_show_normal_sty[i], PushButtonToSty::InfectionClear);
    }

    connect(button_quarantine_pig, QPushButton::clicked, this, [ = ]()
    {
        this -> label_infection_status -> setStyleSheet("QLabel{color:rgb(0,205,0);}");
        this -> label_infection_status -> setText(QString("No Infections."));
        this -> label_infection_status -> adjustSize();
    });
}

void GameMainWindow::Connect_button_show_quarantine_sty()
{
    connect(button_show_quarantine_sty, QPushButton::clicked, quarantine_sty_window, QuarantineStyWindow::Start);
}

void GameMainWindow::Sty_Detail_Window_PreLoad()
{
    sty_detail_window -> Preload(99);
}

void GameMainWindow::ConfigueArchiveStore()
{

}

void GameMainWindow::StoreGameData()
{
    FileManager::GameData game_data;
    game_data.difficulty = difficulty;
    QDateTime current_date_time = QDateTime::currentDateTime();
    game_data.edit_time = current_date_time.toString("yyyy.MM.dd hh:mm:ss ddd");
    game_data.name = archive_name;
    game_data.never_be_edited = false;
    game_data.total_time = game_timer -> GetTime();
    game_data.pigsty_info.money = PigSty::money;
    game_data.pigsty_info.pig_sold_amount.BlackPig = PigSty::pig_sold_amount.BlackPig;
    game_data.pigsty_info.pig_sold_amount.SmallFlowerPig = PigSty::pig_sold_amount.SmallFlowerPig;
    game_data.pigsty_info.pig_sold_amount.BigWhitePig = PigSty::pig_sold_amount.BigWhitePig;
    game_data.pigsty_info.pig_sold_amount.total = PigSty::pig_sold_amount.total;
    game_data.pig_data = pig_sty_manager -> GetAllPigData();
    FileManager::WriteArchiveData(game_data, game_data.name);
}

void GameMainWindow::closeEvent(QCloseEvent *event)
{
    auto choose = QMessageBox::question(this, tr("Quit"),
                                        QString(tr("Save the archive?")),
                                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                        QMessageBox::Save);

    if (choose == QMessageBox::Cancel)
    {
        event -> ignore();
    }
    else if (choose == QMessageBox::Save)
    {
        StoreGameData();
        event -> accept();
    }
    else if (choose == QMessageBox::Discard)
    {
        event -> accept();
    }

}
