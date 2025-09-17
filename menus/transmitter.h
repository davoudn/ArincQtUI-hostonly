#ifndef TRANSMITTER_H
#define TRANSMITTER_H


#include "types.h"
#include "action.h"
#include <QWidget>
#include <vector>
#include <QMutex>


class EditorDelegate;
class Equipment;
class EquipmentsIds;
class MyDataModel;
class DArincData;

namespace Ui {
class transmitter;
}

class transmitter : public QWidget
{
    Q_OBJECT
    explicit transmitter(QWidget *parent = nullptr);
    static transmitter* instance;


public:
    ~transmitter();
    static transmitter* getInstance();

    void FillEquipmentSelector();
    void FillLabelSelector();
    void setEditorDelegate();
    void unsetEditorDelegate();
    MyDataModel * getDataModel();
    void resetDataModel(str_t _eqId);
    void initUiCombos();
    void selectEquipment(str_t eqId);
    std::vector<DArincData> getListOfAvailableLabelData();
    void incrementLabelsDataRateCounter();
    QMutex modelMutex;
    void Disable();
    void Enable();
    std::vector<action>& getActions();

public slots:
    void OnEquipmentSelectorChanged(int);
    void OnLabelSelectorChanged(int _selectorindexd);
    void addLabel(bool);
    void removeLabel(bool);
    void onArinc_parity_bitRate(int index);
    void onSelectSaveConfigFile(bool);
    void onSelectLoadConfigFile(bool);
    void onTransmitterEnabled(bool);
    void onTransmitterDisabled(bool);
    void onLoadConfig(bool);

protected:
    Ui::transmitter *ui;
    MyDataModel *dataModel = nullptr;
    EquipmentsIds* equipmentsIds = nullptr;
    str_t equipmentId;
    str_t labelId;
    str_t defaultEquipment;
    EditorDelegate* editorDelegate = nullptr;
    str_t selectedLabelId;
    str_t configFileName;
    bool bIfEnabled = false;

};

#endif // TRANSMITTER_H
