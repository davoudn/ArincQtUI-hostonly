#pragma once
#include "types.h"
#include "action.h"

#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <QSerialPort>
#include <QTimer>
#include <tuple>

class QMutex;
class QSerialPort;

enum class State{
     WaitForInitial,
     InitialReceived,
     WaitForFinal,
     FinalReceived
};

class DEI1016 : public QObject {
    Q_OBJECT
    explicit DEI1016(QObject* parent = nullptr);
    static DEI1016* instance;
public:
     static DEI1016* getInstance(QObject* parent = nullptr);
     ~DEI1016() ;

    void setControlWord_receiver_32Bits(int chanell, int index, word_t& control_word) ;
    void setControlWord_transmitter_32Bits(int chanell, int index, word_t& control_word) ;
    const word_t& setControlWord_transmitter_32Bits(int dei, int index) ;
    const word_t& setControlWord_receiver_32Bits(int dei, int index);
    void select_enable_receiver_SDIChanell(int chanell, int ifEnable, int index, word_t& control_word);
    void setControlInstruction(uint8_t instruction);
    void createMaps();
    void updateRecordsTable();
    void start();
    void stop();
    bool openPort();
    void closePort();
    void configurePort(int baudrate);
    void parse(uint8_t byte);


public:
   bool  bIfSerialOpen = false;
   bool  bIfDataReceived = false;


public slots:
   bool sendData(char* ac);
   void dataReceivedTask();

protected:
    bool running = false;
    record_t recData;
    record_t transData;
    std::array<word_t, NUM_DEI1016> controlWords;
    std::tuple<bool,uint32_t, uint32_t> parseResult;
    std::array<uint8_t, RX_BUFFER_SIZE * 2> recDataBuffer;
    // QByteArray recDataBuffer;
    int fd = -1;
    QThread mainThread ;
    QTimer serialResetTimer;
    State state;
    int counter = 1;
};

