#include "types.h"
#include "DEI1016.h"
#include "DEI1016RasberryConfigurations.h"
#include "settingsdialog.h"
#include "actionsrecord.h"
#include "action.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <asm-generic/termbits.h>
#include <linux/serial.h>
#include "bitutils.h"
#include <QSerialPortInfo>
#include <tuple>
#include <QSerialPort>
//

DEI1016::DEI1016(QObject* parent )
{
    this->moveToThread(&mainThread);
    mainThread.start();
    connect(&mainThread, &QThread::started, this, &DEI1016::dataReceivedTask);
    openPort();
    start();
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
        int numSent = write(fd, ac, TX_BUFFER_SIZE);
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
enum class State{
     WaitForInitial,
     InitialReceived,
     WaitForFinal,
     FinalReceived
};

void DEI1016::dataReceivedTask()
{
    uint8_t byte;
    uint32_t counter = 0;
    State state = State::WaitForInitial;
    std::array<uint8_t, 8> buffer;
    while(true)
    {

        int n = read(fd, &byte, 1);
        if (n<=0){
            //recDataBuffer.append(byte);
            //updateRecordsTable();
            continue;
        }
        qInfo() <<n << "\t" <<byte;

        switch (state)
        {
            case State::WaitForInitial :
            {
                if (byte==255) {
                    state = State::InitialReceived;
                }
                break;
            }
            //
            case State::InitialReceived:
            {
                if (byte==255) {
                    state = State::InitialReceived;
                }
                else {
                    state = State::WaitForFinal;
                    buffer[counter] = byte;
                    counter++;
                }
                break;
            }
            //
            case State::WaitForFinal:
            {
                if (counter < 8) {
                    buffer[counter] = byte;
                    counter++;
                }
                if (counter == 8){
                    if (byte==0xff){
                        state = State::FinalReceived;
                    }
                    else {
                        state = State::WaitForInitial;
                    }
                    counter = 0;
                }
                break;
            }

        }
        if (state == State::FinalReceived){
            state == State::WaitForInitial;
            recData[0] = 255;
            recData[FRAME_POCKET_SIZE-1] = 255;
            for (int i=1;i < FRAME_POCKET_SIZE - 1; i++){
                recData[i] = buffer[i-1];
            }
            AUX::log(recData, "updateRecordsTable");
            auto r =  ReceiverRecords::getInstance()->record(recData);
        }
    }
}

void DEI1016::updateRecordsTable()
{


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
        if (std::get<2>(parseResult)- std::get<1>(parseResult) == 1){
            ba.remove(0, std::get<1>(parseResult));
        }
        if (recDataBuffer.size()==10){
                //    log(recDataBuffer, "--");
                  //  recDataBuffer.clear();
        }

    }
    return parseResult;
}



/*
  @Serial port
*/
//! [4]

void DEI1016::closePort()
{
    stop();
    if (fd >= 0) close(fd);
}
//p.name.toStdString().c_str()
bool DEI1016::openPort()
{
    const SettingsDialog::Settings p = SettingsDialog::getInstance()->settings();
    QString  pname = "/dev/" + p.name;
    fd = open(pname.toStdString().c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        qInfo() << "DEI1016::openPort() :: Failed to open serial port: " << p.name;
        return false;
    }
    else {
        qInfo() << "Serial port : " << pname << "is open.";
    }
    configurePort(p.baudRate);
    return true;

}

void DEI1016::configurePort(int baudrate) {
    termios2 tio;
    ioctl(fd, TCGETS2, &tio);
    tio.c_cflag &= ~CBAUD;
    tio.c_cflag |= BOTHER;
    tio.c_ispeed = 115200;
    tio.c_ospeed = 115200;
    tio.c_cflag &= ~(PARENB | CSTOPB | CRTSCTS);
    tio.c_cflag |= CS8;// | CREAD | CLOCAL;
    tio.c_lflag = 0;
    tio.c_oflag = 0;
    tio.c_iflag = 0;
    tio.c_cc[VMIN] = 1;
    tio.c_cc[VTIME] = 1;
    ioctl(fd, TCSETS2, &tio);
}


//! [5]

void DEI1016::stop() {
    running = false;
}

void DEI1016::start() {
    running = true;
    mainThread.start();
}


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




