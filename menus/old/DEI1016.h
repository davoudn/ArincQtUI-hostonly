#pragma once
#include "types.h"
#include <QMutex>
#include <QMutexLocker>
#include <array>
#include <QThread>


class QMutex;
class QSerialPort;

class DEI1016 : public QThread {
    DEI1016();
    static DEI1016* instance;
public:
 
    static DEI1016* getInstance();
 //   using words_t = std::array <word_t, NUM_WORDS_PER_DATA>;

   ~DEI1016() = default;

    bool checkIfDataReceived();
    bool sendData(uint8_t chanel, float rate, dword_t& arincData) ;

    // Callback function for GPIO interrupts
   void setControlInstruction(uint8_t instruction);
   void createMaps();
   void run() override;


   //
   void openSerialPort();
   void closeSerialPort();
protected:
    char recData[RECEIVE_PACKET_SIZE];
    char transData[TRANSMMIT_PACKET_SIZE];

    QMutex mutex;

    QSerialPort *serial = nullptr;
};

