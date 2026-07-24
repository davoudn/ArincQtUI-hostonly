#include "Utils/Singleton.h"
#include "Utils/Event.h"
#include "Utils/SpscRingBuffer.hpp"
#include "Utils/BaseReactor.h"
#include "Utils/ThreadedBuffer.h"
#include <QThread>
#include <array>
#include <map>

class ReceiverDispatcher : public Singleton<ReceiverDispatcher>, ThreadedBuffer
{
    private:
        ReceiverDispatcher();
    public:
    // one reactor for each label
        using reactor_table_t  = std::array<std::map<uint64_t,Reactor>,   MAX_LABELS>;
    public:
        void push(BaseEventPtr event);
        void run() override;
        void addReactor(uint32_t channel, uint64_t reactor_index);
        void removeReactor(uint32_t channel, uint64_t reactor_index);
    public slots:
        void dispatch();
    private:
        std::array<reactor_table_t, NUM_REC_CHANNELS> reactor_table_array;
};