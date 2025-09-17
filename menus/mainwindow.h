// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "types.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU
//! [0]

//! [1]
private slots:
    //
    void transmitter1();

    //
    void receiver0();
    void receiver1();
    void receiver2();
    void receiver3();

    //
    void record ();
    void playback ();
    void importRecordFile  ();
    void exportRecordFile  ();
    //
    void receiverSetup();
    void transmitterSetup();
    void arinc429Database();
    void serialBusSetup();
    //
    void about();
//! [1]

//! [2]
private:
    void createActions();
    void createMenus();
//! [2]

//! [3]
    QMenu *transmitterMenu;
    QMenu *receiverMenu;
    QMenu *recorderMenu;
    QMenu *graphMenu;
    QMenu *acquisitionMenu;
    QMenu *setupMenu;
    QMenu *specialMenu;
    QMenu *helpMenu;


    QActionGroup *alignmentGroup;

    // receiverMenu
    QAction *receiver0Act;
    QAction *receiver1Act;
    QAction *receiver2Act;
    QAction *receiver3Act;

    // transmitterMenu
    QAction *transmitter1Act;

    // recorderMenu
    QAction *recordAct;
    QAction *playbackAct;
    QAction *exportRecordFileAct;
    QAction *importRecordFileAct;

    // setupMenu
    QAction *arinc429DatabaseAct;
    QAction *receiverSetupAct;
    QAction *transmitterSetupAct;
    QAction *serialBusSetupAct;

    // helpMenu
    QAction *aboutAct;

    QLabel *infoLabel;

};
//! [3]

#endif
