#ifndef BASEITEM_H
#define BASEITEM_H

#include "types.h"
#include <QObject>


class BaseItem : public QObject {
public:
    enum class ItemType { Equipment, Label, Parameter };
    ItemType type;
    BaseItem *parent;

    BaseItem(ItemType _type);

    str_t typeAsString() const;
};
#endif // BASEITEM_H
