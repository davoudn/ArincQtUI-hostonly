#include "types.h"
#include <bitset>

/*
Physical    Meaning
1           3v3 Power
3           GPIO 2 (I2C1 SDA)
5           GPIO 3 (I2C1 SCL)
7           GPIO 4 (GPCLK0)
9           Ground
11          GPIO 17
13          GPIO 27
15          GPIO 22
17          3v3 Power
19          GPIO 10 (SPI0 MOSI)
21          GPIO 9 (SPI0 MISO)
23          GPIO 11 (SPI0 SCLK)
25          Ground
27          GPIO 0 (EEPROM SDA)
29          GPIO 5
31          GPIO 6
33          GPIO 13 (PWM1)
35          GPIO 19 (PCM FS)
37          GPIO 26
39          Ground
2           5v Power
4           5v Power
6           Ground
8           GPIO 14 (UART TX)
10          GPIO 15 (UART RX)
12          GPIO 18 (PCM CLK)
14          Ground
16          GPIO 23
18          GPIO 24
20          Ground
22          GPIO 25
24          GPIO 8 (SPI0 CE0)
26          GPIO 7 (SPI0 CE1)
28          GPIO 1 (EEPROM SCL)
30          Ground
32          GPIO 12 (PWM0)
34          Ground
36          GPIO 16
38          GPIO 20 (PCM DIN)
40          GPIO 21 (PCM DOUT)

*/


/*
Table 4: Pin Definitions
 SYMBOL DEFINITION
 VDD Power Input.  +5VDC�10%
 GND Power Return and Signal Ground.
 DI1(A) ARINC 429 Input.  Receiver Channel 1, �A� input
 DI1(B) ARINC 429 Input.  Receiver Channel 1, �B� input
 DI2(A) ARINC 429 Input.  Receiver Channel 2, �A� input
 DI2(B) ARINC 429 Input.  Receiver Channel 2,�B� input
 /DR1 Logic Output. Data Ready, Receiver 1. A Low output indicates valid data in receiver 1.
 /DR2 Logic Output. Data Ready, Receiver 2.  A Low output  indicates valid data in receiver 2.
 SEL Logic Input.  Receiver word select.  A Low input selects receiver Word 1; Hi selects Word 2 to be read on D[15:0] port.
 /OE1 Logic Input.  Receiver 1 Output Enable.  A Low input  enables the  D[15:0] port to output Receiver 1 data.   Word 1 or
 Word 2 will be output  as determined by the SEL input.
 /OE2 Logic Input.  Receiver 2 Output Enable.  A Low input  enables the  D[15:0] port to output Receiver 2 data.   Word 1 or
 Word 2 will be output  as determined by the SEL input.
 D[15:0] Logic Input / Tri-state Output.  This 16-bit bi-directional data port is the uP data interface.  Receiver data is read from
 this port. Control Register and Transmitter FIFO data is written into this port.
 /LD1 Logic Input.  Load Transmitter Word 1.  A Low input  pulse loads Word 1 into the Transmitter FIFO from D[15:0].
 /LD2 Logic Input.  Load Transmitter Word 2.  A Low input  pulse loads Word 2 into the Transmitter FIFO from D[15:0].
 TXR Logic Output.  Transmitter Ready.  A Hi output  indicates the Transmitter FIFO is empty and ready to accept new data.
 DO(A) Logic Output.  Transmitter serial data �A� output.  This is a  return-to-zero format signal which will normally feed an
 ARINC 429 Line Driver IC.   A Hi output  indicates the Transmitter data bit is a 1.  The signal returns to zero for
 second half of bit time.
 DO(B) Logic Output.  Transmitter serial data �B� output.  This is a  return-to-zero format signal which will normally feed an
 ARINC 429 Line Driver IC.   A Hi output  indicates the Transmitter data bit is a 0.  The signal returns to zero for
 second half of bit time.
 ENTX Logic Input.  Enable Transmitter.  A Hi input enables the Transmitter to send data from the Transmitter FIFO.  This
 must be Low while writing data into Transmitter FIFO.  Transmitter memory is cleared by high-to-low transition.
 /LDCW Logic Input. Load Control Register.  A Low input pulse loads the Control Register from D[15:0].
 1MCK Logic Input.  External Clock.  Master clock used by both the Receivers and Transmitter.  The 1MHz rate is a X10 clock
 for the HI data rate (100 kbps), and a X80 clockfor LO data rate (12.5 kbps).
 TXCK Logic Output.  Transmitter Clock.  This outputs a clock frequency equal to the transmit data rate.  The clock is always
 enabled and in phase with the data.  The output is Hi during the first half of the data bit time.
 /MR Logic Input.  Master Reset.  A Lo input resets the Transmitter FIFO, bit counters, word counter, gap timers, /DRx, and
 TXR.  The Control Register is not affected.  Used on power up and system reset.
 /DBCEN Logic Input with internal pull up to VDD.  Data Bit Control Enable.  A Low input enables the transmitter parity bit
 control function as defined by control register bit 4 (PAREN).  A Hi input forces transmitter parity bit insertion
 regardless of PAREN value.  The pin is normally left open or tied to ground
*/
#define ISR_TIMEOUT 1000
#define WORDSIZE 16
#define ARINC32_SIZE 32
#define ARINC25_SIZE 25

#define NUM_REC_CHANELS 2
#define NUM_SEND_CHANELS 1

#define CONTROL_INSTRUCTION_LEN 16

#define CHANELL0 0
#define CHANELL1 1
#define CHANELL2 2
#define CHANELL3 3

#define WORD1 0
#define WORD2 1

#define RECEIVE 0
#define SEND 1

#define SET   1
#define UNSET 0

#define DR1   21 // physical 40 // input
#define DR2   20 // physical 38 // input

#define TXR   15 // physical 10 // input

#define SEL   26 // physical 37 // output
#define OE1   16 // physical 36 // output
#define OE2   19 // physical 35 // output
#define LD1   17 // physical 11 // output
#define LD2   18 // physical 12 // output
#define ENTX  14 // physical 8  // output
#define LDCW  4  // physical 7  // output
#define MCK
#define TXCK
#define MR
#define DBCEN
// DATA
#define D0   22 // physical 15
#define D1   27 // physical 13
#define D2   23 // physical 16
#define D3   24 // physical 18
#define D4   10 // physical 19
#define D5   9  // physical 21
#define D6   25 // physical 22
#define D7   11 // physical 23 
#define D8   8  // physical 24
#define D9   7  // physical 26
#define D10  0  // physical 27
#define D11  1  // physical 28
#define D12  5  // physical 29
#define D13  6  // physical 31
#define D14  12 // physical 32
#define D15  13 // physical 33



/* control mode instructions
Functional Description:
The DEI 1016 supports a number of various options
which are selected by data written into the control
register. Data is written into the control register from the
16-bit data bus when the /LDCW signal is pulsed to a
logic “0”. The twelve control bits control the following
functions:
1) Word Length (32 or 25 bits)
2) Transmitter bit 32 (Parity or Data)
3) Wrap around self test.
4) Source Destination code checking of received data.
5) Transmitter parity (even or odd)
6) Transmitter and Receiver data rate (100 or 12.5 kbps)
BITSYMBOLBIT
SYMBOL
D15: (MSB)WLSEL, 
D7: X1
D14:  RCVSEL,  
D6: SDENB1
D13 TXSELD5/SLFTST
D12 PARCK
D4  PAREN
D11 Y2
D3  NOT USED
D10 X2
D2  NOT USED
D9  SDENB2
D1  NOT USED
D8  Y1
D0 NOT USED
Table 6: DEI1016 Control Word

PAREN/D4 : Transmitter Parity Enable. Enables parity bit insertion into transmitter data bit 32. Parity is always
inserted if /DBCEN is open or HI. If /DBCEN is LO, Logic “0” on PAREN inserts data on bit 32, and
Logic “1” on PAREN inserts parity on bit 32.

SLFTST/D5  : Self Test Enable. Logic “0” enables a “wrap around” test mode which internally connects the transmitter
outputs to both receiver inputs, bypassing the receiver front end. The test data is inverted before going
into receiver 2 so that its data is the complement of that received by receiver 1. The transmitter output is
active during test mode.

SDENB1 D6: S/D Code Check Enable for receiver 1. Logic “1” enables the Source/Destination Decoder for receiver 1.

X1, Y1 : S/D compare code RX1. If the receiver 1 S/D code check is enabled (SDENB1=1), then incoming
receiver data S/D fields will be compared to X1, Y1. If they match, the word will be accepted by receiver 2; if not, it will be ignored. X2 (D10) is compared to serial data bit 9, Y2 (D11) is compared to serial
data bit 10.

PARCK, D12 :Parity Check Enable. Logic “1” inverts the transmitter parity bit for test of parity circuits. Logic “0”
D12 selects normal odd parity; logic “1” selects even parity.

TXSEL3, D13 :Transmitter Data Rate Select. Logic “0” sets the transmitter to the HI data rate. HI rate is equal to the
clock rate divided 10. Logic “1” sets the transmitter to the LO data rate. LO rate is equal to the clock rate
divided by 80.

RCVSEL4, D14: Receiver Data Rate Select. Logic “0” sets both receivers to accept the HI data rate. The nominal HI data
rate is the input clock divided by 10. Logic “1” sets both receivers to the LO data rate. The nominal LO
data rate is the input clock divided by 80.

WLSEL5, D15 :Word Length Select. Logic “0” sets the transmitter and receivers to a 32 bit word format. Logic ”1” sets
them to a 25 bit word format.

NOT USED D0-D3 : When writing to the control register, the four “not used bits” are “don’t care” bits. These four bits will not
 be used on the chip.
*/


std::bitset<WORDSIZE> operator+ (std::bitset<WORDSIZE> b1, std::bitset<WORDSIZE> b2);


namespace CONTROL
{
using word_t = std::bitset<WORDSIZE>;

struct SDI_ENB1
{
    static void ENABLE(word_t& word){
        word[6] =  1;
    }
    static void DISABLE(word_t& word){
        word[6] = 0;
    }
};

struct SDI_X1
{
    static void ENABLE(word_t& word){
        word[7] =  1;
    }
    static void DISABLE(word_t& word){
        word[7] = 0;
    }
};

struct SDI_Y1
{
    static void ENABLE(word_t& word){
        word[8] =  1;
    }
    static void DISABLE(word_t& word){
        word[8] = 0;
    }
};

struct SDI_ENB2
{
    static void ENABLE(word_t& word){
        word[9] = 1;
    }
    static void DISABLE(word_t& word){
        word[9] = 0;
    }
};

struct SDI_X2
{
    static void ENABLE(word_t& word){
        word[10] =  1;
    }
    static void DISABLE(word_t& word){
        word[10] = 0;
    }
};

struct SDI_Y2
{
    static void ENABLE(word_t& word){
        word[11] =  1;
    }
    static void DISABLE(word_t& word){
        word[11] = 0;
    }
};


struct SELF_TEST
{
    static void ENABLE(word_t& word){
        word[5] =  0;
    }
    static void DISABLE(word_t& word){
        word[5] = 1;
    }
};

struct PARCK
{
    static void SET_ODD(word_t& word){
        word[12] = 0;
    }
    static void SET_EVEN(word_t& word){
        word[12] = 1;
    }
};

struct WORD_LENGTH
{
    static void SELECT_32(word_t& word){
        word[15]= 0;
    }
    static void SELECT_25(word_t& word){
        word[15]= 1;
    }
};

struct TRANSMITTER_DATA_RATE
{
    static void SELECT_HI(word_t& word){
        word[13]= 0;
    }
    static void SELECT_LOW(word_t& word){
        word[13]= 1;
    }
};

struct RECEIVER_DATA_RATE
{
    static void SELECT_HI(word_t& word){
        word[14]= 0;
    }
    static void SELECT_LOW(word_t& word){
        word[14]= 1;
    }
};

struct PAREN
{
    static void ENABLE(word_t& word){
        word[4]= 1;
    }
    static void DISABLE(word_t& word){
        word[4]= 0;
    }
};
}
