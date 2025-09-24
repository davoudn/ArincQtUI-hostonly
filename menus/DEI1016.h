#pragma once
#include "types.h"
#include "action.h"
#include <QMutex>
#include <QMutexLocker>
#include <QThread>


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
    void updateTask();

public:
   bool  bIfSerialOpen = false;
   bool  bIfDataReceived = false;

private slots:
   void dataReceived();
public slots:
   bool sendData(BaseAction* ac);

signals:
   void update(uint8_t& dei, uint8_t& chanell, float& rate, dword_t& arincData);

public:
   //
   void openSerialPort();
   void closeSerialPort();
protected:
    char recData  [RECEIVE_PACKET_SIZE];
    char transData[TRANSMMIT_PACKET_SIZE];
    QThread* mainThread = nullptr;
    QMutex mutex;
    std::array<word_t, NUM_DEI1016> controlWords;
    QSerialPort *serial = nullptr;
};

