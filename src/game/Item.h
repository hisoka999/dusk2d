#pragma once

#include "ItemType.h"
#include <string>

class Item
{
private:
    size_t m_id;
    ItemType m_itemType;
    std::string m_name;
    std::string m_subTextureName;
    std::string m_prefab;

public:
    Item(size_t id,
         ItemType itemType,
         std::string name,
         std::string subTextureName,
         std::string prefab);
    ~Item();

    size_t getId();
    ItemType getType();
    std::string &getName();
    std::string &getSubTextureName();
    std::string &getPrefab();

    bool operator==(const Item &other);
};