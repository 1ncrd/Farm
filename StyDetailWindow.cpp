#include "StyDetailWindow.hpp"
#include "ui_stydetailwindow.h"
#include "GameMainWindow.hpp"
#include <QStringList>
#include <QTableWidgetItem>
#include <QCloseEvent>
#include <QHBoxLayout>
#include "GameTimer.hpp"

Sty_Detail_Window::Sty_Detail_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StyDetailWindow)
{
    ui->setupUi(this);
    this -> setFixedSize(GameMainWindow::WindowWidth / 2, GameMainWindow::WindowHeight);
    this -> setWindowTitle("Details");
    this -> setWindowIcon(QIcon(":/Resources/Picture/PigFace.png"));
    // Block other Windows when showing.
    this -> setAttribute(Qt::WA_ShowModal, true);

    // Insert into the object tree.
    // IMPORTANT!!!
    ui -> label_title -> setParent(this);
    ui -> label_TotalPig -> setParent(this);
    ui -> table_pig_information -> setParent(this);
    ui -> table_pig_amount -> setParent(this);

    this -> SetTablePigInformation();
    this -> SetTablePigAmount();

    ui -> label_TotalPig -> setText("Total:");
    ui -> label_TotalPig -> setFont(QFont("Minecraft", 14));
}

Sty_Detail_Window::~Sty_Detail_Window()
{
    ui -> table_pig_information -> clearContents();
    ui -> table_pig_amount -> clearContents();
    delete ui;
}

void Sty_Detail_Window::SetTablePigInformation()
{
    // The table's size has been set in the Qt Designer.

    ui -> table_pig_information -> setSelectionMode(QAbstractItemView::NoSelection);
    ui -> table_pig_information -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui -> table_pig_information -> setColumnCount(4);
    /*
     * Extra 1 column to display the header.
     * You might wonder why I don't use the function-"setVerticalHeaderItem".
     * The reason is that the built-in header of the QTableWidget do not support the HTML tags to show multiple styles of text.
     * In lieu of this display method, I used the QLabel.
     */
    ui -> table_pig_information -> setRowCount(11);

    // Set width of columns.
    ui -> table_pig_information -> setColumnWidth(0, 113);
    ui -> table_pig_information -> setColumnWidth(1, 138);
    ui -> table_pig_information -> setColumnWidth(2, 118);
    ui -> table_pig_information -> setColumnWidth(3, 102);

    // Let the grid's size be fixed and adjust to the table's size.
    ui -> table_pig_information -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Fixed);

    // Header height = 40. Others = 30.
    ui -> table_pig_information -> setRowHeight(0, 40);

    for (int i = 0; i < 10; i++)
    {
        ui -> table_pig_information -> setRowHeight(i + 1, 30);
    }

    // Set height.
    ui -> table_pig_information -> setFixedHeight(ui -> table_pig_information -> rowHeight(0)
            + ui -> table_pig_information -> rowHeight(1) * 10);

    // Hide the built-in header.
    ui -> table_pig_information -> horizontalHeader() -> setHidden(true);
    ui -> table_pig_information -> verticalHeader() -> setHidden(true);
}

void Sty_Detail_Window::SetTablePigAmount()
{
    // table_pig_amount:
    // 4*2 table.
    ui -> table_pig_amount -> setRowCount(4);
    ui -> table_pig_amount -> setColumnCount(2);
    // Set the width of columns.
    ui -> table_pig_amount -> setColumnWidth(0, 110);
    ui -> table_pig_amount -> setColumnWidth(1, 33);

    ui -> table_pig_amount -> verticalHeader() -> setSectionResizeMode(QHeaderView::Stretch);
    // Hide horizontal header and vertical header.
    ui -> table_pig_amount -> horizontalHeader() -> setHidden(true);
    ui -> table_pig_amount -> verticalHeader() -> setHidden(true);
}

void Sty_Detail_Window::Start(const PigSty * ptr_sty)
{
    this -> LoadTitle(ptr_sty);

    // [Bug Solved]: *** Before updating the pigs' information of ptr_sty, disconnect this to other ptr_sty ***.
    // Otherwise the connection command will pile up when you press other button as the Sty_Detail_Window is open.
    // This will take up memory, although it won't effect the correctness of what is displayed.
    disconnect(game_timer, GameTimer::timeout, this, 0);
    disconnect(this, Sty_Detail_Window::RequestStyData, 0, 0);

    // Call the `UpdateDetail` once a second to update the pigs' information.
    connect(this, Sty_Detail_Window::RequestStyData, ptr_sty, PigSty::GetStyData);
    connect(this, Sty_Detail_Window::LoadCompleted, this, [ = ]()
    {
        this -> show();
        disconnect(this, Sty_Detail_Window::LoadCompleted, 0, 0);
        this -> StartUpdating(ptr_sty);
    });
    emit RequestStyData();

}

void Sty_Detail_Window::LoadContent(const PigSty::PigStyData &pig_data)
{
    this -> LoadPigInformation(pig_data);
    this -> LoadPigAmount(pig_data);
    emit LoadCompleted();
}

void Sty_Detail_Window::StartUpdating(const PigSty * ptr_sty)
{
    connect(game_timer, GameTimer::timeout, this, [ = ]()
    {
        // Send a query request to `ptr_sty`
        emit RequestStyData();
    });
}

void Sty_Detail_Window::LoadTitle(const PigSty * ptr_sty)
{
    // Display a title like "Sty # 01".
    ui -> label_title -> setText(QString("Sty # ") + ptr_sty -> GetID());
    ui -> label_title -> setFont(QFont("Minecraft", 19));
}
void Sty_Detail_Window::LoadHeaderOfTablePigInformation()
{
    /*
     * Set the header without using the built-in header of the QTableWidget.
     * Maybe the code looks like a shit, it is the best idea so far I have to show the header with different font in one grid.
     */

    /*
     * According to my test, the Widget(maybe any object) inserted to the QTableWidget will be released when the "clearContents()" is called.
     * So we need not to concern the memory of the "header" label, so is the other contents in the table.
     */

    // For some unknown reseans, the third arguments in "QFont("Minecraft", 12, 75)" have no effects, which is supposed to make the font bold.
    // so I use the method "setBold()" methods to set font.
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

void Sty_Detail_Window::LoadHeaderOfTablePigAmount()
{
    // Four header: BlackPig, SmallFlowerPig, BigWhitePig, Sum.
    QTableWidgetItem * header_BlackPig = new QTableWidgetItem(QString("BlackPig"));
    QTableWidgetItem * header_SmallFlowerPig = new QTableWidgetItem(QString("SmallFlowerPig"));
    QTableWidgetItem * header_BigWhitePig = new QTableWidgetItem(QString("BigWhitePig"));
    QTableWidgetItem * header_sum = new QTableWidgetItem(QString("Sum"));
    header_sum -> setFont(QFont("Minecraft", 11));
    ui -> table_pig_amount -> setItem(0, 0, header_BlackPig);
    ui -> table_pig_amount -> setItem(1, 0, header_SmallFlowerPig);
    ui -> table_pig_amount -> setItem(2, 0, header_BigWhitePig);
    ui -> table_pig_amount -> setItem(3, 0, header_sum);
}

void Sty_Detail_Window::LoadPigInformation(const PigSty::PigStyData &pig_data)
{
    ui -> table_pig_information -> clearContents();
    ui -> table_pig_information -> setFont(QFont("Minecraft", 9));
    /*
     * According to my test, the Widget(maybe any object) inserted to the QTableWidget will be released when the "clearContents()" is called.
     * So we need not to concern the memory of the "header" label, so is the other contents in the table.
     */
    this -> LoadHeaderOfTablePigInformation();

    // Show the pig data.
    for (int i = 0; i < pig_data.pig_amount; i++)
    {
        // Load the data.
        QTableWidgetItem * content_id = new QTableWidgetItem(pig_data.pig_id[i]);
        QTableWidgetItem * content_species = new QTableWidgetItem(Pig::EnumToSpeciesName[pig_data.pig_species[i]]);
        QTableWidgetItem * content_weight = new QTableWidgetItem(QString::number(pig_data.pig_weight[i], 'f', 1));  // Keep one decimal place
        QTableWidgetItem * content_age = new QTableWidgetItem(QString::number(pig_data.pig_age[i]));
        // Set them to center.
        content_id -> setTextAlignment(Qt::AlignCenter);
        content_species -> setTextAlignment(Qt::AlignCenter);
        content_weight -> setTextAlignment(Qt::AlignCenter);
        content_age -> setTextAlignment(Qt::AlignCenter);
        int line_current = i + 1;
        ui -> table_pig_information -> setItem(line_current, 0, content_id);
        ui -> table_pig_information -> setItem(line_current, 1, content_species);
        ui -> table_pig_information -> setItem(line_current, 2, content_weight);
        ui -> table_pig_information -> setItem(line_current, 3, content_age);
    }
}

void Sty_Detail_Window::LoadPigAmount(const PigSty::PigStyData &pig_data)
{
    ui -> table_pig_amount -> clearContents();
    ui -> table_pig_amount -> setFont(QFont("Minecraft", 8));

    this -> LoadHeaderOfTablePigAmount();

    // Count the amount of every species of pig.
    int BlackPig_amount = 0, SmallFlowerPig_amount = 0, BigWhitePig_amount = 0, Pig_Sum = 0;

    for (int i = 0; i < pig_data.pig_amount; i++)
    {
        switch (pig_data.pig_species[i])
        {
            case Pig::BlackPig:
                BlackPig_amount++;
                break;

            case Pig::SmallFlowerPig:
                SmallFlowerPig_amount++;
                break;

            case Pig::BigWhitePig:
                BigWhitePig_amount++;
                break;
        }
    }

    // Calculate the sum of pigs.
    Pig_Sum = BlackPig_amount + SmallFlowerPig_amount + BigWhitePig_amount;
    // Display the data.
    QTableWidgetItem * BlackPig_amount_Item = new QTableWidgetItem(QString::number(BlackPig_amount));
    QTableWidgetItem * SmallFlowerPig_amount_Item = new QTableWidgetItem(QString::number(SmallFlowerPig_amount));
    QTableWidgetItem * BigWhitePig_amount_Item = new QTableWidgetItem(QString::number(BigWhitePig_amount));
    QTableWidgetItem * Pig_Sum_Item = new QTableWidgetItem(QString::number(Pig_Sum));
    BlackPig_amount_Item -> setTextAlignment(Qt::AlignCenter);
    SmallFlowerPig_amount_Item -> setTextAlignment(Qt::AlignCenter);
    BigWhitePig_amount_Item -> setTextAlignment(Qt::AlignCenter);
    Pig_Sum_Item -> setTextAlignment(Qt::AlignCenter);
    ui -> table_pig_amount -> setItem(0, 1, BlackPig_amount_Item);
    ui -> table_pig_amount -> setItem(1, 1, SmallFlowerPig_amount_Item);
    ui -> table_pig_amount -> setItem(2, 1, BigWhitePig_amount_Item);
    ui -> table_pig_amount -> setItem(3, 1, Pig_Sum_Item);
}

void Sty_Detail_Window::Preload(const PigSty *ptr_sty)
{
    // Load once to get ready
    this -> LoadHeaderOfTablePigInformation();
    this -> LoadHeaderOfTablePigAmount();

    disconnect(game_timer, GameTimer::timeout, this, 0);
    disconnect(this, Sty_Detail_Window::RequestStyData, 0, 0);

    connect(this, Sty_Detail_Window::RequestStyData, ptr_sty, PigSty::GetStyData);
    connect(this, Sty_Detail_Window::LoadCompleted, this, [ = ]()
    {
        disconnect(this, Sty_Detail_Window::LoadCompleted, 0, 0);
    });
    emit RequestStyData();
}

void Sty_Detail_Window::closeEvent(QCloseEvent *event)
{
    // When the window closed, clear the contents to release the memory.
    // The clearContents() will delete the QTableWidgetItem in the QTableWidget.
    disconnect(this, Sty_Detail_Window::RequestStyData, 0, 0);
    disconnect(game_timer, GameTimer::timeout, this, 0);
    ui -> table_pig_information -> clearContents();
    ui -> table_pig_amount -> clearContents();
    event -> accept();
}
