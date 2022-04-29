#ifndef ARCHIVEITEM_HPP
#define ARCHIVEITEM_HPP

#include <QWidget>

namespace Ui
{
    class ArchiveItem;
}

class ArchiveItem : public QWidget
{
    Q_OBJECT

public:
    static std::map<int, QString> DifficultyLevelToQString;
    explicit ArchiveItem(QWidget *parent = nullptr);
    ~ArchiveItem();
    void SetLogo();
    void SetEditDate(const QString &date);
    void SetArchiveName(const QString &name);
    void SetDifficultyDisplay(const int &difficulty_level);

private:
    Ui::ArchiveItem *ui;

protected:
    virtual void mouseReleaseEvent(QMouseEvent * event);
signals:
    clicked();
};

#endif // ARCHIVEITEM_HPP
