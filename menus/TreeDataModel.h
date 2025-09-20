#ifndef TREEDATAMODEL_H
#define TREEDATAMODEL_H
#include "PointerVector.h"
#include "action.h"
#include <QAbstractItemModel>
#include <vector>

class Label;
class BaseItem;
class DArincData;
class QThread;
/**
 * This is the model used to display our data.
 */
class MyDataModel: public QAbstractItemModel
{
    Q_OBJECT
private:
    PointerVector<BaseItem>& myData;

    std::vector<action> actions;
    uint32_t chanell = 0;

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
    bool setLabelData(str_t labelName, const QVariant &value);
    bool setLabelData(str_t labelId, const QVariant &value, QThread* thread);
    bool setLabelData(str_t labelName, const float& rate,  const QVariant &value);

    // for transmitter
    std::vector<DArincData> getListOfAvailableLabelData();
    void incrementLabelsDataRateCounter();
    std::vector<action>& getActions();
    void addLabelAction(uint32_t ch, uint32_t trans_rec, uint32_t instr, Label* label);
    void addControlAction(uint32_t ch, uint32_t trans_rec, uint32_t instr, uint16_t controlword);

    // for receiver
    void evalDataRates();
    void evalDataRates(double _resettime);
    std::vector<str_t> getTimeOutList();
    void               cleanTimeoutList();

    //
    QModelIndex findLabel(str_t _labelName);
    bool checkLabel(str_t _labelName);

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
