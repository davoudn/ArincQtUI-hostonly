#ifndef RECEIVERINTERFACE_H
#define RECEIVERINTERFACE_H
#include <QObject>
class ReceiverWorker;

class ReceiverInterface: public QObject
{
    Q_OBJECT
public:

    explicit ReceiverInterface(QObject* _parent = nullptr);
public slots:
    void idleLabelCleaner();
    void evalDataRates();
    void receiveTask();

protected:
    ReceiverWorker* parent = nullptr;
};

#endif // RECEIVERINTERFACE_H
