#include "PigSty.hpp"
#include "Pig.hpp"
#include "GameTimer.hpp"
#include <QDebug>


long PigSty::money = 0;
PigSty::PigSoldAmount PigSty::pig_sold_amount;

int PigSty::Random() const
{
    srand(time(0) ^ (id.toInt() * 6180339887) ^ (4989484820 ^ (pig_amount * 4586834365)));
    return rand();
}
PigSty::PigSty(const QString &sty_id_temp, QObject *parent)
    : QObject{parent}
{
    // Initialization.
    if (sty_id_temp.length() == 1)
    {
        this -> id = QString("0") + sty_id_temp;
    }
    else
    {
        this -> id = sty_id_temp;
    }

    this -> pig_amount = 0;
    this -> ptr_pig_head = nullptr;
    // When a new pig sty is created, add five pigs automatically.
    AddPig(5);
}

PigSty::~PigSty()
{
    this -> DeleteAllPigs();
//    qDebug() << "Pig_Sty released." << sty_id;
}

void PigSty::AddPig(int number/* = 1*/)
{
    rwlock.lockForRead();
    // Add pig(s) to the end of the linked list.
    Pig * ptr_pig_current = this -> ptr_pig_head;
    int order = 0;

    // If the linked list is empty, create the head pig node.
    if (ptr_pig_current == nullptr)
    {
        ptr_pig_current = new Pig(id, order++, int(this -> CheckStySpeciesSituation()));
        ptr_pig_current -> previous_pig = nullptr;
        this -> ptr_pig_head = ptr_pig_current;
        RecordTrade(FileManager::Buy, ptr_pig_current);
        number--;
        pig_amount++;
    }
    else
    {
        while (ptr_pig_current -> next_pig != nullptr)
        {
            ptr_pig_current = ptr_pig_current -> next_pig;
        }
    }

    StySpeciesSituation sty_species_situation = CheckStySpeciesSituation();

    for (int i = 0; i < number; i++)
    {
        ptr_pig_current -> next_pig = new Pig(id, order++, int(sty_species_situation));
        ptr_pig_current -> next_pig -> previous_pig = ptr_pig_current;
        RecordTrade(FileManager::Buy, ptr_pig_current -> next_pig);
        pig_amount++;
        ptr_pig_current = ptr_pig_current -> next_pig;
    }

    rwlock.unlock();
}

void PigSty::AddRandomPig()
{
    int number = this -> Random() % (10 - this -> GetPigAmount());
    AddPig(number);
}

void PigSty::SellPig()
/*
 * Sell pigs meet the requirements.
 */
{
    rwlock.lockForRead();
    Pig * ptr_pig_current = ptr_pig_head;
    Pig * ptr_pig_next = ptr_pig_head -> next_pig;

    while (ptr_pig_next != nullptr)
    {
        if (ptr_pig_current -> weight > 75 or ptr_pig_current -> age > 365)
        {
            assert(ptr_pig_current != nullptr);
            money += Pig::SpeciesPrice[ptr_pig_current->species];
            CountSoldPig(ptr_pig_current);
            RecordTrade(FileManager::Sell, ptr_pig_current);
            DeletePig(ptr_pig_current);
        }

        ptr_pig_current = ptr_pig_next;
        ptr_pig_next = ptr_pig_next -> next_pig;
    }

    if (ptr_pig_current -> weight > 75 or ptr_pig_current -> age > 365)
    {
        assert(ptr_pig_current != nullptr);
        money += Pig::SpeciesPrice[ptr_pig_current->species];
        RecordTrade(FileManager::Sell, ptr_pig_current);
        DeletePig(ptr_pig_current);
    }

    if (id.toInt() == 99)
    {
        emit SellPigFinished();
    }

    rwlock.unlock();
}
void PigSty::CountSoldPig(Pig * const &ptr_pig_to_count) const
{
    pig_sold_amount.total++;

    switch (ptr_pig_to_count -> species)
    {
        case Pig::BlackPig:
            pig_sold_amount.BlackPig++;
            break;

        case Pig::SmallFlowerPig:
            pig_sold_amount.SmallFlowerPig++;
            break;

        case Pig::BigWhitePig:
            pig_sold_amount.BigWhitePig++;
            break;
    }
}
void PigSty::RecordTrade(const FileManager::TradeType &type, Pig * const &ptr_pig_to_record) const
{
    FileManager::TradeRecord sale_record_temp;
    sale_record_temp.trade_time = game_timer -> GetTime();
    sale_record_temp.trade_type = type;
    sale_record_temp.pig_id = ptr_pig_to_record -> id;
    sale_record_temp.pig_species = ptr_pig_to_record -> species;
    sale_record_temp.pig_weight = ptr_pig_to_record -> weight;
    sale_record_temp.pig_age = ptr_pig_to_record -> age;
    FileManager::WriteTradeRecord(sale_record_temp, "Record_1.dat");
}

void PigSty::DeletePig(Pig * const &ptr_pig_to_delete)
{
    rwlock.lockForRead();

    // When the ptr_pig_to_delete at the beginning of the list.
    if (ptr_pig_to_delete == ptr_pig_head)
    {
        ptr_pig_head = ptr_pig_head -> next_pig;

        if (ptr_pig_head != nullptr)
        {
            ptr_pig_head -> previous_pig = nullptr;
        }

        delete ptr_pig_to_delete;
        pig_amount--;
    }
    else
    {
        ptr_pig_to_delete -> previous_pig -> next_pig = ptr_pig_to_delete -> next_pig;

        if (ptr_pig_to_delete -> next_pig != nullptr)
        {
            ptr_pig_to_delete -> next_pig -> previous_pig = ptr_pig_to_delete -> previous_pig;
        }

        delete ptr_pig_to_delete;
        pig_amount--;
    }

    rwlock.unlock();
}

void PigSty::LetAllPigGrow()
{
    rwlock.lockForRead();

    if (ptr_pig_head == nullptr)
    {
        return;
    }

    Pig * ptr_pig_current = ptr_pig_head;

    while (ptr_pig_current != nullptr)
    {
        ptr_pig_current -> Grow();
        ptr_pig_current = ptr_pig_current -> next_pig;
    }

    rwlock.unlock();
}

QString PigSty::GetID() const
{
    return this -> id;
}

int PigSty::GetPigAmount() const
{
    return this -> pig_amount;
}

void PigSty::GetStyData()
{
    // Use sending signal to communicate between thread safely.
    // Critical function: `SendStyData(result_data)`.
//    qDebug() << id;
    PigSty::PigStyData result_data;

    if (ptr_pig_head == nullptr)
    {
        emit SendStyData(result_data);
        return;
    }

    Pig * ptr_pig_current = ptr_pig_head;
    result_data.pig_amount = pig_amount;
    unsigned int i = 0;

    while (ptr_pig_current != nullptr)
    {
        result_data.pig_id[i] = ptr_pig_current-> id;
        result_data.pig_species[i] = ptr_pig_current-> species;
        result_data.pig_weight[i] = ptr_pig_current-> weight;
        result_data.pig_age[i] = ptr_pig_current-> age;
        ptr_pig_current = ptr_pig_current -> next_pig;
        i++;
    }

    emit SendStyData(result_data);
    return;
}

PigSty::StySpeciesSituation PigSty::CheckStySpeciesSituation() const
{
    if (ptr_pig_head == nullptr)
    {
        return PigSty::NoPig;
    }

    Pig * ptr_pig_current = ptr_pig_head;

    while (ptr_pig_current != nullptr)
    {
        if (ptr_pig_current -> species == Pig::BlackPig)
        {
            return PigSty::BlackPigExistence;
        }

        ptr_pig_current = ptr_pig_current -> next_pig;
    }

    return PigSty::BlackPigNonexistence;
}

void PigSty::DeleteAllPigs()
{
    rwlock.lockForRead();

    if (ptr_pig_head == nullptr)
    {
        return;
    }

    Pig * ptr_pig_current = ptr_pig_head;
    Pig * ptr_pig_next = ptr_pig_current -> next_pig;

    while (ptr_pig_next != nullptr)
    {
        delete ptr_pig_current;
        ptr_pig_current = ptr_pig_next;
        ptr_pig_next = ptr_pig_next -> next_pig;
    }

    delete ptr_pig_current;
    rwlock.unlock();
}
