#pragma once
#include <array>
#include <engine/core/ecs/Component.h>
#include "ItemRecipe.h"
#include "game/Item.h"

inline constexpr bool isItemTypeStackable(const ItemType itemType);

struct ItemSlot
{
    std::shared_ptr<Item> item = nullptr;
    size_t amount = 0;
    int slotId = 0;
    bool blockedForCrafting = false;
};

typedef std::array<ItemSlot, 25> ItemSlots;
typedef std::array<ItemSlot, 6> HotBarSlots;

enum class SlotTarget
{
    INVENTORY,
    HOTBAR,
    BOTH
};

class Inventory : public core::ecs::ScriptableEntity
{

private:
    ItemSlots itemSlots;
    HotBarSlots hotbarSlots;
    size_t selectedHotBarSlot = 0;

public:
    Inventory();
    ~Inventory();
    Inventory(Inventory &) = delete;
    Inventory(Inventory &&) = delete;
    Inventory &operator=(Inventory &other) = delete;

    void addItem(const std::shared_ptr<Item> &item, int amount);
    void addItem(const std::shared_ptr<Item> &item, SlotTarget slotTarget, int amount);
    void removeItemById(size_t itemId, int amount);
    ItemSlots &getItemSlots();
    HotBarSlots &getHotBarSlots();
    const HotBarSlots &getHotBarSlots() const;
    size_t countItemsById(size_t id);
    bool canCraftRecipe(const std::shared_ptr<ItemRecipe> &recipe);
    void craftItem(const std::shared_ptr<ItemRecipe> &recipe);
    void craftItem(const std::shared_ptr<ItemRecipe> &recipe, SlotTarget slotTarget);
    void setItemBySlot(ItemSlot slotData);
    void setSelectedHotbarSlot(size_t slot);
    void setSelectedHotbarSlotById(int slotId);
    ItemSlot &getSelectedHotbarSlot();
};
