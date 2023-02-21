#include "Inventory.h"

inline constexpr bool isItemTypeStackable(const ItemType itemType)
{
    return itemType != ItemType::NONE;
}

Inventory::Inventory(/* args */)
{
}

Inventory::~Inventory()
{
}

void Inventory::addItem(ItemType type, int amount)
{
    ItemSlot *lastEmptySlot = nullptr;
    for (auto &slot : itemSlots)
    {
        if (slot.itemType == type && isItemTypeStackable(type))
        {
            slot.amount += amount;
            if (slot.amount == 0)
            {
                slot.itemType = ItemType::NONE;
            }
            return;
        }
        else if (slot.itemType == ItemType::NONE && !lastEmptySlot)
        {
            lastEmptySlot = &slot;
        }
    }
    if (lastEmptySlot)
    {
        lastEmptySlot->amount += amount;
        if (lastEmptySlot->amount != 0)
        {
            lastEmptySlot->itemType = type;
        }
    }
}

ItemSlots &Inventory::getItemSlots()
{
    return itemSlots;
}
