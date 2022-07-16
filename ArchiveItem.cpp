#include "ArchiveItem.hpp"
#include "ui_ArchiveItem.h"
#include <QDebug>
#include "FileManager.hpp"

// Convert the difficulty level to the corresponding string.
std::map<int, QString> ArchiveItem::DifficultyLevelToQString =
{
    {0, QString("Easy")},
    {1, QString("Medium")},
    {2, QString("Difficult")}
};

ArchiveItem::ArchiveItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArchiveItem)
{
    ui->setupUi(this);
    this -> setFixedSize(540, 90);
    this -> SetLogo();
}

ArchiveItem::~ArchiveItem()
{
    qDebug() << "ArchiveItem released";
    delete ui;
}

void ArchiveItem::SetLogo()
{
    QPixmap Pigimage;
    // Get the pig picture and set up the logo.
    Pigimage.load(":/Resources/Picture/PigFace.png");
    ui -> label_logo -> setScaledContents(true);
    ui -> label_logo -> setPixmap(Pigimage);
}

void ArchiveItem::SetEditDate(const QString &date)
{
    ui -> label_edit_time -> setFont(QFont("Minecraft", 11));
    ui -> label_edit_time -> setText(date);
    ui -> label_edit_time -> adjustSize();
}

void ArchiveItem::SetArchiveName(const QString &name)
{
    QFont font("Minecraft", 14);
    font.setBold(true);
    ui -> label_name -> setFont(font);
    ui -> label_name -> setText(name);
    ui -> label_name -> adjustSize();
}

void ArchiveItem::SetDifficultyDisplay(const int &difficulty_level)
{
    QFont font("Minecraft", 12);
    font.setBold(true);
    ui -> label_difficulty -> setFont(font);
    ui -> label_difficulty -> setText(DifficultyLevelToQString[difficulty_level]);
    ui -> label_difficulty -> adjustSize();
}
