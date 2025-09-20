#ifndef BASEITEM_H
#define BASEITEM_H

#include "types.h"
#include <QObject>


class BaseItem : QObject  {
public:
    enum class ItemType { Equipment, Label, Parameter };
    ItemType type;
    BaseItem *parent;
    virtual ~BaseItem();
    BaseItem(ItemType _type);
    BaseItem() = delete;
    str_t typeAsString() const;
};
#endif // BASEITEM_H
