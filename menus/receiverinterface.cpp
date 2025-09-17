#include "receiverinterface.h"
#include "receiver.h"
#include "receiverworker.h"
#include "ArincData.h"

ReceiverInterface::ReceiverInterface(QObject* _parent)
{
  if (_parent){
      parent = dynamic_cast<ReceiverWorker*>(_parent);
  }

}

void ReceiverInterface::idleLabelCleaner(){
     Receiver::getInstance)->idleLabelCleaner();
}

void ReceiverInterface::evalDataRates(){
       Receiver::getInstance()->evalDataRates(DATA_RATE_EVAL_TIME);
}

void ReceiverInterface::receiveTask(){
    qInfo() <<"ReceiverWorker::run() : started .........\n";
    while(1) {
        if (parent->bIfDataUpdated) {
            //
            str_t labelid = parent->getArincData().template Get<LabelIdOctal>().toString();
            value_t value = parent->getArincData().template Get<DataBits>();
            float rate = parent->rate;
            //
            if ( Receiver::getInstance()->setLabelData(labelid, rate, value)){
                parent->bIfDataHandled = true;
                parent->bIfDataUpdated = false;
            }
        }

    }
}
