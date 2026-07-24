#ifndef Transmitter_H
#define Transmitter_H

#include "PointerVector.h"
#include "Types.h"
#include "Singleton.h"

#include <QWidget>
#include <vector>
#include <QMutex>

class EditorDelegate;
class Equipment;
class EquipmentsIds;
class MyDataModel;
class DArincData;
class BaseEvent;

namespace Ui {
class Transmitter;
}

class Transmitter : public QWidget, public SingletonArray<Transmitter, 2>
{
    Q_OBJECT
    int m_channel = 0;

public:
    Transmitter(QWidget *parent = nullptr, int ch = 0);
    ~Transmitter();
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
    int dei = 0;
    int dei_channel = 0;
    int channel = 0;
    uint64_t control_word = 0;
    Ui::Transmitter *ui = nullptr;
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

#endif // Transmitter_H