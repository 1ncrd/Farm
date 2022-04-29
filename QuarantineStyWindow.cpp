#include "QuarantineStyWindow.hpp"
#include "ui_QuarantineStyWindow.h"
#include <QHBoxLayout>
#include <QDebug>

const int QuarantineStyWindow::WindowWidth = 512;
const int QuarantineStyWindow::WindowHeight = 576;

QuarantineStyWindow::QuarantineStyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QuarantineStyWindow)
{
    ui->setupUi(this);

    this -> setFixedSize(WindowWidth, WindowHeight);
    this -> setWindowTitle("Quarantine Sty");
    this -> setWindowIcon(QIcon(":/Resources/Picture/PigFace.png"));
    // Block other Windows when showing.
    this -> setAttribute(Qt::WA_ShowModal, true);

    ui -> table_pig_information -> setSelectionMode(QAbstractItemView::NoSelection);
    ui -> table_pig_information -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui -> table_pig_information -> setColumnCount(4);
    // Set width of columns.
    ui -> table_pig_information -> setColumnWidth(0, 113);
    ui -> table_pig_information -> setColumnWidth(1, 138);
    ui -> table_pig_information -> setColumnWidth(2, 118);
    ui -> table_pig_information -> setColumnWidth(3, 102);

    connect(this, QuarantineStyWindow::RequestQuarantineStyData, pig_sty_manager, PigStyManager::GetQuarantineStyData);
    connect(pig_sty_manager, PigStyManager::SendQuarantineStyData, this, QuarantineStyWindow::LoadContent);

    this -> Configure_label_title();
    this -> Configure_button_dispose();
}

QuarantineStyWindow::~QuarantineStyWindow()
{
    delete ui;
}

void QuarantineStyWindow::Start()
{
    emit RequestQuarantineStyData(PigStyManager::QuarantineStyID_int);
}

void QuarantineStyWindow::LoadHeader()
{
    ui -> table_pig_information -> insertRow(0);
    ui -> table_pig_information -> setRowHeight(0, 40);
    ui -> table_pig_information -> setFont(QFont("Minecraft", 9));
    QFont header_font = QFont("Minecraft", 13);
    header_font.setBold(true);

    // Header1: ID
    QTableWidgetItem * header_ID = new QTableWidgetItem(QString("ID"));
    header_ID -> setFont(header_font);
    header_ID -> setTextAlignment(Qt::AlignCenter);
    ui -> table_pig_information -> setItem(0, 0, header_ID);

    // Header2: Species
    QTableWidgetItem * header_species = new QTableWidgetItem(QString("Species"));
    header_species -> setFont(header_font);
    header_species -> setTextAlignment(Qt::AlignCenter);
    ui -> table_pig_information -> setItem(0, 1, header_species);

    // Header3: Weight/kg
    QLabel * header_weight = new QLabel(QString("<font style='font-size:17px;font:Minecraft;font-weight:bold'>Weight</font><font style='font-size:12px;font:Minecraft;'>/kg</font>"));
    QHBoxLayout * layout_weight = new QHBoxLayout;
    QWidget * widget_weight = new QWidget;
    header_weight -> setAlignment(Qt::AlignHCenter);
    // [Warning]: If the following line of code is added, the "AlignHCenter" will be invalid, although it seems ok.
    // header_weight -> setAlignment(Qt::AlignVCenter);
    widget_weight -> setLayout(layout_weight);
    layout_weight -> addWidget(header_weight);
    ui -> table_pig_information -> setCellWidget(0, 2, widget_weight);

    // Header4: Age/day
    QLabel * header_age = new QLabel(QString("<font style='font-size:17px;font:Minecraft;font-weight:bold'>Age</font><font style='font-size:12px;font:Minecraft;'>/day</font>"));
    QHBoxLayout * layout_age = new QHBoxLayout;
    QWidget * widget_age = new QWidget;
    header_age -> setAlignment(Qt::AlignHCenter);
    widget_age -> setLayout(layout_age);
    layout_age -> addWidget(header_age);
    ui -> table_pig_information -> setCellWidget(0, 3, widget_age);
}

void QuarantineStyWindow::LoadContent(const QVector<Pig::PigInfo> &sty_data)
{
    ui -> table_pig_information -> clearContents();
    this -> LoadHeader();
    this -> Load_label_total(sty_data);

    for (int i = 0; i < sty_data.length(); i++)
    {
        int row_count = ui -> table_pig_information -> rowCount();
        ui -> table_pig_information -> insertRow(row_count);
        // Load the data.
        QTableWidgetItem * content_id = new QTableWidgetItem(sty_data[i].id);
        QTableWidgetItem * content_species = new QTableWidgetItem(Pig::EnumToSpeciesName[sty_data[i].species]);
        QTableWidgetItem * content_weight = new QTableWidgetItem(QString::number(sty_data[i].weight, 'f', 1));  // Keep one decimal place
        QTableWidgetItem * content_age = new QTableWidgetItem(QString::number(sty_data[i].age));
        // Set them to center.
        content_id -> setTextAlignment(Qt::AlignCenter);
        content_species -> setTextAlignment(Qt::AlignCenter);
        content_weight -> setTextAlignment(Qt::AlignCenter);
        content_age -> setTextAlignment(Qt::AlignCenter);

        // Infection
        if (sty_data[i].is_infected)
        {
            content_id -> setBackground(QBrush(QColor(255, 192, 203)));
        }

        int line_current = i + 1;
        ui -> table_pig_information -> setItem(line_current, 0, content_id);
        ui -> table_pig_information -> setItem(line_current, 1, content_species);
        ui -> table_pig_information -> setItem(line_current, 2, content_weight);
        ui -> table_pig_information -> setItem(line_current, 3, content_age);
    }

    this -> show();
}

void QuarantineStyWindow::Load_label_total(const QVector<Pig::PigInfo> &sty_data)
{
    QString amount = QString::number(sty_data.length());
    ui -> label_total_pig -> setText(QString("Total: ") + amount + QString(" infected"));
}

void QuarantineStyWindow::Configure_label_title()
{
    ui -> label_title -> setFont(QFont("Minecraft", 19));
    ui -> label_title -> setText(QString("Quarantine Sty"));
}

void QuarantineStyWindow::Configure_button_dispose()
{
    ui -> button_dispose -> setFont(QFont("Minecraft", 12));
    connect(ui -> button_dispose, QPushButton::clicked, pig_sty_manager, PigStyManager::DisposeQuarantineSty);
}

void QuarantineStyWindow::closeEvent(QCloseEvent *event)
{
    ui -> table_pig_information -> clearContents();
    ui -> table_pig_information -> setRowCount(0);
    event -> accept();
}
