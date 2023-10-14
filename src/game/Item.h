#pragma once

#include "ItemType.h"
#include <string>
#include <map>
#include "game/actions/ItemAction.h"

class Item
{
private:
    size_t m_id;
    ItemType m_itemType;
    std::string m_name;
    std::string m_subTextureName;
    std::string m_prefab;
    std::map<std::string, std::string> m_properties;

public:
    Item(size_t id,
         ItemType itemType,
         std::string name,
         std::string subTextureName,
         std::string prefab);

    Item(size_t id,
         ItemType itemType,
         std::string name,
         std::string subTextureName,
         std::string prefab,
         std::map<std::string, std::string> &data);
    ~Item();

    std::string getProperty(const std::string &property);
    const std::string getProperty(const std::string &property) const;

    size_t getId();
    ItemType getType();
    std::string &getName();
    std::string &getSubTextureName();
    std::string &getPrefab();

    bool operator==(const Item &other);
};