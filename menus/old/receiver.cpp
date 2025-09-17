#include "receiver.h"
#include "ui_receiver.h"
#include "devicedelegate.h"
#include "TreeDataModel.h"
#include "equipmentsids.h"
#include "editordelegate.h"
#include "TreeDataModel.h"

 Receiver* Receiver::instance=nullptr;

Receiver::Receiver(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::Receiver), dataModel(new MyDataModel(nullptr, DeviceDelegate<DReceiver,DEVICEONE_CHANELLONE>::getInstance()->getEquipments(), false)), defaultEquipment("002"){
   ui->setupUi(this);
//
   ui->treeView->setModel(dataModel);
   emit dataModel->layoutChanged();

   ui->treeView->setColumnWidth(0, 300);
   ui->treeView->setColumnWidth(1, 60);
   ui->treeView->setColumnWidth(2, 100);
   ui->treeView->setColumnWidth(3, 370);
   ui->treeView->setColumnWidth(4, 300);
   ui->treeView->setColumnWidth(5, 50);


   equipmentsIds = new EquipmentsIds();
   FillEquipmentSelector();

   DeviceDelegate<DReceiver,DEVICEONE_CHANELLONE>::getInstance()->start();
   connect(ui->equipmentSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(OnEquipmentSelectorChanged(int)));
   connect(this, SIGNAL(OnArincDataUpdated()), this, SLOT(UpdateTree()));
}

Receiver::~Receiver() {
    delete ui;
    delete dataModel;
    DeviceDelegate<DReceiver,DEVICEONE_CHANELLONE>::getInstance()->quit();
}

void Receiver::resetDataModel(str_t _eqId){
    ui->treeView->setModel(nullptr);
    auto __datamodel =  dataModel;

    DeviceDelegate<DTransmitter,DEVICEONE_CHANELLONE>::getInstance()->getEquipment()->init(_eqId);
    dataModel = new MyDataModel(nullptr, DeviceDelegate<DTransmitter,DEVICEONE_CHANELLONE>::getInstance()->getEquipments(), true);
    ui->treeView->setModel(dataModel);
    emit dataModel->layoutChanged();

    ui->treeView->setColumnWidth(0, 300);
    ui->treeView->setColumnWidth(1, 60);
    ui->treeView->setColumnWidth(2, 100);
    ui->treeView->setColumnWidth(3, 370);
    ui->treeView->setColumnWidth(4, 300);
    ui->treeView->setColumnWidth(5, 50);

    if (__datamodel){
        delete __datamodel;
    }
}




MyDataModel *  Receiver::getDataModel(){
    return dataModel;
}


void Receiver::updateModelData(Label* _baseItem){
     dataModel->updateModelData(_baseItem);
}


void Receiver::insertLabel(bool _ifChecked){
    const QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->treeView->model();

    if (model->insertRows(index.row()+1, 1, index.parent()))
        return;
}

void Receiver::removeLabel(bool _ifChecked){
    const QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->treeView->model();

    if (!model->removeRows(index.row(),1, index.parent()))
        return;
}


 Receiver* Receiver::getInstance(){
     if (!instance){
         instance = new Receiver();
     }
    return instance;
}



void Receiver::FillEquipmentSelector() {
    for (auto& x : equipmentsIds->GetNames()){
        ui->equipmentSelector->addItem(x);
    }
    //
    ui->equipmentSelector->setCurrentText(equipmentsIds->FindName(defaultEquipment));

}

void Receiver::OnEquipmentSelectorChanged(int _selectorindexd){
    QString _qname = ui->equipmentSelector->itemText(_selectorindexd);
    auto _id = equipmentsIds->FindId(_qname);
    resetDataModel(_id);
}

void Receiver::UpdateTree() {
    std::cout << "Tree was updated!!!";
    dataModel->layoutChanged();
}
