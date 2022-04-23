#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QString>
#include <QVector>
#include "Pig.hpp"

class FileManager : public QObject
{
    Q_OBJECT
public:
    enum TradeType {Sell = 0, Buy};
    static const bool WriteEnable;
    static const QString SaleRecordFolderPath;
    static const QString GameDataFolderPath;
    FileManager(QObject *parent = nullptr);
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
    static std::map<TradeType, QString> EnumToTradeTypeName;            // Literal meaning

    static void WriteTradeRecord(const TradeRecord &record, const QString &file_name);
    static QString FormatTradeRecord(const TradeRecord &record);
    static bool CreateDir(const QString &path);
    static void CreateFile(const QString &file_path, const QString &file_name);
    void ReadTradeRecord(QVector<TradeRecord> * result_qvector,  const QString &file_name);
    static void ClearRecordFile(const QString &file_name);
    static bool CheckFileIsEmpty(const QString &file_name);
signals:
    ReadTradeRecordCompleted();
};
extern FileManager * file_manager;

#endif // FILEMANAGER_H
