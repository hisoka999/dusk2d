#include "Inventory.h"
#include "services/ItemService.h"

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

void Inventory::addItem(const std::shared_ptr<Item> &item, int amount)
{
    ItemSlot *lastEmptySlot = nullptr;
    for (auto &slot : itemSlots)
    {

        if (slot.item && slot.item->getId() == item->getId() && isItemTypeStackable(item->getType()))
        {
            slot.amount += amount;
            if (slot.amount == 0)
            {
                slot.item = nullptr;
            }
            return;
        }
        else if (!slot.item && !lastEmptySlot)
        {
            lastEmptySlot = &slot;
        }
    }

    if (!lastEmptySlot)
    {
        for (auto &slot : hotbarSlots)
        {

            if (slot.item && slot.item->getId() == item->getId() && isItemTypeStackable(item->getType()))
            {
                slot.amount += amount;
                if (slot.amount == 0)
                {
                    slot.item = nullptr;
                }
                return;
            }
            else if (!slot.item && !lastEmptySlot)
            {
                lastEmptySlot = &slot;
            }
        }
    }

    if (lastEmptySlot)
    {
        lastEmptySlot->amount += amount;
        if (lastEmptySlot->amount != 0)
        {
            lastEmptySlot->item = item;
        }
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