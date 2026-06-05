#include "Transmitter.h"
#include "ui_Transmitter.h"
#include "equipmentsids.h"
#include "TransmitterWorker.h"
#include "Equipment.h"
#include "editordelegate.h"
#include "TreeDataModel.h"
#include "DEI1016RasberryConfigurations.h"
#include "utils.h"
#include "generaldata.h"
#include "ArincData.h"
#include "action.h"
#include "DEI1016.h"

#include <vector>
#include <thread>

void transmitter::initUiCombos()
{
    ui->cArinc_parity_bitRate->addItem("12.5 Kbps/ODD  Parity");
    ui->cArinc_parity_bitRate->addItem("100  Kbps/ODD  Parity");
    ui->cArinc_parity_bitRate->addItem("12.5 Kbps/EVEN Parity");
    ui->cArinc_parity_bitRate->addItem("100  Kbps/EVEN Parity");
}

void transmitter::ResetDataModel(str_t eq_id)
{
     ui->treeView->setModel(nullptr);
     auto old_data_model = data_model;
     
     TransmitterWorker::getInstance(channel)->getEquipment()->init(eq_id);
     data_model = new MyDataModel(this, TransmitterWorker::getInstance(channel)->getEquipments(), true);
     ui->treeView->setModel(data_model);
     data_model->addReservedLabel();
     
     ui->treeView->setColumnWidth(0, 300);
     ui->treeView->setColumnWidth(1, 60);
     ui->treeView->setColumnWidth(2, 120);
     ui->treeView->setColumnWidth(3, 330);
     ui->treeView->setColumnWidth(4, 600);
     ui->treeView->setColumnWidth(5, 50);
     
     emit data_model->layoutChanged();
     FillLabelSelector();
     
     if (old_data_model)
         delete old_data_model;
 }

transmitter::transmitter(QWidget *parent, int ch):
    QWidget(parent), m_channel(ch), channel(ch),
    ui(new Ui::transmitter), data_model(new MyDataModel(this, TransmitterWorker::getInstance(std::min(ch, 1))->getEquipments(), true))
{
    if (ch >= static_cast<int>(CHANNEL_DEI_MAPPING.size()) || ch < 0) {
        qWarning() << "Invalid channel:" << ch << ", defaulting to 0";
        channel = 0;
        m_channel = 0;
    }
    auto mapping = CHANNEL_DEI_MAPPING[channel];
    dei = mapping.first;
    dei_channel = mapping.second;
    
ui->setupUi(this);
    equipments_ids = new EquipmentsIds();
    FillEquipmentSelector();
     equipment_id = "002";
     ResetDataModel(equipment_id);
    SelectEquipment(equipment_id);
    
    SetEditorDelegate();
    setObjectName("Transmitter thread.");
    initUiCombos();
    
    TransmitterWorker::getInstance(channel)->startTasks();
    connect(ui->equipmentSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(onEquipmentSelectorChanged(int)));
    connect(ui->labelSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(onLabelSelectorChanged(int)));
    connect(ui->addLabel, SIGNAL(clicked(bool)), this, SLOT(addLabel(bool)));
    connect(ui->removeLabel, SIGNAL(clicked(bool)), this, SLOT(removeLabel(bool)));
    connect(ui->cArinc_parity_bitRate, SIGNAL(currentIndexChanged(int)), this, SLOT(onArincParityBitRate(int)));
    connect(ui->pSaveConfig, SIGNAL(clicked(bool)), this, SLOT(onSelectSaveConfigFile(bool)));
    connect(ui->pLoadConfig, SIGNAL(clicked(bool)), this, SLOT(onSelectLoadConfigFile(bool)));
    connect(ui->chTransmitterEnabled, &QCheckBox::clicked, this, &transmitter::onTransmitterEnabled);
    connect(ui->chTransmitterDisabled, &QCheckBox::clicked, this, &transmitter::onTransmitterDisabled);
    
    this->setWindowTitle("Transmitter " + QString::number(channel));
    
    ui->chTransmitterDisabled->setChecked(true);
    ui->chTransmitterEnabled->setChecked(false);
    if_enabled = false;
}

transmitter::~transmitter()
{
    delete ui;
    delete equipments_ids;
    delete editor_delegate;
    delete data_model;
}

MyDataModel* transmitter::getDataModel()
{
    return data_model;
}

void transmitter::onArincParityBitRate(int index)
{
    QMutexLocker locker(&GeneralData::getInstance()->mutex);
    auto control_word = DEI1016::getInstance()->setControlWord_transmitter_32Bits(0,index);
    GeneralData::getInstance()->getActions().push_back(
                   MakeControlAction(dei, static_cast<uint16_t>(control_word.to_ulong())));
}

std::vector<DArincData> transmitter::GetListOfAvailableLabelData()
{
    std::vector<DArincData> list;
    if (if_enabled)
    {
        if (data_model){
            list = data_model->getListOfAvailableLabelData();
        }
    }
    return list;
}

void transmitter::incrementLabelsDataRateCounter()
{
    if (if_enabled)
    {
        if (data_model){
            data_model->incrementLabelsDataRateCounter();
        }
    }
}

void transmitter::addLabel(bool if_checked)
{
     Disable();
     data_model->addLabel(selected_label_id);
     Enable();
     emit data_model->layoutChanged();
 }
 
 void transmitter::removeLabel(bool if_checked)
 {
     const QModelIndex index = ui->treeView->selectionModel()->currentIndex();
     QAbstractItemModel *model = ui->treeView->model();
 
     Disable();
     if (index.row()< data_model->rowCount(index.parent())-1){
         if (!model->removeRow(index.row(), index.parent())){
             return;
         }
     }
     Enable();
     emit data_model->layoutChanged();
 }

void transmitter::FillEquipmentSelector()
{
    for (const auto &x : equipments_ids->GetNames()){
        ui->equipmentSelector->addItem(x);
    }
    SelectEquipment(equipment_id);
    FillLabelSelector();
}

void transmitter::SetEditorDelegate()
{
    editor_delegate = new EditorDelegate(this);
    ui->treeView->setItemDelegateForColumn(1, editor_delegate);
    ui->treeView->setItemDelegateForColumn(2, editor_delegate);
    ui->treeView->setItemDelegateForColumn(4, editor_delegate);
}

void transmitter::UnsetEditorDelegate()
{
    ui->treeView->setItemDelegateForColumn(1, nullptr);
    ui->treeView->setItemDelegateForColumn(2, nullptr);
    ui->treeView->setItemDelegateForColumn(4, nullptr);
    delete editor_delegate;
}

void transmitter::FillLabelSelector()
{
    ui->labelSelector->clear();
    Equipment* tmp = TransmitterWorker::getInstance(channel)->getEquipment();

    for (const std::pair<str_t,str_t>& x: tmp->getLabelsIdAndNames()){
        ui->labelSelector->addItem(x.first);
    }
}

void transmitter::OnEquipmentSelectorChanged(int selector_index)
{
     QString eq_name = ui->equipmentSelector->itemText(selector_index);
     emit onTransmitterEnabled(false);
     auto eq_id = equipments_ids->FindId(eq_name);
     ResetDataModel(eq_id);
 }

void transmitter::OnLabelSelectorChanged(int selector_index)
{
    Equipment* tmp = TransmitterWorker::getInstance(channel)->getEquipment();
    str_t selected_label_name = ui->labelSelector->itemText(selector_index);
    selected_label_id  = tmp->FindLabelIdFromName(selected_label_name);
}

void transmitter::onSelectSaveConfigFile(bool if_clicked)
{
    config_file_name =  utils::openFileDialogForSaving(this, GeneralData::getInstance()->TRANSMIT_CONFIGS_PATH);
    if (!config_file_name.isEmpty()) {
        Equipment* equipment = TransmitterWorker::getInstance(channel)->getEquipment();
        jsonobj_t jconfig;
        jconfig["arincConfig"] = ui->cArinc_parity_bitRate->currentIndex();
        jconfig["equipmentId"] = equipment->getEqipmentId();
        QJsonArray labels_list;

        if (equipment){
            for (BaseItem* x:equipment->GetLabels()){
            QString s = static_cast<Label*>(x)->getArincData().template Get<Bits>().toString();
            QString invs;
            for (int i=s.size()-1; i>=0; i--){
                invs+=s[i];
            }
            labels_list.append(invs);
            }
        }
        jconfig["labels"] = labels_list;
        utils::dumpJsonObj(config_file_name, jconfig);
    }
}

void transmitter::SelectEquipment(str_t eq_id)
{
    str_t eq_name = equipments_ids->FindName(eq_id);
    if (eq_name!= "null"){
        ui->equipmentSelector->setCurrentText(eq_name);
    }
}

void transmitter::onLoadConfig(bool if_clicked)
{
     if (!config_file_name.isEmpty()) {
         jsonobj_t jconfig = utils::GetJsonObj(config_file_name);
         emit onTransmitterEnabled(false);
         ui->cArinc_parity_bitRate->setCurrentIndex(jconfig["equipmentId"].toInt());
         SelectEquipment(jconfig["arincConfig"].toString());
         ResetDataModel(jconfig["equipmentId"].toString());

         DArincData arinc_data;

         for (QJsonValue x: jconfig["labels"].toArray()){
             arinc_data.Init(x.toString().toStdString());
             str_t label_id  = arinc_data.template Get<LabelIdOctal>().toString();
             value_t value = arinc_data.template Get<DataBits>();
             if (label_id!="000"){
                 if (GetDataModel()){
                     GetDataModel()->setLabelData(label_id, value);
                 }
             }
         }
     }
 }

void transmitter::onSelectLoadConfigFile(bool if_clicked)
{
    config_file_name =  utils::openFileDialogForOpening(this, GeneralData::getInstance()->TRANSMIT_CONFIGS_PATH);

    if (!config_file_name.isEmpty()) {
        emit onLoadConfig(if_clicked);
        ui->lSelectedFile->setText(config_file_name);
        return;
    }
    ui->lSelectedFile->setText("No file selected.");
}

void transmitter::onTransmitterEnabled(bool checked)
{
     ui->chTransmitterDisabled->setChecked(!checked);
     if (checked)
         Enable();
     else
         Disable();
 }
 
 void transmitter::onTransmitterDisabled(bool checked)
 {
     ui->chTransmitterEnabled->setChecked(!checked);
     if (checked)
         Disable();
     else
         Enable();
 }
 
 void transmitter::Disable()
 {
      if_enabled = false;
      std::this_thread::sleep_for(std::chrono::milliseconds (MIN_TICK));
  }
  
  void transmitter::Enable()
  {
       if_enabled = true;
       std::this_thread::sleep_for(std::chrono::milliseconds (MIN_TICK));
  }
  
  transmitter* transmitter::getInstance(int ch)
  {
      return SingletonArray<transmitter, 2>::getInstance(ch);
  }