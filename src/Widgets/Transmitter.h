#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include "PointerVector.h"
#include "types.h"
#include <QWidget>
#include <vector>
#include <QMutex>
#include "singleton.h"

class EditorDelegate;
class Equipment;
class EquipmentsIds;
class MyDataModel;
class DArincData;
class BaseEvent;

namespace Ui {
class transmitter;
}

class transmitter : public QWidget, public SingletonArray<transmitter, 2>
{
    Q_OBJECT
    friend class SingletonArray<transmitter, 2>;
    int m_channel = 0;

public:
    transmitter(QWidget *parent = nullptr, int ch = 0);
    ~transmitter();
    static transmitter* getInstance(int ch);

    void fillEquipmentSelector();
    void fillLabelSelector();
    void setEditorDelegate();
    void unsetEditorDelegate();
    MyDataModel* getDataModel();
    void resetDataModel(str_t eq_id);
    void initUiCombos();
    void selectEquipment(str_t eq_id);
    std::vector<DArincData> getListOfAvailableLabelData();
    void incrementLabelsDataRateCounter();
    QMutex model_mutex;
    void disable();
    void enable();
    int dei = 0;
    int dei_channel = 0;
    int channel = 0;
public slots:
    void onEquipmentSelectorChanged(int);
    void onLabelSelectorChanged(int);
    void addLabel(bool);
    void removeLabel(bool);
    void onArincParityBitRate(int index);
    void onSelectSaveConfigFile(bool);
    void onSelectLoadConfigFile(bool);
    void onTransmitterEnabled(bool);
    void onTransmitterDisabled(bool);
    void onLoadConfig(bool);

protected:
    uint64_t control_word = 0;
    Ui::transmitter *ui = nullptr;
    MyDataModel *data_model = nullptr;
    EquipmentsIds* equipments_ids = nullptr;
    str_t equipment_id;
    str_t label_id;
    str_t default_equipment;
    EditorDelegate* editor_delegate = nullptr;
    str_t selected_label_id;
    str_t config_file_name;
    bool if_enabled = false;

};

#endif // TRANSMITTER_H