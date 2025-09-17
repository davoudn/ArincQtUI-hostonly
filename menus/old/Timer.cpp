#include "Timer.h"
#include "Equipment.h"
#include "Label.h"




void Timer<Single>::SetTimeOut(int t) {
    TimeOut = t;
}
void Timer<Single>::Start() {
    Increment = [this]() {
        //
        while (1) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            this->Counter++;
            if (Counter > TimeOut) {
                Counter = 0;
                if (this->CallBack) {
                    this->CallBack();
                }
                // std::cout << "hello!\n";
            }
        }
    };

}

Timer<Multi>::Timer(Equipment* _equipment):equipment(_equipment){
}

void Timer<Multi>::Start() {

}

