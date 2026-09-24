
#ifndef ThreadedBuffer_H
#define ThreadedBuffer_H

#include "Types.h"
#include <QObject>
#include <QThread>
#include <QtConcurrent/QtConcurrent>

class ThreadedBuffer: public QObject
{
    Q_OBJECT
    using ring_buffer_t = SpscRingBuffer<BaseEventPtr,MAX_BUFFER_SIZE>;
    public:
        virtual ~ThreadedBuffer();
        
    public slots:
        virtual void run() = 0;

    protected:
        QThread* mainThread = nullptr;
        std::unique_ptr<ring_buffer_t> ring_buffer;
};

#endif // TRANSMITTERWORKER_H
