#include "linuxserial.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <termios.h>
#include <QThread>

LinuxSerial::LinuxSerial()
{

}

LinuxSerial::LinuxSerial() : fd(-1), running(false) {
    openPort(device, baudrate);
}

LinuxSerial::~LinuxSerial() {
    stop();
    if (fd >= 0) close(fd);
}

void LinuxSerial::start() {
    running = true;
    QThread* thread = QThread::create([this]() { run(); });
    thread->start();
}

void LinuxSerial::stop() {
    running = false;
}

bool LinuxSerial::openPort(const QString& device, int baudrate) {
    fd = ::open(device.toStdString().c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        emit errorOccurred("Failed to open serial port");
        return false;
    }
    configurePort(baudrate);
    return true;
}

void LinuxSerial::configurePort(int baudrate) {
    struct termios2 tio;
    ioctl(fd, TCGETS2, &tio);
    tio.c_cflag &= ~CBAUD;
    tio.c_cflag |= BOTHER;
    tio.c_ispeed = baudrate;
    tio.c_ospeed = baudrate;
    tio.c_cflag &= ~(PARENB | CSTOPB | CRTSCTS);
    tio.c_cflag |= CS8 | CREAD | CLOCAL;
    tio.c_lflag = 0;
    tio.c_oflag = 0;
    tio.c_iflag = 0;
    tio.c_cc[VMIN] = 1;
    tio.c_cc[VTIME] = 0;
    ioctl(fd, TCSETS2, &tio);
}

std::tuple<bool,uint32_t, uint32_t>& LinuxSerial::parse(QByteArray& ba)
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

void LinuxSerial::run()
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


void LinuxSerial::runOld() {
    uint8_t byte;
    QByteArray buffer;
    enum State { WaitingForHeader, ReceivingPayload } state = WaitingForHeader;

    while (running) {
        int n = read(fd, &byte, 1);
        if (n <= 0) continue;

        switch (state) {
        case WaitingForHeader:
            if (byte == 0xAA) {
                buffer.clear();
                buffer.append(byte);
                state = ReceivingPayload;
            }
            break;
        case ReceivingPayload:
            buffer.append(byte);
            if (buffer.size() == 4) {
                if (validateCRC(buffer)) {
                    emit packetReceived(buffer);
                } else {
                    emit errorOccurred("CRC error");
                }
                state = WaitingForHeader;
            }
            break;
        }
    }
}

bool LinuxSerial::validateCRC(const QByteArray& packet) {
    uint8_t crc = 0;
    for (int i = 0; i < packet.size() - 1; ++i) crc ^= packet[i];
    return crc == static_cast<uint8_t>(packet.back());
}
