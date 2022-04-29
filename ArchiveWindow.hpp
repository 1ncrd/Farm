#ifndef ARCHIVEWINDOW_HPP
#define ARCHIVEWINDOW_HPP

#include <QMainWindow>
#include <QMouseEvent>
#include "CreateWindow.hpp"
#include "FileManager.hpp"

namespace Ui
{
    class ArchiveWindow;
}

class ArchiveWindow : public QMainWindow
{
    Q_OBJECT
public:
    static const int WindowWidth;
    static const int WindowHeight;
public:
    friend class CreateWindow;
    explicit ArchiveWindow(QWidget *parent = nullptr);
    ~ArchiveWindow();
    void CreateArchive();
    void ConfigueArchiveUpdate();
    void ConfigueButtonEnter();
    void Start();
    void UpdateArchiveItem(QVector<FileManager::GameData> data);
    void mousePressEvent(QMouseEvent *event);
    void UpdateButtonEnable();
private:
    Ui::ArchiveWindow *ui;

    QVector<QString> current_existing_archive;
    CreateWindow * create_window;
    QVector<FileManager::GameData> current_existing_archive_data;
signals:
    RequestAllArchiveInfo();
    RequestDeleteArchive(QString file_name);
    GameStart(QString file_name);
    Back();
};

#endif // ARCHIVEWINDOW_HPP
