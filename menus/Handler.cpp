#include "Handler.h"
#include "DataSourceBase.h"

void Handler::SetDataSource(DataSourceBase* dataSource)
{
    if (dataSource) {
     DataSource = dataSource;
     //DataSource->SetDataHanler(this);
    }
}
