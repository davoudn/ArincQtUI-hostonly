/********************************************************************************
** Form generated from reading UI file 'receiver.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECEIVER_H
#define UI_RECEIVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Receiver
{
public:
    QFrame *frame;
    QLabel *label;
    QComboBox *equipmentSelector;
    QTreeView *treeView;
    QLabel *lSDIEnable;
    QComboBox *c_bitRate_enableSDI;
    QCheckBox *chReceiverDisabled;
    QCheckBox *chReceiverEnabled;

    void setupUi(QWidget *Receiver)
    {
        if (Receiver->objectName().isEmpty())
            Receiver->setObjectName("Receiver");
        Receiver->resize(1550, 751);
        QFont font;
        font.setBold(true);
        Receiver->setFont(font);
        frame = new QFrame(Receiver);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(10, 10, 681, 41));
        frame->setFrameShape(QFrame::NoFrame);
        label = new QLabel(frame);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 10, 241, 21));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Liberation Mono")});
        font1.setPointSize(16);
        font1.setBold(true);
        font1.setItalic(true);
        label->setFont(font1);
        equipmentSelector = new QComboBox(frame);
        equipmentSelector->setObjectName("equipmentSelector");
        equipmentSelector->setGeometry(QRect(270, 0, 401, 31));
        treeView = new QTreeView(Receiver);
        treeView->setObjectName("treeView");
        treeView->setGeometry(QRect(10, 50, 1511, 651));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(true);
        treeView->setFont(font2);
        treeView->setTabletTracking(true);
        treeView->setAlternatingRowColors(true);
        lSDIEnable = new QLabel(Receiver);
        lSDIEnable->setObjectName("lSDIEnable");
        lSDIEnable->setGeometry(QRect(1190, 10, 101, 31));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Liberation Sans")});
        font3.setPointSize(11);
        font3.setBold(true);
        font3.setItalic(true);
        lSDIEnable->setFont(font3);
        c_bitRate_enableSDI = new QComboBox(Receiver);
        c_bitRate_enableSDI->setObjectName("c_bitRate_enableSDI");
        c_bitRate_enableSDI->setGeometry(QRect(1300, 10, 221, 31));
        chReceiverDisabled = new QCheckBox(Receiver);
        chReceiverDisabled->setObjectName("chReceiverDisabled");
        chReceiverDisabled->setGeometry(QRect(1140, 720, 85, 20));
        chReceiverEnabled = new QCheckBox(Receiver);
        chReceiverEnabled->setObjectName("chReceiverEnabled");
        chReceiverEnabled->setGeometry(QRect(1040, 720, 91, 20));

        retranslateUi(Receiver);

        QMetaObject::connectSlotsByName(Receiver);
    } // setupUi

    void retranslateUi(QWidget *Receiver)
    {
        Receiver->setWindowTitle(QCoreApplication::translate("Receiver", "Form", nullptr));
        label->setText(QCoreApplication::translate("Receiver", "Equipment ID = ", nullptr));
        lSDIEnable->setText(QCoreApplication::translate("Receiver", "SDI/BitRate : ", nullptr));
        chReceiverDisabled->setText(QCoreApplication::translate("Receiver", "Disabled", nullptr));
        chReceiverEnabled->setText(QCoreApplication::translate("Receiver", "Enabled", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Receiver: public Ui_Receiver {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECEIVER_H
