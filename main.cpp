#include "HomeWindow.hpp"
#include "GameMainWindow.hpp"
#include "FileManager.hpp"
#include <BgmPlayer.h>
#include <QApplication>
#include <QPushButton>
#include <QObject>
#include <QDebug>
#include <QFont>
#include "TradeRecordWindow.hpp"
#include <QMetaType>

int main(int argc, char *argv[])
{

    qDebug() << qRegisterMetaType<PigSty>("Pig_Sty");
    qDebug() << qRegisterMetaType<PigSty>("Pig_Sty&");
    qDebug() << qRegisterMetaType<PigSty::PigStyData>("PigStyData");
    qDebug() << qRegisterMetaType<PigSty::PigStyData>("PigStyData&");

    QApplication a(argc, argv);

    // Create GameDatas and SalesRecords Folder.
    FileManager::CreateGameDataFolder();
    FileManager::CreateTradeRecordFolder();
    FileManager::CreateSaleRecordFile(QString("Record_1.dat"));

    // Create Initial_Window and Game_Main_Window.
    HomeWindow home_window;
    GameMainWindow Game_window;
    home_window.show();

    // Click the "Start Game" button to show the game window.
    QObject::connect(&home_window, HomeWindow::Start_button_clicked, &Game_window, GameMainWindow::StartGame);


    return a.exec();
}
/*
 *
 *
 */
