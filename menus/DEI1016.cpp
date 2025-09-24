#include "types.h"
#include "DEI1016.h"
#include "DEI1016RasberryConfigurations.h"
#include "receiverworker.h"
#include "settingsdialog.h"
#include "bitutils.h"
#include "action.h"
#include <QSerialPort>
#include <QSerialPortInfo>

//

DEI1016::DEI1016(QObject* parent )
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

bool DEI1016::sendData(BaseAction* ac)
{
    if (ac){
        int numSent = serial->write(ac->toPacket(), TRANSMMIT_PACKET_SIZE);
        if (numSent==TRANSMMIT_PACKET_SIZE){
            ac->bIfApplied = true;
            qInfo() << "Num bytes sent: "<<numSent;
            return true;
        }
        else {
            qInfo() << "Failed to send all data!";
            return false;
        }
    }
    return false;
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
    uint8_t  dei = 0;
    float    rate = 0.f;
    dword_t  arincData;

            AUX::convertBytesToData(recData, dei, chanell, rate, arincData);
            AUX::convertFromDEIToArinc(arincData);
            emit update(dei, chanell, rate, arincData);
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


void DEI1016::setControlWord_receiver_32Bits(int receiveChanell, int index, word_t& control_word){


    CONTROL::WORD_LENGTH::SELECT_32(control_word) ;
    //
    switch(index){
    case PREDEFINED_RECEIVER::SLOW_SDI_DISABLED :{
        CONTROL::RECEIVER_DATA_RATE::SELECT_LOW(control_word);
        select_enable_receiver_SDIChanell(receiveChanell, SET_DISABLE, 0, control_word);
    }
    case PREDEFINED_RECEIVER::FAST_SDI_DISABLED :{
        CONTROL::RECEIVER_DATA_RATE::SELECT_HI(control_word);
        select_enable_receiver_SDIChanell(receiveChanell, SET_DISABLE, 0, control_word);
        break;
    }
    case PREDEFINED_RECEIVER::SLOW_SDI0  :{
        CONTROL::RECEIVER_DATA_RATE::SELECT_LOW(control_word);
        select_enable_receiver_SDIChanell(receiveChanell, SET_ENABLE, SDI0, control_word);
        break;
    }
    case PREDEFINED_RECEIVER::SLOW_SDI1  :{
        CONTROL::RECEIVER_DATA_RATE::SELECT_LOW(control_word);
        select_enable_receiver_SDIChanell(receiveChanell, SET_ENABLE, SDI1, control_word);
        break;
    }
    case PREDEFINED_RECEIVER::SLOW_SDI2  :{
        CONTROL::RECEIVER_DATA_RATE::SELECT_LOW(control_word);
        select_enable_receiver_SDIChanell(receiveChanell, SET_ENABLE, SDI2, control_word);
        break;
    }
    case PREDEFINED_RECEIVER::SLOW_SDI3  :{
        CONTROL::RECEIVER_DATA_RATE::SELECT_LOW(control_word);
        select_enable_receiver_SDIChanell(receiveChanell, SET_ENABLE, SDI3, control_word);
        break;
    }
    case PREDEFINED_RECEIVER::FAST_SDI0  :{
        CONTROL::RECEIVER_DATA_RATE::SELECT_HI(control_word);
        select_enable_receiver_SDIChanell(receiveChanell, SET_ENABLE, SDI0, control_word);
        break;
    }
    case PREDEFINED_RECEIVER::FAST_SDI1  :{
        CONTROL::RECEIVER_DATA_RATE::SELECT_HI(control_word);
        select_enable_receiver_SDIChanell(receiveChanell, SET_ENABLE, SDI1, control_word);
        break;
    }
    case PREDEFINED_RECEIVER::FAST_SDI2  :{
        CONTROL::RECEIVER_DATA_RATE::SELECT_HI(control_word);
        select_enable_receiver_SDIChanell(receiveChanell, SET_ENABLE, SDI2, control_word);
        break;
    }
    case PREDEFINED_RECEIVER::FAST_SDI3  :{
        CONTROL::RECEIVER_DATA_RATE::SELECT_HI(control_word);
        select_enable_receiver_SDIChanell(receiveChanell, SET_ENABLE, SDI3, control_word);
        break;
    }

    } // switch block
}



void DEI1016::select_enable_receiver_SDIChanell(int chanell, int ifEnable, int index, word_t& control_word){
    if (chanell==CHANELL0){
        switch(index){
        case 0:{
            CONTROL::SDI_X1::DISABLE(control_word);
            CONTROL::SDI_Y1::DISABLE(control_word);
            break;
        }
        case 1:{
            CONTROL::SDI_X1::ENABLE(control_word);
            CONTROL::SDI_Y1::DISABLE(control_word);
            break;
        }
        case 2:{
            CONTROL::SDI_X1::DISABLE(control_word);
            CONTROL::SDI_Y1::ENABLE(control_word);
            break;
        }
        case 3:{
            CONTROL::SDI_X1::ENABLE(control_word);
            CONTROL::SDI_Y1::ENABLE(control_word);
            break;
        }
        }
        switch(ifEnable){
        case SET_ENABLE :{
            CONTROL::SDI_ENB1::ENABLE(control_word);
            break;
        }
        case SET_DISABLE: {
            CONTROL::SDI_ENB1::DISABLE(control_word);
            break;
        }
        }
    }
    if (chanell==CHANELL1){
        switch(index){
        case 0:{
            CONTROL::SDI_X2::DISABLE(control_word);
            CONTROL::SDI_Y2::DISABLE(control_word);
            break;
        }
        case 1:{
            CONTROL::SDI_X2::ENABLE(control_word);
            CONTROL::SDI_Y2::DISABLE(control_word);
            break;
        }
        case 2:{
            CONTROL::SDI_X2::DISABLE(control_word);
            CONTROL::SDI_Y2::ENABLE(control_word);
            break;
        }
        case 3:{
            CONTROL::SDI_X2::ENABLE(control_word);
            CONTROL::SDI_Y2::ENABLE(control_word);
            break;
        }
        }
        switch(ifEnable){
        case SET_ENABLE :{
            CONTROL::SDI_ENB1::ENABLE(control_word);
            break;
        }
        case SET_DISABLE: {
            CONTROL::SDI_ENB1::DISABLE(control_word);
            break;
        }
        }
    }
}
//

void DEI1016::setControlWord_transmitter_32Bits(int transmitChanell, int index, word_t& control_word){

    CONTROL::WORD_LENGTH::SELECT_32(control_word) ;
    CONTROL::PAREN::ENABLE(control_word);
    //
    switch(index){
    case PREDEFINED_TRANSMITTER::SLOW_ODD_PARITY :{
        CONTROL::TRANSMITTER_DATA_RATE::SELECT_LOW(control_word);
        CONTROL::PARCK::SET_ODD(control_word);
        break;
    }
    case PREDEFINED_TRANSMITTER::FAST_ODD_PARITY :{
        CONTROL::TRANSMITTER_DATA_RATE::SELECT_HI(control_word);
        CONTROL::PARCK::SET_ODD(control_word);
        break;
    }
    case PREDEFINED_TRANSMITTER::SLOW_EVEN_PARITY  :{
        CONTROL::TRANSMITTER_DATA_RATE::SELECT_LOW(control_word);
        CONTROL::PARCK::SET_EVEN(control_word);

        break;
    }
    case PREDEFINED_TRANSMITTER::FAST_EVEN_PARITY  :{
        CONTROL::TRANSMITTER_DATA_RATE::SELECT_HI(control_word);
        CONTROL::PARCK::SET_EVEN(control_word);
        break;
    }
    } // switch block
}

const word_t& DEI1016::setControlWord_transmitter_32Bits(int dei, int index)
{
    setControlWord_transmitter_32Bits(dei,  index, controlWords[dei]);
    return controlWords[dei];
}

const word_t& DEI1016::setControlWord_receiver_32Bits(int dei, int index)
{
    setControlWord_receiver_32Bits(dei,  index, controlWords[dei]);
    return controlWords[dei];
}


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




