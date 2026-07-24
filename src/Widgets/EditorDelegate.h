#ifndef EDITORDELEGATE_H
#define EDITORDELEGATE_H
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QComboBox>
#include <QModelIndex>
#include "checkboxarray.h"
#include "BaseParameter.h"
#include "Label.h"
//#include "BNRParameter.h"
//#include "BCDParameter.h"

#include "types.h"
 // EDITORDELEGATE_H


class EditorDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    EditorDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QWidget *editor = nullptr;
        BaseItem* item = static_cast<BaseItem*>(index.internalPointer());

        switch (item->type) {
            case BaseItem::ItemType::Parameter: {
                BaseParameter* param = static_cast<BaseParameter*>(item);
                switch (param->getType()) {
                    case DArincParamType::BCD :{
                        if (index.column() == 2){
                           editor = new QLineEdit(parent);
                           editor->setWindowFlags(Qt::FramelessWindowHint);
                        }
                        break;
                    }
                    case DArincParamType::BNR:{
                        if (index.column() == 2){
                            editor = new QLineEdit(parent);
                            editor->setWindowFlags(Qt::FramelessWindowHint);
                        }
                        break;
                    }
                    case DArincParamType::DISCRETE:{
                        if (index.column() == 2){
                           editor = new QComboBox(parent);
                           for (auto& x: param->GetDetails()){
                               static_cast<QComboBox*>(editor)->addItem(x.second.toString());
                           }
                           editor->setWindowFlags(Qt::FramelessWindowHint);
                        }
                        break;
                    }
                }
                break; // BaseItem::ItemType::Parameter:
            }

            case BaseItem::ItemType::Label :{
                Label* label =  static_cast<Label*>(item);
                if (label && index.column() == 1){
                      editor = new QComboBox(parent);
                      static_cast<QComboBox*>(editor)->addItem("Active");
                      static_cast<QComboBox*>(editor)->addItem("Idle");
                      editor->setWindowFlags(Qt::FramelessWindowHint);
                }
                if (label && index.column() == 4){
                    editor = new CheckBoxarray(parent);
                }
                break;
            } // BaseItem::ItemType::Label :

            case BaseItem::ItemType::Equipment: {
                 break;
            } // BaseItem::ItemType::Equipment:
        }

    return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override
    {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();
        auto item  = static_cast<BaseItem*>(index.internalPointer());
        switch (item->type) {
            case BaseItem::ItemType::Parameter: {
                auto param = static_cast<BaseParameter*>(item);
                switch (param->getType()) {
                    case DArincParamType::BCD :{
                        if (index.column() == 2){
                          static_cast<QLineEdit*>(editor)->setText(value);
                        }
                        break;
                    }
                    case DArincParamType::BNR:{
                        if (index.column() == 2){
                            static_cast<QLineEdit*>(editor)->setText(value);
                        }
                        break;
                    }
                    case DArincParamType::DISCRETE:{
                        if (index.column() == 2){
                            static_cast<QComboBox*>(editor)->setEditText(value);
                        }
                        break;
                    }
                }
                break;
            }
            case BaseItem::ItemType::Label :{
                if (index.column() == 4){
                    static_cast<CheckBoxarray*>(editor)->setBits(value);
                }
                break;
            }
            case BaseItem::ItemType::Equipment:{
                break;
            }
        }
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override
    {
        auto item  = static_cast<BaseItem*>(index.internalPointer());
        switch (item->type) {
        case BaseItem::ItemType::Parameter: {
            auto param = static_cast<BaseParameter*>(item);
            switch (param->getType()) {
            case DArincParamType::BCD : {
                if (index.column() == 2){
                    model->setData(index, static_cast<QLineEdit*>(editor)->text(), Qt::EditRole);
                }
                break;
            }
            case DArincParamType::BNR:  {
                if (index.column() == 2){
                    model->setData(index, static_cast<QLineEdit*>(editor)->text(), Qt::EditRole);
                }
                break;
            }
            case DArincParamType::DISCRETE: {
                if (index.column() == 2){
                    int idx = static_cast<QComboBox*>(editor)->currentIndex();
                    value_t value = static_cast<QComboBox*>(editor)->itemText(idx);
                    model->setData(index, value, Qt::EditRole);
                }
                break;
            }
            }
            break;
        }
        case BaseItem::ItemType::Label :{
            if (index.column() == 4){
                auto value = static_cast<CheckBoxarray*>(editor)->getBits();
                model->setData(index, value, Qt::EditRole);
            }
            if (index.column() == 1){
                int idx = static_cast<QComboBox*>(editor)->currentIndex();
                auto value = static_cast<QComboBox*>(editor)->itemText(idx);
                model->setData(index, value, Qt::EditRole);
            }
            break;
        }
        case BaseItem::ItemType::Equipment:{
            break;
        }
        }
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyleOptionViewItem _option( option);
        BaseItem* item = static_cast<BaseItem*>(index.internalPointer());
         int x,y,w,h;
        _option.rect.getRect(&x, &y, &w, &h);

        switch (item->type) {
            case BaseItem::ItemType::Parameter: {
             break;
             }
            case BaseItem::ItemType::Label :{
                if (index.column() == 4){
                  //   x+=20;
                 //    y+=95;
                       h+=100;
                }
                break;
            }
            case BaseItem::ItemType::Equipment:{
                break;
            }
        }
        _option.rect.setRect(x,y,w,h);
        editor->setGeometry(_option.rect);
    }
};
#endif
