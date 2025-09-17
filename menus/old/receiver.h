#ifndef RECEIVER_H
#define RECEIVER_H
#include "types.h"
#include <QWidget>
#include <QString>
#include <QTimer>

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
    explicit Receiver(QWidget *parent = nullptr);
    static Receiver* instance;

public:
    ~Receiver();
    void CleanTree();
    void FillEquipmentSelector();
    static Receiver* getInstance();
    void insertLabel(bool);
    void removeLabel(bool);
    void updateModelData(Label* _baseItem);
     MyDataModel * getDataModel();
    void resetDataModel(str_t _eqId);

signals:

    void OnArincDataUpdated();

public slots:
    void OnEquipmentSelectorChanged(int _id);
    void UpdateTree();
private:
    Ui::Receiver *ui;
    MyDataModel *dataModel = nullptr;
    EquipmentsIds* equipmentsIds = nullptr;
    QString defaultEquipment;
};

#endif // RECEIVER_H
