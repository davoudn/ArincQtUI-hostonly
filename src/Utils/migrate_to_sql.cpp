#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QSqlQuery>
#include "Database.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    QString path = QDir::homePath() + "/Arinc429Data";
    QStringList labelsPath;
    labelsPath << path + "/labels";
    
    auto* db = Database::GetInstance();
    if (!db->OpenDatabase(path)) {
        qCritical() << "Failed to open database";
        return -1;
    }
    
    QDir dir(path + "/labels");
    QStringList jsonFiles = dir.entryList(QStringList() << "*.json", QDir::Files);
    
    for (const QString& jsonFile : jsonFiles) {
        QString fullPath = path + "/labels/" + jsonFile;
        auto jobj = utils::GetJsonObj(fullPath);
        
        if (!jobj.contains("Id") || !jobj.contains("Label") || !jobj.contains("Type")) {
            qDebug() << "Skipping invalid file:" << jsonFile;
            continue;
        }
        
        Database::LabelData labelData;
        labelData.id = jobj["Label"].toString();
        labelData.equipmentId = jobj["Id"].toString();
        labelData.type = jobj["Type"].toString();
        labelData.name = jobj["Name"].toString();
        
        if (jobj.contains("Params")) {
            QJsonObject paramsObj = jobj["Params"].toObject();
            labelData.params = QJsonDocument(paramsObj).toJson(QJsonDocument::Compact);
        }
        
        db->InsertLabel(labelData);
        
        if (jobj.contains("Params")) {
            QJsonObject paramsObj = jobj["Params"].toObject();
            for (const QString& paramName : paramsObj.keys()) {
                QJsonObject paramObj = paramsObj[paramName].toObject();
                Database::ParameterData paramData;
                paramData.name = paramName;
                paramData.type = paramObj["Type"].toString();
                
                if (paramObj.contains("IBit")) paramData.iBit = paramObj["IBit"].toInt();
                if (paramObj.contains("FBit")) paramData.fBit = paramObj["FBit"].toInt();
                paramData.details = QJsonDocument(paramObj).toJson(QJsonDocument::Compact);
                
                db->InsertParameter(labelData.id, labelData.equipmentId, paramData);
            }
        }
        
        qDebug() << "Migrated:" << labelData.id << "for equipment" << labelData.equipmentId;
    }
    
    qDebug() << "Migration complete!";
    return 0;
}