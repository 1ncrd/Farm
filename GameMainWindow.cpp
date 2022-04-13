#include "GameMainWindow.hpp"
#include "ui_gamemainwindow.h"
#include <QDebug>
#include <QScrollBar>
#include <QLabel>

const int GameMainWindow::WindowWidth = 1024;
const int GameMainWindow::WindowHeight = 576;

GameMainWindow::GameMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameMainWindow)
{
    ui -> setupUi(this);
    // Set window's size, title and icon.
    this -> setFixedSize(WindowWidth, WindowHeight);
    this -> setWindowTitle("The Pig Farm");
    this -> setWindowIcon(QIcon(":/Resources/Picture/PigFace.png"));

    // *********************************************************************
    // The task about data processing would be put into `thread_to_process`.
    // The main thread should only process the task about UI display
    // to avoid interface lag.
    // *********************************************************************

    // Start `thread_to_process`.
    this -> thread_to_process = new QThread;
    this -> thread_to_process -> start();
    this -> thread_to_process -> setPriority(QThread::TimeCriticalPriority);

    file_manager -> moveToThread(thread_to_process);
    pig_sty_manager -> moveToThread(thread_to_process);

    // Create widget.
    this -> Create_label_date();
    this -> Create_label_money();
    this -> Create_label_pig_sold_amount();
    this -> Create_area_choose_sty();
    this -> Create_sty_window();
    this -> Create_trade_record_window();
    this -> Create_button_to_trade_record_window();


    // ********************************************************
    // The order of connection is critical.
    // It should determine the order of the function execution.
    // ********************************************************

    // Read the pig data after all the process about pig being finished.
    this -> Connect_label_date();
    this -> Connect_label_money();
    this -> Connect_label_pig_sold_amount();
    this -> Connect_sty_window();
    this -> Connect_trade_record_window();

    // Without the "Sty_Detail_Window_PreLoad()",
    // there will be a bit of lag to show the `sty_window` when the user clicks the pushbutton_entersty first time for some unknown reason.
}

GameMainWindow::~GameMainWindow()
{
    // Release the `thread_to_process` safely.
    this -> thread_to_process -> quit();
    this -> thread_to_process -> wait();
    this -> thread_to_process -> deleteLater();

    delete pig_sty_manager;
    delete file_manager;
    delete game_timer;
    delete ui;
}

void GameMainWindow::StartGame()
{
    // Start the game.
    pig_sty_manager -> Create100Sty();
    this -> Sty_Detail_Window_PreLoad();
    game_timer -> SetInfectionPosibility(0.2);
    game_timer -> start();
    this -> show();

}

void GameMainWindow::Create_label_date()
{
    // Create a `QLabel` to display the *date*.
    this -> label_date = new QLabel(this);
    label_date -> move(10, 10);
    label_date -> setFont(QFont("Minecraft", 14));
    label_date -> setText(QString("Day:\t0"));
    label_date -> adjustSize();
    label_date -> show();
}

void GameMainWindow::Create_label_money()
{
    // Create a `QLabel` object to display the *money*.
    this -> label_money = new QLabel(this);
    label_money -> move(10, 35);
    label_money -> setFont(QFont("Minecraft", 14));
    label_money -> setText(QString("Money:\t0"));
    label_money -> adjustSize();
    label_money -> show();
}

void GameMainWindow::Create_label_pig_sold_amount()
{
    // Create a `QLabel` object to display the *amount of pig sold*.
    this -> label_pig_sold_amount = new QLabel(this);
    label_pig_sold_amount -> move(10, 70);
    label_pig_sold_amount -> setFont(QFont("Minecraft", 10));
    label_pig_sold_amount -> setText(QString("Amount of pig sold:"
                                     "\n\nBlackPig:\t\t0"
                                     "\n\nSmallFlowerPig:\t0"
                                     "\n\nBigWhitePig:\t0"
                                     "\n\nSum:\t\t0"));
    label_pig_sold_amount -> adjustSize();
    label_pig_sold_amount -> show();
}

void GameMainWindow::Create_area_choose_sty()
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
            ptr_btn_entersty[i * 10 + j] = new PushButtonToSty(area_choose_sty);

            // Set the text content.
            ptr_btn_entersty[i * 10 + j] -> SetID(QString::number(i * 10 + j));
            ptr_btn_entersty[i * 10 + j] -> setText(ptr_btn_entersty[i * 10 + j] -> GetID());
            ptr_btn_entersty[i * 10 + j] -> setFont(QFont("Minecraft", 12));

            // Set the position and the sizes.
            ptr_btn_entersty[i * 10 + j] ->
            setGeometry(margin_x + j * (area_choose_sty_width - margin_x * 2 - W) / 9,
                        margin_y + i * (area_choose_sty_height - margin_y * 2 - H - 15) / 9,
                        W, H);
        }
    }
}
void GameMainWindow::Create_sty_window()
{
    // Create a `Sty_Detail_Window` object.
    sty_window = new Sty_Detail_Window(this);
}

void GameMainWindow::Create_trade_record_window()
{
    // Create a `TradeRecord_Window` object.
    trade_record_window = new TradeRecordWindow(this);
}

void GameMainWindow::Create_button_to_trade_record_window()
{
    // Create a `QPushButton` object.
    button_to_trade_record_window = new QPushButton(this);
    button_to_trade_record_window -> move(10, WindowHeight - 40);
    button_to_trade_record_window -> resize(100, 25);
}

void GameMainWindow::Connect_label_date()
{
    connect(game_timer, GameTimer::timeout, this, [ = ]()
    {
        label_date -> setText(QString("Day:\t") + QString::number(game_timer -> GetTime()));
        label_date -> adjustSize();
    });
}

void GameMainWindow::Connect_label_money()
{
    // Update the `label_money` after the last `pig_sty` finished the sale.
    connect(pig_sty_manager, PigStyManager::SellPigFinished, this, [ = ]()
    {
        label_money -> setText(QString("Money:\t") + QString::number(PigSty::money));
        label_money -> adjustSize();
    });
}

void GameMainWindow::Connect_label_pig_sold_amount()
{
    // Update the `label_pig_sold_amount` after the last `pig_sty` finished the sale.
    connect(pig_sty_manager, PigStyManager::SellPigFinished, this, [ = ]()
    {
        label_pig_sold_amount -> setText(QString("Amount of pig sold:") +
                                         QString("\n\nBlackPig:\t\t") + QString::number(PigSty::pig_sold_amount.BlackPig) +
                                         QString("\n\nSmallFlowerPig:\t") + QString::number(PigSty::pig_sold_amount.SmallFlowerPig) +
                                         QString("\n\nBigWhitePig:\t") + QString::number(PigSty::pig_sold_amount.BigWhitePig) +
                                         QString("\n\nSum:\t\t") + QString::number(PigSty::pig_sold_amount.total));
        label_pig_sold_amount -> adjustSize();
    });
}

void GameMainWindow::Connect_sty_window()
{
    // Press the ptr_btn_entersty to start the `sty_window`.
    for (int i = 0; i < PigStyManager::PigStyAmount; i++)
    {
        // Let the `sty_window` accept the data from `pig_sty`.

        connect(ptr_btn_entersty[i], PushButtonToSty::clicked, this, [ = ]()
        {
            sty_window -> Start(i);
        });
    }
}

void GameMainWindow::Connect_trade_record_window()
{
    connect(button_to_trade_record_window, QPushButton::clicked, trade_record_window, TradeRecordWindow::Start);
}

void GameMainWindow::Sty_Detail_Window_PreLoad()
{
    sty_window -> Preload(99);
}
