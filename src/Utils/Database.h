#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QString>
#include <memory>
#include <vector>
#include <optional>
#include <mutex>

class Database {
public:
    static Database* getInstance();
    
    bool openDatabase(const QString& path);
    void closeDatabase();
    
    bool createTables();
    
    struct LabelData {
        QString id;
        QString equipment_id;
        QString type;
        QString name;
        QString params;
    };
    
    std::optional<LabelData> getLabel(const QString& label_id, const QString& equipment_id);
    bool insertLabel(const LabelData& label);
    bool updateLabel(const LabelData& label);
    bool deleteLabel(const QString& label_id, const QString& equipment_id);
    
    struct ParameterData {
        QString name;
        QString type;
        int i_bit;
        int f_bit;
        QString details;
    };
    
    bool insertParameter(const QString& label_id, const QString& equipment_id, const ParameterData& param);
    std::vector<ParameterData> getParameters(const QString& label_id, const QString& equipment_id);

private:
    Database();
    ~Database();
    
    QSqlDatabase m_db;
    static Database* s_instance;
    static std::mutex s_mutex;
};