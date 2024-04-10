#pragma once
#include <cstdint>
enum class ItemType : uint8_t
{
    WOOD,
    STONE,
    EQUIPMENT,
    PLACEABLE,
    FOOD,
    OTHER,
    NONE
};

enum class ItemSubType : uint8_t
{
    ITEM,
    AXE,
    PICK_AXE,
    FUEL
};

enum class EquipmentType
{
    None,
    Weapon,
    Armor,
    Boots,
    Pants,
    Helmet
};
