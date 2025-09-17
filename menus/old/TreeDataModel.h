#ifndef TREEDATAMODEL_H
#define TREEDATAMODEL_H

#include <QAbstractItemModel>
#include "PointerVector.h"
#include <vector>
class Label;
class BaseItem;
class DArincData;
/**
 * This is the model used to display our data.
 */
class MyDataModel: public QAbstractItemModel
{
    Q_OBJECT
private:
    PointerVector<BaseItem>& myData;

    int depth(QModelIndex &index);
    bool bIfEditable = false;
public:
    MyDataModel(QObject *parent, PointerVector<BaseItem>& vec, bool _bIfEditable);
    ~MyDataModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool hasChildren(const QModelIndex &) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    bool insertRows(int position, int rows,
                    const QModelIndex &parent = {}) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = {}) override;

    bool addLabel(str_t _labelName);
    bool removeLabel(str_t _labelName);
    bool setLabelData(str_t labelName, const QVariant &value);
    bool setLabelData(str_t labelName, const float& rate,  const QVariant &value);

    std::vector<DArincData> getListOfAvailableLabelData();

    QModelIndex findLabel(str_t _labelName);
    bool checkLabel(str_t _labelName);

    std::vector<str_t> getTimeOutList(double _timeOut);
    void               cleanTimeoutList(double _timeOut);

    bool updateModelData(Label* _item);
    QModelIndex findIndex(Label* _item, const  QModelIndex parent = QModelIndex());
    QModelIndex index ( int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent ( const QModelIndex& index ) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setLabelToInsert(str_t _labelToInsert);
private:
    str_t labelToInsert;
};

#endif // TREEDATAMODEL_H
