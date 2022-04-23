#include <QApplication>
#include <QObject>
#include <QDebug>
#include <QMetaType>
#include "HomeWindow.hpp"
#include "GameMainWindow.hpp"
#include "SettingWindow.hpp"
#include "FileManager.hpp"
#include "BgmPlayer.hpp"
#include "TradeRecordWindow.hpp"
#include "SettingWindow.hpp"

int main(int argc, char *argv[])
{
    // Register the custom type into the `MetaType` to transport them across the QThreads.
    qRegisterMetaType<PigSty>("Pig_Sty");
    qRegisterMetaType<PigSty>("Pig_Sty&");
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
    SettingWindow setting_window;
    home_window.show();

    // Click the "Start Game" button to show the game window.
    QObject::connect(&home_window, HomeWindow::OnStartButtonClicked, &Game_window, GameMainWindow::StartGame);
    QObject::connect(&home_window, HomeWindow::OnSettingButtonClicked, &setting_window, SettingWindow::show);

    return a.exec();
}
