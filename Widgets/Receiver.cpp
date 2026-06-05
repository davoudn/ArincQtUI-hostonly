#include "Receiver.h"
#include "ui_Receiver.h"
#include "ReceiverWorker.h"
#include "TreeDataModel.h"
#include "equipmentsids.h"
#include "TreeDataModel.h"
#include "DEI1016RasberryConfigurations.h"
#include "Equipment.h"
#include "generaldata.h"
#include "DEI1016.h"

#include <thread>
#include <chrono>

Receiver* Receiver::getInstance(int ch)
{
    return SingletonArray<Receiver, 4>::getInstance(ch);
}

Receiver::Receiver(QWidget *parent, int ch) :
   QWidget(parent),
   chanell(ch), ui(new Ui::Receiver), dataModel(new MyDataModel(nullptr, ReceiverWorker::getInstance(ch)->getEquipments(), false)), equipmentId("002")
{
   if (ch >= CHANNEL_DEI_MAPPING.size()) {
       qWarning() << "Invalid channel:" << ch << ", defaulting to 0";
       chanell = 0;
   }
   auto mapping = CHANNEL_DEI_MAPPING[chanell];
   dei = mapping.first;
   deiChanell = mapping.second;
   
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
    ResetDataModel(equipmentId);
   ReceiverWorker::getInstance(chanell)->startTasks();
   connect(ui->equipmentSelector, SIGNAL(currentIndexChanged(int))
    , this, SLOT(OnEquipmentSelectorChanged(int)));
   connect(ui->c_bitRate_enableSDI, SIGNAL(currentIndexChanged(int))
    , this, SLOT(on_SDI_bitRate(int)));
   connect(this, SIGNAL(OnArincDataUpdated())
    , this, SLOT(UpdateTree()));
   connect(ui->chReceiverEnabled, &QCheckBox::clicked
    , this, &Receiver::onReceiverEnabled);
   connect(ui->chReceiverDisabled, &QCheckBox::clicked
    , this, &Receiver::onReceiverDisabled);
   ui->chReceiverDisabled->setCheckState(Qt::CheckState::Checked);
   ui->chReceiverEnabled->setCheckState(Qt::CheckState::Unchecked);
   bIfEnabled = false;
}

Receiver::~Receiver()
{
    Disable();
    delete ui;
    delete dataModel;
}

void Receiver::ResetDataModel(str_t _eqId)
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


void Receiver::on_SDI_bitRate(int index)
{
    //QMutexLocker<QMutex> locker(&GeneralData::getInstance()->mutex);
    auto control_word = DEI1016::getInstance()->setControlWord_receiver_32Bits(dei,index);
    GeneralData::getInstance()->getActions().push_back(MakeControlAction(dei, static_cast<uint16_t>(control_word.to_ulong())));
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

void Receiver::FillEquipmentSelector()
{
    for (auto& x : equipmentsIds->GetNames()){
        ui->equipmentSelector->addItem(x);
    }

    ui->equipmentSelector->setCurrentText(equipmentsIds->FindName(equipmentId));
}

void Receiver::OnEquipmentSelectorChanged(int _selectorindexd)
{
    QString _qname = ui->equipmentSelector->itemText(_selectorindexd);
    auto _id = equipmentsIds->FindId(_qname);
    Disable();
    ResetDataModel(_id);
    Enable();
}

void Receiver::UpdateTree()
{
    qInfo() << "Tree was updated!!!";
    emit dataModel->layoutChanged();
}

bool Receiver::SetLabelData(str_t labelId, const QVariant &value)
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
                ResetDataModel("1111");
                if (dataModel){
                    return dataModel->setLabelData(labelId, value);

                }
                Enable();
            }
        }
    }
    return false;
}


bool Receiver::SetLabelData(str_t labelId, const float& rate, const QVariant &value)
{
    if (bIfEnabled)
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
                ResetDataModel("1111");
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
    std::this_thread::sleep_for(std::chrono::milliseconds (MIN_TICK));
}

void Receiver::Enable()
{
    bIfEnabled = true;;
    std::this_thread::sleep_for(std::chrono::milliseconds (MIN_TICK));
}





