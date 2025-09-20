#include "types.h"
#include "DEI1016.h"
#include "DEI1016RasberryConfigurations.h"
#include "receiverworker.h"
#include "settingsdialog.h"
#include "bitutils.h"
#include <QSerialPort>
#include <QSerialPortInfo>

//
DEI1016::DEI1016()
{
    serial = new QSerialPort(this);

  //  mainThread = new QThread();
  //  connect(mainThread, &QThread::started, this, &DEI1016::updateTask);
   // this->moveToThread(mainThread);
  //  mainThread->start();
    connect(serial, &QSerialPort::readyRead, this, &DEI1016::dataReceived);

}


/*     
  @Control instructions  
*/

void DEI1016::setControlInstruction(uint8_t instruction)
{
    if (instruction == PREDEFINED::SLOW_32BIT_SELFTEST) {
        word_t control_word("1111111111111111");;
        CONTROL::SELF_TEST::ENABLE(control_word);
        CONTROL::WORD_LENGTH::SELECT_32(control_word) ;
        CONTROL::TRANSMITTER_DATA_RATE::SELECT_LOW(control_word);
        CONTROL::RECEIVER_DATA_RATE::SELECT_LOW(control_word);
    }
    if (instruction == PREDEFINED::FAST_32BIT_SELFTEST) {
        word_t control_word("1111111111111111");;
        CONTROL::SELF_TEST::ENABLE(control_word);
        CONTROL::WORD_LENGTH::SELECT_32(control_word) ;
        CONTROL::TRANSMITTER_DATA_RATE::SELECT_HI(control_word);
        CONTROL::RECEIVER_DATA_RATE::SELECT_HI(control_word);
    }

    if (instruction == PREDEFINED::SLOW_32BIT_NORMAL_OPERATION) {
        word_t control_word("1111111111111111");;
        CONTROL::SELF_TEST::DISABLE(control_word);
        CONTROL::PAREN::ENABLE(control_word);
        CONTROL::WORD_LENGTH::SELECT_32(control_word) ;
        CONTROL::TRANSMITTER_DATA_RATE::SELECT_LOW(control_word);
        CONTROL::RECEIVER_DATA_RATE::SELECT_LOW(control_word);
    }

    if (instruction == PREDEFINED::FAST_32BIT_NORMAL_OPERATION) {
        word_t control_word("1111111111111111");;
        CONTROL::SELF_TEST::DISABLE(control_word);
        CONTROL::PAREN::ENABLE(control_word);
        CONTROL::WORD_LENGTH::SELECT_32(control_word) ;
        CONTROL::TRANSMITTER_DATA_RATE::SELECT_HI(control_word);
        CONTROL::RECEIVER_DATA_RATE::SELECT_HI(control_word);
    }

    if (instruction == PREDEFINED::ALLONES) {
        word_t control_word("1111111111111111");        
    }
}


/*
  @Send

bool DEI1016::sendData(uint8_t chanel, float rate, dword_t& arincData)
{

    AUX::convertDataToBytes(chanel, rate, arincData, transData);
    int numSent = serial->write(transData, TRANSMMIT_PACKET_SIZE);
    if (numSent==TRANSMMIT_PACKET_SIZE){
        return true;
    }
    return false;
}
*/
bool DEI1016::sendData(action& ac)
{
    int numSent = serial->write(ac.toPacket(), TRANSMMIT_PACKET_SIZE);
    if (numSent==TRANSMMIT_PACKET_SIZE){
        ac.bIfApplied = true;
        qInfo() << "Num bytes sent: "<<numSent;
        return true;
    }
    else {
        qInfo() << "Failed to send all data!";
        return false;
    }
}

/*
  @Receive
*/
void DEI1016::dataReceived()
{
    qint64 numReadTotal = 0;

    for (;;)
    {
        const qint64 numRead  = serial->read(recData, RECEIVE_PACKET_SIZE);
        numReadTotal += numRead;
        if (numReadTotal == RECEIVE_PACKET_SIZE){
            bIfDataReceived = true;
            updateTask();
            return ;
        }
        if (numReadTotal == 0){
            bIfDataReceived = false;

            return ;
        }
    }
}

void DEI1016::updateTask()
{
    uint8_t  chanell = 0;
    float    rate = 0.f;
    dword_t  arincData;
//    while(1)
    {
        if (bIfDataReceived){
           // qInfo() << "DEI1016::dataReceived() ... ";
           // qInfo() << recData ;

            bIfDataReceived = false;
            AUX::convertBytesToData(recData, chanell, rate, arincData);
            AUX::convertFromDEIToArinc(arincData);
            ReceiverWorker::getInstance(chanell)->update(rate, arincData);
           // qInfo() << chanell<<"\t" << arincData.to_string() << "\t" << rate;
        }
    }
}
/*
  @Serial port
*/
//! [4]
void DEI1016::openSerialPort()
{
    const SettingsDialog::Settings p = SettingsDialog::getInstance()->settings();
     serial->setPortName(p.name);
     serial->setBaudRate(p.baudRate);
     serial->setDataBits(p.dataBits);
     serial->setParity(p.parity);
     serial->setStopBits(p.stopBits);
     serial->setFlowControl(p.flowControl);
     if (serial->open(QIODevice::ReadWrite) ){
         bIfSerialOpen = true;
         qInfo() << "Serial port is connected...";
     }
}
//! [4]

//! [5]
void DEI1016::closeSerialPort()
{
    if (serial->isOpen()){
        serial->close();
    }

}
//! [5]


/*
 Defining a global  datasource and to assign a callback from igpio library.
*/
DEI1016* DEI1016::instance = nullptr;

DEI1016* DEI1016::getInstance()
{
    if (!instance){
        instance = new DEI1016();
    }
    return instance;
}




