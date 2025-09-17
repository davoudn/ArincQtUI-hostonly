#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <QWidget>
#include "types.h"

class EditorDelegate;
class Equipment;
class EquipmentsIds;
class MyDataModel;

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
public slots:
    void OnEquipmentSelectorChanged(int);
    void OnLabelSelectorChanged(int _selectorindexd);
    void addLabel(bool);
    void removeLabel(bool);

private:
    Ui::transmitter *ui;
    MyDataModel *dataModel = nullptr;
    EquipmentsIds* equipmentsIds = nullptr;
    str_t equipmentId;
    str_t labelId;
    str_t defaultEquipment;
    EditorDelegate* editorDelegate = nullptr;
    str_t selectedLabelId;
};

#endif // TRANSMITTER_H
