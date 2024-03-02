#include "Character.h"
#include <string>
#include "game/messages.h"

Character::Character() :
    hunger(100, 100), thirst(100, 100), hp(20, 20), level(1), timer(2000, -1), attack(10), defence(5)
{
    update();
    timer.setCallback(
            [this]([[maybe_unused]] uint32_t execs)
            {
                hunger.addValue(-1);
                thirst.addValue(-1);
                update();
            });
    timer.start();
    equipment[0].type = EquipmentType::Helmet;
    equipment[1].type = EquipmentType::Weapon;
    equipment[2].type = EquipmentType::Weapon;
    equipment[3].type = EquipmentType::Armor;
    equipment[4].type = EquipmentType::Pants;
    equipment[5].type = EquipmentType::Boots;


    for (size_t index = 0; index < equipment.size(); ++index)
    {
        equipment[index].slotId = index;
        attack.addModifier(
                [this, index]() -> int
                {
                    auto equip = equipment[index];
                    if (equip.item == nullptr)
                        return 0;
                    if (equip.item->hasProperty(ItemProperty::damage))
                    {
                        return std::stoi(equip.item->getProperty(ItemProperty::damage));
                    }
                    return 0;
                });
    }
}

Character::~Character() {}

Stat &Character::getHp() { return hp; }

Stat &Character::getHunger() { return hunger; }

Stat &Character::getThirst() { return thirst; }

size_t Character::getLevel() { return level; }

Attribute &Character::getAttack() { return attack; }

Attribute &Character::getDefence() { return defence; }

void Character::update() { core::MessageSystem<MessageType>::get().sendMessage(MessageType::CHARACTER_UPDATED, this); }

void Character::updateAttributes([[maybe_unused]] size_t delta) { timer.update(); }


EquipmentSlotList &Character::getEquipment() { return equipment; }

void Character::updateEquipment(const EquipmentSlot &slot) { equipment[slot.slotId] = slot; }

std::vector<std::pair<std::string, std::string>> Character::displayAttributes()
{
    std::vector<std::pair<std::string, std::string>> attributes;
    attributes.push_back(std::make_pair("Level", std::to_string(level)));
    attributes.push_back(std::make_pair("HP", utils::string_format("%i/%i", hp.getValue(), hp.getMaxValue())));
    attributes.push_back(
            std::make_pair("Hunger", utils::string_format("%i/%i", hunger.getValue(), hunger.getMaxValue())));
    attributes.push_back(
            std::make_pair("Thirst", utils::string_format("%i/%i", thirst.getValue(), thirst.getMaxValue())));
    attributes.push_back(std::make_pair("Attack", std::to_string(attack.getValue())));
    attributes.push_back(std::make_pair("Defence", std::to_string(defence.getValue())));
    return attributes;
}
