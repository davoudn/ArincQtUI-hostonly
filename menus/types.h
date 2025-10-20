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
#define CLEANING_CHECK_TIME 2000// miliseconds
#define DATA_RATE_EVAL_TIME 2000// miliseconds
#define NUM_DEI1016 2
#define LAYOUT_REFRESHER_INTERVAL 100 // milliseconds
#define SERIAL_RESET_INTERVAL 100 // milliseconds

// for timers
#define MAX_TIME 10000 // miliseconds
#define IDLE_THRESHOLD 2000 // miliseconds
#define MIN_TICK 1000 // microseconds
#define ACTION_CLEANER_TIME 2000
#define SET_ENABLE 1
#define SET_DISABLE 0

#define SDI0 0
#define SDI1 1
#define SDI2 2
#define SDI3 3

#define BAUD_RATE 921600

#define TRANSMITTER_INTERVAL_TIME 2 // MILLISECONDS
#define MILLISECONDS_TO_SECONDS 0.001
#define SECONDS_TO_MILLISECONDS 1000.0

#define DWORD_SIZE 32 // num bits
#define WORD_SIZE  16 // num bits

#define WAIT_FOR_SERIAL_READ_READY 1000


#define DATA_POCKET_SIZE 8
#define FRAME_POCKET_SIZE 10 // bytes
#define RX_BUFFER_SIZE 10 // bytes
#define TX_BUFFER_SIZE 10 // bytes

#define FINAL_BYTE_INDEX 9
#define ARINC_BYTE3 8
#define ARINC_BYTE2 7
#define ARINC_BYTE1 6
#define LABEL_BYTE 5
#define ARINC_BYTE0 5
#define CONTROL_BYTE1 4
#define CONTROL_BYTE0 3
#define RATE_BYTE 2
#define INSTRUCTION_BYTE 1
#define INITIAL_BYTE_INDEX 0
// actual frame bytes
#define INITIAL_BYTE 0xFF
#define FINAL_BYTE 0xFF
#define INITIAL_BYTE 0xFF


#define RECEIVER_RECORDE_SIZE 64

using value_t = QVariant;
using jsonobj_t = QJsonObject;
using jsondoc_t = QJsonDocument;
using jsonarray_t = QJsonArray;
using str_t = QString;
using word_t = std::bitset<WORDSIZE>;
using parameters_list_t = std::vector<str_t>;
using dword_t = std::bitset<ARINC32_SIZE>;
using record_t = std::array<uint8_t, FRAME_POCKET_SIZE>;
//
enum class EquipmentRole{
    Receiver,
    Transmitter
};


enum Instructions {
    ADD_LABEL_TO_TRANSMIT  ,
    REMOVE_LABEL_FROM_TRANSMIT,
    UPDATE_LABEL_DATA_FOR_TRANSMIT,
    APPLY_CONTROL_WORD,
    BURST_A_LABEL,
    UART_ECHO,
    CLEAR_CHANELL,
    CLEAR_ALL
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


#define RASBERRYPI
#define MR_Pin 2
#define MR_RESET_HOLD 10000000
#endif // TYPES_H
