#include "HomeWindow.hpp"
#include "FileManager.hpp"
#include "BgmPlayer.hpp"
#include <QApplication>
#include <QObject>
#include <QDebug>
#include "TradeRecordWindow.hpp"
#include <QMetaType>

int main(int argc, char *argv[])
{
    // Register the custom type into the `MetaType` to transport them across the QThreads.
    qRegisterMetaType<PigSty>("Pig_Sty");
    qRegisterMetaType<PigSty>("Pig_Sty&");
    qRegisterMetaType<PigSty::PigStyData>("PigStyData");
    qRegisterMetaType<PigSty::PigStyData>("PigStyData&");
    qRegisterMetaType<Pig::PigInfo>("PigInfo");
    qRegisterMetaType<Pig::PigInfo>("PigInfo&");
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
    QObject::connect(&home_window, HomeWindow::OnStartButtonClicked, &Game_window, GameMainWindow::StartGame);

    return a.exec();
}
