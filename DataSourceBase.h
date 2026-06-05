#pragma once
#include <functional>
#include <vector>
#include <bitset>
#include <cstdint>

#include "types.h"

class Handler;

class DataSourceBase {
public:
	DataSourceBase() = default;
	~DataSourceBase() = default;
	virtual void handleReceivedData(uint8_t chanel) = 0;
	virtual bool sendData(uint8_t chanel) = 0;
	virtual void receiveDataCallback(int gpio, int level, unsigned int tick) = 0 ;
	//
	
	// inline functions
	template<typename T>
	void setSendBuff(uint8_t chanel, T sendbuff) {
		SendArincBuffer[chanel] = std::bitset<ARINC32_SIZE>(sendbuff);
	}
	
	
	inline const std::bitset<ARINC32_SIZE>& getReceiveBuffer(int chanel) const {
		return RecArincBuffer[chanel];
	}
	
	inline const std::bitset<ARINC32_SIZE>& getSendBuffer(int chanel) const {
		return SendArincBuffer[chanel];
	}
protected:
	//
	std::vector<std::bitset<ARINC32_SIZE>> RecArincBuffer;
	std::vector<std::bitset<ARINC32_SIZE>> SendArincBuffer;
	//
};



