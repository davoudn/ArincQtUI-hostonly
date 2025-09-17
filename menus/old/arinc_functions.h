#include <utility>
#include <array>
#include <cmath>
#include <cstdint>

#include "types.h"




class  DArincData;

namespace ArincFunctions {

template <typename T, typename U>
class CALCULATOR {
    static T Value(const uint8_t& bi, const uint8_t& bf, const DArincData& Data);
};


class IEEEFLOT{
public:
    template <typename T>
    static T Value(const uint8_t& bi, const uint8_t& exponent, const uint8_t& mantisa,const DArincData& Data);

    template <typename T>
    static void SetValue(const T& t, const uint8_t& bi, const uint8_t& exponent, const uint8_t& mantisa, DArincData& Data);
};

class INT{
    public:
        template <typename T>
        static T Value(const uint8_t& bi, const uint8_t& bf, const DArincData& Data);

        template <typename T>
        static void SetValue(const T& t ,const uint8_t& bi, const uint8_t& bf, DArincData& Data);
};

class BCD {
    public:

        template <typename T>
        static T Value(const uint8_t& sigdigits, const DArincData& Data);

        template <typename T>
        static void SetValue(const T& t ,const uint8_t& sigdigits,  DArincData& Data);

        template <typename T>
        static void SetValue(const T& t ,const uint8_t& bi, const uint8_t& bf,  DArincData& Data);

        static double getMax(uint32_t sigbits);
};

class FLOATBCD {
public:

    template <typename T>
    static T Value(const uint8_t& sigdigits, const DArincData& Data);

    template <typename T>
    static void SetValue(const T& t ,const uint8_t& sigdigits,  DArincData& Data);

    template <typename T>
    static void SetValue(const T& t ,const uint8_t& bi, const uint8_t& bf,  DArincData& Data);

    static double getMax(uint32_t sigbits);
};

class FLNOBCD {
public:
    const static uint8_t sigdigits = 4;
    template <typename T>
    static T Value(const DArincData& Data);

    template <typename T>
    static void SetValue(const T& t,  DArincData& Data);

};

class ATCBCD {
public:
    const static uint8_t sigdigits = 4;
    template <typename T>
    static T Value(const DArincData& Data);

    template <typename T>
    static void SetValue(const T& t,  DArincData& Data);

};

class BNR { 
    public:
        template <typename T>
        static T Value(const uint8_t& bi, const uint8_t& bf, const DArincData& Data);

        template <typename T>
        static void SetValue(const T& t ,const uint8_t& bi, const uint8_t& bf,  DArincData& Data);
};

class BITS {
    public:
        template <typename T>
        static T Value(const uint8_t& bi, const uint8_t& bf, const DArincData& Data);

        template <typename T>
        static void SetValue(const T& t ,const uint8_t& bi, const uint8_t& bf,  DArincData& Data);

        template <typename T>
        static void SetValue(const value_t& value, const uint8_t& bi, DArincData& Data);

};

class HEX {
    public:
        template <typename T>
        static T Value(const uint8_t& bi, const uint8_t& bf, const DArincData& Data);

        template <typename T>
        static void SetValue(const T& t ,const uint8_t& bi, const uint8_t& bf,  DArincData& Data);

};


} // end of namespace

