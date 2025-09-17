#ifndef TYPES_H
#define TYPES_H
#include <QString>
#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <bitset>
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
#define CLEANING_CHECK_TIME 40// miliseconds
#define TIME_OUT_COUNT_VALUE 50 // times
#define MILLISECONDS_TO_SECONDS 1000
#define SECONDS_TO_MILLISECONDS 1000.0

#define DWORD_SIZE 32 // num bits
#define WORD_SIZE  16 // num bits

#define RECEIVE_PACKET_SIZE 8 // bytes
#define TRANSMMIT_PACKET_SIZE 8 // bytes

#define WAIT_FOR_SERIAL_READ_READY 1000

using value_t   = QVariant;
using jsonobj_t = QJsonObject;
using jsondoc_t = QJsonDocument;
using jsonarray_t = QJsonArray;
using str_t = QString;
using word_t    = std::bitset<WORDSIZE>;
using parameters_list_t = std::vector<str_t>;
using dword_t  = std::bitset<ARINC32_SIZE>;

//
enum class EquipmentRole{
    Receiver,
    Transmitter
};

//
enum PREDEFINED {
   SLOW_32BIT_SELFTEST,
   FAST_32BIT_SELFTEST,
   SLOW_32BIT_NORMAL_OPERATION,
   FAST_32BIT_NORMAL_OPERATION,
   ALLONES
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

#define WINDOWS
//#define RASBERRY
//

//


#endif // TYPES_H
