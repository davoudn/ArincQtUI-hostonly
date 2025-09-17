#include "baseitem.h"


BaseItem::BaseItem(ItemType _type)
    : type(_type), parent(nullptr)
{
}

str_t BaseItem::typeAsString() const {
    switch(type) {
    case ItemType::Equipment: return "Equipment";
    case ItemType::Label: return "Label";
    case ItemType::Parameter: return "Parameter";
    }

    return "Unknown";
}
