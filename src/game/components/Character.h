#pragma once

#include <array>
#include <cstddef>
#include <engine/core/Timer.h>
#include "Attribute.h"
#include "Stat.h"
#include "game/Item.h"


struct EquipmentSlot
{
    std::shared_ptr<Item> item = nullptr;
    EquipmentType type = EquipmentType::None;
    size_t slotId = 0;
};
using EquipmentSlotList = std::array<EquipmentSlot, 6>;
class Character
{
private:
    Stat hunger;
    Stat thirst;
    Stat hp;
    size_t level;
    core::Timer timer;
    EquipmentSlotList equipment;
    // todo add item slots and more
    Attribute attack;
    Attribute defence;

public:
    Character(/* args */);
    ~Character();
    void updateAttributes(size_t delta);
    void update();

    Character(Character &) = delete;
    Character(Character &&) = delete;
    Character &operator=(Character &other) = delete;

    Stat &getHunger();
    Stat &getThirst();
    Stat &getHp();
    size_t getLevel();
    Attribute &getAttack();
    Attribute &getDefence();
    EquipmentSlotList &getEquipment();
    void updateEquipment(const EquipmentSlot &slot);
    bool doAttack(Character &other);

    std::vector<std::pair<std::string, std::string>> displayAttributes();
};
