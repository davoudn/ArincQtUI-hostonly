#include "types.h"
#include "DEI1016.h"
#include "DEI1016RasberryConfigurations.h"
#include "settingsdialog.h"
#include "actionsrecord.h"
#include "action.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <tuple>
//

DEI1016::DEI1016(QObject* parent )
{
    this->moveToThread(&mainThread);
    mainThread.start();

    serial.moveToThread(&mainThread);
    openSerialPort();

    serialResetTimer.start(SERIAL_RESET_INTERVAL);
    connect(&serial, &QSerialPort::readyRead, this, &DEI1016::dataReceived, Qt::DirectConnection);
   // connect(&serialResetTimer, &QTimer::timeout, this, &DEI1016::serialReset);

}
DEI1016::~DEI1016(){
    auto x = ResetBoard().toPacket();
    char b[FRAME_POCKET_SIZE];
    for (int i=0; i < x.size(); i++){
        b[i] = x[i];
    }
    sendData(b);
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


bool DEI1016::sendData(char* ac)
{
        int numSent = serial.write(ac, TX_BUFFER_SIZE);
        if (numSent==TX_BUFFER_SIZE){
            qInfo() << "Num bytes sent: "<<numSent;
            return true;
        }
        else {
            qInfo() << "Failed to send all data!";
            return false;
        }
}


void log (QByteArray& data, str_t msg)
{
    if (data.size()== RX_BUFFER_SIZE)
    {
        qInfo() << msg;
        qInfo() << "\t"<<static_cast<uint8_t>(data[0]) <<"\t" << static_cast<uint8_t>(data[1])
                << "\t"<<static_cast<uint8_t>(data[2]) <<"\t" << static_cast<uint8_t>(data[3])
                << "\t"<<static_cast<uint8_t>(data[4]) <<"\t" << static_cast<uint8_t>(data[5])
                << "\t"<<static_cast<uint8_t>(data[6]) <<"\t" << static_cast<uint8_t>(data[7])
                << "\t"<<static_cast<uint8_t>(data[8]) << "\t"<< static_cast<uint8_t>(data[9]);
    }
}

/*
  @Receive
*/
void DEI1016::dataReceived()
{
    recDataBuffer.append(serial.readAll());
    updateTask();
}

void DEI1016::serialReset()
{
    serial.reset();
    qInfo() << " DEI1016::serialReset(), data were not visible for 100 milliseconds, reseting the port ...";
}

std::tuple<bool,uint32_t, uint32_t>& DEI1016::parse(QByteArray& ba)
{
    bool ifInitfound = false;
    bool ifFinalFound = false;
    std::get<0>(parseResult) = false;
    std::get<1>(parseResult) = -1;
    std::get<1>(parseResult) = -1;

    for(int i=0;i<ba.size();i++){
        if ( static_cast<uint8_t>(ba[i]) == INITIAL_BYTE && !ifInitfound ){
            std::get<1>(parseResult) = i;
            ifInitfound = true;
        }
        if ( static_cast<uint8_t>(ba[i]) == FINAL_BYTE && ifInitfound ){
            std::get<2>(parseResult) = i;
            ifFinalFound = true;
        }

    }
    if (ifInitfound && ifFinalFound)
    {
        if (std::get<2>(parseResult)- std::get<1>(parseResult) == DATA_POCKET_SIZE+1 ){
            std::get<0>(parseResult) = true;
        }
    }
    return parseResult;
}

void DEI1016::updateTask()
{
        parse(recDataBuffer);
        if(std::get<0>(parseResult))
        {
            bIfUpdated = true;
                    int initbyteidx= std::get<1>(parseResult);
                    for (int i=0;i < FRAME_POCKET_SIZE; i++){
                        recData[i] = static_cast<uint8_t>(recDataBuffer[initbyteidx+i]);
                    }
                    auto r =  ReceiverRecords::getInstance()->record(recData);
                    recDataBuffer.clear(); //remove(0,FRAME_POCKET_SIZE+initbyteidx);
        }
}


/*
  @Serial port
*/
//! [4]
void DEI1016::openSerialPort()
{
    const SettingsDialog::Settings p = SettingsDialog::getInstance()->settings();
     serial.setPortName(p.name);
     serial.setBaudRate(p.baudRate);
     serial.setDataBits(p.dataBits);
     serial.setParity(p.parity);
     serial.setStopBits(p.stopBits);
     serial.setFlowControl(p.flowControl);
     if (serial.open(QIODevice::ReadWrite) ){
         bIfSerialOpen = true;
         qInfo() << "Serial port is connected...";
     }
     serial.setReadBufferSize(1);
}
//! [4]

//! [5]
void DEI1016::closeSerialPort()
{
    if (serial.isOpen()){
        serial.close();
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
        break;
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
    if (chanell==CHANELL0)
    {
        switch(index)
        {
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
        switch(ifEnable)
        {
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
    if (chanell==CHANELL1)
    {
        switch(index)
        {
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
        switch(ifEnable)
        {
            case SET_ENABLE :{
            CONTROL::SDI_ENB2::ENABLE(control_word);
            break;
            }
            case SET_DISABLE: {
            CONTROL::SDI_ENB2::DISABLE(control_word);
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

DEI1016* DEI1016::getInstance(QObject* parent)
{
    if (!instance){
        instance = new DEI1016(parent);
    }
    return instance;
}




