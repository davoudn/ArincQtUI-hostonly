// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtWidgets>

#include "mainwindow.h"
#include "transmitter.h"
#include "receiver.h"
#include "settingsdialog.h"
#include "DEI1016RasberryConfigurations.h"
#include "DEI1016.h"


//! [0]
MainWindow::MainWindow()
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);
//! [0]

//! [1]
    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
                              "invoke a context menu</i>"));
    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    infoLabel->setAlignment(Qt::AlignCenter);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(topFiller);
    layout->addWidget(infoLabel);
    layout->addWidget(bottomFiller);
    widget->setLayout(layout);
//! [1]

//! [2]
    createActions();
    createMenus();

    QString message = tr("A context menu is available by right-clicking");
    statusBar()->showMessage(message);

    setWindowTitle(tr("Menus"));
    setMinimumSize(160, 160);
    resize(480, 320);

    DEI1016::getInstance(this);
}
//! [2]

//! [3]
#ifndef QT_NO_CONTEXTMENU
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
   // QMenu menu(this);

}
#endif // QT_NO_CONTEXTMENU
//! [3]

//

void MainWindow::transmitter0()
{
   // infoLabel->setText(tr("Invoked <b>File|New</b>"));
    transmitter::getInstance(CHANELL0)->show();

}

void MainWindow::transmitter1()
{
    // infoLabel->setText(tr("Invoked <b>File|New</b>"));
    transmitter::getInstance(CHANELL1)->show();

}

void MainWindow::receiver0()
{
    //infoLabel->setText(tr("Invoked <b>File|Open</b>"));
    Receiver::getInstance(CHANELL0)->show();

    // DeviceDelegate<DReceiver,DEVICEONE_CHANELLONE>::getInstance(R);

   // R->show();
}

void MainWindow::receiver1()
{
    //infoLabel->setText(tr("Invoked <b>File|Save</b>"));
    Receiver::getInstance(CHANELL1)->show();

}

void MainWindow::receiver2()
{
    //infoLabel->setText(tr("Invoked <b>File|Open</b>"));
    Receiver::getInstance(CHANELL2)->show();

    // DeviceDelegate<DReceiver,DEVICEONE_CHANELLONE>::getInstance(R);

    // R->show();
}

void MainWindow::receiver3()
{
    //infoLabel->setText(tr("Invoked <b>File|Save</b>"));
    Receiver::getInstance(CHANELL3)->show();

}


void MainWindow::record()
{
    infoLabel->setText(tr("Invoked <b>File|Print</b>"));
}

void MainWindow::importRecordFile()
{
    infoLabel->setText(tr("Invoked <b>Edit|Undo</b>"));
}

void MainWindow::receiverSetup()
{
    infoLabel->setText(tr("Invoked <b>Edit|Undo</b>"));
}

void MainWindow::serialBusSetup(){
    SettingsDialog::getInstance()->show();
}

void MainWindow::transmitterSetup()
{
    infoLabel->setText(tr("Invoked <b>Edit|Undo</b>"));
}

void MainWindow::exportRecordFile()
{
    infoLabel->setText(tr("Invoked <b>Edit|Redo</b>"));
}

void MainWindow::playback()
{
    infoLabel->setText(tr("Invoked <b>Edit|Redo</b>"));
}

void MainWindow::arinc429Database()
{
    infoLabel->setText(tr("Invoked <b>Edit|Redo</b>"));
}

void MainWindow::about()
{
    infoLabel->setText(tr("Invoked <b>Help|About</b>"));
    QMessageBox::about(this, tr("About Menu"),
            tr("The <b>Menu</b> example shows how to create "
               "menu-bar menus and context menus."));
}


//! [4]
void MainWindow::createActions()
{
//! [5]
    transmitter0Act = new QAction(QIcon::fromTheme("folder-alt"),
                         tr("&Transmitter 0"), this);
    transmitter0Act->setShortcuts(QKeySequence::New);
    transmitter0Act->setStatusTip(tr("Select transmitter 0."));
    connect(transmitter0Act, &QAction::triggered, this, &MainWindow::transmitter0);

    transmitter1Act = new QAction(QIcon::fromTheme("folder-alt"),
                                  tr("&Transmitter 1"), this);
    transmitter1Act->setShortcuts(QKeySequence::New);
    transmitter1Act->setStatusTip(tr("Select transmitter 1."));
    connect(transmitter1Act, &QAction::triggered, this, &MainWindow::transmitter1);
//! [4]

    receiver0Act = new QAction(QIcon::fromTheme("folder-alt"),
                          tr("&Receiver 0"), this);
    receiver0Act->setShortcuts(QKeySequence::Open);
    receiver0Act->setStatusTip(tr("Select receiver 0"));
    connect(receiver0Act, &QAction::triggered, this, &MainWindow::receiver0);

    receiver1Act = new QAction(QIcon::fromTheme("folder-alt"),
                               tr("&Receiver 1"), this);
    receiver1Act->setShortcuts(QKeySequence::Open);
    receiver1Act->setStatusTip(tr("Select receiver 1"));
    connect(receiver1Act, &QAction::triggered, this, &MainWindow::receiver1);

    receiver2Act = new QAction(QIcon::fromTheme("folder-alt"),
                               tr("&Receiver 2"), this);
    receiver2Act->setShortcuts(QKeySequence::Open);
    receiver2Act->setStatusTip(tr("Select receiver 2"));
    connect(receiver2Act, &QAction::triggered, this, &MainWindow::receiver2);

    receiver3Act = new QAction(QIcon::fromTheme("folder-alt"),
                               tr("&Receiver 3"), this);
    receiver3Act->setShortcuts(QKeySequence::Open);
    receiver3Act->setStatusTip(tr("Select receiver 3"));
    connect(receiver3Act, &QAction::triggered, this, &MainWindow::receiver3);
//! [5]

    recordAct = new QAction(QIcon::fromTheme("folder-alt"),
                          tr("&Record"), this);
    recordAct->setShortcuts(QKeySequence::Save);
    recordAct->setStatusTip(tr("Record ..."));
    connect(recordAct, &QAction::triggered, this, &MainWindow::record);

    playbackAct = new QAction(QIcon::fromTheme("folder-alt"),
                           tr("&Playback"), this);
    playbackAct->setShortcuts(QKeySequence::Print);
    playbackAct->setStatusTip(tr("Playback ..."));
    connect(playbackAct, &QAction::triggered, this, &MainWindow::playback );

    importRecordFileAct = new QAction(QIcon::fromTheme("folder-alt"),
                          tr("&Import Record File"), this);
    importRecordFileAct->setShortcuts(QKeySequence::Undo);
    importRecordFileAct->setStatusTip(tr("Import Record File"));
    connect(importRecordFileAct, &QAction::triggered, this, &MainWindow::importRecordFile);

    exportRecordFileAct = new QAction(QIcon::fromTheme("folder-alt"),
                                      tr("&Export Record File"), this);
    exportRecordFileAct->setShortcuts(QKeySequence::Undo);
    exportRecordFileAct->setStatusTip(tr("Export Record File..."));
    connect(exportRecordFileAct, &QAction::triggered, this, &MainWindow::exportRecordFile);


    arinc429DatabaseAct = new QAction(QIcon::fromTheme("folder-alt"),
                                      tr("&Arinc429 Database"), this);
    arinc429DatabaseAct->setShortcuts(QKeySequence::Undo);
    arinc429DatabaseAct->setStatusTip(tr("Arinc429 Database ..."));
    connect(arinc429DatabaseAct, &QAction::triggered, this, &MainWindow::arinc429Database );


    receiverSetupAct = new QAction(QIcon::fromTheme("folder-alt"),
                                      tr("&Receiver Setup"), this);
    receiverSetupAct->setShortcuts(QKeySequence::Undo);
    receiverSetupAct->setStatusTip(tr("Receiver Setup ..."));
    connect(receiverSetupAct, &QAction::triggered, this, &MainWindow::receiverSetup  );

    transmitterSetupAct = new QAction(QIcon::fromTheme("folder-alt"),
                                   tr("&Transmitter Setup"), this);
    transmitterSetupAct->setShortcuts(QKeySequence::Undo);
    transmitterSetupAct->setStatusTip(tr("Transmitter Setup ..."));
    connect(transmitterSetupAct, &QAction::triggered, this, &MainWindow::transmitterSetup  );

    aboutAct = new QAction(QIcon::fromTheme("folder-alt"),
                           tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    //
    serialBusSetupAct = new QAction(QIcon::fromTheme("folder-alt"),
                                    tr("&Serial Bus Setup"), this);
    serialBusSetupAct->setShortcuts(QKeySequence::Undo);
    serialBusSetupAct->setStatusTip(tr("Serial Bus Setup ..."));
    connect(serialBusSetupAct, &QAction::triggered, this, &MainWindow::serialBusSetup  );

}

//! [8]
void MainWindow::createMenus()
{
//
    transmitterMenu = menuBar()->addMenu(tr("&Transmitter"));
    transmitterMenu->addAction(transmitter0Act);
    transmitterMenu->addAction(transmitter1Act);


//
    receiverMenu = menuBar()->addMenu(tr("&Receiver"));
    receiverMenu->addAction(receiver0Act);
    receiverMenu->addAction(receiver1Act);
    receiverMenu->addAction(receiver2Act);
    receiverMenu->addAction(receiver3Act);

//
    recorderMenu = menuBar()->addMenu(tr("&Recorder"));
    recorderMenu->addAction(recordAct);
    recorderMenu->addAction(playbackAct);
    recorderMenu->addSeparator(); //
    recorderMenu->addAction(exportRecordFileAct);
    recorderMenu->addAction(importRecordFileAct);
//
    acquisitionMenu = menuBar()->addMenu(tr("&Acquisition"));

//
    setupMenu = menuBar()->addMenu(tr("&Setup"));
    setupMenu->addAction(arinc429DatabaseAct);
    setupMenu->addAction(receiverSetupAct);
    setupMenu->addAction(transmitterSetupAct);
    setupMenu->addAction(serialBusSetupAct);

//
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);

}
//! [12]
