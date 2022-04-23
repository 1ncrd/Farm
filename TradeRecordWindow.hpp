#ifndef TRADERECORDWINDOW_H
#define TRADERECORDWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "FileManager.hpp"

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
    void LoadTableRecord();
    void FilterTableRecord(bool if_filter_type = false, bool if_filter_species = false, FileManager::TradeType filter_type = FileManager::Buy, Pig::PigSpecies filter_species = Pig::BlackPig);
    void ConfigueFilter();

    void closeEvent(QCloseEvent *event);
private:
    Ui::TradeRecordWindow *ui;
    // Used to transmit data.
    QVector<FileManager::TradeRecord> * result_qvector;
signals:
    RequestTradeRecord(QVector<FileManager::TradeRecord> * result_qvector, QString file_name);
};

#endif // TRADERECORDWINDOW_H
