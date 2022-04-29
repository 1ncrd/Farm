#include "FileManager.hpp"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include <QStringBuilder>

FileManager * file_manager = new FileManager;

const bool FileManager::WriteTradeRecordEnable = true;
const QString FileManager::ArchiveDataFolderPath = "./ArchiveData/";
const QString FileManager::TradeRecordFolderPath = "./TradeRecord/";
const QString FileManager::ArchiveFileSuffix = ".dat";
const QString FileManager::RecordFileSuffix = ".rec";
const QString FileManager::ArchiveFileEndMark = "END";
QString FileManager::CurrentArchiveName;

FileManager::FileManager(QObject *parent)
    : QObject{parent}
{

}

std::map<FileManager::TradeType, QString> FileManager::EnumToTradeTypeName =
{
    {FileManager::Sell, QString("Sell")}, {FileManager::Buy, QString("Buy")},
};

void FileManager::CreateTradeRecordFolder()
{
    // Store records of sales and purchases in files.
    CreateDir(TradeRecordFolderPath);
}

void FileManager::CreateGameDataFolder()
{
    CreateDir(ArchiveDataFolderPath);
}

void FileManager::CreateSaleRecordFile(const QString &archive_name)
{
    CreateFile(TradeRecordFolderPath, archive_name + RecordFileSuffix);
}

void FileManager::WriteTradeRecord(const TradeRecord &record, const QString &file_name)
{
    if (WriteTradeRecordEnable)
    {
        QFile file(TradeRecordFolderPath + file_name + RecordFileSuffix);
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream write_in(&file);
        write_in << FormatTradeRecord(record);
        file.close();
        qDebug().noquote() << "[+] Pig ID : " + record.pig_id + ", Trade record write in.";
    }
}

QString FileManager::FormatTradeRecord(const TradeRecord &record)
{
    return QString::number(record.trade_time) + "|" +
           QString::number(record.trade_type) + "|" +
           record.pig_id + "|" +
           QString::number(record.pig_species) + "|" +
           QString::number(record.pig_weight, 'f', 1) + "|" +
           QString::number(record.pig_age) + "\n";
}

bool FileManager::CreateDir(const QString &path)
{
    QDir temp_dir;

    // If the data folder does not exist in the current directory, then create one.
    if (!temp_dir.exists(path))
    {
        bool ismkdir = temp_dir.mkdir(path);

        if (!ismkdir)
        {
            qDebug().noquote() << "[+] Create folder " + path + " fail.";
        }
        else
        {
            qDebug().noquote() << "[+] Create folder " + path + " success.";
        }
    }
    else
    {
        qDebug().noquote() << "[+] Folder " + path + " exist.";
    }

    return temp_dir.mkdir(path);
}

void FileManager::CreateFile(const QString &file_path, const QString &file_name)
{
    QFile temp_file(file_path + file_name);

    if (temp_file.exists())
    {
        qDebug().noquote() << "[+] " + file_name << " exists in " << file_path;
        return;
    }
    else
    {
        if (!temp_file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug().noquote() << "[+] File : " + file_name + " create failed!";
        }

        temp_file.close();
    }
}

void FileManager::ReadTradeRecord(QVector<TradeRecord> * result_qvector, const QString &file_name)
{
    QFile temp_file(TradeRecordFolderPath + file_name + RecordFileSuffix);

    if (temp_file.exists() and temp_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug().noquote() << "[+] Reading trade record in " + file_name + RecordFileSuffix;
    }

    QTextStream reader(&temp_file);

    while (!reader.atEnd())
    {
        QString line = reader.readLine();
        QStringList data = line.split("|");
        TradeRecord record_temp;
        record_temp.trade_time = QString(data.at(0).toLocal8Bit().constData()).toInt();
        record_temp.trade_type = TradeType(QString(data.at(1).toLocal8Bit().constData()).toInt());
        record_temp.pig_id = QString(data.at(2).toLocal8Bit().constData());
        record_temp.pig_species = Pig::PigSpecies(QString(data.at(3).toLocal8Bit().constData()).toInt());
        record_temp.pig_weight = QString(data.at(4).toLocal8Bit().constData()).toFloat();
        record_temp.pig_age = QString(data.at(5).toLocal8Bit().constData()).toInt();
        result_qvector -> append(record_temp);
    }

    temp_file.close();
    emit ReadTradeRecordCompleted();
}

void FileManager::ClearRecordFile(const QString &file_name)
{
    QFile temp_file(TradeRecordFolderPath + file_name);
    temp_file.open(QFile::WriteOnly | QFile::Truncate);
    temp_file.close();
}

bool FileManager::CheckFileIsEmpty(const QString &file_name)
{
    QFile temp_file(TradeRecordFolderPath + file_name + RecordFileSuffix);
    temp_file.open(QFile::ReadOnly);

    if (temp_file.atEnd())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void FileManager::CreateArchiveFile(const QString &archive_name)
{
    CreateFile(ArchiveDataFolderPath, archive_name + ArchiveFileSuffix);
}

void FileManager::WriteArchiveData(const GameData &game_data, const QString &file_name)
{
    QFile file_temp(ArchiveDataFolderPath + file_name + FileManager::ArchiveFileSuffix);

    file_temp.open(QIODevice::WriteOnly);
    QTextStream write_in(&file_temp);
    write_in << FormatGameDate(game_data);
    qDebug().noquote() << "[+] Archive data : " + game_data.name + " write finished.";
    file_temp.close();
}

QString FileManager::FormatGameDate(const GameData &game_data)
{
    // *******************************************************************************
    // time | never_be_ecited | total_time | difficulty | money | pig_sold_amount.BlackPig | SmallFlowerPig | BigWhitePig | total
    // piginfo....
    // *******************************************************************************
    QString result;
    QString sep('|');
    QString newline('\n');
    // Store the pig sty info.
    result = result % game_data.edit_time % sep
             % QString::number(int(game_data.never_be_edited)) % sep
             % QString::number(game_data.total_time) % sep
             % QString::number(game_data.difficulty) % sep
             % QString::number(game_data.pigsty_info.money) % sep
             % QString::number(game_data.pigsty_info.pig_sold_amount.BlackPig) % sep
             % QString::number(game_data.pigsty_info.pig_sold_amount.SmallFlowerPig) % sep
             % QString::number(game_data.pigsty_info.pig_sold_amount.BigWhitePig) % sep
             % QString::number(game_data.pigsty_info.pig_sold_amount.total) % newline;

    // Store the pig info.
    for (int i = 0; i < game_data.pig_data.length(); i++)
    {
        result = result % game_data.pig_data[i].id % sep
                 % QString::number(game_data.pig_data[i].age) % sep
                 % QString::number(game_data.pig_data[i].weight) % sep
                 % QString::number(int(game_data.pig_data[i].species)) % sep
                 % QString::number(int(game_data.pig_data[i].is_infected)) % sep
                 % QString::number(int(game_data.pig_data[i].infected_time)) % sep
                 % QString::number(int(game_data.pig_data[i].is_quarantined)) % newline;
    }

    result = result % ArchiveFileEndMark;
    return result;
}

void FileManager::ReadAllArchiveData()
{
    QVector<GameData> all_data;

    for (int i = 0; i < current_existing_archive.length(); i++)
    {
        all_data.append(ReadArchiveData(current_existing_archive[i]));
    }

    emit SendAllArchiveInfo(all_data);
}

FileManager::GameData FileManager::ReadArchiveData(const QString &file_name)
{
    // ********************************************************************************************
    // time | never_be_ecited | total_time | difficulty | money | pig_sold_amount.BlackPig | SmallFlowerPig | BigWhitePig | total
    // piginfo....
    // ********************************************************************************************

    // struct PigInfo
    // {
    //    QString id;
    //    int age;
    //    float weight;
    //    PigSpecies species;
    //    bool is_infected;
    // };

    QFile temp_file(ArchiveDataFolderPath + file_name + ArchiveFileSuffix);

    if (temp_file.exists() and temp_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug().noquote() << "[+] Reading archive in " + file_name + ArchiveFileSuffix;
    }

    GameData result_data;
    QTextStream reader(&temp_file);
    QString line;
    QStringList data;

    if (!temp_file.atEnd())
    {
        try
        {
            line = reader.readLine();
            data = line.split("|");
            result_data.name = file_name;
            result_data.edit_time = QString(data.at(0).toLocal8Bit().constData());
            result_data.never_be_edited = bool(QString(data.at(1).toLocal8Bit().constData()).toInt());
            result_data.total_time = QString(data.at(2).toLocal8Bit().constData()).toInt();
            result_data.difficulty = QString(data.at(3).toLocal8Bit().constData()).toInt();
            result_data.pigsty_info.money = QString(data.at(4).toLocal8Bit().constData()).toInt();
            result_data.pigsty_info.pig_sold_amount.BlackPig = QString(data.at(5).toLocal8Bit().constData()).toInt();
            result_data.pigsty_info.pig_sold_amount.SmallFlowerPig = QString(data.at(6).toLocal8Bit().constData()).toInt();
            result_data.pigsty_info.pig_sold_amount.BigWhitePig = QString(data.at(7).toLocal8Bit().constData()).toInt();
            result_data.pigsty_info.pig_sold_amount.total = QString(data.at(8).toLocal8Bit().constData()).toInt();

            while ((line = reader.readLine()) != ArchiveFileEndMark)
            {
                data = line.split("|");
                Pig::PigInfo pig_info;
                pig_info.id = QString(data.at(0).toLocal8Bit().constData());
                pig_info.age = TradeType(QString(data.at(1).toLocal8Bit().constData()).toInt());
                pig_info.weight = QString(data.at(2).toLocal8Bit().constData()).toFloat();
                pig_info.species = Pig::PigSpecies(QString(data.at(3).toLocal8Bit().constData()).toInt());
                pig_info.is_infected = bool(QString(data.at(4).toLocal8Bit().constData()).toInt());
                pig_info.infected_time = QString(data.at(5).toLocal8Bit().constData()).toInt();
                pig_info.is_quarantined = bool(QString(data.at(6).toLocal8Bit().constData()).toInt());
                result_data.pig_data.append(pig_info);
            }
        }
        catch (...)
        {
            qDebug() << "[#] Archive file corrupted.";
        }
    }
    else
    {
        qDebug() << "[#] Empty archive file: " << file_name;
    }

    temp_file.close();
    return result_data;
}

void FileManager::StatisticsExistingArchives()
{
    QDir dir(ArchiveDataFolderPath);

    if (!dir.exists())
    {
        return;
    }

    QStringList filters;
    filters << QString("*.dat");
    dir.setFilter (QDir::Files | QDir::NoSymLinks); // Only statistic files.
    dir.setNameFilters(filters);

    int dir_count = dir.count();

    QVector<QString> result;

    if (dir_count <= 0)
    {
        current_existing_archive = result;
        emit SendExistingArchivesList(result);
        return;
    }

    for (int i = 0; i < dir_count; i++)
    {
        int len = QString(dir[i]).length();
        QString file_name = QString(dir[i]).left(len - 4);
        result.append(file_name);
    }

    current_existing_archive = result;
    emit SendExistingArchivesList(result);
}

void FileManager::DeleteArchive(const QString &file_name)
{
    if (QFile::exists(ArchiveDataFolderPath % file_name % ArchiveFileSuffix))
    {
        QFile::remove(ArchiveDataFolderPath % file_name % ArchiveFileSuffix);
        QFile::remove(TradeRecordFolderPath % file_name % RecordFileSuffix);
        qDebug() << "[+] Archive " + file_name + " deleted (trade record deleted too).";
    }
}

void FileManager::SetCurrentArchiveName(const QString &archive_name)
{
    CurrentArchiveName = archive_name;
}

QString FileManager::GetCurrentArchiveName()
{
    return CurrentArchiveName;
}
