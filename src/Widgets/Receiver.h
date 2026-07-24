#ifndef RECEIVER_H
#define RECEIVER_H
#include "types.h"
#include <QWidget>
#include <QString>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include "Utils/Singleton.h"

class EquipmentsIds;
class Equipment;
class MyDataModel;
class BaseItem;
class Label;
class ReceiverWorker;
namespace Ui 
{
class Receiver;
}

class Receiver : public QWidget, public SingletonArray<Receiver, 4>
{
    Q_OBJECT
    friend class SingletonArray<Receiver, 4>;
public:
    Receiver(QWidget *parent = nullptr, int ch = 0);
public:
    uint8_t chanell = 0;
    ~Receiver();
    void cleanTree();
    void fillEquipmentSelector();
    void updateModelData(Label* base_item);
    void evalDataRates(uint32_t interval);
    void idleLabelCleaner();
    void enable();
    void disable();
    void resetDataModel(str_t eq_id);
    void initUiCombos();
    void removeLabel(bool if_checked);
    bool setLabelData(str_t label_id, const QVariant &value);
    bool setLabelData(str_t label_id
        , const float& rate, const QVariant &value);
    MyDataModel* getDataModel();
    static Receiver* getInstance(int ch);
signals:
    void onArincDataUpdated();
public slots:
    void onEquipmentSelectorChanged(int id);
    void updateTree();
    void onSdiBitrate(int index);
    void onReceiverEnabled(bool);
    void onReceiverDisabled(bool);
public:
    uint8_t dei = 0;
protected:
    Ui::Receiver *ui = nullptr;
    MyDataModel *dataModel = nullptr;
    EquipmentsIds* equipmentsIds = nullptr;
    uint8_t deiChanell = 0;
    QMutex model_mutex;
    QString equipmentId;
    bool bIfEnabled = false;
};

#endif // RECEIVER_H