/********************************************************************************
** Form generated from reading UI file 'transmmiter_setup_ui.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSMMITER_SETUP_UI_H
#define UI_TRANSMMITER_SETUP_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_transmmiter_setup_ui
{
public:
    QComboBox *cDatarate;
    QPushButton *pbCancel;
    QLabel *ldataRate;
    QPushButton *pbApply;

    void setupUi(QWidget *transmmiter_setup_ui)
    {
        if (transmmiter_setup_ui->objectName().isEmpty())
            transmmiter_setup_ui->setObjectName("transmmiter_setup_ui");
        transmmiter_setup_ui->resize(249, 87);
        cDatarate = new QComboBox(transmmiter_setup_ui);
        cDatarate->setObjectName("cDatarate");
        cDatarate->setGeometry(QRect(120, 10, 121, 22));
        pbCancel = new QPushButton(transmmiter_setup_ui);
        pbCancel->setObjectName("pbCancel");
        pbCancel->setGeometry(QRect(160, 50, 80, 22));
        ldataRate = new QLabel(transmmiter_setup_ui);
        ldataRate->setObjectName("ldataRate");
        ldataRate->setGeometry(QRect(20, 10, 81, 16));
        pbApply = new QPushButton(transmmiter_setup_ui);
        pbApply->setObjectName("pbApply");
        pbApply->setGeometry(QRect(70, 50, 80, 22));

        retranslateUi(transmmiter_setup_ui);

        QMetaObject::connectSlotsByName(transmmiter_setup_ui);
    } // setupUi

    void retranslateUi(QWidget *transmmiter_setup_ui)
    {
        transmmiter_setup_ui->setWindowTitle(QCoreApplication::translate("transmmiter_setup_ui", "Form", nullptr));
        pbCancel->setText(QCoreApplication::translate("transmmiter_setup_ui", "Cancel", nullptr));
        ldataRate->setText(QCoreApplication::translate("transmmiter_setup_ui", "Data Rate:", nullptr));
        pbApply->setText(QCoreApplication::translate("transmmiter_setup_ui", "Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class transmmiter_setup_ui: public Ui_transmmiter_setup_ui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSMMITER_SETUP_UI_H
