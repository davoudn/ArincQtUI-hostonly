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
#include <thread>
#include <chrono>
#include <poll.h>

#ifdef RASBERRYPI
#include <pigpio.h>
#endif

//

DEI1016::DEI1016(QObject* parent )
{
    this->moveToThread(&mainThread);
    mainThread.start();
    connect(&mainThread, &QThread::started, this, &DEI1016::dataReceivedTask);
    openPort();
    start();
#ifdef RASBERRYPI
    gpioInitialise();
#endif
    resetBoard();
}

DEI1016::~DEI1016(){
 /*   auto x = ResetBoard().toPacket();
    char b[FRAME_POCKET_SIZE];
    for (int i=0; i < x.size(); i++){
        b[i] = x[i];
    }
    sendData(b);
*/
    resetBoard();
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


void DEI1016::dataReceivedTask()
{

    uint8_t bytes[FRAME_POCKET_SIZE];
    state = State::WaitForInitial;

    while(true)
    {

        int n = ::read(fd, &bytes, FRAME_POCKET_SIZE);
        if (n<=0){
            continue;
        }

        for (uint32_t i=0; i < n; i++){
            parse(bytes[i]);
        }
    }
}

void DEI1016::updateRecordsTable()
{


}


void DEI1016::parse(uint8_t byte)
{
    switch (state)
    {
        case State::WaitForInitial :
        {
            if (byte==255 && counter==1) {
                state = State::InitialReceived;
            }
            break;
        }
        //
        case State::InitialReceived:
        {
            if (byte==255 && counter==1) {
                state = State::InitialReceived;
            }
            else {
                state = State::WaitForFinal;
                recData[counter] = byte;
                counter++;
            }
            break;
        }
        //
        case State::WaitForFinal:
        {
            if (counter < 9) {
                recData[counter] = byte;
                counter++;
            }
            else
            {
                if (counter == 9)
                {
                    if (byte==255){
                        state = State::FinalReceived;
                    }
                    else {
                        state = State::WaitForInitial;
                    }
                    counter = 1;
                }
            }
            break;
        }
    }
    if ( state==State::FinalReceived)
    {
        state = State::WaitForInitial;
        recData[0] = 255;
        recData[FRAME_POCKET_SIZE-1] = 255;
        AUX::log(recData, "updateRecordsTable");
        auto r =  ReceiverRecords::getInstance()->record(recData);
    }
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
#ifdef ORANGEPI
    QString  pname = "/dev/ttyS1";
#endif
#ifdef RASBERRYPI
    QString  pname = "/dev/ttyAMA0";
#endif

    fd = open(pname.toStdString().c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) {
        qInfo() << "DEI1016::openPort() :: Failed to open serial port: " << pname;
        return false;
    }
    else {
        qInfo() << "Serial port : " << pname << "is open.";
    }
    configurePort(BAUD_RATE);
    return true;

}

void DEI1016::configurePort(int baudrate)
{
    fcntl(fd, F_SETFL, 0);

    struct termios2 tio2;
       if (ioctl(fd, TCGETS2, &tio2) < 0) {
           qInfo() << "ioctl TCGETS2";
           close(fd);
           return;
       }

       // Set raw mode
          tio2.c_iflag = 0;
          tio2.c_oflag = 0;
          tio2.c_cflag &= ~CBAUD;
          tio2.c_cflag |= BOTHER | CLOCAL | CREAD;
          tio2.c_lflag = 0;

          // Set custom baud rate
          tio2.c_ispeed = baudrate;
          tio2.c_ospeed = baudrate;
          tio2.c_cc[VMIN]  = 1;
          tio2.c_cc[VTIME] = 0;

       // 5. Apply settings
       if (ioctl(fd, TCSETS2, &tio2) < 0) {
           qInfo() <<  "ioctl TCSETS2";
           close(fd);
       }
       AUX::serialFlush(fd);
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
    switch(index)
    {
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


void DEI1016::resetBoard()
{
#ifdef RASBERRYPI
    gpioSetMode(MR_Pin, PI_OUTPUT);
   // gpioWrite(MR_Pin, 1);
    //
    gpioWrite(MR_Pin, 0);
    //for (int i=0; i < MR_RESET_HOLD; i++);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    gpioWrite(MR_Pin, 1);

#endif

#ifdef ORANGEPI
#endif

#ifdef UBUNTU
#endif

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




