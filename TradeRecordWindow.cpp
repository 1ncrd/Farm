#include "TradeRecordWindow.hpp"
#include "ui_traderecordwindow.h"
#include <QLabel>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QStandardItemModel>
#include "GameMainWindow.hpp"
#include "Pig.hpp"

TradeRecordWindow::TradeRecordWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TradeRecordWindow)
{
    ui->setupUi(this);
    this -> setFixedSize(GameMainWindow::WindowWidth / 2, GameMainWindow::WindowHeight);
    this -> setWindowTitle("Records");
    this -> setWindowIcon(QIcon(":/Resources/Picture/PigFace.png"));
    // Block other windows when showing.
    this -> setAttribute(Qt::WA_ShowModal, true);

    this -> SetTitle();
    this -> SetTableRecord();
    // this -> SetTableHeader();
    this -> ConfigueFilter();
    this -> ConfigueComboBox();

    this -> result_qvector = new QVector<FileManager::TradeRecord>;
    connect(this, TradeRecordWindow::RequestTradeRecord, file_manager, FileManager::ReadTradeRecord);
    connect(file_manager, FileManager::ReadTradeRecordCompleted, this, TradeRecordWindow::LoadTableRecord);
}

TradeRecordWindow::~TradeRecordWindow()
{
    delete result_qvector;
    delete ui;
}

void TradeRecordWindow::SetTitle()
{
    ui -> label_title -> setFont(QFont("Minecraft", 19));
    ui -> label_title -> setText("Trade Record");
}

void TradeRecordWindow::SetTableHeader()
{
    // Rely on the `table_record` to set width.
    ui -> table_header -> setSelectionMode(QAbstractItemView::NoSelection);
    ui -> table_header -> setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int column_number = 0; column_number < 6; column_number++)
    {
        ui -> table_header -> setColumnWidth(column_number, ui -> table_record -> columnWidth(column_number) - 3);
    }

    ui -> table_header -> setFont(QFont("Minecraft", 10));
    QTableWidgetItem * header_time = new QTableWidgetItem(QString("Time"));
    QTableWidgetItem * header_type = new QTableWidgetItem(QString("Type"));
    QTableWidgetItem * header_id = new QTableWidgetItem(QString("ID"));
    QTableWidgetItem * header_species = new QTableWidgetItem(QString("Species"));
    QTableWidgetItem * header_weight = new QTableWidgetItem(QString("Weight"));
    QTableWidgetItem * header_age = new QTableWidgetItem(QString("Age"));
    header_type -> setTextAlignment(Qt::AlignCenter);
    header_time -> setTextAlignment(Qt::AlignCenter);
    header_id -> setTextAlignment(Qt::AlignCenter);
    header_species -> setTextAlignment(Qt::AlignCenter);
    header_weight -> setTextAlignment(Qt::AlignCenter);
    header_age -> setTextAlignment(Qt::AlignCenter);

    //ui -> table_header -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);
    ui -> table_header -> setItem(0, 0, header_time);
    ui -> table_header -> setItem(0, 1, header_type);
    ui -> table_header -> setItem(0, 2, header_id);
    ui -> table_header -> setItem(0, 3, header_species);
    ui -> table_header -> setItem(0, 4, header_weight);
    ui -> table_header -> setItem(0, 5, header_age);
}

void TradeRecordWindow::SetTableRecord()
{
    ui -> table_record -> setSelectionMode(QAbstractItemView::NoSelection);
    ui -> table_record -> setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui -> table_record -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);
    ui -> table_record -> horizontalHeader() -> setSectionResizeMode(3, QHeaderView::ResizeToContents);
}

void TradeRecordWindow::Start()
{
    auto archive_name = FileManager::GetCurrentArchiveName();

    if (FileManager::CheckFileIsEmpty(archive_name))
    {
        QMessageBox::information(this, "Hint", tr("<font face = Minecraft size = 4>There's no record of any transactions</font>"));
    }
    else
    {
        // this -> LoadTableRecord(QString("Record_1.dat"));
        result_qvector -> clear();
        emit RequestTradeRecord(result_qvector, archive_name);
    }
}

void TradeRecordWindow::LoadTableRecord()
{
    ui -> table_record -> clearContents();
    ui -> table_record -> setRowCount(0);

    for (QVector<FileManager::TradeRecord>::iterator iterator_record = result_qvector -> begin(); iterator_record != result_qvector -> end(); iterator_record++)
    {
        int row_count = ui -> table_record -> rowCount();
        ui -> table_record -> insertRow(row_count);
        QTableWidgetItem * content_time = new QTableWidgetItem(QString::number(iterator_record -> trade_time));
        QTableWidgetItem * content_type = new QTableWidgetItem(FileManager::EnumToTradeTypeName[iterator_record -> trade_type]);
        QTableWidgetItem * content_id = new QTableWidgetItem(iterator_record -> pig_id);
        QTableWidgetItem * content_species = new QTableWidgetItem(Pig::EnumToSpeciesName[iterator_record -> pig_species]);
        QTableWidgetItem * content_weight = new QTableWidgetItem(QString::number(iterator_record -> pig_weight));
        QTableWidgetItem * content_age = new QTableWidgetItem(QString::number(iterator_record -> pig_age));
        content_time -> setTextAlignment(Qt::AlignCenter);
        content_type -> setTextAlignment(Qt::AlignCenter);
        content_id -> setTextAlignment(Qt::AlignCenter);
        content_species -> setTextAlignment(Qt::AlignCenter);
        content_weight -> setTextAlignment(Qt::AlignCenter);
        content_age -> setTextAlignment(Qt::AlignCenter);
        ui -> table_record -> setRowHeight(row_count, 17);
        ui -> table_record -> setItem(row_count, 0, content_time);
        ui -> table_record -> setItem(row_count, 1, content_type);
        ui -> table_record -> setItem(row_count, 2, content_id);
        ui -> table_record -> setItem(row_count, 3, content_species);
        ui -> table_record -> setItem(row_count, 4, content_weight);
        ui -> table_record -> setItem(row_count, 5, content_age);
    }

    this -> SetTableHeader();
    this -> show();
}

void TradeRecordWindow::FilterTableRecord(bool if_filter_type, bool if_filter_species, FileManager::TradeType filter_type, Pig::PigSpecies filter_species)
{
    ui -> table_record -> clearContents();
    ui -> table_record -> setRowCount(0);

    QVector<FileManager::TradeRecord>::iterator iterator_record;

    for (iterator_record = result_qvector -> begin(); iterator_record != result_qvector -> end(); iterator_record++)
    {
        if ((!if_filter_type or (if_filter_type and iterator_record -> trade_type == filter_type))
                and (!if_filter_species or (if_filter_species and iterator_record -> pig_species == filter_species)))
        {
            int row_count = ui -> table_record -> rowCount();
            ui -> table_record -> insertRow(row_count);
            QTableWidgetItem * content_time = new QTableWidgetItem(QString::number(iterator_record -> trade_time));
            QTableWidgetItem * content_type = new QTableWidgetItem(FileManager::EnumToTradeTypeName[iterator_record -> trade_type]);
            QTableWidgetItem * content_id = new QTableWidgetItem(iterator_record -> pig_id);
            QTableWidgetItem * content_species = new QTableWidgetItem(Pig::EnumToSpeciesName[iterator_record -> pig_species]);
            QTableWidgetItem * content_weight = new QTableWidgetItem(QString::number(iterator_record -> pig_weight));
            QTableWidgetItem * content_age = new QTableWidgetItem(QString::number(iterator_record -> pig_age));
            content_time -> setTextAlignment(Qt::AlignCenter);
            content_type -> setTextAlignment(Qt::AlignCenter);
            content_id -> setTextAlignment(Qt::AlignCenter);
            content_species -> setTextAlignment(Qt::AlignCenter);
            content_weight -> setTextAlignment(Qt::AlignCenter);
            content_age -> setTextAlignment(Qt::AlignCenter);
            ui -> table_record -> setRowHeight(row_count, 20);
            ui -> table_record -> setItem(row_count, 0, content_time);
            ui -> table_record -> setItem(row_count, 1, content_type);
            ui -> table_record -> setItem(row_count, 2, content_id);
            ui -> table_record -> setItem(row_count, 3, content_species);
            ui -> table_record -> setItem(row_count, 4, content_weight);
            ui -> table_record -> setItem(row_count, 5, content_age);
        }
    }
}

void TradeRecordWindow::ConfigueComboBox()
{
    for (int i = 0; i < ui -> comboBox_filter_Species -> count(); ++i)
    {
        // Set the content of the combobox to be horizontal center.
        static_cast<QStandardItemModel*>(ui -> comboBox_filter_Species -> model())
        -> item(i) -> setTextAlignment(Qt::AlignCenter);
    }

    for (int i = 0; i < ui -> comboBox_filter_TradeType -> count(); ++i)
    {
        // Set the content of the combobox to be horizontal center.
        static_cast<QStandardItemModel*>(ui -> comboBox_filter_TradeType -> model())
        -> item(i) -> setTextAlignment(Qt::AlignCenter);
    }

    QString str = "QComboBox QAbstractItemView::item{\
                    min-height:30px;\
                    min-width:20px;\
                    }";

    ui -> comboBox_filter_Species -> setStyleSheet(str);
    ui -> comboBox_filter_TradeType -> setStyleSheet(str);
}

void TradeRecordWindow::ConfigueFilter()
{
    // Filter the content of the table according to the user's selections.
    connect(ui -> pushButton_exec_filter, QPushButton::clicked, this, [ = ]()
    {
        int filter_type = ui -> comboBox_filter_TradeType -> currentIndex();
        int filter_species = ui -> comboBox_filter_Species -> currentIndex();
        this -> FilterTableRecord(filter_type, filter_species,
                                  FileManager::TradeType(filter_type - 1), Pig::PigSpecies(filter_species - 1));
    });
}

void TradeRecordWindow::closeEvent(QCloseEvent *event)
{
    ui -> table_record -> clearContents();
    ui -> comboBox_filter_Species -> setCurrentIndex(0);
    ui -> comboBox_filter_TradeType -> setCurrentIndex(0);
    event -> accept();
}
// TODO 时间大于1500时就卡的一批。
