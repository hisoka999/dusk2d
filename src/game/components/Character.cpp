#include "Character.h"
#include "game/messages.h"
Character::Character() : hunger(100, 100), thirst(100, 100), hp(20, 20), level(1), timer(2000, -1)
{
    update();
    timer.setCallback([this]()
                      {
            hunger.addValue(-1);
    thirst.addValue(-1);
    update(); });
    timer.start();
}

Character::~Character()
{
}

Attribute &Character::getHp()
{
    return hp;
}

Attribute &Character::getHunger()
{
    return hunger;
}

Attribute &Character::getThirst()
{
    return thirst;
}

size_t Character::getLevel()
{
    return level;
}

void Character::update()
{
    core::MessageSystem<MessageType>::get().sendMessage(MessageType::CHARACTER_UPDATED, this);
}

void Character::updateAttributes([[maybe_unused]] size_t delta)
{
    timer.update();
}