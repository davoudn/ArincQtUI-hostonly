#ifndef RECEIVER_H
#define RECEIVER_H
#include "types.h"
#include <QWidget>
#include <QString>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>


class EquipmentsIds;
class Equipment;
class MyDataModel;
class BaseItem;
class Label;
//
namespace Ui {
class Receiver;
}

class Receiver : public QWidget
{
    Q_OBJECT
    explicit Receiver(QWidget *parent = nullptr, uint8_t ch = 0);
    static Receiver* instance0;
    static Receiver* instance1;
    static Receiver* instance2;
    static Receiver* instance3;

public:
    ~Receiver();
    void CleanTree();
    void FillEquipmentSelector();
    static Receiver* getInstance(uint8_t ch);
    void updateModelData(Label* _baseItem);
     MyDataModel * getDataModel();
    void resetDataModel(str_t _eqId);
    void initUiCombos();
    void removeLabel(bool _ifChecked);

    void Enable();
    void Disable();
signals:

    void OnArincDataUpdated();

public slots:
    void OnEquipmentSelectorChanged(int _id);
    void UpdateTree();
    void on_SDI_bitRate(int index);
    void onReceiverEnabled(bool);
    void onReceiverDisabled(bool);
    bool setLabelData(uint32_t dei, uint32_t ch, str_t labelId, const float& rate, const QVariant &value);
    void evalDataRates();
    void idleLabelCleaner();

public:
    void makeDeviceIndex();
    uint32_t chanell = 0;
    uint32_t deiChanell = 0;
    uint32_t dei = 0;
protected:
    Ui::Receiver *ui;
    MyDataModel *dataModel = nullptr;
    EquipmentsIds* equipmentsIds = nullptr;
    QString equipmentId;
    QMutex modelMutex;
    QTimer  idleLabelCleanerTimer ;
    QTimer  dataRateTimer ;
    bool bIfEnabled = false;
};

#endif // RECEIVER_H
