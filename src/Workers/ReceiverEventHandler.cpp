#include "ReceiverEventHandler.h"
#include "ArincData.h"
#include "Label.h"
#include "arinc_functions.h"
#include "BaseItem.h"
#include "DEI1016.h"
#include "DEI1016RasberryConfigurations.h"
#include "BitUtils.h"

#include <chrono>

ReceiverWorker::ReceiverWorker()
{
    mainThread.setObjectName("Main receiver thread...");  
    connect(&mainThread, &QThread::started, this, &ReceiverWorker::handle);
    this->moveToThread(&mainThread);
}
/*
 * SLOTS ....
 */

void ReceiverWorker::start()
{
    mainThread.start();
}



void ReceiverWorker::handle(Event* event)
{
    qInfo() <<"ReceiverWorker::run() : started on chanel: "
            << chanell << ", running thread: " 
            << QThread::currentThread() <<  "\n";
    dword_t arincBits;
    DArincData arincData;
    uint8_t de=0;
    uint8_t ch = 0;
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        AUX::convertBytesToData(d.second, de, ch, rate, arincBits);
        AUX::convertFromDEIToArinc(arincBits);
        arincData.Init(arincBits);
        str_t labelid = arincData.template Get<LabelIdOctal>().toString();
        value_t value = arincData.template Get<DataBits>();
        auto res = QtConcurrent::run([=]()
        {
            emit setLabelData(de,ch,labelid, rate, value);
        });
       
       if (bIfToClean)
        {
           auto res = QtConcurrent::run([=]()
           {
                emit idleLabelCleaner();
           });
            bIfToClean = false;
        }
        if (bIfEvalDatarates)
        {
            auto res = QtConcurrent::run([=]()
            {
                emit evalDataRates();
            });
            bIfEvalDatarates = false;
        }
    }
}

void ReceiverWorker::makeDeviceIndex()
{
    switch (chanell) {
    case 0:
        dei = 0;
        deiChanell = 0;
        break;
    case 1:
        dei = 0;
        deiChanell = 1;
        break;
    case 2:
        dei = 1;
        deiChanell = 0;
        break;
    case 3:
        dei = 1;
        deiChanell = 1;
        break;
    }
}

void ReceiverWorker::update(uint8_t& deiId, uint8_t& chanellId,
     float& _rate, std::bitset<ARINC32_SIZE>& arincBitsData)
{
    if (deiId==dei && chanellId==deiChanell) 
    {
        ArincData->UpdateData(arincBitsData);
        rate = _rate;
        bIfDataUpdated = true;
    }
}

void  ReceiverWorker::enableEvalrates()
{
    bIfEvalDatarates = true;
}
void  ReceiverWorker::enableIdleCleaner()
{
    bIfToClean = true;
}
