#include "Utils/Event.h"

#include <exception>
uint64_t Reactor::reactor_index = 0;

class Reactor 
{
        static uint64_t reactor_index;
    public:
        Reactor(QObject* host)
        {
        }
        ~Reactor()
        {
        }
        void run(BaseEventPtr event)
        {
            host->run(event);
        }
};


