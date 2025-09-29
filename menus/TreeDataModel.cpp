#include <iostream>
#include "TreeDataModel.h"
#include "Label.h"
#include "Equipment.h"
#include "BaseParameter.h"
#include "baseitem.h"
#include "ArincData.h"
#include "labelfor.h"
#include <QMutexLocker>
#include "receiver.h"
#include "transmitter.h"
#include "receiverworker.h"
#include "actionsrecord.h"
#include "action.h"
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
    : QAbstractItemModel(parent), myData(vec), bIfEditable(_bIfEditable),tranciver(parent){
    connect(&layoutRefresher, &QTimer::timeout, this, &MyDataModel::enableLayoutRefresh);
    layoutRefresher.start(LAYOUT_REFRESHER_INTERVAL);
}
void MyDataModel::enableLayoutRefresh(){
    bIfLayoutRefresh = true;
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
    /*if (thisitem.isValid()){
        auto item = static_cast<BaseItem*>(thisitem.internalPointer());
        if (item->type == BaseItem::ItemType::Equipment){
            retVal = static_cast<Equipment*>(item)->GetLabels().size();
        }
        if (item->type == BaseItem::ItemType::Label){
            retVal = static_cast<Label*>(item)->getParameters().size();
        }
    }*/

    if (thisitem.column() <= 0) {
        // If the parent isn't valid, this is the number of immediate children of the
        // root, which for our data model is the count of TopData objects in our vector. Easy.
        if (!thisitem.isValid()) {
            retVal = static_cast<int>(myData.size());
        }

        if (thisitem.isValid()){
            auto item = static_cast<BaseItem*>(thisitem.internalPointer());
            if (item->type == BaseItem::ItemType::Equipment){
                retVal = static_cast<Equipment*>(item)->GetLabels().size();
            }
            if (item->type == BaseItem::ItemType::Label){
                retVal = static_cast<Label*>(item)->getParameters().size();
            }
        }
        // At this point, we're working on drill downs. We can be for:
        // -A TopData Row -- our parent is the root.
        // -A MiddleData row
        // -A ChildData row.
        //
        // There's probably a more clever way to write this using lambdas or something, but this works.
        /*
        else if (!thisitem.parent().isValid()) {
            // Our parent is the root (which is handled above, so we're a TopData, and
            // we just return the number of children.
            Equipment *td = static_cast<Equipment*>(myData.at(thisitem.row()));
            retVal = static_cast<int>(td->GetLabels().size());
        }

        // This is a little gross.
        else if (!thisitem.parent().parent().isValid()) {
            // We're a MiddleData.
            Equipment *td = static_cast<Equipment*>(myData.at(thisitem.parent().row()));
            Label *md = static_cast<Label*>(static_cast<Equipment*>(td)->GetLabels().at(thisitem.row()));
            retVal = static_cast<int>(md->getParameters().size());
        }
        */
    }

    return retVal;
}

/**
 * The number of columns is a constant.
 */
int MyDataModel::columnCount(const QModelIndex &) const {
    return MAX_COLUMNS;
}

/**
 * Does this node have children?
 */
bool MyDataModel::hasChildren(const QModelIndex &index) const {
    bool retVal = true;

    if (!index.isValid()) {
        // We're for the invisible root, which of course has children.
    }
    else if (!index.parent().isValid()) {
        // We don't have a parent, so we're a TopData object. We can retrieve the object and
        // return false if we have no middle data, but I'm just going to return true.
    }
    else if (!index.parent().parent().isValid()) {
        // Like rowCount(), this is getting gross, but it's just a demo.
        // In this case, we're a middle data. We have a parent, but no grandparent.
        // We can return true.
    }
    else {
        // We're a ChildData.
        retVal = false;
    }

    return retVal;
}

QVariant MyDataModel::headerData(int section, Qt::Orientation, int role) const
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
                    str_t info = static_cast<BaseParameter *>(baseItem)->getInfo();
                    switch(column) {
                        case COLUMN_NAME: retVal = paramname; break;
                        case COLUMN_STATUS:   break;
                        case COLUMN_BINARYVALUE: break;
                        case COLUMN_INFO: retVal =info; break;
                        case COLUMN_ENGVALUE: retVal = label->valueEng(paramname); break;
                        default: retVal = ""; break;
                    }
                    break;
                }
                case BaseItem::ItemType::Label:
                {
                    Label *label = static_cast<Label *>(baseItem);
                    switch(column) {
                        case COLUMN_NAME: retVal = label->getName(); break;
                        case COLUMN_STATUS: retVal = label->getStatus(); break;
                        case COLUMN_ENGVALUE: break;
                        case COLUMN_INFO: retVal = "Type: " + label->Type; break;
                        case COLUMN_BINARYVALUE:  retVal = label->valueBits(); break;
                        case COLUMN_DATARATE: retVal = label->getDataRate(); break;


                        default: retVal = ""; break;
                    }
                    break;
                }
                case BaseItem::ItemType::Equipment:
                {
                    Equipment *topData = static_cast<Equipment *>(baseItem);
                    switch(column) {
                        case COLUMN_NAME: retVal = topData->getEqipmentIdAndName(); break;
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

/**
 * This creates a QModelIndex for a particular cell. Our internal data points to an IndexData.
 */
bool  MyDataModel::updateModelData(Label* _item){
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
                    std::cout << " Item name  = " <<_item << "     Searched item name = "<< _tmp << "  ...\n";
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

    // If this is top level...
    if (!parent.isValid()) {
        if (row >= static_cast<int>(myData.size()) ) {
            return QModelIndex();
        }
        Equipment * topData = static_cast<Equipment*>(myData.at(row));
        return createIndex(row, column, topData);
    }

    // As the parent is valid, we can use his internalPointer.
    BaseItem * baseData = static_cast<BaseItem *>(parent.internalPointer());
    if (baseData == nullptr) {
        std::cout << "We have a legitimate parent, but his internalPointer is null!" << endl;
        exit(0);
    }

    // The parent can either be a TopData or MiddleData.
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
        ptr = md->getParameters().at(row);
    }

    return createIndex(row, column, ptr);
}

/**
 * The need for this method is extremely annoying. For this previously-created index, we need
 * to return our parent. But we couldn't have created the index without traversing the parent.
 * Unforunately, indexes don't save their parents, which is ridiculous. I'm sure it has something
 * to do with the expand/collapse business.
 *
 * Inside this method, it is invalid to call parent(), because that can cause infinite recursion.
 *
 * You need to rely on the internalPointer(). If your data is homogenous (like a true tree structure
 * where all nodes are the same data type), and your data keeps a pointer to the parent, then this
 * isn't remotely difficult. My original data was NOT homogenous, which is why I ended up making all
 * of my objects subclass from BaseData. Your model may need to do something more clever if you have
 * complicated data that you can't alter. You may need to create wrapper objects or something.
 */
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
        // No parent means it's the root.
        return retVal;
    }

    int loc = -1;
    if (parent->type == BaseItem::ItemType::Equipment) {
        loc = myData.indexOf(static_cast<Equipment *>(parent));
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
    bool bIfSatusChanged = false;

    BaseItem *item = static_cast<BaseItem*>(_index.internalPointer());

    if (item->type == BaseItem::ItemType::Parameter)
    {
        auto param = static_cast<BaseParameter*>(item);
        auto label = static_cast<Label*>(item->parent);
        if (param && label)
        {
            switch(param->getType())
            {
            case DArincParamType::BCD :
            {
                switch(column) {
                    case COLUMN_NAME:  break;
                    case COLUMN_STATUS: break;
                    case COLUMN_ENGVALUE: label->setValueEng(param->GetName(), value); break;
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
                    case COLUMN_ENGVALUE: label->setValueEng(param->GetName(), value); break;
                    case COLUMN_BINARYVALUE: break;

                    case COLUMN_INFO:   break;

                    default:  break;
                }
                break;
            }
            case DArincParamType::DISCRETE :
            {
                //
                switch(column) {
                    case COLUMN_NAME:  break;
                    case COLUMN_STATUS: break;
                    case COLUMN_ENGVALUE: label->setValueEng(param->GetName(), value); break;
                   // break;
                    case COLUMN_BINARYVALUE: break;

                    case COLUMN_INFO:   break;

                    default:  break;
                }
                break;
                }
            }
        }
    }
    //
    if (item->type == BaseItem::ItemType::Label)
    {
        auto label = static_cast<Label*>(item);
        switch(column) {
            case COLUMN_NAME: break;
            case COLUMN_STATUS: {
                label->setStatus(value.toString());
                bIfSatusChanged = true;
                break;
            }
            case COLUMN_ENGVALUE: break;
            case COLUMN_BINARYVALUE: {
                label->setValueBits(value);
                break;
            }                
            case COLUMN_INFO: break;
            case COLUMN_DATARATE: label->setDataRate(value); break;

            default:  break;
        }
        QModelIndex _parent = index(0,0);
        emit dataChanged(index(_index.row(),0,_parent), index(_index.row(),MAX_COLUMNS,_parent), {Qt::DisplayRole, Qt::EditRole});
        emit layoutChanged();
    }
    /*
     *
     * recording  actions to be sent to arinc board through usb
     *
    */
    int chanell = getChanell();
    int dei = getDEI();

    if (item->type == BaseItem::ItemType::Label)
    {
        auto label = static_cast<Label*>(item);
        if (label->getIfActive())
        {
            if (!bIfSatusChanged){
                addLabelAction(dei, chanell, 1, Instructions::UPDATE_LABEL_DATA_FOR_TRANSMIT, label);
            }
            else {
                addLabelAction(dei, chanell, 1, Instructions::ADD_LABEL_TO_TRANSMIT, label);
            }
        }
        else
        {
            addLabelAction(dei, chanell, 1, Instructions::REMOVE_LABEL_FROM_TRANSMIT, label);
        }
    }

    if (item->type == BaseItem::ItemType::Parameter)
    {
        auto label = static_cast<Label*>(_index.parent().internalPointer());
        if (label->getIfActive())
        {
            if (!bIfSatusChanged){
                addLabelAction(dei, chanell, 1, Instructions::UPDATE_LABEL_DATA_FOR_TRANSMIT, label);
            }
            else {
                addLabelAction(dei, chanell, 1, Instructions::ADD_LABEL_TO_TRANSMIT, label);
            }
        }
        else
        {
            addLabelAction(dei, chanell, 1, Instructions::REMOVE_LABEL_FROM_TRANSMIT, label);
        }
    }
    return true;
}

bool MyDataModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    const bool result = true; //rootItem->setData(section, value);

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
        /*  do our insertion ... */
        equipment->insertLabel(position,labelToInsert);
        endInsertRows();
    }
    return true;
}

bool MyDataModel::setLabelData(str_t labelId, const float& rate,  const QVariant &value)
{
    bool retval = false;
    counter++;
    QModelIndex _index = findLabel(labelId);
    if (_index.isValid())
    {
        BaseItem *item = static_cast<BaseItem*>(_index.internalPointer());
        auto label = static_cast<Label*>(item);
        label->setValueBits(value);
        label->setDataRate(rate);
        QModelIndex _parent = index(0,0);
        if (bIfLayoutRefresh){
             dataChanged(index(_index.row(),0,_parent), index(_index.row(),MAX_COLUMNS,_parent), {Qt::DisplayRole, Qt::EditRole});
             layoutChanged();
            bIfLayoutRefresh = false;
        }
        retval =  true;
    }
    else
    {
        if (addLabel(labelId)){
            QModelIndex _index = findLabel(labelId);
            BaseItem *item = static_cast<BaseItem*>(_index.internalPointer());
            auto label = static_cast<Label*>(item);
            label->setValueBits(value);
            label->setDataRate(rate);
            QModelIndex _parent = index(0,0);
            if (bIfLayoutRefresh){
                dataChanged(index(_index.row(),0,_parent), index(_index.row(),MAX_COLUMNS,_parent), {Qt::DisplayRole, Qt::EditRole});
                layoutChanged();
                bIfLayoutRefresh = false;
            }
             retval = true;
            }
    }

    return retval;
}

/*
    QModelIndex _index = findLabel(labelId);
    QModelIndex __index;
    if (_index.isValid()){
        __index = index(_index.row(), COLUMN_BINARYVALUE, _index.parent());
        setData(__index, value, Qt::EditRole);
        __index = index(_index.row(), COLUMN_DATARATE, _index.parent());
        setData(__index, rate, Qt::EditRole);
        return true;
    }
    if (addLabel(labelId)){
        QModelIndex _index = findLabel(labelId);
        __index = index(_index.row(), COLUMN_BINARYVALUE, _index.parent());
        setData(__index, value, Qt::EditRole);
        __index = index(_index.row(), COLUMN_DATARATE, _index.parent());
        setData(__index, rate, Qt::EditRole);
        return true;
    }
    return false;
*/


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
               if (_tmp->LabelId == labelId){
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
                Label* _label = static_cast<Label*>(_base);
                LabelFor<DReceiver>* _tmp = dynamic_cast<LabelFor<DReceiver>*>(_label);
                if (_tmp){
                    _tmp->evalDataRate() ;
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
   //  qInfo() << "MyDataModel::addLabel is runnig on " << QThread::currentThread() ;
    QModelIndex topindex = index(0,0,QModelIndex());
    BaseItem* topItem = static_cast<BaseItem*>(topindex.internalPointer());

    if (!topItem)
        return false;

    if (topItem->type == BaseItem::ItemType::Equipment){
        Equipment* equipment = static_cast<Equipment*>(topItem);
    //    if (equipment->checkLabel(labelId)){
        beginInsertRows(topindex, 0, 0);
        /*  do our insertion ... */
        equipment->insertLabel(0,labelId);
        endInsertRows();
  //      }

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
        /*  do our insertion ... */
        equipment->addReservedLabel();
        endInsertRows();
    }
    else {
        return false;
    }
    return true;
}


void MyDataModel::setLabelToInsert(str_t _labelToInsert){
     labelToInsert = _labelToInsert;
}

bool MyDataModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    BaseItem* parentItem = static_cast<BaseItem*>(parent.internalPointer());
    //    return false;
    if (parentItem->type == BaseItem::ItemType::Equipment){
       Equipment* equipment = static_cast<Equipment*>(parentItem);
       beginRemoveRows(parent, position, position + rows - 1);
         equipment->removeLabel(position);
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

bool MyDataModel::removeRow(QModelIndex _index){
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


//! [3]
Qt::ItemFlags MyDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    if (bIfEditable && (index.column() !=0)){
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    }
    else {
        return QAbstractItemModel::flags(index);
    }
}
//! [3]

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
                    if ( _tmp->getIfDataAvailable() ){
                        tmpListOfDataToSend.push_back(_tmp->getArincDataAndReset());
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
                    _tmp->incrementDataRateCounter();
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
                    _tmp->evalDataRate() ;
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
                        _garbage.push_back(_tmp->LabelId);
                    }
                }
            }
        }
    }
    return _garbage;
}

void MyDataModel::addLabelAction(uint32_t dei, uint32_t deichanell, uint32_t transrec, uint32_t instr, Label* label)
{
    auto a =  MakeDataAction(dei, deichanell, transrec, instr, label->getArincData().getBitSet().to_ulong(), label->getDataRate().toFloat())->toPacket();
    auto x = TransmitterRecords::getInstance()->record(a);
}

int MyDataModel::getChanell()
{
    transmitter* x = dynamic_cast<transmitter*>(tranciver);
    if (x){
        return x->chanell;
    }

    Receiver* y = dynamic_cast<Receiver*>(tranciver);
    if (y){
        return y->chanell;
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

/*

bool MyDataModel::setLabelData(str_t labelId, const QVariant &value){
    QModelIndex _index = findLabel(labelId);

    if (_index.isValid()){

        BaseItem *item = static_cast<BaseItem*>(_index.internalPointer());
        auto label = static_cast<Label*>(item);
         label->setValueBits(value);
        QModelIndex _parent = index(0,0);
        emit dataChanged(index(_index.row(),0,_parent), index(_index.row(),MAX_COLUMNS,_parent), {Qt::DisplayRole, Qt::EditRole});
        layoutChanged();
        return true;
    }
    if (addLabel(labelId)){
        QModelIndex _index = findLabel(labelId);

        BaseItem *item = static_cast<BaseItem*>(_index.internalPointer());
        auto label = static_cast<Label*>(item);
         label->setValueBits(value);
        QModelIndex _parent = index(0,0);
        emit dataChanged(index(_index.row(),0,_parent), index(_index.row(),MAX_COLUMNS,_parent), {Qt::DisplayRole, Qt::EditRole});
        layoutChanged();

       // setData(_index,value, Qt::EditRole);
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
        label->setValueBits(value);
        QModelIndex _parent = index(0,0);
        emit dataChanged(index(_index.row(),0,_parent), index(_index.row(),MAX_COLUMNS,_parent), {Qt::DisplayRole, Qt::EditRole});
        layoutChanged();
        return true;
    }

    if (addLabel(labelId)){
        QModelIndex _index = findLabel(labelId);
        BaseItem *item = static_cast<BaseItem*>(_index.internalPointer());
        auto label = static_cast<Label*>(item);
        label->setValueBits(value);
        QModelIndex _parent = index(0,0);
        emit dataChanged(index(_index.row(),0,_parent), index(_index.row(),MAX_COLUMNS,_parent), {Qt::DisplayRole, Qt::EditRole});
        layoutChanged();
        return true;
    }

    return false;
}

 */
