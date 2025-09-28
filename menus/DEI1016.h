#pragma once
#include "types.h"
#include "action.h"

#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <QTimer>
#include <tuple>

class QMutex;
class QSerialPort;

class DEI1016 : public QObject {
    Q_OBJECT
    explicit DEI1016(QObject* parent= nullptr);
    static DEI1016* instance;
public:
    static DEI1016* getInstance();
     ~DEI1016() = default;

    void setControlWord_receiver_32Bits(int chanell, int index, word_t& control_word) ;
    void setControlWord_transmitter_32Bits(int chanell, int index, word_t& control_word) ;
    const word_t& setControlWord_transmitter_32Bits(int dei, int index) ;
    const word_t& setControlWord_receiver_32Bits(int dei, int index);
    void select_enable_receiver_SDIChanell(int chanell, int ifEnable, int index, word_t& control_word);
    void setControlInstruction(uint8_t instruction);
    void createMaps();

public:
   bool  bIfSerialOpen = false;
   bool  bIfDataReceived = false;


private slots:
   void dataReceived();
public slots:
   bool sendAction(BaseAction* ac);
   bool sendData(char* ac);
   void updateTask();
   void serialReset();

public:
   //
   void openSerialPort();
   void closeSerialPort();
   void init();
   std::tuple<bool,uint32_t, uint32_t>& parse(QByteArray& ba);
protected:
    record_t recData;
    record_t transData;
    std::array<word_t, NUM_DEI1016> controlWords;
    std::tuple<bool,uint32_t, uint32_t> parseResult;
    bool bIfUpdated = false;
    QByteArray recDataBuffer;
    QMutex mutex;

    QThread* mainThread = nullptr;
    QSerialPort *serial = nullptr;
    QTimer serialResetTimer;
};

