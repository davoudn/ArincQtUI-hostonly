#include "transmitter.h"
#include "ui_transmitter.h"

#include "equipmentsids.h"
#include "transmitterworker.h"
#include "Equipment.h"

#include "editordelegate.h"
#include "TreeDataModel.h"
#include "DEI1016RasberryConfigurations.h".
#include <thread>
#include "utils.h"
#include "generaldata.h"
#include "ArincData.h"

#include <vector>

transmitter* transmitter::instance = nullptr;


transmitter* transmitter::getInstance()
{
    if (!instance){
        instance = new transmitter();
    }
   return instance;
}

void transmitter::initUiCombos()
{
    //
    ui->cArinc_parity_bitRate->addItem("12.5 Kbps/ODD  Parity");
    ui->cArinc_parity_bitRate->addItem("100  Kbps/ODD  Parity");
    ui->cArinc_parity_bitRate->addItem("12.5 Kbps/EVEN Parity");
    ui->cArinc_parity_bitRate->addItem("100  Kbps/EVEN Parity");
}

void transmitter::resetDataModel(str_t _eqId){

    ui->treeView->setModel(nullptr);
    auto __datamodel =  dataModel;

    TransmitterWorker::getInstance()->getEquipment()->init(_eqId);

    dataModel = new MyDataModel(nullptr, TransmitterWorker::getInstance()->getEquipments(), true);
    ui->treeView->setModel(dataModel);
    dataModel->addReservedLabel();

    //
    ui->treeView->setColumnWidth(0, 300);
    ui->treeView->setColumnWidth(1, 60);
    ui->treeView->setColumnWidth(2, 120);
    ui->treeView->setColumnWidth(3, 330);
    ui->treeView->setColumnWidth(4, 600);
    ui->treeView->setColumnWidth(5, 50);

    emit dataModel->layoutChanged();
    FillLabelSelector();

    if(__datamodel)
      delete __datamodel;
}

transmitter::transmitter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::transmitter), dataModel(new MyDataModel(nullptr, TransmitterWorker::getInstance()->getEquipments(), true))
{
    ui->setupUi(this);
    equipmentsIds = new EquipmentsIds();
    FillEquipmentSelector();
    equipmentId = "002";
    resetDataModel(equipmentId);
    selectEquipment(equipmentId);

    setEditorDelegate();
    setObjectName("Transmitter thread.");
    initUiCombos();

    TransmitterWorker::getInstance()->startTasks();
    //
    connect(ui->equipmentSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(OnEquipmentSelectorChanged(int)));
    connect(ui->labelSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(OnLabelSelectorChanged(int)));
    connect(ui->addLabel, SIGNAL(clicked(bool)), this, SLOT(addLabel(bool)));
    connect(ui->removeLabel, SIGNAL(clicked(bool)), this, SLOT(removeLabel(bool)));
    connect(ui->cArinc_parity_bitRate, SIGNAL(currentIndexChanged(int)), this, SLOT(onArinc_parity_bitRate(int)));
    // connect(ui->pLoadConfig, SIGNAL(clicked(bool)), this, SLOT(onLoadConfig(bool)));
    connect(ui->pSaveConfig, SIGNAL(clicked(bool)), this, SLOT(onSelectSaveConfigFile(bool)));
    connect(ui->pLoadConfig, SIGNAL(clicked(bool)), this, SLOT(onSelectLoadConfigFile(bool)));

    connect(ui->chTransmitterEnabled, &QCheckBox::clicked, this, &transmitter::onTransmitterEnabled);
    connect(ui->chTransmitterDisabled, &QCheckBox::clicked, this, &transmitter::onTransmitterDisabled);

    emit
    ui->chTransmitterDisabled->setChecked(true);
    ui->chTransmitterEnabled->setChecked(false);
    bIfEnabled = false;
}

transmitter::~transmitter()
{
    delete ui;
    delete equipmentsIds;
    delete editorDelegate;
    delete dataModel;
   // TransmitterWorker::getInstance()->quit();
   // TransmitterWorker::getInstance()->wait();


}

MyDataModel * transmitter::getDataModel()
{
    return dataModel;
}

void transmitter::onArinc_parity_bitRate(int index)
{
}


std::vector<DArincData> transmitter::getListOfAvailableLabelData(){
    std::vector<DArincData> list;

    if (bIfEnabled){
        if (dataModel){
            list = dataModel->getListOfAvailableLabelData();
        }
    }
    return list;
}

void transmitter::incrementLabelsDataRateCounter(){
  if (bIfEnabled){
    if(dataModel){
      dataModel->incrementLabelsDataRateCounter();
    }
  }
}

void transmitter::addLabel(bool _ifChecked)
{
    Disable();
    dataModel->addLabel(selectedLabelId);
    Enable();
    emit dataModel->layoutChanged();
}
void transmitter::removeLabel(bool _ifChecked)
{
    const QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->treeView->model();

   // dataModel->removeRow(index);
    Disable();
    if (index.row()< dataModel->rowCount(index.parent())-1){
        if (!model->removeRow(index.row(), index.parent())){
            return;
        }
    }
    Enable();
    emit dataModel->layoutChanged();

}

void transmitter::FillEquipmentSelector()
{
    for (const auto &x : equipmentsIds->GetNames()){
        ui->equipmentSelector->addItem(x);
    }
    //
    selectEquipment(equipmentId);
    FillLabelSelector();
}
void transmitter::setEditorDelegate()
{
    editorDelegate = new EditorDelegate(this);
    ui->treeView->setItemDelegateForColumn(1, editorDelegate);
    ui->treeView->setItemDelegateForColumn(2, editorDelegate);
    ui->treeView->setItemDelegateForColumn(4, editorDelegate);

}
void transmitter::unsetEditorDelegate()
{
    ui->treeView->setItemDelegateForColumn(1, nullptr);
    ui->treeView->setItemDelegateForColumn(2, nullptr);
    ui->treeView->setItemDelegateForColumn(4, nullptr);
    delete editorDelegate;
}
void transmitter::FillLabelSelector()
{

    ui->labelSelector->clear();
    Equipment* tmp = TransmitterWorker::getInstance()->getEquipment();

    for (const std::pair<str_t,str_t>& x: tmp->getLabelsIdAndNames()){
        ui->labelSelector->addItem(x.first);
    }
}

void transmitter::OnEquipmentSelectorChanged(int _selectorindexd)
{
    QString _eqname = ui->equipmentSelector->itemText(_selectorindexd);
    emit onTransmitterEnabled(false);
    auto _eqId = equipmentsIds->FindId(_eqname);
    resetDataModel(_eqId);

}

void transmitter::OnLabelSelectorChanged(int _selectorindexd)
{
    Equipment* tmp = TransmitterWorker::getInstance()->getEquipment();
    str_t _selectedLabelName = ui->labelSelector->itemText(_selectorindexd);
    selectedLabelId  = tmp->findLabelIdFromName(_selectedLabelName);
  //  qDebug() << "OnLabelSelectorChanged== " << _selectedLabelName<< "  "<<selectedLabelId;

}

void transmitter::onSelectSaveConfigFile(bool bIfClicked)
{
    configFileName =  utils::openFileDialogForSaving(this, GeneralData::getInstance()->TRANSMIT_CONFIGS_PATH);
    if (!configFileName.isEmpty()) {
        Equipment* equipment = TransmitterWorker::getInstance()->getEquipment();
        jsonobj_t jconfig;
        jconfig["arincConfig"] = ui->cArinc_parity_bitRate->currentIndex();
        jconfig["equipmentId"] = equipment->getEqipmentId();
        QJsonArray labelslist;

        if (equipment){
            for (BaseItem* x:equipment->GetLabels()){
            QString s = static_cast<Label*>(x)->getArincData().template Get<Bits>().toString();
            QString invs;
            for (int i=s.size()-1; i>=0; i--){
                invs+=s[i];
            }
            labelslist.append(invs);
            }
        }
        jconfig["labels"] = labelslist;
        utils::dumpJsonObj(configFileName, jconfig);
    }
}
void transmitter::selectEquipment(str_t eqId){
    str_t eqName = equipmentsIds->FindName(eqId);
    if (eqName!= "null"){
        ui->equipmentSelector->setCurrentText(eqName);
    }
}

void transmitter::onLoadConfig(bool bIfClicked)
{
    if (!configFileName.isEmpty()) {
        jsonobj_t jconfig = utils::GetJsonObj(configFileName);
    emit onTransmitterEnabled(false);
        // refreshing the equipment and labels combos
        ui->cArinc_parity_bitRate->setCurrentIndex(jconfig["equipmentId"].toInt());
        selectEquipment(jconfig["arincConfig"].toString());
        resetDataModel(jconfig["equipmentId"].toString());

        DArincData arincData;

        for (QJsonValue x: jconfig["labels"].toArray()){
            arincData.Init(x.toString().toStdString());
            str_t labelid  = arincData.template Get<LabelIdOctal>().toString();
            value_t value = arincData.template Get<DataBits>();
            if (labelId!="000"){
                if (getDataModel()){
                    getDataModel()->setLabelData(labelid, value);
                }
            }
        }
    } // check if configFileName is empty
}

void transmitter::onSelectLoadConfigFile(bool bIfClicked){
    configFileName =  utils::openFileDialogForOpening(this, GeneralData::getInstance()->TRANSMIT_CONFIGS_PATH);

    if (!configFileName.isEmpty()) {
       emit onLoadConfig(bIfClicked);
      ui->lSelectedFile->setText(configFileName);
      return;
   }
    ui->lSelectedFile->setText("No file selected.");
}

void transmitter::onTransmitterEnabled(bool checked){
    ui->chTransmitterDisabled->setChecked(!checked);
    if (checked)
        Enable();
    else
        Disable();

}
void transmitter::onTransmitterDisabled(bool checked){
    ui->chTransmitterEnabled->setChecked(!checked);
    if (checked)
        Disable();
    else
        Enable();
}

void transmitter::Disable(){
    bIfEnabled = false;
    std::this_thread::sleep_for(std::chrono::milliseconds (MIN_TICK));
}
void transmitter::Enable(){
    bIfEnabled = true;;
    std::this_thread::sleep_for(std::chrono::milliseconds (MIN_TICK));
}
