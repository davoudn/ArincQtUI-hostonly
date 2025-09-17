#include "checkboxarray.h"
#include "ui_checkboxarray.h"
#include <QListWidget>
#include <QStringList>
#include <QString>
#include "types.h"
CheckBoxarray::CheckBoxarray(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CheckBoxarray)
{
    ui->setupUi(this);
    /*
     *
     */

    QStringList itemLabels;
    itemLabels.push_back("Bit09");
    for (int i=10 ; i<=ARINC32_SIZE;i++ ){
        itemLabels.push_back("Bit"+QString::number(i));
    }
    listWidget = new QListWidget(this);
    for (const QString &label : itemLabels) {
        QListWidgetItem *listItem = new QListWidgetItem(label, listWidget);

        listItem->setCheckState(Qt::Unchecked);
        listWidget->addItem(listItem);
    }
    listWidget->setViewMode(QListWidget::IconMode);
    listWidget->setWrapping(true);

    listWidget->setFlow(QListWidget::LeftToRight);

    listWidget->resize(600,60);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

CheckBoxarray::~CheckBoxarray()
{
    delete ui;
    delete listWidget;
}

void CheckBoxarray::setBits(value_t _bits){
    if (_bits.toString().size() == listWidget->count()){
        for (uint32_t i=0; i<_bits.toString().size();i++){
            if (_bits.toString()[i] == '0'){
                listWidget->item(i)->setCheckState(Qt::Unchecked);
            }
            if (_bits.toString()[i] == '1'){
                listWidget->item(i)->setCheckState(Qt::Checked);
            }
        }
    }
}
value_t CheckBoxarray::getBits(){
    str_t tmp;//="111111111111111111111111";
   // qDebug() << listWidget->count() <<"<<---\n";
    for (uint32_t i=0; i<listWidget->count();i++){
        if (listWidget->item(i)->checkState()){
            tmp+='1';
        }
        else {
            tmp+='0';
        }
    }

    return tmp;
}

//  listItem->setSizeHint(QSize(50,10));
//  auto _font = QFont();
//  _font.setPointSize(9);
//  listItem->setFont(_font);
// listWidget->setResizeMode(QListView::Adjust);
// int _size = listWidget->count();
//QListWidgetItem *listItem = listWidget->item(_size-1);
//listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
// listWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
