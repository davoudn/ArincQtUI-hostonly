#ifndef GENERALDATA_H
#define GENERALDATA_H
#include <QString>
#include <QMutex>


class GeneralData
{
    GeneralData();
    static    GeneralData* instance;
public:
    static GeneralData* getInstance();
    QString LABELS_PATH;
    QString TRANSMIT_CONFIGS_PATH;
    QMutex  mutex;
};

#endif // GENERALDATA_H
