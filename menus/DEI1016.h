#pragma once
#include "types.h"
#include "action.h"
#include <QMutex>
#include <QMutexLocker>
#include <QThread>


class QMutex;
class QSerialPort;

class DEI1016 : public QObject {
    DEI1016();
    static DEI1016* instance;
public:
 
    static DEI1016* getInstance();
   ~DEI1016() = default;

    void setControlWord_receiver_32Bits(int chanell, int index, word_t& control_word) ;
    void setControlWord_transmitter_32Bits(int chanell, int index, word_t& control_word) ;
    void select_enable_receiver_SDIChanell(int chanell, int ifEnable, int index, word_t& control_word);
    void setControlInstruction(uint8_t instruction);
    void createMaps();

private slots:
    void dataReceived();

   public:
   bool  bIfSerialOpen = false;
   bool  bIfDataReceived = false;

public slots:
   void updateTask();
   bool sendData(action& ac);


   //
   void openSerialPort();
   void closeSerialPort();
protected:
    char recData  [RECEIVE_PACKET_SIZE];
    char transData[TRANSMMIT_PACKET_SIZE];
    QThread* mainThread = nullptr;
    QMutex mutex;

    QSerialPort *serial = nullptr;
};

