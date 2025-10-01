/********************************************************************************
** Form generated from reading UI file 'transmitter.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSMITTER_H
#define UI_TRANSMITTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_transmitter
{
public:
    QComboBox *labelSelector;
    QComboBox *equipmentSelector;
    QLabel *label;
    QLabel *label_2;
    QTreeView *treeView;
    QPushButton *addLabel;
    QPushButton *removeLabel;
    QLabel *lArincBitRate;
    QComboBox *cArinc_parity_bitRate;
    QPushButton *pLoadConfig;
    QPushButton *pSaveConfig;
    QLabel *lSelectedFile;
    QCheckBox *chTransmitterEnabled;
    QCheckBox *chTransmitterDisabled;

    void setupUi(QWidget *transmitter)
    {
        if (transmitter->objectName().isEmpty())
            transmitter->setObjectName("transmitter");
        transmitter->resize(1616, 638);
        transmitter->setAutoFillBackground(true);
        labelSelector = new QComboBox(transmitter);
        labelSelector->setObjectName("labelSelector");
        labelSelector->setGeometry(QRect(560, 10, 311, 31));
        equipmentSelector = new QComboBox(transmitter);
        equipmentSelector->setObjectName("equipmentSelector");
        equipmentSelector->setGeometry(QRect(150, 10, 271, 31));
        label = new QLabel(transmitter);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 20, 131, 16));
        QFont font;
        font.setFamilies({QString::fromUtf8("Liberation Mono")});
        font.setBold(true);
        font.setItalic(true);
        label->setFont(font);
        label_2 = new QLabel(transmitter);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(470, 20, 81, 16));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Liberation Sans")});
        font1.setBold(true);
        font1.setItalic(true);
        label_2->setFont(font1);
        treeView = new QTreeView(transmitter);
        treeView->setObjectName("treeView");
        treeView->setGeometry(QRect(0, 50, 1601, 501));
        QFont font2;
        font2.setPointSize(9);
        font2.setBold(true);
        font2.setStyleStrategy(QFont::PreferDefault);
        treeView->setFont(font2);
        treeView->setAutoFillBackground(true);
        treeView->setFrameShape(QFrame::NoFrame);
        treeView->setAlternatingRowColors(true);
        treeView->header()->setCascadingSectionResizes(true);
        addLabel = new QPushButton(transmitter);
        addLabel->setObjectName("addLabel");
        addLabel->setGeometry(QRect(890, 10, 171, 31));
        addLabel->setFont(font1);
        removeLabel = new QPushButton(transmitter);
        removeLabel->setObjectName("removeLabel");
        removeLabel->setGeometry(QRect(1070, 10, 151, 31));
        removeLabel->setFont(font);
        lArincBitRate = new QLabel(transmitter);
        lArincBitRate->setObjectName("lArincBitRate");
        lArincBitRate->setGeometry(QRect(1280, 20, 121, 20));
        lArincBitRate->setFont(font);
        cArinc_parity_bitRate = new QComboBox(transmitter);
        cArinc_parity_bitRate->setObjectName("cArinc_parity_bitRate");
        cArinc_parity_bitRate->setGeometry(QRect(1400, 10, 201, 31));
        pLoadConfig = new QPushButton(transmitter);
        pLoadConfig->setObjectName("pLoadConfig");
        pLoadConfig->setGeometry(QRect(30, 600, 171, 31));
        pSaveConfig = new QPushButton(transmitter);
        pSaveConfig->setObjectName("pSaveConfig");
        pSaveConfig->setGeometry(QRect(220, 600, 171, 31));
        lSelectedFile = new QLabel(transmitter);
        lSelectedFile->setObjectName("lSelectedFile");
        lSelectedFile->setGeometry(QRect(580, 610, 611, 16));
        chTransmitterEnabled = new QCheckBox(transmitter);
        chTransmitterEnabled->setObjectName("chTransmitterEnabled");
        chTransmitterEnabled->setGeometry(QRect(1390, 610, 91, 20));
        chTransmitterDisabled = new QCheckBox(transmitter);
        chTransmitterDisabled->setObjectName("chTransmitterDisabled");
        chTransmitterDisabled->setGeometry(QRect(1490, 610, 85, 20));

        retranslateUi(transmitter);

        QMetaObject::connectSlotsByName(transmitter);
    } // setupUi

    void retranslateUi(QWidget *transmitter)
    {
        transmitter->setWindowTitle(QCoreApplication::translate("transmitter", "Form", nullptr));
        label->setText(QCoreApplication::translate("transmitter", "Select Equipment:", nullptr));
        label_2->setText(QCoreApplication::translate("transmitter", "Select Label:", nullptr));
        addLabel->setText(QCoreApplication::translate("transmitter", "Add Label", nullptr));
        removeLabel->setText(QCoreApplication::translate("transmitter", "Remove Label", nullptr));
        lArincBitRate->setText(QCoreApplication::translate("transmitter", "Chanel Bit Rate:", nullptr));
        pLoadConfig->setText(QCoreApplication::translate("transmitter", "Load Config", nullptr));
        pSaveConfig->setText(QCoreApplication::translate("transmitter", "Save Config", nullptr));
        lSelectedFile->setText(QCoreApplication::translate("transmitter", "No file selected", nullptr));
        chTransmitterEnabled->setText(QCoreApplication::translate("transmitter", "Enabled", nullptr));
        chTransmitterDisabled->setText(QCoreApplication::translate("transmitter", "Disabled", nullptr));
    } // retranslateUi

};

namespace Ui {
    class transmitter: public Ui_transmitter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSMITTER_H
