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
//#include <termio.h>
#include <linux/serial.h>
#include "bitutils.h"
#include <QSerialPortInfo>
#include <tuple>
#include <QSerialPort>
#include <thread>
#include <chrono>
#include <poll.h>
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
/*
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    timeval  timeout = {0,1};
    int result = select(fd, &readfds, nullptr, nullptr, nullptr);
    std::this_thread::sleep_for(std::chrono::microseconds(50));

 */
void DEI1016::dataReceivedTask()
{
    uint8_t byte;
    uint32_t counter = 0;
    State state = State::WaitForInitial;
    std::array<uint8_t, 8> buffer;
    //
    //pollfd pfd;
   // pfd.fd = fd;
   // pfd.events = POLLIN;
   // write (fd, "PING\n", 5);
    while(true)
    {
   // int result = poll(&pfd, 1, 1000);
   // qInfo() << "Polling result : " << result;
   // if (result > 0 && (pfd.events & POLLIN))

      //  std::this_thread::sleep_for(std::chrono::microseconds(1));
        int n = ::read(fd, &byte, 1);
        if (n<=0){
            continue;
        }
    //    qInfo() << counter <<n << "\t" <<byte;

        switch (state)
        {
            case State::WaitForInitial :
            {
                if (byte==255 && counter==0) {
                    state = State::InitialReceived;
                }
                break;
            }
            //
            case State::InitialReceived:
            {
                if (byte==255 && counter==0) {
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
                if (counter == 8){
                    if (byte==255){
                        state = State::FinalReceived;
                    }
                    else {
                        state = State::WaitForInitial;
                    }
                    counter = 0;
                }
                else
                {
                    if (counter < 8) {
                        buffer[counter] = byte;
                        counter++;
                    }
                }

                break;
            }
            case State::FinalReceived:
            {
                state = State::WaitForInitial;
                recData[0] = 255;
                recData[FRAME_POCKET_SIZE-1] = 255;
                for (int i=1;i < FRAME_POCKET_SIZE - 1; i++){
                    recData[i] = buffer[i-1];
                    buffer[i-1] = 0;
                }
                AUX::log(recData, "updateRecordsTable");
                auto r =  ReceiverRecords::getInstance()->record(recData);
                break;
            }

        }
        // qInfo() << counter;
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
    QString  pname = "/dev/ttyAMA0"; // + p.name;
    fd = open(pname.toStdString().c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        qInfo() << "DEI1016::openPort() :: Failed to open serial port: " << pname;
        return false;
    }
    else {
        qInfo() << "Serial port : " << pname << "is open.";
    }
    configurePort(p.baudRate);
    return true;

}

void DEI1016::configurePort(int baudrate) {
    struct termios2 tio2;
       if (ioctl(fd, TCGETS2, &tio2) < 0) {
           qInfo() << "ioctl TCGETS2";
           close(fd);
           return;
       }

       // 2. Configure raw mode
       tio2.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                        | INLCR  | IGNCR  | ICRNL  | IXON);
       tio2.c_oflag &= ~OPOST;
       tio2.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
       tio2.c_cflag &= ~(CSIZE | PARENB | CSTOPB | CRTSCTS);
       tio2.c_cflag |= CS8 | CREAD | CLOCAL ;

       // 3. Set VMIN/VTIME for blocking read of at least 1 byte
       tio2.c_cc[VMIN]  = 1;
       tio2.c_cc[VTIME] = 0;

       // 4. Select custom baud rate
       //    BOTHER tells the driver to use c_ispeed / c_ospeed directly
       tio2.c_cflag |= BOTHER;
       tio2.c_ispeed = 921600;   // e.g. 250k baud
       tio2.c_ospeed = 921600;

       // 5. Apply settings
       if (ioctl(fd, TCSETS2, &tio2) < 0) {
           qInfo() <<  "ioctl TCSETS2";
           close(fd);
       }
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




