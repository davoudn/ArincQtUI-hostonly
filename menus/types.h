#ifndef TYPES_H
#define TYPES_H
#include <QString>
#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <bitset>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QDebug>

#define ARINC32_SIZE 32
#define FOREWARD 0
#define BACKWARD 1
#define WORDSIZE 16
#define NUM_WORDS_PER_DATA 2
#define LABEL_SIZE 8
#define BCDBNR_MSB 29
#define SSM_LSB 30
#define SSM_MSB 31
#define SDI_LSB 9
#define SDI_MSB 10
#define ARINC32_DATA_LENGTH 24
#define CLEANING_CHECK_TIME 1500// miliseconds
#define DATA_RATE_EVAL_TIME 1000// miliseconds
#define NUM_DEI1016 2
// for timers
#define MAX_TIME 10000 // miliseconds
#define IDLE_THRESHOLD 2000 // miliseconds
#define MIN_TICK 1 // miliseconds
#define ACTION_CLEANER_TIME 2000
#define SET_ENABLE 1
#define SET_DISABLE 0

#define SDI0 0
#define SDI1 1
#define SDI2 2
#define SDI3 3


#define TRANSMITTER_INTERVAL_TIME 2 // MILLISECONDS
#define MILLISECONDS_TO_SECONDS 0.001
#define SECONDS_TO_MILLISECONDS 1000.0

#define DWORD_SIZE 32 // num bits
#define WORD_SIZE  16 // num bits

#define RECEIVE_PACKET_SIZE 8 // bytes
#define TRANSMMIT_PACKET_SIZE 8 // bytes

#define WAIT_FOR_SERIAL_READ_READY 1000

#define Byte0 0
#define Byte1 1
#define Byte2 2
#define Byte3 3
#define Byte4 4
#define Byte5 5
#define Byte6 6
#define Byte7 7
#define LABEL_BYTE 4
#define INSTRUCTION_BYTE 0
#define ARINC_BYTE0 4
#define ARINC_BYTE1 5
#define ARINC_BYTE2 6
#define ARINC_BYTE3 7
#define CONTROL_BYTE0 2
#define CONTROL_BYTE1 3
#define RATE_BYTE 1

using value_t = QVariant;
using jsonobj_t = QJsonObject;
using jsondoc_t = QJsonDocument;
using jsonarray_t = QJsonArray;
using str_t = QString;
using word_t = std::bitset<WORDSIZE>;
using parameters_list_t = std::vector<str_t>;
using dword_t = std::bitset<ARINC32_SIZE>;

//
enum class EquipmentRole{
    Receiver,
    Transmitter
};


enum Instructions {
    ADD_LABEL_TO_TRANSMIT  ,
    REMOVE_LABEL_FROM_TRANSMIT,
    UPDATE_LABEL_DATA_FOR_TRANSMIT,
    APPLY_CONTROL_WORD
};

enum PREDEFINED {
    SLOW_32BIT_SELFTEST,
    FAST_32BIT_SELFTEST,
    SLOW_32BIT_NORMAL_OPERATION,
    FAST_32BIT_NORMAL_OPERATION,
    ALLONES
};
//
enum PREDEFINED_RECEIVER {
    SLOW_SDI_DISABLED,
    FAST_SDI_DISABLED,
    SLOW_SDI0        ,
    SLOW_SDI1        ,
    SLOW_SDI2        ,
    SLOW_SDI3        ,
    FAST_SDI0        ,
    FAST_SDI1        ,
    FAST_SDI2        ,
    FAST_SDI3
};

enum PREDEFINED_TRANSMITTER {
    SLOW_ODD_PARITY ,
    FAST_ODD_PARITY ,
    SLOW_EVEN_PARITY,
    FAST_EVEN_PARITY
};

//
enum DArincRepType {
    RAW,
    HEX,
    ENGINEERING
};
//
enum  DArincParamType {
    BNR = 0,
    BCD,
    FLOATBCD,
    DISCRETE,
    FLNOBCD,
    ATCBCD,
    INT,
    UNKOWN
};

enum class LabelTypes {
    BNR,
    BCD  ,
    DISCRETE ,
    VHF,
    ATC,
    ADF,
    ILS,
    VOR,
    DME,
    HF,
    TIMEBCD,
    GAOMEGA,
    GACOVAR,
    GPSHV,
    GAMSGCHK,
    GMT,
    TCASR,
    TCASA,
    TCASB,
    HFAF,
    ALTVAR,
    DATEFL,
    FLNO,
    GAMSGLEN,
    ENGSN,
    UNKOWN
};

class DReceiver{};
class DTransmitter{};
class DReserved{};



#endif // TYPES_H

//
/*
enum PREDEFINED {
   SLOW_32BIT_SELFTEST,
   FAST_32BIT_SELFTEST,
   SLOW_32BIT_NORMAL_OPERATION,
   FAST_32BIT_NORMAL_OPERATION,
   ALLONES
};
*/
//
