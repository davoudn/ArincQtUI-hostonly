// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QApplication>

#include "mainwindow.h"
#include "DEI1016.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    // DE1016 is initialized
    QThread DEIThread;

    DEI1016::getInstance();//->moveToThread(&DEIThread);
   // DEI1016::getInstance()->openSerialPort();
    window.show();


    return app.exec();
}
