#ifndef TREEDATAMODEL_H
#define TREEDATAMODEL_H
#include "PointerVector.h"
#include <QTimer>
#include <QAbstractItemModel>
#include <vector>

class Label;
class BaseItem;
class DArincData;
class QThread;
class BaseAction;
/**
 * This is the model used to display our data.
 */
class MyDataModel: public QAbstractItemModel
{
    Q_OBJECT
private:
    QTimer layoutRefresher;
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

    bool removeRow(int position);
    bool removeRow(QModelIndex _index);
    bool addLabel(str_t _labelName);
    bool addReservedLabel();
    bool removeLabel(str_t _labelName);

public slots:
    bool setLabelData(str_t labelName, const float& rate,  const QVariant &value);
    void enableLayoutRefresh();
    void evalDataRates();
    void cleanTimeoutList();

public:
    // for transmitter
    std::vector<DArincData> getListOfAvailableLabelData();
    void incrementLabelsDataRateCounter();
    void addLabelAction(uint32_t dei, uint32_t deichanell, uint32_t transrec, uint32_t instr, Label* label);

    // for receiver
    std::vector<str_t> getTimeOutList();

    //
    QModelIndex findLabel(str_t _labelName);
    bool checkLabel(str_t _labelName);

    bool updateModelData(Label* _item);
    QModelIndex findIndex(Label* _item, const  QModelIndex parent = QModelIndex());
    QModelIndex index ( int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent ( const QModelIndex& index ) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setLabelToInsert(str_t _labelToInsert);

    QObject *tranciver = nullptr;

    int getChanell();
    int getDEI();
private:
    int counter = 0;
    str_t labelToInsert;
    bool bIfLayoutRefresh = false;
};

#endif // TREEDATAMODEL_H
