#ifndef STYDETAILWINDOW_H
#define STYDETAILWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include "PigSty.hpp"
namespace Ui
{
    class StyDetailWindow;
}

class Sty_Detail_Window : public QMainWindow
{
    Q_OBJECT

private:
    Ui::StyDetailWindow *ui;
public:

    explicit Sty_Detail_Window(QWidget *parent = nullptr);
    ~Sty_Detail_Window();
    void SetTablePigInformation();
    void SetTablePigAmount();
    void Start(const PigSty * ptr_sty);
    void ShowDetail(const PigSty * ptr_sty);
    void StartUpdating(const PigSty * ptr_sty);
    void LoadContent(const PigSty::PigStyData &pig_data);
    void LoadHeaderOfTablePigInformation();
    void LoadHeaderOfTablePigAmount();
    void LoadTitle(const PigSty * ptr_sty);
    void LoadPigInformation(const PigSty::PigStyData &pig_data);
    void LoadPigAmount(const PigSty::PigStyData &pig_data);
    void Preload(const PigSty * ptr_sty);
    void closeEvent(QCloseEvent *event);
signals:
    RequestStyData();
    LoadCompleted();
};

#endif // SPECIFICSTY_WINDOW_H
