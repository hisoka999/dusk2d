#pragma once

#include <map>
#include <string>
#include "ItemType.h"
#include "game/actions/ItemAction.h"

enum class ItemProperty
{
    fuel,
    hunger,
    thirst,
    damage
};

class Item
{
private:
    size_t m_id;
    ItemType m_itemType;
    ItemSubType m_itemSubType;
    EquipmentType m_equipmentType;
    std::string m_name;
    std::string m_subTextureName;
    std::string m_prefab;
    std::map<ItemProperty, std::string> m_properties;

public:
    Item(size_t id, ItemType itemType, ItemSubType itemSubType, EquipmentType equipmentType, std::string name,
         std::string subTextureName, std::string prefab);

    Item(size_t id, ItemType itemType, ItemSubType itemSubType, EquipmentType equipmentType, std::string name,
         std::string subTextureName, std::string prefab, std::map<ItemProperty, std::string> &data);
    ~Item();

    std::string getProperty(const ItemProperty property);
    const std::string getProperty(const ItemProperty property) const;
    bool hasProperty(const ItemProperty property) const;

    size_t getId();
    ItemType getType();
    ItemSubType getItemSubType();
    EquipmentType getEquipmentType();
    std::string &getName();
    std::string &getSubTextureName();
    std::string &getPrefab();

    bool operator==(const Item &other);
};
