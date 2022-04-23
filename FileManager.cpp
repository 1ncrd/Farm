#include "FileManager.hpp"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QStringList>

FileManager * file_manager = new FileManager;

const bool FileManager::WriteEnable = true;
const QString FileManager::GameDataFolderPath = "./GameData/";
const QString FileManager::SaleRecordFolderPath = "./TradeRecord/";
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
    CreateDir(SaleRecordFolderPath);
}

void FileManager::CreateGameDataFolder()
{
    CreateDir(GameDataFolderPath);
}

void FileManager::CreateSaleRecordFile(const QString &file_name)
{
    CreateFile(SaleRecordFolderPath, file_name);
}

void FileManager::WriteTradeRecord(const TradeRecord &record, const QString &file_name)
{
    if (WriteEnable)
    {
        QFile file(SaleRecordFolderPath + file_name);
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream write_in(&file);
        write_in << FormatTradeRecord(record);
        file.close();
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
            qDebug() << "Create folder fail.";
        }
        else
        {
            qDebug() << "Create folder success.";
        }
    }
    else
    {
        qDebug() << path << "folder exist.";
    }

    return temp_dir.mkdir(path);
}

void FileManager::CreateFile(const QString &file_path, const QString &file_name)
{
    QFile temp_file(SaleRecordFolderPath + file_name);

    if (temp_file.exists())
    {
        qDebug() << file_name << " exists in " << file_path;
        return;
    }
    else
    {
        if (!temp_file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Create failed!";
        }

        temp_file.close();
    }
}

void FileManager::ReadTradeRecord(QVector<TradeRecord> * result_qvector, const QString &file_name)
{
    QFile temp_file(SaleRecordFolderPath + file_name);

    if (temp_file.exists() and temp_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Reading...";
        qDebug() << file_name << " exists in " << SaleRecordFolderPath;
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
    QFile temp_file(SaleRecordFolderPath + file_name);
    temp_file.open(QFile::WriteOnly | QFile::Truncate);
    temp_file.close();
}

bool FileManager::CheckFileIsEmpty(const QString &file_name)
{
    QFile temp_file(SaleRecordFolderPath + file_name);
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
