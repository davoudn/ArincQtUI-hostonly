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
 //   using words_t = std::array <word_t, NUM_WORDS_PER_DATA>;

   ~DEI1016() = default;
  // bool sendData(uint8_t chanel, float rate, dword_t& arincData) ;

    // Callback function for GPIO interrupts
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

