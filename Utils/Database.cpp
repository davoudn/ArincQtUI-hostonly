#include "Database.h"
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <mutex>

static Database* s_instance = nullptr;
static std::mutex s_mutex;

Database* Database::getInstance()
{
    std::lock_guard<std::mutex> lock(s_mutex);
    if (!s_instance) {
        s_instance = new Database();
    }
    return s_instance;
}

Database::Database()
{
}

Database::~Database()
{
    closeDatabase();
}

bool Database::openDatabase(const QString& path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    QString db_path = path.isEmpty() ? "labels.db" : path + "/labels.db";
    m_db.setDatabaseName(db_path);
    
    if (!m_db.open()) {
        qCritical() << "Failed to open database:" << m_db.lastError().text();
        return false;
    }
    
    return createTables();
}
    
    return CreateTables();
}

void Database::closeDatabase()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool Database::createTables()
{
    QSqlQuery query(m_db);
    
    QString create_labels_table = 
        "CREATE TABLE IF NOT EXISTS labels ("
        "label_id TEXT NOT NULL,"
        "equipment_id TEXT NOT NULL,"
        "type TEXT,"
        "name TEXT,"
        "params TEXT,"
        "PRIMARY KEY (label_id, equipment_id)"
        ")";
    
    if (!query.exec(create_labels_table)) {
        qCritical() << "Failed to create labels table:" << query.lastError().text();
        return false;
    }
    
    QString create_params_table = 
        "CREATE TABLE IF NOT EXISTS parameters ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "label_id TEXT NOT NULL,"
        "equipment_id TEXT NOT NULL,"
        "name TEXT NOT NULL,"
        "type TEXT NOT NULL,"
        "i_bit INTEGER DEFAULT 0,"
        "f_bit INTEGER DEFAULT 0,"
        "details TEXT,"
        "FOREIGN KEY (label_id, equipment_id) REFERENCES labels(label_id, equipment_id)"
        ")";
    
    if (!query.exec(create_params_table)) {
        qCritical() << "Failed to create parameters table:" << query.lastError().text();
        return false;
    }
    
    return true;
}

std::optional<Database::LabelData> Database::getLabel(const QString& label_id, const QString& equipment_id)
{
    if (!m_db.isOpen()) {
        return std::nullopt;
    }
    QSqlQuery query(m_db);
    query.prepare("SELECT id, equipment_id, type, name, params FROM labels WHERE label_id = ? AND equipment_id = ?");
    query.addBindValue(label_id);
    query.addBindValue(equipment_id);
    
    if (query.exec() && query.next()) {
        LabelData data;
        data.id = query.value("id").toString();
        data.equipment_id = query.value("equipment_id").toString();
        data.type = query.value("type").toString();
        data.name = query.value("name").toString();
        data.params = query.value("params").toString();
        return data;
    }
    
    return std::nullopt;
}

bool Database::insertLabel(const LabelData& label)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT OR REPLACE INTO labels (label_id, equipment_id, type, name, params) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(label.id);
    query.addBindValue(label.equipment_id);
    query.addBindValue(label.type);
    query.addBindValue(label.name);
    query.addBindValue(label.params);
    
    if (!query.exec()) {
        qCritical() << "Failed to insert label:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool Database::updateLabel(const LabelData& label)
{
    return insertLabel(label);
}

bool Database::deleteLabel(const QString& label_id, const QString& equipment_id)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM labels WHERE label_id = ? AND equipment_id = ?");
    query.addBindValue(label_id);
    query.addBindValue(equipment_id);
    
    return query.exec();
}

bool Database::insertParameter(const QString& label_id, const QString& equipment_id, const ParameterData& param)
{
    if (!m_db.isOpen()) {
        return false;
    }
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO parameters (label_id, equipment_id, name, type, i_bit, f_bit, details) VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(label_id);
    query.addBindValue(equipment_id);
    query.addBindValue(param.name);
    query.addBindValue(param.type);
    query.addBindValue(param.i_bit);
    query.addBindValue(param.f_bit);
    query.addBindValue(param.details);
    
    if (!query.exec()) {
        qCritical() << "Failed to insert parameter:" << query.lastError().text();
        return false;
    }
    
    return true;
}

std::vector<Database::ParameterData> Database::getParameters(const QString& label_id, const QString& equipment_id)
{
    std::vector<ParameterData> params;
    
    if (!m_db.isOpen()) {
        return params;
    }
    QSqlQuery query(m_db);
    query.prepare("SELECT name, type, i_bit, f_bit, details FROM parameters WHERE label_id = ? AND equipment_id = ?");
    query.addBindValue(label_id);
    query.addBindValue(equipment_id);
    
    if (query.exec()) {
        while (query.next()) {
            ParameterData param;
            param.name = query.value("name").toString();
            param.type = query.value("type").toString();
            param.i_bit = query.value("i_bit").toInt();
            param.f_bit = query.value("f_bit").toInt();
            param.details = query.value("details").toString();
            params.push_back(param);
        }
    }
    
    return params;
}