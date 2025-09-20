#include "devicedelegate.h"

template<>
DeviceDelegate<DReceiver, DEVICEONE_CHANELLONE>* DeviceDelegate<DReceiver, DEVICEONE_CHANELLONE>::instance = nullptr;

template<>
DeviceDelegate<DReceiver, DEVICEONE_CHANELLTWO>* DeviceDelegate<DReceiver, DEVICEONE_CHANELLTWO>::instance = nullptr;

template<>
DeviceDelegate<DTransmitter, DEVICEONE_CHANELLONE>* DeviceDelegate<DTransmitter, DEVICEONE_CHANELLONE>::instance = nullptr;




