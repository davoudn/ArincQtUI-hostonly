#ifndef TRANSMITTERWORKER_H
#define TRANSMITTERWORKER_H

#include "Types.h"
#include "Utils/ThreadedBuffer.h"

#include <QObject>
#include <QThread>
#include <QtConcurrent/QtConcurrent>

class TransmitterEventHandler: public ThreadedBuffer,
    public SingletonArray<TransmitterEventHandler,2>
{
    Q_OBJECT
    using ring_buffer_t = SpscRingBuffer<BaseEventPtr,MAX_BUFFER_SIZE>;
    public:
         ~TransmitterEventHandler();
        void run() override;
    public slots:
        void handle();

    private:
        QThread* mainThread = nullptr;
        std::unique_ptr<ring_buffer_t> ring_buffer;
};

#endif // TRANSMITTERWORKER_H
