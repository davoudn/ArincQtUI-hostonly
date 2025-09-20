#include "types.h"

#include <bitset>

class DataSourceBase;
class Handler {
public:
    Handler()  = default;
    ~Handler() = default;

    virtual void Update(std::bitset<ARINC32_SIZE>&& _ArincBitData) = 0;
    virtual void Update(std::bitset<ARINC32_SIZE>& _ArincBitData) = 0;
    void SetDataSource (DataSourceBase* dataSourceBase);  
    protected: 
    DataSourceBase* DataSource = nullptr;
};
