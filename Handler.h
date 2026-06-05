#include "types.h"

#include <bitset>

class DataSourceBase;
class Handler {
public:
    Handler()  = default;
    ~Handler() = default;

    virtual void update(std::bitset<ARINC32_SIZE>&& _ArincBitData) = 0;
    virtual void update(std::bitset<ARINC32_SIZE>& _ArincBitData) = 0;
    void setDataSource (DataSourceBase* dataSourceBase);  
    protected: 
    DataSourceBase* dataSource = nullptr;
};
