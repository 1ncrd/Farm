#ifndef TRADERECORDWINDOW_H
#define TRADERECORDWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui
{
    class TradeRecordWindow;
}

class TradeRecordWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TradeRecordWindow(QWidget *parent = nullptr);
    ~TradeRecordWindow();

    void SetTitle();
    void SetTableHeader();
    void SetTableRecord();

    void Start();
    void LoadTableRecord(const QString &file_name);

    void closeEvent(QCloseEvent *event);
private:
    Ui::TradeRecordWindow *ui;
};

#endif // TRADERECORDWINDOW_H
