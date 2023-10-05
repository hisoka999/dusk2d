#pragma once

#include "Attribute.h"
#include <cstddef>
#include <engine/core/Timer.h>

class Character
{
private:
    Attribute hunger;
    Attribute thirst;
    Attribute hp;
    size_t level;
    core::Timer timer;
    // todo add item slots and more

    void update();

public:
    Character(/* args */);
    ~Character();
    void updateAttributes(size_t delta);

    Attribute &getHunger();
    Attribute &getThirst();
    Attribute &getHp();
    size_t getLevel();
};
