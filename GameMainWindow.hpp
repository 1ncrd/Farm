#ifndef GAMEMAINWINDOW_H
#define GAMEMAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QThread>
#include "GameTimer.hpp"
#include "PushButtonToSty.hpp"
#include "PigStyManager.hpp"
#include "StyDetailWindow.hpp"
#include "QuarantineStyWindow.hpp"
#include "TradeRecordWindow.hpp"
#include "MyPushButton.hpp"

namespace Ui
{
    class GameMainWindow;
}

class GameMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const int WindowWidth;
    static const int WindowHeight;
    static std::map<int, int> DifficultyToInfectionPercent;
    int difficulty;
    QString archive_name;
private:
    Ui::GameMainWindow * ui;

    // Main thread to deal with the UI display.
    QLabel * label_date;
    QLabel * label_money;
    QLabel * label_pig_sold_amount;
    QLabel * label_infection_status;
    QScrollArea * scroll_area_choose_sty;
    QWidget * area_choose_sty;
    PushButtonToSty * button_show_normal_sty[PigStyManager::PigStyAmount];
    TradeRecordWindow * trade_record_window;
    Sty_Detail_Window * sty_detail_window;
    QuarantineStyWindow * quarantine_sty_window;
    MyPushButton * button_show_trade_record_window;
    MyPushButton * button_quarantine_pig;
    MyPushButton * button_show_quarantine_sty;

    // `thread_to_process` to deal with computing tasks.
public:
    explicit GameMainWindow(QWidget *parent = nullptr);
    ~GameMainWindow();
    void StartGame(QString file_name);
    void Create_label_date();
    void Create_label_money();
    void Create_label_pig_sold_amount();
    void Create_label_infection_status();
    void Create_area_choose_sty_with_btn();
    void Create_sty_detail_window();
    void Create_trade_record_window();
    void Create_button_show_trade_record_window();
    void Create_button_quarantine_pig();
    void Create_button_show_quarantine_sty();
    void Create_quarantine_sty_window();

    void Connect_label_date();
    void Connect_label_money();
    void Connect_label_pig_sold_amount();
    void Connect_label_infection_status();
    void Connect_button_to_Sty_Detail();
    void Connect_button_show_trade_record_window();
    void Connect_button_quarantine_pig();
    void Connect_button_show_quarantine_sty();

    void Sty_Detail_Window_PreLoad();
    void ConfigueArchiveStore();
    void ConfiguePauseButton();
    void StoreGameData();
    void closeEvent(QCloseEvent *event);
signals:
    Pig_Sty_LetAllPigGrow();
    Pig_Sty_SellPig();
    Pig_Sty_AddRandomPig();
    Pig_Sty_AddPig(int number);
    Pig_Sty_InfectOnePig(const int &id);
};

#endif // GAMEMAINWINDOW_H
