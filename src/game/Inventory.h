#pragma once
#include <engine/core/ecs/Component.h>
#include <array>
#include "game/Item.h"
#include "ItemRecipe.h"

inline constexpr bool isItemTypeStackable(const ItemType itemType);

struct ItemSlot
{
    std::shared_ptr<Item> item = nullptr;
    size_t amount = 0;
    int slotId = 0;
};

typedef std::array<ItemSlot, 25> ItemSlots;
typedef std::array<ItemSlot, 6> HotBarSlots;

class Inventory : public core::ecs::ScriptableEntity
{

private:
    ItemSlots itemSlots;
    HotBarSlots hotbarSlots;
    size_t selectedHotBarSlot = 0;

public:
    Inventory();
    ~Inventory();
    void addItem(const std::shared_ptr<Item> &item, int amount);
    void removeItemById(size_t itemId, int amount);
    ItemSlots &getItemSlots();
    HotBarSlots &getHotBarSlots();
    const HotBarSlots &getHotBarSlots() const;
    size_t countItemsById(size_t id);
    bool canCraftRecipe(const std::shared_ptr<ItemRecipe> &recipe);
    void craftItem(const std::shared_ptr<ItemRecipe> &recipe);
    void setItemBySlot(ItemSlot slotData);
    void setSelectedHotbarSlot(size_t slot);
    void setSelectedHotbarSlotById(int slotId);
    ItemSlot &getSelectedHotbarSlot();
};