#include "Inventory.h"
#include "services/ItemService.h"
#include "game/messages.h"

inline constexpr bool isItemTypeStackable(const ItemType itemType)
{
    return itemType != ItemType::NONE;
}

Inventory::Inventory(/* args */)
{
    int slotId = 0;
    for (auto &slot : itemSlots)
    {
        slot.slotId = slotId;
        slotId++;
    }
    slotId = -1;
    for (auto &slot : hotbarSlots)
    {
        slot.slotId = slotId;
        slotId--;
    }
}

Inventory::~Inventory()
{
}

void Inventory::addItem(const std::shared_ptr<Item> &item, int amount)
{
    ItemSlot *lastEmptySlot = nullptr;
    ItemSlot *lastEmptyHotbarSlot = nullptr;
    auto &msgSystem = core::MessageSystem<MessageType>::get();

    for (auto &slot : itemSlots)
    {

        if (slot.item && slot.item->getId() == item->getId() && isItemTypeStackable(item->getType()))
        {
            slot.amount += amount;
            if (slot.amount == 0)
            {
                slot.item = nullptr;
            }
            msgSystem.sendMessage<ItemSlot *>(MessageType::INVENTORY_UPDATED, &slot);
            return;
        }
        else if (!slot.item && !lastEmptySlot)
        {
            lastEmptySlot = &slot;
        }
    }

    for (auto &slot : hotbarSlots)
    {

        if (slot.item && slot.item->getId() == item->getId() && isItemTypeStackable(item->getType()))
        {
            slot.amount += amount;
            if (slot.amount == 0)
            {
                slot.item = nullptr;
            }
            msgSystem.sendMessage<ItemSlot *>(MessageType::INVENTORY_UPDATED, &slot);
            return;
        }
        else if (!slot.item && !lastEmptyHotbarSlot)
        {
            lastEmptyHotbarSlot = &slot;
        }
    }
    if (lastEmptyHotbarSlot)
    {
        lastEmptyHotbarSlot->amount += amount;
        if (lastEmptyHotbarSlot->amount != 0)
        {
            lastEmptyHotbarSlot->item = item;
        }
        msgSystem.sendMessage<ItemSlot *>(MessageType::INVENTORY_UPDATED, lastEmptyHotbarSlot);
    }
    else if (lastEmptySlot)
    {
        lastEmptySlot->amount += amount;
        if (lastEmptySlot->amount != 0)
        {
            lastEmptySlot->item = item;
        }
        msgSystem.sendMessage<ItemSlot *>(MessageType::INVENTORY_UPDATED, lastEmptySlot);
    }
}

void Inventory::removeItemById(size_t itemId, int amount)
{
    size_t remainingAmount = amount;
    for (auto &slot : itemSlots)
    {
        if (slot.item && slot.item->getId() == itemId)
        {
            if (slot.amount >= remainingAmount)
            {
                slot.amount -= amount;
                remainingAmount = 0;
            }
            else
            {
                remainingAmount -= slot.amount;
                slot.amount = 0;
            }

            if (slot.amount == 0)
            {
                slot.item = nullptr;
            }
            auto &msgSystem = core::MessageSystem<MessageType>::get();
            msgSystem.sendMessage<ItemSlot *>(MessageType::INVENTORY_UPDATED, &slot);
        }

        if (remainingAmount == 0)
            break;
    }

    if (remainingAmount > 0)
    {
        for (auto &slot : hotbarSlots)
        {
            if (slot.item && slot.item->getId() == itemId)
            {
                if (slot.amount >= remainingAmount)
                {
                    slot.amount -= amount;
                    remainingAmount = 0;
                }
                else
                {
                    remainingAmount -= slot.amount;
                    slot.amount = 0;
                }

                if (slot.amount == 0)
                {
                    slot.item = nullptr;
                }
                auto &msgSystem = core::MessageSystem<MessageType>::get();
                msgSystem.sendMessage<ItemSlot *>(MessageType::INVENTORY_UPDATED, &slot);
            }

            if (remainingAmount == 0)
                break;
        }
    }
}

size_t Inventory::countItemsById(size_t id)
{
    size_t count = 0;
    for (auto &slot : itemSlots)
    {
        if (slot.item && slot.item->getId() == id)
            count += slot.amount;
    }

    for (auto &slot : hotbarSlots)
    {
        if (slot.item && slot.item->getId() == id)
            count += slot.amount;
    }
    return count;
}

bool Inventory::canCraftRecipe(const std::shared_ptr<ItemRecipe> &recipe)
{
    bool canCraft = true;

    for (const auto &[itemId, amount] : recipe->getInput())
    {
        if (countItemsById(itemId) < amount)
        {
            canCraft = false;
            break;
        }
    }
    return canCraft;
}

ItemSlots &Inventory::getItemSlots()
{
    return itemSlots;
}

HotBarSlots &Inventory::getHotBarSlots()
{
    return hotbarSlots;
}
const HotBarSlots &Inventory::getHotBarSlots() const
{
    return hotbarSlots;
}

void Inventory::craftItem(const std::shared_ptr<ItemRecipe> &recipe)
{
    if (!canCraftRecipe(recipe))
        return;
    for (const auto &[itemId, amount] : recipe->getInput())
    {
        removeItemById(itemId, amount);
    }

    auto item = services::ItemService::Instance().getItemById(recipe->getOutputId());

    addItem(item, recipe->getAmount());
}

void Inventory::setItemBySlot(ItemSlot slotData)
{
    if (slotData.slotId >= 0)
    {
        itemSlots[slotData.slotId] = slotData;
    }
    else
    {
        hotbarSlots[(slotData.slotId * -1) - 1] = slotData;
    }
}
void Inventory::setSelectedHotbarSlot(size_t slot)
{
    if (slot < hotbarSlots.size())
        selectedHotBarSlot = slot;
}

void Inventory::setSelectedHotbarSlotById(int slotId)
{
    for (size_t i = 0; i < hotbarSlots.size(); ++i)
    {
        if (hotbarSlots[i].slotId == slotId)
        {
            selectedHotBarSlot = i;
            break;
        }
    }
}

ItemSlot &Inventory::getSelectedHotbarSlot()
{
    return hotbarSlots[selectedHotBarSlot];
}