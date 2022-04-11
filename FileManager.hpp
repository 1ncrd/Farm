#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QVector>
#include "Pig.hpp"

class FileManager
{
public:
    enum TradeType {Sell = 0, Buy};
    static const bool WriteEnable;
    static const QString SaleRecordFolderPath;
    static const QString GameDataFolderPath;
    FileManager();
    static void CreateTradeRecordFolder();
    static void CreateGameDataFolder();
    static void CreateSaleRecordFile(const QString &file_name);
    struct TradeRecord
    {
        long trade_time;
        TradeType trade_type;
        QString pig_id;
        Pig::PigSpecies pig_species;
        float pig_weight;
        int pig_age;
    };
    static std::map<int, QString> EnumToTradeTypeName;            // Literal meaning

    static void WriteTradeRecord(const TradeRecord &record, const QString &file_name);
    static QString FormatTradeRecord(const TradeRecord &record);
    static bool CreateDir(const QString &path);
    static void CreateFile(const QString &file_path, const QString &file_name);
    static void ReadTradeRecord(QVector<TradeRecord> * result_qvector,  const QString &file_name);
    static void ClearRecordFile(const QString &file_name);
};
extern FileManager * file_manager;

#endif // FILEMANAGER_H
