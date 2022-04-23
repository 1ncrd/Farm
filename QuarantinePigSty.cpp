#include "QuarantinePigSty.hpp"

QuarantinePigSty::QuarantinePigSty(const QString &sty_id_temp, QObject *parent)
    : PigSty{sty_id_temp, parent}
{

}

void QuarantinePigSty::GetQuarantineStyData()
{
    // Use sending signal to communicate between thread safely.
    // Critical function: `ReturnQuarantineStyData(result_data)`.
    QVector<Pig::PigInfo> result;
    bool infection_exists = false;

    if (ptr_pig_head == nullptr)
    {
        emit ReturnQuarantineStyData(result);
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

    emit ReturnQuarantineStyData(result);
    return;
}

void QuarantinePigSty::DisposeInfectedPig()
{
    this -> DeleteAllPigs();

    this -> GetQuarantineStyData();
}
