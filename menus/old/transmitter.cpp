#include "transmitter.h"
#include "ui_transmitter.h"

#include "equipmentsids.h"
#include "devicedelegate.h"
#include "editordelegate.h"
#include "TreeDataModel.h"


transmitter* transmitter::instance = nullptr;


transmitter* transmitter::getInstance(){
    if (!instance){
        instance = new transmitter();
    }
   return instance;
}

void transmitter::resetDataModel(str_t _eqId){
    ui->treeView->setModel(nullptr);
    auto __datamodel =  dataModel;

    DeviceDelegate<DTransmitter,DEVICEONE_CHANELLONE>::getInstance()->getEquipment()->init(_eqId);

    dataModel = new MyDataModel(nullptr, DeviceDelegate<DTransmitter,DEVICEONE_CHANELLONE>::getInstance()->getEquipments(), true);
    ui->treeView->setModel(dataModel);

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
    ui(new Ui::transmitter), dataModel(new MyDataModel(nullptr, DeviceDelegate<DTransmitter,DEVICEONE_CHANELLONE>::getInstance()->getEquipments(), true))
{
    ui->setupUi(this);
    resetDataModel( "002");
    setEditorDelegate();

    defaultEquipment = "002";
    equipmentsIds = new EquipmentsIds();

    FillEquipmentSelector();
    FillLabelSelector();

    DeviceDelegate<DTransmitter,DEVICEONE_CHANELLONE>::getInstance()->start();

    //
    connect(ui->equipmentSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(OnEquipmentSelectorChanged(int)));
    connect(ui->labelSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(OnLabelSelectorChanged(int)));
    connect(ui->addLabel, SIGNAL(clicked(bool)), this, SLOT(addLabel(bool)));
    connect(ui->removeLabel, SIGNAL(clicked(bool)), this, SLOT(removeLabel(bool)));
}

transmitter::~transmitter()
{
    delete ui;
    delete equipmentsIds;
    delete editorDelegate;
    delete dataModel;
    DeviceDelegate<DTransmitter,DEVICEONE_CHANELLONE>::getInstance()->quit();
   // DeviceDelegate<DTransmitter,DEVICEONE_CHANELLONE>::getInstance()->wait();


}

MyDataModel * transmitter::getDataModel(){
    return dataModel;
}


void transmitter::addLabel(bool _ifChecked){
    dataModel->addLabel(selectedLabelId);
    emit dataModel->layoutChanged();
}
void transmitter::removeLabel(bool _ifChecked){
    const QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->treeView->model();

    if (!model->removeRow(index.row(), index.parent()))
        return;

    emit dataModel->layoutChanged();

}

void transmitter::FillEquipmentSelector() {
    for (const auto &x : equipmentsIds->GetNames()){
        ui->equipmentSelector->addItem(x);
    }
    //
    ui->equipmentSelector->setCurrentText(equipmentsIds->FindName(defaultEquipment));
    equipmentId = defaultEquipment;
    FillLabelSelector();
}
void transmitter::setEditorDelegate(){
    editorDelegate = new EditorDelegate(this);
    ui->treeView->setItemDelegateForColumn(1, editorDelegate);
    ui->treeView->setItemDelegateForColumn(2, editorDelegate);
    ui->treeView->setItemDelegateForColumn(4, editorDelegate);

}
void transmitter::unsetEditorDelegate(){
    ui->treeView->setItemDelegateForColumn(1, nullptr);
    ui->treeView->setItemDelegateForColumn(2, nullptr);
    ui->treeView->setItemDelegateForColumn(4, nullptr);
    delete editorDelegate;
}
void transmitter::FillLabelSelector(){

    ui->labelSelector->clear();
    Equipment* tmp = DeviceDelegate<DTransmitter,DEVICEONE_CHANELLONE>::getInstance()->getEquipment();

    for (const std::pair<str_t,str_t>& x: tmp->getLabelsIdAndNames()){
        ui->labelSelector->addItem(x.first);
    }
}

void transmitter::OnEquipmentSelectorChanged(int _selectorindexd){
    QString _eqname = ui->equipmentSelector->itemText(_selectorindexd);
    auto _eqId = equipmentsIds->FindId(_eqname);
    resetDataModel(_eqId);

}

void transmitter::OnLabelSelectorChanged(int _selectorindexd){
    Equipment* tmp = DeviceDelegate<DTransmitter,DEVICEONE_CHANELLONE>::getInstance()->getEquipment();
    str_t _selectedLabelName = ui->labelSelector->itemText(_selectorindexd);
    selectedLabelId  = tmp->findLabelIdFromName(_selectedLabelName);
  //  qDebug() << "OnLabelSelectorChanged== " << _selectedLabelName<< "  "<<selectedLabelId;

}

