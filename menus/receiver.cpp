#include "receiver.h"
#include "ui_receiver.h"
#include "receiverworker.h"
#include "TreeDataModel.h"
#include "equipmentsids.h"
#include "TreeDataModel.h"
#include "DEI1016RasberryConfigurations.h"
#include "Equipment.h"
#include "generaldata.h"
#include "DEI1016.h"
#include "actionsrecord.h"


Receiver* Receiver::instance0 = nullptr;
Receiver* Receiver::instance1 = nullptr;
Receiver* Receiver::instance2 = nullptr;
Receiver* Receiver::instance3 = nullptr;


Receiver::Receiver(QWidget *parent, uint8_t ch) :
   QWidget(parent),
   chanell(ch), ui(new Ui::Receiver), dataModel(new MyDataModel(nullptr, ReceiverWorker::getInstance(ch)->getEquipments(), false)), equipmentId("002")
{
   ui->setupUi(this);
   ui->treeView->setModel(dataModel);
   emit dataModel->layoutChanged();

   ui->treeView->setColumnWidth(0, 300);
   ui->treeView->setColumnWidth(1, 60);
   ui->treeView->setColumnWidth(2, 100);
   ui->treeView->setColumnWidth(3, 370);
   ui->treeView->setColumnWidth(4, 300);
   ui->treeView->setColumnWidth(5, 50);
   this->setWindowTitle("Receiver " + QString::number(chanell));
   equipmentsIds = new EquipmentsIds();
   FillEquipmentSelector();
   initUiCombos();
   resetDataModel(equipmentId);
   ReceiverWorker::getInstance(chanell)->startTasks();
   makeDeviceIndex();
   connect(ui->equipmentSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(OnEquipmentSelectorChanged(int)));
   connect(ui->c_bitRate_enableSDI, SIGNAL(currentIndexChanged(int)), this, SLOT(on_SDI_bitRate(int)));
   connect(this, SIGNAL(OnArincDataUpdated()), this, SLOT(UpdateTree()));
   connect(ui->chReceiverEnabled, &QCheckBox::clicked, this, &Receiver::onReceiverEnabled);
   connect(ui->chReceiverDisabled, &QCheckBox::clicked, this, &Receiver::onReceiverDisabled);
   connect(ReceiverWorker::getInstance(chanell), &ReceiverWorker::setLabelData, this, &Receiver::setLabelData, Qt::QueuedConnection);

   ui->chReceiverDisabled->setCheckState(Qt::CheckState::Unchecked);
   ui->chReceiverEnabled->setCheckState(Qt::CheckState::Checked);
   bIfEnabled = true;
}

Receiver::~Receiver()
{
    Disable();
    delete ui;
    delete dataModel;
   // ReceiverWorker::getInstance(chanell)->quit();
}

void Receiver::resetDataModel(str_t _eqId)
{
    ui->treeView->setModel(nullptr);
    auto __datamodel =  dataModel;

    ReceiverWorker::getInstance(chanell)->getEquipment()->init(_eqId);
    dataModel = new MyDataModel(nullptr,  ReceiverWorker::getInstance(chanell)->getEquipments(), true);
    ui->treeView->setModel(dataModel);

    dataModel   ->addReservedLabel();
    ui->treeView->setColumnWidth(0, 300);
    ui->treeView->setColumnWidth(1, 60);
    ui->treeView->setColumnWidth(2, 100);
    ui->treeView->setColumnWidth(3, 370);
    ui->treeView->setColumnWidth(4, 300);
    ui->treeView->setColumnWidth(5, 50);

    emit dataModel->layoutChanged();

    if (__datamodel){
        delete __datamodel;
    }
}


void Receiver::initUiCombos()
{
    ui->c_bitRate_enableSDI->addItem("12.5  Kbps/SDI Disable");
    ui->c_bitRate_enableSDI->addItem("100.0 Kbps/SDI Disable");
    ui->c_bitRate_enableSDI->addItem("12.5  Kbps/SDI 0      ");
    ui->c_bitRate_enableSDI->addItem("12.5  Kbps/SDI 1      ");
    ui->c_bitRate_enableSDI->addItem("12.5  Kbps/SDI 2      ");
    ui->c_bitRate_enableSDI->addItem("12.5  Kbps/SDI 3      ");
    ui->c_bitRate_enableSDI->addItem("100.0 Kbps/SDI 0      ");
    ui->c_bitRate_enableSDI->addItem("100.0 Kbps/SDI 1      ");
    ui->c_bitRate_enableSDI->addItem("100.0 Kbps/SDI 2      ");
    ui->c_bitRate_enableSDI->addItem("100.0 Kbps/SDI 3      ");
}

void Receiver::makeDeviceIndex()
{
    switch (chanell) {
    case 0:
        dei = 0;
        deiChanell = 0;
        break;
    case 1:
        dei = 0;
        deiChanell = 1;
        break;
    case 2:
        dei = 1;
        deiChanell = 0;
        break;
    case 3:
        dei = 1;
        deiChanell = 1;
        break;
    }
}


void Receiver::on_SDI_bitRate(int index)
{
    QMutexLocker locker(&GeneralData::getInstance()->mutex);
    auto control_word = DEI1016::getInstance()->setControlWord_receiver_32Bits(dei,index);
    TransmitterRecords::getInstance()->record(MakeControlAction(dei, static_cast<uint16_t>(control_word.to_ulong()))->toPacket());
}

MyDataModel *  Receiver::getDataModel()
{
    return dataModel;
}

void Receiver::updateModelData(Label* _baseItem)
{
     dataModel->updateModelData(_baseItem);
}


void Receiver::removeLabel(bool _ifChecked)
{
    const QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->treeView->model();
    model->removeRows(index.row(),1, index.parent());
}

 Receiver* Receiver::getInstance(uint8_t ch)
 {
    switch(ch){
        case CHANELL0: {
            if (!instance0){
                 instance0 = new Receiver(nullptr, CHANELL0);
            }
            return instance0;
        }
        case CHANELL1: {
            if (!instance1){
                instance1 = new Receiver(nullptr, CHANELL1);
            }
            return instance1;
        }
        case CHANELL2: {
            if (!instance2){
                instance2 = new Receiver(nullptr, CHANELL2);
            }
            return instance2;
        }
        case CHANELL3: {
            if (!instance3){
                instance3 = new Receiver(nullptr, CHANELL3);
            }
            return instance3;
        }
    }
    return nullptr;
}

void Receiver::FillEquipmentSelector()
{
    for (auto& x : equipmentsIds->GetNames()){
        ui->equipmentSelector->addItem(x);
    }
    //
    ui->equipmentSelector->setCurrentText(equipmentsIds->FindName(equipmentId));
}

void Receiver::OnEquipmentSelectorChanged(int _selectorindexd)
{
    QString _qname = ui->equipmentSelector->itemText(_selectorindexd);
    auto _id = equipmentsIds->FindId(_qname);
    Disable();
    resetDataModel(_id);
    Enable();
}

void Receiver::UpdateTree()
{
    qInfo() << "Tree was updated!!!";
    emit dataModel->layoutChanged();
}




bool Receiver::setLabelData(uint32_t deiId, uint32_t deiCh, str_t labelId, const float& rate, const QVariant &value)
{
    if (bIfEnabled && dei==deiId && deiChanell==deiCh)
    {
        Equipment* equipment =  ReceiverWorker::getInstance(chanell)->getEquipment();
        if (equipment)
        {
            if (equipment->checkLabel(labelId))
            {
                if (dataModel){
                    return dataModel->setLabelData(labelId, rate, value);
                }
            }
            else
            {
                Disable();
                resetDataModel("1111");
                if (dataModel){
                    return dataModel->setLabelData(labelId, rate, value);

                }
                Enable();
            }
        }
    }
    return false;
}

void Receiver::idleLabelCleaner()
{
    Disable();
        if (dataModel){
            dataModel->cleanTimeoutList();
        }
    Enable();
}

void Receiver::evalDataRates(uint32_t interval)
{
    if (bIfEnabled)
    {
        if (dataModel){
            dataModel->evalDataRates(interval);
        }
    }
}

void Receiver::onReceiverEnabled(bool checked)
{
    ui->chReceiverDisabled->setChecked(!checked);
    if (checked)
        Enable();
    else
        Disable();

}

void Receiver::onReceiverDisabled(bool checked)
{
    ui->chReceiverEnabled->setChecked(!checked);
    if (checked)
        Disable();
    else
        Enable();
}

void Receiver::Disable()
{
    bIfEnabled = false;
   // std::this_thread::sleep_for(std::chrono::milliseconds (MIN_TICK));
}

void Receiver::Enable()
{
    bIfEnabled = true;;
  //  std::this_thread::sleep_for(std::chrono::milliseconds (MIN_TICK));
}



/*
bool Receiver::setLabelDataSimple(str_t labelId, const QVariant &value)
{
    if (bIfEnabled)
    {
        Equipment* equipment =  ReceiverWorker::getInstance(chanell)->getEquipment();
        if (equipment)
        {
            if (equipment->checkLabel(labelId))
            {
                if (dataModel){
                    return dataModel->setLabelData(labelId, value);
                }
            }
            else
            {
                Disable();
                resetDataModel("1111");
                if (dataModel){
                    return dataModel->setLabelData(labelId, value);

                }
                Enable();
            }
        }
    }
    return false;
}
 */

