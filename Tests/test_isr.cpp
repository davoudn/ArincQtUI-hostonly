#include "../DEI1016RasberryConfigurations.h"
#include <chrono>
#include <iostream>
#include  <array>

#define SIZE 26

void InterruptCallback(int gpio, int level, unsigned int tick);
void SetGpioModes();
void SetDataModes(uint8_t mode);
void Init();

std::array<uint8_t, SIZE> Gpio_Word_Map   = std::array<uint8_t, SIZE>{ D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13 , D14, D15, DR1, DR2, TXR, SEL, OE1, OE2, LD1, LD2, ENTX,LDCW};

int main(){
    bool f = false;
    Init();
   // gpioWrite(OE1, SET);
   /// for (int i=0; i < SIZE; i++){
   //     gpioWrite(Gpio_Word_Map[i], UNSET);
   // }
  /*  while(1){
        for (int i=0; i < SIZE; i++){
            gpioWrite(Gpio_Word_Map[i], SET);
            std::this_thread::sleep_for(std::chrono::nanoseconds(100000));
        //    std::cout << "GPIO state: " << gpioRead(DR1) << "\n"; 
            gpioWrite(Gpio_Word_Map[i], UNSET);
        }
       // std::this_thread::sleep_for(std::chrono::nanoseconds(10));
    }
*/
return 0;
}

//
void SetDataModes(uint8_t mode) {
    //
    gpioSetMode(D0, mode);  gpioSetMode(D1, mode);
    gpioSetMode(D2, mode);  gpioSetMode(D3, mode);
    gpioSetMode(D4, mode);  gpioSetMode(D5, mode);
    gpioSetMode(D6, mode);  gpioSetMode(D7, mode);
    gpioSetMode(D8, mode);  gpioSetMode(D9, mode);
    gpioSetMode(D10, mode); gpioSetMode(D11, mode);
    gpioSetMode(D12, mode); gpioSetMode(D13, mode);
    gpioSetMode(D14, mode); gpioSetMode(D15, mode);
}

void SetGpioModes() {
    // input gpios
    gpioSetMode(DR1, PI_INPUT);
    gpioSetPullUpDown(DR1, PI_PUD_UP);

    gpioSetMode(DR2, PI_INPUT);
    gpioSetPullUpDown(DR2, PI_PUD_UP);

    gpioSetMode(TXR, PI_INPUT);
    gpioSetPullUpDown(TXR, PI_PUD_UP);

    // output pgios
    gpioSetMode(SEL, PI_OUTPUT);
    gpioSetMode(OE1, PI_OUTPUT);
    gpioSetMode(OE2, PI_OUTPUT);
    gpioSetMode(LD1, PI_OUTPUT);
    gpioSetMode(LD2, PI_OUTPUT);
    gpioSetMode(ENTX, PI_OUTPUT);
    gpioSetMode(LDCW, PI_OUTPUT);
}

void InterruptCallback(int gpio, int level, unsigned int tick){

    //
    gpioWrite(SEL, level);
  /*  if ( gpio == DR1 && PIN::_DR::READY(level) ) {
        std::cout << "State of DR: " << DR1 << "\n";
    }
        
    //
    if ( gpio == DR2 && PIN::_DR::READY(level) ) {
        std::cout << "State of DR: " << DR2 << "\n";       
    }    
  */      
}

//
void Init(){
    // Initialize pigpio
    if (gpioInitialise() < 0) {
        std::cerr << "Failed to initialize pigpio" << std::endl;
        exit(-1);
    }
    SetGpioModes();
    /*
    if (!gpioSetISRFunc(DR1, RISING_EDGE, 0, InterruptCallback) ){
        std::cout << "Failed to set ISR func for gpio: " << DR1 << "\n";
    }
    if (!gpioSetISRFunc(DR2, RISING_EDGE, 0, InterruptCallback)){
        std::cout << "Failed to set ISR func for gpio: " << DR2 << "\n";
    }
         */

        if (gpioSetAlertFunc(DR1, InterruptCallback)){
            std::cout << "Failed to set ISR func for gpio: " << DR1 << "\n";
            exit(0);
        }
    /*    if (gpioSetAlertFunc(DR2, InterruptCallback)){
            std::cout << "Failed to set ISR func for gpio: " << DR2 << "\n";
            exit(0);
        }
            */

}