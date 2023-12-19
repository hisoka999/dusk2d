#pragma once
#include <cstdint>
enum class ItemType : uint8_t
{
    WOOD,
    STONE,
    EQUIPMENT,
    PLACEABLE,
    FOOD,
    NONE
};

enum class ItemSubType : uint8_t
{
    ITEM,
    AXE,
    PICK_AXE,
    FUEL
};