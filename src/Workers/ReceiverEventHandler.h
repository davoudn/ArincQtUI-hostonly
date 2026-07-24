#ifndef ReceiverEventHandler_H
#define ReceiverEventHandler_H
#include "Types.h"
#include "Utilts/SpscRingBuffer.hpp"
#include "Utils/Event.h"
#include "ThreadedBuffer.h"
#include <QThread>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>
#include  <memory>

class ReceiverEventHandler: public SingletonArray<ReceiverEventHandler, NUM_CHANNELS>, public ThreadedBuffer
{
    Q_OBJECT
private:
    ReceiverEventHandler();

public:
    void start();
    void push(BaseEventPtr event);
    void run() override;
    
public slots:
    void handle();    

};

#endif // ReceiverEventHandler_H



