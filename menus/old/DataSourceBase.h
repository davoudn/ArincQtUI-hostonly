#pragma once
#include <functional>
#include <vector>
#include <bitset>
#include <cstdint>
#include <QThread>
#include "types.h"#include <QThread>


class Handler;

class DataSourceBase : public QThread{
public:
	DataSourceBase() = default;
	~DataSourceBase() = default;
	virtual void HandleReceivedData(uint8_t chanel) = 0;
	virtual bool SendData(uint8_t chanel) = 0;
	virtual void SetupDevice()  = 0;
	virtual void ReceiveDataCallback(int gpio, int level, unsigned int tick) = 0 ;
    virtual void SetControlInstruction(uint8_t instruction) = 0;
	//

	// inline functions
	template<typename T>
	void SetSendBuff(uint8_t chanel, T sendbuff) {
		SendArincBuffer[chanel] = std::bitset<ARINC32_SIZE>(sendbuff);
	}


	inline const std::bitset<ARINC32_SIZE>& GetReceiveBuffer(int chanel) const {
		return RecArincBuffer[chanel];
	}

	inline const std::bitset<ARINC32_SIZE>& GetSendBuffer(int chanel) const {
		return SendArincBuffer[chanel];
	}
protected:
//
	std::vector<std::bitset<ARINC32_SIZE>> RecArincBuffer;
	std::vector<std::bitset<ARINC32_SIZE>> SendArincBuffer;
//
};



