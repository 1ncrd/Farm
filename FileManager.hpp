#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QDateTime>
#include "Pig.hpp"
#include "PigSoldAmount.hpp"

class FileManager : public QObject
{
    Q_OBJECT
public:
    enum TradeType {Sell = 0, Buy};
    static std::map<TradeType, QString> EnumToTradeTypeName;
    struct TradeRecord
    {
        long trade_time;
        TradeType trade_type;
        QString pig_id;
        Pig::PigSpecies pig_species;
        float pig_weight;
        int pig_age;
    };
    struct PigStyInfo
    {
        PigSoldAmount pig_sold_amount;
        long money = 0;
    };
    struct GameData
    {
        QString name;
        bool never_be_edited;
        long total_time;
        QVector<Pig::PigInfo> pig_data;
        PigStyInfo pigsty_info;
        QString edit_time;
        int difficulty;
    };
    static const bool WriteTradeRecordEnable;
    static const QString TradeRecordFolderPath;
    static const QString ArchiveDataFolderPath;
    static const QString ArchiveFileSuffix;
    static const QString RecordFileSuffix;
    static const QString ArchiveFileEndMark;
    FileManager(QObject *parent = nullptr);
    static bool CreateDir(const QString &path);
    static void CreateFile(const QString &file_path, const QString &file_name);
    static void CreateGameDataFolder();
    static void CreateTradeRecordFolder();
    static void CreateArchiveFile(const QString &archive_name);
    static void WriteArchiveData(const GameData &game_data, const QString &file_name);
    static QString FormatGameDate(const GameData &game_data);
    void ReadAllArchiveData();
    GameData ReadArchiveData(const QString &file_name);
    void DeleteArchive(const QString &file_name);
    static void CreateSaleRecordFile(const QString &archive_name);
    static void WriteTradeRecord(const TradeRecord &record, const QString &file_name);
    static QString FormatTradeRecord(const TradeRecord &record);
    void ReadTradeRecord(QVector<TradeRecord> * result_qvector,  const QString &file_name);
    static void ClearRecordFile(const QString &file_name);
    static bool CheckFileIsEmpty(const QString &file_name);
    void StatisticsExistingArchives();
    static void SetCurrentArchiveName(const QString &archive_name);
    static QString GetCurrentArchiveName();
private:
    QVector<QString> current_existing_archive;
    static QString CurrentArchiveName;
signals:
    ReadTradeRecordCompleted(); // Use the pointer to trans the info.
    SendExistingArchivesList(QVector<QString> result);
    SendAllArchiveInfo(QVector<GameData> data);
    NewArchiveCreated();
};
extern FileManager * file_manager;

#endif // FILEMANAGER_H
