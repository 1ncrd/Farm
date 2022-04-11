#ifndef GAMEMAINWINDOW_H
#define GAMEMAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include "PushButtonToSty.hpp"
#include "PigSty.hpp"
#include "StyDetailWindow.hpp"
#include <QThread>
#include "TradeRecordWindow.hpp"

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
    static const int PigStyAmount = 100;
    static const int PigStyPerRow;
    static const int PigStyPerColumn;
private:
    Ui::GameMainWindow * ui;

    // Main thread to deal with the UI display.
    QLabel * label_date;
    QLabel * label_money;
    QLabel * label_pig_sold_amount;
    QScrollArea * scroll_area_choose_sty;
    QWidget * area_choose_sty;
    PushButtonToSty * ptr_btn_entersty[PigStyAmount];
    TradeRecordWindow * trade_record_window;
    QPushButton * button_to_trade_record_window;
    PigSty * pig_sty[PigStyAmount];
    Sty_Detail_Window * sty_window;
    // `thread_to_process` to deal with computing tasks.
    QThread * thread_to_process;
public:
    explicit GameMainWindow(QWidget *parent = nullptr);
    ~GameMainWindow();
    void StartGame();
    void Create_label_date();
    void Create_label_money();
    void Create_label_pig_sold_amount();
    void Create_area_choose_sty();
    void Create_sty_window();
    void Create_trade_record_window();
    void Create_button_to_trade_record_window();

    void Connect_PigProcess();
    void Connect_label_date();
    void Connect_label_money();
    void Connect_label_pig_sold_amount();
    void Connect_sty_window();
    void Connect_trade_record_window();

    void Sty_Detail_Window_PreLoad();
signals:
    Pig_Sty_LetAllPigGrow();
    Pig_Sty_SellPig();
    Pig_Sty_AddRandomPig();
    Pig_Sty_AddPig(int number);
};

#endif // GAMEMAINWINDOW_H
