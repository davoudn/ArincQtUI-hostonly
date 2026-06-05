#include <iostream>
#include "TreeDataModel.h"
#include "Label.h"
#include "Equipment.h"
#include "BaseParameter.h"
#include "baseitem.h"
#include "ArincData.h"
#include "labelfor.h"
#include "generaldata.h"
#include <QMutexLocker>
#include "Receiver.h"
#include "Transmitter.h"
#include "action.h"
#include "generaldata.h"
#include <QThread>

#define COLUMN_NAME 0
#define COLUMN_STATUS 1
#define COLUMN_ENGVALUE 2
#define COLUMN_INFO 3
#define COLUMN_BINARYVALUE 4
#define COLUMN_DATARATE 5


#define MAX_COLUMNS 6

using namespace std;

MyDataModel::MyDataModel(QObject *parent, PointerVector<BaseItem>& vec, bool _bIfEditable)
    : QAbstractItemModel(parent), my_data(vec), b_if_editable(_bIfEditable), tranciver(parent){
}

MyDataModel::~MyDataModel(){

}
int MyDataModel::depth(QModelIndex &index) {
    int retVal = 0;
    for (QModelIndex thisIndex = index; thisIndex.isValid(); thisIndex = thisIndex.parent()) {
        ++retVal;
    }
    return retVal;
}

int MyDataModel::rowCount(const QModelIndex &thisitem) const {

    int retVal = 0;

    if (thisitem.column() <= 0) {
        if (!thisitem.isValid()) {
            retVal = static_cast<int>(my_data.size());
        }

        if (thisitem.isValid()){
            auto item = static_cast<BaseItem*>(thisitem.internalPointer());
            if (item->type == BaseItem::ItemType::Equipment){
                retVal = static_cast<Equipment*>(item)->GetLabels().size();
            }
            if (item->type == BaseItem::ItemType::Label){
                retVal = static_cast<Label*>(item)->GetParameters().size();
            }
        }
    }

    return retVal;
}

int MyDataModel::columnCount(const QModelIndex &) const {
    return MAX_COLUMNS;
}

bool MyDataModel::hasChildren(const QModelIndex &index) const {
    bool retVal = true;

    if (!index.isValid()) {
    }
    else if (!index.parent().isValid()) {
    }
    else if (!index.parent().parent().isValid()) {
    }
    else {
        retVal = false;
    }

    return retVal;
}

QVariant MyDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant retVal;
    if (role == Qt::DisplayRole){
        switch (section) {
            case COLUMN_NAME: retVal = "Name"; break;
            case COLUMN_STATUS: retVal = "Status"; break;
            case COLUMN_ENGVALUE: retVal = "Value(Engineering)"; break;
            case COLUMN_INFO: retVal = "              Info                "; break;
            case COLUMN_BINARYVALUE: retVal = "                 Value(Binary)                "; break;
            case COLUMN_DATARATE: retVal = "Data Rate"; break;

            default: retVal = ""; break;
        }
    }
    return retVal;
}


QVariant MyDataModel::data(const QModelIndex &index, int role) const
{
    QVariant retVal;
    int row = index.row();
    int column = index.column();
    if (role == Qt::DisplayRole && index.isValid() && row >= 0 && column >= 0) {
        BaseItem *baseItem = static_cast<BaseItem *>(index.internalPointer());
        if (baseItem != nullptr)
        {
            switch (baseItem->type)
            {
                case BaseItem::ItemType::Parameter:
                {
                    str_t paramname = static_cast<BaseParameter *>(baseItem)->GetName();
                    Label *label = static_cast<Label*> (baseItem->parent);
                    str_t info = static_cast<BaseParameter *>(baseItem)->GetInfo();
                    switch(column) {
                        case COLUMN_NAME: retVal = paramname; break;
                        case COLUMN_STATUS:   break;
                        case COLUMN_BINARYVALUE: break;
                        case COLUMN_INFO: retVal =info; break;
                        case COLUMN_ENGVALUE: retVal = label->ValueEng(paramname); break;
                        default: retVal = ""; break;
                    }
                    break;
                }
                case BaseItem::ItemType::Label:
                {
                    Label *label = static_cast<Label *>(baseItem);
                    switch(column) {
                        case COLUMN_NAME: retVal = label->GetName(); break;
                        case COLUMN_STATUS: retVal = label->GetStatus(); break;
                        case COLUMN_ENGVALUE: break;
                        case COLUMN_INFO: retVal = "Type: " + label->type; break;
                        case COLUMN_BINARYVALUE:  retVal = label->ValueBits(); break;
                        case COLUMN_DATARATE: retVal = label->GetDataRate(); break;


                        default: retVal = ""; break;
                    }
                    break;
                }
                case BaseItem::ItemType::Equipment:
                {
                    Equipment *topData = static_cast<Equipment *>(baseItem);
                    switch(column) {
                        case COLUMN_NAME: retVal = topData->GetEquipmentIdAndName(); break;
                        case COLUMN_STATUS: ; break;
                        case COLUMN_BINARYVALUE: break;
                        case COLUMN_ENGVALUE: break;
                        default: retVal = ""; break;
                    }
                    break;
                }
            }
        }
    }
    return retVal;
}

bool MyDataModel::updateModelData(Label* _item){
      QModelIndex _index = findIndex(_item);
      if (_index.isValid()){
          emit dataChanged(_index, _index, {Qt::DisplayRole, Qt::EditRole});

          std::cout << "MyDataModel::updateModelData is valid..." ;
          return true;
      }
      std::cout << "MyDataModel::updateModelData, index is not valid..." ;

      return false;
}

QModelIndex MyDataModel::findIndex(Label* _item, const  QModelIndex parent)
{
    if (_item)
    {
        for (int row=0; row < this->rowCount(parent); row++)
        {
            QModelIndex index = this->index(row,0,parent);
            BaseItem* _base = static_cast<BaseItem*>(index.internalPointer());
            if (_base->type == BaseItem::ItemType::Label)
            {
Label* _tmp = static_cast<Label*>(_base);
                if (_tmp)
                {
                     if (_tmp == _item){
                        return index;
                     }
                }
            }
            QModelIndex childIndex = findIndex(_item, index);
            if (childIndex.isValid()){
                return childIndex;
            }
        }
    }
    else {
        std::cout << "MyDataModel::findIndex, item to find is nullptr ... " ;

    }
    return QModelIndex();
}


QModelIndex MyDataModel::index ( int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || column < 0) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        if (row >= static_cast<int>(my_data.size()) ) {
            return QModelIndex();
        }
        Equipment * topData = static_cast<Equipment*>(my_data.at(row));
        return createIndex(row, column, topData);
    }

    BaseItem * baseData = static_cast<BaseItem *>(parent.internalPointer());
    if (baseData == nullptr) {
        std::cout << "We have a legitimate parent, but his internalPointer is null!" << endl;
        exit(0);
    }

    void *ptr = nullptr;
    if (baseData->type == BaseItem::ItemType::Equipment) {
        Equipment *td = static_cast<Equipment *>(baseData);
        if (row < td->GetLabels().size()){
            ptr = td->GetLabels().at(row);
        }
        else {
            qInfo()<<  td->GetLabels().size() <<"---> " <<row ;
        }
    }
    else if (baseData->type == BaseItem::ItemType::Label) {
        Label *md = static_cast<Label *>(baseData);
        ptr = md->GetParameters().at(row);
    }

    return createIndex(row, column, ptr);
}

QModelIndex MyDataModel::parent(const QModelIndex& index) const {
    QModelIndex retVal;
    if (!index.isValid()) {
        return retVal;
    }

    BaseItem * baseData = static_cast<BaseItem *>(index.internalPointer());
    if (baseData == nullptr) {
        return retVal;
    }

    BaseItem *parent = baseData->parent;
    if (parent == nullptr) {
        return retVal;
    }

    int loc = -1;
    if (parent->type == BaseItem::ItemType::Equipment) {
        loc = my_data.indexOf(static_cast<Equipment *>(parent));
    }

    else if (parent->type == BaseItem::ItemType::Label) {
        Label *middleParent = static_cast<Label *>(parent);
        Equipment *topData = static_cast<Equipment *>(middleParent->parent);
        loc = topData->GetLabels().indexOf(middleParent);
    }

    return createIndex(loc, 0, parent);
}


bool MyDataModel::setData(const QModelIndex &_index, const QVariant &value, int role)
{
    int row = _index.row();
    int column = _index.column();

    if (role != Qt::EditRole)
        return false;
    bool bIfStatusChanged = false;

    BaseItem *item = static_cast<BaseItem*>(_index.internalPointer());

    if (item->type == BaseItem::ItemType::Parameter)
    {
        auto param = static_cast<BaseParameter*>(item);
        auto label = static_cast<Label*>(item->parent);
        if (param && label)
        {
            switch(param->GetType())
            {
            case DArincParamType::BCD :
            {
                switch(column) {
                    case COLUMN_NAME:  break;
                    case COLUMN_STATUS: break;
                    case COLUMN_ENGVALUE: label->SetValueEng(param->GetName(), value); break;
                    case COLUMN_BINARYVALUE: break;

                    case COLUMN_INFO:   break;

                    default:  break;
                }
            break;
            }
            case DArincParamType::BNR :
            {
                switch(column) {
                    case COLUMN_NAME:  break;
                    case COLUMN_STATUS: break;
                    case COLUMN_ENGVALUE: label->SetValueEng(param->GetName(), value); break;
                    case COLUMN_BINARYVALUE: break;

                    case COLUMN_INFO:   break;

                    default:  break;
                }
                break;
            }
            case DArincParamType::DISCRETE :
            {
                switch(column) {
                    case COLUMN_NAME:  break;
                    case COLUMN_STATUS: break;
                    case COLUMN_ENGVALUE: label->SetValueEng(param->GetName(), value); break;
                    case COLUMN_BINARYVALUE: break;

                    case COLUMN_INFO:   break;

                    default:  break;
                }
                break;
                }
            }
        }
    }
    if (item->type == BaseItem::ItemType::Label)
    {
        auto label = static_cast<Label*>(item);
        switch(column) {
            case COLUMN_NAME: break;
            case COLUMN_STATUS: {
                label->SetStatus(value.toString());
                bIfStatusChanged = true;
                break;
            }
            case COLUMN_ENGVALUE: break;
            case COLUMN_BINARYVALUE: {
                label->SetValueBits(value);
                break;
            }                
            case COLUMN_INFO: break;
            case COLUMN_DATARATE: label->SetDataRate(value); break;

            default:  break;
        }
        QModelIndex _parent = index(0,0);
        emit dataChanged(index(_index.row(),0,_parent), index(_index.row(),MAX_COLUMNS,_parent), {Qt::DisplayRole, Qt::EditRole});
        emit layoutChanged();
    }

    int channel = getChannel();
    int dei = getDEI();

    if (item->type == BaseItem::ItemType::Label)
    {
        auto label = static_cast<Label*>(item);
        if (label->GetIfActive())
        {
            if (!bIfStatusChanged){
                addLabelAction(dei, channel, 1, Instructions::UPDATE_LABEL_DATA_FOR_TRANSMIT, label);
            }
            else {
                addLabelAction(dei, channel, 1, Instructions::ADD_LABEL_TO_TRANSMIT, label);
            }
        }
        else
        {
            addLabelAction(dei, channel, 1, Instructions::REMOVE_LABEL_FROM_TRANSMIT, label);
        }
    }

    if (item->type == BaseItem::ItemType::Parameter)
    {
        auto label = static_cast<Label*>(_index.parent().internalPointer());
        if (label->GetIfActive())
        {
            if (!bIfStatusChanged){
                addLabelAction(dei, channel, 1, Instructions::UPDATE_LABEL_DATA_FOR_TRANSMIT, label);
            }
            else {
                addLabelAction(dei, channel, 1, Instructions::ADD_LABEL_TO_TRANSMIT, label);
            }
        }
        else
        {
            addLabelAction(dei, channel, 1, Instructions::REMOVE_LABEL_FROM_TRANSMIT, label);
        }
    }
    return true;
}


bool MyDataModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    const bool result = true;

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

bool MyDataModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    BaseItem* parentItem = static_cast<BaseItem*>(parent.internalPointer());

    if (!parentItem)
        return false;

    if (parentItem->type == BaseItem::ItemType::Equipment){
        Equipment* equipment = static_cast<Equipment*>(parentItem);
        beginInsertRows(parent, position, position + rows - 1);
        equipment->InsertLabel(position,label_to_insert);
        endInsertRows();
    }
    return true;
}


bool MyDataModel::setLabelData(str_t labelId, const QVariant &value){
    QModelIndex _index = findLabel(labelId);

    if (_index.isValid()){

        BaseItem *item = static_cast<BaseItem*>(_index.internalPointer());
        auto label = static_cast<Label*>(item);
         label->SetValueBits(value);
        QModelIndex _parent = index(0,0);
        emit dataChanged(index(_index.row(),0,_parent), index(_index.row(),MAX_COLUMNS,_parent), {Qt::DisplayRole, Qt::EditRole});
        layoutChanged();
        return true;
    }
    if (addLabel(labelId)){
        QModelIndex _index = findLabel(labelId);

        BaseItem *item = static_cast<BaseItem*>(_index.internalPointer());
        auto label = static_cast<Label*>(item);
         label->SetValueBits(value);
        QModelIndex _parent = index(0,0);
        emit dataChanged(index(_index.row(),0,_parent), index(_index.row(),MAX_COLUMNS,_parent), {Qt::DisplayRole, Qt::EditRole});
        layoutChanged();

       return true;
    }

    return false;
}


bool MyDataModel::setLabelData(str_t labelId, const QVariant &value, QThread* thread)
{
    QModelIndex _index = findLabel(labelId);
    if (_index.isValid()){
        BaseItem *item = static_cast<BaseItem*>(_index.internalPointer());
        auto label = static_cast<Label*>(item);
        label->SetValueBits(value);
        QModelIndex _parent = index(0,0);
        emit dataChanged(index(_index.row(),0,_parent), index(_index.row(),MAX_COLUMNS,_parent), {Qt::DisplayRole, Qt::EditRole});
        layoutChanged();
        return true;
    }

    if (addLabel(labelId)){
        QModelIndex _index = findLabel(labelId);
        BaseItem *item = static_cast<BaseItem*>(_index.internalPointer());
        auto label = static_cast<Label*>(item);
        label->SetValueBits(value);
        QModelIndex _parent = index(0,0);
        emit dataChanged(index(_index.row(),0,_parent), index(_index.row(),MAX_COLUMNS,_parent), {Qt::DisplayRole, Qt::EditRole});
        layoutChanged();
        return true;
    }

    return false;
}


bool MyDataModel::setLabelData(str_t labelId, const float& rate,  const QVariant &value)
{

    QModelIndex _index = findLabel(labelId);
    if (_index.isValid()){
        BaseItem *item = static_cast<BaseItem*>(_index.internalPointer());
        auto label = static_cast<Label*>(item);
        label->SetValueBits(value);
        label->SetDataRate(rate);
        QModelIndex _parent = index(0,0);
        emit dataChanged(index(_index.row(),0,_parent), index(_index.row(),MAX_COLUMNS,_parent), {Qt::DisplayRole, Qt::EditRole});
        layoutChanged();
        return true;
    }

    if (addLabel(labelId)){
        QModelIndex _index = findLabel(labelId);
        BaseItem *item = static_cast<BaseItem*>(_index.internalPointer());
        auto label = static_cast<Label*>(item);
        label->SetValueBits(value);
        label->SetDataRate(rate);
        QModelIndex _parent = index(0,0);
        emit dataChanged(index(_index.row(),0,_parent), index(_index.row(),MAX_COLUMNS,_parent), {Qt::DisplayRole, Qt::EditRole});
        layoutChanged();
        return true;
    }
    return false;
}

QModelIndex MyDataModel::findLabel(str_t labelId)
{
    QModelIndex topindex = index(0,0,QModelIndex());
    for (int row=0; row < rowCount(topindex); row++)
    {
        QModelIndex index = this->index(row,0,topindex);
        BaseItem* _base = static_cast<BaseItem*>(index.internalPointer());
        if (_base->type == BaseItem::ItemType::Label)
        {
            Label* _tmp = static_cast<Label*>(_base);
            if (_tmp)
            {
               if (_tmp->label_id == labelId){
                       return index;
               }
            }
        }
    }
    return QModelIndex();
}

bool MyDataModel::checkLabel(str_t labelId)
{
    QModelIndex _index = findLabel(labelId);
    if(_index.isValid()){
        return true;
    }
    return false;
}

void MyDataModel::evalDataRates()
{
    QModelIndex topindex = index(0,0,QModelIndex());
    if (topindex.isValid())
    {
        for (int row=0; row < rowCount(topindex); row++)
        {
            QModelIndex _index = index(row,0,topindex);
            BaseItem* _base = static_cast<BaseItem*>(_index.internalPointer());
            Label* _label = static_cast<Label*>(_base);
            if (_base->type == BaseItem::ItemType::Label)
            {
                LabelFor<DReceiver>* _tmp = dynamic_cast<LabelFor<DReceiver>*>(_label);
                if (_tmp){
                    _tmp->EvalDataRate() ;
                }
            }
        }
    }
    return;
}



void  MyDataModel::cleanTimeoutList(){
    auto _garbage = getTimeOutList();

    for (auto& x:_garbage){
        std::cout << "MyDataModel::cleanTimeoutList : try to remove " << x.toStdString() << std::endl;
        removeLabel(x);
    }
    emit layoutChanged();
}



bool MyDataModel::addLabel(str_t labelId)
{
    qInfo() << "MyDataModel::addLabel is runnig on " << QThread::currentThread() ;
    QModelIndex topindex = index(0,0,QModelIndex());
    BaseItem* topItem = static_cast<BaseItem*>(topindex.internalPointer());

    if (!topItem)
        return false;

    if (topItem->type == BaseItem::ItemType::Equipment){
        Equipment* equipment = static_cast<Equipment*>(topItem);
        beginInsertRows(topindex, 0, 0);
        equipment->InsertLabel(0,labelId);
        endInsertRows();
    }
    else {
        return false;
    }
    return true;
}

bool MyDataModel::addReservedLabel()
{
    QModelIndex topindex = index(0,0,QModelIndex());
    BaseItem* topItem = static_cast<BaseItem*>(topindex.internalPointer());

    if (!topItem)
        return false;

    if (topItem->type == BaseItem::ItemType::Equipment){
        Equipment* equipment = static_cast<Equipment*>(topItem);
        beginInsertRows(topindex, 0, 0);
        equipment->AddReservedLabel();
        endInsertRows();
    }
    else {
        return false;
    }
    return true;
}


void MyDataModel::setLabelToInsert(str_t _labelToInsert){
     label_to_insert = _labelToInsert;
}

bool MyDataModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    BaseItem* parentItem = static_cast<BaseItem*>(parent.internalPointer());
    if (parentItem->type == BaseItem::ItemType::Equipment){
       Equipment* equipment = static_cast<Equipment*>(parentItem);
       beginRemoveRows(parent, position, position + rows - 1);
         equipment->RemoveLabel(position);
       endRemoveRows();
    }

    return true;
}

bool MyDataModel::removeRow(int position)
{
    QModelIndex topItem = index(0,0,QModelIndex());
    if ( position < rowCount(topItem)-1){
      removeRows(position,1,topItem);
      return true;
    }
    return false;
}

bool MyDataModel::removeRow(QModelIndex _index)
{
    BaseItem* topItem = static_cast<BaseItem*>(_index.parent().internalPointer());

    if (topItem){
        if (topItem->type == BaseItem::ItemType::Equipment) {
            if (_index.row() < rowCount(_index.parent()) ){
                removeRows(_index.row(),1,_index.parent());
                return true;
            }
        }
    }
    return false;
}



bool MyDataModel::removeLabel(str_t _labelName)
{
    qInfo() << "MyDataModel::removeLabel runs on: " << QThread::currentThread();
    QModelIndex _index = findLabel(_labelName);
    if (_index.isValid())
    {
        if (_index.parent() == index(0,0,QModelIndex())){
           qInfo() << _labelName << "  " << _index.row() << "  " <<_index.column() << "   parent --> " << _index.parent().row() << " " << _index.parent().column();
           removeRow(_index);
            return true;
        }
    }
    return false;
}


Qt::ItemFlags MyDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    if (b_if_editable && (index.column() !=0)){
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    }
    else {
        return QAbstractItemModel::flags(index);
    }
}

std::vector<DArincData> MyDataModel::getListOfAvailableLabelData()
{
    std::vector<DArincData> tmpListOfDataToSend;
    QModelIndex topindex = index(0,0,QModelIndex());
    if (topindex.isValid())
    {
        for (int row=0; row < rowCount(topindex); row++)
        {
            QModelIndex _index = index(row,0,topindex);
            BaseItem* _base = static_cast<BaseItem*>(_index.internalPointer());
            if (_base->type == BaseItem::ItemType::Label)
            {
                Label* _label = static_cast<Label*>(_base);
                LabelFor<DTransmitter>* _tmp = dynamic_cast<LabelFor<DTransmitter>*>(_label);
                if (_tmp)
                {
                    if ( _tmp->GetIfDataAvailable() ){
                        tmpListOfDataToSend.push_back(_tmp->GetArincDataAndReset());
                    }
                }
            }
        }
    }
    return tmpListOfDataToSend;
}

void MyDataModel::incrementLabelsDataRateCounter()
{
    QModelIndex topindex = index(0,0,QModelIndex());
    if (topindex.isValid())
    {
        for (int row=0; row < rowCount(topindex); row++)
        {
            QModelIndex _index = index(row,0,topindex);
            BaseItem* _base = static_cast<BaseItem*>(_index.internalPointer());
            if (_base->type == BaseItem::ItemType::Label)
            {
                Label* _label = static_cast<Label*>(_base);
                LabelFor<DTransmitter>* _tmp = dynamic_cast<LabelFor<DTransmitter>*>(_label);
                if (_tmp){
                    _tmp->IncrementDataRateCounter();
                }
            }
        }
    }
    return ;
}

void MyDataModel::evalDataRates(double _resettime)
{
    std::vector<str_t> _garbage;
    QModelIndex topindex = index(0,0,QModelIndex());
    if (topindex.isValid())
    {
        for (int row=0; row < rowCount(topindex); row++)
        {
            QModelIndex _index = index(row,0,topindex);
            BaseItem* _base = static_cast<BaseItem*>(_index.internalPointer());
            if (_base->type == BaseItem::ItemType::Label)
            {
                Label* _label = static_cast<Label*>(_base);
                LabelFor<DReceiver>* _tmp = dynamic_cast<LabelFor<DReceiver>*>(_label);
                if (_tmp){
                    _tmp->EvalDataRate() ;
                }
            }
        }
    }
    return ;
}

std::vector<str_t> MyDataModel::getTimeOutList()
{
    std::vector<str_t> _garbage;
    QModelIndex topindex = index(0,0,QModelIndex());
    if (topindex.isValid())
    {
        for (int row=0; row < rowCount(topindex); row++)
        {
            QModelIndex _index = index(row,0,topindex);
            BaseItem* _base = static_cast<BaseItem*>(_index.internalPointer());
            if (_base->type == BaseItem::ItemType::Label)
            {
                Label* _label = static_cast<Label*>(_base);
                LabelFor<DReceiver>* _tmp = dynamic_cast<LabelFor<DReceiver>*>(_label);
                if (_tmp){
                    if ( _tmp->timeoutCheck() ){
                        _garbage.push_back(_tmp->label_id);
                    }
                }
            }
        }
    }
    return _garbage;
}

void MyDataModel::addLabelAction(uint32_t dei, uint32_t channel, uint32_t transrec, uint32_t instr, Label* label)
{
    QMutexLocker mutexlocker(&GeneralData::getInstance()->mutex);
    GeneralData::getInstance()->getActions().push_back(MakeDataAction(dei, channel, transrec, instr, label->GetArincData().getBitSet().to_ulong(), label->GetDataRate().toFloat()));
}

int MyDataModel::getChannel()
{
    transmitter* x = dynamic_cast<transmitter*>(tranciver);
    if (x){
        return x->channel;
    }

    Receiver* y = dynamic_cast<Receiver*>(tranciver);
    if (y){
        return y->channel;
    }
    return 0;
}

int MyDataModel::getDEI()
{
    transmitter* x = dynamic_cast<transmitter*>(tranciver);
    if (x){
        return x->dei;
    }

    Receiver* y = dynamic_cast<Receiver*>(tranciver);
    if (y){
        return y->dei;
    }
    return 0;
}