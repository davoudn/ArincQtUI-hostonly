#ifndef UTILS_H
#define UTILS_H
#include "types.h"
#include <fstream>
#include <vector>
#include <math.h>
namespace utils
{

str_t ReadFile(str_t qfilename);
jsonobj_t GetJsonObj(str_t);

template <typename T>
double sign(T t){
    if (t==T(0)){
        return 1.0;
    }

    return  t/std::abs(t);
}

std::vector<uint8_t> getDecimals(uint32_t value, uint8_t sidigits);

class MyBitset {
public:
    MyBitset() = default;

    template <typename T>
    MyBitset init(T t, uint8_t _length);
    inline uint32_t size() const {
        return data.size();
    }
    uint8_t operator[](uint8_t i){
        return data[i];
    }
    bool ifInitialized(){

    }
    void invert(){
        std::vector<uint8_t> _data;
        for (int i=data.size()-1; i>=0; i--){
            _data.push_back(data[i]);
        }
        //data.erase(data.begin(),data.end());
        data.clear();
        data = _data;
    }
private:
    std::vector<uint8_t> data;
    bool bIfInitialized = false;
};
}

#endif // UTILS_H
