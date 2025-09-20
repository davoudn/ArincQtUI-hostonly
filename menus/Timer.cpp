#include "Timer.h"
#include "Equipment.h"
#include "transmitterworker.h"

#include <thread>
#include <chrono>



 Timer::Timer(QObject* _parent)
 {
      if (_parent){
          transmitterWorker = dynamic_cast<TransmitterWorker*>(_parent);
      }
 }

 Timer::~Timer()
 {

 }
void Timer::setTimeout(int t)
{
    timeOut = t;
}

void Timer::reset()
{
    counter = 0;
}

float Timer::getTime()
{
       return counter * MIN_TICK;
}


void Timer::stop()
{
    bIfStartTask = false;
}

void Timer::run()
{
    counterTask();
}
void Timer::counterTask()
{
    qInfo() << "Timer::task() : Task started on " << QThread::currentThread();
   // emit onTimeout();

    while(bIfStartTask){
            std::this_thread::sleep_for(std::chrono::milliseconds (MIN_TICK));
            counter++;
            if (counter * MIN_TICK >= timeOut ){
                counter = 0;
             /*  if (transmitterWorker){
                    transmitterWorker->incrementLabelsDataRateCounter();
                }
*/
                emit onTimeout();
            }
    }

}



