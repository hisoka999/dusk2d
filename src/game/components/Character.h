#pragma once

#include <cstddef>
#include <engine/core/Timer.h>
#include "Attribute.h"
#include "Stat.h"
#include "game/Item.h"


struct EquipmentSlot
{
    std::shared_ptr<Item> item = nullptr;
    EquipmentType type = EquipmentType::None;
};
typedef std::array<EquipmentSlot, 6> EquipmentSlotList;
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

    void update();

public:
    Character(/* args */);
    ~Character();
    void updateAttributes(size_t delta);

    Stat &getHunger();
    Stat &getThirst();
    Stat &getHp();
    size_t getLevel();
    Attribute &getAttack();
    Attribute &getDefence();
    EquipmentSlotList &getEquipment();

    std::vector<std::pair<std::string, std::string>> displayAttributes();
};
