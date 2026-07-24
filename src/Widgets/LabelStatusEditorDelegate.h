#ifndef LABELSTATUSEDITORDELEGATE_H
#define LABELSTATUSEDITORDELEGATE_H

#include "Label.h"
#include "types.h"

#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QComboBox>
#include <QModelIndex>

// EDITORDELEGATE_H


class LabelStatusEditorDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    LabelStatusEditorDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QWidget *editor = nullptr;
        BaseItem* item = static_cast<BaseItem*>(index.internalPointer());

        switch (item->type) {
        case BaseItem::ItemType::Parameter: {

            break;
        }
        case BaseItem::ItemType::Label :{
            Label* label =  static_cast<Label*>(item);
            if (label && index.column() == 1){
                qDebug() << "yaboo---";
                //  editor = new QLineEdit(parent);
                editor = new QComboBox(parent);
                static_cast<QComboBox*>(editor)->addItem("Activate");
                static_cast<QComboBox*>(editor)->addItem("Idle");
                editor->setWindowFlags(Qt::FramelessWindowHint);
            }
            break;
        }
        case BaseItem::ItemType::Equipment: {
            break;
        }

        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override {

    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {
        //model->setData(index, static_cast<QLineEdit*>(editor)->text(), Qt::EditRole);
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QStyleOptionViewItem _option( option);
        BaseItem* item = static_cast<BaseItem*>(index.internalPointer());
        int x,y,w,h;
        _option.rect.getRect(&x, &y, &w, &h);
        if ( static_cast<BaseParameter*>(item)){
            if (static_cast<BaseParameter*>(item)->getType() == DArincParamType::DISCRETE){
                //     y+=170;
                //     x+=145;
            }
            else {
                y+=0;
                x+=0;
            }
        }
        else {
            Label* label =  static_cast<Label*>(item);
            if (label){
                y+=0;
                x+=0;
            }
        }
        _option.rect.setRect(x,y,w,h);
        editor->setGeometry(_option.rect);
    }
};
#endif // LABELSTATUSEDITORDELEGATE_H
