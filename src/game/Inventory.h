#pragma once
#include <engine/core/ecs/Component.h>
#include <array>
#include "ItemType.h"
inline constexpr bool isItemTypeStackable(const ItemType itemType);

struct ItemSlot
{
    ItemType itemType = ItemType::NONE;
    size_t amount = 0;
};

typedef std::array<ItemSlot, 25> ItemSlots;

class Inventory : public core::ecs::ScriptableEntity
{

private:
    ItemSlots itemSlots;

public:
    Inventory(/* args */);
    ~Inventory();
    void addItem(ItemType type, int amount);
    ItemSlots &getItemSlots();
};