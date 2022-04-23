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
    static const int WindowWidth;
    static const int WindowHeight;

    explicit Sty_Detail_Window(QWidget *parent = nullptr);
    ~Sty_Detail_Window();
    void SetTablePigInformation();
    void SetTablePigAmount();
    void Start(const int &sty_num);
    void ShowDetail(const PigSty * ptr_sty);
    void StartUpdating(const int &sty_num);
    void LoadContent(const QVector<Pig::PigInfo> &sty_data);
    void LoadHeaderOfTablePigInformation();
    void LoadHeaderOfTablePigAmount();
    void LoadTitle(const int &sty_num);
    void LoadPigInformation(const QVector<Pig::PigInfo> &sty_data);
    void LoadPigAmount(const QVector<Pig::PigInfo> &sty_data);
    void Preload(const int &sty_num);
    void closeEvent(QCloseEvent *event);
signals:
    RequestStyData(const int &sty_num);
    LoadCompleted();
};

#endif // SPECIFICSTY_WINDOW_H
