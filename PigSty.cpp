#include "PigSty.hpp"
#include "Pig.hpp"
#include <QDebug>
#include "MyRandom.hpp"

const int PigSty::PigLiveTimeAfterInfected = 10;
long PigSty::money = 0;
PigAmount PigSty::pig_sold_amount{0, 0, 0, 0};
int PigSty::InfectionTransRateInSty = 50;
int PigSty::InfectionTransRateAcrossSty = 15;
PigSty * PigSty::temp_instance = new PigSty("Temp Instance");
QString PigSty::ArchiveName;

PigSty::PigSty(QObject *parent)
    : QObject{parent}
{
    this -> ptr_pig_head = nullptr;
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

    this -> ptr_pig_head = nullptr;
}

PigSty::~PigSty()
{
    this -> DeleteAllPigs();
    qDebug() << "PigSty released." << id;
}

void PigSty::AddPig(int number/* = 1*/)
{
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
        AddPigAmount(ptr_pig_current -> species);
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
        AddPigAmount(ptr_pig_current -> species);
        ptr_pig_current = ptr_pig_current -> next_pig;
    }

}

void PigSty::AppendPig(Pig * const &ptr_pig_to_append_head)
{
    if (ptr_pig_to_append_head == nullptr)
    {
        return;
    }

    if (ptr_pig_head == nullptr)
    {
        ptr_pig_head = ptr_pig_to_append_head;
        auto ptr_to_count = ptr_pig_to_append_head;

        // Count the added amount.
        while (ptr_to_count != nullptr)
        {
            AddPigAmount(ptr_to_count -> species);
            ptr_to_count = ptr_to_count -> next_pig;
        }
    }
    else
    {
        Pig * ptr_pig_current = ptr_pig_head;

        while (ptr_pig_current -> next_pig != nullptr)
        {
            ptr_pig_current = ptr_pig_current -> next_pig;
        }

        ptr_pig_current -> next_pig = ptr_pig_to_append_head;
        ptr_pig_to_append_head -> previous_pig = ptr_pig_current;

        auto ptr_to_count = ptr_pig_to_append_head;

        // Count the added amount.
        while (ptr_to_count != nullptr)
        {
            AddPigAmount(ptr_pig_current -> species);
            ptr_to_count = ptr_to_count -> next_pig;

        }
    }
}

void PigSty::AddTo10Pig()
{
    int number = 10 - this -> GetPigTotalAmount();
    AddPig(number);
}

int PigSty::SellPig()
/*
 * Sell pigs meet the requirements.
 */
{
    if (ptr_pig_head == nullptr)
    {
        return 0;
    }

    int sale_money = 0;
    Pig * ptr_pig_current = ptr_pig_head;
    Pig * ptr_pig_next = ptr_pig_head -> next_pig;

    while (ptr_pig_next != nullptr)
    {
        if (ptr_pig_current -> weight > 75 or ptr_pig_current -> age > 365)
        {
            assert(ptr_pig_current != nullptr);
            sale_money += Pig::SpeciesPrice[ptr_pig_current->species];
            AddMoney(Pig::SpeciesPrice[ptr_pig_current -> species]);
            AddSoldAmountPig(ptr_pig_current -> species);
            RecordTrade(FileManager::Sell, ptr_pig_current);
            DeletePig(ptr_pig_current);
        }

        ptr_pig_current = ptr_pig_next;
        ptr_pig_next = ptr_pig_next -> next_pig;
    }

    if (ptr_pig_current -> weight > 75 or ptr_pig_current -> age > 365)
    {
        assert(ptr_pig_current != nullptr);
        sale_money += Pig::SpeciesPrice[ptr_pig_current -> species];
        AddMoney(Pig::SpeciesPrice[ptr_pig_current -> species]);
        AddSoldAmountPig(ptr_pig_current -> species);
        RecordTrade(FileManager::Sell, ptr_pig_current);
        DeletePig(ptr_pig_current);
    }

    if (id.toInt() == 99)
    {
        emit SellPigFinished();
    }

    return sale_money;
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
    FileManager::WriteTradeRecord(sale_record_temp, ArchiveName);
}

void PigSty::DeletePig(Pig * const &ptr_pig_to_delete)
{
    if (ptr_pig_to_delete == nullptr)
    {
        return;
    }

    // When the ptr_pig_to_delete at the beginning of the list.
    if (ptr_pig_to_delete == ptr_pig_head)
    {
        ptr_pig_head = ptr_pig_head -> next_pig;

        if (ptr_pig_head != nullptr)
        {
            ptr_pig_head -> previous_pig = nullptr;
        }

        ReducePigAmount(ptr_pig_to_delete -> species);
        delete ptr_pig_to_delete;
    }
    else
    {
        ptr_pig_to_delete -> previous_pig -> next_pig = ptr_pig_to_delete -> next_pig;

        if (ptr_pig_to_delete -> next_pig != nullptr)
        {
            ptr_pig_to_delete -> next_pig -> previous_pig = ptr_pig_to_delete -> previous_pig;
        }

        ReducePigAmount(ptr_pig_to_delete -> species);
        delete ptr_pig_to_delete;
    }

}

void PigSty::LetAllPigGrow()
{
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
}

void PigSty::InfectOnePig()
{
    if (ptr_pig_head == nullptr)
    {
        return;
    }

    int infected_pig_num = Random() % pig_amount.total;
    Pig * ptr_pig_current = ptr_pig_head;

    while (ptr_pig_current != nullptr and infected_pig_num > 0)
    {
        ptr_pig_current = ptr_pig_current -> next_pig;
        infected_pig_num--;
    }

    if (ptr_pig_current != nullptr)
    {
        ptr_pig_current -> is_infected = true;
        ptr_pig_current -> infected_time = 0;
    }
}

void PigSty::InfectionSpreadInSty()
{
    if (ptr_pig_head == nullptr)
    {
        return;
    }

    Pig * ptr_pig_current = ptr_pig_head;

    if (this -> InfectionExists())
    {
        // If the infection exists, start spreading.
        while (ptr_pig_current != nullptr)
        {
            if (!(ptr_pig_current -> is_infected) and (Probability(InfectionTransRateInSty)))
            {
                ptr_pig_current -> is_infected = true;
            }

            ptr_pig_current = ptr_pig_current -> next_pig;
        }
    }
}

void PigSty::InfectionSpreadFromOthers()
{
    if (ptr_pig_head == nullptr)
    {
        return;
    }

    Pig * ptr_pig_current = ptr_pig_head;

    while (ptr_pig_current != nullptr)
    {
        if (!(ptr_pig_current -> is_infected) and (Probability(InfectionTransRateAcrossSty)))
        {
            ptr_pig_current -> is_infected = true;
        }

        ptr_pig_current = ptr_pig_current -> next_pig;
    }
}

bool PigSty::InfectionExists()
{
    if (ptr_pig_head == nullptr)
    {
        return false;
    }

    Pig * ptr_pig_current = ptr_pig_head;

    while (ptr_pig_current != nullptr)
    {
        if (ptr_pig_current -> is_infected)
        {
            return true;
        }

        ptr_pig_current = ptr_pig_current -> next_pig;
    }

    return false;
}

void PigSty::InfectedPigDiedAfterTime()
{
    // The pigs died after being infected for some time.
    if (ptr_pig_head == nullptr)
    {
        return;
    }

    Pig * ptr_pig_current = ptr_pig_head;
    Pig * ptr_pig_next = ptr_pig_head -> next_pig;

    while (ptr_pig_next != nullptr)
    {
        if (ptr_pig_current -> is_infected
                and ptr_pig_current -> infected_time >= PigLiveTimeAfterInfected)
        {
            DeletePig(ptr_pig_current);
        }

        ptr_pig_current = ptr_pig_next;
        ptr_pig_next = ptr_pig_next -> next_pig;
    }

    if (ptr_pig_current -> is_infected
            and ptr_pig_current -> infected_time >= PigLiveTimeAfterInfected)
    {
        DeletePig(ptr_pig_current);
    }
}

Pig * PigSty::ExtractInfectedPigs()
{
    if (ptr_pig_head == nullptr)
    {
        return nullptr;
    }

    Pig * result_pig_head = nullptr;
    Pig * result_pig_current = nullptr;
    // The operation about deletion need two pointers.
    Pig * ptr_pig_current = ptr_pig_head;
    Pig * ptr_pig_next = ptr_pig_head -> next_pig;

    while (ptr_pig_next != nullptr)
    {
        if (ptr_pig_current -> is_infected)
        {
            if (result_pig_head == nullptr)
            {
                ExtractPig(ptr_pig_current);
                result_pig_head = ptr_pig_current;
                result_pig_head -> previous_pig = nullptr;
                result_pig_head -> next_pig = nullptr;
                result_pig_head -> is_quarantined = true;
                result_pig_current = result_pig_head;
            }
            else
            {
                ExtractPig(ptr_pig_current);
                result_pig_current -> next_pig = ptr_pig_current;
                result_pig_current -> next_pig -> previous_pig = result_pig_current;
                result_pig_current = result_pig_current -> next_pig;
                result_pig_current -> next_pig = nullptr;
                result_pig_current -> is_quarantined = true;
            }
        }

        ptr_pig_current = ptr_pig_next;
        ptr_pig_next = ptr_pig_next -> next_pig;
    }

    if (ptr_pig_current -> is_infected)
    {
        if (result_pig_head == nullptr)
        {
            ExtractPig(ptr_pig_current);
            result_pig_head = ptr_pig_current;
            result_pig_head -> previous_pig = nullptr;
            result_pig_head -> next_pig = nullptr;
            result_pig_head -> is_quarantined = true;
            result_pig_current = result_pig_head;
        }
        else
        {
            ExtractPig(ptr_pig_current);
            result_pig_current -> next_pig = ptr_pig_current;
            result_pig_current -> next_pig -> previous_pig = result_pig_current;
            result_pig_current -> next_pig -> next_pig = nullptr;
            result_pig_current = result_pig_current -> next_pig;
            result_pig_current -> is_quarantined = true;
        }
    }

    return result_pig_head;
}

void PigSty::ExtractPig(Pig * pig_middle)
{
    if (pig_middle == nullptr)
    {
        return;
    }

    if (pig_middle == ptr_pig_head)
    {
        ptr_pig_head = ptr_pig_head -> next_pig;

        if (ptr_pig_head != nullptr)
        {
            ptr_pig_head -> previous_pig = nullptr;
        }

        ReducePigAmount(pig_middle -> species);
    }
    else
    {
        pig_middle -> previous_pig -> next_pig = pig_middle -> next_pig;

        if (pig_middle -> next_pig != nullptr)
        {
            pig_middle -> next_pig -> previous_pig = pig_middle -> previous_pig;
        }

        ReducePigAmount(pig_middle -> species);
    }

}

QString PigSty::GetID() const
{
    return this -> id;
}

int PigSty::GetPigTotalAmount() const
{
    return this -> pig_amount.total;
}

PigAmount PigSty::GetPigAmount() const
{
    return pig_amount;
}

void PigSty::AddPigAmount(Pig::PigSpecies species)
{
    switch (species)
    {
        case Pig::BlackPig:
            pig_amount.BlackPig += 1;
            break;

        case Pig::SmallFlowerPig:
            pig_amount.SmallFlowerPig += 1;
            break;

        case Pig::BigWhitePig:
            pig_amount.BigWhitePig += 1;
            break;
    }

    pig_amount.total += 1;
}

void PigSty::ReducePigAmount(Pig::PigSpecies species)
{
    switch (species)
    {
        case Pig::BlackPig:
            pig_amount.BlackPig -= 1;
            break;

        case Pig::SmallFlowerPig:
            pig_amount.SmallFlowerPig -= 1;
            break;

        case Pig::BigWhitePig:
            pig_amount.BigWhitePig -= 1;
            break;
    }

    pig_amount.total -= 1;
}

void PigSty::GetStyData_ReturnWithSignal()
{
    // Use sending signal to communicate between thread safely.
    // Critical function: `SendStyData(result_data)`.
    QVector<Pig::PigInfo> result;
    bool infection_exists = false;

    if (ptr_pig_head == nullptr)
    {
        emit ReturnStyData(result);
        return;
    }

    Pig * ptr_pig_current = ptr_pig_head;

    while (ptr_pig_current != nullptr)
    {
        Pig::PigInfo pig_info;
        pig_info.id = ptr_pig_current-> id;
        pig_info.species = ptr_pig_current-> species;
        pig_info.weight = ptr_pig_current-> weight;
        pig_info.age = ptr_pig_current-> age;
        pig_info.is_infected = ptr_pig_current -> is_infected;

        if (pig_info.is_infected)
        {
            infection_exists = true;
        }

        result.append(pig_info);

        ptr_pig_current = ptr_pig_current -> next_pig;
    }

    emit ReturnStyData(result);
    return;
}

QVector<Pig::PigInfo> PigSty::GetStyData_ReturnWithValue()
{
    QVector<Pig::PigInfo> result;
    bool infection_exists = false;

    if (ptr_pig_head == nullptr)
    {
        return result;
    }

    Pig * ptr_pig_current = ptr_pig_head;

    while (ptr_pig_current != nullptr)
    {
        Pig::PigInfo pig_info;
        pig_info.id = ptr_pig_current-> id;
        pig_info.species = ptr_pig_current-> species;
        pig_info.weight = ptr_pig_current-> weight;
        pig_info.age = ptr_pig_current-> age;
        pig_info.is_infected = ptr_pig_current -> is_infected;
        pig_info.is_quarantined = ptr_pig_current -> is_quarantined;
        pig_info.infected_time = ptr_pig_current -> infected_time;

        if (pig_info.is_infected)
        {
            infection_exists = true;
        }

        result.append(pig_info);

        ptr_pig_current = ptr_pig_current -> next_pig;
    }

    return result;
}

bool PigSty::CheckStyIsInfected()
{
    bool is_infected = false;

    if (ptr_pig_head == nullptr)
    {
        emit ReturnIsInfected(is_infected);
        return is_infected;
    }

    Pig * ptr_pig_current = ptr_pig_head;

    while (ptr_pig_current != nullptr)
    {
        if (ptr_pig_current -> is_infected)
        {
            is_infected = true;
        }

        ptr_pig_current = ptr_pig_current -> next_pig;
    }

    emit ReturnIsInfected(is_infected);
    return is_infected;
}

int PigSty::CountInfectedAmount()
{
    int infected_amount = 0;
    Pig * ptr_pig_current = ptr_pig_head;

    while (ptr_pig_current != nullptr)
    {
        if (ptr_pig_current -> is_infected)
        {
            infected_amount++;
        }

        ptr_pig_current = ptr_pig_current -> next_pig;
    }

    return infected_amount;
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
    ptr_pig_head = nullptr;
}

void PigSty::SetSoldAmount(const PigAmount &amount)
{
    pig_sold_amount.BlackPig = amount.BlackPig;
    pig_sold_amount.SmallFlowerPig = amount.SmallFlowerPig;
    pig_sold_amount.BigWhitePig = amount.BigWhitePig;
    pig_sold_amount.total = amount.total;

    emit GetInstance() -> SoldAmountUpdate();
}

void PigSty::AddSoldAmountPig(const Pig::PigSpecies &species, const int &amount/* = 1*/)
{
    switch (species)
    {
        case Pig::BlackPig:
            pig_sold_amount.BlackPig += amount;
            break;

        case Pig::SmallFlowerPig:
            pig_sold_amount.SmallFlowerPig += amount;
            break;

        case Pig::BigWhitePig:
            pig_sold_amount.BigWhitePig += amount;
            break;
    }

    pig_sold_amount.total += amount;
    emit GetInstance() -> SoldAmountUpdate();
}

void PigSty::SetMoney(const int &amount)
{
    money = amount;
    emit GetInstance() -> MoneyUpdate();
}

void PigSty::AddMoney(const int &amount)
{
    money += amount;
    emit GetInstance() -> MoneyUpdate();
}

PigSty * PigSty::GetInstance()
{
    return temp_instance;
}

void PigSty::DeleteInstance()
{
    delete temp_instance;
    temp_instance = nullptr;
}

void PigSty::SetArchiveName(const QString &name)
{
    ArchiveName = name;
}
