#ifndef LINUXSERIAL_H
#define LINUXSERIAL_H

#include <QObject>
#include <QByteArray>
#include <tuple>
#include <atomic>

class LinuxSerial : public QObject
{
    Q_OBJECT
    LinuxSerial();
    static LinuxSerial* instance;
public:
    static LinuxSerial* getInstance();

    void start();
    void stop();
    bool openPort(const QString& device, int baudrate);
    void closePort();

signals:
    void packetReceived(QByteArray packet);
    void errorOccurred(QString message);

private:
    void run();
    void configurePort(int baudrate);
    bool validateCRC(const QByteArray& packet);
    std::tuple<bool,uint32_t, uint32_t>& parse(QByteArray& ba);

    std::tuple<bool,uint32_t, uint32_t> parseResult;
    QByteArray recDataBuffer;

    int fd;
    std::atomic<bool> running;
};


#endif // LINUXSERIAL_H
