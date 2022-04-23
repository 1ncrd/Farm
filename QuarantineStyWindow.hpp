#ifndef QUARANTINESTYWINDOW_HPP
#define QUARANTINESTYWINDOW_HPP

#include <QMainWindow>
#include <QCloseEvent>
#include "PigStyManager.hpp"

namespace Ui
{
    class QuarantineStyWindow;
}

class QuarantineStyWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const int WindowWidth;
    static const int WindowHeight;

    explicit QuarantineStyWindow(QWidget *parent = nullptr);
    ~QuarantineStyWindow();

    void Start();
    void LoadHeader();
    void LoadContent(const QVector<Pig::PigInfo> &sty_data);
    void Load_label_total(const QVector<Pig::PigInfo> &sty_data);
    void Configure_label_title();
    void Configure_button_dispose();
    void closeEvent(QCloseEvent *event);
private:
    Ui::QuarantineStyWindow *ui;

signals:
    RequestQuarantineStyData(const int &sty_num);
};

#endif // QUARANTINESTYWINDOW_HPP
