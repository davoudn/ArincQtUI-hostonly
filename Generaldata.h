#ifndef GENERALDATA_H
#define GENERALDATA_H
#include <QString>
#include <QMutex>
#include "PointerVector.h"
#include "action.h"

class GeneralData
{
    GeneralData();
    static    GeneralData* instance;
public:
    static GeneralData* getInstance();
    QString LABELS_PATH;
    QString TRANSMIT_CONFIGS_PATH;
    QMutex  mutex;
    PointerVector<BaseAction> actions;

    PointerVector<BaseAction>& getActions();
};

#endif // GENERALDATA_H
