#ifndef CREATEWINDOW_HPP
#define CREATEWINDOW_HPP

#include <QMainWindow>
#include <QComboBox>

namespace Ui
{
    class CreateWindow;
}

class CreateWindow : public QMainWindow
{
    Q_OBJECT
public:
    static const int WindowWidth;
    static const int WindowHeight;
public:
    explicit CreateWindow(QWidget *parent = nullptr);
    ~CreateWindow();

    void ConfigueComboBox();
    void ConfigueButton();
    void Start();
    void SetDefaults();
    void SetCurrntExsistingArchiveList(const QVector<QString> &current_existing_archive);
private:
    Ui::CreateWindow *ui;

    QVector<QString> current_existing_archive;
signals:

};

#endif // CREATEWINDOW_HPP
