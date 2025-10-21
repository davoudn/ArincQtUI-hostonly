/********************************************************************************
** Form generated from reading UI file 'receiver_setup.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECEIVER_SETUP_H
#define UI_RECEIVER_SETUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QComboBox *cSDIEnable;
    QComboBox *cSDIChanell;
    QComboBox *cDatarate;
    QLabel *lSDIEnable;
    QLabel *lSDIChanell;
    QLabel *ldataRate;
    QPushButton *pbApply;
    QPushButton *pbCancel;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(267, 186);
        cSDIEnable = new QComboBox(Dialog);
        cSDIEnable->setObjectName("cSDIEnable");
        cSDIEnable->setGeometry(QRect(130, 10, 121, 22));
        cSDIChanell = new QComboBox(Dialog);
        cSDIChanell->setObjectName("cSDIChanell");
        cSDIChanell->setGeometry(QRect(130, 40, 121, 22));
        cDatarate = new QComboBox(Dialog);
        cDatarate->setObjectName("cDatarate");
        cDatarate->setGeometry(QRect(130, 70, 121, 22));
        lSDIEnable = new QLabel(Dialog);
        lSDIEnable->setObjectName("lSDIEnable");
        lSDIEnable->setGeometry(QRect(20, 20, 91, 16));
        lSDIChanell = new QLabel(Dialog);
        lSDIChanell->setObjectName("lSDIChanell");
        lSDIChanell->setGeometry(QRect(20, 50, 91, 16));
        ldataRate = new QLabel(Dialog);
        ldataRate->setObjectName("ldataRate");
        ldataRate->setGeometry(QRect(20, 80, 81, 16));
        pbApply = new QPushButton(Dialog);
        pbApply->setObjectName("pbApply");
        pbApply->setGeometry(QRect(80, 160, 80, 22));
        pbCancel = new QPushButton(Dialog);
        pbCancel->setObjectName("pbCancel");
        pbCancel->setGeometry(QRect(170, 160, 80, 22));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        lSDIEnable->setText(QCoreApplication::translate("Dialog", "SDI Enable:", nullptr));
        lSDIChanell->setText(QCoreApplication::translate("Dialog", "SDI Chanell:", nullptr));
        ldataRate->setText(QCoreApplication::translate("Dialog", "Data Rate:", nullptr));
        pbApply->setText(QCoreApplication::translate("Dialog", "Apply", nullptr));
        pbCancel->setText(QCoreApplication::translate("Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECEIVER_SETUP_H
